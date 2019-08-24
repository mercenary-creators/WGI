#include <math.h>
#include <stdio.h>
#include <string.h>
#include <values.h>

#include "wgiStruct.h"
#include "wgiDevice.h"
#include "wgiRubber.h"
#include "wgiSymbol.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Producer: Dean S. Jones    Inspection: MPM 08/11/93    Coverage: 100% --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_common_stub(data, common, i)
void *data;
wgiRubberCommon_t *common;
long i;
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

static long iwgi_rubber_common_init(data, common, i)
void *data;
wgiRubberCommon_t *common;
long i;
{
	SAYIT("Rubber[%d] Common found", i);

	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_xhairs_init(data, xhairs, i)
void *data;
wgiRubberXhairs_t *xhairs;
long i;
{
	xhairs->Output[0] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_bounds_init(data, bounds, i)
void *data;
wgiRubberBounds_t *bounds;
long i;
{
	bounds->Output[0] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_vector_init(data, vector, i)
void *data;
wgiRubberVector_t *vector;
long i;
{
	vector->Output[0] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_mitred_init(data, mitred, i)
void *data;
wgiRubberMitred_t *mitred;
long i;
{
	mitred->Mitred = WGI_RUBBER_MITRED_NOT;
	mitred->Number = 0;
	mitred->Length = 0;
	mitred->LockIn = WGI_RUBBER_LOCKIN_NOT;

	mitred->PrevOct = WGI_RUBBER_LOCKIN_NOT;
	mitred->Changed = 0;

	mitred->Output[0] = NULL;
	mitred->Output[1] = NULL;
	mitred->Output[2] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_bend90_init(data, bend90, i)
void *data;
wgiRubberBend90_t *bend90;
long i;
{
	bend90->Number = 0;
	bend90->LockIn = WGI_RUBBER_LOCKIN_NOT;

	bend90->PrevOct = WGI_RUBBER_LOCKIN_NOT;
	bend90->Changed = 0;

	bend90->Output[0] = NULL;
	bend90->Output[1] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc90_init(data, larc90, i)
void *data;
wgiRubberLarc90_t *larc90;
long i;
{
	larc90->Radius = 0;
	larc90->StartA = 0;
	larc90->SweepA = 0;
	larc90->LockIn = WGI_RUBBER_LOCKIN_NOT;

	larc90->PrevOct = WGI_RUBBER_LOCKIN_NOT;
	larc90->Changed = 0;

	larc90->Output[0] = NULL;
	larc90->Output[1] = NULL;
	larc90->Output[2] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45_init(data, larc45, i)
void *data;
wgiRubberLarc45_t *larc45;
long i;
{
	larc45->Radius = 0;
	larc45->StartA = 0;
	larc45->SweepA = 0;

	larc45->Output[0] = NULL;
	larc45->Output[1] = NULL;
	larc45->Output[2] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_arc3pt_init(data, arc3pt, i)
void *data;
wgiRubberArc3pt_t *arc3pt;
long i;
{
	arc3pt->Radius = 0;
	arc3pt->StartA = 0;
	arc3pt->SweepA = 0;

	arc3pt->Output[0] = NULL;
	arc3pt->Output[1] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_circle_init(data, circle, i)
void *data;
wgiRubberCircle_t *circle;
long i;
{
	circle->Output[0] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_nshape_init(data, nshape, i)
void *data;
wgiRubberNshape_t *nshape;
long i;
{
	long n;

	if(nshape->Nsides > WGI_RUBBER_NSHAPE_MAX)
	{
		SAYIT("Rubber[%d].Nshape.Nsides(%d) too large",
			i, nshape->Nsides);

		nshape->Nsides = WGI_RUBBER_NSHAPE_MAX;

		SAYIT("Rubber[%d].Nshape.Nsides(%d) reset",
			i, nshape->Nsides);

	}

	if(nshape->Nsides < WGI_RUBBER_NSHAPE_MIN)
	{
		SAYIT("Rubber[%d].Nshape.Nsides(%d) too small",
			i, nshape->Nsides);

		nshape->Nsides = WGI_RUBBER_NSHAPE_MIN;

		SAYIT("Rubber[%d].Nshape.Nsides(%d) reset",
			i, nshape->Nsides);
	}

	for(n = 0; n < WGI_RUBBER_NSHAPE_MAX; n++)
	{
		nshape->Output[n] = NULL;
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

static long iwgi_rubber_ellipt_init(data, ellipt, i)
void *data;
wgiRubberEllipt_t *ellipt;
long i;
{
	ellipt->Output[0] = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_closer_init(data, closer, i)
void *data;
wgiRubberCloser_t *closer;
long i;
{
	WLDcrd *points;

	closer->Output[0] = NULL;

	if((closer->Closer = closer->Number) == 0)
	{
		SAYIT("Rubber[%d].Closer.Number is 0", i);

		return (WGI_PROBLEM);
	}

	if((points = closer->Points) == NULL)
	{
		SAYIT("Rubber[%d].Closer.Points is NULL", i);

		return (WGI_PROBLEM);
	}

	if(closer->DoCopy != TRUE)
		return (WGI_SUCCESS);

	if((closer->Points = WGI_MALLOC(closer->Number, WLDcrd)) == NULL)
	{
		SAYIT("Rubber[%d].Closer out of memory", i);

		return (WGI_PROBLEM);
	}

	memcpy(closer->Points, points, closer->Number * sizeof(WLDcrd));

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_closer_done(data, closer, i)
void *data;
wgiRubberCloser_t *closer;
long i;
{
	if(closer->DoCopy != TRUE)
		return (WGI_SUCCESS);

	if(closer->Number == 0)
	{
		SAYIT("Rubber[%d].Closer.Number is 0", i);

		return (WGI_PROBLEM);
	}

	if(closer->Points == NULL)
	{
		SAYIT("Rubber[%d].Closer.Points is NULL", i);

		return (WGI_PROBLEM);
	}

	WGI_FREE(closer->Points); closer->Points = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_textex_done(data, textex, i)
void *data;
wgiRubberTextEx_t *textex;
long i;
{
	wgiStroke_t *tmpstr, *stroke = textex->Stroke;

	if((textex->DoCopy) && (textex->TextRe))
		WGI_FREE(textex->TextRe);
	if(textex->Native)
		WGI_FREE(textex->Native);
	if(textex->Scaled)
		WGI_FREE(textex->Scaled);

	while(stroke)
	{
		tmpstr = stroke->Stroke; WGI_FREE(stroke); stroke = tmpstr;
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

static long iwgi_rubber_textex_init(data, textex, i)
void *data;
wgiRubberTextEx_t *textex;
long i;
{
	long nelm;
	short *font;
	double base = 0.0;
	wgiStroke_t *stroke;
	char c, *word, *temp;

	textex->Number = 0;
	textex->StrLen = 0;
	textex->Stroke = NULL;
	textex->Native = NULL;
	textex->Scaled = NULL;
	textex->Mirror = wgiDeviceMirrorDoNot;

	textex->Output[0] = NULL;
	textex->Output[1] = NULL;

	if((word = textex->TextRe) == NULL)
	{
		SAYIT("Rubber[%d].TextEx.TextRe is NULL", i);

		return (WGI_PROBLEM);
	}

	if((textex->StrLen = strlen(word)) < 1)
	{
		SAYIT("Rubber[%d].TextEx.TextRe is empty", i);

		return (WGI_PROBLEM);
	}

	if(textex->DoCopy)
	{
		if((temp = WGI_MALLOC(textex->StrLen+1, char)) == NULL)
		{
			SAYIT("Rubber[%d].TextEx out of memory", i);

			return (WGI_PROBLEM);
		}

		textex->TextRe = strcpy(temp, word);
	}

	nelm = textex->StrLen * FONT_MAXP;

	if((textex->Native = WGI_MALLOC(nelm, WLDcrd)) == NULL)
	{
		iwgi_rubber_textex_done(data, textex, i);

		SAYIT("Rubber[%d].TextEx out of memory", i);

		return (WGI_PROBLEM);
	}

	if((textex->Scaled = WGI_MALLOC(nelm, VDCcrd)) == NULL)
	{
		iwgi_rubber_textex_done(data, textex, i);

		SAYIT("Rubber[%d].TextEx out of memory", i);

		return (WGI_PROBLEM);
	}

	while(c = *word++)
	{
		font = iwgi_font_data[ CHAR_FONT(c) ];

		while(*font != FONT_STOP)
		{
			if((stroke = WGI_MALLOC(1, wgiStroke_t)) == NULL)
			{
				iwgi_rubber_textex_done(data, textex, i);

				SAYIT("Rubber[%d].TextEx out of memory", i);

				return (WGI_PROBLEM);
			}

			stroke->Number = 0;
			stroke->Origin = textex->Number;
			stroke->Stroke = textex->Stroke;
			textex->Stroke = stroke;

			while(*font != FONT_STOP)
			{
				textex->Native[textex->Number].x = base+*font++;
				textex->Native[textex->Number].y = *font++;
				stroke->Number++;
				textex->Number++;
			}

			font++;font++;
		}

		base = base + FONT_WVAL + (textex->Spaces * FONT_WVAL);
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

static unsigned long iwgi_calculate_octant(dx, dy, ax, ay)
double dx, dy, ax, ay;
{
	if(ax > ay)
	{
		if(dx > 0)
		{
			return ((dy > 0) ? (0) : (7));
		}
		else
		{
			return ((dy > 0) ? (3) : (4));
		}
	}
	else
	{
		if(dx > 0)
		{
			return ((dy > 0) ? (1) : (6));
		}
		else
		{
			return ((dy > 0) ? (2) : (5));
		}
	}
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static unsigned long iwgi_calculate_lockin(dx, dy, ax, ay, lockin)
double dx, dy, ax, ay;
unsigned long *lockin;
{
	switch(iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0: if(*lockin != 1) *lockin = 0; return (0); break;
	case 1: if(*lockin != 0) *lockin = 1; return (1); break;
	case 2: if(*lockin != 3) *lockin = 2; return (2); break;
	case 3: if(*lockin != 2) *lockin = 3; return (3); break;
	case 4: if(*lockin != 5) *lockin = 4; return (4); break;
	case 5: if(*lockin != 4) *lockin = 5; return (5); break;
	case 6: if(*lockin != 7) *lockin = 6; return (6); break;
	case 7: if(*lockin != 6) *lockin = 7; return (7); break;
	}
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_xhairs_draw(data, xhairs, draw)
void *data;
wgiRubberXhairs_t *xhairs;
long draw;
{
	long length;

	/*
		NOTE: xhairs->Output[0] is always set so we don`t need
		to check it here.
	*/

	wgi_device_rubber_drawop(draw,
		xhairs->ColorI, xhairs->PlaneI,
		xhairs->DrawOp, xhairs->Dashed);

	switch(xhairs->HairTo)
	{
	case wgiRubberHairToEdges:

		wgi_device_rubber_xhairs(xhairs->Mapped[0].x,
			xhairs->Mapped[0].y);

		return (WGI_SUCCESS);
	break;
	case wgiRubberHairToConst:

		length = xhairs->Length;
	break;
	case wgiRubberHairToScale:

		length = xhairs->Mapped[1].x - xhairs->Mapped[0].x;
	break;
	}

	wgi_device_rubber_vector(xhairs->Mapped[0].x-length,
		xhairs->Mapped[0].y,
		xhairs->Mapped[0].x+length,
		xhairs->Mapped[0].y);

	wgi_device_rubber_vector(xhairs->Mapped[0].x,
		xhairs->Mapped[0].y-length,
		xhairs->Mapped[0].x,
		xhairs->Mapped[0].y+length);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_xhairs_calc(data, xhairs, x1, y1, x2, y2)
void *data;
wgiRubberXhairs_t *xhairs;
double x1, y1, x2, y2;
{
	xhairs->Buffer[0].x = x2;
	xhairs->Buffer[0].y = y2;

	if((xhairs->HairTo == wgiRubberHairToScale) && (xhairs->Length > 0))
	{
		xhairs->Buffer[1].x = x2 + xhairs->Length;
		xhairs->Buffer[1].y = 0;

		wgi_wldvdc(2, xhairs->Buffer, xhairs->Mapped);
	}
	else
	{
		wgi_wldvdc(1, xhairs->Buffer, xhairs->Mapped);
	}

	xhairs->Output[0] = &xhairs->Buffer[0];

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_bounds_draw(data, bounds, draw)
void *data;
wgiRubberBounds_t *bounds;
long draw;
{
	long mx;
	long my;
	long x1 = bounds->Mapped[0].x;
	long y1 = bounds->Mapped[1].y;
	long dx = bounds->Mapped[1].x - x1;
	long dy = bounds->Mapped[0].y - y1;

	if(bounds->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		bounds->ColorI, bounds->PlaneI,
		bounds->DrawOp, bounds->Dashed);

	wgi_device_rubber_bounds(x1, y1, dx, dy);

	switch(bounds->Spokes)
	{
	case wgiRubberSpokesCross:

		wgi_device_rubber_vector(x1, y1, x1+dx, y1+dy);
		wgi_device_rubber_vector(x1+dx, y1, x1, y1+dy);
	break;
	case wgiRubberSpokesScope:

		mx = dx / 2;
		my = dy / 2;
		wgi_device_rubber_vector(x1+mx, y1+1, x1+mx, y1+dy-1);
		wgi_device_rubber_vector(x1+1, y1+my, x1+dx-1, y1+my);
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

static long iwgi_rubber_bounds_calc(data, bounds, x1, y1, x2, y2)
void *data;
wgiRubberBounds_t *bounds;
double x1, y1, x2, y2;
{
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);

	if(bounds->Aspect > 0)
	{
		if((ax == 0) || ((ay/ax) > bounds->Aspect))
		{
			ax = ay / bounds->Aspect;
		}
		else
		{
			ay = ax * bounds->Aspect;
		}
	}

	if((ax == 0) || (ay == 0))
	{
		bounds->Output[0] = NULL;

		return (WGI_SUCCESS);
	}

	if(bounds->BaseIs == wgiRubberBaseIsCenter)
	{
		bounds->Buffer[0].x = x1 - ax;
		bounds->Buffer[0].y = y1 - ay;
		bounds->Buffer[1].x = x1 + ax;
		bounds->Buffer[1].y = y1 + ay;
	}
	else
	{
		bounds->Buffer[0].x = ((dx > 0) ? (x1) : (x1 - ax));
		bounds->Buffer[1].y = ((dy < 0) ? (y1) : (y1 + ay));
		bounds->Buffer[1].x = bounds->Buffer[0].x + ax;
		bounds->Buffer[0].y = bounds->Buffer[1].y - ay;
	}

	bounds->Output[0] = &bounds->Buffer[0];

	wgi_wldvdc(2, bounds->Buffer, bounds->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_vector_draw(data, vector, draw)
void *data;
wgiRubberVector_t *vector;
long draw;
{
	if(vector->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		vector->ColorI, vector->PlaneI,
		vector->DrawOp, vector->Dashed);

	wgi_device_rubber_vector(vector->Mapped[0].x,
		vector->Mapped[0].y,
		vector->Mapped[1].x,
		vector->Mapped[1].y);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_diagsnap(vector, x1, y1, x2, y2, dx, dy, ax, ay)
wgiRubberVector_t *vector;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	if(ax == ay)
	{
		vector->Buffer[1].x = x2;
		vector->Buffer[1].y = y2;
	}
	else if(ay < ax)
	{
		if(dx > 0)
		{
			vector->Buffer[1].x = x1-ay;
			vector->Buffer[1].y = y2;
		}
		else
		{
			vector->Buffer[1].x = x1+ay;
			vector->Buffer[1].y = y2;
		}
	}
	else
	{
		if(dy > 0)
		{
			vector->Buffer[1].x = x2;
			vector->Buffer[1].y = y1-ax;
		}
		else
		{
			vector->Buffer[1].x = x2;
			vector->Buffer[1].y = y1+ax;
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

static long iwgi_manhsnap(vector, x1, y1, x2, y2)
wgiRubberVector_t *vector;
double x1, y1, x2, y2;
{
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);

	switch(vector->SnapTo)
	{
	case wgiRubberSnapToManh:

		if (ax > ay)
		{
			vector->Buffer[1].x = x2;
			vector->Buffer[1].y = y1;
		}
		else
		{
			vector->Buffer[1].x = x1;
			vector->Buffer[1].y = y2;
		}
	break;

	case wgiRubberSnapToSemi:

		if((ax == 0) || (ay == 0) || (ax == ay))
		{
			vector->Buffer[1].x = x2;
			vector->Buffer[1].y = y2;
		}
		else if((ay > (ax * 2)) || (ax > (ay * 2)))
		{
			if(ax < ay)
			{
				vector->Buffer[1].x = x1;
				vector->Buffer[1].y = y2;
			}
			else
			{
				vector->Buffer[1].x = x2;
				vector->Buffer[1].y = y1;
			}
		}
		else
		{
			iwgi_diagsnap(vector, x1, y1, x2, y2, dx, dy, ax, ay);
		}
	break;
	case wgiRubberSnapToDiag:

		iwgi_diagsnap(vector, x1, y1, x2, y2, dx, dy, ax, ay);
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

static long iwgi_rubber_vector_calc(data, vector, x1, y1, x2, y2)
void *data;
wgiRubberVector_t *vector;
double x1, y1, x2, y2;
{
	if((x1 == x2) && (y1 == y2))
	{
		vector->Output[0] = NULL;

		return (WGI_SUCCESS);
	}

	vector->Buffer[0].x = x1;
	vector->Buffer[0].y = y1;

	switch(vector->SnapTo)
	{
	case wgiRubberSnapToFree:

		vector->Buffer[1].x = x2;
		vector->Buffer[1].y = y2;
	break;

	case wgiRubberSnapToHorz:

		vector->Buffer[1].x = x2;
		vector->Buffer[1].y = y1;
	break;

	case wgiRubberSnapToVert:

		vector->Buffer[1].x = x1;
		vector->Buffer[1].y = y2;
	break;

	default: iwgi_manhsnap(vector, x1, y1, x2, y2);
	break;
	}

	vector->Output[0] = &vector->Buffer[0];

	wgi_wldvdc(2, vector->Buffer, vector->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_mitred_draw(data, mitred, draw)
void *data;
wgiRubberMitred_t *mitred;
long draw;
{
	if(mitred->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		mitred->ColorI, mitred->PlaneI,
		mitred->DrawOp, mitred->Dashed);

	wgi_device_rubber_nlines(mitred->Mapped, mitred->Number);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_mitred_lockin(mitred, x1, y1, x2, y2, dx, dy, ax, ay)
wgiRubberMitred_t *mitred;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double r1;

	switch(mitred->LockIn)
	{
	case 0: case 3:

		mitred->Buffer[1].x = x2;
		mitred->Buffer[1].y = y1+(r1=ax);
	break;
	case 1: case 6:

		mitred->Buffer[1].x = x1+(r1=ay);
		mitred->Buffer[1].y = y2;
	break;
	case 2: case 5:

		mitred->Buffer[1].x = x1-(r1=ay);
		mitred->Buffer[1].y = y2;
	break;
	case 4: case 7:

		mitred->Buffer[1].x = x2;
		mitred->Buffer[1].y = y1-(r1=ax);
	break;
	}

	mitred->Mitred = 0;
	mitred->Number = 3;
	mitred->Length = r1 * M_SQRT2;

	mitred->Buffer[0].x = x1;
	mitred->Buffer[0].y = y1;
	mitred->Buffer[2].x = x2;
	mitred->Buffer[2].y = y2;

	mitred->Output[0] = &mitred->Buffer[0];
	mitred->Output[1] = &mitred->Buffer[1];
	mitred->Output[2] = NULL;

	wgi_wldvdc(3, mitred->Buffer, mitred->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_mitred_nolock(mitred, x1, y1, x2, y2, dx, dy, ax, ay)
wgiRubberMitred_t *mitred;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double r1;
	unsigned long octant;

	if(ax == ay)
	{
		octant = WGI_RUBBER_LOCKIN_NOT;
		mitred->Changed = (octant != mitred->PrevOct);
		mitred->PrevOct = octant;

		mitred->Mitred = 0;
		mitred->Number = 2;
		mitred->Length = ax * M_SQRT2;

		mitred->Buffer[0].x = x1;
		mitred->Buffer[0].y = y1;
		mitred->Buffer[1].x = x2;
		mitred->Buffer[1].y = y2;

		mitred->Output[0] = &mitred->Buffer[0];
		mitred->Output[1] = NULL;
		mitred->Output[2] = NULL;

		wgi_wldvdc(2, mitred->Buffer, mitred->Mapped);

		return (WGI_SUCCESS);
	}

	if(mitred->Locked)
	{
		octant = iwgi_calculate_lockin(dx, dy, ax, ay, &mitred->LockIn);

		if(octant != mitred->LockIn)
		{
			mitred->Changed = (octant != mitred->PrevOct);
			mitred->PrevOct = octant;

			iwgi_rubber_mitred_lockin(mitred, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}
	}
	else
	{
		octant = iwgi_calculate_octant(dx, dy, ax, ay);
	}

	mitred->Changed = (octant != mitred->PrevOct);
	mitred->PrevOct = octant;

	switch(octant)
	{
	case 0: case 7:

		mitred->Buffer[1].x = x2-(r1=ay);
		mitred->Buffer[1].y = y1;
	break;
	case 1: case 2:

		mitred->Buffer[1].x = x1;
		mitred->Buffer[1].y = y2-(r1=ax);
	break;
	case 3: case 4:

		mitred->Buffer[1].x = x2+(r1=ay);
		mitred->Buffer[1].y = y1;
	break;
	case 5: case 6:

		mitred->Buffer[1].x = x1;
		mitred->Buffer[1].y = y2+(r1=ax);
	break;
	}

	mitred->Mitred = 1;
	mitred->Number = 3;
	mitred->Length = r1 * M_SQRT2;

	mitred->Buffer[0].x = x1;
	mitred->Buffer[0].y = y1;
	mitred->Buffer[2].x = x2;
	mitred->Buffer[2].y = y2;

	mitred->Output[0] = &mitred->Buffer[0];
	mitred->Output[1] = &mitred->Buffer[1];
	mitred->Output[2] = NULL;

	wgi_wldvdc(3, mitred->Buffer, mitred->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_mitred_calc(data, mitred, x1, y1, x2, y2)
void *data;
wgiRubberMitred_t *mitred;
double x1, y1, x2, y2;
{
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);
	double r1 = mitred->MaxLen / M_SQRT2;
	unsigned long octant;

	if((ax == 0) || (ay == 0))
	{
		octant = WGI_RUBBER_LOCKIN_NOT;
		mitred->Changed = (octant != mitred->PrevOct);
		mitred->PrevOct = octant;

		if(ax == ay)
		{
			mitred->Mitred = WGI_RUBBER_MITRED_NOT;
			mitred->Number = 0;
			mitred->Length = 0;

			mitred->Output[0] = NULL;
			mitred->Output[1] = NULL;
			mitred->Output[2] = NULL;

			return (WGI_SUCCESS);
		}

		mitred->Mitred = WGI_RUBBER_MITRED_NOT;
		mitred->Number = 2;
		mitred->Length = 0;

		mitred->Buffer[0].x = x1;
		mitred->Buffer[0].y = y1;
		mitred->Buffer[1].x = x2;
		mitred->Buffer[1].y = y2;

		mitred->Output[0] = &mitred->Buffer[0];
		mitred->Output[1] = NULL;
		mitred->Output[2] = NULL;

		wgi_wldvdc(2, mitred->Buffer, mitred->Mapped);

		return (WGI_SUCCESS);
	}

	if((mitred->MaxLen == 0) || (ax < r1) || (ay < r1))
	{
		iwgi_rubber_mitred_nolock(mitred, x1, y1, x2, y2,
			dx, dy, ax, ay);

		return (WGI_SUCCESS);
	}

	if(mitred->Locked)
	{
		iwgi_calculate_lockin(dx, dy, ax, ay, &mitred->LockIn);
		octant = mitred->LockIn;
	}
	else
	{
		octant = iwgi_calculate_octant(dx, dy, ax, ay);
	}

	mitred->Changed = (octant != mitred->PrevOct);
	mitred->PrevOct = octant;

	switch(octant)
	{
	case 0:
		mitred->Buffer[1].x = x2-r1;
		mitred->Buffer[1].y = y1;
		mitred->Buffer[2].x = x2;
		mitred->Buffer[2].y = y1+r1;
	break;
	case 1:
		mitred->Buffer[1].x = x1;
		mitred->Buffer[1].y = y2-r1;
		mitred->Buffer[2].x = x1+r1;
		mitred->Buffer[2].y = y2;
	break;
	case 2:
		mitred->Buffer[1].x = x1;
		mitred->Buffer[1].y = y2-r1;
		mitred->Buffer[2].x = x1-r1;
		mitred->Buffer[2].y = y2;
	break;
	case 3:
		mitred->Buffer[1].x = x2+r1;
		mitred->Buffer[1].y = y1;
		mitred->Buffer[2].x = x2;
		mitred->Buffer[2].y = y1+r1;
	break;
	case 4:
		mitred->Buffer[1].x = x2+r1;
		mitred->Buffer[1].y = y1;
		mitred->Buffer[2].x = x2;
		mitred->Buffer[2].y = y1-r1;
	break;
	case 5:
		mitred->Buffer[1].x = x1;
		mitred->Buffer[1].y = y2+r1;
		mitred->Buffer[2].x = x1-r1;
		mitred->Buffer[2].y = y2;
	break;
	case 6:
		mitred->Buffer[1].x = x1;
		mitred->Buffer[1].y = y2+r1;
		mitred->Buffer[2].x = x1+r1;
		mitred->Buffer[2].y = y2;
	break;
	case 7:
		mitred->Buffer[1].x = x2-r1;
		mitred->Buffer[1].y = y1;
		mitred->Buffer[2].x = x2;
		mitred->Buffer[2].y = y1-r1;
	break;
	}
	
	mitred->Mitred = 1;
	mitred->Number = 4;
	mitred->Length = mitred->MaxLen;

	mitred->Buffer[0].x = x1;
	mitred->Buffer[0].y = y1;
	mitred->Buffer[3].x = x2;
	mitred->Buffer[3].y = y2;

	mitred->Output[0] = &mitred->Buffer[0];
	mitred->Output[1] = &mitred->Buffer[1];
	mitred->Output[2] = &mitred->Buffer[2];

	wgi_wldvdc(4, mitred->Buffer, mitred->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_bend90_draw(data, bend90, draw)
void *data;
wgiRubberBend90_t *bend90;
long draw;
{
	if(bend90->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		bend90->ColorI, bend90->PlaneI,
		bend90->DrawOp, bend90->Dashed);

	wgi_device_rubber_nlines(bend90->Mapped, bend90->Number);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_bend90_calc(data, bend90, x1, y1, x2, y2)
void *data;
wgiRubberBend90_t *bend90;
double x1, y1, x2, y2;
{
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);
	unsigned long octant;

	if((ax == 0) || (ay == 0))
	{
		octant = WGI_RUBBER_LOCKIN_NOT;
		bend90->Changed = (octant != bend90->PrevOct);
		bend90->PrevOct = octant;
		
		if(ax == ay)
		{
			bend90->Number = 0;

			bend90->Output[0] = NULL;
			bend90->Output[1] = NULL;

			return (WGI_SUCCESS);
		}

		bend90->Number = 2;

		bend90->Buffer[0].x = x1;
		bend90->Buffer[0].y = y1;
		bend90->Buffer[1].x = x2;
		bend90->Buffer[1].y = y2;

		bend90->Output[0] = &bend90->Buffer[0];
		bend90->Output[1] = NULL;

		wgi_wldvdc(2, bend90->Buffer, bend90->Mapped);

		return (WGI_SUCCESS);
	}

	if(bend90->Locked)
	{
		iwgi_calculate_lockin(dx, dy, ax, ay, &bend90->LockIn);
		octant = bend90->LockIn;
	}
	else
	{
		octant = iwgi_calculate_octant(dx, dy, ax, ay);
	}

	bend90->Changed = (octant != bend90->PrevOct);
	bend90->PrevOct = octant;

	switch(octant)
	{
	case 0: case 7: case 3: case 4:

		bend90->Buffer[1].x = x2;
		bend90->Buffer[1].y = y1;
	break;
	case 1: case 2: case 5: case 6:

		bend90->Buffer[1].x = x1;
		bend90->Buffer[1].y = y2;
	break;
	}

	bend90->Number = 3;

	bend90->Buffer[0].x = x1;
	bend90->Buffer[0].y = y1;
	bend90->Buffer[2].x = x2;
	bend90->Buffer[2].y = y2;

	bend90->Output[0] = &bend90->Buffer[0];
	bend90->Output[1] = &bend90->Buffer[1];

	wgi_wldvdc(3, bend90->Buffer, bend90->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc90_draw(data, larc90, draw)
void *data;
wgiRubberLarc90_t *larc90;
long draw;
{
	wgi_device_rubber_drawop(draw,
		larc90->ColorI, larc90->PlaneI,
		larc90->DrawOp, larc90->Dashed);

	if(larc90->Output[0])
	{
		wgi_device_rubber_vector(larc90->Mapped[2].x,
			larc90->Mapped[2].y,
			larc90->Mapped[3].x,
			larc90->Mapped[3].y);
	}

	if(larc90->Output[1])
	{
		long starta;
		long sweepa;
		long r2 = larc90->Mapped[1].x - larc90->Mapped[0].x;

		starta = ((larc90->StartA * 64.0) + 0.5);

		if(larc90->SweepA < 0)
		{
			sweepa = ((larc90->SweepA * 64.0) - 0.5);
		}
		else
		{
			sweepa = ((larc90->SweepA * 64.0) + 0.5);
		}

		wgi_device_rubber_arcbox(larc90->Mapped[0].x,
			larc90->Mapped[0].y, r2, r2, starta, sweepa);
	}

	if(larc90->Output[2])
	{
		wgi_device_rubber_vector(larc90->Mapped[5].x,
			larc90->Mapped[5].y,
			larc90->Mapped[6].x,
			larc90->Mapped[6].y);
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

static long iwgi_rubber_larc90_lockin(larc90, x1, y1, x2, y2, dx, dy, ax, ay)
wgiRubberLarc90_t *larc90;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double cx;
	double cy;
	double r1;

	switch(larc90->LockIn)
	{
	case 0:
		larc90->Buffer[5].y = cy = y1+(r1=ax);
		larc90->Buffer[5].x = x2; cx = x1;
		larc90->StartA = 270.0;
		larc90->SweepA = 90.0;
	break;
	case 1:
		larc90->Buffer[5].x = cx = x1+(r1=ay);
		larc90->Buffer[5].y = y2; cy = y1;
		larc90->StartA = 180.0;
		larc90->SweepA = -90.0;
	break;
	case 2:
		larc90->Buffer[5].x = cx = x1-(r1=ay);
		larc90->Buffer[5].y = y2; cy = y1;
		larc90->StartA = 0.0;
		larc90->SweepA = 90.0;
	break;
	case 3:
		larc90->Buffer[5].y = cy = y1+(r1=ax);
		larc90->Buffer[5].x = x2; cx = x1;
		larc90->StartA = 270.0;
		larc90->SweepA = -90.0;
	break;
	case 4:
		larc90->Buffer[5].y = cy = y1-(r1=ax);
		larc90->Buffer[5].x = x2; cx = x1;
		larc90->StartA = 90.0;
		larc90->SweepA = 90.0;
	break;
	case 5:
		larc90->Buffer[5].x = cx = x1-(r1=ay);
		larc90->Buffer[5].y = y2; cy = y1;
		larc90->StartA = 0.0;
		larc90->SweepA = -90.0;
	break;
	case 6:
		larc90->Buffer[5].x = cx = x1+(r1=ay);
		larc90->Buffer[5].y = y2; cy = y1;
		larc90->StartA = 180.0;
		larc90->SweepA = 90.0;
	break;
	case 7:
		larc90->Buffer[5].y = cy = y1-(r1=ax);
		larc90->Buffer[5].x = x2; cx = x1;
		larc90->StartA = 90.0;
		larc90->SweepA = -90.0;
	break;
	}

	larc90->Radius = r1;

	larc90->Buffer[0].x = cx - r1;
	larc90->Buffer[0].y = cy + r1;
	larc90->Buffer[1].x = cx + r1;
	larc90->Buffer[1].y = cy + r1;
	larc90->Buffer[3].x = x1;	/* Buffer[2] is not needed */
	larc90->Buffer[3].y = y1;
	larc90->Buffer[4].x = cx;
	larc90->Buffer[4].y = cy;
	larc90->Buffer[6].x = x2;
	larc90->Buffer[6].y = y2;

	larc90->Output[0] = NULL;
	larc90->Output[1] = &larc90->Buffer[3];

	if(ax == ay)
	{
		larc90->Output[2] = NULL;
	}
	else
	{
		larc90->Output[2] = &larc90->Buffer[5];
	}

	wgi_wldvdc(7, larc90->Buffer, larc90->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc90_nolock(larc90, x1, y1, x2, y2, dx, dy, ax, ay)
wgiRubberLarc90_t *larc90;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double cx;
	double cy;
	double r1;
	unsigned long octant;

	if(larc90->Locked)
	{
		octant = iwgi_calculate_lockin(dx, dy, ax, ay, &larc90->LockIn);

		if(octant != larc90->LockIn)
		{
			larc90->Changed = (octant != larc90->PrevOct);
			larc90->PrevOct = octant;

			iwgi_rubber_larc90_lockin(larc90, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}
	}
	else
	{
		octant = iwgi_calculate_octant(dx, dy, ax, ay);
	}

	larc90->Changed = (octant != larc90->PrevOct);
	larc90->PrevOct = octant;

/* ?? not sure about octant when ax = ay ?? */

	switch(octant)
	{
	case 0:
		larc90->Buffer[3].x = cx = x2-(r1=ay);
		larc90->Buffer[3].y = y1; cy = y1+r1;
		larc90->StartA = 270.0;
		larc90->SweepA = 90.0;
	break;
	case 1:
		larc90->Buffer[3].y = cy = y2-(r1=ax);
		larc90->Buffer[3].x = x1; cx = x1+r1;
		larc90->StartA = 180.0;
		larc90->SweepA = -90.0;
	break;
	case 2:
		larc90->Buffer[3].y = cy = y2-(r1=ax);
		larc90->Buffer[3].x = x1; cx = x1-r1;
		larc90->StartA = 0.0;
		larc90->SweepA = 90.0;
	break;
	case 3:
		larc90->Buffer[3].x = cx = x2+(r1=ay);
		larc90->Buffer[3].y = y1; cy = y1+r1;
		larc90->StartA = 270.0;
		larc90->SweepA = -90.0;
	break;
	case 4:
		larc90->Buffer[3].x = cx = x2+(r1=ay);
		larc90->Buffer[3].y = y1; cy = y1-r1;
		larc90->StartA = 90.0;
		larc90->SweepA = 90.0;
	break;
	case 5:
		larc90->Buffer[3].y = cy = y2+(r1=ax);
		larc90->Buffer[3].x = x1; cx = x1-r1;
		larc90->StartA = 0.0;
		larc90->SweepA = -90.0;
	break;
	case 6:
		larc90->Buffer[3].y = cy = y2+(r1=ax);
		larc90->Buffer[3].x = x1; cx = x1+r1;
		larc90->StartA = 180.0;
		larc90->SweepA = 90.0;
	break;
	case 7:
		larc90->Buffer[3].x = cx = x2-(r1=ay);
		larc90->Buffer[3].y = y1; cy = y1-r1;
		larc90->StartA = 90.0;
		larc90->SweepA = -90.0;
	break;
	}

	larc90->Radius = r1;

	larc90->Buffer[0].x = cx - r1;
	larc90->Buffer[0].y = cy + r1;
	larc90->Buffer[1].x = cx + r1;
	larc90->Buffer[1].y = cy + r1;
	larc90->Buffer[2].x = x1;
	larc90->Buffer[2].y = y1;
	larc90->Buffer[4].x = cx;
	larc90->Buffer[4].y = cy;
	larc90->Buffer[5].x = x2;	/* Buffer[5] is not mapped */
	larc90->Buffer[5].y = y2;

	if(ax == ay)
	{
/* ?? not sure about octant when ax = ay ?? */
		larc90->Output[0] = NULL;
	}
	else
	{
		larc90->Output[0] = &larc90->Buffer[2];
	}

	larc90->Output[1] = &larc90->Buffer[3];
	larc90->Output[2] = NULL;

	wgi_wldvdc(5, larc90->Buffer, larc90->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc90_calc(data, larc90, x1, y1, x2, y2)
void *data;
wgiRubberLarc90_t *larc90;
double x1, y1, x2, y2;
{
	double cx;
	double cy;
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);
	double r1 = larc90->MaxRad;
	unsigned long octant;

	if((ax == 0) || (ay == 0))
	{
		octant = WGI_RUBBER_LOCKIN_NOT;
		larc90->Changed = (octant != larc90->PrevOct);
		larc90->PrevOct = octant;

		if(ax == ay)
		{
			larc90->Radius = 0;
			larc90->StartA = 0;
			larc90->SweepA = 0;

			larc90->Output[0] = NULL;
			larc90->Output[1] = NULL;
			larc90->Output[2] = NULL;

			return (WGI_SUCCESS);
		}

		larc90->Radius = 0;
		larc90->StartA = 0;
		larc90->SweepA = 0;

		larc90->Buffer[2].x = x1;
		larc90->Buffer[2].y = y1;
		larc90->Buffer[3].x = x2;
		larc90->Buffer[3].y = y2;

		larc90->Output[0] = &larc90->Buffer[2];
		larc90->Output[1] = NULL;
		larc90->Output[2] = NULL;

		wgi_wldvdc(2, &larc90->Buffer[2], &larc90->Mapped[2]);

		return (WGI_SUCCESS);
	}

	if((r1 == 0) || (ax < r1) || (ay < r1))
	{
		iwgi_rubber_larc90_nolock(larc90, x1, y1, x2, y2,
			dx, dy, ax, ay);

		return (WGI_SUCCESS);
	}

	if(larc90->Locked)
	{
		iwgi_calculate_lockin(dx, dy, ax, ay, &larc90->LockIn);
		octant = larc90->LockIn;
	}
	else
	{
		octant = iwgi_calculate_octant(dx, dy, ax, ay);
	}

	larc90->Changed = (octant != larc90->PrevOct);
	larc90->PrevOct = octant;

	switch(octant)
	{
	case 0:
		larc90->Buffer[3].x = cx = x2-r1;
		larc90->Buffer[3].y = y1;
		larc90->Buffer[5].x = x2;
		larc90->Buffer[5].y = cy = y1+r1;
		larc90->StartA = 270.0;
		larc90->SweepA = 90.0;
	break;
	case 1:
		larc90->Buffer[3].x = x1;
		larc90->Buffer[3].y = cy = y2-r1;
		larc90->Buffer[5].x = cx = x1+r1;
		larc90->Buffer[5].y = y2;
		larc90->StartA = 180.0;
		larc90->SweepA = -90.0;
	break;
	case 2:
		larc90->Buffer[3].x = x1;
		larc90->Buffer[3].y = cy = y2-r1;
		larc90->Buffer[5].x = cx = x1-r1;
		larc90->Buffer[5].y = y2;
		larc90->StartA = 0.0;
		larc90->SweepA = 90.0;
	break;
	case 3:
		larc90->Buffer[3].x = cx = x2+r1;
		larc90->Buffer[3].y = y1;
		larc90->Buffer[5].x = x2;
		larc90->Buffer[5].y = cy = y1+r1;
		larc90->StartA = 270.0;
		larc90->SweepA = -90.0;
	break;
	case 4:
		larc90->Buffer[3].x = cx = x2+r1;
		larc90->Buffer[3].y = y1;
		larc90->Buffer[5].x = x2;
		larc90->Buffer[5].y = cy = y1-r1;
		larc90->StartA = 90.0;
		larc90->SweepA = 90.0;
	break;
	case 5:
		larc90->Buffer[3].x = x1;
		larc90->Buffer[3].y = cy = y2+r1;
		larc90->Buffer[5].x = cx = x1-r1;
		larc90->Buffer[5].y = y2;
		larc90->StartA = 0.0;
		larc90->SweepA = -90.0;
	break;
	case 6:
		larc90->Buffer[3].x = x1;
		larc90->Buffer[3].y = cy = y2+r1;
		larc90->Buffer[5].x = cx = x1+r1;
		larc90->Buffer[5].y = y2;
		larc90->StartA = 180.0;
		larc90->SweepA = 90.0;
	break;
	case 7:
		larc90->Buffer[3].x = cx = x2-r1;
		larc90->Buffer[3].y = y1;
		larc90->Buffer[5].x = x2;
		larc90->Buffer[5].y = cy = y1-r1;
		larc90->StartA = 90.0;
		larc90->SweepA = -90.0;
	break;
	}

	larc90->Radius = r1;

	larc90->Buffer[0].x = cx - r1;
	larc90->Buffer[0].y = cy + r1;
	larc90->Buffer[1].x = cx + r1;
	larc90->Buffer[1].y = cy + r1;
	larc90->Buffer[2].x = x1;
	larc90->Buffer[2].y = y1;
	larc90->Buffer[4].x = cx;
	larc90->Buffer[4].y = cy;
	larc90->Buffer[6].x = x2;
	larc90->Buffer[6].y = y2;

	larc90->Output[0] = &larc90->Buffer[2];
	larc90->Output[1] = &larc90->Buffer[3];
	larc90->Output[2] = &larc90->Buffer[5];

	wgi_wldvdc(7, larc90->Buffer, larc90->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45_draw(data, larc45, draw)
void *data;
wgiRubberLarc45_t *larc45;
long draw;
{
	wgi_device_rubber_drawop(draw,
		larc45->ColorI, larc45->PlaneI,
		larc45->DrawOp, larc45->Dashed);

	if(larc45->Output[0])
	{
		wgi_device_rubber_vector(larc45->Mapped[2].x,
			larc45->Mapped[2].y,
			larc45->Mapped[3].x,
			larc45->Mapped[3].y);
	}

	if(larc45->Output[1])
	{
		long starta;
		long sweepa;
		long r2 = larc45->Mapped[1].x - larc45->Mapped[0].x;

		starta = ((larc45->StartA * 64.0) + 0.5);

		if(larc45->SweepA < 0)
		{
			sweepa = ((larc45->SweepA * 64.0) - 0.5);
		}
		else
		{
			sweepa = ((larc45->SweepA * 64.0) + 0.5);
		}

		wgi_device_rubber_arcbox(larc45->Mapped[0].x,
			larc45->Mapped[0].y, r2, r2, starta, sweepa);
	}

	if(larc45->Output[2])
	{
		wgi_device_rubber_vector(larc45->Mapped[5].x,
			larc45->Mapped[5].y,
			larc45->Mapped[6].x,
			larc45->Mapped[6].y);
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

#define LARC45_SMALL	(0.29289321881345242726)	/* 1 - sqrt(2.0)/2.0 */
#define LARC45_LARGE	(0.41421356237309514547)	/*   sqrt(2.0) - 1   */

static long iwgi_rubber_larc45_fringe(larc45, x1, y1, x2, y2, dx, dy, ax, ay)
wgiRubberLarc45_t *larc45;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double cx;
	double cy;
	double ds;
	double dl;
	double r1;
	double t1;

	if(ax > ay)
	{
		ds = LARC45_SMALL * (r1 = ((dl = (ax-ay)) / LARC45_LARGE));
	}
	else
	{
		ds = LARC45_SMALL * (r1 = ((dl = (ay-ax)) / LARC45_LARGE));
	}

	switch(iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0:
		t1 = x2-ay;
		cx = t1-dl;
		cy = y1+r1;
		larc45->Buffer[5].x = t1+ds;
		larc45->Buffer[5].y = y1+ds;
		larc45->StartA = 270.0;
		larc45->SweepA = 45.0;
	break;
	case 1:
		t1 = y2-ax;
		cy = t1-dl;
		cx = x1+r1;
		larc45->Buffer[5].x = x1+ds;
		larc45->Buffer[5].y = t1+ds;
		larc45->StartA = 180.0;
		larc45->SweepA = -45.0;
	break;
	case 2:
		t1 = y2-ax;
		cy = t1-dl;
		cx = x1-r1;
		larc45->Buffer[5].x = x1-ds;
		larc45->Buffer[5].y = t1+ds;
		larc45->StartA = 0.0;
		larc45->SweepA = 45.0;
	break;
	case 3:
		t1 = x2+ay;
		cx = t1+dl;
		cy = y1+r1;
		larc45->Buffer[5].x = t1-ds;
		larc45->Buffer[5].y = y1+ds;
		larc45->StartA = 270.0;
		larc45->SweepA = -45.0;
	break;
	case 4:
		t1 = x2+ay;
		cx = t1+dl;
		cy = y1-r1;
		larc45->Buffer[5].x = t1-ds;
		larc45->Buffer[5].y = y1-ds;
		larc45->StartA = 90.0;
		larc45->SweepA = 45.0;
	break;
	case 5:
		t1 = y2+ax;
		cy = t1+dl;
		cx = x1-r1;
		larc45->Buffer[5].x = x1-ds;
		larc45->Buffer[5].y = t1-ds;
		larc45->StartA = 0.0;
		larc45->SweepA = -45.0;
	break;
	case 6:
		t1 = y2+ax;
		cy = t1+dl;
		cx = x1+r1;
		larc45->Buffer[5].x = x1+ds;
		larc45->Buffer[5].y = t1-ds;
		larc45->StartA = 180.0;
		larc45->SweepA = 45.0;
	break;
	case 7:
		t1 = x2-ay;
		cx = t1-dl;
		cy = y1-r1;
		larc45->Buffer[5].x = t1+ds;
		larc45->Buffer[5].y = y1-ds;
		larc45->StartA = 90.0;
		larc45->SweepA = -45.0;
	break;
	}

	larc45->Radius = r1;

	larc45->Buffer[0].x = cx - r1;
	larc45->Buffer[0].y = cy + r1;
	larc45->Buffer[1].x = cx + r1;
	larc45->Buffer[1].y = cy + r1;
	larc45->Buffer[3].x = x1;	/* Buffer[2] is not needed */
	larc45->Buffer[3].y = y1;
	larc45->Buffer[4].x = cx;
	larc45->Buffer[4].y = cy;
	larc45->Buffer[6].x = x2;
	larc45->Buffer[6].y = y2;

	larc45->Output[0] = NULL;
	larc45->Output[1] = &larc45->Buffer[3];
	larc45->Output[2] = &larc45->Buffer[5];

        wgi_wldvdc(7, larc45->Buffer, larc45->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45_nolock(larc45, x1, y1, x2, y2, dx, dy, ax, ay)
wgiRubberLarc45_t *larc45;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double cx;
	double cy;
	double ds;
	double dl;
	double r1;
	double t1;
	double t2;

	if(ax > ay)
	{
		dl = LARC45_LARGE * (r1 = ((ds = ay) / LARC45_SMALL));

		if((ax-ay) < dl)
		{
			iwgi_rubber_larc45_fringe(larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}
	}
	else
	{
		dl = LARC45_LARGE * (r1 = ((ds = ax) / LARC45_SMALL));

		if((ay-ax) < dl)
		{
			iwgi_rubber_larc45_fringe(larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}
	}

	switch(iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0:
		t1 = x2-ay;
		t2 = t1-dl;
		cx = t2;
		cy = y1+r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->StartA = 270.0;
		larc45->SweepA = 45.0;
	break;
	case 1:
		t1 = y2-ax;
		t2 = t1-dl;
		cx = x1+r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->StartA = 180.0;
		larc45->SweepA = -45.0;
	break;
	case 2:
		t1 = y2-ax;
		t2 = t1-dl;
		cx = x1-r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->StartA = 0.0;
		larc45->SweepA = 45.0;
	break;
	case 3:
		t1 = x2+ay;
		t2 = t1+dl;
		cx = t2;
		cy = y1+r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->StartA = 270.0;
		larc45->SweepA = -45.0;
	break;
	case 4:
		t1 = x2+ay;
		t2 = t1+dl;
		cx = t2;
		cy = y1-r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->StartA = 90.0;
		larc45->SweepA = 45.0;
	break;
	case 5:
		t1 = y2+ax;
		t2 = t1+dl;
		cx = x1-r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->StartA = 0.0;
		larc45->SweepA = -45.0;
	break;
	case 6:
		t1 = y2+ax;
		t2 = t1+dl;
		cx = x1+r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->StartA = 180.0;
		larc45->SweepA = 45.0;
	break;
	case 7:
		t1 = x2-ay;
		t2 = t1-dl;
		cx = t2;
		cy = y1-r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->StartA = 90.0;
		larc45->SweepA = -45.0;
	break;
	}

	larc45->Radius = r1;

	larc45->Buffer[0].x = cx - r1;
	larc45->Buffer[0].y = cy + r1;
	larc45->Buffer[1].x = cx + r1;
	larc45->Buffer[1].y = cy + r1;
	larc45->Buffer[2].x = x1;
	larc45->Buffer[2].y = y1;
	larc45->Buffer[4].x = cx;
	larc45->Buffer[4].y = cy;
	larc45->Buffer[5].x = x2; /* this point is not mapped */
	larc45->Buffer[5].y = y2;

	larc45->Output[0] = &larc45->Buffer[2];
	larc45->Output[1] = &larc45->Buffer[3];
	larc45->Output[2] = NULL;

        wgi_wldvdc(5, larc45->Buffer, larc45->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45_calc(data, larc45, x1, y1, x2, y2)
void *data;
wgiRubberLarc45_t *larc45;
double x1, y1, x2, y2;
{
	double cx;
	double cy;
	double t1;
	double t2;
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);
	double r1 = larc45->MaxRad;
	double ds = r1 * LARC45_SMALL;
	double dl = r1 * LARC45_LARGE;

	if((ax == 0) || (ay == 0) || (ax == ay))
	{
		if((ax == 0) && (ay == 0))
		{
			larc45->Radius = 0;
			larc45->StartA = 0;
			larc45->SweepA = 0;

			larc45->Output[0] = NULL;
			larc45->Output[1] = NULL;
			larc45->Output[2] = NULL;

			return (WGI_SUCCESS);
		}

		larc45->Radius = 0;
		larc45->StartA = 0;
		larc45->SweepA = 0;

		larc45->Buffer[2].x = x1;
		larc45->Buffer[2].y = y1;
		larc45->Buffer[3].x = x2;
		larc45->Buffer[3].y = y2;

		larc45->Output[0] = &larc45->Buffer[2];
		larc45->Output[1] = NULL;
		larc45->Output[2] = NULL;

		wgi_wldvdc(2, &larc45->Buffer[2], &larc45->Mapped[2]);

		return (WGI_SUCCESS);
	}

	if(ax > ay)
	{
		if((ax-ay) < dl)
		{
			iwgi_rubber_larc45_fringe(larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}

		if((r1 == 0) || (ay < ds))
		{
			iwgi_rubber_larc45_nolock(larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}
	}
	else
	{
		if((ay-ax) < dl)
		{
			iwgi_rubber_larc45_fringe(larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}

		if((r1 == 0) || (ax < ds))
		{
			iwgi_rubber_larc45_nolock(larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);

			return (WGI_SUCCESS);
		}
	}

	switch(iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0:
		t1 = x2-ay;
		t2 = t1-dl;
		cx = t2;
		cy = y1+r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->Buffer[5].x = t1+ds;
		larc45->Buffer[5].y = y1+ds;
		larc45->StartA = 270.0;
		larc45->SweepA = 45.0;
	break;
	case 1:
		t1 = y2-ax;
		t2 = t1-dl;
		cx = x1+r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->Buffer[5].x = x1+ds;
		larc45->Buffer[5].y = t1+ds;
		larc45->StartA = 180.0;
		larc45->SweepA = -45.0;
	break;
	case 2:
		t1 = y2-ax;
		t2 = t1-dl;
		cx = x1-r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->Buffer[5].x = x1-ds;
		larc45->Buffer[5].y = t1+ds;
		larc45->StartA = 0.0;
		larc45->SweepA = 45.0;
	break;
	case 3:
		t1 = x2+ay;
		t2 = t1+dl;
		cx = t2;
		cy = y1+r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->Buffer[5].x = t1-ds;
		larc45->Buffer[5].y = y1+ds;
		larc45->StartA = 270.0;
		larc45->SweepA = -45.0;
	break;
	case 4:
		t1 = x2+ay;
		t2 = t1+dl;
		cx = t2;
		cy = y1-r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->Buffer[5].x = t1-ds;
		larc45->Buffer[5].y = y1-ds;
		larc45->StartA = 90.0;
		larc45->SweepA = 45.0;
	break;
	case 5:
		t1 = y2+ax;
		t2 = t1+dl;
		cx = x1-r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->Buffer[5].x = x1-ds;
		larc45->Buffer[5].y = t1-ds;
		larc45->StartA = 0.0;
		larc45->SweepA = -45.0;
	break;
	case 6:
		t1 = y2+ax;
		t2 = t1+dl;
		cx = x1+r1;
		cy = t2;
		larc45->Buffer[3].x = x1;
		larc45->Buffer[3].y = t2;
		larc45->Buffer[5].x = x1+ds;
		larc45->Buffer[5].y = t1-ds;
		larc45->StartA = 180.0;
		larc45->SweepA = 45.0;
	break;
	case 7:
		t1 = x2-ay;
		t2 = t1-dl;
		cx = t2;
		cy = y1-r1;
		larc45->Buffer[3].x = t2;
		larc45->Buffer[3].y = y1;
		larc45->Buffer[5].x = t1+ds;
		larc45->Buffer[5].y = y1-ds;
		larc45->StartA = 90.0;
		larc45->SweepA = -45.0;
	break;
	}

	larc45->Radius = r1;

	larc45->Buffer[0].x = cx - r1;
	larc45->Buffer[0].y = cy + r1;
	larc45->Buffer[1].x = cx + r1;
	larc45->Buffer[1].y = cy + r1;
	larc45->Buffer[2].x = x1;
	larc45->Buffer[2].y = y1;
	larc45->Buffer[4].x = cx;
	larc45->Buffer[4].y = cy;
	larc45->Buffer[6].x = x2;
	larc45->Buffer[6].y = y2;

	larc45->Output[0] = &larc45->Buffer[2];
	larc45->Output[1] = &larc45->Buffer[3];
	larc45->Output[2] = &larc45->Buffer[5];

        wgi_wldvdc(7, larc45->Buffer, larc45->Mapped);

	return (WGI_SUCCESS);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*									     */
/*	start of: added by enno						     */
/*									     */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45rot_draw(data, larc45, draw)
void *data;
wgiRubberLarc45Rot_t *larc45;
long draw;
{
	wgi_device_rubber_drawop(draw,
		larc45->ColorI, larc45->PlaneI,
		larc45->DrawOp, larc45->Dashed);

	if(larc45->Output[0])
	{
		wgi_device_rubber_vector(larc45->Mapped[2].x,
			larc45->Mapped[2].y,
			larc45->Mapped[3].x,
			larc45->Mapped[3].y);
	}

	if(larc45->Output[1])
	{
		long starta;
		long sweepa;
		long r2 = larc45->Mapped[1].x - larc45->Mapped[0].x;

		starta = ((larc45->StartA * 64.0) + 0.5);

		if(larc45->SweepA < 0)
		{
			sweepa = ((larc45->SweepA * 64.0) - 0.5);
		}
		else
		{
			sweepa = ((larc45->SweepA * 64.0) + 0.5);
		}

		wgi_device_rubber_arcbox(larc45->Mapped[0].x,
			larc45->Mapped[0].y, r2, r2, starta, sweepa);
	}

	if(larc45->Output[2])
	{
		wgi_device_rubber_vector(larc45->Mapped[5].x,
			larc45->Mapped[5].y,
			larc45->Mapped[6].x,
			larc45->Mapped[6].y);
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

static long iwgi_rubber_larc45rot_line_arc (larc45, x1, y1, x2, y2, 
						dx, dy, ax, ay)
wgiRubberLarc45Rot_t *larc45;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double cx;
	double cy;
	double ds;
	double dl;
	double r1;
	double m;

	if(ax > ay)
	{
		ds = LARC45_SMALL * (r1 = ((dl = (ax-ay)) / LARC45_LARGE));
	}
	else
	{
		ds = LARC45_SMALL * (r1 = ((dl = (ay-ax)) / LARC45_LARGE));
	}

	m = dl+ds;

	switch(iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0:
		cx = x2;
		cy = y2-r1;
		larc45->Buffer[3].x = x2-m;
		larc45->Buffer[3].y = y2-ds;
		larc45->StartA = 135.0;
		larc45->SweepA = -45.0;
	break;
	case 1:
		cx = x2-r1;
		cy = y2;
		larc45->Buffer[3].x = x2-ds;
		larc45->Buffer[3].y = y2-m;
		larc45->StartA = 315.0;
		larc45->SweepA = 45.0;
	break;
	case 2:
		cx = x2+r1;
		cy = y2;
		larc45->Buffer[3].x = x2+ds;
		larc45->Buffer[3].y = y2-m;
		larc45->StartA = 225.0;
		larc45->SweepA = -45.0;
	break;
	case 3:
		cx = x2;
		cy = y2-r1;
		larc45->Buffer[3].x = x2+m;
		larc45->Buffer[3].y = y2-ds;
		larc45->StartA = 45.0;
		larc45->SweepA = 45.0;
	break;
	case 4:
		cx = x2;
		cy = y2+r1;
		larc45->Buffer[3].x = x2+m;
		larc45->Buffer[3].y = y2+ds;
		larc45->StartA = 315.0;
		larc45->SweepA = -45.0;
	break;
	case 5:
		cx = x2+r1;
		cy = y2;
		larc45->Buffer[3].x = x2+ds;
		larc45->Buffer[3].y = y2+m;
		larc45->StartA = 135.0;
		larc45->SweepA = 45.0;
	break;
	case 6:
		cx = x2-r1;
		cy = y2;
		larc45->Buffer[3].x = x2-ds;
		larc45->Buffer[3].y = y2+m;
		larc45->StartA = 45.0;
		larc45->SweepA = -45.0;
	break;
	case 7:
		cx = x2;
		cy = y2+r1;
		larc45->Buffer[3].x = x2-m;
		larc45->Buffer[3].y = y2+ds;
		larc45->StartA = 225.0;
		larc45->SweepA = 45.0;
	break;
	}

	larc45->Radius = r1;

	larc45->Buffer[0].x = cx - r1;
	larc45->Buffer[0].y = cy + r1;
	larc45->Buffer[1].x = cx + r1;
	larc45->Buffer[1].y = cy + r1;
	larc45->Buffer[2].x = x1;
	larc45->Buffer[2].y = y1;
	larc45->Buffer[4].x = cx;
	larc45->Buffer[4].y = cy;
	larc45->Buffer[5].x = x2;
	larc45->Buffer[5].y = y2;

	larc45->Output[0] = &larc45->Buffer[2];
	larc45->Output[1] = &larc45->Buffer[3];
	larc45->Output[2] = NULL;

        wgi_wldvdc(7, larc45->Buffer, larc45->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45rot_arc_line (larc45, x1, y1, x2, y2, 
						dx, dy, ax, ay)
wgiRubberLarc45Rot_t *larc45;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double cx;
	double cy;
	double r1;
	double m;

	if (ax > ay)
	{
	    m = ay / LARC45_LARGE;
	}
	else
	{
	    m = ax / LARC45_LARGE;
	}
	r1 = m * M_SQRT2;

	switch (iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0:
		cx = x1+m;
		cy = y2-r1;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 135.0;
		larc45->SweepA = -45.0;
	break;
	case 1:
		cx = x2-r1;
		cy = y1+m;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 315.0;
		larc45->SweepA = 45.0;
	break;
	case 2:
		cx = x2+r1;
		cy = y1+m;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 225.0;
		larc45->SweepA = -45.0;
	break;
	case 3:
		cx = x1-m;
		cy = y2-r1;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 45.0;
		larc45->SweepA = 45.0;
	break;
	case 4:
		cx = x1-m;
		cy = y2+r1;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 315.0;
		larc45->SweepA = -45.0;
	break;
	case 5:
		cx = x2+r1;
		cy = y1-m;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 135.0;
		larc45->SweepA = 45.0;
	break;
	case 6:
		cx = x2-r1;
		cy = y1-m;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 45.0;
		larc45->SweepA = -45.0;
	break;
	case 7:
		cx = x1+m;
		cy = y2+r1;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 225.0;
		larc45->SweepA = 45.0;
	break;
	}

	larc45->Radius = r1;

	larc45->Buffer[0].x = cx - r1;
	larc45->Buffer[0].y = cy + r1;
	larc45->Buffer[1].x = cx + r1;
	larc45->Buffer[1].y = cy + r1;

	larc45->Buffer[3].x = x1;
	larc45->Buffer[3].y = y1;

	larc45->Buffer[4].x = cx;
	larc45->Buffer[4].y = cy;
	larc45->Buffer[6].x = x2; /* this point is not mapped 
				     enno: it is now! */
	larc45->Buffer[6].y = y2;

	larc45->Output[0] = NULL;
	larc45->Output[1] = &larc45->Buffer[3];
	larc45->Output[2] = &larc45->Buffer[5];

        wgi_wldvdc(7, larc45->Buffer, larc45->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45rot_arc (larc45, x1, y1, x2, y2, 
						dx, dy, ax, ay)
wgiRubberLarc45Rot_t *larc45;
double x1, y1, x2, y2;
double dx, dy, ax, ay;
{
	double cx;
	double cy;
	double ds;
	double dl;
	double r1;
	double m;

	if (ax > ay)
	{
	    r1 = ay / LARC45_SMALL;
	}
	else
	{
	    r1 = ax / LARC45_SMALL;
	}

	switch (iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0:
		cx = x2;
		cy = y2-r1;
		larc45->StartA = 135.0;
		larc45->SweepA = -45.0;
	break;
	case 1:
		cx = x2-r1;
		cy = y2;
		larc45->StartA = 315.0;
		larc45->SweepA = 45.0;
	break;
	case 2:
		cx = x2+r1;
		cy = y2;
		larc45->StartA = 225.0;
		larc45->SweepA = -45.0;
	break;
	case 3:
		cx = x2;
		cy = y2-r1;
		larc45->StartA = 45.0;
		larc45->SweepA = 45.0;
	break;
	case 4:
		cx = x2;
		cy = y2+r1;
		larc45->StartA = 315.0;
		larc45->SweepA = -45.0;
	break;
	case 5:
		cx = x2+r1;
		cy = y2;
		larc45->StartA = 135.0;
		larc45->SweepA = 45.0;
	break;
	case 6:
		cx = x2-r1;
		cy = y2;
		larc45->StartA = 45.0;
		larc45->SweepA = -45.0;
	break;
	case 7:
		cx = x2;
		cy = y2+r1;
		larc45->StartA = 225.0;
		larc45->SweepA = 45.0;
	break;
	}

	larc45->Radius = r1;

	larc45->Buffer[0].x = cx - r1;
	larc45->Buffer[0].y = cy + r1;
	larc45->Buffer[1].x = cx + r1;
	larc45->Buffer[1].y = cy + r1;

	larc45->Buffer[3].x = x1;
	larc45->Buffer[3].y = y1;

	larc45->Buffer[4].x = cx;
	larc45->Buffer[4].y = cy;
	larc45->Buffer[5].x = x2;
	larc45->Buffer[5].y = y2;

	larc45->Output[0] = NULL;
	larc45->Output[1] = &larc45->Buffer[3];
	larc45->Output[2] = NULL;

        wgi_wldvdc(6, larc45->Buffer, larc45->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_larc45rot_calc(data, larc45, x1, y1, x2, y2)
void *data;
wgiRubberLarc45_t *larc45;
double x1, y1, x2, y2;
{
	double cx;
	double cy;
	double t1;
	double t2;
	double rad, rc;
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);
	double r1 = larc45->MaxRad;
	double ds = r1 * LARC45_SMALL;
	double dl = r1 * LARC45_LARGE;

	static double TWO_MINUS_SQRT_2 = 2.0 - M_SQRT2;
	static double TWO_PLUS_SQRT_2 = 2.0 + M_SQRT2;

	if((ax == 0) || (ay == 0) || (ax == ay))
	{
		if((ax == 0) && (ay == 0))
		{
			larc45->Radius = 0;
			larc45->StartA = 0;
			larc45->SweepA = 0;

			larc45->Output[0] = NULL;
			larc45->Output[1] = NULL;
			larc45->Output[2] = NULL;

			return (WGI_SUCCESS);
		}

		larc45->Radius = 0;
		larc45->StartA = 0;
		larc45->SweepA = 0;

		larc45->Buffer[2].x = x1;
		larc45->Buffer[2].y = y1;
		larc45->Buffer[3].x = x2;
		larc45->Buffer[3].y = y2;

		larc45->Output[0] = &larc45->Buffer[2];
		larc45->Output[1] = NULL;
		larc45->Output[2] = NULL;

		wgi_wldvdc(2, &larc45->Buffer[2], &larc45->Mapped[2]);

		return (WGI_SUCCESS);
	}

	if (ax > ay)
	{
	    rc = ay / ax;
	    if (rc > LARC45_LARGE)
	    {
		rad = ax * TWO_MINUS_SQRT_2;
		if (r1 == 0.0 || rad <= r1)
		{
		    iwgi_rubber_larc45rot_line_arc (larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);
		    return (WGI_SUCCESS);
		}
	    }
	    else if (rc == LARC45_LARGE)
	    {
		rad = ay / LARC45_SMALL;
		if (r1 == 0.0 || rad <= r1)
		{
		    /* just one arc, no lines */
		    iwgi_rubber_larc45rot_arc (larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);
		    return (WGI_SUCCESS);
		}
	    }
	    else	/* rc > LARC45_LARGE */
	    {
		rad = ay * TWO_PLUS_SQRT_2;
		if (r1 == 0.0 || rad <= r1)
		{
		    iwgi_rubber_larc45rot_arc_line (larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);
		    return (WGI_SUCCESS);
		}
	    }
	}
	else
	{
	    rc = ax / ay;
	    if (rc > LARC45_LARGE)
	    {
		rad = ay * TWO_MINUS_SQRT_2;
		if (r1 == 0.0 || rad <= r1)
		{
		    iwgi_rubber_larc45rot_line_arc (larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);
		    return (WGI_SUCCESS);
		}
	    }
	    else if (rc == LARC45_LARGE)
	    {
		rad = ax / LARC45_SMALL;
		if (r1 == 0.0 || rad <= r1)
		{
		    /* just one arc, no lines */
		    iwgi_rubber_larc45rot_arc (larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);
		    return (WGI_SUCCESS);
		}
	    }
	    else	/* rc > LARC45_LARGE */
	    {
		rad = ax * TWO_PLUS_SQRT_2;
		if (r1 == 0.0 || rad <= r1)
		{
		    iwgi_rubber_larc45rot_arc_line (larc45, x1, y1, x2, y2,
				dx, dy, ax, ay);
		    return (WGI_SUCCESS);
		}
	    }
	}

	switch(iwgi_calculate_octant(dx, dy, ax, ay))
	{
	case 0:
		t1 = x1+ay;
		cx = t1+dl;
		cy = y2-r1;
		larc45->Buffer[3].x = t1-ds;
		larc45->Buffer[3].y = y2-ds;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 135.0;
		larc45->SweepA = -45.0;
	break;
	case 1:
		t1 = y1+ax;
		cx = x2-r1;
		cy = t1+dl;
		larc45->Buffer[3].x = x2-ds;
		larc45->Buffer[3].y = t1-ds;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 315.0;
		larc45->SweepA = 45.0;
	break;
	case 2:
		t1 = y1+ax;
		cx = x2+r1;
		cy = t1+dl;
		larc45->Buffer[3].x = x2+ds;
		larc45->Buffer[3].y = t1-ds;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 225.0;
		larc45->SweepA = -45.0;
	break;
	case 3:
		t1 = x1-ay;
		cx = t1-dl;
		cy = y2-r1;
		larc45->Buffer[3].x = t1+ds;
		larc45->Buffer[3].y = y2-ds;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 45.0;
		larc45->SweepA = 45.0;
	break;
	case 4:
		t1 = x1-ay;
		cx = t1-dl;
		cy = y2+r1;
		larc45->Buffer[3].x = t1+ds;
		larc45->Buffer[3].y = y2+ds;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 315.0;
		larc45->SweepA = -45.0;
	break;
	case 5:
		t1 = y1-ax;
		cx = x2+r1;
		cy = t1-dl;
		larc45->Buffer[3].x = x2+ds;
		larc45->Buffer[3].y = t1+ds;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 135.0;
		larc45->SweepA = 45.0;
	break;
	case 6:
		t1 = y1-ax;
		cx = x2-r1;
		cy = t1-dl;
		larc45->Buffer[3].x = x2-ds;
		larc45->Buffer[3].y = t1+ds;
		larc45->Buffer[5].x = x2;
		larc45->Buffer[5].y = cy;
		larc45->StartA = 45.0;
		larc45->SweepA = -45.0;
	break;
	case 7:
		t1 = x1+ay;
		cx = t1+dl;
		cy = y2+r1;
		larc45->Buffer[3].x = t1-ds;
		larc45->Buffer[3].y = y2+ds;
		larc45->Buffer[5].x = cx;
		larc45->Buffer[5].y = y2;
		larc45->StartA = 225.0;
		larc45->SweepA = 45.0;
	break;
	}

	larc45->Radius = r1;

	larc45->Buffer[0].x = cx - r1;
	larc45->Buffer[0].y = cy + r1;
	larc45->Buffer[1].x = cx + r1;
	larc45->Buffer[1].y = cy + r1;

	larc45->Buffer[2].x = x1;
	larc45->Buffer[2].y = y1;
	larc45->Buffer[4].x = cx;
	larc45->Buffer[4].y = cy;
	larc45->Buffer[6].x = x2;
	larc45->Buffer[6].y = y2;

	larc45->Output[0] = &larc45->Buffer[2];
	larc45->Output[1] = &larc45->Buffer[3];
	larc45->Output[2] = &larc45->Buffer[5];

        wgi_wldvdc(7, larc45->Buffer, larc45->Mapped);

	return (WGI_SUCCESS);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*									     */
/*	end of: added by enno						     */
/*									     */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_arc3pt_draw(data, arc3pt, draw)
void *data;
wgiRubberArc3pt_t *arc3pt;
long draw;
{
	long starta;
	long sweepa;
	long r2 = arc3pt->Mapped[2].x - arc3pt->Mapped[1].x;

	if((arc3pt->Output[0] == NULL) && (arc3pt->Output[1] == NULL))
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		arc3pt->ColorI, arc3pt->PlaneI,
		arc3pt->DrawOp, arc3pt->Dashed);

	if(arc3pt->Output[0] == NULL)
	{
		wgi_device_rubber_vector(arc3pt->Mapped[0].x,
			arc3pt->Mapped[0].y,
			arc3pt->Mapped[1].x,
			arc3pt->Mapped[1].y);

		return (WGI_SUCCESS);
	}

	starta = ((arc3pt->StartA * 64.0) + 0.5);

	if(arc3pt->SweepA < 0)
	{
		sweepa = ((arc3pt->SweepA * 64.0) - 0.5);
	}
	else
	{
		sweepa = ((arc3pt->SweepA * 64.0) + 0.5);
	}

	wgi_device_rubber_arcbox(arc3pt->Mapped[1].x,
		arc3pt->Mapped[1].y, r2, r2, starta, sweepa);

	switch(arc3pt->Spokes)
	{
	case wgiRubberSpokesScope:

		wgi_device_rubber_vector(arc3pt->Mapped[4].x,
			arc3pt->Mapped[4].y,
			arc3pt->Mapped[0].x,
			arc3pt->Mapped[0].y);
	/* fall-thru */
	case wgiRubberSpokesSlice:

		wgi_device_rubber_vector(arc3pt->Mapped[4].x,
			arc3pt->Mapped[4].y,
			arc3pt->Mapped[3].x,
			arc3pt->Mapped[3].y);
		wgi_device_rubber_vector(arc3pt->Mapped[4].x,
			arc3pt->Mapped[4].y,
			arc3pt->Mapped[5].x,
			arc3pt->Mapped[5].y);
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

#define WGI_180_PI (57.29577951308232286465)	/* 180/pi */

static double iwgi_vector_angle(x1, y1, x2, y2)
double x1, y1, x2, y2;
{
	double theta;
	double dx = (x2-x1);
	double dy = (y2-y1);

	if(dy == 0)
	{
		return ((dx > 0) ? (0.000) : (180.0));
	}

	if(dx == 0)
	{
		return ((dy > 0) ? (90.00) : (270.0));
	}

	theta = WGI_180_PI * atan2(dy, dx);

	return ((theta < 0) ? (theta + 360.0) : (theta));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_arc3pt_calc(data, arc3pt, x1, y1, x2, y2)
void *data;
wgiRubberArc3pt_t *arc3pt;
double x1, y1, x2, y2;
{
	double x3, y3;
	double xc, yc;
	double a, b, c, d;
	double i1, i2, i3;
	double sa, ma, ea;

	switch(arc3pt->LastIs)
	{
	case wgiRubberLastIsMid:

		x3 = arc3pt->Second.x;
		y3 = arc3pt->Second.y;
	break;
	case wgiRubberLastIsEnd:

		x3 = x2; x2 = arc3pt->Second.x;
		y3 = y2; y2 = arc3pt->Second.y;
	break;
	}

	/*
		Scale the three points by shifting the starting point to 0,0
	*/

	a = x2 - x1;
	b = y2 - y1;
	c = x3 - x1;
	d = y3 - y1;

	/*
		The formula for finding the center (x,y) of the circle
		given the three points (0,0), (a,b) and (c,d) is:

				2    2        2    2
			x = d (a  + b ) - b (c  + d )
			    -------------------------
				  2 (ad - bc)

				2    2        2    2
			y = a (c  + d ) - c (a  + b )
			    -------------------------
				  2 (ad - bc)
	*/

	i3 = 2.0 * ((a * d) - (b * c));

	/*
		Test for co-linear points
	*/

	if (fabs(i3) < 0.00001)
	{
		arc3pt->Radius = 0;
		arc3pt->StartA = 0;
		arc3pt->SweepA = 0;

		arc3pt->Buffer[0].x = x1;
		arc3pt->Buffer[0].y = y1;
		arc3pt->Buffer[1].x = x3;
		arc3pt->Buffer[1].y = y3;

		arc3pt->Output[0] = NULL;
		arc3pt->Output[1] = &arc3pt->Buffer[0];

		wgi_wldvdc(2, arc3pt->Buffer, arc3pt->Mapped);

		return (WGI_SUCCESS);
	}

	i1 = (a * a) + (b * b);
	i2 = (c * c) + (d * d);

	xc = ((d * i1) - (b * i2)) / i3;
	yc = ((a * i2) - (c * i1)) / i3;

	if((arc3pt->Radius = sqrt((xc * xc)+(yc * yc))) == 0)
	{
		arc3pt->StartA = 0;
		arc3pt->SweepA = 0;

		arc3pt->Output[0] = NULL;
		arc3pt->Output[1] = NULL;

		return (WGI_SUCCESS);
	}

	/*
		Re-scale the center back to the original coordinate system
	*/

	xc += x1;
	yc += y1;

	/*
		Get angle data.
	*/

	sa = iwgi_vector_angle(xc, yc, x1, y1);
	ma = iwgi_vector_angle(xc, yc, x2, y2);
	ea = iwgi_vector_angle(xc, yc, x3, y3);

	/*
		Need to test mid-angle within a range between the start
		and end angles that does NOT cross over from 0 to 360.
	*/

	arc3pt->StartA = sa;

	if(ea>sa)
	{
		if((ma<ea) && (ma>sa))
		{
			arc3pt->SweepA = ea-sa;
		}
		else
		{
			arc3pt->SweepA = (ea-360.0)-sa;
		}
	}
	else
	{
		if((ma>ea) && (ma<sa))
		{
			arc3pt->SweepA = ea-sa;
		}
		else
		{
			arc3pt->SweepA = (ea+360.0)-sa;
		}
	}
	
	if(arc3pt->SweepA == 0)
	{
		arc3pt->Radius = 0;
		arc3pt->StartA = 0;

		arc3pt->Output[0] = NULL;
		arc3pt->Output[1] = NULL;

		return (WGI_SUCCESS);
	}

	/*
		The first 3 points are control points that are not passed
		back in Output[0]
	*/

	arc3pt->Buffer[0].x = x2;
	arc3pt->Buffer[0].y = y2;
	arc3pt->Buffer[1].x = xc - arc3pt->Radius;
	arc3pt->Buffer[1].y = yc + arc3pt->Radius;
	arc3pt->Buffer[2].x = xc + arc3pt->Radius;
	arc3pt->Buffer[2].y = yc + arc3pt->Radius;

	/*
		The next 3 points are start, center & end.  They need to
		passed back in this order.
	*/

	arc3pt->Buffer[3].x = x1;
	arc3pt->Buffer[3].y = y1;
	arc3pt->Buffer[4].x = xc;
	arc3pt->Buffer[4].y = yc;
	arc3pt->Buffer[5].x = x3;
	arc3pt->Buffer[5].y = y3;

	arc3pt->Output[0] = &arc3pt->Buffer[3];
	arc3pt->Output[1] = NULL;

	switch(arc3pt->Spokes)
	{
	case wgiRubberSpokesScope:

		wgi_wldvdc(6, &arc3pt->Buffer[0], &arc3pt->Mapped[0]);
	break;
	case wgiRubberSpokesSlice:

		wgi_wldvdc(5, &arc3pt->Buffer[1], &arc3pt->Mapped[1]);
	break;
	default:

		wgi_wldvdc(4, &arc3pt->Buffer[1], &arc3pt->Mapped[1]);
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

static long iwgi_rubber_circle_draw(data, circle, draw)
void *data;
wgiRubberCircle_t *circle;
long draw;
{
	long dx;
	long dy;
	long x1 = circle->Mapped[0].x;
	long y1 = circle->Mapped[0].y;
	long x2 = circle->Mapped[1].x;
	long y2 = circle->Mapped[1].y;
	long r1 = circle->Mapped[2].x - x1;

	if(circle->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		circle->ColorI, circle->PlaneI,
		circle->DrawOp, circle->Dashed);

	wgi_device_rubber_arcbox(x1-r1, y1-r1, r1*2, r1*2, 0, 360*64);

	switch(circle->Spokes)
	{
	case wgiRubberSpokesSlice:

		wgi_device_rubber_vector(x1, y1, x2, y2);
	break;
	case wgiRubberSpokesCross:

		dx = (x2-x1);
		dy = (y2-y1);
		wgi_device_rubber_vector(x1-dx, y1-dy, x1+dx, y1+dy);
		wgi_device_rubber_vector(x1-dy, y1+dx, x1+dy, y1-dx);
	break;
	case wgiRubberSpokesScope:

		wgi_device_rubber_vector(x1-r1+1, y1, x1+r1-1, y1);
		wgi_device_rubber_vector(x1, y1-r1+1, x1, y1+r1-1);
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

static long iwgi_rubber_circle_calc(data, circle, x1, y1, x2, y2)
void *data;
wgiRubberCircle_t *circle;
double x1, y1, x2, y2;
{
	long dx = (x2-x1);
	long dy = (y2-y1);

	if((circle->Radius = sqrt((dx * dx) + (dy * dy))) == 0)
	{
		circle->Output[0] = NULL;

		return (WGI_SUCCESS);
	}

	circle->Buffer[0].x = x1;
	circle->Buffer[0].y = y1;
	circle->Buffer[1].x = x2;
	circle->Buffer[1].y = y2;
	circle->Buffer[2].x = x1 + circle->Radius;
	circle->Buffer[2].y = 0;

	circle->Output[0] = &circle->Buffer[0];

	wgi_wldvdc(3, circle->Buffer, circle->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_nshape_draw(data, nshape, draw)
void *data;
wgiRubberNshape_t *nshape;
long draw;
{
	long i;

	if(nshape->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		nshape->ColorI, nshape->PlaneI,
		nshape->DrawOp, nshape->Dashed);

	wgi_device_rubber_nlines(&nshape->Mapped[1], nshape->Nsides+1);

	if(nshape->Spokes == wgiRubberSpokesSlice)
	{
		for(i = 1; i <= nshape->Nsides; i++)
		{
			wgi_device_rubber_vector(nshape->Mapped[0].x,
				nshape->Mapped[0].y,
				nshape->Mapped[i].x,
				nshape->Mapped[i].y);
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

static long iwgi_rubber_nshape_calc(data, nshape, x1, y1, x2, y2)
void *data;
wgiRubberNshape_t *nshape;
double x1, y1, x2, y2;
{
	long i;

	if((nshape->Nsides < 3) || ((x1 == x2) && (y1 == y2)))
	{
		for(i = 0; i < nshape->Nsides; i++)
		{
			nshape->Output[i] = NULL;
		}

		return (WGI_SUCCESS);
	}

	nshape->Buffer[0].x = x1;
	nshape->Buffer[0].y = y1;
	nshape->Buffer[1].x = nshape->Buffer[nshape->Nsides+1].x = x2;
	nshape->Buffer[1].y = nshape->Buffer[nshape->Nsides+1].y = y2;

	for(i = 2; i <= nshape->Nsides; i++)
	{
		nshape->Buffer[i].x = x2;
		nshape->Buffer[i].y = y2;
		iwgi_xfrm(1, nshape->Output[i-1] = &nshape->Buffer[i],
			nshape->Buffer, (360 * (i-1)) / nshape->Nsides, 0);
	}

	nshape->Output[0] = &nshape->Buffer[1];

	if(nshape->Spokes == wgiRubberSpokesSlice)
	{
		wgi_wldvdc(nshape->Nsides+2,
			&nshape->Buffer[0], &nshape->Mapped[0]);
	}
	else
	{
		wgi_wldvdc(nshape->Nsides+1,
			&nshape->Buffer[1], &nshape->Mapped[1]);
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

static long iwgi_rubber_ellipt_draw(data, ellipt, draw)
void *data;
wgiRubberEllipt_t *ellipt;
long draw;
{
	long x1 = ellipt->Mapped[0].x;
	long y1 = ellipt->Mapped[1].y;
	long dx = ellipt->Mapped[1].x - x1;
	long dy = ellipt->Mapped[0].y - y1;

	if(ellipt->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		ellipt->ColorI, ellipt->PlaneI,
		ellipt->DrawOp, ellipt->Dashed);

	wgi_device_rubber_arcbox(x1, y1, dx, dy, 0, 360*64);

	if(ellipt->DoRect)
	{
		wgi_device_rubber_bounds(x1-1, y1-1, dx+2, dy+2);
	}

	if(ellipt->Spokes == wgiRubberSpokesScope)
	{
		long mx = dx / 2;
		long my = dy / 2;
		wgi_device_rubber_vector(x1+mx, y1+1, x1+mx, y1+dy-1);
		wgi_device_rubber_vector(x1+1, y1+my, x1+dx-1, y1+my);
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

static long iwgi_rubber_ellipt_calc(data, ellipt, x1, y1, x2, y2)
void *data;
wgiRubberEllipt_t *ellipt;
double x1, y1, x2, y2;
{
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);

	if(ellipt->Aspect > 0)
	{
		if((ax == 0) || ((ay/ax) > ellipt->Aspect))
		{
			ax = ay / ellipt->Aspect;
		}
		else
		{
			ay = ax * ellipt->Aspect;
		}
	}

	if((ax == 0) || (ay == 0))
	{
		ellipt->Output[0] = NULL;

		return (WGI_SUCCESS);
	}

	if(ellipt->BaseIs == wgiRubberBaseIsCenter)
	{
		ellipt->Buffer[0].x = x1 - ax;
		ellipt->Buffer[0].y = y1 - ay;
		ellipt->Buffer[1].x = x1 + ax;
		ellipt->Buffer[1].y = y1 + ay;
	}
	else
	{
		ellipt->Buffer[0].x = ((dx > 0) ? (x1) : (x1 - ax));
		ellipt->Buffer[1].y = ((dy < 0) ? (y1) : (y1 + ay));
		ellipt->Buffer[1].x = ellipt->Buffer[0].x + ax;
		ellipt->Buffer[0].y = ellipt->Buffer[1].y - ay;
	}

	ellipt->Output[0] = &ellipt->Buffer[0];

	wgi_wldvdc(2, ellipt->Buffer, ellipt->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_closer_draw(data, closer, draw)
void *data;
wgiRubberCloser_t *closer;
long draw;
{
	if(closer->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		closer->ColorI, closer->PlaneI,
		closer->DrawOp, closer->Dashed);

	wgi_device_rubber_vector(closer->Mapped[0].x,
		closer->Mapped[0].y,
		closer->Mapped[1].x,
		closer->Mapped[1].y);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_closer_calc(data, closer, x1, y1, x2, y2)
void *data;
wgiRubberCloser_t *closer;
double x1, y1, x2, y2;
{
	long i;
	double dx;
	double dy;
	double dist;
	double less = MAXFLOAT;

	if(((closer->Closer = closer->Number) == 0) || (closer->Points == NULL))
	{
		closer->Output[0] = NULL;

		return (WGI_SUCCESS);
	}

	for(i = 0; i < closer->Number; i++)
	{
		dx = closer->Points[i].x - x2;
		dy = closer->Points[i].y - y2;

		if((dist = ((dx * dx) + (dy * dy))) < less)
		{
			closer->Closer = i; less = dist;
		}
	}

	if(closer->Closer == closer->Number)
	{
		closer->Output[0] = NULL;

		return (WGI_SUCCESS);
	}

	closer->Buffer[0].x = x2;
	closer->Buffer[0].y = y2;
	closer->Buffer[1].x = closer->Points[closer->Closer].x;
	closer->Buffer[1].y = closer->Points[closer->Closer].y;

	closer->Output[0] = &closer->Buffer[0];

	wgi_wldvdc(2, closer->Buffer, closer->Mapped);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_rubber_textex_draw(data, textex, draw)
void *data;
wgiRubberTextEx_t *textex;
long draw;
{
	wgiStroke_t *stroke;

	long x1 = textex->Mapped[0].x;
	long y1 = textex->Mapped[1].y;
	long dx = textex->Mapped[1].x - x1;
	long dy = textex->Mapped[0].y - y1;

	if(textex->Output[0] == NULL)
		return (WGI_SUCCESS);

	wgi_device_rubber_drawop(draw,
		textex->ColorI, textex->PlaneI,
		textex->DrawOp, textex->Dashed);

	for(stroke = textex->Stroke; stroke; stroke = stroke->Stroke)
	{
		wgi_device_rubber_nlines(textex->Scaled + stroke->Origin,
			stroke->Number);
	}

	if(textex->DoRect)
	{
		wgi_device_rubber_bounds(x1-1, y1-1, dx+2, dy+2);
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

static long iwgi_rubber_textex_calc(data, textex, x1, y1, x2, y2)
void *data;
wgiRubberTextEx_t *textex;
double x1, y1, x2, y2;
{
	long i;
	double hf;
	double wf;
	double xm;
	double ym;
	double nc;
	double dx = (x2-x1);
	double dy = (y2-y1);
	double ax = ABS(dx);
	double ay = ABS(dy);
	double lf = textex->StrLen;
	double cf = lf + ((lf - 1.0) * textex->Spaces);

	if(textex->Aspect > 0)
	{
		if((ax == 0) || ((ay/ax) > textex->Aspect))
		{
			ax = ay / textex->Aspect;
		}
		else
		{
			ay = ax * textex->Aspect;
		}
	}

	if((ax == 0) || (ay == 0))
	{
		textex->Output[0] = NULL;
		textex->Output[1] = NULL;

		return (WGI_SUCCESS);
	}

	if(dx > 0)
	{
		if(dy > 0)
		{
			xm = 1.0;
			ym = 1.0;
			textex->Mirror = wgiDeviceMirrorDoNot;
		}
		else
		{
			xm =  1.0;
			ym = -1.0;
			textex->Mirror = wgiDeviceMirrorYonly;
		}
	}
	else
	{
		if(dy > 0)
		{
			xm = -1.0;
			ym =  1.0;
			textex->Mirror = wgiDeviceMirrorXonly;
		}
		else
		{
			xm = -1.0;
			ym = -1.0;
			textex->Mirror = wgiDeviceMirrorXandY;
		}
	}

	textex->ChWide = ax / cf;
	textex->ChHigh = ay;

	textex->Buffer[0].x = ((dx > 0) ? (x1) : (x1 - ax));
	textex->Buffer[1].y = ((dy < 0) ? (y1) : (y1 + ay));
	textex->Buffer[1].x = textex->Buffer[0].x + ax;
	textex->Buffer[0].y = textex->Buffer[1].y - ay;
	textex->Buffer[2].x = x1;
	textex->Buffer[2].y = y1;

	textex->Output[0] = &textex->Buffer[0];
	textex->Output[1] = &textex->Buffer[2];

	wgi_wldvdc(3, textex->Buffer, textex->Mapped);

	ax = textex->Mapped[1].x - textex->Mapped[0].x;
	ay = textex->Mapped[0].y - textex->Mapped[1].y;

	wf = xm * FONT_WFAC(ax / cf);
	hf = ym * FONT_HFAC(ay);

	for(i = 0; i < textex->Number; i++)
	{
		textex->Scaled[i].x = textex->Mapped[2].x +
			(wf * textex->Native[i].x);

		textex->Scaled[i].y = textex->Mapped[2].y -
			(hf * textex->Native[i].y);
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

long wgi_rubber_init(manage)
wgiRubberManage_t *manage;
{
	long i;
	register wgiRubberStruct_t *rptr;
	register wgiRubberStruct_t *rend = manage->DoList + manage->Number;

	for(i = 0, rptr = manage->DoList; rptr < rend; i++, rptr++)
	{
		/* We have to set this flag here because we do not
		   want to have the users update their code for
		   wgiRubberTypeOfMyType cases (enno).
		*/
		rptr->Common.Changed = FALSE;

		switch(rptr->Common.TypeOf)
		{
		case wgiRubberTypeOfCommon:

			rptr->Common.InitFn = iwgi_rubber_common_init;
			rptr->Common.CalcFn = iwgi_rubber_common_stub;
			rptr->Common.DrawFn = iwgi_rubber_common_stub;
			rptr->Common.DoneFn = iwgi_rubber_common_stub;
		break;

		case wgiRubberTypeOfXhairs:

			rptr->Xhairs.InitFn = iwgi_rubber_xhairs_init;
			rptr->Xhairs.CalcFn = iwgi_rubber_xhairs_calc;
			rptr->Xhairs.DrawFn = iwgi_rubber_xhairs_draw;
			rptr->Xhairs.DoneFn = iwgi_rubber_common_stub;
		break;

		case wgiRubberTypeOfBounds:

			rptr->Bounds.InitFn = iwgi_rubber_bounds_init;
			rptr->Bounds.CalcFn = iwgi_rubber_bounds_calc;
			rptr->Bounds.DrawFn = iwgi_rubber_bounds_draw;
			rptr->Bounds.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfVector:

			rptr->Vector.InitFn = iwgi_rubber_vector_init;
			rptr->Vector.CalcFn = iwgi_rubber_vector_calc;
			rptr->Vector.DrawFn = iwgi_rubber_vector_draw;
			rptr->Vector.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfMitred:

			rptr->Mitred.InitFn = iwgi_rubber_mitred_init;
			rptr->Mitred.CalcFn = iwgi_rubber_mitred_calc;
			rptr->Mitred.DrawFn = iwgi_rubber_mitred_draw;
			rptr->Mitred.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfBend90:

			rptr->Bend90.InitFn = iwgi_rubber_bend90_init;
			rptr->Bend90.CalcFn = iwgi_rubber_bend90_calc;
			rptr->Bend90.DrawFn = iwgi_rubber_bend90_draw;
			rptr->Bend90.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfLarc90:

			rptr->Larc90.InitFn = iwgi_rubber_larc90_init;
			rptr->Larc90.CalcFn = iwgi_rubber_larc90_calc;
			rptr->Larc90.DrawFn = iwgi_rubber_larc90_draw;
			rptr->Larc90.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfLarc45:

			rptr->Larc45.InitFn = iwgi_rubber_larc45_init;
			rptr->Larc45.CalcFn = iwgi_rubber_larc45_calc;
			rptr->Larc45.DrawFn = iwgi_rubber_larc45_draw;
			rptr->Larc45.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfLarc45Rot:

/* ?? enno: should update this when it works ?? */

			rptr->Larc45.InitFn = iwgi_rubber_larc45_init;
			rptr->Larc45.CalcFn = iwgi_rubber_larc45rot_calc;
			rptr->Larc45.DrawFn = iwgi_rubber_larc45rot_draw;
			rptr->Larc45.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfArc3pt:

			rptr->Arc3pt.InitFn = iwgi_rubber_arc3pt_init;
			rptr->Arc3pt.CalcFn = iwgi_rubber_arc3pt_calc;
			rptr->Arc3pt.DrawFn = iwgi_rubber_arc3pt_draw;
			rptr->Arc3pt.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfCircle:

			rptr->Circle.InitFn = iwgi_rubber_circle_init;
			rptr->Circle.CalcFn = iwgi_rubber_circle_calc;
			rptr->Circle.DrawFn = iwgi_rubber_circle_draw;
			rptr->Circle.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfNshape:

			rptr->Nshape.InitFn = iwgi_rubber_nshape_init;
			rptr->Nshape.CalcFn = iwgi_rubber_nshape_calc;
			rptr->Nshape.DrawFn = iwgi_rubber_nshape_draw;
			rptr->Nshape.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfEllipt:

			rptr->Ellipt.InitFn = iwgi_rubber_ellipt_init;
			rptr->Ellipt.CalcFn = iwgi_rubber_ellipt_calc;
			rptr->Ellipt.DrawFn = iwgi_rubber_ellipt_draw;
			rptr->Ellipt.DoneFn = iwgi_rubber_common_stub;
		break;
		case wgiRubberTypeOfCloser:

			rptr->Closer.InitFn = iwgi_rubber_closer_init;
			rptr->Closer.CalcFn = iwgi_rubber_closer_calc;
			rptr->Closer.DrawFn = iwgi_rubber_closer_draw;
			rptr->Closer.DoneFn = iwgi_rubber_closer_done;
		break;
		case wgiRubberTypeOfTextEx:

			rptr->TextEx.InitFn = iwgi_rubber_textex_init;
			rptr->TextEx.CalcFn = iwgi_rubber_textex_calc;
			rptr->TextEx.DrawFn = iwgi_rubber_textex_draw;
			rptr->TextEx.DoneFn = iwgi_rubber_textex_done;
		break;

		case wgiRubberTypeOfMyType:

			if(rptr->MyType.InitFn == NULL)
				rptr->MyType.InitFn = iwgi_rubber_common_stub;
			if(rptr->MyType.CalcFn == NULL)
				rptr->MyType.CalcFn = iwgi_rubber_common_stub;
			if(rptr->MyType.DrawFn == NULL)
				rptr->MyType.DrawFn = iwgi_rubber_common_stub;
			if(rptr->MyType.DoneFn == NULL)
				rptr->MyType.DoneFn = iwgi_rubber_common_stub;
		break;

		default:

			SAYIT("Rubber[%d].TypeOf is invalid", i);

			rptr->Common.TypeOf = wgiRubberTypeOfCommon;
			rptr->Common.InitFn = iwgi_rubber_common_stub;
			rptr->Common.CalcFn = iwgi_rubber_common_stub;
			rptr->Common.DrawFn = iwgi_rubber_common_stub;
			rptr->Common.DoneFn = iwgi_rubber_common_stub;
		continue;
		}

		rptr->Common.InitFn(rptr->Common.FnData, rptr, i);
	}

	manage->UnDraw = FALSE;
	manage->MyView = WGI_VIEWPORT_MAX;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_rubber_move(manage, view, move)
wgiRubberManage_t *manage;
unsigned long view;
WLDcrd *move;
{
	long i, rc;
	register wgiRubberStruct_t *rptr;
	register wgiRubberStruct_t *rend = manage->DoList + manage->Number;

	manage->Changed = FALSE;
	if(manage->UnDraw)
	{
		if((rc = wgi_viewport(manage->MyView)) != WGI_SUCCESS)
			return (rc);

		for(rptr = manage->DoList; rptr < rend; rptr++)
		{
			rptr->Common.DrawFn(rptr->Common.FnData, rptr, FALSE);
		}

		manage->UnDraw = FALSE;
	}

	if((rc = wgi_viewport(manage->MyView = view)) != WGI_SUCCESS)
			return (rc);

	for(i = 0, rptr = manage->DoList; rptr < rend; i++, rptr++)
	{
		rptr->Common.CalcFn(rptr->Common.FnData, rptr,
			(double) rptr->Common.Anchor.x,
			(double) rptr->Common.Anchor.y,
			(double) rptr->Common.Offset.x + move->x,
			(double) rptr->Common.Offset.y + move->y);

		if(rptr->Common.UserFn)
			rptr->Common.UserFn(rptr->Common.FnData, rptr, i, move);

		rptr->Common.DrawFn(rptr->Common.FnData, rptr, TRUE);

		/* Accumulate the Changed flags from the 
		   wgiRubberStruct_t's.
		*/
		manage->Changed |= rptr->Common.Changed;
	}

	manage->UnDraw = TRUE;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_rubber_done(manage)
wgiRubberManage_t *manage;
{
	long rc, i;
	register wgiRubberStruct_t *rptr;
	register wgiRubberStruct_t *rend = manage->DoList + manage->Number;

	if(manage->UnDraw)
	{
		if((rc = wgi_viewport(manage->MyView)) != WGI_SUCCESS)
			return (rc);

		for(rptr = manage->DoList; rptr < rend; rptr++)
		{
			rptr->Common.DrawFn(rptr->Common.FnData, rptr, FALSE);
		}
	}

	for(i = 0, rptr = manage->DoList; rptr < rend; i++, rptr++)
	{
		rptr->Common.DoneFn(rptr->Common.FnData, rptr, i);
	}

	manage->UnDraw = FALSE;
	manage->MyView = WGI_VIEWPORT_MAX;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_rubber_zonk(manage, draw)
wgiRubberManage_t *manage;
long draw;
{
	long rc;
	register wgiRubberStruct_t *rptr;
	register wgiRubberStruct_t *rend = manage->DoList + manage->Number;

	if((draw) && (manage->UnDraw))
	{
		if((rc = wgi_viewport(manage->MyView)) != WGI_SUCCESS)
			return (rc);

		for(rptr = manage->DoList; rptr < rend; rptr++)
		{
			rptr->Common.DrawFn(rptr->Common.FnData, rptr, FALSE);
		}
	}

	manage->UnDraw = FALSE;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The End                                                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
