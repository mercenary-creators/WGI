#ifndef Listdeclare

#include <generic.h>
#include <iostream.h>
#include <new.h>
#include <Objection.h>
#include <Pool.h>

#ifndef TRUE
#define FALSE	0
#define TRUE	(!FALSE)
#endif

typedef void*	voidP;
class voidP_List;
class Lizt;

struct lnk {
    lnk*     nxt;
    lnk*     prv;
    void     init(lnk* p, lnk* s) { prv = p; nxt = s; }
             lnk() {}
    virtual  ~lnk() {}
    virtual  lnk* copy();
    virtual  int operator==(lnk&);
};

class Liztiter {
private:
    friend Lizt;
           Liztiter(Lizt* lp) : theLizt(lp), cache(0), nextIt(0) { }
protected:
    Lizt* theLizt;		// associated List
    Liztiter* nextIt;		// next on chain
    lnk*  cache;		// a recently retrieved link
    int   cacheNo;		// its index or garbage if cache == NULL
    int   index;		// current position
    lnk*  pred;			// current position
    lnk*  getAt(int);
    lnk*  next();
    lnk*  prev();
    lnk*  peek_next() const	{ return at_end() ? (lnk*)NULL : pred->nxt; }
    lnk*  peek_prev() const	{ return at_head() ? (lnk*)NULL : pred; }
    void  insert_prev(lnk*);
    void  insert_next(lnk*);
    lnk*  remove_prev();
    lnk*  remove_next();
    void  reset0();
public:
    int operator==( const Liztiter& l) {	// this should be protected
        return theLizt == l.theLizt && index == l.index; }
    Liztiter&  operator=(Liztiter& l);
        Liztiter(Lizt&);
        Liztiter(Liztiter&);
        ~Liztiter();
    int position() const	{ return index; }
    void reset(unsigned i=0);
    void end_reset(unsigned i=0);
    int at_end() const;
    int at_head() const	{ return index == 0 ; }
};

class Lizt {
protected:
    friend Liztiter;
    lnk* t;			// tail
    int  sz;			// number of elements
    Liztiter myit;		// current position pointer
    void delete_all();		// used by dtor and operator=()
    void init_all(const Lizt&);	// used by ctor and operator=()
    void add_a_link(lnk*);	// used by put() and unget()
    lnk*  tail() const	{ return t; }
    lnk*  head() const	{ return t ? t->nxt : 0; }
          Lizt() : sz(0), t(0), myit(this)	{ myit.reset0(); }
          Lizt(const Lizt& x) : myit(this)		{ init_all(x); }
          Lizt(const Lizt& a0, const Lizt& a1) : myit(this)	{
             init_all(a0); put(a1); myit.reset0(); 
          }
          ~Lizt();
    	  operator void*()	{ return sz ? this : 0; }
    Lizt& unget(lnk*);
    Lizt& unget(const Lizt&);
    Lizt& put(lnk*);
    Lizt& put(const Lizt&);
    lnk*  get();
    lnk*  unput();
    lnk*  getAt(int i)		{ return myit.getAt(i); }
    lnk*  next()		{ return myit.next(); }
    lnk*  prev()		{ return myit.prev(); }
    lnk*  peek_next() const	{ return myit.peek_next(); }
    lnk*  peek_prev() const	{ return myit.peek_prev(); }
    void  insert_prev(lnk* l)	{ myit.insert_prev(l); }
    void  insert_next(lnk* l)	{ myit.insert_next(l); }
    lnk*  remove_prev()		{ return myit.remove_prev(); }
    lnk*  remove_next()		{ return myit.remove_next(); }
public:
    int   operator==(const Lizt&);
    Lizt& operator=(const Lizt&);
    int   operator!=(const Lizt& x)	{ return !(*this == x); }
    int	  length() const	{ return sz; }
    int	  position() const	{ return myit.index; }
    void  reset(unsigned i=0)	{ myit.reset(i); }
    void  end_reset(unsigned i=0) { myit.end_reset(i); }
    int   at_end() const	{return myit.at_end(); }
    int   at_head() const	{ return myit.at_head(); }
};

