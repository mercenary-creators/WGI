#include "wgiStruct.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--            Global Variables Used Throughout The Software              --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

Mapping vmap;

Table *wld_table = 0;			/* The WLD Coordinate Table */
Tbuff *vdc_tbuff = 0;			/* The VDC Coordinate Table */

Table wld_ltabl;			/* The WLD Line Width Table */
Table wld_ttabl;			/* The WLD TextVector Table */
Table txt_dscrp;			/* A Text Description Table */

Function dispatch_clip[WGI_PRIM_SIZE];	/* The Graphics Clip Routines */
Function dispatch_draw[WGI_PRIM_SIZE];	/* The Graphics Draw Routines */

Seglist *iseglistp = 0;
Segment *isegmentp = 0;
Segprim *isegprimp = 0;

Flag iwgi_draw_flag = FALSE;
Flag iwgi_levl_flag = FALSE;
Flag iwgi_cont_flag = FALSE;
Flag iwgi_zero_flag = TRUE;	/* enno changed it: 7/20/94 */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Graphical Routines Are Indexed By Boolean Values And Chosen If The   --*/
/*--  Extents Of A Piece Of Graphics Fall Totally Inside A Window          --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

Function *dispatch_tabl[] =
{
	dispatch_draw,			/* Are Inside - Draw Graphics */
	dispatch_clip,			/* Not Inside - Clip Graphics */
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
