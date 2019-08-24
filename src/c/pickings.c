#include <math.h>

#include "wgiStruct.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Wgi Pick Support --- Pick Defined By Primitives That Touch A Pick Box --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static Extent pick_rect;	/* Rectangle Used In Some Picking Operations */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Pick A Polyline Accounting For The Width Of The Line In The Pick Rect --*/
/*-- Box. Uses iwgi_pick_line() Routine To Pick Line Sub-Segments.         --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_pick_polyline(prim, pbox)
Segprim *prim;
Extent pbox;
{
	long i;
	Single radi;
	WLDcrd *wldw = (WLDcrd *)wld_ltabl.data + (prim->wide);
	WLDcrd *wldp = (WLDcrd *)wld_table->data + (prim->indx + 1);

	/*
		Add The Line Radius To The Pick Box To Account For Line Widths
	*/

	radi = (wldw->x / 2.0);

	pick_rect.xmin = pbox.xmin - radi;
	pick_rect.xmax = pbox.xmax + radi;
	pick_rect.ymin = pbox.ymin - radi;
	pick_rect.ymax = pbox.ymax + radi;

	/*
		Loop Through Each Line Segment
	*/

	for(i = 1; i < prim->nelm; i++, wldp++)
	{
		if(iwgi_pick_line((wldp-1)->x, (wldp-1)->y, (wldp)->x, (wldp)->y))
			return (TRUE);
	}

	/*
		No Matching Line Segments Found
	*/

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Re-Calculate The Picks Delta To Check If The Pick Was Within The Area --*/
/*-- Of The Circle If Filled Or Within The Delta Of The Edge Of The Circle --*/
/*-- If It Is Not Filled.                                                  --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_pick_circle(prim, pbox)
Segprim *prim;
Extent pbox;
{
	WLDcrd pick;
	Single slop;
	Single radi;
	Single dist;
	WLDcrd *wldw = (WLDcrd *)wld_ltabl.data + prim->wide;
	WLDcrd *wldp = (WLDcrd *)wld_table->data + prim->indx;

	/*
		Re-Calculate The Pick Delta
	*/

	slop = ((pbox.xmax - pbox.xmin)/2.0);
	pick.x = pbox.xmin + slop;
	pick.y = pbox.ymin + slop;

	/*
		Calculate The Circles Radius And The Pick Distance From
		The Center Of The Circle
	*/

	radi = wldw->x;
	dist = hypot(fabs(pick.x - wldp[0].x), fabs(pick.y - wldp[0].y));

	/*
		If The Distance From The Center Is Less Than The Radius
		Plus The Delta Then We Are Inside Circle.  If Fill Flag
		Is Off We Must Be Within The Delta From The Edge Of The
		Circle.
	*/

	if((dist <= (radi+slop)) && (prim->user || ((radi-slop) <= dist)))
		return (TRUE);

	/*
		Circle Pick Failed
	*/

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The Definition Of Text Pick Is A Pick Within The Extent So The Extent --*/
/*-- Check That Passed And Called This Routine Confirmes The Pick.         --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_pick_text(prim, pbox)
Segprim *prim;
Extent pbox;
{
	/*
		The Extent Check Already Confirms That The Pick Box
		Intersects Text
	*/

	return (TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                 Flush Is Not Pickable By Definition                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_pick_flush(prim, pbox)
Segprim *prim;
Extent pbox;
{
	/*
		Flushes Dont Pick
	*/

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--              Markers Are Not Pickable By Definition                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_pick_marker(prim, pbox)
Segprim *prim;
Extent pbox;
{
	/*
		Markers Dont Pick
	*/

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--               Points Are Not Pickable By Definition                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_pick_points(prim, pbox)
Segprim *prim;
Extent pbox;
{
	/*
		Points Dont Pick
	*/

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Pick A Polygon By Trying To Pick The Edges First - If None Match Then --*/
/*-- If The Polygon Is Filled Solid We Use The Inside Polygon Test Routine --*/
/*-- iwgi_isin() On Each Edge And Check The Inside Flag "poly_isin" At End --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long last_dirc = 0;
static long poly_isin = 0;

static long wgi_pick_polygon(prim, pbox)
Segprim *prim;
Extent pbox;
{
	long i;
	WLDcrd *wldp;
	Single xval, yval, slop;

	pick_rect = pbox;

	/*
		Loop Through Each Line Segment And Try To Pick An Edge
	*/

	wldp = (WLDcrd *)wld_table->data + (prim->indx + 1);

	for(i = 1; i < prim->nelm; i++, wldp++)
	{
		if(iwgi_pick_line((wldp-1)->x, (wldp-1)->y, (wldp)->x, (wldp)->y))
			return (TRUE);
	}

	/*
		No Line Segments Found So Check For Inside Of Filled Polygon
	*/

	if(prim->user == WGI_FILL_FILL)
	{
		/*
			Re-Calculate Pick Point From Box And Use Pick Point For
			Inside Check. Delta Is Not Needed Because If We Were In
			The Delta Range We Would Have Picked Up An Edge By Now.
		*/

		slop = ((pbox.xmax - pbox.xmin)/2.0);
		xval = pbox.xmin + slop;
		yval = pbox.ymin + slop;

		wldp = (WLDcrd *)wld_table->data + (prim->indx + 1);

		/*
			Initialize Global Flag And Direction Variables
		*/

		poly_isin = 0;
		last_dirc = 0;

		/*
			Loop Through Each Line Segment And Test
		*/

		for(i = 1; i < prim->nelm; i++, wldp++)
		{
			iwgi_isin(xval, yval, (wldp-1)->x, (wldp-1)->y, (wldp)->x, (wldp)->y);
		}

		/*
			If The Polygon Inside Flag Is Set We Have A Pick
		*/

		if(poly_isin)
			return (TRUE);
	}

	/*
		Polygon Was Either Not Filled Solid Or Point Was Not Inside
	*/

	return (FALSE);

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- A Pick On A Rectangle Is Trivial If The Rectangle Is Filled. If It Is --*/
/*-- Not Filled We Check For A Pick On Each Edge                           --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_pick_rectangle(prim, pbox)
Segprim *prim;
Extent pbox;
{
	WLDcrd *wldp = (WLDcrd *)wld_table->data + prim->indx;

	if(prim->user)
	{
		/*
			The Extent Check Already Confirms That The Pick Box
			Intersects A Filled Rectangle
		*/

		return (TRUE);
	}

	/*
		Check If Each Individual Line Intersects Pick Box
	*/

	pick_rect = pbox;

	if(iwgi_pick_line(wldp[0].x, wldp[1].y, wldp[1].x, wldp[1].y))
		return (TRUE);

	if(iwgi_pick_line(wldp[1].x, wldp[1].y, wldp[1].x, wldp[0].y))
		return (TRUE);

	if(iwgi_pick_line(wldp[1].x, wldp[0].y, wldp[0].x, wldp[0].y))
		return (TRUE);

	if(iwgi_pick_line(wldp[0].x, wldp[0].y, wldp[0].x, wldp[1].y))
		return (TRUE);

	/*
		No Matching Line Segments Found
	*/

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- This Is A Support Routine For The Line Segment Picking Code That Does --*/
/*-- Endpoint Encoding For The iwgi_pick_line() Routine.                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long pick_code(x, y)
Single x, y;
{
	return(	((pick_rect.ymax-y) < (0) ? (1) : (0))|
		((y-pick_rect.ymin) < (0) ? (2) : (0))|
		((pick_rect.xmax-x) < (0) ? (4) : (0))|
		((x-pick_rect.xmin) < (0) ? (8) : (0)));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- This Is A Modified Version Of A Line Clipper That Is Used To Check If --*/
/*-- A Line Intersects The Pick Box ---- Very Fast And Simple ---- Yow !!! --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_pick_line(x1, y1, x2, y2)
Single x1, y1, x2, y2;
{
	Single x0, y0;
	long o1, o2, o3;

	o1 = pick_code(x1, y1);
	o2 = pick_code(x2, y2);

	while((o1 != 0)||(o2 != 0))
	{
		if((o1 & o2) != 0) return(FALSE);

		if((o3 = o1) == 0) o3 = o2;

		if(o3 & 1)
		{
			x0 = x1+(x2-x1)*(pick_rect.ymax-y1)/(y2-y1);
			y0 = pick_rect.ymax;
		}
		else if(o3 & 2)
		{
			x0 = x1+(x2-x1)*(pick_rect.ymin-y1)/(y2-y1);
			y0 = pick_rect.ymin;
		}
		else if(o3 & 4)
		{
			y0 = y1+(y2-y1)*(pick_rect.xmax-x1)/(x2-x1);
			x0 = pick_rect.xmax;
		}
		else if(o3 & 8)
		{
			y0 = y1+(y2-y1)*(pick_rect.xmin-x1)/(x2-x1);
			x0 = pick_rect.xmin;
		}
		if(o3 == o1)
		{
			x1 = x0;
			y1 = y0;
			o1 = pick_code(x1, y1);
		}
		else
		{
			x2 = x0;
			y2 = y0;
			o2 = pick_code(x2, y2);
		}
	}

	return (TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- This Is A Support Routine To Check For Insidedness (Can You Spell It) --*/
/*-- Of A Point In A Polygon.  Called With Each Edge Of A Polygon And Then --*/
/*-- "poly_isin" Flag Is Checked When Done. Dont Ask Me How It Works.      --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define R	1	/* Right */
#define L	2	/* Guess */

static long iwgi_isin(x0, y0, x1, y1, x2, y2)
Single x0, y0, x1, y1, x2, y2;
{
	if (x1 == x2)
		return (FALSE);

	if (x1 < x2)
	{
		if ((x0 < x1) || (x0 >= x2) || ((last_dirc == L) && (x0 == x1)))
		{
			last_dirc = R;
			return (FALSE);
		}
		else
		{
			last_dirc = R;
		}

	}
	else if ((x0 > x1) || (x0 <= x2) || ((last_dirc == R) && (x0 == x1)))
	{
		last_dirc = L;
		return (FALSE);
	}
	else
	{
		last_dirc = L;
	}

	if (y0 > MAX(y1, y2))
	{
		return (FALSE);
	}

	if (y1 < y2)
	{
		if ((y0 < y1) || (fabs((y0 - y1) * (x2 - x1)) <= fabs((x0 - x1) * (y2 - y1))))
			poly_isin = poly_isin ^ 1;
	}
	else
	{
		if ((y0 < y2) || (fabs((y0 - y2) * (x2 - x1)) <= fabs((x0 - x2) * (y2 - y1))))
			poly_isin = poly_isin ^ 1;
	}

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--       A Pick Dispatch Table Indexed By Primitive Type Value           --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static Function dispatch_pick[] =
{
	wgi_pick_polyline,	/* WGI_PRIM_POLYLINE */
	wgi_pick_circle,	/* WGI_PRIM_CIRCLE */
	wgi_pick_text,		/* WGI_PRIM_TEXT */
	wgi_pick_marker,	/* WGI_PRIM_MARKER */
	wgi_pick_polygon,	/* WGI_PRIM_POLYGON */
	wgi_pick_rectangle,	/* WGI_PRIM_RECTANGLE */
	wgi_pick_points,	/* WGI_PRIM_POINTS */
	wgi_pick_polygon,	/* WGI_PRIM_STENCIL */
	wgi_pick_polygon,	/* WGI_PRIM_HOLE */
	wgi_pick_flush,		/* WGI_PRIM_FLUSH */
	wgi_pick_polyline,	/* WGI_PRIM_ZEBRA */
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Support Routine The Check If A Primitives Extents Intersect The Pick  --*/
/*-- Box.                                                                  --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long overlapit(xtnt, pbox)
Extent xtnt;
Extent pbox;
{
	if(xtnt.xmin > pbox.xmax)
		return (FALSE);
	if(xtnt.xmax < pbox.xmin)
		return (FALSE);
	if(xtnt.ymin > pbox.ymax)
		return (FALSE);
	if(xtnt.ymax < pbox.ymin)
		return (FALSE);

	return (TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Real Pick Call -- Save Current Position And Re-Start If Continue Flag --*/
/*-- Is One. Resets If New Seglist Pointer Is Passed In.  "WGI_PICKYES" Is --*/
/*-- Returned If A Matching Item Is Found.                                 --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_pick(manage, iden, levl, piid, ptyp)
wgiPickManage_t *manage;
unsigned long *iden;
unsigned long *levl;
unsigned long *piid;
unsigned long *ptyp;
{
	long rc;

	/*
		If Continue And We Have The Same Seglist Point At The
		Next Primitive In The Last Picked Segment, Else Reset
		All The Pointers.
	*/

	if((manage->Continue) && (manage->LastList == manage->SeglistP))
	{
		if(manage->SegprimP)
		{
			manage->SegprimP = manage->SegprimP->next;
		}
	}
	else
	{
		manage->LastList = manage->SeglistP;
		manage->SegmentP = (manage->LastList ? manage->LastList->segs : NULL);
		manage->SegprimP = (manage->SegmentP ? manage->SegmentP->prim : NULL);
	}

	if ((rc = wgi_seglist_current(manage->LastList)) != WGI_SUCCESS)
		return (rc);

	/* Check the extent of the current segment list */
	if (!overlapit(manage->LastList->xtnt, manage->PickRect))
		return (WGI_SUCCESS);

	while (manage->SegmentP)
	{
	    /*
			If The Segment Is Pickable And It Matches The Viewport We Want
			And Its Extents Overlap Our Pick Box Then Check Its Primitives
	    */

	    if (manage->SegmentP->pick && (manage->SegmentP->mask & manage->ViewMask)
	     && overlapit(manage->SegmentP->xtnt, manage->PickRect))
	    {
		while (manage->SegprimP)
		{
		    /*
			If The Primitive It Pickable And Its Extents Overlap
			Our Pick Box Call The Primitive Pick Routine To Test
			If We Really Picked It
		    */

		    if (manage->SegprimP->pick
		     && overlapit(manage->SegprimP->xtnt, manage->PickRect))
		    {
			if ((manage->PickMode == wgiPickModeExtent) ||
			    ((manage->PickMode == wgiPickModeDoPoly) &&
			     (manage->SegprimP->type != WGI_PRIM_POLYLINE) &&
			     (manage->SegprimP->type != WGI_PRIM_ZEBRA)) ||
/* ?? enno: added the above line */
			    dispatch_pick[manage->SegprimP->type](manage->SegprimP, manage->PickRect))
			{
			    /*
				We Got One So Send  Back Important
				Information And Return WGI_PICKYES
			    */

			    *iden = manage->SegmentP->iden;
			    *levl = manage->SegmentP->levl;

			    *piid = manage->SegprimP->piid;
			    *ptyp = manage->SegprimP->ptyp;

			    return (WGI_PICKYES);
			}
		    }
		    manage->SegprimP = manage->SegprimP->next;
		}
	    }
	    manage->SegmentP = manage->SegmentP->next;
	    manage->SegprimP = (manage->SegmentP ? manage->SegmentP->prim : NULL);
	}

	/*
		Found None But We Return Cleanly
	*/

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The End                                                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