/*  
   MACROS: 
            __PDEC(T)  - Declares Pools used for lnnk memory
            __PDEF(T)  - Defines Pools used for lnnk memory
                         List_init is used for static initialization
           __OBJ(S,T)  - Objection raising line
     #define name(..)  - makes names intelligible to compiler
       Listdeclare(T)  - declares the List(T) and Listiter(T)
     Listimplement(T)  - defines the List(T) and Listiter(T)
  List_of_pdeclare(T)  - declares and defines List_of_p(T) and
                         List_of_piter(T)
*/

#define __PDEC(T)							\
extern Pool* Lpoolp(T);							\
class List_Init(T) {							\
    static init_count;							\
public:									\
    List_Init(T)();							\
    ~List_Init(T)();							\
};									\
static List_Init(T) List_init(T);

#define __PDEF(T)							\
Pool* Lpoolp(T);							\
List_Init(T)::List_Init(T)()						\
{									\
    if(++init_count > 1) return;					\
    Lpoolp(T) = new Pool(sizeof(lnnk(T)));				\
}									\
List_Init(T)::~List_Init(T)()						\
{									\
    if(--init_count > 0) return;					\
    delete Lpoolp(T);							\
}

#define __OBJ(S,T) {CCList(T)::domain_error.raise(S);genericerror(0,S);}

#define lnnk(T) name2(T,lnnk)
#define Lpoolp(T) name2(T,Lpoolp)
#define CCList(T) name2(T,_List)
#define List_Init(T) name2(T,List_Init)
#define List_init(T) name2(T,List_init)
#define Listiter(T) name2(T,_Listiter)
#define List_of_piter(T) name2(T,_Listofpiter)
#define Listsubitem(T) name2(T,Listsubitem)
#define List_of_p(T) name2(T,P_List)
#define pListsubitem(T) name2(T,PListsubitem)
#define CMPF(T) name2(T,CMPFN)
#define CMPFP(T) name2(T,CMPFNP)

