#include "Wgi.h"
#include "Zfile.h"
#include "DebugFlag.h"

DEBUG_FLAG(debug_wgi_read, "wgi_read");

Listimplement(SegprimPtr);

Mapimplement(WgiSegListId,WgiSegListPtr);

void WgiDefaultError (const char* file, int line, const char* func,
	const char* msg)
{
    fprintf(stderr, "Error in %s: %s [%s/%d]\n",
	func, msg, file, line);
}

ostream& operator<<(ostream& os, const WgiWorldLoc& l)
{
    char buffer[40];
    sprintf(buffer, "(%.9G,%.9G)", l.x, l.y);
    return os << buffer;
}

//------------------------------------------------------------
//
//		Wgi class
//
//------------------------------------------------------------

/* static class data members */

long		Wgi::priv_seglist_card = 0;
WgiSegListMap	Wgi::priv_map;
Boolean		Wgi::priv_create_when_read = TRUE;
WgiSegmentId	Wgi::priv_max_id = -1;

long		Wgi::priv_seglist_count = 0;
long		Wgi::priv_segment_count = 0;
long		Wgi::priv_segprim_count [WGI_PRIM_SIZE] = { 0 };

Boolean
Wgi::debug ()
{
    return (int) debug_wgi_read;
}

Boolean
Wgi::debug (Boolean new_flag)
{
    int old = debug_wgi_read;
    debug_wgi_read.set (new_flag);
    return Boolean (old);
}

void
Wgi::dump (FILE* fp, long card, const WgiWorldLoc* loc)
{
    if (debug())
    {
        cout << "p[" << card << "]=";
        for (int i=0; i < card; i++)
        {
	    cout << " " << loc[i];
        }
        cout << endl;
    }
    fwrite (loc, sizeof(WgiWorldLoc), (int) card, fp);
}

void
Wgi::read (FILE* fp, long card, WgiWorldLoc* loc)
{
    cout.sync_with_stdio();

    fread (loc, sizeof(WgiWorldLoc), (int) card, fp);
    if (debug())
    {
        cout << "p[" << card << "]=";
        for (int i=0; i < card; i++)
        {
	    cout << " " << loc[i];
        }
        cout << endl;
    }
}

Boolean
Wgi::addSegList (const WgiSegList* list)
{
    if (seglist (list->id()) != NULL)
	return FALSE;
    else
    {
	priv_map[list->id()] = (WgiSegList*) list;
	priv_seglist_card++;
	return TRUE;
    }
}

Boolean
Wgi::removeSegList (const WgiSegList* list)
{
    if (seglist (list->id()) == NULL)
	return FALSE;
    else
    {
	priv_map[list->id()] = NULL;
	priv_seglist_card--;
	return TRUE;
    }
}

Boolean
Wgi::read (const char* fileName)
{
    Zfile file (fileName);   // file or pipe will be closed automatically
    FILE* fp = file.filePointer();

    if (fp == NULL)
    {
	return FALSE;
    }
    long seglist_card;
    read (fp, &seglist_card);

    if (debug())
	cout << "Wgi n=" << seglist_card << endl;

    for (int i=0; i < seglist_card; i++)
    {
	WgiSegList* list = new WgiSegList(fp);

	addSegList (list);
    }

    return TRUE;
}

Boolean
Wgi::dump (const char* fileName)
{
    FILE* fp = fopen (fileName, "w");
    if (fp == NULL)
    {
	return FALSE;
    }
    long seglist_card = cardinality();

    if (debug())
	cout << "Wgi n=" << seglist_card << endl;

    Wgi::dump (fp, seglist_card);
    WGI_SEGLIST_LOOP (list)
    {
	list->dump (fp);
    }
    fclose (fp);
    return TRUE;
}

long
Wgi::segPrimCount ()
{
    long total = 0;
    for (int i = 0; i < WGI_PRIM_SIZE; i++)
    {
	total += segPrimCount(i);
    }
    return total;
}

/* static */ Extent
Wgi::allSeglistExtent ()
{
    Extent ext;
    iwgi_extent_inverse(&ext);

    WGI_SEGLIST_LOOP(list)
    {
	iwgi_extent_combine (list->extent(), &ext);
    }
    return ext;
}

//------------------------------------------------------------
//
//		WgiSegList class
//
//------------------------------------------------------------

