#include "Zfile.h"
#include "Form.h"

#define Z_ERROR(func,str) ZDefaultError(__FILE__, __LINE__, func, str);

void ZDefaultError (const char* file, int line, const char* func,
	const char* msg)
{
    fprintf(stderr, "Error in %s: %s [%s/%d]\n",
	func, msg, file, line);
}

Zfile::Zfile (const char* fileName)
: priv_file (NULL), priv_is_pipe (FALSE)
{
    char open_line[500];	// was: UNIX_FILEPATH_LENGTH
    sprintf(open_line, "%s.Z", fileName);
    priv_file = fopen (open_line, "r");

    if (priv_file = fopen (open_line, "r"))	// open succeeded
    {
	fclose(priv_file);
	sprintf (open_line, "uncompress < %s.Z", fileName);
	if (priv_file = popen(open_line, "r"))
	{
	    priv_is_pipe = TRUE;
	}
	else
	{
	    Z_ERROR ("Zfile::Zfile", form("can't open pipe '%s'",
		open_line));
	}
    }
    else	// regular file
    {
	if ((priv_file = fopen (fileName, "r")) == NULL)
	{
	    Z_ERROR ("Zfile::Zfile", form("can't open file '%s'",
		fileName));
	}
    }
}

Zfile::~Zfile ()
{
    if (priv_file)
    {
	if (priv_is_pipe)
	    pclose (priv_file);
	else
	    fclose (priv_file);
    }
}
