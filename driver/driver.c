#include "driver.h"
#include <stdio.h>

#include "wgiStruct.h"
#include "wgiDevice.h"
#include "wgiRubber.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct drvStruct
{
	XFontStruct    *fontStruct;

} drvStruct;

static drvStruct drvDeflts;

static XtResource drvDescrp[] =
{
	{
	XmNfontList, XmCFontList, XmRFontStruct, sizeof(XFontStruct *),
	XtOffsetOf(drvStruct, fontStruct), XtRString, (XtPointer)"Fixed"
	},
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long		isactive = TRUE;
static long		picknumb = 0;

static Widget		toplevel;
static Widget		mainarea;
static Widget		commands;
static Widget		graphics;
static Window		drawable;
static XtAppContext	xcontext;

static Dimension	wide;
static Dimension	high;
static XGCValues	vals;
static WLDcrd		move;

Seglist			*seglist1;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

wgiRubberManage_t	manager;
wgiRubberStruct_t	rubbers[2];

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_motion_mapper(xpos, ypos)
long xpos;
long ypos;
{
	VDCcrd vdcp;

	vdcp.x = xpos;
	vdcp.y = ypos;

	wgi_vdcwld(1, &vdcp, &move);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long drv_create_graphics()
{
	Pickdef pick;
	WLDcrd list[10];

	wgi_seglist_create(&seglist1, 0);
	wgi_seglist_current(seglist1);
	wgi_creseg(1, 1, 1<<1, TRUE, 3, 1, 2, TRUE, TRUE);

	list[0].x = 400;
	list[0].y = 400;
	list[1].x = 400+150;
	list[1].y = 400+240;

	wgi_rectangle(&list[0], FALSE, TRUE, pick, TRUE);

	list[0].x = 10;
	list[0].y = 10;
	list[1].x = 160;
	list[1].y = 110;
	list[2].x = 260;
	list[2].y = 260;
	list[3].x = 590;
	list[3].y = 560;

	wgi_polyln(4, list, 10, 1, TRUE, pick, TRUE);

	list[0].x = 360;
	list[0].y = 360;

	wgi_circle(&list[0], 150, TRUE, TRUE, pick, TRUE);

	wgi_endseg();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_graphics_expose(widget, client, xevent, contin)
Widget widget;
XtPointer client;
XEvent *xevent;
Boolean *contin;
{
	WLDcrd window[2];
	static long init = TRUE;

	if(init)
	{
		XtVaGetValues(widget,
			XmNwidth, &wide,
			XmNheight, &high,
			XmNforeground, &vals.foreground,
			XmNbackground, &vals.background,
			0);

		wgi_init();

		wgi_device_init(XtDisplay(widget), XtWindow(widget),
			wide, high, drvDeflts.fontStruct,
			vals.foreground, vals.background);

		drv_allocate_colormap(toplevel);

		XtVaSetValues(widget,
			XmNbackground, 0,
			0);

		wgi_erase_area(0, 0, wide, high, 0xFF);

		wgi_viewport_create(1, 0, 0, wide, high);

		window[0].x = 0;
		window[0].y = 0;
		window[1].x = wide;
		window[1].y = high;

		wgi_viewport_view(1, window);

		drv_create_graphics();

		wgi_viewport(1);

		init = FALSE;
	}
	wgi_seglist_parseit(seglist1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_graphics_motion(widget, client, xevent, contin)
Widget widget;
XtPointer client;
XEvent *xevent;
Boolean *contin;
{
	drv_motion_mapper(xevent->xmotion.x, xevent->xmotion.y);

	if(picknumb)
	{
		wgi_rubber_move(&manager, 1, &move);
	}
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_graphics_button(widget, client, xevent, contin)
Widget widget;
XtPointer client;
XEvent *xevent;
Boolean *contin;
{
	drv_motion_mapper(xevent->xbutton.x, xevent->xbutton.y);

	if(picknumb)
	{
		picknumb = 0;
		wgi_rubber_done(&manager);
	}
	else
	{
		picknumb = 1;

		rubbers[0].Circle.TypeOf = wgiRubberTypeOfCircle;
		rubbers[0].Circle.Anchor.x = move.x;
		rubbers[0].Circle.Anchor.y = move.y;
		rubbers[0].Circle.Offset.x = 0;
		rubbers[0].Circle.Offset.y = 0;
		rubbers[0].Circle.PlaneI = 1;
		rubbers[0].Circle.ColorI = 8;
		rubbers[0].Circle.Dashed = wgiDeviceDashedDoNone;
		rubbers[0].Circle.DrawOp = wgiDeviceDrawOpSetDel;
		rubbers[0].Circle.Spokes = wgiRubberSpokesDoNot;

		rubbers[1].Vector.TypeOf = wgiRubberTypeOfVector;
		rubbers[1].Vector.Anchor.x = move.x;
		rubbers[1].Vector.Anchor.y = move.y;
		rubbers[1].Vector.Offset.x = 0;
		rubbers[1].Vector.Offset.y = 0;
		rubbers[1].Vector.PlaneI = 1;
		rubbers[1].Vector.ColorI = 8;
		rubbers[1].Vector.Dashed = wgiDeviceDashedDitDit;
		rubbers[1].Vector.DrawOp = wgiDeviceDrawOpSetDel;
		rubbers[1].Vector.SnapTo = wgiRubberSnapToFree;

		manager.Number = 2;
		manager.DoList = rubbers;

		wgi_rubber_init(&manager);
	}
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_graphics_events(widget, client, xevent, contin)
Widget widget;
XtPointer client;
XEvent *xevent;
Boolean *contin;
{
	switch(xevent->type)
	{
	case Expose:
		drv_graphics_expose(widget, client, xevent, contin);
	break;
	case MotionNotify:
		drv_graphics_motion(widget, client, xevent, contin);
	break;
	case ButtonPress:
		drv_graphics_button(widget, client, xevent, contin);
	break;
	}
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long drv_create_window()
{
	mainarea = XtVaCreateManagedWidget("MainArea",
			xmMainWindowWidgetClass, toplevel,
			XmNshowSeparator, True,
			0);

	commands = XmCreateMenuBar(mainarea, "Commands", NULL, 0);
	XtManageChild(commands);

	drv_create_menubar(commands);

	graphics = XtVaCreateManagedWidget("Graphics",
			xmDrawingAreaWidgetClass, mainarea,
			0);

	XtAddEventHandler(graphics,
		ExposureMask|ButtonPressMask|PointerMotionMask,
		FALSE, drv_graphics_events, NULL);

	XmMainWindowSetAreas(mainarea, commands, NULL, NULL, NULL, graphics);

	return (DRV_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int main(argc, argv)
int argc;
char *argv[];
{
	XEvent that;

	XtSetLanguageProc(NULL, NULL, NULL);

	toplevel = XtVaAppInitialize(&xcontext,
		"WgiDriver1-1",
		NULL, 0,
		&argc, argv,
		NULL,
		NULL);

	XmRepTypeInstallTearOffModelConverter();

	XtGetApplicationResources(toplevel, &drvDeflts, drvDescrp,
		XtNumber(drvDescrp), NULL, 0);

	drv_create_window();

	XtRealizeWidget(toplevel);

	while(isactive)
	{
		XtAppNextEvent(xcontext, &that);
		XtDispatchEvent(&that);
	}

	exit(0);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void drv_process_command(what)
char *what;
{
	if(strcmp(what, "quit") == 0)
	{
		exit(0);
	}
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
