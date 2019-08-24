#include "WgiRubber.h"

WgiRubberSeqList WgiRubber::priv_seq_list;
WgiDevice* WgiRubber::priv_device = NULL;
WgiVirtualLoc WgiRubber::priv_last_pick = WgiInvVirtualLoc;

int
WgiRubber::addSequence (WgiRubberSeq* seq)
{
    priv_seq_list.put(seq);
    return priv_seq_list.length() - 1;
}

void
WgiRubber::removeSequence (WgiRubberSeq* seq)
{
    WgiRubberSeqListIter iter (priv_seq_list);
    if (iter.find_next (seq))
	iter.remove_next ();
}

int
WgiRubber::seqIndex (const WgiRubberSeq* seq)
{
    int i = 0;
    WGI_SEQ_LOOP (priv_seq_list, seq2)
    {
	if ((const WgiRubberSeq*) seq2 == seq) return i;
	i++;
    }
    return -1;
}

void
WgiRubber::picked (const WgiVirtualLoc& p)
{
    priv_last_pick = p;
    WgiRubberSeq* curr = WgiRubberSeq::currSequence();
    if (curr) curr->picked (p);
}

Boolean
WgiRubber::active ()
{
    return Boolean(WgiRubberSeq::currSequence() != NULL);
}

void
WgiRubber::move (const WgiVirtualLoc& loc, WgiDevice* device)
{
    if (active())
	WgiRubberSeq::currSequence ()->move (loc, device);
}

void 
WgiRubber::done ()
{
    WgiRubberSeq* curr = WgiRubberSeq::currSequence();
    if (curr == NULL) return;
    curr->done();
}

//=========================================================

WgiRubberSeq* WgiRubberSeq::priv_curr_seq = NULL;

void
WgiRubberSeq::reset ()
{
    priv_curr_man_id = -1;
    priv_curr_man = NULL;
    priv_pick_count = -1;
    priv_last_pick = WgiInvVirtualLoc;
}

WgiRubberSeq::WgiRubberSeq (WgiPickFunc pick_func /*= NULL */, 
		WgiRubberSeqInitFunc init_func /*= NULL */)
:	priv_pick_func (pick_func), priv_init_func(init_func)
{
    reset();
    priv_id = WgiRubber::addSequence (this);
}

void
WgiRubberSeq::addManager (WgiRubberManager* man, int index /*= -1 */)
{
    WgiRubberManagerListIter iter (priv_man_list);
    if (index == -1)
	iter.end_reset ();
    else
	iter.reset (index);

    iter.insert_next (man);
}

void
WgiRubberSeq::removeManager (WgiRubberManager* man)
{
    WgiRubberManagerListIter iter (priv_man_list);
    if (iter.find_next (man))
	iter.remove_next ();
}

int
WgiRubberSeq::managerIndex (const WgiRubberManager* man) const
{
    int i = 0;
    WGI_MANAGER_LOOP (this, man2)
    {
	if ((const WgiRubberManager*) man2 == man) return i;
	i++;
    }
    return -1;
}

void
WgiRubberSeq::start ()
{
    WgiRubber::done();

    reset ();

    callInitFunc ();   // initialize

    priv_curr_seq = this;
}

void
WgiRubberSeq::move (const WgiVirtualLoc& loc, WgiDevice* device)
{
    if (!active()) return;

    if (priv_curr_man)
	priv_curr_man->move(loc, device);
}

WgiRubberSeq* 
WgiRubberSeq::currSequence ()
{
    return priv_curr_seq;
}

const WgiVirtualLoc& 
WgiRubberSeq::prevPick ()
{
    return manager(currManagerId()-1)->lastPick();
}

void
WgiRubberSeq::picked (const WgiVirtualLoc& p)
{
    priv_last_pick = p;
    if (priv_curr_man) priv_curr_man->picked (p);
    priv_pick_count++;
    callPickFunc ();
}

WgiRubberSeq::~WgiRubberSeq ()
{
    if (active()) done();

    WGI_MANAGER_LOOP (this, man)
    {
	delete man;
    }

    WgiRubber::removeSequence (this);
}

void
WgiRubberSeq::done ()
{
    if (!active()) return;

    if (priv_curr_man) priv_curr_man->done();
    reset ();
    priv_curr_seq = NULL;
// ??
}

void
WgiRubberSeq::cont ()
{
    if (priv_curr_man)
    {
	priv_curr_man->start();
    }
}

void
WgiRubberSeq::next_manager ()
{
    if (active())
    {
	if (priv_curr_man) priv_curr_man->done();
    }

    priv_curr_man_id++;

    if (priv_curr_man_id < managerCardinality())
    {
	priv_curr_man = manager(priv_curr_man_id);
	cont ();
    }
    else
    {
	done ();
    }
}

void
WgiRubberSeq::prev_manager ()
{
    if (active())
    {
	if (priv_curr_man) priv_curr_man->done();
    }

    priv_curr_man_id--;

    if (priv_curr_man_id >= 0)
    {
	priv_curr_man = manager(priv_curr_man_id);
	cont ();
    }
    else
    {
	done ();
    }
}

//=========================================================

WgiRubberManager* WgiRubberManager::priv_curr_man = NULL;