/* Split up macro for sun cpp - gross! */
#define Listdeclare1(T)							\
__PDEC(T)								\
class lnnk(T);								\
class CCList(T);								\
class Listiter(T);							\
class Listsubitem(T);							\
typedef int     (*CMPF(T))(const T&, const T&);				\
ostream& operator<<(ostream&,CCList(T)&);
#define lnnkdeclare(T)							\
class lnnk(T) : public lnk {						\
    friend CCList(T);							\
    friend Listiter(T);							\
    friend Listsubitem(T);						\
    friend void voidP_List_sort_internal(voidP_List&, CMPF(voidP)); 	\
    T      val;								\
    lnk*   copy();							\
    int    operator==(lnk&);						\
    	   lnnk(T)(T& pp) : val(pp) {}					\
    	   ~lnnk(T)() {}						\
public:									\
    void* operator new(size_t) {return Lpoolp(T)->alloc();}		\
    void operator delete(void* l) {Lpoolp(T)->free(l); }		\
};
#define Listdeclare2(T)							\
class CCList(T) : public Lizt {						\
private:								\
        friend Listsubitem(T);						\
    	CCList(T)(const CCList(T)& a0, const CCList(T)& a1) 			\
            : ((Lizt&)a0, (Lizt&)a1) {}					\
    	CCList(T)(const CCList(T)& a0, const T& t);				\
protected:								\
    T*	getAt(int i) { lnnk(T)* ll = (lnnk(T)*)Lizt::getAt(i); 		\
                       if(ll) return &(ll->val); else return (T*)NULL;}
#define Listdeclare25(T)						\
public:									\
    friend void voidP_List_sort_internal(voidP_List&, CMPF(voidP));	\
    static Objection domain_error;					\
    	CCList(T)() {}							\
    	CCList(T)(const CCList(T)& a) : (*(Lizt*)&a) {}			\
    	CCList(T)(const T& x);						\
    	CCList(T)(const T& x, const T& y);				\
    	CCList(T)(const T& x, const T& y, const T& z);			\
    	CCList(T)(const T& x, const T& y, const T& z, const T& w);
#define Listdeclare3(T)							\
    	~CCList(T)() {}							\
    	operator void*() { return Lizt::operator void*(); }		\
    int	operator==(const CCList(T)& x) const {				\
        if((Lizt&)*this==(Lizt&)x) return 1;			 	\
        else return 0; }						\
    int	operator!=(const CCList(T)& x) const {				\
        return (Lizt&)*this != (Lizt&)x; }				\
    CCList(T)  operator+(const CCList(T)& ll) { return CCList(T)(*this, ll); }
#define Listdeclare31(T)						\
    CCList(T)  operator+(const T& t) { return CCList(T)(*this, t); }	\
    CCList(T)& operator=(const CCList(T)& a) {				\
        return (CCList(T)&) (*(Lizt*)this = *(Lizt*)&a); }		\
    CCList(T)& put(const T& x) { 						\
        return (CCList(T)&) Lizt::put(new lnnk(T)((T&)x)); }		\
    CCList(T)& put(const CCList(T)& ll) {					\
        return (CCList(T)&) Lizt::put((Lizt&) ll); }			\
    T   unput() { lnnk(T)* ll = (lnnk(T)*)Lizt::unput();		\
    		 if(!ll) __OBJ("unput: empty List",T)			\
                 T ans = ll->val; delete ll; return ans; }
#define Listdeclare33(T)						\
    int unput(T& t) { 							\
        lnnk(T)* ll = (lnnk(T)*)Lizt::unput();				\
    	if(ll) { t = ll->val; delete ll; return 1; }			\
    	else  return 0; }						\
    T   get() { lnnk(T)* ll = (lnnk(T)*)Lizt::get();			\
    		if(!ll) __OBJ("get: empty List",T)			\
                T ans = ll->val; delete ll; return ans; }		\
    int get(T& t) { lnnk(T)* ll = (lnnk(T)*)Lizt::get();		\
                    if(ll) return ((t = ll->val), delete ll, 1);	\
                    else return 0; }					\
    CCList(T)&  unget(const T& x) { return (CCList(T)&) 			\
                    Lizt::unget(new lnnk(T)((T&)x)); }			\
    CCList(T)&  unget(const CCList(T)& ll) { return (CCList(T)&)		\
                    Lizt::unget((Lizt&) ll); }				\
    T   head() const; 							\
    T   tail() const;
#define Listdeclare35(T)						\
    int find_next(const T& t)	{ 					\
        return ((Listiter(T)*)(&myit))->find_next(t);}			\
    int find_prev(const T& t)	{ 					\
        return ((Listiter(T)*)(&myit))->find_prev(t);}			\
    int next(T& t) { return ((Listiter(T)*)(&myit))->next(t); }		\
    int next(T*& t)	{ return ((Listiter(T)*)(&myit))->next(t); }	\
    T   next()		{ return ((Listiter(T)*)(&myit))->next(); }	\
    int prev(T& t) { return ((Listiter(T)*)(&myit))->prev(t); }		\
    int prev(T*& t)	{ return ((Listiter(T)*)(&myit))->prev(t); }	\
    T   prev()		{ return ((Listiter(T)*)(&myit))->prev(); }
#define Listdeclare4(T)							\
    int step_next() { return Lizt::next() != 0; }			\
    int step_prev() { return Lizt::prev() != 0; }			\
    int peek_next(T& t) const { 					\
        return ((Listiter(T)*)(&myit))->peek_next(t); }			\
    int peek_next(T*& t) { 					\
        return ((Listiter(T)*)(&myit))->peek_next(t); }			\
    T   peek_next() const {						\
        return ((Listiter(T)*)(&myit))->peek_next(); }			\
    int peek_prev(T& t) const {						\
        return ((Listiter(T)*)(&myit))->peek_prev(t); }			\
    int	peek_prev(T*& t) {					\
        return ((Listiter(T)*)(&myit))->peek_prev(t); }			\
    T	peek_prev() const {						\
        return ((Listiter(T)*)(&myit))->peek_prev(); }			\
    /* remove_prev -- deletes next from List */				\
    int remove_prev() { return ((Listiter(T)*)(&myit))->remove_prev(); }\
    int remove_next() { return ((Listiter(T)*)(&myit))->remove_next(); }
#define Listdeclare45(T)						\
    int remove_prev(T& t) {						\
        return ((Listiter(T)*)(&myit))->remove_prev(t); }		\
    int remove_next(T& t) {						\
        return ((Listiter(T)*)(&myit))->remove_next(t); }		\
    /* insert_prev -- put it at the left of the pointer */		\
    void insert_prev(const T& x) {					\
        Lizt::insert_prev(new lnnk(T)((T&)x)); }			\
    void insert_next(const T& x) {					\
        Lizt::insert_next(new lnnk(T)((T&)x)); }			\
    /* replace_prev -- remove_prev and insert_prev */			\
    int replace_prev(const T& x) {					\
        if(at_head()) return FALSE;					\
        else return (((lnnk(T)*)Lizt::peek_prev())->val=(T&)x, TRUE); }	\
    int replace_next(const T& x) {					\
        if(at_end()) return FALSE;					\
        else return (((lnnk(T)*)Lizt::peek_next())->val=(T&)x, TRUE); }	\
    inline Listsubitem(T) operator[](unsigned ii);			\
    void    sort(CMPF(T));						\
};
#define __SUB(T)							\
class Listsubitem(T)  {							\
    friend CCList(T);							\
    T* p;								\
protected:								\
    Listsubitem(T)(T* pp) { p = pp;}					\
public:									\
    T& operator=(const T& x) { if(p) *p = (T&)x; return *p; }		\
       operator T()    { if(!p) __OBJ("[]: out of bounds",T)		\
			 return *p; }					\
       operator T&()    { if(!p) __OBJ("[]: out of bounds",T)		\
			 return *p; }					\
};									\
inline Listsubitem(T)							\
CCList(T)::operator[](unsigned ii)					\
{									\
    return Listsubitem(T)(getAt(ii));					\
}

