#include "Form.h"
#include <stdarg.h>

#ifdef SOLARIS
extern "C"
{
 extern int vsprintf(char *, const char *, void *);
}
#endif
 
extern int debug_form_func ();

char*
fform (CircBuffer& cb, const char* format ...)
{
    va_list ap;
    va_start (ap, format);
    vsprintf (cb.next (), (char*) format, ap);
    va_end (ap);
    if (debug_form_func ()) cb.check ();
    return cb.curr ();
}

char*
CircBuffer_form (const char* format ...)
{
    va_list ap;
    char* buf = e_circ_buffer->next ();

    va_start (ap, format);
    vsprintf (buf, (char*) format, ap);
    va_end (ap);
    if (debug_form_func ()) e_circ_buffer->check ();
    return buf;
}
