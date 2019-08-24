#include "wgiSymbol.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Stroke data for each character -- name is SS_(n) where (n) is numeric --*/
/*-- value for each character                                              --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static short SS_032[] = {15,15,15,15};
static short SS_033[] = {4,4,4,14,15,15,15,15};
static short SS_034[] = {3,12,2,14,3,14,3,12,15,15,5,12,4,14,5,14,5,12,15,15,15,15};
static short SS_035[] = {0,8,8,8,15,15,0,3,8,3,15,15,1,0,3,11,3,11,15,15,7,11,5,0,15,15,15,15};
static short SS_036[] = {0,2,2,0,6,0,8,2,8,4,6,6,2,6,0,8,0,10,2,12,6,12,8,10,15,15,4,13,4,-1,15,15,15,15};
static short SS_037[] = {0,12,0,14,2,14,2,12,0,12,15,15,0,0,8,14,15,15,6,0,6,2,8,2,8,0,6,0,15,15,15,15};
static short SS_038[] = {8,0,1,11,1,12,3,14,4,14,6,11,0,4,0,2,2,0,4,0,8,3,15,15,15,15};
static short SS_039[] = {3,11,3,14,5,14,3,11,15,15,15,15};
static short SS_040[] = {6,0,4,2,4,12,6,14,15,15,15,15};
static short SS_041[] = {2,0,4,2,4,12,2,14,15,15,15,15};
static short SS_042[] = {2,3,6,11,15,15,0,7,8,7,15,15,2,11,6,3,15,15,15,15};
static short SS_043[] = {0,7,8,7,15,15,4,11,4,3,15,15,15,15};
static short SS_044[] = {2,2,4,2,4,2,4,0,3,-4,3,-4,3,0,2,0,2,0,2,2,15,15,15,15};
static short SS_045[] = {0,7,8,7,15,15,15,15};
static short SS_046[] = {2,0,2,2,4,2,4,0,2,0,15,15,15,15};
static short SS_047[] = {0,0,8,14,15,15,15,15};
static short SS_048[] = {0,2,2,0,6,0,8,2,8,12,6,14,2,14,0,12,0,2,15,15,0,0,8,14,15,15,15,15};
static short SS_049[] = {1,0,7,0,15,15,4,0,4,14,1,11,15,15,15,15};
static short SS_050[] = {0,10,0,12,2,14,6,14,8,12,8,10,0,2,0,0,8,0,15,15,15,15};
static short SS_051[] = {0,12,2,14,6,14,8,12,8,10,6,8,8,6,8,2,6,0,2,0,0,2,15,15,2,8,6,8,15,15,15,15};
static short SS_052[] = {8,5,0,5,3,13,3,13,15,15,6,14,6,0,15,15,15,15};
static short SS_053[] = {8,14,0,14,0,8,6,8,8,6,8,2,6,0,2,0,0,2,0,4,15,15,15,15};
static short SS_054[] = {8,12,6,14,2,14,0,12,0,2,2,0,6,0,8,2,8,6,6,8,2,8,0,6,15,15,15,15};
static short SS_055[] = {0,14,8,14,8,12,0,0,15,15,15,15};
static short SS_056[] = {0,12,2,14,6,14,8,12,8,10,6,8,8,6,8,2,6,0,2,0,0,2,0,6,2,8,0,10,0,12,15,15,2,8,6,8,15,15,15,15};
static short SS_057[] = {0,2,2,0,6,0,8,2,8,12,6,14,2,14,0,12,0,8,2,6,6,6,8,8,15,15,15,15};
static short SS_058[] = {2,5,4,5,4,3,2,3,2,5,15,15,2,9,2,11,4,11,4,9,2,9,15,15,15,15};
static short SS_059[] = {2,5,4,5,4,5,4,3,3,-1,3,-1,3,3,2,3,2,3,2,5,15,15,2,9,2,11,4,11,4,9,2,9,15,15,15,15};
static short SS_060[] = {8,4,0,7,8,10,15,15,15,15};
static short SS_061[] = {0,3,8,3,15,15,0,8,8,8,15,15,15,15};
static short SS_062[] = {0,4,8,7,0,10,15,15,15,15};
static short SS_063[] = {4,4,4,8,6,8,8,10,8,12,6,14,2,14,0,12,0,10,15,15,15,15,15,15};
static short SS_064[] = {8,2,6,0,2,0,0,2,0,12,2,14,6,14,8,12,8,6,7,5,3,5,2,6,2,11,3,12,7,12,8,11,15,15,15,15};
static short SS_065[] = {0,0,0,11,3,14,5,14,8,11,8,0,15,15,0,6,8,6,15,15,15,15};
static short SS_066[] = {0,0,0,14,6,14,8,12,8,10,6,8,8,6,8,2,6,0,0,0,15,15,0,8,6,8,15,15,15,15};
static short SS_067[] = {8,2,6,0,2,0,0,2,0,12,2,14,6,14,8,12,15,15,15,15};
static short SS_068[] = {0,0,0,14,6,14,8,12,8,2,6,0,0,0,15,15,15,15};
static short SS_069[] = {8,0,0,0,0,14,8,14,15,15,0,8,6,8,15,15,15,15};
static short SS_070[] = {0,0,0,14,8,14,15,15,0,8,6,8,15,15,15,15};
static short SS_071[] = {8,10,8,12,6,14,2,14,0,12,0,2,2,0,6,0,8,2,8,6,6,6,15,15,15,15};
static short SS_072[] = {0,8,8,8,15,15,0,0,0,14,15,15,8,0,8,14,15,15,15,15};
static short SS_073[] = {1,14,7,14,15,15,1,0,7,0,15,15,4,0,4,14,15,15,15,15};
static short SS_074[] = {0,4,0,2,2,0,6,0,8,2,8,14,15,15,15,15};
static short SS_075[] = {8,14,0,6,15,15,0,0,0,14,15,15,8,0,4,10,15,15,15,15};
static short SS_076[] = {0,14,0,0,8,0,15,15,15,15};
static short SS_077[] = {0,0,0,14,4,8,8,14,8,0,15,15,15,15};
static short SS_078[] = {0,0,0,14,8,0,8,14,15,15,15,15};
static short SS_079[] = {0,2,2,0,6,0,8,2,8,12,6,14,2,14,0,12,0,2,15,15,15,15};
static short SS_080[] = {0,0,0,14,6,14,8,12,8,10,6,8,0,8,15,15,15,15};
static short SS_081[] = {2,0,0,2,0,12,2,14,6,14,8,12,8,2,6,0,2,0,15,15,5,3,8,0,15,15,15,15};
static short SS_082[] = {0,0,0,14,6,14,8,12,8,10,6,8,0,8,15,15,4,8,8,0,15,15,15,15};
static short SS_083[] = {0,4,0,2,2,0,6,0,8,2,8,6,6,8,2,8,0,10,0,12,2,14,6,14,8,12,15,15,15,15};
static short SS_084[] = {0,14,8,14,15,15,4,14,4,0,15,15,15,15};
static short SS_085[] = {0,14,0,2,2,0,6,0,8,2,8,14,15,15,15,15};
static short SS_086[] = {0,14,4,0,8,14,15,15,15,15};
static short SS_087[] = {0,14,0,0,4,8,8,0,8,14,15,15,15,15};
static short SS_088[] = {0,0,8,14,15,15,8,0,0,14,15,15,15,15};
static short SS_089[] = {0,14,4,8,8,14,15,15,4,8,4,0,15,15,15,15};
static short SS_090[] = {0,14,8,14,0,0,8,0,15,15,3,8,6,8,15,15,15,15};
static short SS_091[] = {6,0,4,0,4,14,6,14,15,15,15,15};
static short SS_092[] = {0,14,8,0,15,15,15,15};
static short SS_093[] = {2,0,4,0,4,14,2,14,15,15,15,15};
static short SS_094[] = {6,4,3,12,0,4,15,15,15,15};
static short SS_095[] = {0,-2,8,-2,15,15,15,15};
static short SS_096[] = {3,14,5,14,5,11,3,14,15,15,15,15};
static short SS_097[] = {6,2,4,0,2,0,0,2,0,6,2,8,4,8,6,6,15,15,6,8,6,0,15,15,15,15};
static short SS_098[] = {0,12,0,0,15,15,0,2,2,0,4,0,6,2,6,6,4,8,2,8,0,6,15,15,15,15};
static short SS_099[] = {6,2,4,0,2,0,0,2,0,6,2,8,4,8,6,6,15,15,15,15};
static short SS_100[] = {6,2,4,0,2,0,0,2,0,6,2,8,4,8,6,6,15,15,6,12,6,0,15,15,15,15};
static short SS_101[] = {6,2,4,0,2,0,0,2,0,6,2,8,4,8,6,6,6,4,0,4,15,15,15,15};
static short SS_102[] = {0,6,4,6,15,15,6,10,4,12,2,10,2,0,15,15,15,15};
static short SS_103[] = {0,-4,2,-6,4,-6,6,-4,6,6,4,8,2,8,0,6,0,2,2,0,4,0,6,2,15,15,15,15};
static short SS_104[] = {0,12,0,0,15,15,0,6,2,8,4,8,6,6,6,0,15,15,15,15};
static short SS_105[] = {3,0,3,8,2,8,15,15,15,15};
static short SS_106[] = {0,-4,2,-6,4,-6,6,-4,6,8,5,8,15,15,15,15};
static short SS_107[] = {0,4,5,9,15,15,0,0,0,12,15,15,2,6,6,0,15,15,15,15};
static short SS_108[] = {4,0,3,1,3,11,2,12,15,15,15,15};
static short SS_109[] = {0,6,2,8,4,6,6,8,8,6,8,0,15,15,0,0,0,8,15,15,4,6,4,0,15,15,15,15};
static short SS_110[] = {0,8,0,0,15,15,0,6,2,8,4,8,6,6,6,0,15,15,15,15};
static short SS_111[] = {2,0,0,2,0,6,2,8,4,8,6,6,6,2,4,0,2,0,15,15,15,15};
static short SS_112[] = {0,8,0,-4,15,15,0,2,2,0,4,0,6,2,6,6,4,8,2,8,0,6,15,15,15,15};
static short SS_113[] = {6,6,4,8,2,8,0,6,0,2,2,0,4,0,6,2,15,15,6,8,6,-4,15,15,15,15};
static short SS_114[] = {0,8,0,0,15,15,0,6,2,8,4,8,6,6,15,15,15,15};
static short SS_115[] = {0,2,2,0,4,0,6,2,4,4,2,4,0,6,2,8,4,8,6,6,15,15,15,15};
static short SS_116[] = {0,8,6,8,15,15,2,12,2,1,4,0,6,1,15,15,15,15};
static short SS_117[] = {0,8,0,2,2,0,4,0,6,2,15,15,6,8,6,0,15,15,15,15};
static short SS_118[] = {0,8,3,0,6,8,15,15,15,15};
static short SS_119[] = {0,8,2,0,4,6,6,0,8,8,15,15,15,15};
static short SS_120[] = {0,0,6,8,15,15,0,8,6,0,15,15,15,15};
static short SS_121[] = {6,8,6,-4,4,-6,2,-6,0,-4,15,15,0,8,0,2,2,0,4,0,6,2,15,15,15,15};
static short SS_122[] = {0,8,6,8,0,0,6,0,15,15,2,4,4,4,15,15,15,15};
static short SS_123[] = {6,0,5,0,4,1,4,6,3,7,4,8,4,13,5,14,6,14,15,15,15,15};
static short SS_124[] = {4,14,3,14,3,0,4,0,4,14,15,15,15,15};
static short SS_125[] = {2,0,3,1,3,6,4,7,3,8,3,13,2,14,15,15,15,15};
static short SS_126[] = {0,11,2,14,6,11,8,14,15,15,15,15};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The stroke array holds pointers to the stroke data for each character --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

short *iwgi_font_data[95] = {
	SS_032, /* letter ( ) has ( 0) total points */
	SS_033, /* letter (!) has ( 2) total points */
	SS_034, /* letter (") has (12) total points */
	SS_035, /* letter (#) has (10) total points */
	SS_036, /* letter ($) has (24) total points */
	SS_037, /* letter (%) has (18) total points */
	SS_038, /* letter (&) has (20) total points */
	SS_039, /* letter (') has ( 6) total points */
	SS_040, /* letter (() has ( 6) total points */
	SS_041, /* letter ()) has ( 6) total points */
	SS_042, /* letter (*) has ( 6) total points */
	SS_043, /* letter (+) has ( 4) total points */
	SS_044, /* letter (,) has (18) total points */
	SS_045, /* letter (-) has ( 2) total points */
	SS_046, /* letter (.) has ( 8) total points */
	SS_047, /* letter (/) has ( 2) total points */
	SS_048, /* letter (0) has (18) total points */
	SS_049, /* letter (1) has ( 6) total points */
	SS_050, /* letter (2) has (16) total points */
	SS_051, /* letter (3) has (22) total points */
	SS_052, /* letter (4) has ( 8) total points */
	SS_053, /* letter (5) has (18) total points */
	SS_054, /* letter (6) has (22) total points */
	SS_055, /* letter (7) has ( 6) total points */
	SS_056, /* letter (8) has (30) total points */
	SS_057, /* letter (9) has (22) total points */
	SS_058, /* letter (:) has (16) total points */
	SS_059, /* letter (;) has (26) total points */
	SS_060, /* letter (<) has ( 4) total points */
	SS_061, /* letter (=) has ( 4) total points */
	SS_062, /* letter (>) has ( 4) total points */
	SS_063, /* letter (?) has (16) total points */
	SS_064, /* letter (@) has (32) total points */
	SS_065, /* letter (A) has (12) total points */
	SS_066, /* letter (B) has (20) total points */
	SS_067, /* letter (C) has (14) total points */
	SS_068, /* letter (D) has (12) total points */
	SS_069, /* letter (E) has ( 8) total points */
	SS_070, /* letter (F) has ( 6) total points */
	SS_071, /* letter (G) has (20) total points */
	SS_072, /* letter (H) has ( 6) total points */
	SS_073, /* letter (I) has ( 6) total points */
	SS_074, /* letter (J) has (10) total points */
	SS_075, /* letter (K) has ( 6) total points */
	SS_076, /* letter (L) has ( 4) total points */
	SS_077, /* letter (M) has ( 8) total points */
	SS_078, /* letter (N) has ( 6) total points */
	SS_079, /* letter (O) has (16) total points */
	SS_080, /* letter (P) has (12) total points */
	SS_081, /* letter (Q) has (18) total points */
	SS_082, /* letter (R) has (14) total points */
	SS_083, /* letter (S) has (24) total points */
	SS_084, /* letter (T) has ( 4) total points */
	SS_085, /* letter (U) has (10) total points */
	SS_086, /* letter (V) has ( 4) total points */
	SS_087, /* letter (W) has ( 8) total points */
	SS_088, /* letter (X) has ( 4) total points */
	SS_089, /* letter (Y) has ( 6) total points */
	SS_090, /* letter (Z) has ( 8) total points */
	SS_091, /* letter ([) has ( 6) total points */
	SS_092, /* letter (\) has ( 2) total points */
	SS_093, /* letter (]) has ( 6) total points */
	SS_094, /* letter (^) has ( 4) total points */
	SS_095, /* letter (_) has ( 2) total points */
	SS_096, /* letter (`) has ( 6) total points */
	SS_097, /* letter (a) has (16) total points */
	SS_098, /* letter (b) has (16) total points */
	SS_099, /* letter (c) has (14) total points */
	SS_100, /* letter (d) has (16) total points */
	SS_101, /* letter (e) has (18) total points */
	SS_102, /* letter (f) has ( 8) total points */
	SS_103, /* letter (g) has (22) total points */
	SS_104, /* letter (h) has (10) total points */
	SS_105, /* letter (i) has ( 2) total points */
	SS_106, /* letter (j) has ( 8) total points */
	SS_107, /* letter (k) has ( 6) total points */
	SS_108, /* letter (l) has ( 6) total points */
	SS_109, /* letter (m) has (14) total points */
	SS_110, /* letter (n) has (10) total points */
	SS_111, /* letter (o) has (16) total points */
	SS_112, /* letter (p) has (16) total points */
	SS_113, /* letter (q) has (16) total points */
	SS_114, /* letter (r) has ( 8) total points */
	SS_115, /* letter (s) has (18) total points */
	SS_116, /* letter (t) has ( 8) total points */
	SS_117, /* letter (u) has (10) total points */
	SS_118, /* letter (v) has ( 4) total points */
	SS_119, /* letter (w) has ( 8) total points */
	SS_120, /* letter (x) has ( 4) total points */
	SS_121, /* letter (y) has (16) total points */
	SS_122, /* letter (z) has ( 8) total points */
	SS_123, /* letter ({) has (16) total points */
	SS_124, /* letter (|) has ( 8) total points */
	SS_125, /* letter (}) has (12) total points */
	SS_126, /* letter (~) has ( 6) total points */
};