#define Listiterdec1(T)							\
class Listiter(T) : public Liztiter {					\
protected:								\
    T*	getAt(int i) { lnnk(T)* ll = ((lnnk(T)*)Liztiter::getAt(i));	\
                       if(ll) return &(ll->val); else return (T*)NULL;}	\
public:									\
    	Listiter(T)(CCList(T)& a) : (*(Lizt*)&a) {}			\
    	Listiter(T)(Listiter(T)& a) : (*(Liztiter*)&a) {}		\
    	~Listiter(T)() {}						\
    int operator==(const Listiter(T)& l) const {			\
        if((Liztiter&)*this==(Liztiter&)l) return 1;			\
        else return 0; }                                                \
    Listiter(T)& operator=(Listiter(T)& l) {				\
        return (Listiter(T)&) ((Liztiter&)*this=(Liztiter&)l); }	\
    int find_next(const T&);						\
    int find_prev(const T&);						\
    int next(T& t) {							\
        if(at_end()) return FALSE;					\
    	else return (t = ((lnnk(T)*)Liztiter::next())->val, TRUE); }	\
    int next(T*& t) {							\
        if(at_end()) return FALSE;					\
    	else return (t = &((lnnk(T)*)Liztiter::next())->val, TRUE);}	\
    T   next();								\
    int prev(T& t) {							\
        if(at_head()) return FALSE;					\
        else return (t=((lnnk(T)*)Liztiter::prev())->val, TRUE); }	\
    int prev(T*& t) {							\
        if(at_head()) return FALSE;					\
        else return (t= &((lnnk(T)*)Liztiter::prev())->val, TRUE); }	\
    T   prev();
#define Listiterdec2(T)							\
    int step_next() { return Liztiter::next() != 0; }			\
    int step_prev() { return Liztiter::prev() != 0; }			\
    int peek_next(T&) const;						\
    int peek_next(T*&) ;						\
    T   peek_next() const;						\
    int peek_prev(T&) const;						\
    int	peek_prev(T*&) ;						\
    T	peek_prev() const;						\
    /* remove_prev -- deletes next from List */				\
    int remove_prev();							\
    int remove_next();							\
    int remove_prev(T&);						\
    int remove_next(T&);						\
    void insert_prev(const T& x) {					\
        Liztiter::insert_prev(new lnnk(T)((T&)x)); }			\
    void insert_next(const T& x) {					\
        Liztiter::insert_next(new lnnk(T)((T&)x)); }			\
    int replace_prev(const T& x) {					\
        if(at_head()) return FALSE;					\
        else 								\
            return (((lnnk(T)*)Liztiter::peek_prev())->val=(T&)x,TRUE);}\
    int replace_next(const T& x) {					\
        if(at_end()) return FALSE;					\
        else 								\
            return (((lnnk(T)*)Liztiter::peek_next())->val=(T&)x,TRUE);}\
};

