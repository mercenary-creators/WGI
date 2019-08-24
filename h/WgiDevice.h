#ifndef WGI_DEVICE_H
#define WGI_DEVICE_H

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiDevice.h -- This file contains types and definitions that are used --*/
/*--                to communicate information to the graphics device.     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- As of 7/28/93 this is an incomplete specification and is provided for --*/
/*-- the interface between rubber-banding and the graphics device routines --*/
/*-- for rubber-band drawing.                                              --*/
/*--                                                                       --*/
/*-- Producer: Dean S. Jones    Inspection: MPM 08/11/93    Coverage: 100% --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#if defined(__cplusplus)
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiDeviceDrawOp_t -- This type is used to specify how the rubber-band --*/
/*--                      lines are drawn and erased.                      --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- wgiDeviceDrawOpXorXor: Lines are drawn and erased using the exclusive --*/
/*--                        or (Xor) drawing operations.                   --*/
/*--                                                                       --*/
/*-- wgiDeviceDrawOpSetDel: Lines are drawn with a set (Set) operation and --*/
/*--                        are un-drawn with a delete (Del) operation.    --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--  WARNING: The effect of this type depends on the current value of the --*/
/*--           colormap and bit-plane mask in the graphics device!!        --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiDeviceDrawOpClears,
	wgiDeviceDrawOpXorXor,
	wgiDeviceDrawOpSetDel

} wgiDeviceDrawOp_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiDeviceDashed_t -- This type is used to specify the dash style used --*/
/*--                      in drawing graphics.                             --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- wgiDeviceDashedDoNone: The lines are drawn using a solid line with no --*/
/*--                        dashes of any kind. (Is that clear!!!)         --*/
/*--                                                                       --*/
/*-- wgiDeviceDashedDitDit: The lines are drawn using long dashes followed --*/
/*--                        by a short space.                              --*/
/*--                                                                       --*/
/*-- wgiDeviceDashedDitDot: The lines are drawn using a long dash followed --*/
/*--                        by a short space followed by a short dash then --*/
/*--                        a short space.                                 --*/
/*--                                                                       --*/
/*-- wgiDeviceDashedDitDit: The lines are drawn using short dashes or dots --*/
/*--                        followed by a short space.                     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiDeviceDashedDoNone,
	wgiDeviceDashedDitDit,
	wgiDeviceDashedDitDot,
	wgiDeviceDashedDotDot

} wgiDeviceDashed_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiDeviceMirror_t -- This type is used to specify the vectorized text --*/
/*--                      reflections.                                     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- wgiDeviceMirrorDoNot:  The text is drawn normally.  ( non-reflected ) --*/
/*--                                                                       --*/
/*-- wgiDeviceMirrorXonly:  The text is drawn reflected across the X axis. --*/
/*--                                                                       --*/
/*-- wgiDeviceMirrorYonly:  The text is drawn reflected across the Y axis. --*/
/*--                                                                       --*/
/*-- wgiDeviceMirrorXandY:  The text is drawn reflected across the X and Y --*/
/*--                        axis.                                          --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiDeviceMirrorDoNot,
	wgiDeviceMirrorXonly,
	wgiDeviceMirrorYonly,
	wgiDeviceMirrorXandY

} wgiDeviceMirror_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct wgiDevice_t
{
	struct wgiDevice_t	*Parent;

} wgiDevice_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Function prototypes for the graphics device rubber-banding routines  --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#if defined(__cplusplus) || defined(__STDC__)

extern long wgi_device_rubber_drawop(
	long	draw_graphic_flag,
	long	color_table_index,
	long	plane_table_index,
	wgiDeviceDrawOp_t,
	wgiDeviceDashed_t);

extern long wgi_device_rubber_xhairs(
	long	xpos,
	long	ypos);

extern long wgi_device_rubber_bounds(
	long	xpos,
	long	ypos,
	long	wide,
	long	high);

extern long wgi_device_rubber_vector(
	long	x1,
	long	y1,
	long	x2,
	long	y2);

extern long wgi_device_rubber_arcbox(
	long	arc_box_xpos,
	long	arc_box_ypos,
	long	arc_box_wide,
	long	arc_box_high,
	long	arc_start_angle_64th,
	long	arc_sweep_angle_64th);

extern long wgi_device_rubber_nlines(
	VDCcrd *vdc_vertex_array,
	long	number_of_points);

#else

extern long wgi_device_rubber_drawop(/* SEE PARAMETERS ABOVE */);

extern long wgi_device_rubber_xhairs(/* SEE PARAMETERS ABOVE */);
extern long wgi_device_rubber_bounds(/* SEE PARAMETERS ABOVE */);
extern long wgi_device_rubber_vector(/* SEE PARAMETERS ABOVE */);
extern long wgi_device_rubber_arcbox(/* SEE PARAMETERS ABOVE */);
extern long wgi_device_rubber_nlines(/* SEE PARAMETERS ABOVE */);

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The End                                                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* WGI_DEVICE_H -- Do not add anything after this line !!! */
