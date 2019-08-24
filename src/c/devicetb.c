#include "wgiStruct.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  External References To Graphical Functions For The Sun Workstation   --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

extern long sun_polyline_do_clip();
extern long sun_polyline_no_clip();

extern long sun_circle_do_clip();
extern long sun_circle_no_clip();

extern long sun_text();

extern long sun_marker();

extern long sun_polygon_no_clip();
extern long sun_polygon_do_clip();

extern long sun_rectangle_no_clip();
extern long sun_rectangle_do_clip();

extern long sun_points_no_clip();
extern long sun_points_do_clip();

extern long sun_stencil_no_clip();
extern long sun_stencil_do_clip();

extern long sun_hole_no_clip();
extern long sun_hole_do_clip();

extern long sun_zebra_no_clip();
extern long sun_zebra_do_clip();

extern long sun_flush();

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Assign Functions To The Graphical Lookup Tables And Initialize Device --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_device_init()
{
	long rc;

	dispatch_clip[WGI_PRIM_POLYLINE] = sun_polyline_do_clip;
	dispatch_clip[WGI_PRIM_CIRCLE] = sun_circle_do_clip;
	dispatch_clip[WGI_PRIM_TEXT] = sun_text;
	dispatch_clip[WGI_PRIM_MARKER] = sun_marker;
	dispatch_clip[WGI_PRIM_POLYGON] = sun_polygon_do_clip;
	dispatch_clip[WGI_PRIM_RECTANGLE] = sun_rectangle_do_clip;
	dispatch_clip[WGI_PRIM_POINTS] = sun_points_do_clip;
	dispatch_clip[WGI_PRIM_STENCIL] = sun_stencil_do_clip;
	dispatch_clip[WGI_PRIM_HOLE] = sun_hole_do_clip;
	dispatch_clip[WGI_PRIM_FLUSH] = sun_flush;
	dispatch_clip[WGI_PRIM_ZEBRA] = sun_zebra_do_clip;

	dispatch_draw[WGI_PRIM_POLYLINE] = sun_polyline_no_clip;
	dispatch_draw[WGI_PRIM_CIRCLE] = sun_circle_no_clip;
	dispatch_draw[WGI_PRIM_TEXT] = sun_text;
	dispatch_draw[WGI_PRIM_MARKER] = sun_marker;
	dispatch_draw[WGI_PRIM_POLYGON] = sun_polygon_no_clip;
	dispatch_draw[WGI_PRIM_RECTANGLE] = sun_rectangle_no_clip;
	dispatch_draw[WGI_PRIM_POINTS] = sun_points_no_clip;
	dispatch_draw[WGI_PRIM_STENCIL] = sun_stencil_no_clip;
	dispatch_draw[WGI_PRIM_HOLE] = sun_hole_no_clip;
	dispatch_draw[WGI_PRIM_FLUSH] = sun_flush;
	dispatch_draw[WGI_PRIM_ZEBRA] = sun_zebra_no_clip;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
