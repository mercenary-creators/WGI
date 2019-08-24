#ifndef _WGISYMBOL_H_INCLUDED
#define _WGISYMBOL_H_INCLUDED

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Some Macros to help with the mapping of vectors for the text routines --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define FONT_MAXP		32		/* maximum points per stroke */
#define FONT_NLET		95		/* letters in the stroke lst */

#define FONT_MINC		(' ')
#define FONT_MAXC		('~')
#define FONT_HVAL		(14.0)
#define FONT_WVAL		( 8.0)
#define CHAR_ISOK(a)		(((a)>=FONT_MINC)&&((a)<=FONT_MAXC))
#define CHAR_FONT(a)		((CHAR_ISOK(a)?(a):(' '))-FONT_MINC)
#define	FONT_HFAC(a)		((a)/(FONT_HVAL))
#define	FONT_WFAC(a)		((a)/(FONT_WVAL))
#define FONT_STOP		(15)

extern short *iwgi_font_data[];

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--        Structures That Define The Vector Text Description Tables      --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Texttprm
{
	long  indx;
	long  nelm;
	struct Texttprm *next;

} Texttprm;

typedef struct Texttseg
{
	Texttprm *prim;

} Texttseg;

typedef struct Textdscr
{
	long  high;
	long  wide;
	long  spac;
	long  rotn;
	long  refl;
	Texttseg *alph[FONT_NLET];

} Textdscr;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* _WGISYMBOL_H_INCLUDED */