long
WgiSegList::cardinality () const
{
    long c = 0;
    for (Segment* seg = priv_seglist->segs; seg != NULL; seg = seg->next)
    {
	c++;
    }
    return c;
}

void
WgiSegList::dump (FILE* fp)
{
    if (Wgi::debug())
	cout << " WgiSegList id=" << id() << " mask=" << mask() << 
	" n=" << cardinality() << endl;

    Wgi::dump (fp, id());
    Wgi::dump (fp, mask());
    long n = cardinality ();
    Wgi::dump (fp, n);

// ?? maybe these should be in reverse order

    for (Segment* seg = priv_seglist->segs; seg != NULL; seg = seg->next)
    {
	WgiSegment(seg).dump(fp, worldTable());
    }
}

void
WgiSegList::setCurrent ()
{
    wgi_seglist_current (priv_seglist);
}

/* static */ WgiSegList
WgiSegList::current ()
{
    Seglist* p;
    wgi_get_current_seglist (&p);
    return WgiSegList(p, -1);
// ?? id ??
}


WgiReturnCode
WgiSegList::create (WgiPlaneMask mask, WgiSegListId appl_seglist_id,
	Boolean set_current /*= TRUE */)
{
    priv_appl_id = appl_seglist_id;
    WgiReturnCode rc = wgi_seglist_create (&priv_seglist, mask);    
    if (set_current) setCurrent ();

    Wgi::addSegList (this);
    return rc;
}

WgiSegList::WgiSegList (WgiPlaneMask mask, WgiSegListId appl_seglist_id)
: priv_seglist(NULL), priv_appl_id(-1)
{
    create (mask, appl_seglist_id);
}

static long s_segment_count = 0;

WgiSegList::WgiSegList (FILE* fp)
: priv_seglist(NULL), priv_appl_id(-1)
{
    WgiSegListId id;
    WgiPlaneMask mask;
    Wgi::read (fp, &id);
    Wgi::read (fp, &mask);
    long n;
    Wgi::read (fp, &n);

    if (Wgi::debug())
    {
	cout << " WgiSegList id=" << id << " mask=" << mask << " n="
		<< n << endl;
    }

    WgiSegList* list = NULL;
    if (Wgi::create()) 
    {
	create (mask, id);
    }
    Wgi::priv_seglist_count++;

    s_segment_count = 0;
    for (int i = 0; i < n; i++)
    {
	WgiSegment::read (fp);
	s_segment_count++;
    }
}

WgiSegList::WgiSegList (Seglist* list, WgiSegListId appl_seglist_id)
:	priv_appl_id (appl_seglist_id), priv_seglist(list)
{
    Wgi::addSegList (this);    
}

WgiReturnCode
WgiSegList::changeColors (WgiVirtualColorIndex edgeColor, 
		WgiVirtualColorIndex fillColor, WgiVirtualColorIndex textColor)
{
    setCurrent();
    return wgi_change_seglist_colors (edgeColor, fillColor, textColor);
}

WgiSegList::~WgiSegList ()
{
    Wgi::removeSegList (this);
    purge (TRUE);
// ?? not sure about TRUE
}

Boolean 
WgiSegList::segment (WgiSegmentId id, WgiSegmentLevel level,
	WgiSegment* segment)
{
    setCurrent ();
    Segment* seg = wgi_get_segment_from_iden(id, level);
    if (seg)
    {
	if (segment)
	    *segment = WgiSegment (seg);
	return TRUE;
    }
    else
	return FALSE;
}

//------------------------------------------------------------
//
//		WgiSegment class
//
//------------------------------------------------------------

WgiReturnCode 
WgiSegment::create (WgiSegmentId id, WgiSegmentLevel level, 
	WgiViewPortMask mask, WgiBoolean pickable, 
	WgiVirtualColorIndex edgeColor, WgiVirtualColorIndex fillColor, 
	WgiVirtualColorIndex textColor, WgiBoolean visible, 
	WgiBoolean drawable)
{
    return wgi_creseg (id, level, mask, pickable, edgeColor,
	fillColor, textColor, visible, drawable);
}

