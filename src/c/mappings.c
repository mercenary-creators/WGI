#include "wgiStruct.h"
#include "wgiSymbol.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Variables Used For Calculating The Mappings Between Coordinate Spaces --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static Single vdc_aspc;				/* the aspect ratio value */
static Extent vdc_size;				/* current viewports size */
static Extent vdc_view;				/* current viewports view */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--   Initialize The Package -- Set Up The Coordinate Mapping Variables   --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

extern long iwgi_init_dash_styles ();

long wgi_init()
{
	long rc;

	if((rc = iwgi_device_init()) != WGI_SUCCESS)
		return (rc);

	/*
		Create Scaled Line Table
	*/

	if((rc = wgi_table_init(&wld_ltabl, sizeof(WLDcrd), 100, 33)) != WGI_SUCCESS)
		return (rc);

	/*
		Create Text Vector Table
	*/

	if((rc = wgi_table_init(&wld_ttabl, sizeof(WLDcrd), 1000, 33)) != WGI_SUCCESS)
		return (rc);

	/*
		Create Text Description Table
	*/

	if((rc = wgi_table_init(&txt_dscrp, sizeof(Textdscr), 100, 33)) != WGI_SUCCESS)
		return (rc);

	iwgi_init_dash_styles ();

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--   Set The Size Of The Viewport For The Internal Coordinate Mappings   --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_size(wide, high)
long wide;
long high;
{
	/*
		Size Of The Viewport Window
	*/

	vdc_size.xmax = (Single)wide;
	vdc_size.ymax = (Single)high;

	/*
		Calculates The Aspect Ratio
	*/

	vdc_aspc = vdc_size.ymax / vdc_size.xmax;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_posn(xpos, ypos)
long xpos;
long ypos;
{
	vdc_size.xmin = (Single)xpos;
	vdc_size.ymin = (Single)ypos;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Sets The Area Of WLD Coordinate System We Are Mapping Into Viewport  --*/
/*--           And Corrects For The Aspect Ratio Of The Viewport           --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_view(view)
Extent view;
{
	long rc;

#ifdef ASPECT_CORRECT

	Single xdif, ydif, hdif;

	xdif = view.xmax - view.xmin;
	ydif = view.ymax - view.ymin;

	if((ydif/xdif) < vdc_aspc)
	{
		hdif = 0.5 * (xdif * vdc_aspc - ydif);

		view.ymin = view.ymin - hdif;
		view.ymax = view.ymax + hdif;
	}
	else
	{
		hdif = 0.5 * (ydif / vdc_aspc - xdif);

		view.xmin = view.xmin - hdif;
		view.xmax = view.xmax + hdif;
	}

#endif

	vdc_view.xmin = MIN(view.xmin, view.xmax);
	vdc_view.xmax = MAX(view.xmin, view.xmax);
	vdc_view.ymin = MIN(view.ymin, view.ymax);
	vdc_view.ymax = MAX(view.ymin, view.ymax);

	/*
		Set The Clippers View Of What Should Be On The Screen
	*/

	if((rc = iwgi_clip_view(vdc_view)) != WGI_SUCCESS)
		return (rc);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Use X Transforms On The Y Coordinates For WLD To VDC Scale Operations --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_scaler(nelm, wld_listp, vdc_listp)
long nelm;
register WLDcrd *wld_listp;
register VDCcrd *vdc_listp;
{
	/*
		Point At The End Of The List As The Terminator
	*/

	register WLDcrd *wld_liste = wld_listp + nelm;

	/*
		Use Lots Of Registers In The Computation Loop
	*/

	register Single mx = vdc_size.xmax / (vdc_view.xmax - vdc_view.xmin);

	/*
		Round And Round We Go...
	*/

	while(wld_listp < wld_liste)
	{
		vdc_listp->x = ((wld_listp->x - 0.0) * mx) + 0.5;
		vdc_listp->y = ((0.0 - wld_listp->y) * mx) + 0.5;

		wld_listp++;
		vdc_listp++;
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Use The Current Transform To Map From World To Viewport Coordinates  --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*-- NOTE: this maps from World to Canvas (not ViewPort) coordinates.      --*/
/*--       This function is NOT the inverse of wgi_vdcwld(), but of        --*/
/*--	   wgi_scrwld() and wgi_scrwld_n() (enno) !                        --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_wldvdc(nelm, wld_listp, vdc_listp)
long nelm;
register WLDcrd *wld_listp;
register VDCcrd *vdc_listp;
{
	/*
		Point At The End Of The List As The Terminator
	*/

	register WLDcrd *wld_liste = wld_listp + nelm;

	/*
		Use Lots Of Registers In The Computation Loop
	*/

	register Single vx = vdc_view.xmin;
	register Single vy = vdc_view.ymax;
	register Single mx = vdc_size.xmax / (vdc_view.xmax - vdc_view.xmin);
	register Single my = vdc_size.ymax / (vdc_view.ymax - vdc_view.ymin);

	/*
		Round And Round We Go...
	*/

	while(wld_listp < wld_liste)
	{
		vdc_listp->x = ((wld_listp->x - vx) * mx) + vdc_size.xmin;
		vdc_listp->y = ((vy - wld_listp->y) * my) + vdc_size.ymin;

		wld_listp++;
		vdc_listp++;
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Use The Current Transform To Map From Viewport To World Coordinates  --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*-- NOTE: this maps from ViewPort (not Canvas) to World coordinates.      --*/
/*--       This function is NOT the inverse of wgi_wldvdc().               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_vdcwld(nelm, vdc_listp, wld_listp)
long nelm;
register VDCcrd *vdc_listp;
register WLDcrd *wld_listp;
{
	/*
		Point At The End Of The List As The Terminator
	*/

	register VDCcrd *vdc_liste = vdc_listp + nelm;

	/*
		Use Lots Of Registers In The Computation Loop
	*/

	register Single vx = vdc_view.xmin;
	register Single vy = vdc_view.ymax;
	register Single mx = (vdc_view.xmax - vdc_view.xmin) / vdc_size.xmax;
	register Single my = (vdc_view.ymax - vdc_view.ymin) / vdc_size.ymax;

	/*
		Round And Round We Go...
	*/

	while(vdc_listp < vdc_liste)
	{
		wld_listp->x = (vdc_listp->x * mx) + vx;
		wld_listp->y = vy - (vdc_listp->y * my);

		vdc_listp++;
		wld_listp++;
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Use The Current Transform To Map From Viewport To World Coordinates  --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*-- NOTE: this maps from Canvas (not ViewPort) to World coordinates.      --*/
/*--       This function is the inverse of wgi_wldvdc().                   --*/
/*--                                                                       --*/
/*-- NOTE: this function is similar to wgi_scrwld(), except it does        --*/
/*--	   change the current viewport (with wgi_viewport).                --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_scrwld_n(nelm, vdc_listp, wld_listp)
long nelm;
register VDCcrd *vdc_listp;
register WLDcrd *wld_listp;
{
	/*
		Point At The End Of The List As The Terminator
	*/

	register VDCcrd *vdc_liste = vdc_listp + nelm;

	/*
		Use Lots Of Registers In The Computation Loop
	*/

	register Single vx = vdc_view.xmin;
	register Single vy = vdc_view.ymax;
	register Single mx = (vdc_view.xmax - vdc_view.xmin) / vdc_size.xmax;
	register Single my = (vdc_view.ymax - vdc_view.ymin) / vdc_size.ymax;

	/*
		Round And Round We Go...
	*/

	while(vdc_listp < vdc_liste)
	{
		wld_listp->x = ((vdc_listp->x - vdc_size.xmin) * mx) + vx;
		wld_listp->y = vy - ((vdc_listp->y - vdc_size.ymin) * my);

		vdc_listp++;
		wld_listp++;
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
