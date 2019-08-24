#ifndef _FORM_H_INCLUDED
#define _FORM_H_INCLUDED

#ifdef NAME
form, fform, CircBuffer - form() with a larger and circular buffer
#endif

#ifdef DESCRIPTION
The form macro replaces the form function from stream.h with different
buffer scheme. The original form uses a static character buffer of 128
characters, which is far too small! You also can not use the result
of a form call in another form call.

The new mechanism uses the CircBuffer class which provides a means to
use up to N form calls (N=6 if the form macro is used) before the same 
character buffer is used. If the number of buffers (6) or
the buffer size (256) is too small, create your own CircBuffer object
and use the fform() function.

The DebugFlag "form" can be used to check for unexpected buffer overflows.
If the flag is set, it will check the string length that was printed by 
(f)form with the allocated buffer size and print an error if it overflowed.
The (fatal) errors are memory leaks and will cause strange things to happen.
#endif

#ifdef EXAMPLE

#include "Form.h"

void f1 ()
{
    printf (form ("%s %d %G\n", "asd", 10, double (3.14e-12)));

    // Yippie! Nested form calls!
    printf (form ("%s %s %s\n", form ("%d %c %f", 10, 'a', float (3)),
                  form ("%G %s %ld", double (3.405e-12), "abcdef", long(200)),
                  form ("%-*s %d", 10, "ab", 34)));
}

// The following example explains how to use fform(), not why...
void f2 ()
{
    CircBuffer tmp (10, 10);
#define p(i) fform(tmp, "%4d", i)
    printf (form ("%s %s %s %s %s %s %s %s %s %s", p(1), p(2), p(3),
		  p(4), p(5), p(6), p(7), p(8), p(9), p(10)));
#undef p
}

#endif

class CircBuffer
{
 private:

  int priv_size;
  int priv_card;
  char** priv_buf;
  int priv_curr;

  CircBuffer (const CircBuffer& other);
  void operator= (const CircBuffer& other);
	// The copy constructor and assignment operator are private and 
	//  unimplemented to prevent users from copying CircBuffers.
	// Instead they should create their own!

 public:

  CircBuffer (int n, int size = 256);
	// Creates n character buffers of the specified size.
	// The next() call can be used to cycle thru the buffers.
  ~CircBuffer ();

  char* next ()
  { return priv_buf[priv_curr = (priv_curr + 1) % bufferCardinality ()]; }
	// Makes the next buffer the current buffer and
	//  returns a pointer to it.
  char* curr () { return priv_buf[priv_curr]; }
	// Returns a pointer to the current buffer.
  int check ();
	// Checks if the string that was printed in the current buffer
	//  is less than the bufferSize().
	// If so, it returns FALSE (0) and prints an error message to stderr.

  int bufferCardinality () const { return priv_card; }
	// Returns the number of buffers.
  int bufferSize () const { return priv_size; }
	// Returns the size of each buffer.
};

extern CircBuffer* e_circ_buffer;
	// Points to a static CircBuffer(6, 256) which is used by the
	//  form() macros.

#define form CircBuffer_form

extern char* CircBuffer_form (const char* format ...);
	// Replaces the form() function with a circular buffer scheme.
extern char* fform (CircBuffer& cb, const char* format ...);
	// Similar to form (or CircBuffer_form) except you can use your
	// own CircBuffer instead of the default e_circ_buffer.

#endif /* _FORM_H_INCLUDED */