WgiSegment::WgiSegment (WgiSegmentId id, WgiSegmentLevel level, 
	WgiViewPortMask mask, WgiBoolean pickable, 
	WgiVirtualColorIndex edgeColor, WgiVirtualColorIndex fillColor, 
	WgiVirtualColorIndex textColor, WgiBoolean visible, 
	WgiBoolean drawable)
{
    create (id, level, mask, pickable, edgeColor,
	fillColor, textColor, visible, drawable);
    wgi_get_current_segment (&priv_segment);
}

void
WgiSegment::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "  WgiSegment id=" << id() << " level=" <<
		level() << " mask=" << mask() << " fill=" << (int)
		fillColor() << " edge=" << (int) edgeColor() <<
		" text=" << (int) textColor() << " draw=" << (int)
		drawable() << " pick=" << (int) pickable() << " visi="
		<< (int) visible() << " n=" << cardinality() << endl;
    }

    Wgi::dump (fp, id());
    Wgi::dump (fp, level());
    Wgi::dump (fp, mask());
    Wgi::dump (fp, fillColor());
    Wgi::dump (fp, edgeColor());
    Wgi::dump (fp, textColor());
    Wgi::dump (fp, drawable());
    Wgi::dump (fp, pickable());
    Wgi::dump (fp, visible());

    Wgi::dump (fp, cardinality ());
    SegprimList list;
    Segprim* prim;
    for (prim = segment()->prim; prim != NULL; prim = prim->next)
    {
	list.unget(prim);	// reverse order of list !
    }

    SegprimListIter iter(list);
    while (iter.next(prim))
    {
	WgiSegPrim(prim).dump(fp, table);
    }
}

static long s_segprim_count = 0;

void
WgiSegment::read (FILE* fp)
{
    WgiSegmentId id;
    WgiSegmentLevel level;
    WgiViewPortMask mask;
    WgiVirtualColorIndex fillColor, edgeColor, textColor;
    WgiBoolean drawable, pickable, visible;

    Wgi::read (fp, &id);
    Wgi::read (fp, &level);
    Wgi::read (fp, &mask);
    Wgi::read (fp, &fillColor);
    Wgi::read (fp, &edgeColor);
    Wgi::read (fp, &textColor);
    Wgi::read (fp, &drawable);
    Wgi::read (fp, &pickable);
    Wgi::read (fp, &visible);

    if (Wgi::create()) 
	create (id, level, mask, pickable, edgeColor, fillColor,
		textColor, visible, drawable);
    Wgi::priv_segment_count++;

    long n;
    Wgi::read (fp, &n);

    if (Wgi::debug())
    {
	cout << "  WgiSegment id=" << id << " level=" << level << 
		" mask=" << mask << " fill=" << (int) fillColor << 
		" edge=" << (int) edgeColor << " text=" << (int)
		textColor << " draw=" << (int) drawable << " pick=" <<
		(int) pickable << " visi=" << (int) visible << " n="
		<< n << endl;
    }

    s_segprim_count = 0;
    for (int i = 0; i < n; i++)
    {
	WgiSegPrim::read (fp);
	s_segprim_count++;
    }

	// keep track of maximum segment id that was read

    if (id > Wgi::priv_max_id)
	    Wgi::priv_max_id = id;

    if (Wgi::create()) 
	wgi_endseg();
}

long
WgiSegment::cardinality () const
{
    long c = 0;
    for (Segprim* prim = segment()->prim; prim != NULL; prim = prim->next)
    {
	c++;
    }
    return c;
}

Boolean 
WgiSegment::isCurrent () const
{
    Segment* seg = NULL;

    if (getCurrent (&seg) == WGI_SUCCESS)
	return Boolean (seg == segment());
    else
	return FALSE;
}

//------------------------------------------------------------
//
//		WgiSegPrim class
//
//------------------------------------------------------------

