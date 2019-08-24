#include "wgiStruct.h"
#include "wgiSymbol.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_text_dscr(high, wide, spac, rotn, refl)
long high;
long wide;
long spac;
long rotn;
long refl;
{
	long indx, rc;
	Textdscr *tdsc = (Textdscr *)txt_dscrp.data;
	Textdscr *ttry = (Textdscr *)txt_dscrp.data;
	Textdscr *tend = (Textdscr *)txt_dscrp.data + txt_dscrp.used;

	while(ttry < tend)
	{
		if(ttry->high == high)
			if(ttry->wide == wide)
				if(ttry->spac == spac)
					if(ttry->rotn == rotn)
						if(ttry->refl == refl)
							return (ttry - tdsc);
		ttry++;
	}

        if((rc = wgi_table_aloc(&txt_dscrp, &indx, 1)) != WGI_SUCCESS)
               	return (WGI_NEGATIV);

	tdsc = (Textdscr *)txt_dscrp.data + indx;

	tdsc->high = high;
	tdsc->wide = wide;
	tdsc->spac = spac;
	tdsc->rotn = rotn;
	tdsc->refl = refl;

	return (indx);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_text_save(indx, word)
long indx;
char *word;
{
	char c;
	long rc;

	Texttseg *tseg = 0;
	Textdscr *tdsc = (Textdscr *)txt_dscrp.data + indx;

	/*
		Loop Through The Word
	*/

	while(c = *word++)
	{
		if(!tdsc->alph[ CHAR_FONT(c) ])
		{
			/*
				Add Vector Description To Alphabet List
			*/

			if(!(tseg = WGI_MALLOC(1, Texttseg)))
				return (WGI_PROBLEM);

			tseg->prim = 0;
			if((rc = save(c, tdsc, tseg)) != WGI_SUCCESS)
				return (rc);

			tdsc->alph[ CHAR_FONT(c) ] = tseg;

			wld_ttabl.sync++;
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

static long save(c, tdsc, tseg)
char c;
Textdscr *tdsc;
Texttseg *tseg;
{
	long rc, nelm, indx = 0;
	double dhig, dwid, hfac, wfac;
	WLDcrd cent[1], hold[FONT_MAXP];

	short *font = iwgi_font_data[ CHAR_FONT(c) ];

	cent[0].x = 0.0;
	cent[0].y = 0.0;

	dhig = tdsc->high;
	dwid = tdsc->wide;

	hfac = FONT_HFAC(dhig);
	wfac = FONT_WFAC(dwid);

	while(font[indx] != FONT_STOP)
	{
		nelm = 0;
		while(font[indx] != FONT_STOP)
		{
			hold[nelm].x = wfac * font[indx++];
			hold[nelm].y = hfac * font[indx++];
			nelm++;
		}

		if((rc = iwgi_xfrm(nelm, hold, cent, tdsc->rotn, tdsc->refl)) != WGI_SUCCESS)
			return (rc);

		if((rc = keep(tseg, nelm, hold)) != WGI_SUCCESS)
			return (rc);

		indx = indx + 2;
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

static long keep(tseg, nelm, buff)
Texttseg *tseg;
long nelm;
WLDcrd *buff;
{
	WLDcrd *wldp;
	long rc, indx;
	Texttprm *tprm;

	if(!(tprm = WGI_MALLOC(1, Texttprm)))
		return (WGI_PROBLEM);

	/*
               	Allocate A New Entry In The Table
        */

        if((rc = wgi_table_aloc(&wld_ttabl, &indx, nelm)) != WGI_SUCCESS)
               	return (rc);

	tprm->indx = indx;
	tprm->nelm = nelm;

	tprm->next = tseg->prim;
	tseg->prim = tprm;

	wldp = (WLDcrd *)wld_ttabl.data + indx;

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
