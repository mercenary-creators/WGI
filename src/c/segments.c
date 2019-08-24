#include <math.h>
#include <values.h>

#include "wgiStruct.h"
#include "wgiSymbol.h"

static Extent wind;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                   Allocate A Segment Structure                        --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_creseg(iden, levl, mask, pick, edge, fill, text, visi, draw)
long iden;
long levl;
long mask;
long pick;
long edge;
long fill;
long text;
long visi;
long draw;
{
	long rc;

	/*
		Check that the color indexes are within range
	*/

	if((fill < 0) || (fill > 255))		/* fill index */
	{
		WOOPS("The segment fill color %d is out of range", fill);
		return (WGI_PROBLEM);
	}

	if((edge < 0) || (edge > 255))		/* edge index */
	{
		WOOPS("The segment edge color %d is out of range", edge);
		return (WGI_PROBLEM);
	}

	if((text < 0) || (text > 255))		/* text index */
	{
		WOOPS("The segment text color %d is out of range", edge);
		return (WGI_PROBLEM);
	}

	/*
		Allocate segment and set up values
	*/

	if (isegmentp = (Segment *)ewgi_malloc(sizeof(Segment)))
	{
		isegmentp->iden = iden;
		isegmentp->levl = levl;
		isegmentp->mask = mask;
		isegmentp->pick = pick;
		isegmentp->fill = fill;
		isegmentp->edge = edge;
		isegmentp->text = text;
		isegmentp->visi = visi;
		isegmentp->prim = NULL;
		isegmentp->next = NULL;
		isegmentp->draw = draw;

		if((rc = iwgi_extent_inverse(&isegmentp->xtnt)) != WGI_SUCCESS)
		{
			ewgi_free((char *)isegmentp);
			return (rc);
		}

		if((rc = wgi_flag(WGI_DRAW, draw)) != WGI_SUCCESS)
			return (rc);

		if(draw)
		{
			sun_color_set(isegmentp->edge, isegmentp->fill, isegmentp->text);
		}

		return (WGI_SUCCESS);
	}

	WOOPS("Not enough memory to allocate segment");
	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_get_current_segment(csegmentp)
Segment **csegmentp;
{
	*csegmentp = isegmentp;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_segment_current(csegmentp)
Segment *csegmentp;
{
	long rc;

	if(csegmentp == NULL)
	{
		WOOPS("The current segment pointer is null");
		return (WGI_PROBLEM);
	}

	isegmentp = csegmentp;

	if((rc = wgi_flag(WGI_DRAW, isegmentp->draw)) != WGI_SUCCESS)
		return (rc);

	if(isegmentp->draw)
	{
		sun_color_set(isegmentp->edge, isegmentp->fill, isegmentp->text);
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

Segment* wgi_get_segment_from_iden(iden, levl)
long iden;
long levl;
{
	Segment *this;

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return 0;
	}

	this = iseglistp->segs;
	while(this)
	{
		if((this->iden == iden) && LOOK_LEVL(this->levl, levl))
			return (this);
		this = this->next;
	}

	return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--           Returns true if Segment List has no Segments                --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_is_seglist_empty(list)
Seglist *list;
{
	return (list->segs == NULL);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--           Insert Segment Into Display Level Segment List              --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_endseg()
{
	long rc;

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	if(!isegmentp)
	{
		WOOPS("The current segment pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Insert The Segment Into The Current Segment List
	*/

	if((rc = iwgi_ln_segment(iseglistp, isegmentp)) != WGI_SUCCESS)
		return (rc);

	iseglistp->smap.sync = TRUE;

	if((rc = iwgi_extent_combine(isegmentp->xtnt, &iseglistp->xtnt)) != WGI_SUCCESS)
		return (rc);

	if((rc = wgi_flag(WGI_DRAW, FALSE)) != WGI_SUCCESS)
		return (rc);

	isegprimp = NULL;
	isegmentp = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_viewport(iden)
long iden;
{
	iwgi_viewport_select(iden, TRUE, &vmap);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_clip_view(view)
Extent view;
{
	wind = view;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_initlz_mapping(pmap)
Mapping *pmap;
{
	long rc;

	pmap->view = 0;
	pmap->wide = 0;
	pmap->high = 0;
	pmap->sync = 0;

	if((rc = iwgi_extent_inverse(&pmap->xtnt)) != WGI_SUCCESS)
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

long iwgi_checks_mapping(pmap)
Mapping pmap;
{
	if(pmap.sync)
		return (TRUE);

	if(pmap.view != vmap.view)
		return (TRUE);

	if(pmap.high != vmap.high)
		return (TRUE);

	if(pmap.wide != vmap.wide)
		return (TRUE);

	if(pmap.xtnt.xmin != vmap.xtnt.xmin)
		return (TRUE);

	if(pmap.xtnt.xmax != vmap.xtnt.xmax)
		return (TRUE);

	if(pmap.xtnt.ymin != vmap.xtnt.ymin)
		return (TRUE);

	if(pmap.xtnt.ymax != vmap.xtnt.ymax)
		return (TRUE);

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_seglist_create(lptr, mask)
Seglist **lptr;
unsigned long mask;
{
	long rc;
	Seglist *list;

	if(list = (Seglist *)ewgi_malloc(sizeof(Seglist)))
	{
		list->mask = mask;
		list->segs = NULL;
		list->last = NULL;
		list->hold = NULL;
		list->back = NULL;

		if((rc = iwgi_extent_inverse(&list->xtnt)) != WGI_SUCCESS)
			return (rc);

		if((rc = iwgi_initlz_mapping(&list->smap)) != WGI_SUCCESS)
			return (rc);

		if((rc = wgi_table_init(&list->wldt, sizeof(WLDcrd), 3000, 33)) != WGI_SUCCESS)
			return (rc);

		if((rc = wgi_tbuff_init(&list->vdcb)) != WGI_SUCCESS)
			return (rc);

		*lptr = list;

		return (WGI_SUCCESS);
	}

	WOOPS("The seglist create failed");
	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_seglist_current(list)
Seglist *list;
{
	if(!list)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	iseglistp = list;
	wld_table = &list->wldt;
	vdc_tbuff = &list->vdcb;

	iwgi_writemask(list->mask);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_get_current_seglist(cseglistp)
Seglist **cseglistp;
{
	*cseglistp = iseglistp;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

unsigned long mask = 0;	/* enno: added unsigned */

static long iwgi_seglist_parse_look(list)
Seglist *list;
{
    register Segment *this;
    register Segprim *prim;
    VDCcrd *vdcp = (VDCcrd *)vdc_tbuff->data;

    for (this = list->segs; this; this = this->next)
    {
	if (this->visi && (this->mask & mask) && iwgi_extent_overlap(this->xtnt))
	{
	    sun_color_set(this->edge, this->fill, this->text);

	    if (iwgi_extent_outside(this->xtnt))
	    {
		for (prim = this->prim; prim; prim = prim->next)
		{
		    if (iwgi_extent_overlap(prim->xtnt))
		    {
			dispatch_tabl[iwgi_extent_outside(prim->xtnt)][prim->type]
				(vdcp + prim->indx, prim);
		    }
		}
	    }
	    else
	    {
		for (prim = this->prim; prim; prim = prim->next)
		{
		    dispatch_draw[prim->type](vdcp + prim->indx, prim);
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

static long iwgi_seglist_parse_draw(list)
Seglist *list;
{
    register Segment *this;
    register Segprim *prim;
    VDCcrd *vdcp = (VDCcrd *)vdc_tbuff->data;

    for(this = list->segs; this; this = this->next)
    {
	if((this->visi) && (this->mask & mask))
	{
	    sun_color_set(this->edge, this->fill, this->text);

	    for(prim = this->prim; prim; prim = prim->next)
	    {
		dispatch_draw[prim->type](vdcp+prim->indx, prim);
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

long wgi_seglist_mapping(list)
Seglist *list;
{
	long rc;

	if(iwgi_checks_mapping(list->smap))
	{
		list->smap = vmap;

		rc = wgi_wldvdc(list->wldt.used, list->wldt.data, list->vdcb.data);
		return (rc);
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

long wgi_seglist_parseit(list)
Seglist *list;
{
	long rc = WGI_SUCCESS;

	if(iwgi_extent_overlap(list->xtnt))
	{
		if((rc = wgi_seglist_current(list)) != WGI_SUCCESS)
			return (rc);

		if((rc = wgi_seglist_mapping(list)) != WGI_SUCCESS)
			return (rc);

		if(iwgi_extent_outside(list->xtnt))
		{
			rc = iwgi_seglist_parse_look(list);
		}
		else
		{
			rc = iwgi_seglist_parse_draw(list);
		}
	}

	return (rc);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Return TRUE if the extent is not totally inside the viewport window  --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_extent_outside(xtnt)
Extent xtnt;
{
	if(xtnt.xmin < wind.xmin)
		return (TRUE);
	if(xtnt.xmax > wind.xmax)
		return (TRUE);
	if(xtnt.ymin < wind.ymin)
		return (TRUE);
	if(xtnt.ymax > wind.ymax)
		return (TRUE);

	return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--         Return TRUE if extent touches the viewport window             --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_extent_overlap(xtnt)
Extent xtnt;
{
	if(xtnt.xmin > wind.xmax)
		return (FALSE);
	if(xtnt.xmax < wind.xmin)
		return (FALSE);
	if(xtnt.ymin > wind.ymax)
		return (FALSE);
	if(xtnt.ymax < wind.ymin)
		return (FALSE);

	return (TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--   Set extent values to inverse limits for extent limit calculations   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_extent_inverse(xtnt)
Extent *xtnt;
{
	if(xtnt)
	{
		xtnt->xmin = MAXFLOAT;
		xtnt->xmax = -MAXFLOAT;
		xtnt->ymin = MAXFLOAT;
		xtnt->ymax = -MAXFLOAT;

		return (WGI_SUCCESS);
	}

	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--       Combine two extents into one extent that contains both          --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_extent_combine(xtin, xtio)
Extent xtin, *xtio;
{
	if(!xtio)
	{
		WOOPS("The extent pointer is null");
		return (WGI_PROBLEM);
	}

	if(xtin.xmin < xtio->xmin)
		xtio->xmin = xtin.xmin;
	if(xtin.xmax > xtio->xmax)
		xtio->xmax = xtin.xmax;
	if(xtin.ymin < xtio->ymin)
		xtio->ymin = xtin.ymin;
	if(xtin.ymax > xtio->ymax)
		xtio->ymax = xtin.ymax;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_seglist_purgeit(list, draw)
Seglist *list;
long draw;
{
	long rc;
	Segment *this, *save;

	if(!list)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	if((rc = wgi_seglist_current(list)) != WGI_SUCCESS)
		return (rc);

	this = list->segs;
	while(this)
	{
		save = this->next;
		if((rc = iwgi_rm_segment(list, this, 0, 0, 0, draw)) != WGI_SUCCESS)
			return (rc);
		this = save;
	}

	list->segs = NULL;
	list->last = NULL;
	list->hold = NULL;
	list->back = NULL;

	if((rc = iwgi_extent_inverse(&list->xtnt)) != WGI_SUCCESS)
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

long wgi_delseg(iden, levl, hold, draw, done)
long iden;
long levl;
long hold;
long draw;
long *done;
{
	/*
		Does This Need To Know Display Level
	*/

	long rc;
	Segment *this, *back, *save;

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	*done = 0;

	back = 0;
	this = iseglistp->segs;
	while(this)
	{
		if((this->iden == iden) && LOOK_LEVL(this->levl, levl))
		{
			*done = 1;

			save = this->next;
			if((rc = iwgi_rm_segment(iseglistp, this, back, 1, hold, draw)) != WGI_SUCCESS)
				return (rc);
			this = save;

			if(iwgi_cont_flag == FALSE)
				return (WGI_SUCCESS);
		}
		else
		{
			back = this;
			this = this->next;
		}
	}

	return (WGI_SUCCESS);
}

long wgi_draw_segment (seg)
Segment* seg;
{
	long rc;
	sun_color_set(seg->edge, seg->fill, seg->text);
	return iwgi_draw_segm(seg);
}

long wgi_undraw_segment (seg)
Segment* seg;
{
	long rc;
	sun_color_set(0, 0, 0);
	sun_undrw_set(seg->edge, seg->fill, seg->text);
	return iwgi_draw_segm(seg);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_seg_update(iden, levl, visi, pick)
long iden;
long levl;
long visi;
long pick;
{
	long rc;
	Segment *this;
	Segprim *prim;

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	for(this = iseglistp->segs; this; this = this->next)
	{
		if((this->iden == iden) && LOOK_LEVL(this->levl, levl))
		{
			this->pick = pick;
			if(this->visi != visi)
			{
				if(visi)
				{
					/*
						Visibility Must Be Set To True Before
						The Primitive Is Drawn
					*/

					this->visi = visi;

					sun_color_set(this->edge, this->fill, this->text);

					if((rc = iwgi_draw_segm(this)) != WGI_SUCCESS)
						return (rc);
				}
				else
				{
					sun_color_set(0, 0, 0);
					sun_undrw_set(this->edge, this->fill, this->text);

					if((rc = iwgi_draw_segm(this)) != WGI_SUCCESS)
						return (rc);

					/*
						Visibility Must Be Set To False After
						Primitive Is Undrawn
					*/

					this->visi = visi;
				}
			}

			if(iwgi_cont_flag == FALSE)
				return (WGI_SUCCESS);
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

long wgi_append_seglist(old_seglist, new_seglist, unique, next_id)
Seglist *old_seglist;
Seglist *new_seglist;
long unique;
long next_id;
{
	long rc;
	Segment *new_this;
	Segment *old_this;
	WLDcrd* wldp;
	Segment* wgi_copy_seg();

	/* set wldp for wgi_copy_seg to use the data for the new segment list
	   (the one being appended) */
	if((rc = wgi_seglist_current(new_seglist)) != WGI_SUCCESS)
		return (rc);

	wldp = (WLDcrd *)wld_table->data;

	for(new_this = new_seglist->segs; new_this; new_this = new_this->next)
	{
		if (unique)
		{
			for(old_this = old_seglist->segs; old_this; old_this = old_this->next)
			{
				if (old_this->iden == new_this->iden)
				{
					new_this->iden = next_id++;
					break;
				}
			}
		}

		if((rc = wgi_seglist_current(old_seglist)) != WGI_SUCCESS)
			return (rc);

		wgi_copy_seg (new_this, new_this->iden, wldp);
	}

	old_seglist->smap.sync = TRUE;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_seg_transform(segmentp, x_offset, y_offset)
Segment *segmentp;
long x_offset;
long y_offset;
{
	long rc;
	WLDcrd *wldp;
	Segprim *prim;

	if(segmentp == NULL)
	{
		WOOPS("The current segment pointer is null");
		return (WGI_PROBLEM);
	}

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	wldp = (WLDcrd *)wld_table->data;

	for(prim = segmentp->prim; prim; prim = prim->next)
	{
		WLDcrd *wld_listp = wldp+prim->indx;
		WLDcrd *wld_liste = wld_listp + prim->nelm;
		while (wld_listp < wld_liste)
		{
			wld_listp->x += x_offset;
			wld_listp->y += y_offset;
			wld_listp++;
		}
		/* update extent of this primitive */
		prim->xtnt.xmin += x_offset;
		prim->xtnt.ymin += y_offset;
		prim->xtnt.xmax += x_offset;
		prim->xtnt.ymax += y_offset;
	}

	/* update extent of this segment */
	segmentp->xtnt.xmin += x_offset;
	segmentp->xtnt.ymin += y_offset;
	segmentp->xtnt.xmax += x_offset;
	segmentp->xtnt.ymax += y_offset;

	/* update segment list extent of all segments */
	if((rc = iwgi_extent_combine(segmentp->xtnt, &iseglistp->xtnt)) != WGI_SUCCESS)
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

long wgi_seg_rotate(segmentp, rotation, pivot_x, pivot_y)
Segment *segmentp;
long rotation;
long pivot_x;
long pivot_y;
{
	long rc;
	WLDcrd *wldp;
	Segprim *prim;
	WLDcrd buff;
	WLDcrd center;
	center.x = pivot_x;
	center.y = pivot_y;

	if(segmentp == NULL)
	{
		WOOPS("The current segment pointer is null");
		return (WGI_PROBLEM);
	}

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	wldp = (WLDcrd *)wld_table->data;

	if((rc = iwgi_extent_inverse(&segmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	for(prim = segmentp->prim; prim; prim = prim->next)
	{
		WLDcrd *wld_listp = wldp+prim->indx;
		WLDcrd *wld_liste = wld_listp + prim->nelm;

		if((rc = iwgi_extent_inverse(&prim->xtnt)) != WGI_SUCCESS)
			return (rc);

		while (wld_listp < wld_liste)
		{
			buff.x = wld_listp->x;
			buff.y = wld_listp->y;
			iwgi_xfrm (1, &buff, &center, rotation, WGI_NREFL);
			wld_listp->x = buff.x;
			wld_listp->y = buff.y;
			wld_listp++;

			/* update extent of this primitive */
			if (buff.x < prim->xtnt.xmin)
				prim->xtnt.xmin = buff.x;
			if (buff.y < prim->xtnt.ymin)
				prim->xtnt.ymin = buff.y;
			if (buff.x > prim->xtnt.xmax)
				prim->xtnt.xmax = buff.x;
			if (buff.y > prim->xtnt.ymax)
				prim->xtnt.ymax = buff.y;
		}

		/* update extent of this segment */
		if((rc = iwgi_extent_combine(prim->xtnt, &segmentp->xtnt)) != WGI_SUCCESS)
			return (rc);
	}

	/* update segment list extent of all segments */
	if((rc = iwgi_extent_combine(segmentp->xtnt, &iseglistp->xtnt)) != WGI_SUCCESS)
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

long wgi_seg_mirror(segmentp, refl, pivot_x, pivot_y)
Segment *segmentp;
long refl;
long pivot_x;
long pivot_y;
{
	long rc;
	WLDcrd *wldp;
	Segprim *prim;
	WLDcrd buff;
	WLDcrd center;
	center.x = pivot_x;
	center.y = pivot_y;

	if(segmentp == NULL)
	{
		WOOPS("The current segment pointer is null");
		return (WGI_PROBLEM);
	}

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	wldp = (WLDcrd *)wld_table->data;

	if((rc = iwgi_extent_inverse(&segmentp->xtnt)) != WGI_SUCCESS)
		return (rc);

	for(prim = segmentp->prim; prim; prim = prim->next)
	{
		WLDcrd *wld_listp = wldp+prim->indx;
		WLDcrd *wld_liste = wld_listp + prim->nelm;

		if((rc = iwgi_extent_inverse(&prim->xtnt)) != WGI_SUCCESS)
			return (rc);

		while (wld_listp < wld_liste)
		{
			buff.x = wld_listp->x;
			buff.y = wld_listp->y;
			iwgi_xfrm (1, &buff, &center, 0, refl);
			wld_listp->x = buff.x;
			wld_listp->y = buff.y;
			wld_listp++;

			/* update extent of this primitive */
			if (buff.x < prim->xtnt.xmin)
				prim->xtnt.xmin = buff.x;
			if (buff.y < prim->xtnt.ymin)
				prim->xtnt.ymin = buff.y;
			if (buff.x > prim->xtnt.xmax)
				prim->xtnt.xmax = buff.x;
			if (buff.y > prim->xtnt.ymax)
				prim->xtnt.ymax = buff.y;
		}

		/* update extent of this segment */
		if((rc = iwgi_extent_combine(prim->xtnt, &segmentp->xtnt)) != WGI_SUCCESS)
			return (rc);
	}

	/* update segment list extent of all segments */
	if((rc = iwgi_extent_combine(segmentp->xtnt, &iseglistp->xtnt)) != WGI_SUCCESS)
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

long
wgi_calc_text_center_from_base (wide, high, rotn, refl, base0, cen)
long wide;
long high;
long rotn;
long refl;
WLDcrd* base0;
WLDcrd* cen;
{
	/*
	//	Imagine that the characters in the foll. pictures are square.
	//	This is what the extent of a character would look like:
	//
	//	+----+	^
	//	|    |	|
	//	|    |	|
	//	|    |	| height
	//	|    |	|
	//	|    |	|
	//	|    |	|
	//	+----+	v
	//
	//	<---->
	//	 width
	//
	//	First we calculate 'half', which is the vector
	//	(from the origin) to the center of the character
	//	extent if the origin is (0,0) and the rotation and
	//	reflection are 0.
	//
	//	Then we rotate and reflect 'half' using the text
	//	rotation and reflection.
	//
	//	      /
	//	     /\
	//	    /  \
	//	   /    \
	//	  /      \
	//	  \   c   \
	//	   \  /\   \/
	//	    \/	\  /
	//	     \	 \/
	//	      \  /
	//	       \/
	//	      base0
	//		
	//	Then we add (the rotated and reflected) 'half' to
	//	base0, where base0 is the origin (lowerleft) of the
	//	first character's extent box (not taking the "tail"
	//	into account.
	//
	*/

    static WLDcrd orig = { 0.0, 0.0 };
    WLDcrd half;

    half.x = wide / 2.0;
    half.y = high / 2.0;

    iwgi_xfrm (1, &half, &orig, rotn, refl);
    cen->x = base0->x + half.x;
    cen->y = base0->y + half.y;

    return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

Segment* wgi_copy_seg(segmentp, iden, wldp)
Segment *segmentp;
long iden;
WLDcrd *wldp;
{
	Segprim *prim;
	Pickdef pickdef;
	long wide;
	Segment* s;
	Textdscr* textd;
	WLDcrd cen;

	if(segmentp == NULL)
	{
		WOOPS("The passed segment pointer is null");
		return 0;
	}

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return 0;
	}

	if (!wldp)
		wldp = (WLDcrd *)wld_table->data;

	wgi_creseg (iden, segmentp->levl, segmentp->mask, segmentp->pick,
		segmentp->edge, segmentp->fill, segmentp->text,
		segmentp->visi, segmentp->draw);

	for(prim = segmentp->prim; prim; prim = prim->next)
	{
		switch (prim->type)
		{
		case WGI_PRIM_POLYLINE:
			wide = ((WLDcrd*) wld_ltabl.data)[prim->wide].x;
			pickdef.iden = prim->piid;
			pickdef.type = prim->ptyp;
			wgi_polyln (prim->nelm, wldp+prim->indx, wide,
				prim->user, prim->pick, pickdef, 0);
			break;
		case WGI_PRIM_ZEBRA:
			wide = ((WLDcrd*) wld_ltabl.data)[prim->wide].x;
			pickdef.iden = prim->piid;
			pickdef.type = prim->ptyp;
			wgi_zebra (prim->nelm, wldp+prim->indx, wide,
				prim->user % 2, prim->user / 2, 
				prim->pick, pickdef, 0);
			break;
		case WGI_PRIM_POLYGON:
			pickdef.iden = prim->piid;
			pickdef.type = prim->ptyp;
/* ?? enno: BUG fix: this used to be polyln */
			wgi_polygon (prim->nelm, wldp+prim->indx,
				prim->user, prim->pick, pickdef, 0);
			break;
		case WGI_PRIM_RECTANGLE:
			pickdef.iden = prim->piid;
			pickdef.type = prim->ptyp;
			wgi_rectangle (wldp+prim->indx, prim->user,
				prim->pick, pickdef, 0);
			break;
		case WGI_PRIM_TEXT:
			textd = (Textdscr *)txt_dscrp.data + prim->wide;
			pickdef.iden = prim->piid;
			pickdef.type = prim->ptyp;
/* ?? enno: BUG fix: was using lowerleft of 1st char as center */
			wgi_calc_text_center_from_base (textd->wide, 
				textd->high, textd->rotn, textd->refl, 
				wldp+prim->indx, &cen);
			wgi_text (&cen, prim->user, textd->high,
				textd->wide, textd->spac, textd->rotn,
				textd->refl, prim->pick, pickdef, 0);
			break;
		}
	}

	/* wgi_endseg clears isegmentp */
	s = isegmentp;
	wgi_endseg ();

	return s;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_undraw_pickdef(iden, levl, pick, nelm, done)
long iden;
long levl;
Pickdef *pick;
long nelm;
long *done;
{
	long rc;
	Pickdef *pptr;
	Segprim *prim;
	Segment *this;

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	sun_color_set(0, 0, 0);

	for(*done = FALSE, this = iseglistp->segs; this; this = this->next)
	{
		if((this->visi) && (this->iden == iden) && LOOK_LEVL(this->levl, levl))
		{
			sun_undrw_set(this->edge, this->fill, this->text);

			for(prim = this->prim; prim; prim = prim->next)
			{
				for(pptr = pick; pptr < (pick + nelm); pptr++)
				{
					if((pptr->iden == prim->piid) && (pptr->type == prim->ptyp))
					{
						*done = TRUE;
						if((rc = iwgi_draw_prim(prim, this)) != WGI_SUCCESS)
							return (rc);

					}
				}
			}

			if(iwgi_cont_flag == FALSE)
				return (WGI_SUCCESS);
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

long wgi_seglist_delview(list, vpid, dele, dean)
Seglist *list;
long vpid;
long dele;
long *dean;
{
	long rc, view;
	Segment *this, *back, *save;

	back = 0;
	this = list->segs;
	view = (1 << vpid);

	if((rc = wgi_seglist_current(list)) != WGI_SUCCESS)
		return (rc);

	while(this)
	{
		/*
			If This Is The Only Viewport This Segment Is In
			We Delete It If The Delete Flag Is On
		*/

		if((this->mask == view) && dele)
		{
			*dean |= list->mask;

			save = this->next;
			if((rc = iwgi_rm_segment(list, this, back, 1, 0, 0)) != WGI_SUCCESS)
				return (rc);
			this = save;
		}
		else
		{
			/*
				This Is Either Not The Only Viewport Segment Is In Or
				Delete Flag Is Off, So If Segment Is In Viewport Then
				We Set Clear Write Mask And If Delete Flag Is On Then
				We Turn Off Viewport In Viewport Mask
			*/

			if(this->mask & view)
			{
				*dean |= list->mask;

				if(dele)
				{
					this->mask &= ~view;
				}
			}

			/*
				Move To The Next Segment
			*/

			back = this;
			this = this->next;
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

long wgi_segment_view(iden, levl, mask, draw, done)
long iden;
long mask;
long draw;
long *done;
{
	long hold, rc;
	Segment *this;
	Segprim *prim;

	/*
		Check For A Valid Seglist Pointer
	*/

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Set The Done Flag Off And Loop Through Segment List
		Looking For A Matching Segment
	*/

	for(*done = FALSE, this = iseglistp->segs; this; this = this->next)
	{
		if((this->iden == iden) && LOOK_LEVL(this->levl, levl))
		{
			/*
				If The Draw Flag Is On And The Segment Is
				Visible Then Draw The Segment In The Only
				The Viewports It Was Not Already In
			*/

			if((draw) && (this->visi))
			{
				/*
					Hold Onto The Current Viewport Mask
				*/

				hold = this->mask;

				/*
					Turn Off Any Viewports That Segment Is
					Already In
				*/

				mask &= ~hold;

				/*
					Set The Segment Mask To This Temporary
					Viewport Mask
				*/

				this->mask = mask;

				/*
					Draw The Segment In The Viewports In
					Its Viewport Mask
				*/

				sun_color_set(this->edge, this->fill, this->text);

				if((rc = iwgi_draw_segm(this)) != WGI_SUCCESS)
					return (rc);

				/*
					Re-Combine Viewport Masks Into The
					Segments Viewport Mask
				*/

				this->mask = mask | hold;
			}
			else
			{
				/*
					Turn On The New Viewports In The
					Segments Viewport Mask
				*/

				this->mask |= mask;
			}

			/*
				Tell The Callers We Modified A Segments
				Viewport Mask.
			*/

			*done = TRUE;

			/*
				Should We Continue Looking For Segments ???
			*/

			if(iwgi_cont_flag == FALSE)
				return (WGI_SUCCESS);
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

static long iwgi_rm_segment(list, this, back, link, hold, draw)
Seglist *list;
Segment *this;
Segment *back;
long link;
long hold;
long draw;
{
	long rc;
	Segprim *prim, *temp;

	/*
		If Primitives Are To Be Un-Drawn Do Some Setup
	*/

	if((draw) && (this->visi))
	{
		sun_color_set(0, 0, 0);
		sun_undrw_set(this->edge, this->fill, this->text);

		if((rc = iwgi_draw_segm(this)) != WGI_SUCCESS)
			return (rc);
	}

	/*
		Point To The Primitive List And Null Out The Segment Primitive
		Pointer So It Is Not Be Seen In The Holding Area Segment
	*/

	prim = this->prim;
	this->prim = NULL;

	/*
		Delete The Primitives 
	*/

	while(prim)
	{
		/*
			Free Text Primitive String
		*/

		switch(prim->type)
		{
			case WGI_PRIM_TEXT:
				ewgi_free((char *)prim->user);
			break;
		}

		/*
			Free WLD And VDC Coordinate Data Points
		*/

		if((rc = wgi_free_elements(prim->indx, prim->nelm)) != WGI_SUCCESS)
			return (rc);

		/*
			Point To The Next Primitive And Free Up The Memory
		*/

		temp = prim;
		prim = prim->next;
		ewgi_free((char *)temp);
	}

	/*
		We Have A Segment In The Holding Area That Was Not
		Re-Used So We Must Delete It Now
	*/

	if(list->hold)
	{
		ewgi_free((char *)list->hold);
		list->hold = NULL;
		list->back = NULL;
	}

	/*
		Do We Hold This Segment
	*/

	if(hold)
	{
		/*
			Save This Segment And The Previous Segment In The
			Holding Area
		*/

		list->back = back;
		list->hold = this;

		if((rc = iwgi_ul_segment(list, this, back)) != WGI_SUCCESS)
			return (rc);

		return (WGI_SUCCESS);
	}

	/*
		Un-Link This Segment From The List
	*/

	if(link)
	{
		if((rc = iwgi_ul_segment(list, this, back)) != WGI_SUCCESS)
			return (rc);
	}

	/*
		Free The Memory For The Segment
	*/

	ewgi_free((char *)this);

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long iwgi_ul_segment(list, this, back)
Seglist *list;
Segment *this;
Segment *back;
{
	/*
		If We Are Removing The Last Segment In The
		List We Reset The Last Segment Pointer
	*/

	if(this == list->last)
	{
		list->last = back;
	}

	/*
		If We Are Removing The First Segment In The List
		We Reset The First Segment Pointer, Else We Must
		Unlink The Segment From The Middle Of The List
	*/

	if(this == list->segs)
	{
		list->segs = this->next;
	}
	else
	{
		back->next = this->next;
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

static long iwgi_ln_segment(list, this)
Seglist *list;
Segment *this;
{
	long rc;

	if(list->hold)
	{
		if((this->iden == list->hold->iden) && LOOK_LEVL(this->levl, list->hold->levl))
		{
			if(list->back == list->last)
			{
				list->last = this;
			}

			if(list->back)
			{
				this->next = list->back->next;
				list->back->next = this;
			}
			else
			{
				this->next = list->segs;
				list->segs = this;
			}

			ewgi_free((char *)list->hold);
			list->hold = NULL;
			list->back = NULL;

			return (WGI_SUCCESS);
		}

		ewgi_free((char *)list->hold);
		list->hold = NULL;
		list->back = NULL;
	}

	if(list->last)
	{
		list->last->next = this;
		list->last = this;
	}
	else
	{
		list->segs = this;
		list->last = this;
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

long iwgi_draw_segm(this)
Segment *this;
{
	long rc;
	long iden = 1;
	Segprim *prim;
	long mask = this->mask >> 1;
	VDCcrd *vdcp = (VDCcrd *)vdc_tbuff->data;
	WLDcrd *wldp = (WLDcrd *)wld_table->data;

	if(this->visi == FALSE)
	    return (WGI_SUCCESS);

	while(mask)
	{
	    while(!(mask & 0x1))
	    {
		iden++; mask >>= 1;
	    }

	    wgi_viewport(iden);

	    if(iwgi_extent_overlap(this->xtnt))
	    {
		if(iwgi_extent_outside(this->xtnt))
		{
		    for(prim = this->prim; prim; prim = prim->next)
		    {
			if(iwgi_extent_overlap(prim->xtnt))
			{
			    wgi_wldvdc(prim->nelm, wldp+prim->indx, vdcp+prim->indx);

			    dispatch_tabl[iwgi_extent_outside(prim->xtnt)][prim->type]
					(vdcp + prim->indx, prim);
			}
		    }
		}
		else
		{
		    for(prim = this->prim; prim; prim = prim->next)
		    {
			wgi_wldvdc(prim->nelm, wldp+prim->indx, vdcp+prim->indx);

			dispatch_draw[prim->type](vdcp + prim->indx, prim);
		    }
		}
	    }
	    iden++; mask >>= 1;
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--   Change the color of all the segments in the current segment list    --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_change_seglist_colors(edge, fill, text)
long edge;
long fill;
long text;
{
	register Segment *this;

	/*
		Check that the color indexes are within range
	*/

	if((fill < -1) || (fill > 255))		/* fill index */
	{
		WOOPS("The segment fill color %d is out of range", fill);
		return (WGI_PROBLEM);
	}

	if((edge < -1) || (edge > 255))		/* edge index */
	{
		WOOPS("The segment edge color %d is out of range", edge);
		return (WGI_PROBLEM);
	}

	if((text < -1) || (text > 255))		/* text index */
	{
		WOOPS("The segment text color %d is out of range", edge);
		return (WGI_PROBLEM);
	}

	if(!iseglistp)
	{
		WOOPS("The current seglist pointer is null");
		return (WGI_PROBLEM);
	}

	for (this = iseglistp->segs; this; this = this->next)
	{
	    if (fill != -1)
	        this->fill = fill;
	    if (edge != -1)
	        this->edge = edge;
	    if (text != -1)
	        this->text = text;
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

long wgi_change_seglist_fill_style(fill)
long fill;
{
	register Segment *segp;
	register Segprim *prim;

	for (segp = iseglistp->segs; segp; segp = segp->next)
	{
		for (prim = segp->prim; prim; prim = prim->next)
		{
			switch(prim->type)
			{
			case WGI_PRIM_POLYGON:
			case WGI_PRIM_RECTANGLE:
			case WGI_PRIM_CIRCLE:
			case WGI_PRIM_STENCIL:
			case WGI_PRIM_HOLE:
				prim->user = fill;
			break;
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