#define Listdeclare(T)							\
Listdeclare1(T)								\
lnnkdeclare(T)								\
Listiterdec1(T)								\
Listiterdec2(T)								\
Listdeclare2(T)								\
Listdeclare25(T)							\
Listdeclare3(T)								\
Listdeclare31(T)							\
Listdeclare33(T)							\
Listdeclare35(T)							\
Listdeclare4(T)								\
Listdeclare45(T)							\
__SUB(T)

Listdeclare(voidP)

/* split up macro for sun cpp */
#define List_of_pdeclare1(T)						\
typedef int     (*CMPFP(T))(const T*&, const T*&);			\
ostream& operator<<(ostream&,List_of_p(T)&);				\
									\
class List_of_p(T) : public CCList(voidP) {				\
public:									\
        List_of_p(T)() {}						\
        List_of_p(T)(const T* x) : ((voidP) x) {}			\
        List_of_p(T)(const T* x, const T* y) : 				\
            ((voidP) x, (voidP) y) {}					\
        List_of_p(T)(const T* x, const T* y, const T* z) :		\
            ((voidP)x, (voidP)y, (voidP)z) {}				\
        List_of_p(T)(const T* x, const T* y, const T* z, const T* w) :	\
            ((voidP) x, (voidP) y, (voidP) z, (voidP) w) {}		\
        List_of_p(T)(const List_of_p(T)& ll) : ((voidP_List&) ll) {}	\
        ~List_of_p(T)() {}						\
        operator void*() { return voidP_List::operator void*(); }	\
    int	operator==(const List_of_p(T)& ll) const { return		\
			    ((voidP_List&)*this == (voidP_List&)ll); }	\
    int	operator!=(const List_of_p(T)& ll) const { return		\
			     ((voidP_List&)*this != (voidP_List&)ll); }	\
    List_of_p(T)& operator=(const List_of_p(T)& ll) {			\
        return (List_of_p(T)&) ((voidP_List&)*this = (voidP_List&)ll); }\
    List_of_p(T)  operator+(const List_of_p(T)& ll) { 			\
        return (List_of_p(T)&) ((voidP_List&)*this + (voidP_List&)ll); }\
    List_of_p(T)  operator+(const T* t) { 				\
        return (List_of_p(T)&) ((voidP_List&)*this + (voidP)t); }	\
    List_of_p(T)& put(const T *t) { 					\
        return (List_of_p(T)&) voidP_List::put((voidP)t);}		\
    List_of_p(T)& put(const List_of_p(T)& ll) {				\
        return (List_of_p(T)&) voidP_List::put((voidP_List&)ll); }
#define List_of_pdeclare2(T)						\
    int	unput(T *&t) { return voidP_List::unput((voidP&)t); }		\
    T*  unput() { return (T*)voidP_List::unput(); }			\
    int get(T *&t) { return voidP_List::get((voidP&)t); }		\
    T*  get() { return (T*)voidP_List::get(); }				\
    List_of_p(T)&  unget(const T *x) {					\
        return (List_of_p(T)&) voidP_List::unget((voidP)x); }		\
    List_of_p(T)&  unget(const List_of_p(T)& ll) {			\
        return (List_of_p(T)&) voidP_List::unget((voidP_List&)ll); }	\
    inline pListsubitem(T) operator[](unsigned ii);			\
    T*  head() const { return (T*)voidP_List::head(); }			\
    T*  tail() const { return (T*)voidP_List::tail(); }			\
    int find_next(const T*& t) {					\
        return voidP_List::find_next((voidP&)t); }			\
    int find_prev(const T*& t) {					\
        return voidP_List::find_prev((voidP&)t); }			\
    int next(T *&t) { return voidP_List::next((voidP&)t); }		\
    int next(T **&t) { return voidP_List::next((voidP*&)t); }		\
    T*  next() { return (T*)voidP_List::next(); }			\
    int prev(T *&t) { return voidP_List::prev((voidP&)t); }		\
    int prev(T **&t) { return voidP_List::prev((voidP*&)t); }		\
    T*  prev() { return (T*)voidP_List::prev(); }			\
    int peek_next(T *&t) const { 					\
	return voidP_List::peek_next((voidP&)t); }			\
    int peek_next(T **&t) { 					\
	return voidP_List::peek_next((voidP*&)t); }			\
    T*  peek_next() const { 						\
	return (T*)voidP_List::peek_next(); }				\
    int peek_prev(T *&t) const { 					\
	return voidP_List::peek_prev((voidP&)t); }			\
    int peek_prev(T **&t) { 					\
	return voidP_List::peek_prev((voidP*&)t); }			\
    T*  peek_prev() const { 						\
	return (T*)voidP_List::peek_prev(); }				\
    int remove_prev() { return voidP_List::remove_prev(); }		\
    int remove_next() { return voidP_List::remove_next(); }		\
    int remove_prev(T *&x) { return voidP_List::remove_prev((voidP&)x);}\
    int remove_next(T *&x) { return voidP_List::remove_next((voidP&)x);}\
    void insert_prev(const T *&x) {					\
        voidP_List::insert_prev((voidP&)x); }				\
    void insert_next(const T *&x) {					\
        voidP_List::insert_next((voidP&)x); }				\
    int replace_prev(const T *x) {					\
        return voidP_List::replace_prev((voidP)x);}			\
    int replace_next(const T *x) {					\
        return voidP_List::replace_next((voidP)x);}			\
    void  sort(CMPFP(T) pf) { voidP_List::sort((CMPF(voidP))pf); }	\
};
#define __PSUB(T)							\
class pListsubitem(T) : public voidPListsubitem {			\
    pListsubitem(T)(T** p) : ((voidP*)p) {}				\
    friend List_of_p(T);						\
public:									\
    T*& operator=(const T*& t) { return					\
                (T*&) ((voidPListsubitem&)*this = (voidP&)t); }		\
    operator T*() { return (T*) voidPListsubitem::operator voidP(); }	\
};									\
									\
