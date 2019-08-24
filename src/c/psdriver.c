#include <stdio.h>
#include <ctype.h>

#include "wgiStruct.h"
#include "wgiSymbol.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static char *PS_PROLOG[] =
{
	"%!PS\n",
	"%BOPrologue\n\n",
	"/fitwin\t{\t\t\t\t\t\t% expects ARGS: xll yll xur yur (in image units)\n",
	"\t2 index sub /idy exch def\t\t\t% construct image DY\n",
	"\t2 index sub /idx exch def\t\t\t%\t& DX\n",
	"\t/ioy exch def\t\t\t\t\t%\t& image origin Y\n",
	"\t/iox exch def\t\t\t\t\t%\t& image origin X\n",
	"\tclippath pathbbox\t\t\t\t% get usable area of plotter as (xll,yll,xur,yur)\n",
	"\t2 index sub /pdy exch def\t\t\t% compute plotter DY & DX\n",
	"\t2 index sub /pdx exch def\t\t\t% (leaving xll,yll on stack for 1st translate)\n",
	"\tpdx pdy lt idx idy lt or\t\t\t% NOW, if plotter or image is Y-oriented\n",
	"\tpdx pdy gt idx idy gt or and\t\t\t%\tbut other is X-oriented,\n",
	"\t{90 rotate\t\t\t\t\t% then rotate axis & adjust plotter origin to new\n",
	"\t exch pdx add neg translate pdx pdy pdy pdx}\t%\tll-corner and prepare for scaling (note XY reversal)\n",
	"\t{translate pdy pdx pdx pdy} ifelse\t\t% otherwise leave axis as-is, set offset and prepare for scaling\n",
	"\tdiv dup idx idy div gt\t\t\t\t% compare aspect-ratios (X/Y) of image to plotter\n",
	"\t{exch 3 1 roll\t\t\t\t\t%\t(interchange pdx with pdy for later scaling)\n",
	"\t idy /iox iox idy 5 -1 roll mul idx}\t\t%\tand begin computing the new x-offset\n",
	"\t{idx /ioy ioy idx 5 -1 roll div idy} ifelse\t%\t\t\t     or new y-offset\n",
	"\tsub 2 div sub def\t\t\t\t% finish computation and redefine selected image offset\n",
	"\t1.005 mul div dup scale pop\t\t\t% scale to fit inside plotter bound (divide args already set)\n",
	"\tiox -0.995 mul ioy -0.995 mul translate\t\t% and finally adjust image origin to (almost) coincide w/plotter\n",
	"\t} bind def\t\t\t\t\t% end-of-function: fitwin\n\n",

	"/L\t{\t\t\t\t\t\t% mnemonic constant-line-weight 'lineto' command\n",
	"\ttransform round exch round exch itransform lineto\t% map and invoke real lineto\n",
	"\t} bind def\t\t\t\t\t% end-of-function: L\n\n",

	"/M\t{\t\t\t\t\t\t% mnemonic constant-line-weight 'moveto' command\n",
	"\ttransform round exch round exch itransform moveto\t% map and invoke real moveto\n",
	"\t} bind def\t\t\t\t\t% end-of-function: M\n",
	"%\n",
	"% Define a macro to do circles\n",
	"%\n",
	"/Circ % stack: x y radius\n",
	"{\n",
	"/r exch def\n",
	"/y exch def\n",
	"/x exch def\n",
	"\tnewpath x y r 0 360 arc closepath\n",
	"} bind def\n",
	"%\n",
	"% Define a macro to do rectangles\n",
	"%\n",
	"/Rect % stack: llx lly urx ury\n",
	"{\n",
	"\t/ury exch def\n",
	"\t/urx exch def\n",
	"\t/lly exch def\n",
	"\t/llx exch def\n",
	"\tnewpath\n",
	"\turx ury M\n",
	"\turx lly L\n",
	"\tllx lly L\n",
	"\tllx ury L\n",
	"\tclosepath\n",
	"} bind def\n",
	"%\n",
	"%EOPrologue\n\n",
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long kind;
static long indx = 0;
static long mask = 0;
static FILE *file[2], *fp = NULL;
static Single xmin, ymin, xmax, ymax;
static char hold[512], name[2][L_tmpnam];

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static Function dispatch_post[WGI_PRIM_SIZE];

static long post_polyline();
static long post_circle();
static long post_text();
static long post_marker();
static long post_polygon();
static long post_rectangle();
static long post_points();
static long post_stencil();
static long post_hole();
static long post_flush();
static long post_zebra();

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long overlap(xtnt)
Extent xtnt;
{
	if(xtnt.xmin > xmax)
		return (FALSE);
	if(xtnt.xmax < xmin)
		return (FALSE);
	if(xtnt.ymin > ymax)
		return (FALSE);
	if(xtnt.ymax < ymin)
		return (FALSE);

	return (TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/* Grey scales for WGI fill styles (1=white, 0=black) */

static double grey[5] = 
{
    /* DOTS, RTOL, LTOR, BIGRTOL, RL */ 
       0.6,  0.7,  0.75, 0.95,    0.8
};

static void wgi_read_grey()
{
    int i;
    FILE* g = fopen("wgi.grey", "r");
    if (g)
    {
	for (i = 0; i < 5; i++)
	{
	    fscanf(g, "%lG", &grey[i]);
	}
	
	fclose(g);
    }
    /* else: use defaults in grey[] */
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- If wgi_fill is set to 0, no filling is done at all.                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/* Whether we want shapes to be filled (1) or not (0) */
static long wgi_fill = 1;

long wgi_set_fill_mode (new_fill_mode)
long new_fill_mode;
{
    long old_mode = wgi_fill;
    wgi_fill = new_fill_mode;
    return old_mode;		/* return previous setting of flag */
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_postscript_init(view, what, buff, iden)
WLDcrd view[2];
long what;
char *buff;
long iden;
{
	long n;

	if(fp != NULL)
	{
		WOOPS("Postscript device was not terminated");
		return (WGI_PROBLEM);
	}

	wgi_read_grey();

	xmin = MIN(view[0].x, view[1].x);
	ymin = MIN(view[0].y, view[1].y);
	xmax = MAX(view[0].x, view[1].x);
	ymax = MAX(view[0].y, view[1].y);

	mask = ((iden == WGI_VIEWPORT_ALL) ? iden : (1 << iden));

	dispatch_post[WGI_PRIM_POLYLINE] = post_polyline;
	dispatch_post[WGI_PRIM_CIRCLE] = post_circle;
	dispatch_post[WGI_PRIM_TEXT] = post_text;
	dispatch_post[WGI_PRIM_MARKER] = post_marker;
	dispatch_post[WGI_PRIM_POLYGON] = post_polygon;
	dispatch_post[WGI_PRIM_RECTANGLE] = post_rectangle;
	dispatch_post[WGI_PRIM_POINTS] = post_points;
	dispatch_post[WGI_PRIM_STENCIL] = post_stencil;
	dispatch_post[WGI_PRIM_HOLE] = post_hole;
	dispatch_post[WGI_PRIM_FLUSH] = post_flush;
	dispatch_post[WGI_PRIM_ZEBRA] = post_zebra;

	for(n=0; n < 2; n++)
	{
		if((file[n] = fopen(tmpnam(name[n]), "w+")) == NULL)
		{
			WOOPS("Can't open %s for writing of postscript", name[n]);
			return (WGI_PROBLEM);
		}
	}

	ipost_fillspec(WGI_FILL_DOTS);

	for (n=0; n < sizeof(PS_PROLOG)/sizeof(PS_PROLOG[0]); n++)
		fputs(PS_PROLOG[n], fp);

	fprintf(fp, "%.0lf %.0lf %.0lf %.0lf fitwin closepath stroke\n",
		xmin, ymin, xmax, ymax);

	kind = what;
	strcpy(hold, buff);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_postscript_done()
{
	long n;
	char buff[512];

	if(fp == NULL)
	{
		WOOPS("Postscript device is not initialized");
		return (WGI_PROBLEM);
	}

	switch(kind)
	{
		case WGI_POST_PRINTCMD:
			if((fp = popen(hold, "w")) == NULL)
			{
				WOOPS("Can't open %s for writing of postscript", hold);
				return (WGI_PROBLEM);
			}
		break;
		case WGI_POST_FILENAME:
			if((fp = fopen(hold, "w")) == NULL)
			{
				WOOPS("Can't open %s for writing of postscript", hold);
				return (WGI_PROBLEM);
			}
		break;
		default:
			WOOPS("Invalid postscript output type");
			return (WGI_PROBLEM);
	}

	for(n = 0; n < 2; n++)
	{
		rewind(file[n]);
		unlink(name[n]);
		while(fgets(buff, 512, file[n]))
			fputs(buff, fp);
		fclose(file[n]);
	}

	fputs("showpage\n", fp);

	switch(kind)
	{
		case WGI_POST_PRINTCMD:
			pclose(fp);
		break;
		case WGI_POST_FILENAME:
			fclose(fp);
		break;
	}

	fp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_postscript_parseit(list)
Seglist *list;
{
	register Segment *this;
	register Segprim *prim;
	WLDcrd *wldp = (WLDcrd *)list->wldt.data;

	if(fp == NULL)
	{
		WOOPS("Postscript device is not initialized");
		return (WGI_PROBLEM);
	}

	if(overlap(list->xtnt))
	{
		for (this = list->segs; this; this = this->next)
		{
			if (this->visi && (this->mask & mask) && overlap(this->xtnt))
			{
				for (prim = this->prim; prim; prim = prim->next)
				{
					if(overlap(prim->xtnt))
					{
						dispatch_post[prim->type](wldp+prim->indx, prim);
					}
				}
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

static long ipost_linewide(wide)
long wide;
{
	static long save[2];

	if(wide != save[indx])
	{
		fprintf(fp, "%d setlinewidth\n", wide);
		save[indx] = wide;
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

static long ipost_linetype(type)
long type;
{
	static long save[2];

	if(type != save[indx])
	{
		fprintf(fp, "%d setlinecap\n", type);
		fprintf(fp, "%d setlinejoin\n", type);
		save[indx] = type;
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

static long ipost_fillspec(fill)
long fill;
{
	switch(fill)
	{
		case WGI_FILL_NONE:
		case WGI_FILL_FILL:
			indx = 1;
		break;
		case WGI_FILL_DOTS:
		case WGI_FILL_DOTS + WGI_FILL_OFFS:
		case WGI_FILL_RTOL:
		case WGI_FILL_RTOL + WGI_FILL_OFFS:
		case WGI_FILL_LTOR:
		case WGI_FILL_LTOR + WGI_FILL_OFFS:
		case WGI_FILL_BIGRTOL:
		case WGI_FILL_BIGRTOL + WGI_FILL_OFFS:
		case WGI_FILL_RL:
		case WGI_FILL_RL + WGI_FILL_OFFS:
			indx = 0;
		break;
		default:	
			fprintf(fp, "%% ipost_fillspec(%d)\n", fill);
			WOOPS("Unexpected fill mode %d in ipost_fillspec", fill);
			return (WGI_PROBLEM);
	}

	fp = file[indx];

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Beware: also setting fill color to 1 (white) in wgi_hole()            --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long ipost_fillpost(fill)
long fill;
{
	/* suppress all filling if wgi_fill = 0 */
	if (!wgi_fill)
		fill = WGI_FILL_NONE;

	switch(fill)
	{
		case WGI_FILL_NONE:
			fputs("stroke\n", fp);
		break;
		case WGI_FILL_FILL:
			fputs("eofill\n", fp);
		break;
		case WGI_FILL_DOTS:
		case WGI_FILL_DOTS + WGI_FILL_OFFS:
			fprintf(fp, "gsave %.6lG setgray eofill grestore stroke\n", grey[0]);
		break;
		case WGI_FILL_RTOL:
		case WGI_FILL_RTOL + WGI_FILL_OFFS:
			fprintf(fp, "gsave %.6lG setgray eofill grestore stroke\n", grey[1]);
		break;
		case WGI_FILL_LTOR:
		case WGI_FILL_LTOR + WGI_FILL_OFFS:
			fprintf(fp, "gsave %.6lG setgray eofill grestore stroke\n", grey[2]);
		break;
		case WGI_FILL_BIGRTOL:
		case WGI_FILL_BIGRTOL + WGI_FILL_OFFS:
			fprintf(fp, "gsave %.6lG setgray eofill grestore stroke\n", grey[3]);
		break;
		case WGI_FILL_RL:
		case WGI_FILL_RL + WGI_FILL_OFFS:
			fprintf(fp, "gsave %.6lG setgray eofill grestore stroke\n", grey[4]);
		break;
		default:
			fprintf(fp, "%% ipost_fillpost(%d)\n", fill);
			WOOPS("Unexpected fill mode %d in ipost_fillpost", fill);
			return (WGI_PROBLEM);
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

static long ipost_coords_out(wldp, nelm, dx, dy)
WLDcrd *wldp;
long nelm;
Single dx;
Single dy;
{
	long n;

	for(n = 0; n < nelm; n++)
	{
		fprintf(fp, "%.0lf %.0lf %s\n",
			wldp[n].x+dx, wldp[n].y+dy, (n) ? "L" : "M");
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

static long post_polyline(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
{
	WLDcrd *wldw = (WLDcrd *)wld_ltabl.data + (prim->wide);

	ipost_fillspec(0);
	ipost_linetype(prim->user);
	ipost_linewide(nint(wldw->x));
	ipost_coords_out(wldp, prim->nelm, (Single)0, (Single)0);
	ipost_fillpost(0);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long post_zebra(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
{
	WLDcrd *wldw = (WLDcrd *)wld_ltabl.data + (prim->wide);

	ipost_fillspec(0);
	ipost_linetype(prim->user % 2);
/* ?? not supporting dashes for now */
	ipost_linewide(nint(wldw->x));
	ipost_coords_out(wldp, prim->nelm, (Single)0, (Single)0);
	ipost_fillpost(0);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long post_circle(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
{
	WLDcrd *wldw = (WLDcrd *)wld_ltabl.data + prim->wide;

	ipost_fillspec(prim->user);
	ipost_linewide(1);
	fprintf(fp, "%.0lf %.0lf %.0lf Circ ", wldp->x, wldp->y, wldw->x);
	ipost_fillpost(prim->user);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long post_marker(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
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

static long post_polygon(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
{
	ipost_fillspec(prim->user);
	ipost_linewide(1);
	ipost_coords_out(wldp, prim->nelm, (Single)0, (Single)0);
	ipost_fillpost(prim->user);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long post_rectangle(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
{
	ipost_fillspec(prim->user);
	ipost_linewide(1);
	fprintf(fp, "%.0lf %.0lf %.0lf %.0lf Rect ",
		wldp[0].x, wldp[0].y, wldp[1].x, wldp[1].y);
	ipost_fillpost(prim->user);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long post_points(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
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

static long post_stencil(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
{
	ipost_fillspec(prim->user);
	ipost_linewide(1);
	ipost_coords_out(wldp, prim->nelm, (Single)0, (Single)0);
	ipost_fillpost(prim->user);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long post_hole(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
{
	ipost_fillspec(prim->user);
	ipost_linewide(1);
	ipost_coords_out(wldp, prim->nelm, (Single)0, (Single)0);
	/* draw the hole in white ( 1 setgray ) */ 
	if (wgi_fill)
	    fputs("gsave 1.0 setgray eofill grestore stroke\n", fp);
	else
	    ipost_fillpost(0);	/* i.e. stroke */

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long post_flush(wldp, prim)
WLDcrd *wldp;
Segprim *prim;
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

static long post_text(buff, prim)
WLDcrd *buff;
Segprim *prim;
{
	long ln;
	Single sx, sy, dx, dy;
	char ch, *word = (char *)prim->user;
	WLDcrd *wldp = (WLDcrd *)wld_ttabl.data;

	Texttprm *ttprmpntr;
	Texttseg **textalpha = ((Textdscr *)txt_dscrp.data + prim->wide)->alph;

	sx = buff[0].x;
	sy = buff[0].y;

	ln = strlen(word);
	dx = ((buff[1].x - sx) / ln);
	dy = ((buff[1].y - sy) / ln);

	ipost_fillspec(0);
	ipost_linewide(1);

	while(ch = *word++)
	{
		ttprmpntr = textalpha[ CHAR_FONT(ch) ]->prim;

		while(ttprmpntr)
		{
			ipost_coords_out(wldp+ttprmpntr->indx, ttprmpntr->nelm, sx, sy);
			ipost_fillpost(0);

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