WgiRubberManager::WgiRubberManager (WgiRubberSeq* seq)
:	priv_seq (seq)
{
    priv_manager.Number = 0;
    priv_manager.DoList = NULL;
    if (seq) seq->addManager(this);
}

int 
WgiRubberManager::bandIndex (const WgiRubberBand* band)
{
    int i = 0;
    WGI_BAND_LOOP (this, band2)
    {
	if ((const WgiRubberBand*) band2 == band) return i;
	i++;
    }
    return -1;
}

void
WgiRubberManager::addBand (WgiRubberBand* band)
{
// ?? what if active

    priv_band_list.put(band);
}

void
WgiRubberManager::removeBand (WgiRubberBand* band)
{
    if (active())
    {
	done ();
    }

    WgiRubberBandListIter iter (priv_band_list);
    if (iter.find_next (band))
	iter.remove_next ();
}

void
WgiRubberManager::start ()
{
    if (priv_curr_man)
    {
	priv_curr_man->done ();
    }

    initList ();

    priv_curr_man = this;
    wgi_rubber_init (manager());
}

void
WgiRubberManager::picked (const WgiVirtualLoc& p)
{
    priv_last_pick = p;
}

void
WgiRubberManager::move (const WgiVirtualLoc& loc, WgiDevice* device)
{
    if (!active()) return;

    WgiViewPortId vp;
    WLDcrd world_loc = loc.worldLoc (device, &vp);
    move (vp, world_loc);
}

void
WgiRubberManager::move (WgiViewPortId vp, const WgiWorldLoc& world_loc)
{
    wgi_rubber_move (manager(), vp, (WLDcrd*) &world_loc);
}

void
WgiRubberManager::initList ()
{
    if (manager()->DoList)
    {
	delete manager()->DoList;
	manager()->DoList = NULL;
    }

    setListCount (bandCardinality());

    if (bandCardinality() > 0)
	manager()->DoList = new wgiRubberStruct_t [bandCardinality()];

    for (int i = 0; i < bandCardinality(); i++)
    {
	manager()->DoList[i] = band(i)->priv_setting_s;
	band(i)->priv_setting = & (manager()->DoList[i]);
    }
}

WgiRubberManager::~WgiRubberManager ()
{
    if (priv_seq && priv_seq->active())
	priv_seq->done();

    if (manager()->DoList) delete manager()->DoList;

    WGI_BAND_LOOP (this, band)
    {
	delete band;
    }

    if (priv_seq) priv_seq->removeManager (this);
}

void
WgiRubberManager::done ()
{
    if (!active()) return;

    wgi_rubber_done (manager());
    priv_curr_man = NULL;

	// copy data back from list to private data
	// disconnect pointer

    for (int i = 0; i < bandCardinality(); i++)
    {
	band(i)->priv_setting_s = manager()->DoList[i];
	band(i)->priv_setting = & (band(i)->priv_setting_s);
    }
}

void
WgiRubberBand::init_outputs ()
{
    switch (TypeOf())
    {
	case wgiRubberTypeOfLarc45:
	case wgiRubberTypeOfLarc45Rot:
	{
	    priv_setting->Larc45.Output[0] = NULL;
	    priv_setting->Larc45.Output[1] = NULL;
	    priv_setting->Larc45.Output[2] = NULL;
	    priv_setting->Larc45.Radius = 0.0;
	    priv_setting->Larc45.StartA = 0.0;
	    priv_setting->Larc45.SweepA = 0.0;
	}
	break;

	case wgiRubberTypeOfMitred:
	{
	    priv_setting->Mitred.Changed = 0;
	    priv_setting->Mitred.PrevOct = WGI_RUBBER_LOCKIN_NOT;
	}
	break;

	case wgiRubberTypeOfBend90:
	{
	    priv_setting->Bend90.Changed = 0;
	    priv_setting->Bend90.PrevOct = WGI_RUBBER_LOCKIN_NOT;
	}
	break;

	case wgiRubberTypeOfLarc90:
	{
	    priv_setting->Larc90.Changed = 0;
	    priv_setting->Larc90.PrevOct = WGI_RUBBER_LOCKIN_NOT;
	}
	break;

	case wgiRubberTypeOfCloser:

	case wgiRubberTypeOfArc3pt:

	case wgiRubberTypeOfCommon:
	case wgiRubberTypeOfXhairs:
	case wgiRubberTypeOfBounds:
	case wgiRubberTypeOfVector:
	case wgiRubberTypeOfCircle:
	case wgiRubberTypeOfNshape:
	case wgiRubberTypeOfEllipt:
	case wgiRubberTypeOfTextEx:
	case wgiRubberTypeOfMyType:
	default:
	{
	    break;
	}
    }
}

//=========================================================

// for some reason the compiler complains if this is not static
static WgiRubberBand s_WgiDefaultRubberBand ((WgiRubberManager*) NULL, 3);
WgiRubberBand& WgiDefaultRubberBand = s_WgiDefaultRubberBand;

WgiRubberBand::WgiRubberBand (WgiRubberManager* man, 
				wgiRubberStruct_t* setting)
