#include <math.h>
#include <stdio.h>
#include <values.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "wgiStruct.h"
#include "wgiDevice.h"
#include "wgiSymbol.h"

#include "pattern_do.h"
#include "pattern_lr.h"
#include "pattern_rl.h"
#include "patt_bigrl.h"
#include "patt_randl.h"

#define WGI_SLOP_INIT	(-1)

static GC gct, gcf, gcp, gcx, gce, gcd;
static GC gc1, gc2, gcg, gcs, gcu, gcr;
static GC gc_zebra1, gc_zebra2;

long wgirti;

static XGCValues values;
static Display *wgidsp;
static Window   wgiwin;
static Pixmap	wgiste;
static int      wgiscr;
static XFontStruct *wgifnt;

static Pixmap pix_do;
static Pixmap pix_rl;
static Pixmap pix_lr;
static Pixmap pixbig;
static Pixmap pix_randl;

static long uflag = 0;
static long fundr = 0;
static long ecolr = 0;
static long fcolr = 0;
static long tcolr = 0;
static long vhigh = 0;
static long vwide = 0;
static long vxpos = 0;
static long vypos = 0;
static long vxmax = 0;
static long vxmin = 0;
static long vymax = 0;
static long vymin = 0;
static long lwide = 0;
static long ltype = 0;
static long lwide_zebra = 0;
static long ltype_zebra = 0;
static long vslop = WGI_SLOP_INIT;
static long fillt = WGI_FILL_NONE;

#define IWGI_CTABLE_SIZE	256
#define IWGI_PTABLE_SIZE	256

static unsigned long ctable[IWGI_CTABLE_SIZE];
static unsigned long ptable[IWGI_PTABLE_SIZE];

static Tbuff *vtext = NULL;
static Tbuff *vline = NULL;

#define CLIP_BOUNDS(s)	if((s)!=(vslop))clip_bounds(s)

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Devicevp
{
	Region		eregn;
	Region		dregn;
	long		ssave;

} Devicevp;

