#include "wgiStruct.h"

extern Seglist *iseglistp;

extern long wgirti;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static WLDcrd drag_here;
static Segment *drag_this;
static long drag_view = 0;
static long drag_draw = 0;

long wgi_drag_segment(iden, here)
long iden;
WLDcrd here;
{
	long rc;
	Segment *this, *back;

	for(back = 0, this = iseglistp->segs; this; back = this, this = this->next)
	{
		if(this->iden == iden)
		{
			if(back)
			{
				back->next = this->next;
				if (this == iseglistp->last)
					iseglistp->last = back;
			}
			else
			{
				iseglistp->segs = this->next;
				iseglistp->last = this->next;
			}

			drag_this = this;
			drag_here = here;

			this->next = NULL;

			return (WGI_SUCCESS);
		}
	}
	drag_this = 0;

	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_drag_cancel()
{
	drag_draw = FALSE;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_drag_update(view, xloc, yloc)
long view;
long xloc;
long yloc;
{
	long rc;
	Segprim *prim;
	WLDcrd from;
	VDCcrd *vdct, *vdce, slop, more;
	VDCcrd *vdcp = (VDCcrd *)vdc_tbuff->data;
	WLDcrd *wldp = (WLDcrd *)wld_table->data;

	if(!drag_this)
		return (WGI_PROBLEM);

	if(drag_draw)
	{
		if((rc = wgi_viewport(drag_view)) != WGI_SUCCESS)
			return (rc);

		sun_color_set(0, 0, 0);

		for(prim = drag_this->prim; prim; prim = prim->next)
		{
			dispatch_draw[prim->type](vdcp+prim->indx, prim);
		}
		drag_draw = 0;
	}

	if((rc = wgi_viewport(view)) != WGI_SUCCESS)
		return (rc);

	drag_view = view;

	from.x = (Single)xloc;
	from.y = (Single)yloc;
	wgi_wldvdc(1, &from, &more);
	wgi_wldvdc(1, &drag_here, &slop);

	for(prim = drag_this->prim; prim; prim = prim->next)
	{
		if(wgi_wldvdc(prim->nelm, wldp+prim->indx, vdcp+prim->indx) != WGI_SUCCESS)
			return (WGI_PROBLEM);

		vdct = vdcp + prim->indx;
		vdce = vdcp + prim->indx + prim->nelm;

		while(vdct < vdce)
		{
			vdct->x = vdct->x - slop.x + more.x;
			vdct->y = vdct->y - slop.y + more.y;
			vdct++;
		}
	}

	sun_color_set(wgirti, wgirti, wgirti);

	for(prim = drag_this->prim; prim; prim = prim->next)
	{
		dispatch_draw[prim->type](vdcp+prim->indx, prim);
	}
	drag_draw = 1;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_drag_done()
{
	long rc;
	Segprim *prim, *temp;
	VDCcrd *vdcp = (VDCcrd *)vdc_tbuff->data;

	if(!drag_this)
		return (WGI_PROBLEM);

	if(drag_draw)
	{
		if((rc = wgi_viewport(drag_view)) != WGI_SUCCESS)
			return (rc);

		sun_color_set(0, 0, 0);

		prim = drag_this->prim;
		while(prim)
		{
			dispatch_draw[prim->type](vdcp+prim->indx, prim);

			switch(prim->type)
			{
				case WGI_PRIM_TEXT:
					WGI_FREE((char *)prim->user);
					break;
			}

			if((rc = wgi_free_elements(prim->indx, prim->nelm)) != WGI_SUCCESS)
				return (rc);

			temp = prim;
			prim = prim->next;
			WGI_FREE((char *)temp);

		}

		WGI_FREE((char *)drag_this);

		drag_this = 0;
		drag_draw = 0;
	}

	drag_view = 0;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_drag_undraw()
{
	long rc;
	Segprim *prim, *temp;
	VDCcrd *vdcp = (VDCcrd *)vdc_tbuff->data;

	if(!drag_this)
		return (WGI_PROBLEM);

	if(drag_draw)
	{
		if((rc = wgi_viewport(drag_view)) != WGI_SUCCESS)
			return (rc);

		sun_color_set(0, 0, 0);

		prim = drag_this->prim;
		while(prim)
		{
			dispatch_draw[prim->type](vdcp+prim->indx, prim);
			prim = prim->next;
		}

		drag_this = 0;
		drag_draw = 0;
	}

	drag_view = 0;

	return (WGI_SUCCESS);
}
