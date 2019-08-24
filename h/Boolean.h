#ifndef _BOOLEAN_H_INCLUDED
#define _BOOLEAN_H_INCLUDED

typedef int Boolean;

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

inline Boolean not(Boolean b)
{ return !b; }

#endif /* _BOOLEAN_H_INCLUDED */