inline									\
pListsubitem(T)								\
List_of_p(T)::operator[](unsigned ii)					\
{									\
    return pListsubitem(T)((T**)getAt(ii));				\
}

#define Listofpiterdec1(T)						\
class List_of_piter(T) : public Listiter(voidP) {			\
public:									\
        List_of_piter(T)(List_of_p(T)& l) : ((voidP_List&) l) {}	\
        List_of_piter(T)(List_of_piter(T)& l) : ((voidP_Listiter&) l){}	\
       ~List_of_piter(T)() {}						\
    int	operator==(const List_of_piter(T)& ll) const {			\
        return ((voidP_Listiter&)*this == (voidP_Listiter&)ll); }	\
    List_of_piter(T)& operator=(List_of_piter(T)& ll) {			\
        return (List_of_piter(T)&)					\
               ((voidP_Listiter&)*this = (voidP_Listiter&)ll); }	\
    int find_next(T*& t) { return voidP_Listiter::find_next((voidP&)t);}\
    int find_prev(T*& t) { return voidP_Listiter::find_prev((voidP&)t);}\
    int next(T *&t)      { return voidP_Listiter::next((voidP&)t); }	\
    int next(T **&t)     { return voidP_Listiter::next((voidP*&)t); }	\
    T*  next()           { return (T*)voidP_Listiter::next(); }		\
    int prev(T *&t)      { return voidP_Listiter::prev((voidP&)t); }	\
    int prev(T **&t)     { return voidP_Listiter::prev((voidP*&)t); }	\
    T*  prev()           { return (T*)voidP_Listiter::prev(); }		\
    int peek_next(T *&t) const { 					\
	return voidP_Listiter::peek_next((voidP&)t); }			\
    int peek_next(T **&t) { 					\
	return voidP_Listiter::peek_next((voidP*&)t); }			\
    T*  peek_next() const { 						\
	return (T*)voidP_Listiter::peek_next(); }
#define Listofpiterdec2(T)						\
    int peek_prev(T *&t) const { 					\
	return voidP_Listiter::peek_prev((voidP&)t); }			\
    int peek_prev(T **&t) { 					\
	return voidP_Listiter::peek_prev((voidP*&)t); }			\
    T*  peek_prev() const { 						\
	return (T*)voidP_Listiter::peek_prev(); }			\
    int remove_prev() { return voidP_Listiter::remove_prev(); }		\
    int remove_next() { return voidP_Listiter::remove_next(); }		\
    int remove_prev(T *&x) {						\
        return voidP_Listiter::remove_prev((voidP&)x);}			\
    int remove_next(T *&x) {						\
        return voidP_Listiter::remove_next((voidP&)x);}			\
    void insert_prev(T *&x) { voidP_Listiter::insert_prev((voidP&)x); }	\
    void insert_next(T *&x) { voidP_Listiter::insert_next((voidP&)x); }	\
    int replace_prev(T *x) {						\
        return voidP_Listiter::replace_prev((voidP)x);}			\
    int replace_next(T *x) {						\
        return voidP_Listiter::replace_next((voidP)x);}			\
};

