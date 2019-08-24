#ifndef _STRINGLIST_H_INCLUDED
#define _STRINGLIST_H_INCLUDED

#include "C++List.h"
#include <String.h>

#ifdef CC30

  typedef List<String> StringList;
  typedef Listiter<String> StringListIter;

#else

  Listdeclare(String)
#define StringList CCList(String)
// can not derive from:  typedef CCList(String) StringList;
  typedef Listiter(String) StringListIter;

#endif /* CC30 */

#endif /* _STRINGLIST_H_INCLUDED */
