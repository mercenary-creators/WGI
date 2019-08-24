#include <stdio.h>
#include <varargs.h>

#include "wgiStruct.h"

static char *hold_file = "UNKNOWN";	 /* File Name In Which Error Occured */
static FILE *hold_errf = stderr;	 /* File For Printing Error Messages */
static long  hold_line = 0;		 /* File Line In Which Error Occured */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--    Extract The Base File Name From The Complete Path Of A File Name   --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static char *basename(path)
char *path;
{
	char *base, *strrchr();

	/*
		Look For The Last Slash In The Path Name
	*/

	if(base = strrchr(path, '/'))
	{
		/*
			Point To The Next Character After Slash
		*/

		return (++base);
	}

	/*
		Ther Was No Slash So Just Return Path
	*/

	return (path);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--        Set The File That Error Messages Will Be Printed On            --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_error_file(errf)
FILE *errf;
{
	/*
		If The Passed in FILE Pointer Is Not Null Then Hold It
		For Printing Error Messages
	*/

	if(errf)
	{
		hold_errf = errf;

		return (WGI_SUCCESS);
	}

	WOOPS("The error file pointer is null");
	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Remember The File And Line For The Two Part WOOPS() Error Print Macro --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void wgi_error_set(line, file)
long line;
char *file;
{
	/*
		Hold The Line And File Values For The Error For Printing Later
	*/

	hold_line = line;
	hold_file = file ? basename(file) : "UNKNOWN";
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Print File And Line Error Was On With A printf() Like Argument List  --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define WGI_ERROR_STRING	"%%%%-WGI_ERROR-%%%% FILE: %s LINE: %d `%s'\n"

void wgi_error_print(mesg, va_alist)
char *mesg;
va_dcl
{
	va_list ap;
	char buff[512];

	va_start(ap);

	/*
		Concatenate Variable Arguments Into A Buffer For Printing
	*/

	vsprintf(buff, mesg, ap);

	/*
		Print Error Message Along With File And Line Information
	*/

	fprintf(hold_errf, WGI_ERROR_STRING, hold_file, hold_line, buff);

	va_end(ap);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--          Print Message With A printf() Like Argument List             --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void wgi_message_print(mesg, va_alist)
char *mesg;
va_dcl
{
	va_list ap;

	va_start(ap);

	/*
		This Just Prints A Message Without File And Line Information
		So We Can Just Use  The Already Defined Vfprintf() To Do The
		Job
	*/

	vfprintf(hold_errf, mesg, ap);

	va_end(ap);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