void
WgiSegPrim::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "   WgiSegPrim type=" << (int) type() << endl;
    }

    Wgi::dump (fp, type());
    switch (type())
    {
	case WGI_PRIM_POLYLINE:
	{
	    WgiPrimPolyLine(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_ZEBRA:
	{
	    WgiPrimZebra(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_CIRCLE:
	{
	    WgiPrimCircle(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_TEXT:
	{
	    WgiPrimText(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_MARKER:
	{
	    WgiPrimMarker(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_POLYGON:
	{
	    WgiPrimPolygon(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_RECTANGLE:
	{
	    WgiPrimRectangle(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_POINTS:
	{
	    WgiPrimPoints(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_STENCIL:
	{
	    WgiPrimStencil(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_HOLE:
	{
	    WgiPrimHole(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_FLUSH:
	{
	    WgiPrimFlush(segprim()).dump (fp, table);
	    break;
	}
	case WGI_PRIM_SIZE:
	default:
	{
	    WGI_ERROR ("WgiSegPrim::dump", 
		form("invalid prim type %d", type()));
	    return;
	}
    }
}

void
WgiSegPrim::read (FILE* fp)
{
    WgiPrimType type;
    Wgi::read (fp, &type);

    if (Wgi::debug())
    {
	cout << "   WgiSegPrim type=" << (int) type << endl;
    }

    if (type >=0 && type < WGI_PRIM_SIZE)
	Wgi::priv_segprim_count[type]++;
    // else: error

    switch (type)
    {
	case WGI_PRIM_POLYLINE:
	{
	    WgiPrimPolyLine::read (fp);
	    break;
	}
	case WGI_PRIM_ZEBRA:
	{
	    WgiPrimZebra::read (fp);
	    break;
	}
	case WGI_PRIM_CIRCLE:
	{
	    WgiPrimCircle::read(fp);
	    break;
	}
	case WGI_PRIM_TEXT:
	{
	    WgiPrimText::read(fp);
	    break;
	}
	case WGI_PRIM_MARKER:
	{
	    WgiPrimMarker::read(fp);
	    break;
	}
	case WGI_PRIM_POLYGON:
	{
	    WgiPrimPolygon::read(fp);
	    break;
	}
	case WGI_PRIM_RECTANGLE:
	{
	    WgiPrimRectangle::read(fp);
	    break;
	}
	case WGI_PRIM_POINTS:
	{
	    WgiPrimPoints::read(fp);
	    break;
	}
	case WGI_PRIM_STENCIL:
	{
	    WgiPrimStencil::read(fp);
	    break;
	}
	case WGI_PRIM_HOLE:
	{
	    WgiPrimHole::read(fp);
	    break;
	}
	case WGI_PRIM_FLUSH:
	{
	    WgiPrimFlush::read(fp);
	    break;
	}
	case WGI_PRIM_SIZE:
	default:
	{
	    WGI_ERROR ("WgiSegPrim::read", 
		form("invalid prim type %d (#list,seg,prim=%ld,%ld,%ld)",
			type, Wgi::priv_seglist_count-1,
			s_segment_count, s_segprim_count));
	    return;
	}
    }
}

//------------------------------------------------------------
//
//		WgiPrimRectangle class
//
//------------------------------------------------------------

void
WgiPrimRectangle::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Rect ";
    }
    Wgi::dump (fp, 2, &corner1(table));

    if (Wgi::debug())
    {
	cout << "    Rect fill=" << (int) fillStyle() << 
		" able=" << (int) pickable() << " id=" << (int)
		pickId() << " typ=" << (int) pickType() << endl;
    }

    Wgi::dump (fp, fillStyle());
    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());
}

void
WgiPrimRectangle::read (FILE* fp)
{
    WgiWorldLoc c[2];
    WgiBoolean pick;
    WgiFillStyle fill;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    if (Wgi::debug())
    {
	cout << "    Rect " ;
    }

    Wgi::read (fp, 2, c);
    Wgi::read (fp, &fill);
    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Rect fill=" << (int) fill << " able=" << (int) pick << 
		" id=" << (int) pick_id << " typ=" << (int) pick_type
		<< endl;
    }

    if (Wgi::create()) 
	create (c, fill, pick, pick_def, FALSE);
}

WgiReturnCode 
WgiPrimRectangle::create (const WgiWorldLoc* corners, 
	WgiFillStyle style, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_rectangle ((WLDcrd*) corners, style, 
		pickable, pick_def, draw);
}

//------------------------------------------------------------
//
//		WgiPrimMarker class
//
//------------------------------------------------------------

void
WgiPrimMarker::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Marker loc=" << location(table) << " marktyp=" 
		<< (int) markerType() << " able=" << (int) pickable()
		<< " id=" << (int) pickId() << " typ="
		<< (int) pickType() << endl;
    }
    Wgi::dump (fp, location(table));
    Wgi::dump (fp, markerType());
    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());
}

WgiReturnCode
WgiPrimMarker::create (const WgiWorldLoc& loc, WgiMarkerType type,
	WgiBoolean pickable, const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_marker((WLDcrd*) &loc, type, pickable, pick_def, draw);
}

void
WgiPrimMarker::read (FILE* fp)
{
    WgiWorldLoc loc;
    WgiMarkerType type;
    WgiBoolean pick;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &loc);
    Wgi::read (fp, &type);
    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Marker loc=" << loc << " marktyp=" << (int) type
		<< " able=" << (int) pick << " id=" << (int) pick_id
		<< " typ=" << (int) pick_type << endl;
    }

    if (Wgi::create()) 
	create (loc, type, pick, pick_def, FALSE);
}

//------------------------------------------------------------
//
//		WgiPrimCircle class
//
//------------------------------------------------------------

void
WgiPrimCircle::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Circle center=" << center(table) << " r=" <<
		radius() << " fill=" << (int) fillStyle() << " able="
		<< (int) pickable() << " id=" << (int) pickId() << 
		" typ=" << (int) pickType() << endl;
    }

    Wgi::dump (fp, center(table));
    Wgi::dump (fp, radius());
    Wgi::dump (fp, fillStyle());
    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());
}

WgiReturnCode
WgiPrimCircle::create (const WgiWorldLoc& center, WgiWorldCoord radius,
	WgiFillStyle style, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_circle((WLDcrd*) &center, radius, style, 
		pickable, pick_def, draw);
}

void
WgiPrimCircle::read (FILE* fp)
{
    WgiWorldLoc cen;
    WgiWorldCoord rad;
    WgiBoolean pick;
    WgiFillStyle fill;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &cen);
    Wgi::read (fp, &rad);
    Wgi::read (fp, &fill);
    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    if (Wgi::debug())
    {
	cout << "    Circle center=" << cen << " r=" << rad <<
		" fill=" << (int) fill << " able=" << (int) pick << 
		" id=" << (int) pick_id << " typ=" << (int) pick_type
		<< endl;
    }

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::create()) 
	create (cen, rad, fill, pick, pick_def, FALSE);
}

//------------------------------------------------------------
//
//		WgiPrimText class
//
//------------------------------------------------------------

WgiWorldLoc
WgiPrimText::center (const WgiWorldLoc* table)
{
    WLDcrd cent;
    wgi_calc_text_center_from_base (width(), height(), rotation(),
	reflection(), (WLDcrd*) baseline(table), &cent);
    return cent;
}

void
WgiPrimText::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Text center=" << center(table) << " str=" <<
		string() << " w=" << (int) width() << " h=" << (int)
		height() << " spac=" << spacingFactor() << " rot=" <<
		(int) rotation() << " refl=" << (int) reflection() <<
		" able=" << (int) pickable() << " id=" << (int)
		pickId() << " typ=" << (int) pickType() << endl;
    }

    Wgi::dump (fp, center(table));
    Wgi::dump_str (fp, string());
    Wgi::dump (fp, width());
    Wgi::dump (fp, height());
    Wgi::dump (fp, spacingFactor());
    Wgi::dump (fp, rotation());
    Wgi::dump (fp, reflection());

    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());
}

WgiReturnCode
WgiPrimText::create (const WgiWorldLoc& center, const char* string,
	WgiTextHeight height, WgiTextWidth width, 
	WgiSpaceFactor spacingFactor, 
	WgiRotation rot, WgiReflection refl, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_text((WLDcrd*) &center, (char*) string, 
	height, width, spacingFactor,
	rot, refl, pickable, pick_def, draw);
}

void
WgiPrimText::read (FILE* fp)
{
    WgiWorldLoc cen;
    char buffer[BUFSIZ];
    WgiTextWidth width;
    WgiTextHeight height;
    WgiSpaceFactor spac;
    WgiRotation rot;
    WgiReflection refl;
    WgiBoolean pick;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &cen);
    Wgi::read_str (fp, buffer);

    Wgi::read (fp, &width);
    Wgi::read (fp, &height);
    Wgi::read (fp, &spac);
    Wgi::read (fp, &rot);
    Wgi::read (fp, &refl);
    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Text center=" << cen << " str=" << buffer <<
		" w=" << (int) width << " h=" << (int) height << 
		" spac=" <<  spac << " rot=" << (int) rot << " refl="
		<< (int) refl << " able=" << (int) pick << " id=" <<
		(int) pick_id << " typ=" << (int) pick_type << endl;
    }

    if (Wgi::create()) 
	create (cen, buffer, height, width, spac, rot, refl,
		pick, pick_def, FALSE);
}