#define List_of_pdeclare(T)						\
class List_of_p(T);							\
class pListsubitem(T);							\
class List_of_piter(T);							\
List_of_pdeclare1(T)							\
List_of_pdeclare2(T)							\
__PSUB(T)								\
Listofpiterdec1(T)							\
Listofpiterdec2(T)

#define Listimplement1(T)						\
int L_error(const char*);						\
Objection CCList(T)::domain_error(L_error);				\
__PDEF(T)								\
									\
lnk*									\
lnnk(T)::copy()								\
{									\
    return new lnnk(T)((T&)val);					\
}									\
int									\
lnnk(T)::operator==(lnk& x)						\
{									\
    return val == ((lnnk(T)*)&x)->val;					\
}									\
CCList(T)::CCList(T)(const CCList(T)& a0, const T& t) : ((Lizt&)a0)		\
{									\
    put(t);								\
}									\
CCList(T)::CCList(T)(const T& t)						\
{									\
    put(t);								\
}									\
CCList(T)::CCList(T)(const T& t, const T& u)				\
{									\
    put(t);								\
    put(u);								\
}									\
CCList(T)::CCList(T)(const T& t, const T& u, const T& v)			\
{									\
    put(t);								\
    put(u);								\
    put(v);								\
}									\
CCList(T)::CCList(T)(const T& t, const T& u, const T& v, const T& w)	\
{									\
    put(t);								\
    put(u);								\
    put(v);								\
    put(w);								\
}									\
T									\
CCList(T)::head() const							\
{									\
    lnnk(T)* ll = (lnnk(T)*)Lizt::head();				\
    if(!ll) __OBJ("head: empty List",T)					\
    return ll->val;							\
}									\
T									\
CCList(T)::tail() const							\
{									\
    lnnk(T)* ll = (lnnk(T)*)Lizt::tail();				\
    if(!ll) __OBJ("tail: empty List",T)					\
    return ll->val;							\
}

