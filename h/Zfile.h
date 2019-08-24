#ifndef _ZFILE_H_INCLUDED
#define _ZFILE_H_INCLUDED

#include <stdio.h>
#include "Boolean.h"

typedef FILE* FILEP;

class Zfile
{
 private:

  FILE* priv_file;
  Boolean priv_is_pipe;

 public:

  Zfile (const char* fileName);
	// Opens a file for reading.
	// If a file exists with extension ".Z", it will open a pipe
	//  that uncompresses the file, otherwise it will open a
	//  regular file.
	// If filePointer() is NULL, the file or pipe could not be opened.
  ~Zfile ();
	// Will close the file or pipe.

  operator int () { return priv_file != NULL; }
  FILE* filePointer () { return priv_file; }
  operator FILEP () { return filePointer (); }
	// Same as filePointer.
  Boolean isPipe () const { return priv_is_pipe; }
};

#ifdef EXAMPLE
Zfile file ("foo");
FILE* fp = file.filePointer ();
if (fp)
{
    // file or pipe was opened
    while (fgets (buffer, BUFSIZ, fp))
    {
	// etc...
    }
}
else
{
    // error: file not found
}
// NOTE: ~Zfile() will close file/pipe automatically
#endif

#endif /* _ZFILE_H_INCLUDED */