//------------------------------------------------------------
//
//		WgiPrimStencil class
//
//------------------------------------------------------------

void
WgiPrimStencil::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Stencil n=" << cardinality() << endl;
    }

    Wgi::dump (fp, cardinality());
    Wgi::dump (fp, cardinality(), coords(table));


    if (Wgi::debug())
    {
	cout << "    Stencil fill=" << (int) fillStyle() << " able=" << (int)
		pickable() << " id=" << (int) pickId() << " typ=" <<
		(int) pickType() << endl;
    }

    Wgi::dump (fp, fillStyle());
    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());
}

WgiReturnCode
WgiPrimStencil::create (long elem_count, const WgiWorldLoc* coords, 
	WgiFillStyle style, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_stencil(elem_count, (WLDcrd*) coords, style,
	pickable, pick_def, draw);
}

void
WgiPrimStencil::read (FILE* fp)
{
    long card;
    WgiBoolean pick;
    WgiFillStyle fill;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &card);
    if (Wgi::debug())
    {
	cout << "    Stencil n=" << card << endl;
    }

    WgiWorldLoc* coords = new WgiWorldLoc[card];
    Wgi::read (fp, card, coords);

    Wgi::read (fp, &fill);
    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Stencil fill=" << (int) fill << " able=" <<
		(int) pick << " id=" << (int) pick_id << " typ=" <<
		(int) pick_type << endl;
    }

    if (Wgi::create()) 
	create (card, (WLDcrd*) coords, fill, pick, pick_def, FALSE);

