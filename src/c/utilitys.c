#include "wgiStruct.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- A Utility Routine To Set Internal Flags That Control The State Of The --*/
/*--                       WGI Graphics Package                            --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_flag(flag, valu)
long flag;
Flag valu;
{
	switch(flag)
	{
		case WGI_DRAW:
			iwgi_draw_flag = valu;
			break;
		case WGI_LEVL:
			iwgi_levl_flag = valu;
			break;
		case WGI_CONT:
			iwgi_cont_flag = valu;
			break;
		case WGI_ZERO:
			iwgi_zero_flag = valu;
			break;
		default:
			WOOPS("Invalid WGI Flag (%d,%d)", flag, valu);
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
