#include "wgiStruct.h"

extern unsigned long mask;
static Viewport iwgi_viewport_data[WGI_VIEWPORT_MAX];

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--            Select A Viewport For Transforms Or For Drawing            --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_viewport_select(iden, draw, vmap)
long iden;
long draw;
Mapping *vmap;
{
	long rc;
	Viewport *vp;
	static long last = WGI_VIEWPORT_MAX;
	static long drew = WGI_VIEWPORT_MAX;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		If We Are Selecting A Different Viewport Or If The Viewport
		Is The Same As The Last One But It Has Changed Then We Must
		Reset The Mapper
	*/

	if((iden != last) || (vp->vsync))
	{
		/*
			Must Save The Viewport We Are Currently Mapped For
		*/

		last = iden;

		/*
			Tell The Mapper About This Viewport
		*/

		if((rc = iwgi_posn(vp->vxpos, vp->vypos)) != WGI_SUCCESS)
			return (rc);

		if((rc = iwgi_size(vp->vwide, vp->vhigh)) != WGI_SUCCESS)
			return (rc);

		if((rc = iwgi_view(vp->vview)) != WGI_SUCCESS)
			return (rc);

		/*
			Everything This Flag Triggers Has Been Checked So It Is
			Safe To Unset It
		*/

		vp->vsync = FALSE;

		vmap->sync = 0;
		vmap->view = iden;
		vmap->wide = vp->vwide;
		vmap->high = vp->vhigh;
		vmap->xtnt = vp->vview;
	}

	/*
		Prepare The Device And Drawing Routines To Draw In This Viewport
	*/

	if(draw)
	{
		/*
			We Must Remap The Internal Text Mapping Buffer If The Viewport
			Has Changed Or The Buffer Is Out Of Sync
		*/

		if((vp->vdraw) || (vp->vtext.sync != wld_ttabl.sync))
		{
			if((rc = iwgi_viewport_mappings(&wld_ttabl, &vp->vtext)) != WGI_SUCCESS)
				return (rc);

			/*
				Set The Sync Flags The Same
			*/

			vp->vtext.sync = wld_ttabl.sync;
		}

		/*
			We Must Remap The Internal Line Mapping Buffer If The Viewport
			Has Changed Or The Buffer Is Out Of Sync
		*/

		if((vp->vdraw) || (vp->vline.sync != wld_ltabl.sync))
		{
			if((rc = iwgi_viewport_mappings(&wld_ltabl, &vp->vline)) != WGI_SUCCESS)
				return (rc);

			/*
				Set The Sync Flags The Same
			*/

			vp->vline.sync = wld_ltabl.sync;
		}

		/*
			If We Are Selecting A Different Viewport Or If The Viewport
			Is The Same As The Last One But It Has Changed Then We Must
			Reset The Segment Selection Mask And The Device Viewport
		*/

		if((iden != drew) || (vp->vdraw))
		{
			/*
				Must Save The Viewport Which We Last Set Up For Draw
			*/

			drew = iden;

			/*
				Set The Segment Viewport Selection Mask
			*/

			mask = 1 << vp->viden;

			/*
				Select The Device Viewport
			*/

			if((rc = iwgi_dev_viewport_select(vp)) != WGI_SUCCESS)
				return (rc);
		}

		/*
			Everything This Flag Triggers Has Been Checked So It Is
			Safe To Unset It
		*/

		vp->vdraw = FALSE;
	}

	if(vp->vclip)
	{
		iwgi_clip_view(vp->vonly);
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                      Delete A Viewport Definition                     --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_viewport_delete(iden)
long iden;
{
	long rc;
	Viewport *vp;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		Delete The Internal Text Mapping Buffer
	*/

	if((rc = wgi_tbuff_free(&vp->vtext)) != WGI_SUCCESS)
		return (rc);

	/*
		Delete The Internal Line Mapping Buffer
	*/

	if((rc = wgi_tbuff_free(&vp->vline)) != WGI_SUCCESS)
		return (rc);

	/*
		Delete The Device Part Of The Viewport
	*/

	if((rc = iwgi_dev_viewport_delete(vp)) != WGI_SUCCESS)
		return (rc);

	/*
		Only Set Used Flag If Everything Is Ok So Far
	*/

	vp->vused = FALSE;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--     Set The Window Extents To Be Mapped Onto A Specified Viewport     --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_viewport_view(iden, view)
long iden;
WLDcrd *view;
{
	Viewport *vp;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		Set The Sync Flag So The Viewport Selection Routine
		Knows We Have Changed
	*/

	vp->vsync = TRUE;
	vp->vdraw = TRUE;

	/*
		Make Sure Viewports Extents Are In The Right State Of Mind
	*/

	vp->vview.xmin = MIN(view[0].x, view[1].x);
	vp->vview.ymin = MIN(view[0].y, view[1].y);
	vp->vview.xmax = MAX(view[0].x, view[1].x);
	vp->vview.ymax = MAX(view[0].y, view[1].y);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--         Create A Viewport Definition And Pass In It`s Geometry        --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_viewport_create(iden, xpos, ypos, wide, high)
long iden;
long xpos;
long ypos;
long wide;
long high;
{
	long rc;
	Viewport *vp;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		If It's Already Used We Blew It!
	*/

	if(vp->vused)
	{
		WOOPS("Viewport %d is already defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		Turn On The Sync Flag So The Viewport Selection Routine Sends The
		The Information To The Mapper
	*/

	vp->vsync = TRUE;
	vp->vdraw = TRUE;

	/*
		Remember The Position And Size Of The Viewport
	*/

	vp->vxpos = xpos;
	vp->vypos = ypos;
	vp->vwide = wide;
	vp->vhigh = high;

	/*
		Initialize The Internal Text Mapping Buffer
	*/

	if((rc = wgi_tbuff_init(&vp->vtext)) != WGI_SUCCESS)
		return (rc);

	/*
		Initialize The Internal Line Mapping Buffer
	*/

	if((rc = wgi_tbuff_init(&vp->vline)) != WGI_SUCCESS)
		return (rc);

	/*
		Save The Viewport Id For The Segment Selection Mask
	*/

	vp->viden = iden;

	/*
		Initialize The Viewports View To All Zero So Nothing
		Can Be Drawn In It
	*/

	vp->vview.xmin = 0.0;
	vp->vview.xmax = 0.0;
	vp->vview.ymin = 0.0;
	vp->vview.ymax = 0.0;

	/*
		Create The Device Dependant Part Of The Viewport
	*/

	if((rc = iwgi_dev_viewport_create(vp)) != WGI_SUCCESS)
		return (rc);

	/*
		Only Set Used Flag If Everything Is Ok So Far
	*/

	vp->vused = TRUE;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                  Zoom Or Pan On A Specified Viewport                  --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_viewport_zoom(iden, wldp, mode, d_frac)
long iden;
WLDcrd *wldp;
long mode;
double d_frac;	/* passing Single parameters from Sun C++ causes problems */
{
	long rc;
	Single xdel;
	Single ydel;

	Viewport *vp;

	Single frac = (Single) d_frac;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		Select The Specified Viewport
	*/

	if((rc = wgi_viewport(iden)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The X And Y Delta For This Viewport
	*/

	xdel = (vp->vview.xmax - vp->vview.xmin);
	ydel = (vp->vview.ymax - vp->vview.ymin);

	switch(mode)
	{
		case WGI_ZOOM_IN:

			/*
				Calculate How Much To Zoom By Adding Half Of
				A Specified Fraction Of This Viewports Delta
				To The Passed In World Coordiante
			*/

			xdel = (xdel * (1.0 - frac)) / 2.0;
			ydel = (ydel * (1.0 - frac)) / 2.0;
			vp->vview.xmin = wldp->x - xdel;
			vp->vview.xmax = wldp->x + xdel;
			vp->vview.ymin = wldp->y - ydel;
			vp->vview.ymax = wldp->y + ydel;

		break;

		case WGI_ZOOM_OUT:

			/*
				Calculate How Much To Zoom By Adding Half Of
				A Specified Fraction Of This Viewports Delta
				To The Passed In World Coordiante
			*/

			xdel = (xdel / (1.0 - frac)) / 2.0;
			ydel = (ydel / (1.0 - frac)) / 2.0;
			vp->vview.xmin = wldp->x - xdel;
			vp->vview.xmax = wldp->x + xdel;
			vp->vview.ymin = wldp->y - ydel;
			vp->vview.ymax = wldp->y + ydel;

		break;

		case WGI_ZOOM_UP:

			/*
				Move The Viewport View Up By A Specified Fraction
				Of The Viewports Y Delta
			*/

			vp->vview.ymin += (ydel * frac);
			vp->vview.ymax += (ydel * frac);

		break;

		case WGI_ZOOM_DOWN:

			/*
				Move The Viewport View Down By A Specified Fraction
				Of The Viewports Y Delta
			*/

			vp->vview.ymin -= (ydel * frac);
			vp->vview.ymax -= (ydel * frac);

		break;

		case WGI_ZOOM_LEFT:

			/*
				Move The Viewport View Left By A Specified Fraction
				Of The Viewports X Delta
			*/

			vp->vview.xmin -= (xdel * frac);
			vp->vview.xmax -= (xdel * frac);
		break;

		case WGI_ZOOM_RIGHT:

			/*
				Move The Viewport View Right By A Specified Fraction
				Of The Viewports X Delta
			*/

			vp->vview.xmin += (xdel * frac);
			vp->vview.xmax += (xdel * frac);

		break;

		default:

			/*
				We Goofed And Sent An Invalid Zoom Mode
			*/

			WOOPS("Viewport %d got invalid zoom mode", iden);
			return (WGI_PROBLEM);
	}

	/*
		Turn On The Sync Flag So The Viewport Selection Routine Sends The
		The Information To The Mapper
	*/

	vp->vsync = TRUE;
	vp->vdraw = TRUE;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--      Calculate What The View Window Would Be If We Would              --*/
/*--      Zoom Or Pan On A Specified Viewport                              --*/
/*--                                                                       --*/
/*--     Producer: Enno Derksen    Inspected: 01/94     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_viewport_zoom_query (iden, wldp, mode, d_frac, view_win)
long iden;
WLDcrd *wldp;
long mode;
double d_frac;	/* passing Single parameters from Sun C++ causes problems */
Extent *view_win;
{
	long rc;
	Single xdel;
	Single ydel;

	Viewport *vp;
	Single frac = (Single) d_frac;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		Select The Specified Viewport
	*/

	if((rc = wgi_viewport(iden)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The X And Y Delta For This Viewport
	*/

	*view_win = vp->vview;
	xdel = (vp->vview.xmax - vp->vview.xmin);
	ydel = (vp->vview.ymax - vp->vview.ymin);

	switch(mode)
	{
		case WGI_ZOOM_IN:

			/*
				Calculate How Much To Zoom By Adding Half Of
				A Specified Fraction Of This Viewports Delta
				To The Passed In World Coordiante
			*/

			xdel = (xdel * (1.0 - frac)) / 2.0;
			ydel = (ydel * (1.0 - frac)) / 2.0;
			view_win->xmin = wldp->x - xdel;
			view_win->xmax = wldp->x + xdel;
			view_win->ymin = wldp->y - ydel;
			view_win->ymax = wldp->y + ydel;

		break;

		case WGI_ZOOM_OUT:

			/*
				Calculate How Much To Zoom By Adding Half Of
				A Specified Fraction Of This Viewports Delta
				To The Passed In World Coordiante
			*/

			xdel = (xdel / (1.0 - frac)) / 2.0;
			ydel = (ydel / (1.0 - frac)) / 2.0;
			view_win->xmin = wldp->x - xdel;
			view_win->xmax = wldp->x + xdel;
			view_win->ymin = wldp->y - ydel;
			view_win->ymax = wldp->y + ydel;

		break;

		case WGI_ZOOM_UP:

			/*
				Move The Viewport View Up By A Specified Fraction
				Of The Viewports Y Delta
			*/

			view_win->ymin += (ydel * frac);
			view_win->ymax += (ydel * frac);

		break;

		case WGI_ZOOM_DOWN:

			/*
				Move The Viewport View Down By A Specified Fraction
				Of The Viewports Y Delta
			*/

			view_win->ymin -= (ydel * frac);
			view_win->ymax -= (ydel * frac);

		break;

		case WGI_ZOOM_LEFT:

			/*
				Move The Viewport View Left By A Specified Fraction
				Of The Viewports X Delta
			*/

			view_win->xmin -= (xdel * frac);
			view_win->xmax -= (xdel * frac);
		break;

		case WGI_ZOOM_RIGHT:

			/*
				Move The Viewport View Right By A Specified Fraction
				Of The Viewports X Delta
			*/

			view_win->xmin += (xdel * frac);
			view_win->xmax += (xdel * frac);

		break;

		default:

			/*
				We Goofed And Sent An Invalid Zoom Mode
			*/

			WOOPS("Viewport %d got invalid zoom mode", iden);
			return (WGI_PROBLEM);
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Synchronize And Map Either The Viewports Line Or Text Buffer With The --*/
/*--                        Global Line Or Text Table                      --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_viewport_mappings(tabl, tbuf)
Table *tabl;
Tbuff *tbuf;
{
	long rc;

	/*
		Match The Buffer With The Full Size Of The Table So The
		Gyrations Of The Used Mark Do Not Make The Tbuff Change
		Size So Often
	*/

	if((rc = wgi_tbuff_match(tbuf, tabl->nelm * tabl->elsz)) != WGI_SUCCESS)
		return (rc);

	/*
		Only Scale The Number Of Used Elements
	*/

	if((rc = wgi_scaler(tabl->used, tabl->data, tbuf->data)) != WGI_SUCCESS)
		return (rc);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  A Utility Routine To Map From Screen Or Canvas Coordinates To World  --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_scrwld(iden, vdcp, wldp)
long iden;
VDCcrd vdcp;
WLDcrd *wldp;
{
	long rc;
	Viewport *vp;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		Select The Specified Viewport
	*/

	if((rc = wgi_viewport(iden)) != WGI_SUCCESS)
		return (rc);

	/*
		Subtract The Viewports Base Coordinate From The Screen
		Coordinate To Get The Real Viewport Coordinate
	*/

	vdcp.x = vdcp.x - vp->vxpos;
	vdcp.y = vdcp.y - vp->vypos;

	/*
		Map From Viewport Coordinate To World Coordinate
	*/

	if((rc = wgi_vdcwld(1, &vdcp, wldp)) != WGI_SUCCESS)
		return (rc);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--     A Utility Routine To Pass Viewport Extents Back To The Caller     --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 04/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

wgi_viewport_query(iden, view, posn)
long iden;
Extent *view;
Extent *posn;
{
	Viewport *vp;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	/*
		Pass Back The Viewport Extent Information
	*/

	posn->xmin = vp->vxpos;
	posn->xmax = vp->vxpos + vp->vwide - 1;
	posn->ymin = vp->vypos;
	posn->ymax = vp->vypos + vp->vhigh - 1;

	view->xmin = vp->vview.xmin;
	view->xmax = vp->vview.xmax;
	view->ymin = vp->vview.ymin;
	view->ymax = vp->vview.ymax;

	return (WGI_SUCCESS);
}

long wgi_viewport_expose(iden, data)
long iden;
char *data;
{
	long rc;
	Viewport *vp;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	vp->vdraw = TRUE;

	if((rc = iwgi_dev_viewport_expose(vp, data)) != WGI_SUCCESS)
		return (rc);

	return (WGI_SUCCESS);
}

wgi_viewport_subset(iden, mode, only)
long iden;
long mode;
WLDcrd *only;
{
	long rc;
	Viewport *vp;

	/*
		Make Sure Viewport Is Valid
	*/

	if((iden < 0) || (iden >= WGI_VIEWPORT_MAX))
	{
		WOOPS("Viewport %d is invalid", iden);
		return (WGI_PROBLEM);
	}

	/*
		Point At Correct Viewport Structure
	*/

	vp = iwgi_viewport_data + iden;

	/*
		Did  We Goof And Try To Use An Un-Defined Viewport
	*/

	if(vp->vused == FALSE)
	{
		WOOPS("Viewport %d is not defined", iden);
		return (WGI_PROBLEM);
	}

	vp->vsync = TRUE;
	vp->vdraw = TRUE;

	/*
		Make Sure Viewports Extents Are In The Right State Of Mind
	*/

	if(vp->vclip = mode)
	{
		vp->vonly.xmin = MIN(only[0].x, only[1].x);
		vp->vonly.ymin = MIN(only[0].y, only[1].y);
		vp->vonly.xmax = MAX(only[0].x, only[1].x);
		vp->vonly.ymax = MAX(only[0].y, only[1].y);
	}

	return (WGI_SUCCESS);
}