#ifdef SOLARIS
    delete[] coords;
#else
    delete[card] coords;
#endif
}

//------------------------------------------------------------
//
//		WgiPrimPoints class
//
//------------------------------------------------------------

void
WgiPrimPoints::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Points x=" << x_cardinality() << " y=" <<
		y_cardinality() << endl;
    }

    Wgi::dump (fp, x_cardinality());
    Wgi::dump (fp, y_cardinality());
    Wgi::dump (fp, cardinality(), coords(table));

    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());

    if (Wgi::debug())
    {
	cout << "    Points able=" << (int) pickable() << " id=" << (int)
		pickId() << " typ=" << (int) pickType() << endl;
    }
}

WgiReturnCode
WgiPrimPoints::create (long x_card, long y_card,
	const WgiWorldLoc* coords, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_points(x_card, y_card, (WLDcrd*) coords,
	pickable, pick_def, draw);
}

void
WgiPrimPoints::read (FILE* fp)
{
    long x_card, y_card;
    WgiBoolean pick;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &x_card);
    Wgi::read (fp, &y_card);

    if (Wgi::debug())
    {
	cout << "    Points x=" << x_card << " y=" << y_card << endl;
    }

    long card = x_card + y_card;
    WgiWorldLoc* coords = new WgiWorldLoc[card];
    Wgi::read (fp, card, coords);

    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Points able=" << (int) pick << " id=" << (int)
		pick_id << " typ=" << (int) pick_type << endl;
    }

    if (Wgi::create()) 
	create (x_card, y_card, coords, pick, pick_def, FALSE);

#ifdef SOLARIS
    delete[] coords;
#else
    delete[card] coords;
#endif
}

//------------------------------------------------------------
//
//		WgiPrimPolyLine class
//
//------------------------------------------------------------

void
WgiPrimPolyLine::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    PolyLine n=" << cardinality() << endl;
    }
    Wgi::dump (fp, cardinality());
    Wgi::dump (fp, cardinality(), coords(table));

    Wgi::dump (fp, width());
    Wgi::dump (fp, termination());

    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());

    if (Wgi::debug())
    {
	cout << "    PolyLine w=" << (int) width() << " term=" << (int)
		termination() << " able=" << (int) pickable() << 
		" id=" << (int) pickId() << " typ=" << (int)
		pickType() << endl;
    }
}

