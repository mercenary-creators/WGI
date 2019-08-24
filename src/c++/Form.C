// see also Form2.C

#include "Form.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "DebugFlag.h"

DEBUG_FLAG (debug_form, "form");

int
debug_form_func ()
{
    return debug_form;   
}

static CircBuffer s_circ_buffer (6, 256);
CircBuffer* e_circ_buffer = &s_circ_buffer;

CircBuffer::CircBuffer (int n, int size /*= 256 */)
: priv_card (n), priv_size (size), priv_curr (0), priv_buf (new char* [n])
{
    for (int i = 0; i < n; i++)
    {
	priv_buf[i] = new char [size];
	priv_buf[i][0] = '\0';
    }
}

CircBuffer::~CircBuffer ()
{
    for (int i = 0; i < bufferCardinality (); i++)
    {
	delete priv_buf[i];
    }
    delete priv_buf;
}

int
CircBuffer::check ()
{
    if (strlen (curr ()) > bufferSize ())
    {
	fprintf (stderr, "FATAL: CircBuffer overflow - this=%p addr=%p strlen=%d size=%d",
		 this, curr (), strlen (curr ()), bufferSize ());
	if (this == e_circ_buffer)
	{
	    fprintf (stderr, "FATAL (cont.): check your form() calls!\n");
	}
	
	return 0;
    }
    return 1;
}

//#define TEST_FORM
#ifdef TEST_FORM

int
main ()
{
    printf (form ("%s %d %lG\n", "asd", 10, double (3.14e-12)));
    printf (form ("%s %s %s\n", form ("%d %c %f", 10, 'a', float (3)),
		  form ("%lG %s %ld", double (3.405e-12), "abcdef", long(200)),
		  form ("%-*s %d", 10, "ab", 34)));
    return 0;
}

#endif