static Devicevp *dv = 0;
static Devicevp vdev_viewport_data[WGI_VIEWPORT_MAX];

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long clip_bounds(islop)
long islop;
{
	vslop = islop;
	vxmin = vxpos - vslop;
	vxmax = vxpos + vwide + vslop;
	vymin = vypos - vslop;
	vymax = vypos + vhigh + vslop;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_dev_viewport_select(vp)
Viewport *vp;
{
	vxpos = vp->vxpos;
	vypos = vp->vypos;
	vwide = vp->vwide;
	vhigh = vp->vhigh;
	vslop = WGI_SLOP_INIT;

	vtext = &vp->vtext;
	vline = &vp->vline;

	dv = vdev_viewport_data + vp->viden;

	XSetRegion(wgidsp, gc1,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gc2,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gc_zebra1,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gc_zebra2,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gcf,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gcp,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gcs,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gct,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gcu,((dv->eregn)?(dv->eregn):(dv->dregn)));

	XSetRegion(wgidsp, gcx,((dv->eregn)?(dv->eregn):(dv->dregn)));
	XSetRegion(wgidsp, gcd,((dv->eregn)?(dv->eregn):(dv->dregn)));

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_dev_viewport_delete(vp)
Viewport *vp;
{
	Devicevp *tv = vdev_viewport_data + vp->viden;

	if (vp->vwide < 0 || vp->vhigh < 0)
	{
	    WOOPS("iwgi_dev_viewport_delete: vp->vwide < 0 || vp->vhigh < 0");
	}

	XFillRectangle(wgidsp, wgiste, gcu,
		vp->vxpos, vp->vypos, vp->vwide, vp->vhigh);

	XDestroyRegion(tv->dregn);

	tv->dregn = NULL;
	tv->ssave = FALSE;

	if(tv == dv) dv = NULL;

	return (WGI_SUCCESS);
}

long iwgi_dev_viewport_expose(vp, eregn)
Viewport *vp;
Region eregn;
{
	Devicevp *tv = vdev_viewport_data + vp->viden;

	tv->eregn = eregn;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_dev_viewport_create(vp)
Viewport *vp;
{
	XRectangle drect;
	Devicevp *tv = vdev_viewport_data + vp->viden;

	drect.x = vp->vxpos;
	drect.y = vp->vypos;
	drect.width = vp->vwide;
	drect.height = vp->vhigh;

	tv->ssave = FALSE;
	tv->dregn = XCreateRegion();

	if (vp->vwide < 0 || vp->vhigh < 0)
	{
	    WOOPS("iwgi_dev_viewport_create: vp->vwide < 0 || vp->vhigh < 0");
	}
	XFillRectangle(wgidsp, wgiste, gcu,
		vp->vxpos, vp->vypos, vp->vwide, vp->vhigh);

	XUnionRectWithRegion(&drect, tv->dregn, tv->dregn);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_color_set(edge, fill, text)
long edge, fill, text;
{
	uflag = FALSE;

	if(ecolr != edge)
	{
		XSetForeground(wgidsp, gc1, ctable[edge]);
		XSetForeground(wgidsp, gc2, ctable[edge]);
		XSetForeground(wgidsp, gc_zebra1, ctable[edge]);
		XSetForeground(wgidsp, gc_zebra2, ctable[edge]);
		XSetForeground(wgidsp, gcs, ctable[edge]);
		ecolr = edge;
	}

	if(fcolr != fill)
	{
		XSetForeground(wgidsp, gcf, ctable[fill]);
		fcolr = fill;
	}

	if(tcolr != text)
	{
		XSetForeground(wgidsp, gct, ctable[text]);
		tcolr = text;
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

long sun_undrw_set(edge, fill, text)
long edge, fill, text;
{
	uflag = TRUE;
	fundr = fill;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_circle_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long wide, radi;

	if((radi = ((VDCcrd *)vline->data + prim->wide)->x) == 0)
		return (WGI_SUCCESS);

	wide = radi + radi;

	if(prim->user)
	{
		XFillArc(wgidsp, wgiwin, gcf, buff->x-radi, buff->y-radi,
			wide, wide, 0, 360*64);
	}
	else
	{
		XDrawArc(wgidsp, wgiwin, gc1, buff->x-radi, buff->y-radi,
			wide, wide, 0, 360*64);
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

long sun_circle_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long wide, radi;

	if((radi = ((VDCcrd *)vline->data + prim->wide)->x) == 0)
		return (WGI_SUCCESS);

	CLIP_BOUNDS(0);
	if((buff->x - radi) > vxmax)
		return (WGI_SUCCESS);
	if((buff->x + radi) < vxmin)
		return (WGI_SUCCESS);
	if((buff->y - radi) > vymax)
		return (WGI_SUCCESS);
	if((buff->y + radi) < vymin)
		return (WGI_SUCCESS);

	wide = radi + radi;

	if(prim->user)
	{
		XFillArc(wgidsp, wgiwin, gcf, buff->x-radi, buff->y-radi,
			wide, wide, 0, 360*64);
	}
	else
	{
		XDrawArc(wgidsp, wgiwin, gc1, buff->x-radi, buff->y-radi,
			wide, wide, 0, 360*64);
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

static long outcode(x, y)
register long x, y;
{
	return(	((vymax-y) < (0) ? (1) : (0))|
		((y-vymin) < (0) ? (2) : (0))|
		((vxmax-x) < (0) ? (4) : (0))|
		((x-vxmin) < (0) ? (8) : (0)));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Added by enno: copied this from sundrive.c in IDS's ios/wgi           --*/
/*-- It basically does the same calculations as iwgi_clip, but it does     --*/
/*-- not draw. Instead it returns whether it actually clipped or not.      --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wclip(x1, y1, x2, y2, x3, y3, x4, y4)
register long x1, y1, x2, y2;
short *x3, *y3, *x4, *y4;
/* NOTE: returning shorts ! */
{
	register long o1, o2, o3, x0, y0;

	o1 = outcode(x1, y1);

	o2 = outcode(x2, y2);

	while ((o1 != 0) || (o2 != 0))
	{
		if ((o1 & o2) != 0)
			return (FALSE);

		if ((o3 = o1) == 0)
			o3 = o2;

		if (o3 & 1)
		{
			x0 = x1 + (x2 - x1) * (vymax - y1) / (y2 - y1);
			y0 = vymax;
		}
		else if (o3 & 2)
		{
			x0 = x1 + (x2 - x1) * (vymin - y1) / (y2 - y1);
			y0 = vymin;
		}
		else if (o3 & 4)
		{
			y0 = y1 + (y2 - y1) * (vxmax - x1) / (x2 - x1);
			x0 = vxmax;
		}
		else if (o3 & 8)
		{
			y0 = y1 + (y2 - y1) * (vxmin - x1) / (x2 - x1);
			x0 = vxmin;
		}
		if (o3 == o1)
		{
			x1 = x0;
			y1 = y0;
			o1 = outcode(x1, y1);
		}
		else
		{
			x2 = x0;
			y2 = y0;
			o2 = outcode(x2, y2);
		}
	}
	*x3 = x1;
	*y3 = y1;
	*x4 = x2;
	*y4 = y2;
	return (TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_clip(x1, y1, x2, y2)
register long x1, y1, x2, y2;
{
	register long o1, o2, o3, x0, y0;
	o1 = outcode(x1, y1);
	o2 = outcode(x2, y2);

	while((o1 != 0)||(o2 != 0))
	{
		if((o1 & o2) != 0) return(WGI_SUCCESS);

		if((o3 = o1) == 0) o3 = o2;

		if(o3 & 1)
		{
			x0 = x1+(x2-x1)*(vymax-y1)/(y2-y1);
			y0 = vymax;
		}
		else if(o3 & 2)
		{
			x0 = x1+(x2-x1)*(vymin-y1)/(y2-y1);
			y0 = vymin;
		}
		else if(o3 & 4)
		{
			y0 = y1+(y2-y1)*(vxmax-x1)/(x2-x1);
			x0 = vxmax;
		}
		else if(o3 & 8)
		{
			y0 = y1+(y2-y1)*(vxmin-x1)/(x2-x1);
			x0 = vxmin;
		}
		if(o3 == o1)
		{
			x1 = x0;
			y1 = y0;
			o1 = outcode(x1, y1);
		}
		else
		{
			x2 = x0;
			y2 = y0;
			o2 = outcode(x2, y2);
		}
	}

	XDrawLine(wgidsp, wgiwin, gcg, x1, y1, x2, y2);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_polyline_no_clip2(buff, wide, nelm, terminal_style)
VDCcrd *buff;
long wide;
unsigned short nelm;
unsigned long terminal_style;
{
	long mask = 0;

	if(wide < 2)
	{
		gcg = gc1;
	}
	else
	{
		if(wide != lwide)
		{
			mask |= GCLineWidth;
			values.line_width = lwide = wide;
		}

		if(terminal_style != ltype)
		{
			if(ltype = terminal_style)
			{
				values.cap_style = CapRound;
				values.join_style = JoinRound;
			}
			else
			{
				values.cap_style = CapButt;
				values.join_style = JoinMiter;
			}
			mask |= (GCJoinStyle|GCCapStyle);
		}

		if(mask)
		{
			XChangeGC(wgidsp, gc2, mask, &values);
		}
		gcg = gc2;
	}

	XDrawLines(wgidsp, wgiwin, gcg, buff, nelm, CoordModeOrigin);

	return (WGI_SUCCESS);
}

long sun_polyline_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	/* enno: I introduced another function call level */
	/*	 I hope it isn't noticeable */

	return sun_polyline_no_clip2 (buff, 
		((VDCcrd *)vline->data + prim->wide)->x,
		prim->nelm,
		prim->user);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define GOOD(i) (i<(nelm-1))

#define	BOTH(i) ((list[i].done)&&(list[i+1].done))

#define SAME(i) ((list[i].b.x==list[i+1].a.x)&&(list[i].b.y==list[i+1].a.y))

long sun_polyline_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long i, radi, j;
	long nelm = prim->nelm;
	long wide = ((VDCcrd *)vline->data + prim->wide)->x;
	VDCcrd poly[1024], z, *pptr;

	static struct
	{
		long done;
		VDCcrd a;
		VDCcrd b;

	} list[1024];

	if(wide < 2)
	{
		gcg = gc1;
		CLIP_BOUNDS(0);
		for(i=1, pptr = &buff[1]; i < nelm; i++, pptr++)
			iwgi_clip((pptr-1)->x,(pptr-1)->y,(pptr)->x,(pptr)->y);

		return (WGI_SUCCESS);
	}

	/* expand clip window with half the polyline width */

	radi = wide >> 1;
	if(!(wide&01))
	    radi--;
	CLIP_BOUNDS(radi);

	for (i = 0; i < (nelm - 1); i++)
	{
	    list[i].done = wclip(
			buff[i].x, buff[i].y, buff[i + 1].x, buff[i + 1].y,
			&list[i].a.x,
			&list[i].a.y,
			&list[i].b.x,
			&list[i].b.y);
	}

	for (i = 0; GOOD(i); i++)
	{
	    j = 0;

	    while(GOOD(i) && !list[i].done)
		i++;

	    if(GOOD(i) && list[i].done)
	    {
		poly[j++] = list[i+0].a;

		while(GOOD(i) && BOTH(i) && SAME(i))
		{
		    poly[j++] = list[i+0].b;
		    poly[j++] = list[i+1].a;
		    i++;
		}

		poly[j++] = list[i+0].b;

		if(j >= 2)
		{
		    sun_polyline_no_clip2 (poly, wide, j, prim->user);
		}
	    }
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

extern long zebra_calc_dashes ();

long sun_zebra_no_clip2(buff, wide, nelm, style)
VDCcrd *buff;
long wide;
unsigned short nelm;
unsigned long style;
{
	long mask = 0, diff_dashes = 0;
	unsigned long terminal_style = style % 2;
	long dash_style = style / 2;
	static int dash_offset = 0;
	unsigned char* zebra_dashes = NULL;
	int count = 0;

	if (wide < 2)
	{
		gcg = gc_zebra1;
	}
	else
	{
		/* Calculate the lengths of the dashes and gaps. */
	        /* diff_dashes = TRUE, when the dashes changed. */

		diff_dashes = zebra_calc_dashes (dash_style, wide,
			 &zebra_dashes, &count);

		if (wide != lwide_zebra)
		{
			mask |= GCLineWidth;
			values.line_width = lwide_zebra = wide;
		}

		if (terminal_style != ltype_zebra)
		{
			if(ltype_zebra = terminal_style)
			{
				values.cap_style = CapButt;	/*CapRound;*/
				values.join_style = JoinRound;
			}
			else
			{
				values.cap_style = CapButt;
				values.join_style = JoinMiter;
			}
			mask |= (GCJoinStyle|GCCapStyle);
		}

		if (mask)
		{
			XChangeGC(wgidsp, gc_zebra2, mask, &values);
		}
		if (diff_dashes)
		{
			XSetDashes(wgidsp, gc_zebra2, dash_offset,
				zebra_dashes, count);
		}

		gcg = gc_zebra2;
	}

	XDrawLines(wgidsp, wgiwin, gcg, buff, nelm, CoordModeOrigin);

	return (WGI_SUCCESS);
}

long sun_zebra_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	/* enno: I introduced another function call level */
	/*	 I hope it isn't noticeable */

	return sun_zebra_no_clip2 (buff, 
		((VDCcrd *)vline->data + prim->wide)->x,
		prim->nelm,
		prim->user);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_zebra_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long i, radi, j;
	long nelm = prim->nelm;
	long wide = ((VDCcrd *)vline->data + prim->wide)->x;
	VDCcrd poly[1024], z, *pptr;

	static struct
	{
		long done;
		VDCcrd a;
		VDCcrd b;

	} list[1024];

	if (wide < 2)
	{
		gcg = gc_zebra1;
		CLIP_BOUNDS(0);
		for(i=1, pptr = &buff[1]; i < nelm; i++, pptr++)
			iwgi_clip((pptr-1)->x,(pptr-1)->y,(pptr)->x,(pptr)->y);

		return (WGI_SUCCESS);
	}

	/* expand clip window with half the zebra width */

	radi = wide >> 1;
	if(!(wide&01))
	    radi--;
	CLIP_BOUNDS(radi);

	for (i = 0; i < (nelm - 1); i++)
	{
	    list[i].done = wclip(
			buff[i].x, buff[i].y, buff[i + 1].x, buff[i + 1].y,
			&list[i].a.x,
			&list[i].a.y,
			&list[i].b.x,
			&list[i].b.y);
	}

	for (i = 0; GOOD(i); i++)
	{
	    j = 0;

	    while(GOOD(i) && !list[i].done)
		i++;

	    if(GOOD(i) && list[i].done)
	    {
		poly[j++] = list[i+0].a;

		while(GOOD(i) && BOTH(i) && SAME(i))
		{
		    poly[j++] = list[i+0].b;
		    poly[j++] = list[i+1].a;
		    i++;
		}

		poly[j++] = list[i+0].b;

		if(j >= 2)
		{
		    sun_zebra_no_clip2 (poly, wide, j, prim->user);
		}
	    }
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

long sun_text(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long ln, i;
	long ix, iy;
	Single sx, sy, dx, dy;
	char ch, *word = (char *)prim->user;
	VDCcrd *vdcp = (VDCcrd *)vtext->data;
	VDCcrd *vdci;

	Texttprm *ttprmpntr;
	Texttseg **textalpha = ((Textdscr *)txt_dscrp.data + prim->wide)->alph;

	sx = buff[0].x;
	sy = buff[0].y;

	ln = strlen(word);

	dx = ((buff[1].x - sx) / ln);
	dy = ((buff[1].y - sy) / ln);

	if(((dx * dx) + (dy * dy)) < 9)
	{
		if(ln > 1)
		{
			XDrawLine(wgidsp, wgiwin, gct, buff[0].x, buff[0].y,
				buff[1].x, buff[1].y);
		}
		else
		{
			XDrawPoint(wgidsp, wgiwin, gct, buff[0].x, buff[0].y);
		}

		return (WGI_SUCCESS);
	}

	while(ch = *word++)
	{
		ttprmpntr = textalpha[ CHAR_FONT(ch) ]->prim;

		ix = (sx+0.5);
		iy = (sy+0.5);
		while(ttprmpntr)
		{
			vdci = vdcp + ttprmpntr->indx;
			for(i=1; i < ttprmpntr->nelm; i++)
			{
				XDrawLine(wgidsp, wgiwin, gct,
				vdci[i-1].x+ix, vdci[i-1].y+iy,
				vdci[i].x+ix, vdci[i].y+iy);
			}

			ttprmpntr = ttprmpntr->next;
		}

		sx += dx;
		sy += dy;
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

long sun_marker(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	int garb;
	char word[2];
	VDCcrd list[5];
	XCharStruct size;

	switch (prim->user)
	{
		case WGI_MARKER_POINT:
			XDrawPoint(wgidsp, wgiwin, gc1, buff->x, buff->y);
			break;

		case WGI_MARKER_ARROW_UP:
			list[0].x = buff->x;
			list[0].y = buff->y + 3;
			list[1].x = buff->x;
			list[1].y = buff->y - 3;
			list[2].x = buff->x + 3;
			list[2].y = buff->y;
			list[3].x = buff->x;
			list[3].y = buff->y - 3;
			list[4].x = buff->x - 3;
			list[4].y = buff->y;
			XDrawLines(wgidsp, wgiwin, gc1, list,
				5, CoordModeOrigin);
			break;

		case WGI_MARKER_ARROW_DOWN:
			list[0].x = buff->x;
			list[0].y = buff->y - 3;
			list[1].x = buff->x;
			list[1].y = buff->y + 3;
			list[2].x = buff->x - 3;
			list[2].y = buff->y;
			list[3].x = buff->x;
			list[3].y = buff->y + 3;
			list[4].x = buff->x + 3;
			list[4].y = buff->y;
			XDrawLines(wgidsp, wgiwin, gc1, list,
				5, CoordModeOrigin);
			break;

		case WGI_MARKER_ARROW_LEFT:
			list[0].x = buff->x + 3;
			list[0].y = buff->y;
			list[1].x = buff->x - 3;
			list[1].y = buff->y;
			list[2].x = buff->x;
			list[2].y = buff->y - 3;
			list[3].x = buff->x - 3;
			list[3].y = buff->y;
			list[4].x = buff->x;
			list[4].y = buff->y + 3;
			XDrawLines(wgidsp, wgiwin, gc1, list,
				5, CoordModeOrigin);
			break;

		case WGI_MARKER_ARROW_RIGHT:
			list[0].x = buff->x - 3; 
			list[0].y = buff->y;
			list[1].x = buff->x + 3; 
			list[1].y = buff->y;
			list[2].x = buff->x; 
			list[2].y = buff->y + 3;
			list[3].x = buff->x + 3; 
			list[3].y = buff->y;
			list[4].x = buff->x; 
			list[4].y = buff->y - 3;
			XDrawLines(wgidsp, wgiwin, gc1, list,
				5, CoordModeOrigin);
			break;

		case WGI_MARKER_TRIANGLE:
			list[0].x = buff->x;
			list[0].y = buff->y - 3;
			list[1].x = buff->x + 3;
			list[1].y = buff->y + 3;
			list[2].x = buff->x - 3;
			list[2].y = buff->y + 3;
			list[3].x = buff->x;
			list[3].y = buff->y - 3;
			XDrawLines(wgidsp, wgiwin, gc1, list,
				4, CoordModeOrigin);
			break;

		case WGI_MARKER_BOX:			
			list[0].x = buff->x - 3; 
			list[0].y = buff->y - 3;
			list[1].x = buff->x + 3; 
			list[1].y = buff->y - 3;
			list[2].x = buff->x + 3; 
			list[2].y = buff->y + 3;
			list[3].x = buff->x - 3; 
			list[3].y = buff->y + 3;
			list[4].x = buff->x - 3; 
			list[4].y = buff->y - 3;
			XDrawLines(wgidsp, wgiwin, gc1, list,
				5, CoordModeOrigin);
			break;

		default:
			word[0] = prim->user;
			word[1] = '\0';

			XTextExtents(wgifnt, word, 1,
				&garb, &garb, &garb, &size);

			XDrawString(wgidsp, wgiwin, gc1,
				buff->x - (size.width / 2),
				buff->y + (size.ascent / 2), word, 1);
			break;
	
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

long sun_rectangle_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
    int x, y;
    unsigned int w, h;
    
    if (buff[0].x < buff[1].x)
    {
	x = buff[0].x;
	w = (unsigned int) (buff[1].x - buff[0].x);
    }
    else
    {
	x = buff[1].x;
	w = (unsigned int) (buff[0].x - buff[1].x);
    }
    if (buff[0].y < buff[1].y)
    {
	y = buff[0].y;
	h = (unsigned int) (buff[1].y - buff[0].y);
    }
    else
    {
	y = buff[1].y;
	h = (unsigned int) (buff[0].y - buff[1].y);
    }
    
    if (prim->user)
    {
	XFillRectangle(wgidsp, wgiwin, gcf, x, y, w, h);
    }
    else
    {
	XDrawRectangle(wgidsp, wgiwin, gc1, x, y, w, h);
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

long sun_rectangle_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
    int x, y;
    unsigned int w, h;
    

    if (prim->user)
    {
	if (buff[0].x < buff[1].x)
	{
	    x = buff[0].x;
	    w = (unsigned int) (buff[1].x - buff[0].x);
	}
	else
	{
	    x = buff[1].x;
	    w = (unsigned int) (buff[0].x - buff[1].x);
	}
	if (buff[0].y < buff[1].y)
	{
	    y = buff[0].y;
	    h = (unsigned int) (buff[1].y - buff[0].y);
	}
	else
	{
	    y = buff[1].y;
	    h = (unsigned int) (buff[0].y - buff[1].y);
	}
	XFillRectangle(wgidsp, wgiwin, gcf, x, y, w, h);
    }
    else
    {
	gcg = gc1;
	CLIP_BOUNDS(0);
	iwgi_clip(buff[0].x, buff[1].y, buff[1].x, buff[1].y);
	iwgi_clip(buff[1].x, buff[1].y, buff[1].x, buff[0].y);
	iwgi_clip(buff[1].x, buff[0].y, buff[0].x, buff[0].y);
	iwgi_clip(buff[0].x, buff[0].y, buff[0].x, buff[1].y);
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

#define SUN_POINTS_MAX	1000

static long sun_points(xbeg, xend, ybeg, yend, numx, buff)
long xbeg;
long xend;
long ybeg;
long yend;
long numx;
VDCcrd *buff;
{
	VDCcrd pnts[SUN_POINTS_MAX];
	register long indx, jndx, pndx, indy;

	for(pndx = 0, indx = xbeg; indx < xend; pndx++, indx++)
	{
		if(pndx == SUN_POINTS_MAX)
		{
			for(indy = ybeg; indy < yend; indy++)
			{
				for(jndx = 0; jndx < pndx; jndx++)
				{
					pnts[jndx].y = buff[indy+numx].y;
				}
				XDrawPoints(wgidsp, wgiwin, gc1, pnts, pndx,
					CoordModeOrigin);
			}
			pndx = 0;
		}
		pnts[pndx].x = buff[indx].x;
	}
	if(pndx)
	{
		for(indy = ybeg; indy < yend; indy++)
		{
			for(jndx = 0; jndx < pndx; jndx++)
			{
				pnts[jndx].y = buff[indy+numx].y;
			}
			XDrawPoints(wgidsp, wgiwin, gc1, pnts, pndx,
				CoordModeOrigin);
		}
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

long sun_points_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long numx = prim->user;

	return (sun_points(0, numx, 0, prim->nelm - numx, numx, buff));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_points_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long numx = prim->user;
	long numy = prim->nelm - prim->user;
	register long xbeg, xend, ybeg, yend;

	CLIP_BOUNDS(0);

	/*
		Find The Leftmost Grid Point In The Window
	*/

	xbeg = 0;
	while((xbeg < numx) && (buff[xbeg].x < vxmin))
		xbeg++;

	/*
		We Got To The End Of The Grid Buffer And
		There Are None In The Window
	*/

	if(xbeg == numx)
		return (WGI_SUCCESS);

	/*
		Find The Rightmost Grid Point In The Window
	*/

	xend = xbeg + 1;
	while((xend < numx) && (buff[xend].x < vxmax))
		xend++;

	/*
		Find The Lowermost Grid Point In The Window
	*/

	ybeg = 0;
	while((ybeg < numy) && (buff[ybeg+numx].y > vymax))
		ybeg++;

	/*
		We Got To The End Of The Grid Buffer And
		There Are None In The Window
	*/

	if(ybeg == numy)
		return (WGI_SUCCESS);

	/*
		Find The Lowermost Grid Point In The Window
	*/

	yend = ybeg + 1;
	while((yend < numy) && (buff[yend+numx].y > vymin))
		yend++;

	return (sun_points(xbeg, xend, ybeg, yend, numx, buff));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_fill_style(type)
long type;
{
	switch(fillt = type)
	{
		case WGI_FILL_LTOR:
			XSetStipple(wgidsp, gcp, pix_lr);
		break;
		case WGI_FILL_LTOR|WGI_FILL_OFFS:
			XSetStipple(wgidsp, gcp, pix_lr);
		break;
		case WGI_FILL_RTOL:
			XSetStipple(wgidsp, gcp, pix_rl);
		break;
		case WGI_FILL_RTOL|WGI_FILL_OFFS:
			XSetStipple(wgidsp, gcp, pix_rl);
		break;
		case WGI_FILL_DOTS:
			XSetStipple(wgidsp, gcp, pix_do);
		break;
		case WGI_FILL_DOTS|WGI_FILL_OFFS:
			XSetStipple(wgidsp, gcp, pix_do);
		break;
		case WGI_FILL_BIGRTOL:
			XSetStipple(wgidsp, gcp, pixbig);
		break;
		case WGI_FILL_BIGRTOL|WGI_FILL_OFFS:
			XSetStipple(wgidsp, gcp, pixbig);
		break;
		case WGI_FILL_RL:
			XSetStipple(wgidsp, gcp, pix_randl);
		break;
		case WGI_FILL_RL|WGI_FILL_OFFS:
			XSetStipple(wgidsp, gcp, pix_randl);
		break;
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

long sun_stencil_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	if(prim->user != fillt)
		sun_fill_style(prim->user);

	XFillPolygon(wgidsp, wgiste, gcp, buff, prim->nelm,
		Complex, CoordModeOrigin);

	dv->ssave = TRUE;

	XDrawLines(wgidsp, wgiwin, gc1, buff, prim->nelm,
		CoordModeOrigin);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_stencil_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long i;
	VDCcrd *pptr;
	long nelm = prim->nelm;

	gcg = gc1;
	CLIP_BOUNDS(0);

	if(prim->user != fillt)
		sun_fill_style(prim->user);

	XFillPolygon(wgidsp, wgiste, gcp, buff, nelm,
		Complex, CoordModeOrigin);

	dv->ssave = TRUE;

	for(i=1, pptr = &buff[1]; i < nelm; i++, pptr++)
		iwgi_clip((pptr-1)->x, (pptr-1)->y, (pptr)->x, (pptr)->y);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long sun_flush(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	unsigned long i;
	long xmin, ymin, xmax, ymax;

	if(dv->ssave)
	{
		xmin = xmax = buff[0].x;
		ymin = ymax = buff[0].y;

		for(i = 1; i < prim->nelm; i++)
		{
			if(buff[i].x > xmax)
				xmax = buff[i].x;
			if(buff[i].x < xmin)
				xmin = buff[i].x;
			if(buff[i].y > ymax)
				ymax = buff[i].y;
			if(buff[i].y < ymin)
				ymin = buff[i].y;
		}

		xmin = MAX(xmin, (vxpos));
		xmax = MIN(xmax, (vxpos + vwide));
		ymin = MAX(ymin, (vypos));
		ymax = MIN(ymax, (vypos + vhigh));

		XFillRectangle(wgidsp, wgiwin, gcs,
			xmin, ymin, ((xmax - xmin) + 1), ((ymax - ymin) + 1));

		XFillRectangle(wgidsp, wgiste, gcu,
			xmin, ymin, ((xmax - xmin) + 1), ((ymax - ymin) + 1));

		dv->ssave = FALSE;
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

long sun_hole_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	if(dv->ssave)
	{
		XFillPolygon(wgidsp, wgiste, gcu, buff, prim->nelm,
			Complex, CoordModeOrigin);

		XDrawLines(wgidsp, wgiwin, gc1, buff, prim->nelm,
			CoordModeOrigin);
	}
	else
	{
		XSetForeground(wgidsp, gcs, ctable[((uflag)?(fundr):(0))]);
		sun_stencil_no_clip(buff, prim);
		sun_flush(buff, prim);
		XSetForeground(wgidsp, gcs, ctable[ecolr]);
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

long sun_hole_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long i;
	VDCcrd *pptr;
	long nelm = prim->nelm;

	gcg = gc1;
	CLIP_BOUNDS(0);

	if(dv->ssave)
	{
		XFillPolygon(wgidsp, wgiste, gcu, buff, nelm,
			Complex, CoordModeOrigin);

		for(i=1, pptr = &buff[1]; i < nelm; i++, pptr++)
			iwgi_clip((pptr-1)->x, (pptr-1)->y,
				(pptr)->x, (pptr)->y);
	}
	else
	{
		XSetForeground(wgidsp, gcs, ctable[((uflag)?(fundr):(0))]);
		sun_stencil_do_clip(buff, prim);
		sun_flush(buff, prim);
		XSetForeground(wgidsp, gcs, ctable[ecolr]);
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

long sun_polygon_no_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long nelm = prim->nelm;

	if(prim->user != WGI_FILL_FILL)
	{
		sun_stencil_no_clip(buff, prim);
		sun_flush(buff, prim);
	}
	else
	{
		XFillPolygon(wgidsp, wgiwin, gcf, buff, nelm,
			Complex, CoordModeOrigin);
		XDrawLines(wgidsp, wgiwin, gc1, buff, nelm,
			CoordModeOrigin);
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

long sun_polygon_do_clip(buff, prim)
VDCcrd *buff;
Segprim *prim;
{
	long nelm = prim->nelm;
	long i;
	VDCcrd *pptr;

	if(prim->user != WGI_FILL_FILL)
	{
		sun_stencil_do_clip(buff, prim);
		sun_flush(buff, prim);
	}
	else
	{
		XFillPolygon(wgidsp, wgiwin, gcf, buff, nelm,
			Complex, CoordModeOrigin);
		gcg = gc1;
		CLIP_BOUNDS(0);
		for(i=1, pptr = &buff[1]; i < nelm; i++, pptr++)
		   iwgi_clip((pptr-1)->x, (pptr-1)->y, (pptr)->x, (pptr)->y);
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

long wgi_device_color_table(itable, inumbr)
unsigned long itable[];
unsigned long inumbr;
{
	unsigned long indx;

	if(inumbr > IWGI_CTABLE_SIZE)
	{
		inumbr = IWGI_CTABLE_SIZE;

		WOOPS("wgi_device_color_table(size=%d) Too large", inumbr);
	}

	for(indx = 0; indx < inumbr; indx++)
	{
		ctable[indx] = itable[indx];
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

long wgi_device_plane_table(itable, inumbr)
unsigned long itable[];
unsigned long inumbr;
{
	unsigned long indx;

	if(inumbr > IWGI_PTABLE_SIZE)
	{
		inumbr = IWGI_PTABLE_SIZE;

		WOOPS("wgi_device_plane_table(size=%d) Too large", inumbr);
	}

	for(indx = 0; indx < inumbr; indx++)
	{
		ptable[indx] = itable[indx];
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

static long wgidev_width = -1;
static long wgidev_height = -1;

static long wgi_device_set_width_height (wide, high)
long wide;
long high;
{
    static int set = 0;

    if (set && (wide == wgidev_width) && (high == wgidev_height))
	return (WGI_SUCCESS);

    if (set)
    {
	XFreePixmap (wgidsp, wgiste);
    }    
    wgiste = XCreatePixmap (wgidsp, wgiwin, wide, high, 1);

    wgidev_width = wide;
    wgidev_height = high;
    set = 1;

    return (WGI_SUCCESS);
}

static long wgi_device_set_stipple ()
{
	/* dimensions of stipple are used to clip stencils, holes etc. */

	XSetStipple(wgidsp, gcs, wgiste);  /* uses width/height in wgiste */
	return (WGI_SUCCESS);
}

long wgi_device_resize (wide, high)
long wide;
long high;
{
    wgi_device_set_width_height(wide, high);
    return wgi_device_set_stipple();
}

long wgi_device_init(dis, win, wide, high, fnt, fore, back)
Display *dis;
Window win;
long wide;
long high;
XFontStruct *fnt;
long fore;
long back;
{
	unsigned long indx;
	unsigned long mask = (GCFont|GCForeground|GCBackground|GCLineStyle|
			GCLineWidth|GCJoinStyle|GCCapStyle);

	for(indx = 0; indx < 256; indx++)
	{
		ctable[indx] = indx;
		ptable[indx] = 0xFF;
	}

	wgirti = fore;
	wgidsp = dis;
	wgiwin = win;
	wgifnt = fnt;
	wgiscr = DefaultScreen(wgidsp);

	values.line_width = 0;
	values.font = wgifnt->fid;
	values.cap_style = CapButt;
	values.join_style = JoinMiter;

	values.foreground = fore;
	values.background = back;

	values.line_style = LineOnOffDash;

	/* for drawing WGI_PRIM_ZEBRA, width <= 1 */
	values.dashes = 1;
	mask |= GCDashList;
	gc_zebra1 = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), 
			     mask, &values);
	mask -= GCDashList;

	/* for drawing WGI_PRIM_ZEBRA, width > 1 */
	gc_zebra2 = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), 
			     mask, &values);
	values.line_style = LineSolid;

	gc1 = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);
	gc2 = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);
	gct = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);
	gcf = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);
	gcs = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);
	gce = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);
	gcx = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);
	gcd = XCreateGC(wgidsp, RootWindow(wgidsp, wgiscr), mask, &values);

	pix_do = XCreateBitmapFromData(wgidsp, RootWindow(wgidsp, wgiscr),
		pattern_do_bits, pattern_do_width, pattern_do_height);
		
	pix_rl = XCreateBitmapFromData(wgidsp, RootWindow(wgidsp, wgiscr),
		pattern_rl_bits, pattern_rl_width, pattern_rl_height);
		
	pix_lr = XCreateBitmapFromData(wgidsp, RootWindow(wgidsp, wgiscr),
		pattern_lr_bits, pattern_lr_width, pattern_lr_height);
		
	pixbig = XCreateBitmapFromData(wgidsp, RootWindow(wgidsp, wgiscr),
		patt_bigrl_bits, patt_bigrl_width, patt_bigrl_height);
		
	pix_randl = XCreateBitmapFromData(wgidsp, RootWindow(wgidsp, wgiscr),
		patt_randl_bits, patt_randl_width, patt_randl_height);
		
	XSetForeground(wgidsp, gcx, back);
	XSetForeground(wgidsp, gcd, back);

	XSetFunction(wgidsp, gcx, GXxor);
	XSetFunction(wgidsp, gcd, GXxor);

	/* width / height related stuff */

	wgi_device_set_width_height (wide, high);	/* sets wgiste */

	values.foreground = 1;
	values.background = 0;

	gcp = XCreateGC(wgidsp, wgiste, mask, &values);
	gcu = XCreateGC(wgidsp, wgiste, mask, &values);

	XSetFunction(wgidsp, gcu, GXclear);

	XSetFillRule(wgidsp, gcp, WindingRule);
	XSetFillRule(wgidsp, gcf, WindingRule);

	XSetFillStyle(wgidsp, gcp, FillStippled);
	XSetFillStyle(wgidsp, gcs, FillStippled);

	wgi_device_set_stipple ();  /* uses width/height in wgiste */

	XSetLineAttributes(wgidsp, gcd, 0, LineOnOffDash, CapButt, JoinMiter);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_cursor_visibility(visi)
long visi;
{
	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_cursor_size(size)
long size;
{
	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_writemask(planei)
unsigned long planei;
{
	unsigned long t_planem;
	static unsigned long r_planem = 0;

	if((t_planem = ptable[planei]) != r_planem)
	{
		r_planem = t_planem;
		XSetPlaneMask(wgidsp, gc1, r_planem);
		XSetPlaneMask(wgidsp, gc2, r_planem);
		XSetPlaneMask(wgidsp, gc_zebra1, r_planem);
		XSetPlaneMask(wgidsp, gc_zebra2, r_planem);
		XSetPlaneMask(wgidsp, gct, r_planem);
		XSetPlaneMask(wgidsp, gcf, r_planem);
		XSetPlaneMask(wgidsp, gcs, r_planem);
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

long wgi_erase_area(xmin, ymin, xmax, ymax, mask)
long xmin;
long ymin;
long xmax;
long ymax;
long mask;
{
	long wide = xmax - xmin + 1;
	long high = ymax - ymin + 1;

	if (xmax < xmin || ymax < ymin)
	{
	    WOOPS("wgi_erase_area: xmax < xmin || ymax < ymin");
	}

	if(mask)
	{
		XSetPlaneMask(wgidsp, gce, mask);
		XSetForeground(wgidsp, gce, ctable[0]);
		XFillRectangle(wgidsp, wgiwin, gce, xmin, ymin, wide, high);
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Producer: Dean S. Jones    Inspection: MPM 08/11/93    Coverage: 100% --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static char dotdot[] = { 2, 3 };
static char ditdit[] = { 6, 3 };
static char ditdot[] = { 6, 3, 2, 3 };

static long iwgi_device_rubber_dashop(dodraw, colori, planei, drawop, dashed)
unsigned long dodraw;
unsigned long colori;
unsigned long planei;
wgiDeviceDrawOp_t drawop;
wgiDeviceDashed_t dashed;
{
	unsigned long t_colorv;
	unsigned long t_planem;

	static unsigned long r_colorv = 0;
	static unsigned long r_planem = 0;
	static wgiDeviceDrawOp_t r_drawop = wgiDeviceDrawOpXorXor;
	static wgiDeviceDashed_t r_dashed = wgiDeviceDashedDoNone;

	if((drawop == wgiDeviceDrawOpSetDel) && (dodraw == FALSE))
	{
		t_colorv = ctable[0];
	}
	else
	{
		t_colorv = ctable[colori];
	}

	if(t_colorv != r_colorv)
	{
		r_colorv = t_colorv;
		XSetForeground(wgidsp, gcd, r_colorv);
	}

	if((t_planem = ptable[planei]) != r_planem)
	{
		r_planem = t_planem;
		XSetPlaneMask(wgidsp, gcd, r_planem);
	}

	if(drawop != r_drawop)
	{
		switch(r_drawop = drawop)
		{
		case wgiDeviceDrawOpSetDel:

			XSetFunction(wgidsp, gcd, GXcopy);
		break;
		case wgiDeviceDrawOpXorXor:

			XSetFunction(wgidsp, gcd, GXxor);
		break;
		}
	}

	if(dashed != r_dashed)
	{
		switch(r_dashed = dashed)
		{
		case wgiDeviceDashedDitDit:

			XSetDashes(wgidsp, gcd, 0, ditdit, 2);
		break;
		case wgiDeviceDashedDitDot:

			XSetDashes(wgidsp, gcd, 0, ditdot, 4);
		break;
		case wgiDeviceDashedDotDot:

			XSetDashes(wgidsp, gcd, 0, dotdot, 2);
		break;
		}
	}

	gcr = gcd;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_device_rubber_drawop(dodraw, colori, planei, drawop, dashed)
unsigned long dodraw;
unsigned long colori;
unsigned long planei;
wgiDeviceDrawOp_t drawop;
wgiDeviceDashed_t dashed;
{
	unsigned long t_colorv;
	unsigned long t_planem;

	static unsigned long r_colorv = 0;
	static unsigned long r_planem = 0;
	static wgiDeviceDrawOp_t r_drawop = wgiDeviceDrawOpXorXor;

	if(dashed != wgiDeviceDashedDoNone)
	{
		iwgi_device_rubber_dashop(dodraw,
			colori, planei, drawop, dashed);

		return (WGI_SUCCESS);
	}

	if((drawop == wgiDeviceDrawOpSetDel) && (dodraw == FALSE))
	{
		t_colorv = ctable[0];
	}
	else
	{
		t_colorv = ctable[colori];
	}

	if(t_colorv != r_colorv)
	{
		r_colorv = t_colorv;
		XSetForeground(wgidsp, gcx, r_colorv);
	}

	if((t_planem = ptable[planei]) != r_planem)
	{
		r_planem = t_planem;
		XSetPlaneMask(wgidsp, gcx, r_planem);
	}

	if(drawop != r_drawop)
	{
		switch(r_drawop = drawop)
		{
		case wgiDeviceDrawOpSetDel:

			XSetFunction(wgidsp, gcx, GXcopy);
		break;
		case wgiDeviceDrawOpXorXor:

			XSetFunction(wgidsp, gcx, GXxor);
		break;
		}
	}

	gcr = gcx;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_device_rubber_xhairs(xpos, ypos)
long xpos;
long ypos;
{
	XDrawLine(wgidsp, wgiwin, gcr, vxpos, ypos, vxpos+vwide, ypos);
	XDrawLine(wgidsp, wgiwin, gcr, xpos, vypos, xpos, vypos+vhigh);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_device_rubber_bounds(xpos, ypos, wide, high)
long xpos;
long ypos;
long wide;
long high;
{
    if (wide < 0 || high < 0)
    {
	WOOPS("wgi_device_rubber_bounds: wide=%d,high=%d (<0)", wide, high);
    }
    
    XDrawRectangle(wgidsp, wgiwin, gcr, xpos, ypos, wide, high);
    
    return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_device_rubber_vector(x1, y1, x2, y2)
long x1;
long y1;
long x2;
long y2;
{
	XDrawLine(wgidsp, wgiwin, gcr, x1, y1, x2, y2);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_device_rubber_arcbox(xpos, ypos, wide, high, asta, aswe)
long xpos;
long ypos;
long wide;
long high;
long asta;
long aswe;
{
	XDrawArc(wgidsp, wgiwin, gcr, xpos, ypos, wide, high, asta, aswe);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_device_rubber_nlines(buff, nelm)
VDCcrd *buff;
long nelm;
{
	XDrawLines(wgidsp, wgiwin, gcr, buff, nelm, CoordModeOrigin);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The End                                                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
