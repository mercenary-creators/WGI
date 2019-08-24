#include "driver.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_menuitem_proc();

static drvMenuItem_t SubMenu_1[] =
{
	{
		"FromFile",
		drv_menuitem_proc,
		NULL,
		NULL,
		NULL,
		0,
		False,
	},
	{
		"FromDatabase",
		drv_menuitem_proc,
		NULL,
		NULL,
		NULL,
		0,
		False,
	},
	{
		"FromNetwork",
		drv_menuitem_proc,
		NULL,
		NULL,
		NULL,
		0,
		False,
	},
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static drvMenuItem_t Menu_1[] =
{
	{
		"Load",
		NULL,
		NULL,
		"Load_Menu",
		SubMenu_1,
		XtNumber(SubMenu_1),
		False,
	},
	{
		"Save",
		NULL,
		NULL,
		"Save_Menu",
		SubMenu_1,
		XtNumber(SubMenu_1),
		False,
	},
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0,
		False,
	},
	{
		"Exit",
		drv_menuitem_proc,
		"quit",
		NULL,
		NULL,
		0,
		False,
	},
};

static drvMenuItem_t Menu_H[] =
{
	{
		"General",
		drv_menuitem_proc,
		NULL,
		NULL,
		NULL,
		0,
		False,
	},
	{
		"Topical",
		drv_menuitem_proc,
		NULL,
		NULL,
		NULL,
		0,
		False,
	},
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static drvMenuItem_t Menu_0[] =
{
	{
		"File",
		NULL,
		NULL,
		"File_Menu",
		Menu_1,
		XtNumber(Menu_1),
		False,
	},
	{
		"Help",
		NULL,
		NULL,
		"Help_Menu",
		Menu_H,
		XtNumber(Menu_H),
		True,
	}
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_create_menuitems(mlabl, mmenu, mlist, mnelm)
char *mlabl;
Widget mmenu;
drvMenuItem_t *mlist;
long mnelm;
{
	long i, mseps = 0;
	Widget *mbtns = (Widget *)XtMalloc(mnelm * sizeof(Widget));

	if(mlabl)
	{
		XtVaCreateManagedWidget(mlabl,
			xmLabelWidgetClass, mmenu,
			NULL);
		XtVaCreateManagedWidget(mlabl,
			xmSeparatorWidgetClass, mmenu,
			NULL);
	}

	for(i = 0; i < mnelm; i++)
	{
		if(mlist[i].mlabl == NULL)
		{
			XtVaCreateManagedWidget("separator",
				xmSeparatorWidgetClass, mmenu,
				NULL);
			mseps++;
		}
		else if(mlist[i].mproc)
		{
			mbtns[i-mseps] = XtVaCreateWidget(mlist[i].mlabl,
				xmPushButtonWidgetClass, mmenu,
				NULL);

			XtAddCallback(mbtns[i-mseps], XmNactivateCallback,
				mlist[i].mproc, mlist[i].mdata);
		}
		else if(mlist[i].smenu == NULL)
		{
			mbtns[i-mseps] = XtVaCreateWidget(mlist[i].mlabl,
				xmLabelWidgetClass, mmenu,
				NULL);
		}
		else
		{

			Widget smenu = XmCreatePulldownMenu(mmenu,
				mlist[i].slabl, NULL, 0);

			mbtns[i-mseps] = XtVaCreateWidget(mlist[i].mlabl,
				xmCascadeButtonWidgetClass, mmenu,
				XmNsubMenuId, smenu,
				NULL);

			if(mlist[i].ihelp)
			{
				XtVaSetValues(mmenu,
					XmNmenuHelpWidget, mbtns[i-mseps],
					0);
			}

			drv_create_menuitems(mlist[i].slabl,
				smenu, mlist[i].smenu, mlist[i].snelm);
		}
	}

	XtManageChildren(mbtns, mnelm-mseps);
	XtFree(mbtns);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void drv_create_menubar(parent)
Widget parent;
{
	drv_create_menuitems(NULL, parent, Menu_0, XtNumber(Menu_0));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void drv_menuitem_proc(widget, client, call)
Widget widget;
char *client;
caddr_t call;
{
	if(client)
	{
		drv_process_command(client);
	}
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
