// use C++ reusable components List class, versus ONTOS List class
//#include "/usr/add-on/DFM/src/cmd/C++/2.0/sun4/include/List.h"
#ifdef CC21

#ifdef sun
#include "CCList21.h"
#else

#ifdef hppa
#include "CCList21hp.h"
#endif /* hppa */

#endif /* sun */

#else
#ifdef CC30

#define CCList(T) List<T>
#ifdef SC201
#include "RList.h"
#else
#include "List.h"
#endif /* SC201 */

#else
#include "CCList20.h"
#endif /* CC30 */
#endif /* CC21 */