:	priv_man (man)
{
    if (setting) priv_setting_s = *setting;
    else priv_setting_s = WgiDefaultRubberBand.setting();
    
    priv_setting = & priv_setting_s;

    init_outputs();

    if (man) man->addBand (this);
}

WgiRubberBand::WgiRubberBand (WgiRubberManager* man, int default_setting)
:	priv_man (man)
{
    wgiRubberStruct_t list[2];
    grub (default_setting, list, 0, 0, 0, 0, 0, 0, 1, 0);
    priv_setting_s = list[0];

    priv_setting = & priv_setting_s;

    init_outputs();

    if (man) man->addBand (this);
}

WgiRubberBand::~WgiRubberBand ()
{
    if (priv_man) priv_man->removeBand (this);
}

void
WgiRubberBandCloser::addPoint (const WgiWorldLoc& loc)
{
    WLDcrd* pts = new WLDcrd [Number()+1];
    for (int i = 0; i < Number(); i++)
    {
	pts[i] = Points(i);
    }
    pts[Number()] = loc;

    if (Points())
	delete priv_setting->Closer.Points;
    priv_setting->Closer.Points = pts;
    priv_setting->Closer.Number = Number() + 1;
}

//------------------------------------------------------------

int grub (long type, wgiRubberStruct_t* list, double xpos, double ypos, 
	double offx, double offy, double xpt2, double ypt2, 
	WgiVirtualColorIndex real, WgiVirtualColorIndex fore,
	String* pick_func /*= NULL */)
{
	WLDcrd here[2];
	long number = 1;

	if (pick_func) *pick_func = "default";

	list[0].Common.UserFn = NULL;
	list[0].Common.FnData = NULL;
	list[0].Common.PlaneI = 0;

	switch(type)
	{
	case 0: default:
	list[0].Bounds.TypeOf = wgiRubberTypeOfBounds;
	list[0].Bounds.BaseIs = wgiRubberBaseIsCorner;
	list[0].Bounds.Anchor.x = xpos;
	list[0].Bounds.Anchor.y = ypos;
	list[0].Bounds.Offset.x = offx;
	list[0].Bounds.Offset.y = offy;
	list[0].Bounds.Dashed = wgiDeviceDashedDitDot;
	list[0].Bounds.ColorI = real;
	list[0].Bounds.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Bounds.Spokes = wgiRubberSpokesDoNot;
	list[0].Bounds.Aspect = 0.0;
	break;

	case 1:
	list[0].Bounds.TypeOf = wgiRubberTypeOfBounds;
	list[0].Bounds.BaseIs = wgiRubberBaseIsCenter;
	list[0].Bounds.Anchor.x = xpos;
	list[0].Bounds.Anchor.y = ypos;
	list[0].Bounds.Offset.x = offx;
	list[0].Bounds.Offset.y = offy;
	list[0].Bounds.Dashed = wgiDeviceDashedDoNone;
	list[0].Bounds.ColorI = real;
	list[0].Bounds.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Bounds.Spokes = wgiRubberSpokesScope;
	list[0].Bounds.Aspect = (3.0/5.0);
	break;

	case 2:
	list[0].Bounds.TypeOf = wgiRubberTypeOfBounds;
	list[0].Bounds.BaseIs = wgiRubberBaseIsCorner;
	list[0].Bounds.Anchor.x = xpos;
	list[0].Bounds.Anchor.y = ypos;
	list[0].Bounds.Offset.x = offx;
	list[0].Bounds.Offset.y = offy;
	list[0].Bounds.Dashed = wgiDeviceDashedDoNone;
	list[0].Bounds.ColorI = real;
	list[0].Bounds.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Bounds.Spokes = wgiRubberSpokesCross;
	list[0].Bounds.Aspect = (3.0/5.0);
	break;

	case 3:
	list[0].Vector.TypeOf = wgiRubberTypeOfVector;
	list[0].Vector.Anchor.x = xpos;
	list[0].Vector.Anchor.y = ypos;
	list[0].Vector.Offset.x = offx;
	list[0].Vector.Offset.y = offy;
	list[0].Vector.Dashed = wgiDeviceDashedDoNone;
	list[0].Vector.ColorI = real;
	list[0].Vector.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Vector.SnapTo = wgiRubberSnapToFree;
	break;

	case 4:
	list[0].Vector.TypeOf = wgiRubberTypeOfVector;
	list[0].Vector.Anchor.x = xpos;
	list[0].Vector.Anchor.y = ypos;
	list[0].Vector.Offset.x = offx;
	list[0].Vector.Offset.y = offy;
	list[0].Vector.Dashed = wgiDeviceDashedDoNone;
	list[0].Vector.ColorI = real;
	list[0].Vector.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Vector.SnapTo = wgiRubberSnapToSemi;
	break;

	case 5:
	list[0].Vector.TypeOf = wgiRubberTypeOfVector;
	list[0].Vector.Anchor.x = xpos;
	list[0].Vector.Anchor.y = ypos;
	list[0].Vector.Offset.x = offx;
	list[0].Vector.Offset.y = offy;
	list[0].Vector.Dashed = wgiDeviceDashedDoNone;
	list[0].Vector.ColorI = real;
	list[0].Vector.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Vector.SnapTo = wgiRubberSnapToManh;
	break;

	case 6:
	list[0].Vector.TypeOf = wgiRubberTypeOfVector;
	list[0].Vector.Anchor.x = xpos;
	list[0].Vector.Anchor.y = ypos;
	list[0].Vector.Offset.x = offx;
	list[0].Vector.Offset.y = offy;
	list[0].Vector.Dashed = wgiDeviceDashedDoNone;
	list[0].Vector.ColorI = real;
	list[0].Vector.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Vector.SnapTo = wgiRubberSnapToVert;
	break;

	case 7:
	list[0].Vector.TypeOf = wgiRubberTypeOfVector;
	list[0].Vector.Anchor.x = xpos;
	list[0].Vector.Anchor.y = ypos;
	list[0].Vector.Offset.x = offx;
	list[0].Vector.Offset.y = offy;
	list[0].Vector.Dashed = wgiDeviceDashedDoNone;
	list[0].Vector.ColorI = real;
	list[0].Vector.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Vector.SnapTo = wgiRubberSnapToHorz;
	break;

	case 8:
	list[0].Vector.TypeOf = wgiRubberTypeOfVector;
	list[0].Vector.Anchor.x = xpos;
	list[0].Vector.Anchor.y = ypos;
	list[0].Vector.Offset.x = offx;
	list[0].Vector.Offset.y = offy;
	list[0].Vector.Dashed = wgiDeviceDashedDoNone;
	list[0].Vector.ColorI = real;
	list[0].Vector.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Vector.SnapTo = wgiRubberSnapToDiag;
	break;

	case 9:
	list[0].Mitred.TypeOf = wgiRubberTypeOfMitred;
	list[0].Mitred.Anchor.x = xpos;
	list[0].Mitred.Anchor.y = ypos;
	list[0].Mitred.Offset.x = offx;
	list[0].Mitred.Offset.y = offy;
	list[0].Mitred.Dashed = wgiDeviceDashedDoNone;
	list[0].Mitred.ColorI = real;
	list[0].Mitred.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Mitred.MaxLen = 70.0;
	list[0].Mitred.Locked = TRUE;
	break;

	case 10:
	list[0].Mitred.TypeOf = wgiRubberTypeOfMitred;
	list[0].Mitred.Anchor.x = xpos;
	list[0].Mitred.Anchor.y = ypos;
	list[0].Mitred.Offset.x = offx;
	list[0].Mitred.Offset.y = offy;
	list[0].Mitred.Dashed = wgiDeviceDashedDoNone;
	list[0].Mitred.ColorI = real;
	list[0].Mitred.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Mitred.MaxLen = 0.0;
	list[0].Mitred.Locked = TRUE;
	break;

	case 11:
	list[0].Mitred.TypeOf = wgiRubberTypeOfMitred;
	list[0].Mitred.Anchor.x = xpos;
	list[0].Mitred.Anchor.y = ypos;
	list[0].Mitred.Offset.x = offx;
	list[0].Mitred.Offset.y = offy;
	list[0].Mitred.Dashed = wgiDeviceDashedDoNone;
	list[0].Mitred.ColorI = real;
	list[0].Mitred.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Mitred.MaxLen = 0.0;
	list[0].Mitred.Locked = TRUE;
	break;

	case 12:
	list[0].Larc90.TypeOf = wgiRubberTypeOfLarc90;
	list[0].Larc90.Anchor.x = xpos;
	list[0].Larc90.Anchor.y = ypos;
	list[0].Larc90.Offset.x = offx;
	list[0].Larc90.Offset.y = offy;
	list[0].Larc90.Dashed = wgiDeviceDashedDoNone;
	list[0].Larc90.ColorI = real;
	list[0].Larc90.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Larc90.MaxRad = 50.0;
	list[0].Larc90.Locked = TRUE;
	break;

	case 13:
	list[0].Larc90.TypeOf = wgiRubberTypeOfLarc90;
	list[0].Larc90.Anchor.x = xpos;
	list[0].Larc90.Anchor.y = ypos;
	list[0].Larc90.Offset.x = offx;
	list[0].Larc90.Offset.y = offy;
	list[0].Larc90.Dashed = wgiDeviceDashedDoNone;
	list[0].Larc90.ColorI = real;
	list[0].Larc90.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Larc90.MaxRad = 0.0;
	list[0].Larc90.Locked = TRUE;
	break;

	case 14:
	list[0].Larc45.TypeOf = wgiRubberTypeOfLarc45;
	list[0].Larc45.Anchor.x = xpos;
	list[0].Larc45.Anchor.y = ypos;
	list[0].Larc45.Offset.x = offx;
	list[0].Larc45.Offset.y = offy;
	list[0].Larc45.Dashed = wgiDeviceDashedDoNone;
	list[0].Larc45.ColorI = real;
	list[0].Larc45.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Larc45.MaxRad = 90.0;
//	list[0].Larc45.Locked = TRUE;
	break;

	case 15:
	list[0].Larc45.TypeOf = wgiRubberTypeOfLarc45;
	list[0].Larc45.Anchor.x = xpos;
	list[0].Larc45.Anchor.y = ypos;
	list[0].Larc45.Offset.x = offx;
	list[0].Larc45.Offset.y = offy;
	list[0].Larc45.Dashed = wgiDeviceDashedDoNone;
	list[0].Larc45.ColorI = real;
	list[0].Larc45.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Larc45.MaxRad = 90.0;
//	list[0].Larc45.Locked = FALSE;
	break;

	case 16:
	list[0].Arc3pt.TypeOf = wgiRubberTypeOfArc3pt;
	list[0].Arc3pt.Anchor.x = xpos;
	list[0].Arc3pt.Anchor.y = ypos;
	list[0].Arc3pt.Offset.x = offx;
	list[0].Arc3pt.Offset.y = offy;
	list[0].Arc3pt.Second.x = xpt2;
	list[0].Arc3pt.Second.y = ypt2;
	list[0].Arc3pt.Dashed = wgiDeviceDashedDoNone;
	list[0].Arc3pt.ColorI = real;
	list[0].Arc3pt.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Arc3pt.LastIs = wgiRubberLastIsMid;
	list[0].Arc3pt.Spokes = wgiRubberSpokesDoNot;

	*pick_func = "arc";

	break;

	case 17:
	list[0].Arc3pt.TypeOf = wgiRubberTypeOfArc3pt;
	list[0].Arc3pt.Anchor.x = xpos;
	list[0].Arc3pt.Anchor.y = ypos;
	list[0].Arc3pt.Offset.x = offx;
	list[0].Arc3pt.Offset.y = offy;
	list[0].Arc3pt.Second.x = xpt2;
	list[0].Arc3pt.Second.y = ypt2;
	list[0].Arc3pt.Dashed = wgiDeviceDashedDoNone;
	list[0].Arc3pt.ColorI = real;
	list[0].Arc3pt.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Arc3pt.LastIs = wgiRubberLastIsEnd;
	list[0].Arc3pt.Spokes = wgiRubberSpokesSlice;

	*pick_func = "arc";

	break;

	case 18:
	list[0].Circle.TypeOf = wgiRubberTypeOfCircle;
	list[0].Circle.Anchor.x = xpos;
	list[0].Circle.Anchor.y = ypos;
	list[0].Circle.Offset.x = offx;
	list[0].Circle.Offset.y = offy;
	list[0].Circle.Dashed = wgiDeviceDashedDitDot;
	list[0].Circle.ColorI = real;
	list[0].Circle.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Circle.Spokes = wgiRubberSpokesDoNot;
	break;

	case 19:
	list[0].Circle.TypeOf = wgiRubberTypeOfCircle;
	list[0].Circle.Anchor.x = xpos;
	list[0].Circle.Anchor.y = ypos;
	list[0].Circle.Offset.x = offx;
	list[0].Circle.Offset.y = offy;
	list[0].Circle.Dashed = wgiDeviceDashedDoNone;
	list[0].Circle.ColorI = real;
	list[0].Circle.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Circle.Spokes = wgiRubberSpokesSlice;
	break;

	case 21:
	list[0].Circle.TypeOf = wgiRubberTypeOfCircle;
	list[0].Circle.Anchor.x = xpos;
	list[0].Circle.Anchor.y = ypos;
	list[0].Circle.Offset.x = offx;
	list[0].Circle.Offset.y = offy;
	list[0].Circle.Dashed = wgiDeviceDashedDoNone;
	list[0].Circle.ColorI = real;
	list[0].Circle.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Circle.Spokes = wgiRubberSpokesCross;
	break;

	case 22:
	list[0].Circle.TypeOf = wgiRubberTypeOfCircle;
	list[0].Circle.Anchor.x = xpos;
	list[0].Circle.Anchor.y = ypos;
	list[0].Circle.Offset.x = offx;
	list[0].Circle.Offset.y = offy;
	list[0].Circle.Dashed = wgiDeviceDashedDoNone;
	list[0].Circle.ColorI = real;
	list[0].Circle.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Circle.Spokes = wgiRubberSpokesScope;
	break;

	case 23:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 0;
	break;

	case 24:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 1;
	break;

	case 25:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 2;
	break;

	case 26:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 3;
	break;

	case 27:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesSlice;
	list[0].Nshape.Nsides = 3;
	break;

	case 28:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 4;
	break;

	case 29:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesSlice;
	list[0].Nshape.Nsides = 4;
	break;

	case 30:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 5;
	break;

	case 31:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesSlice;
	list[0].Nshape.Nsides = 5;
	break;

	case 32:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 6;
	break;

	case 33:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesSlice;
	list[0].Nshape.Nsides = 6;
	break;

	case 34:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 7;
	break;

	case 35:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesSlice;
	list[0].Nshape.Nsides = 7;
	break;

	case 36:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 8;
	break;

	case 37:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesSlice;
	list[0].Nshape.Nsides = 8;
	break;

	case 38:
	list[0].Nshape.TypeOf = wgiRubberTypeOfNshape;
	list[0].Nshape.Anchor.x = xpos;
	list[0].Nshape.Anchor.y = ypos;
	list[0].Nshape.Offset.x = offx;
	list[0].Nshape.Offset.y = offy;
	list[0].Nshape.Dashed = wgiDeviceDashedDoNone;
	list[0].Nshape.ColorI = real;
	list[0].Nshape.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Nshape.Spokes = wgiRubberSpokesDoNot;
	list[0].Nshape.Nsides = 9;
	break;

	case 39:
	list[0].Ellipt.TypeOf = wgiRubberTypeOfEllipt;
	list[0].Ellipt.BaseIs = wgiRubberBaseIsCorner;
	list[0].Ellipt.Anchor.x = xpos;
	list[0].Ellipt.Anchor.y = ypos;
	list[0].Ellipt.Offset.x = offx;
	list[0].Ellipt.Offset.y = offy;
	list[0].Ellipt.Dashed = wgiDeviceDashedDoNone;
	list[0].Ellipt.ColorI = real;
	list[0].Ellipt.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Ellipt.Spokes = wgiRubberSpokesDoNot;
	list[0].Ellipt.DoRect = TRUE;
	list[0].Ellipt.Aspect = 0.0;
	break;

	case 40:
	list[0].Ellipt.TypeOf = wgiRubberTypeOfEllipt;
	list[0].Ellipt.BaseIs = wgiRubberBaseIsCenter;
	list[0].Ellipt.Anchor.x = xpos;
	list[0].Ellipt.Anchor.y = ypos;
	list[0].Ellipt.Offset.x = offx;
	list[0].Ellipt.Offset.y = offy;
	list[0].Ellipt.Dashed = wgiDeviceDashedDoNone;
	list[0].Ellipt.ColorI = real;
	list[0].Ellipt.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Ellipt.Spokes = wgiRubberSpokesScope;
	list[0].Ellipt.DoRect = TRUE;
	list[0].Ellipt.Aspect = (5.0/3.0);
	break;

	case 41:
	list[0].Ellipt.TypeOf = wgiRubberTypeOfEllipt;
	list[0].Ellipt.BaseIs = wgiRubberBaseIsCorner;
	list[0].Ellipt.Anchor.x = xpos;
	list[0].Ellipt.Anchor.y = ypos;
	list[0].Ellipt.Offset.x = offx;
	list[0].Ellipt.Offset.y = offy;
	list[0].Ellipt.Dashed = wgiDeviceDashedDoNone;
	list[0].Ellipt.ColorI = real;
	list[0].Ellipt.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Ellipt.Spokes = wgiRubberSpokesDoNot;
	list[0].Ellipt.DoRect = TRUE;
	list[0].Ellipt.Aspect = (3.0/5.0);
	break;

	case 42:
	list[0].TextEx.TypeOf = wgiRubberTypeOfTextEx;
	list[0].TextEx.Anchor.x = xpos;
	list[0].TextEx.Anchor.y = ypos;
	list[0].TextEx.Offset.x = offx;
	list[0].TextEx.Offset.y = offy;
	list[0].TextEx.Dashed = wgiDeviceDashedDoNone;
	list[0].TextEx.ColorI = real;
	list[0].TextEx.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].TextEx.DoRect = FALSE;
	list[0].TextEx.Aspect = 0.0;
	list[0].TextEx.Spaces = (0.33333);
	list[0].TextEx.TextRe = "DEAN";
	break;

	case 43:
	list[0].TextEx.TypeOf = wgiRubberTypeOfTextEx;
	list[0].TextEx.Anchor.x = xpos;
	list[0].TextEx.Anchor.y = ypos;
	list[0].TextEx.Offset.x = offx;
	list[0].TextEx.Offset.y = offy;
	list[0].TextEx.Dashed = wgiDeviceDashedDoNone;
	list[0].TextEx.ColorI = real;
	list[0].TextEx.DrawOp = wgiDeviceDrawOpSetDel;
	list[0].TextEx.DoRect = FALSE;
	list[0].TextEx.Aspect = (3.0/5.0);
	list[0].TextEx.Spaces = (0.33333);
	list[0].TextEx.TextRe = "Locked";
	break;

	case 44:
	list[0].Xhairs.TypeOf = wgiRubberTypeOfXhairs;
	list[0].Xhairs.Anchor.x = xpos;
	list[0].Xhairs.Anchor.y = ypos;
	list[0].Xhairs.Offset.x = offx;
	list[0].Xhairs.Offset.y = offy;
	list[0].Xhairs.Dashed = wgiDeviceDashedDoNone;
	list[0].Xhairs.ColorI = real;
	list[0].Xhairs.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Xhairs.HairTo = wgiRubberHairToEdges;
	break;

	case 45:
	list[0].Xhairs.TypeOf = wgiRubberTypeOfXhairs;
	list[0].Xhairs.Anchor.x = xpos;
	list[0].Xhairs.Anchor.y = ypos;
	list[0].Xhairs.Offset.x = offx;
	list[0].Xhairs.Offset.y = offy;
	list[0].Xhairs.Dashed = wgiDeviceDashedDitDit;
	list[0].Xhairs.ColorI = real;
	list[0].Xhairs.DrawOp = wgiDeviceDrawOpSetDel;
	list[0].Xhairs.HairTo = wgiRubberHairToEdges;

	number = 2;

	list[1].Vector.TypeOf = wgiRubberTypeOfVector;
	list[1].Vector.Anchor.x = xpos;
	list[1].Vector.Anchor.y = ypos;
	list[1].Vector.Offset.x = offx;
	list[1].Vector.Offset.y = offy;
	list[1].Vector.Dashed = wgiDeviceDashedDoNone;
	list[1].Vector.ColorI = fore;
	list[1].Vector.DrawOp = wgiDeviceDrawOpSetDel;
	list[1].Vector.SnapTo = wgiRubberSnapToFree;
	break;

	case 46:
	list[0].Xhairs.TypeOf = wgiRubberTypeOfXhairs;
	list[0].Xhairs.Anchor.x = xpos;
	list[0].Xhairs.Anchor.y = ypos;
	list[0].Xhairs.Offset.x = offx;
	list[0].Xhairs.Offset.y = offy;
	list[0].Xhairs.Dashed = wgiDeviceDashedDotDot;
	list[0].Xhairs.ColorI = real;
	list[0].Xhairs.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Xhairs.Length = 200.0;
	list[0].Xhairs.HairTo = wgiRubberHairToConst;
	break;

	case 47:
	list[0].Xhairs.TypeOf = wgiRubberTypeOfXhairs;
	list[0].Xhairs.Anchor.x = xpos;
	list[0].Xhairs.Anchor.y = ypos;
	list[0].Xhairs.Offset.x = offx;
	list[0].Xhairs.Offset.y = offy;
	list[0].Xhairs.Dashed = wgiDeviceDashedDoNone;
	list[0].Xhairs.ColorI = fore;
	list[0].Xhairs.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Xhairs.Length = 200.0;
	list[0].Xhairs.HairTo = wgiRubberHairToScale;
	break;

	case 48:
//	wgi_ends[0].x = xpos;
//	wgi_ends[0].y = ypos;
//	wgi_ends[1].x = xpt2;
//	wgi_ends[1].y = ypt2;
	list[0].Closer.TypeOf = wgiRubberTypeOfCloser;
	list[0].Closer.Anchor.x = 0.0;
	list[0].Closer.Anchor.y = 0.0;
	list[0].Closer.Offset.x = offx;
	list[0].Closer.Offset.y = offy;
	list[0].Closer.Dashed = wgiDeviceDashedDoNone;
	list[0].Closer.ColorI = fore;
	list[0].Closer.DrawOp = wgiDeviceDrawOpXorXor;
//	list[0].Closer.Number = 2;
//	list[0].Closer.Points = wgi_ends;
	list[0].Closer.Number = 0;
	list[0].Closer.Points = NULL;
	list[0].Closer.DoCopy = FALSE;

	*pick_func = "closer";

	break;

	case 49:
//	here[0].x = xpos;
//	here[0].y = ypos;
//	here[1].x = xpt2;
//	here[1].y = ypt2;
	list[0].Closer.TypeOf = wgiRubberTypeOfCloser;
	list[0].Closer.Anchor.x = 0.0;
	list[0].Closer.Anchor.y = 0.0;
	list[0].Closer.Offset.x = offx;
	list[0].Closer.Offset.y = offy;
	list[0].Closer.Dashed = wgiDeviceDashedDoNone;
	list[0].Closer.ColorI = fore;
	list[0].Closer.DrawOp = wgiDeviceDrawOpXorXor;
//	list[0].Closer.Number = 2;
//	list[0].Closer.Points = here;
	list[0].Closer.Number = 0;
	list[0].Closer.Points = NULL;
	list[0].Closer.DoCopy = TRUE;

	*pick_func = "closer";

	break;

	case 50:
	list[0].Bend90.TypeOf = wgiRubberTypeOfBend90;
	list[0].Bend90.Anchor.x = xpos;
	list[0].Bend90.Anchor.y = ypos;
	list[0].Bend90.Offset.x = offx;
	list[0].Bend90.Offset.y = offy;
	list[0].Bend90.Dashed = wgiDeviceDashedDoNone;
	list[0].Bend90.ColorI = real;
	list[0].Bend90.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Bend90.Locked = TRUE;
	break;

	case 51:
	list[0].Bend90.TypeOf = wgiRubberTypeOfBend90;
	list[0].Bend90.Anchor.x = xpos;
	list[0].Bend90.Anchor.y = ypos;
	list[0].Bend90.Offset.x = offx;
	list[0].Bend90.Offset.y = offy;
	list[0].Bend90.Dashed = wgiDeviceDashedDoNone;
	list[0].Bend90.ColorI = real;
	list[0].Bend90.DrawOp = wgiDeviceDrawOpSetDel;
	list[0].Bend90.Locked = TRUE;

	number = 2;

	list[1].Bend90.TypeOf = wgiRubberTypeOfBend90;
	list[1].Bend90.Anchor.x = xpt2;
	list[1].Bend90.Anchor.y = ypt2;
	list[1].Bend90.Offset.x = offx;
	list[1].Bend90.Offset.y = offy;
	list[1].Bend90.Dashed = wgiDeviceDashedDoNone;
	list[1].Bend90.ColorI = fore;
	list[1].Bend90.DrawOp = wgiDeviceDrawOpSetDel;
	list[1].Bend90.Locked = TRUE;

	*pick_func = "dbl_mitred";

	break;

	case 52:
	list[0].Mitred.TypeOf = wgiRubberTypeOfMitred;
	list[0].Mitred.Anchor.x = xpos;
	list[0].Mitred.Anchor.y = ypos;
	list[0].Mitred.Offset.x = offx;
	list[0].Mitred.Offset.y = offy;
	list[0].Mitred.Dashed = wgiDeviceDashedDoNone;
	list[0].Mitred.ColorI = real;
	list[0].Mitred.DrawOp = wgiDeviceDrawOpSetDel;
	list[0].Mitred.MaxLen = 0.0;
	list[0].Mitred.Locked = TRUE;

	number = 2;

	list[1].Mitred.TypeOf = wgiRubberTypeOfMitred;
	list[1].Mitred.Anchor.x = xpt2;
	list[1].Mitred.Anchor.y = ypt2;
	list[1].Mitred.Offset.x = offx;
	list[1].Mitred.Offset.y = offy;
	list[1].Mitred.Dashed = wgiDeviceDashedDoNone;
	list[1].Mitred.ColorI = fore;
	list[1].Mitred.DrawOp = wgiDeviceDrawOpSetDel;
	list[1].Mitred.MaxLen = 0.0;
	list[1].Mitred.Locked = TRUE;

	*pick_func = "dbl_mitred";

	break;

	case 53:
	list[0].Mitred.TypeOf = wgiRubberTypeOfMitred;
	list[0].Mitred.Anchor.x = xpos;
	list[0].Mitred.Anchor.y = ypos;
	list[0].Mitred.Offset.x = offx;
	list[0].Mitred.Offset.y = offy;
	list[0].Mitred.Dashed = wgiDeviceDashedDoNone;
	list[0].Mitred.ColorI = real;
	list[0].Mitred.DrawOp = wgiDeviceDrawOpSetDel;
	list[0].Mitred.MaxLen = 70.0;
	list[0].Mitred.Locked = TRUE;

	number = 2;

	list[1].Mitred.TypeOf = wgiRubberTypeOfMitred;
	list[1].Mitred.Anchor.x = xpt2;
	list[1].Mitred.Anchor.y = ypt2;
	list[1].Mitred.Offset.x = offx;
	list[1].Mitred.Offset.y = offy;
	list[1].Mitred.Dashed = wgiDeviceDashedDoNone;
	list[1].Mitred.ColorI = fore;
	list[1].Mitred.DrawOp = wgiDeviceDrawOpSetDel;
	list[1].Mitred.MaxLen = 70.0;
	list[1].Mitred.Locked = FALSE;
	break;

	case 54:
	list[0].Larc45.TypeOf = wgiRubberTypeOfLarc45Rot;
	list[0].Larc45.Anchor.x = xpos;
	list[0].Larc45.Anchor.y = ypos;
	list[0].Larc45.Offset.x = offx;
	list[0].Larc45.Offset.y = offy;
	list[0].Larc45.Dashed = wgiDeviceDashedDoNone;
	list[0].Larc45.ColorI = real;
	list[0].Larc45.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Larc45.MaxRad = 90.0;
//	list[0].Larc45.Locked = TRUE;
	break;

	case 55:
	list[0].Larc45.TypeOf = wgiRubberTypeOfLarc45Rot;
	list[0].Larc45.Anchor.x = xpos;
	list[0].Larc45.Anchor.y = ypos;
	list[0].Larc45.Offset.x = offx;
	list[0].Larc45.Offset.y = offy;
	list[0].Larc45.Dashed = wgiDeviceDashedDoNone;
	list[0].Larc45.ColorI = real;
	list[0].Larc45.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Larc45.MaxRad = 90.0;
//	list[0].Larc45.Locked = FALSE;
	break;

	case 56:

	list[0].Larc45.TypeOf = wgiRubberTypeOfLarc45;
	list[0].Larc45.Anchor.x = xpos;
	list[0].Larc45.Anchor.y = ypos;
	list[0].Larc45.Offset.x = offx;
	list[0].Larc45.Offset.y = offy;
	list[0].Larc45.Dashed = wgiDeviceDashedDoNone;
	list[0].Larc45.ColorI = real;
	list[0].Larc45.DrawOp = wgiDeviceDrawOpXorXor;
	list[0].Larc45.MaxRad = 90.0;

	number = 2;

	list[1].Larc45.TypeOf = wgiRubberTypeOfLarc45Rot;
	list[1].Larc45.Anchor.x = xpos;
	list[1].Larc45.Anchor.y = ypos;
	list[1].Larc45.Offset.x = offx;
	list[1].Larc45.Offset.y = offy;
	list[1].Larc45.Dashed = wgiDeviceDashedDoNone;
	list[1].Larc45.ColorI = real;
	list[1].Larc45.DrawOp = wgiDeviceDrawOpXorXor;
	list[1].Larc45.MaxRad = 90.0;

	*pick_func = "switch45";

	break;

	}

	return number;
}

//------------------------------------------------------------

ostream&
operator<< (ostream& os, const WgiRubberBand& r)
{
    return os << "WgiRubberBand(unimpl)";
}

ostream&
operator<< (ostream& os, const WgiRubberManager& r)
{
    return os << "WgiRubberManager(unimpl)";
}

ostream&
operator<< (ostream& os, const WgiRubberSeq& r)
{
    return os << "WgiRubberSeq(unimpl)";
}

