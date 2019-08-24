#include <math.h>

#include "wgiStruct.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--      Rotation And Reflection Transform On A List Of Coordinates       --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_xfrm(nelm, buff, cent, rotn, refl)
WLDcrd *buff;
WLDcrd *cent;
long nelm;
long rotn;
long refl;
{
	long rc;

	double dx, dy, cx, cy;
	double r11, r12, r21, r22;

	/*
		Point At The End Of The List As The Terminator
	*/

	WLDcrd *endp = buff + nelm;

	/*
		Normalize The Rotation
	*/

	while(rotn <    0) rotn = rotn + 360;
	while(rotn >= 360) rotn = rotn - 360;

	/*
		Do We Have Nothing To Do ???
	*/

	if((rotn == 0) && (refl == WGI_NREFL))
		return (WGI_SUCCESS);

	/*
		Calculate The Rotation And Reflection Matrix
	*/

	if((rc = iwgi_rotn_matx(rotn, &r12, &r11)) != WGI_SUCCESS)
		return (rc);

	r21 = r12;
	r22 = r11;

	/*
		Modify The Matrix For X Reflection
	*/

	if(refl & WGI_XREFL)
		r22 = -r22;
	else
		r21 = -r21;

	/*
		Modify The Matrix For Y Reflection
	*/

	if(refl & WGI_YREFL)
	{
		r11 = -r11;
		r12 = -r12;
	}

	cx = cent->x;
	cy = cent->y;

	/*
		Round And Round We Go...
	*/

	while(buff < endp)
	{
		dx = buff->x - cx;
		dy = buff->y - cy;

		buff->x = ((dx * r11) + (dy * r21)) + cx;
		buff->y = ((dx * r12) + (dy * r22)) + cy;

		buff++;
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--     Tabular Replacement For sin() And cos() For Values In Degrees     --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long	sin_phase[] = { 1, 1,-1,-1 };

static long	cos_phase[] = { 1,-1,-1, 1 };

static double	sin_value[91] = {
	0.000000000000000000, 0.017452406436340361, 0.034899496700615529,
	0.052335956240117825, 0.069756473740361310, 0.087155742742959643,
	0.104528463262024703, 0.121869343398593629, 0.139173100952592521,
	0.156434465031845731, 0.173648177657640701, 0.190808995366359201,
	0.207911690806687149, 0.224951054331916450, 0.241921895586853863,
	0.258819045088853455, 0.275637355802491124, 0.292371704707401398,
	0.309016994358799135, 0.325568154440210499, 0.342020143307940577,
	0.358367949526806783, 0.374606593396670650, 0.390731128469302664,
	0.406736643055118374, 0.422618261719326526, 0.438371146767033881,
	0.453990499716853768, 0.469471562762570115, 0.484809620222411297,
	0.499999999975492470, 0.515038074884988772, 0.529919264207606222,
	0.544639034988920311, 0.559192903444157896, 0.573576436324001504,
	0.587785252265000013, 0.601815023124174353, 0.615661475297411842,
	0.629320391021247405, 0.642787609657635072, 0.656059028961318801,
	0.669130606329416000, 0.681998360032833539, 0.694658370429141081,
	0.707106781156532038, 0.719339800308508803, 0.731353701588934157,
	0.743144825447097212, 0.754709580192447970, 0.766044443088661097,
	0.777145961426695486, 0.788010753576522926, 0.798635510017205318,
	0.809016994345006845, 0.819152044259233913, 0.829037572525502586,
	0.838670567916139964, 0.848048096127433481, 0.857167300673448129,
	0.866025403756139766, 0.874619707111499344, 0.882947592831470196,
	0.891006524161388280, 0.898794046272702118, 0.906307787010737464,
	0.913545457617278477, 0.920504853427745817, 0.927183854542758600,
	0.933580426473876512, 0.939692620763324559, 0.945518575577512199,
	0.951056516274165986, 0.956304755942902606, 0.961261695919078307,
	0.965925826270757581, 0.970295726258652990, 0.974370064768896182,
	0.978147600718508103, 0.981627183433444794, 0.984807752999103924,
	0.987688340583185064, 0.990268068730805262, 0.992546151631780460,
	0.994521895359990848, 0.996194698084757379, 0.997564050254165368,
	0.998629534750278846, 0.999390827016198723, 0.999847695154926063,
	1.000000000000000000
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--            Return sin() And cos() For Values In Degrees               --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long iwgi_rotn_matx(rotn, sinp, cosp)
long rotn;
double *sinp, *cosp;
{
	long quad = rotn / 90;

	/*
		Adjust Indexes For Quadrant
	*/

	switch(quad)
	{

	case 1: 
		rotn = 180 - rotn; 
		break;
	case 2: 
		rotn = rotn - 180; 
		break;
	case 3:	
		rotn = 360 - rotn; 
		break;
	}

	if(sinp) *sinp = sin_phase[quad] * sin_value[rotn];
	if(cosp) *cosp = cos_phase[quad] * sin_value[90-rotn];

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/