#define Listiterimp(T)							\
T									\
Listiter(T)::next()							\
{									\
    lnnk(T)* ll = (lnnk(T)*)Liztiter::next();				\
    if(!ll) __OBJ("next: at end of List",T)				\
    return ll->val;							\
}									\
T									\
Listiter(T)::prev() 							\
{ 									\
    lnnk(T)* ll = (lnnk(T)*)Liztiter::prev();				\
    if(!ll) __OBJ("prev: at head of List",T)				\
    return ll->val;							\
}									\
int									\
Listiter(T)::find_prev(const T& x)					\
{									\
    if(at_head()|| theLizt->length()==0) return FALSE;			\
    lnnk(T)* iter = (lnnk(T)*) pred->nxt;				\
    register int i = index;						\
    do {								\
        iter = (lnnk(T)*) iter->prv;					\
        if (iter->val==(T&)x) {						\
	    index = i;							\
	    pred = iter;						\
	    return TRUE;						\
	}								\
	i--;								\
    } while (i > 0);							\
    return FALSE;							\
}									\
int									\
Listiter(T)::find_next(const T& x)					\
{									\
    if(at_end()||theLizt->length()==0) return FALSE;			\
    lnnk(T)* iter = (lnnk(T)*) pred;					\
    register int i = index;						\
    do {								\
        iter = (lnnk(T)*) iter->nxt;					\
        if (iter->val==(T&)x) {						\
	    index = i;							\
	    pred = iter->prv;						\
	    return TRUE;						\
	}								\
	i++;								\
    } while (i < theLizt->length());					\
    return FALSE;							\
}									\
int									\
Listiter(T)::remove_prev()						\
{									\
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_prev();		\
    if(aLink) return (delete aLink, TRUE);				\
    else return FALSE;							\
}									\
int									\
Listiter(T)::remove_prev(T &x)						\
{									\
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_prev();		\
    if(aLink) return (x = aLink->val,delete aLink, TRUE);		\
    else return FALSE;							\
}									\
int									\
Listiter(T)::remove_next()						\
{									\
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_next();		\
    if(aLink) return (delete aLink, TRUE);                              \
    else return FALSE;                                                  \
}									\
int									\
Listiter(T)::remove_next(T &x)						\
{									\
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_next();		\
    if(aLink) return (x = aLink->val, delete aLink, TRUE);		\
    else return FALSE;							\
}									\
T									\
Listiter(T)::peek_prev() const						\
{									\
    if (at_head()) __OBJ("peek_prev: at head of List",T)		\
    return ((lnnk(T)*)Liztiter::peek_prev())->val;			\
}									\
int									\
Listiter(T)::peek_prev(T& t) const					\
{									\
    if(at_head()) return FALSE;						\
    else return (t = ((lnnk(T)*)Liztiter::peek_prev())->val, TRUE);	\
}									\
int									\
Listiter(T)::peek_prev(T*& t) 						\
{									\
    if(at_head()) return FALSE;                                         \
    else return (t = &((lnnk(T)*)Liztiter::peek_prev())->val, TRUE);	\
}									\
T									\
Listiter(T)::peek_next() const						\
{									\
    if (at_end()) __OBJ("peek_next: at end of List",T)			\
    return ((lnnk(T)*)Liztiter::peek_next())->val;			\
}									\
int									\
Listiter(T)::peek_next(T& t) const					\
{									\
    if(at_end()) return FALSE;						\
    else return (t = ((lnnk(T)*)Liztiter::peek_next())->val, TRUE);	\
}									\
int									\
Listiter(T)::peek_next(T*& t) 						\
{									\
    if(at_end()) return FALSE;						\
    else return (t = &((lnnk(T)*)Liztiter::peek_next())->val, TRUE);	\
}									\
void                                                                    \
CCList(T)::sort(CMPF(T) cmp)						\
{                                                                       \
    if (length() < 2) return;						\
    voidP_List_sort_internal(*(voidP_List*)this, (CMPF(voidP))cmp);	\
}

#define Listimplement(T)						\
Listimplement1(T)							\
Listiterimp(T)

#define Listout(T)							\
ostream&								\
operator<<(ostream& oo, CCList(T)& ll)					\
{									\
    oo<<"( ";								\
    Listiter(T) l(ll);							\
    while (!l.at_end()) oo << l.next() << " ";				\
    oo<<")";	                                                        \
    return oo;								\
}
#define List_of_pout(T)							\
ostream&								\
operator<<(ostream& oo, List_of_p(T)& ll)				\
{									\
    oo<<"( ";								\
    List_of_piter(T) l(ll);						\
    while (!l.at_end()) oo << *(l.next()) << " ";			\
    oo<<")";	                                                        \
    return oo;								\
}

#endif
