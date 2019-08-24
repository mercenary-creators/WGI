#include "wgiStruct.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_extent_fromwld(nelm, buff, xtnt, slop)
long nelm;
WLDcrd *buff;
Extent *xtnt;
Single  slop;
{
	long rc;
	WLDcrd more, less;

	/*
		Point At The End Of The List As A Terminator
	*/

	WLDcrd *eptr = buff + nelm;

	/*
		Set The Boundry Points To The First Point And Increment Past It
	*/

	more = less = *buff++;

	/*
		Loop Through The List Looking For Larger Or Smaller Values
	*/

	while(buff < eptr)
	{
		if(buff->x < less.x)
			less.x = buff->x;
		if(buff->x > more.x)
			more.x = buff->x;
		if(buff->y < less.y)
			less.y = buff->y;
		if(buff->y > more.y)
			more.y = buff->y;

		buff++;
	}

	/*
		Add A Specified Slop Factor To The Extents Of The List
	*/

	xtnt->xmin = less.x - slop;
	xtnt->xmax = more.x + slop;
	xtnt->ymin = less.y - slop;
	xtnt->ymax = more.y + slop;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_wide_save(wide, save)
long wide;
long *save;
{
	long i, rc;

	for(i = 0; i < wld_ltabl.used; i++)
	{
		/*
			Store Width In X Value To Scale Line Widths
		*/

		if(((WLDcrd *)wld_ltabl.data + i)->x == (Single)wide)
		{
			*save = i;
			return (WGI_SUCCESS);
		}
	}

	/*
		Allocate A New Entry In The Table
	*/

	if((rc = wgi_table_aloc(&wld_ltabl, &i, 1)) != WGI_SUCCESS)
		return (rc);

	/*
		Table May Have Been Re-Allocated So We Point At Data Again
	*/

	*save = i;
	((WLDcrd *)wld_ltabl.data + i)->x = (Single)wide;

	wld_ltabl.sync++;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_create_prim()
{
	if (isegprimp = WGI_MALLOC(1, Segprim))
	{
		return (WGI_SUCCESS);
	}

	WOOPS("Not enough memory to allocate primitive");
	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_aloc_prim_elements(indx, nelm)
long *indx;
long  nelm;
{
	long size, rc;

	if((rc = wgi_table_aloc(wld_table, indx, nelm)) != WGI_SUCCESS)
		return (rc);

	size = wld_table->nelm * wld_table->elsz;

	if((rc = wgi_tbuff_match(vdc_tbuff, size)) != WGI_SUCCESS)
		return (rc);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_free_elements(indx, nelm)
long indx;
long nelm;
{
	long rc;

	if((rc = wgi_table_free(wld_table, indx, nelm)) != WGI_SUCCESS)
		return (rc);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_link_prim(nelm, buff)
long nelm;
WLDcrd *buff;
{
	long rc;
	long indx;
	WLDcrd *wldp;

	/*
		Make Sure There Was A Segment Created For This Primitive
	*/

	if(!isegmentp)
	{
		WOOPS("There is no open segment");
		WGI_FREE(isegprimp);

		return (WGI_PROBLEM);
	}

	/*
		Allocate Primitive Elements In WLD And VDC Free Tables
	*/

	if((rc = iwgi_aloc_prim_elements(&indx, nelm)) != WGI_SUCCESS)
		return (rc);

	/*
		Tell The Primitive Where It Belongs In The Table
	*/

	isegprimp->nelm = nelm;
	isegprimp->indx = indx;

	/*
		Link The Primitive Into The Current Segment
	*/

	isegprimp->next = isegmentp->prim;
	isegmentp->prim = isegprimp;

	/*
		Point at the WLD table after allocating primitive elements
		because the memory may have been moved if it was extended.
	*/

	wldp = (WLDcrd *)wld_table->data + indx;

	/*
		Copy The WLD Coordinates To The WLD Coordinate Table
	*/

	while(nelm--)
		*wldp++ = *buff++;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_draw_prim(prim, this)
Segprim *prim;
Segment *this;
{
	long rc;
	long view = 1;
	long mask = this->mask >> 1;
	VDCcrd *vdcp = (VDCcrd *)vdc_tbuff->data;
	WLDcrd *wldp = (WLDcrd *)wld_table->data;

	if(this->visi)
	{
		while(mask)
		{
			while(!(mask & 0x1))
			{
				view++; mask >>= 1;
			}

			wgi_viewport(view);

			if(iwgi_extent_overlap(this->xtnt))
			{
				if(iwgi_extent_outside(this->xtnt))
				{
				    if(iwgi_extent_overlap(prim->xtnt))
				    {
					wgi_wldvdc(prim->nelm, wldp+prim->indx, vdcp+prim->indx);

					dispatch_tabl[iwgi_extent_outside(prim->xtnt)][prim->type]
						(vdcp + prim->indx, prim);
				    }
				}
				else
				{
				    wgi_wldvdc(prim->nelm, wldp+prim->indx, vdcp+prim->indx);

				    dispatch_draw[prim->type](vdcp + prim->indx, prim);
				}
			}

			view++; mask >>= 1;
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

long wgi_points(numx, numy, buff, able, pick, draw)
long numx;
long numy;
WLDcrd *buff;
long able;
Pickdef pick;
long draw;
{
	long rc;
	long nelm = numx + numy;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = numx;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_POINTS;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(nelm, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Points Primitive
	*/

	if((rc = iwgi_extent_fromwld(nelm, buff, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_polyln(nelm, buff, wide, term, able, pick, draw)
long nelm;
WLDcrd *buff;
long wide;
long term;
long able;
Pickdef pick;
long draw;
{
	long rc;
	long save;
	Single slop;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	/*
		Reserve A Space In The Line Width Table
	*/

	if((rc = iwgi_wide_save(wide, &save)) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = term;
	isegprimp->wide = save;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_POLYLINE;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(nelm, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		A Slop Factor Of Half The Line Width Is Added To The Extents
		Of The Polyline
	*/

	slop = (Single)wide / 2;

	/*
		Calculate The Extents Of The Polyline Primitive
	*/

	if((rc = iwgi_extent_fromwld(nelm, buff, &isegprimp->xtnt, slop)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_zebra(nelm, buff, wide, term, dash, able, pick, draw)
long nelm;
WLDcrd *buff;
long wide;
long term;
long dash;	/* dash style */
long able;
Pickdef pick;
long draw;
{
	long rc;
	long save;
	Single slop;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	/*
		Reserve A Space In The Line Width Table
	*/

	if((rc = iwgi_wide_save(wide, &save)) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = (dash << 1) + term;
	isegprimp->wide = save;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_ZEBRA;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(nelm, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		A Slop Factor Of Half The Line Width Is Added To The Extents
		Of The Polyline
	*/

	slop = (Single)wide / 2;

	/*
		Calculate The Extents Of The Polyline Primitive
	*/

	if((rc = iwgi_extent_fromwld(nelm, buff, &isegprimp->xtnt, slop)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_polygon(nelm, buff, fill, able, pick, draw)
long nelm;
WLDcrd *buff;
long fill;
long able;
Pickdef pick;
long draw;
{
	long rc;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = fill;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_POLYGON;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(nelm, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Polyline Primitive
	*/

	if((rc = iwgi_extent_fromwld(nelm, buff, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_rectangle(buff, fill, able, pick, draw)
WLDcrd *buff;
long fill;
long able;
Pickdef pick;
long draw;
{
	long rc;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = fill;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_RECTANGLE;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(2, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Polyline Primitive
	*/

	if((rc = iwgi_extent_fromwld(2, buff, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_marker(here, type, able, pick, draw)
WLDcrd *here;
long type;
long able;
Pickdef pick;
long draw;
{
	long rc;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = type;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_MARKER;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(1, here)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Marker
	*/

	if((rc = iwgi_extent_fromwld(1, here, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_circle(cent, radi, fill, able, pick, draw)
WLDcrd *cent;
long radi;
long fill;
long able;
Pickdef pick;
long draw;
{
	long rc;
	long save;
	Single slop;
	WLDcrd buff[2];

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	/*
		Reserve A Space In The Line Width Table For Radius
	*/

	if((rc = iwgi_wide_save(radi, &save)) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = fill;
	isegprimp->wide = save;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_CIRCLE;

	/*
		Insert Primitive Into Segment List
	*/

	buff[0].x = cent->x;
	buff[0].y = cent->y;

	if((rc = iwgi_link_prim(1, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Circle
	*/

	slop = (Single)radi;

	if((rc = iwgi_extent_fromwld(1, cent, &isegprimp->xtnt, slop)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_text_xtnt(word, cent, orig, dhig, dwid, dspc, rotn, refl)
char *word;
WLDcrd *cent;
WLDcrd *orig;
Single dhig;
Single dwid;
Single dspc;
long rotn;
long refl;
{
	long leng, rc;
	Single dx, dy;
	WLDcrd ebox[4];

	leng = strlen(word);

	/*
		Calculate Length Of Word With Spacing Factors
	*/

	dx = (dwid * leng) + ((dspc - dwid) * (leng - 1));

	/*
		Calculate Height Of Word Accounting For Tails
	*/

	orig[0].y = (orig[0].y - (dy = (dhig * 0.375))); dy += dhig;

	/*
		Construct The Extent Box For The Word
	*/

	ebox[0].x = orig[0].x;
	ebox[0].y = orig[0].y;
	ebox[1].x = orig[0].x + dx;
	ebox[1].y = orig[0].y;
	ebox[2].x = orig[0].x;
	ebox[2].y = orig[0].y + dy;
	ebox[3].x = orig[0].x + dx;
	ebox[3].y = orig[0].y + dy;

	/*
		Transform The Extent Box With The Rotations And Reflection
	*/

	if((rc = iwgi_xfrm(4, ebox, cent, rotn, refl)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Text Primitive After Transformation
	*/

	if((rc = iwgi_extent_fromwld(4, ebox, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_text(cent, word, high, wide, spac, rotn, refl, able, pick, draw)
WLDcrd *cent;
char *word;
long high;
long wide;
long spac;
long rotn;
long refl;
long able;
Pickdef pick;
long draw;
{
	char *temp;
	long rc, indx, leng;
	Single dwid, dhig, dspc;
	WLDcrd base[2], orig[1];

	/*
		Calculate The Height And Width And Spacing Factor
	*/

	dhig = high;
	dwid = wide;
	dspc = dwid + ((dwid * spac)/100.0);

	/*
		Save The Character Base And Distance To The Next Character
	*/

	leng = strlen(word);

	base[0].x = cent[0].x - (dwid / 2.0);
	base[0].y = cent[0].y - (dhig / 2.0);

	base[1].x = base[0].x + (dspc * leng);
	base[1].y = base[0].y;

	orig[0].x = base[0].x;
	orig[0].y = base[0].y;

	/*
		Normalize The Rotation Description
	*/

	while(rotn <    0) rotn = rotn + 360;
	while(rotn >= 360) rotn = rotn - 360;

	/*
		Transform The Character Base Description
	*/

	if((rc = iwgi_xfrm(2, base, cent, rotn, refl)) != WGI_SUCCESS)
		return (rc);

	/*
		Reserve A Space In The Text Table For This Text Type
	*/

	if((indx = iwgi_text_dscr(high, wide, spac, rotn, refl)) < 0)
		return (WGI_PROBLEM);

	if((rc = iwgi_text_save(indx, word)) != WGI_SUCCESS)
		return (rc);

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_TEXT;

	if(!(temp = WGI_MALLOC(leng+1, char)))
		return (WGI_PROBLEM);

	strcpy(temp, word);

	isegprimp->wide = indx;
	isegprimp->user = (long)temp;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(2, base)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_text_xtnt(word, cent, orig, dhig, dwid, dspc, rotn, refl)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_stencil(nelm, buff, fill, able, pick, draw)
long nelm;
WLDcrd *buff;
long fill;
long able;
Pickdef pick;
long draw;
{
	long rc;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = fill;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_STENCIL;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(nelm, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Polyline Primitive
	*/

	if((rc = iwgi_extent_fromwld(nelm, buff, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_hole(nelm, buff, fill, able, pick, draw)
long nelm;
WLDcrd *buff;
long fill;
long able;
Pickdef pick;
long draw;
{
	long rc;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->user = fill;
	isegprimp->pick = able;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_HOLE;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(nelm, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Polyline Primitive
	*/

	if((rc = iwgi_extent_fromwld(nelm, buff, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_flush(nelm, buff, pick, draw)
long nelm;
WLDcrd *buff;
Pickdef pick;
long draw;
{
	long rc;

	/*
		Create The Primitive And Set Prim Type
	*/

	if((rc = iwgi_create_prim()) != WGI_SUCCESS)
		return (rc);

	isegprimp->pick = 0;
	isegprimp->piid = pick.iden;
	isegprimp->ptyp = pick.type;
	isegprimp->type = WGI_PRIM_FLUSH;

	/*
		Insert Primitive Into Segment List
	*/

	if((rc = iwgi_link_prim(nelm, buff)) != WGI_SUCCESS)
		return (rc);

	/*
		Calculate The Extents Of The Polyline Primitive
	*/

	if((rc = iwgi_extent_fromwld(nelm, buff, &isegprimp->xtnt, (Single)0)) != WGI_SUCCESS)
		return (rc);

	if((rc = iwgi_extent_combine(isegprimp->xtnt, &isegmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((draw) && (iwgi_draw_flag))
	{
		if((rc = iwgi_draw_prim(isegprimp, isegmentp)) != WGI_SUCCESS)
			return (rc);
	}

	isegprimp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--        by: enno                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static unsigned char zebra_dashes [WGI_ZEBRA_MAX_DASHES];
static long dash_count [WGI_MAX_DASH_STYLES];
static Single dash_length [WGI_MAX_DASH_STYLES][WGI_ZEBRA_MAX_DASHES];

static long last_style = -1;
static long last_width = -1;

long zebra_calc_dashes (dash_style, width, dashes, count)
long dash_style;
long width;
unsigned char** dashes;
int* count;
{
    int i, l;

    *count = dash_count [dash_style];
    *dashes = zebra_dashes;

    if (last_style == dash_style && last_width == width)
	return 0;

    last_style = dash_style;
    last_width = width;

    for (i = 0; i < dash_count[dash_style]; i++)
    {
	l = dash_length[dash_style][i] * width + 0.5;

	if (l == 0) l = 1;
	else if (l > 255) l = 255;	/* must fit in unsigned char! */

	zebra_dashes[i] = l;
    }
    return 1;
}

long wgi_set_dash_style (style, count, array)
long style;
long count;
Single* array;
{
	int i;

	if (style < 0 || style >= WGI_MAX_DASH_STYLES)
	{
	    WOOPS ("invalid dash style in wgi_set_dash_style");
	    return (WGI_PROBLEM);
	}

	if (count < 2 || count >= WGI_ZEBRA_MAX_DASHES)
	{
	    WOOPS ("invalid dash count in wgi_set_dash_style");
	    return (WGI_PROBLEM);
	}

	dash_count[style] = count;
	for (i = 0; i < count; i++)
	{
	    dash_length[style][i] = array[i];
	}

	/* Set last_style to -1, so that zebra_calc_dashes will not
	 *  skip the (re)calculation.
	*/
	if (style == last_style)
	    last_style = -1;

	return (WGI_SUCCESS);    
}

long iwgi_init_dash_styles ()
{
    int i, j;
    for (i = 0; i < WGI_MAX_DASH_STYLES; i++)
    {
	dash_count[i] = 2;
	for (j = 0; j < WGI_ZEBRA_MAX_DASHES; j++)
	{
	    dash_length[i][j] = 1.0;
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