WgiReturnCode
WgiPrimPolyLine::create (long card, const WgiWorldLoc* coords, 
	WgiLineWidth width, WgiTermination term,
	WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_polyln(card, (WLDcrd*) coords, width, term,
	pickable, pick_def, draw);
}

void
WgiPrimPolyLine::read (FILE* fp)
{
    long card;
    WgiLineWidth width;
    WgiTermination term;
    WgiBoolean pick;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &card);

    if (Wgi::debug())
    {
	cout << "    PolyLine n=" << card << endl;
    }

    WgiWorldLoc* coords = new WgiWorldLoc[card];
    Wgi::read (fp, card, coords);

    Wgi::read (fp, &width);
    Wgi::read (fp, &term);

    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    if (Wgi::debug())
    {
	cout << "    PolyLine w=" << (int) width << " term=" <<
		(int) term << " able=" << (int) pick << " id=" <<
		(int) pick_id << " typ=" << (int) pick_type << endl;
    }

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::create()) 
	create (card, coords, width, term, pick, pick_def, FALSE);

#ifdef SOLARIS
    delete[] coords;
#else
    delete[card] coords;
#endif
}

//------------------------------------------------------------
//
//		WgiPrimZebra class
//
//------------------------------------------------------------

void
WgiPrimZebra::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Zebra n=" << cardinality() << endl;
    }
    Wgi::dump (fp, cardinality());
    Wgi::dump (fp, cardinality(), coords(table));

    Wgi::dump (fp, width());
    Wgi::dump (fp, termination());
    Wgi::dump (fp, dashStyle());

    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());

    if (Wgi::debug())
    {
	cout << "    Zebra w=" << (int) width()
		<< " term=" << (int) termination() 
		<< " dash=" << (int) dashStyle() 
		<< " able=" << (int) pickable() << 
		" id=" << (int) pickId() << " typ=" << (int)
		pickType() << endl;
    }
}

WgiReturnCode
WgiPrimZebra::create (long card, const WgiWorldLoc* coords, 
	WgiLineWidth width, WgiTermination term,
	WgiDashStyle dash, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_zebra(card, (WLDcrd*) coords, width, term, dash,
	pickable, pick_def, draw);
}

void
WgiPrimZebra::read (FILE* fp)
{
    long card;
    WgiLineWidth width;
    WgiTermination term;
    WgiDashStyle dash;
    WgiBoolean pick;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &card);

    if (Wgi::debug())
    {
	cout << "    Zebra n=" << card << endl;
    }

    WgiWorldLoc* coords = new WgiWorldLoc[card];
    Wgi::read (fp, card, coords);

    Wgi::read (fp, &width);
    Wgi::read (fp, &term);
    Wgi::read (fp, &dash);

    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    if (Wgi::debug())
    {
	cout << "    Zebra w=" << (int) width
		<< " term=" << (int) term
		<< " dash=" << (int) dash
		<< " able=" << (int) pick << " id=" <<
		(int) pick_id << " typ=" << (int) pick_type << endl;
    }

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::create()) 
	create (card, coords, width, term, dash, pick, pick_def, FALSE);

#ifdef SOLARIS
    delete[] coords;
#else
    delete[card] coords;
#endif
}

/* static*/ WgiReturnCode
WgiPrimZebra::setDashStyle (WgiDashStyle style, long dash_count,
	Single* dash_length_array)
{
    return wgi_set_dash_style (style, dash_count, dash_length_array);
}

//------------------------------------------------------------
//
//		WgiPrimPolygon class
//
//------------------------------------------------------------

void
WgiPrimPolygon::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Polygon n=" << cardinality() << endl;
    }

    Wgi::dump (fp, cardinality());
    Wgi::dump (fp, cardinality(), coords(table));

    Wgi::dump (fp, fillStyle());

    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());


    if (Wgi::debug())
    {
	cout << "    Polygon fill=" << (int) fillStyle() << " able=" << (int)
		pickable() << " id=" << (int) pickId() << " typ=" <<
		(int) pickType() << endl;
    }
}

WgiReturnCode
WgiPrimPolygon::create (long card, const WgiWorldLoc* coords, 
	WgiFillStyle style,
	WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_polygon(card, (WLDcrd*) coords, style,
	pickable, pick_def, draw);
}

void
WgiPrimPolygon::read (FILE* fp)
{
    long card;
    WgiBoolean pick;
    WgiFillStyle fill;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &card);

    if (Wgi::debug())
    {
	cout << "    Polygon n=" << card << endl;
    }

    WgiWorldLoc* coords = new WgiWorldLoc[card];
    Wgi::read (fp, card, coords);


    Wgi::read (fp, &fill);
    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);

    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Polygon fill= " << (int) fill << " able=" <<
		(int) pick << " id=" << (int) pick_id << " typ=" <<
		(int) pick_type << endl;
    }

    if (Wgi::create()) 
	create (card, coords, fill, pick, pick_def, FALSE);

#ifdef SOLARIS
    delete[] coords;
#else
    delete[card] coords;
#endif
}

//------------------------------------------------------------
//
//		WgiPrimHole class
//
//------------------------------------------------------------

void
WgiPrimHole::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Hole n=" << cardinality() << endl;
    }

    Wgi::dump (fp, cardinality());
    Wgi::dump (fp, cardinality(), coords(table));
    Wgi::dump (fp, fillStyle());

    Wgi::dump (fp, pickable());
    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());

    if (Wgi::debug())
    {
	cout << "    Hole fill=" << (int) fillStyle() << " able=" << (int)
		pickable() << " id=" << (int) pickId() << " typ=" <<
		(int) pickType() << endl;
    }
}

WgiReturnCode
WgiPrimHole::create (long card, const WgiWorldLoc* coords, 
	WgiFillStyle fillStyle, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_hole(card, (WLDcrd*) coords, fillStyle,
	pickable, pick_def, draw);
}

void
WgiPrimHole::read (FILE* fp)
{
    long card;
    WgiBoolean pick;
    WgiFillStyle fill;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &card);

    if (Wgi::debug())
    {
	cout << "    Hole n=" << card << endl;
    }

    WgiWorldLoc* coords = new WgiWorldLoc[card];
    Wgi::read (fp, card, coords);

    Wgi::read (fp, &fill);
    Wgi::read (fp, &pick);
    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);
    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Hole fill= " << (int) fill << " able=" <<
		(int) pick << " id=" << (int) pick_id << " typ=" <<
		(int) pick_type << endl;
    }

    if (Wgi::create()) 
	create (card, coords, fill, pick, pick_def, FALSE);

#ifdef SOLARIS
    delete[] coords;
#else
    delete[card] coords;
#endif
}

//------------------------------------------------------------
//
//		WgiPrimFlush class
//
//------------------------------------------------------------

void
WgiPrimFlush::dump (FILE* fp, const WLDcrd* table)
{
    if (Wgi::debug())
    {
	cout << "    Flush n=" << cardinality() << endl;
    }

    Wgi::dump (fp, cardinality());
    Wgi::dump (fp, cardinality(), coords(table));

    Wgi::dump (fp, pickId());
    Wgi::dump (fp, pickType());

    if (Wgi::debug())
    {
	cout << "    Flush id=" << (int) pickId() << " typ=" << (int)
		pickType() << endl;
    }
}

WgiReturnCode
WgiPrimFlush::create (long card, const WgiWorldLoc* coords, 
	const WgiPickDef& pick_def, WgiBoolean draw)
{
    return wgi_flush(card, (WLDcrd*) coords, pick_def, draw);
}

void
WgiPrimFlush::read (FILE* fp)
{
    long card;
    WgiPickId  pick_id;
    WgiPickType pick_type;

    Wgi::read (fp, &card);

    if (Wgi::debug())
    {
	cout << "    Flush n=" << card << endl;
    }

    WgiWorldLoc* coords = new WgiWorldLoc[card];
    Wgi::read (fp, card, coords);

    Wgi::read (fp, &pick_id);
    Wgi::read (fp, &pick_type);
    Pickdef pick_def;
    pick_def.iden = pick_id;
    pick_def.type = pick_type;

    if (Wgi::debug())
    {
	cout << "    Flush id=" << (int) pick_id << " typ=" << (int)
		pick_type << endl;
    }

    if (Wgi::create()) 
	create (card, coords, pick_def, FALSE);

#ifdef SOLARIS
    delete[] coords;
#else
    delete[card] coords;
#endif
}

//------------------------------------------------------------
//
//		Wgi class
//
//------------------------------------------------------------
