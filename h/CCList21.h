#ifndef LISTH
#define LISTH

/*
 *   List.h  --  parameterized doubly-linked lists
 *
 * user-callable "macros":
 *  Listdeclare(T) -- declaration of classes CCList(T) and Listiter(T)
 *  List_of_pdeclare(T) -- declaration of classes List_of_p(T)
 *	and List_of_piter(T)
 *  List_generatordeclare(T) -- definition of inline functions
 *	CCList(T)::generator() and CCList(T)::new_generator()
 *  List_of_p_generatordeclare(T) -- definition of inline functions
 *	List_of_p(T)::generator() and List_of_p(T)::new_generator()
 *  Listimplement(T) -- implementation of functions in CCList(T)
 *	and Listiter(T)
 *  List_of_pimplement(T) -- implementation of functions in List_of_p(T)
 *	and List_of_piter(T)
 *  Listout(T) -- implementation of operator<<(ostream&, CCList(T)&)
 *  List_of_pout(T) -- implementation of operator<<(ostream&, List_of_p(T)&)
 */
#ifndef NULL
#define NULL 0
#endif
#include <new.h>
#include <Objection.h>
#include <Pool.h>
#include "Func_obj.h"

extern genericerror(int,char*);
class ostream;

#ifndef TRUE
#define FALSE	0
#define TRUE	(!FALSE)
#endif

typedef void*	voidP;
class voidP_List;
class Lizt;
class List_select_dummy;
class List_reject_dummy;
class List_map_dummy;

/* struct lnk implements two-way pointers */
struct lnk {
    lnk*     nxt;
    lnk*     prv;
    void     init(lnk* p, lnk* s) { prv = p; nxt = s; }
             lnk() {}
    virtual  ~lnk() {}
    virtual  lnk* copy();
    virtual  int operator==(lnk&);
};

/* class Liztiter is the base class for all Listiter(T)'s */
class Liztiter {
private:
    friend Lizt;
           Liztiter(Lizt* lp) : theLizt(lp), cache(0), nextIt(0) { }
public:
    int at_head() const	{ return index == 0 ; }
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
};

/* class Lizt is the base class for all CCList(T)'s */
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
          Lizt();
          Lizt(const Lizt& x) : myit(this)		{ init_all(x); }
          Lizt(const Lizt&, const Lizt&);
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
    void  reset_all_iters();
};

/* The rest of this file contains definitions of "parameterized types" */

/*  
   MACROS: 
            Pooldcl_ATTLC(T)  - Declares Pools used for lnnk memory
            Pooldef_ATTLC(T)  - Defines Pools used for lnnk memory
                         List_init is used for static initialization
            OBJ__ATTLC(S,T)  - Objection raising line
       Listdeclare(T)  - declares the CCList(T) and Listiter(T)
     Listimplement(T)  - defines the CCList(T) and Listiter(T)
  List_of_pdeclare(T)  - declares and defines List_of_p(T) and
                         List_of_piter(T)
*/

PT_define Pooldcl_ATTLC(T)
extern Pool* Lpoolp(T);
class List_Init(T) {
    static init_count;
public:
    List_Init(T)();
    ~List_Init(T)();
};
static List_Init(T) List_init(T);
PT_end

PT_define Pooldef_ATTLC(T)
Pool* Lpoolp(T);
List_Init(T)::List_Init(T)()
{
    if(++init_count > 1) return;
    Lpoolp(T) = new Pool(sizeof(lnnk(T)));
}
List_Init(T)::~List_Init(T)()
{
    if(--init_count > 0) return;
    delete Lpoolp(T);
}
PT_end

#define OBJ__ATTLC(S,T) {CCList(T)::domain_error.raise(S);genericerror(0,S);}
#define OBJP__ATTLC(S,T) {List_of_p(T)::domain_error.raise(S);genericerror(0,S);}

PT_names
	lnnk(T) 	T lnnk
	getnewlnnk_ATTLC(T)	getnewlnnk_ATTLC_ T
	deletelnnk_ATTLC(T)	deletelnnk_ATTLC_ T
	Lpoolp(T) 	T Lpoolp
	CCList(T) 	T _List
	List_Init(T) 	T List_Init
	List_init(T) 	T List_init
	Listiter(T) 	T _Listiter
	List_generator(T) 	T _List_generator
	List_of_piter(T) 	T _Listofpiter
	List_of_p_generator(T) 	T _Listofp_generator
	List_of_p(T) 	T P_List
	CMPF(T) 	T CMPFN
	CMPFP(T) 	T CMPFNP
PT_end


/* Listdeclare(T) is the "macro" that the users will call when they want
   all of the declarations for the classes CCList(T) and Listiter(T). */
PT_define Listdeclare(T)
Listdeclare1_ATTLC(T)
lnnkdeclare_ATTLC(T)
Listiterdeclare_ATTLC(T)
Listdeclare2_ATTLC(T)
PT_end

PT_define Listdeclare1_ATTLC(T)
Pooldcl_ATTLC(T)
class lnnk(T);
class CCList(T);
class Listiter(T);
class List_generator(T);
typedef int (*CMPF(T))(const T&, const T&);
ostream& operator<<(ostream&,CCList(T)&);
PT_end


PT_define lnnkdeclare_ATTLC(T)
class lnnk(T) : public lnk {
    friend CCList(T);
    friend Listiter(T);
    friend void voidP_List_sort_internal(voidP_List&, CMPF(voidP));
    T      val;
    lnk*   copy();
    int    operator==(lnk&);
    	   inline lnnk(T)(T&);
    	   inline ~lnnk(T)();
public:
    inline void* operator new(size_t);
    inline void operator delete(void*);
    friend lnk* getnewlnnk_ATTLC(T)(const T&);
    friend void deletelnnk_ATTLC(T)(T&,  lnnk(T)*);
};
void* lnnk(T)::operator new(size_t) {return Lpoolp(T)->alloc();}
void lnnk(T)::operator delete(void* l) {Lpoolp(T)->free(l); }
lnnk(T)::lnnk(T)(T& pp) : val(pp) {}
lnnk(T)::~lnnk(T)() {}
PT_end

PT_define Listdeclare2_ATTLC(T)
/* the following declarations of "Function object" classes are the
   absolute minimum required for compiling the function prototypes
   included in the definition of class CCList(T). */
Function_minideclare(T)
class Pred_obj(T);
class Tran_obj(T);
class Gene_obj(T);
class Proc_obj(T);

class CCList(T) : public Lizt {
private:
	void	make_select(Predicate(T)&, const CCList(T)&);
	void	make_select(Pred_obj(T)&, const CCList(T)&);
	void	make_reject(Predicate(T)&, const CCList(T)&);
	void	make_reject(Pred_obj(T)&, const CCList(T)&);
	void	make_map(Transformer(T)&, const CCList(T)&);
	void	make_map(Tran_obj(T)&, const CCList(T)&);
	CCList(T)(Predicate(T)& f, const CCList(T)& ps,
			List_select_dummy*) { make_select(f, ps); }
	CCList(T)(Pred_obj(T)& f, const CCList(T)& ps,
			List_select_dummy*) { make_select(f, ps); }
	CCList(T)(Predicate(T)& f, const CCList(T)& ps,
			List_reject_dummy*) { make_reject(f, ps); }
	CCList(T)(Pred_obj(T)& f, const CCList(T)& ps,
			List_reject_dummy*) { make_reject(f, ps); }
	CCList(T)(Transformer(T)& f, const CCList(T)& ps,
			List_map_dummy*) { make_map(f, ps); }
	CCList(T)(Tran_obj(T)& f, const CCList(T)& ps,
			List_map_dummy*) { make_map(f, ps); }
    	CCList(T)(const CCList(T)& a0, const CCList(T)& a1)
            : Lizt((Lizt&)a0, (Lizt&)a1) {}
    	CCList(T)(const CCList(T)& a0, const T& t);
protected:
    T*	getAt(int i) { lnnk(T)* ll = (lnnk(T)*)Lizt::getAt(i);
                       if(ll) return &(ll->val); else return (T*)NULL;}
public:
    friend void voidP_List_sort_internal(voidP_List&, CMPF(voidP));
    static Objection domain_error;
    	CCList(T)() {}
    	CCList(T)(const CCList(T)&);
    	CCList(T)(const T& x);
    	CCList(T)(const T& x, const T& y);
    	CCList(T)(const T& x, const T& y, const T& z);
    	CCList(T)(const T& x, const T& y, const T& z, const T& w);
	CCList(T)(Gene_obj(T)&);
    	~CCList(T)() {}
    	operator void*() { return Lizt::operator void*(); }
    int	operator==(const CCList(T)& x) const {
        if((Lizt&)*this==(Lizt&)x) return 1;
        else return 0; }
    int	operator!=(const CCList(T)& x) const {
        return (Lizt&)*this != (Lizt&)x; }
    CCList(T)  operator+(const CCList(T)& ll) { return CCList(T)(*this, ll); }
    CCList(T)  operator+(const T& t) { return CCList(T)(*this, t); }
    CCList(T)& operator=(const CCList(T)& a) {
        return (CCList(T)&) (*(Lizt*)this = *(Lizt*)&a); }
    CCList(T)& put(const T& x) {
        return (CCList(T)&) Lizt::put(getnewlnnk_ATTLC(T)(x)); }
    CCList(T)& put(const CCList(T)& ll) {
        return (CCList(T)&) Lizt::put((Lizt&) ll); }
    T   unput();
    int unput(T& t) {
        lnnk(T)* ll = (lnnk(T)*)Lizt::unput();
	if(ll) { deletelnnk_ATTLC(T)(t, ll); return 1; }
    	else  return 0; }
    T   get();
    int get(T& t) { lnnk(T)* ll = (lnnk(T)*)Lizt::get();
                    if(ll) { deletelnnk_ATTLC(T)(t, ll); return 1; }
                    else return 0; }
    CCList(T)&  unget(const T& x) { return (CCList(T)&)
                    Lizt::unget(getnewlnnk_ATTLC(T)(x)); }
    CCList(T)&  unget(const CCList(T)& ll) { return (CCList(T)&)
                    Lizt::unget((Lizt&) ll); }
    T   head() const;
    T   tail() const;
    int find_next(const T& t)	{
        return ((Listiter(T)*)(&myit))->find_next(t);}
    int find_prev(const T& t)	{
        return ((Listiter(T)*)(&myit))->find_prev(t);}
    int next(T& t) { return ((Listiter(T)*)(&myit))->next(t); }
    int next(T*& t)	{ return ((Listiter(T)*)(&myit))->next(t); }
    T   next()		{ return ((Listiter(T)*)(&myit))->next(); }
    int prev(T& t) { return ((Listiter(T)*)(&myit))->prev(t); }
    int prev(T*& t)	{ return ((Listiter(T)*)(&myit))->prev(t); }
    T   prev()		{ return ((Listiter(T)*)(&myit))->prev(); }
    int step_next() { return Lizt::next() != 0; }
    int step_prev() { return Lizt::prev() != 0; }
    int peek_next(T& t) const {
        return ((const Listiter(T)*)(&(((CCList(T)*)this)->myit)))->peek_next(t); }
    int peek_next(T*& t) const {
        return ((const Listiter(T)*)(&(((CCList(T)*)this)->myit)))->peek_next(t); }
    T   peek_next() const {
        return ((const Listiter(T)*)(&(((CCList(T)*)this)->myit)))->peek_next(); }
    int peek_prev(T& t) const {
        return ((const Listiter(T)*)(&(((CCList(T)*)this)->myit)))->peek_prev(t); }
    int	peek_prev(T*& t) const {
        return ((const Listiter(T)*)(&(((CCList(T)*)this)->myit)))->peek_prev(t); }
    T	peek_prev() const {
        return ((const Listiter(T)*)(&(((CCList(T)*)this)->myit)))->peek_prev(); }
    /* remove_prev -- deletes next from List */
    int remove_prev() { return ((Listiter(T)*)(&myit))->remove_prev(); }
    int remove_next() { return ((Listiter(T)*)(&myit))->remove_next(); }
    int remove_prev(T& t) {
        return ((Listiter(T)*)(&myit))->remove_prev(t); }
    int remove_next(T& t) {
        return ((Listiter(T)*)(&myit))->remove_next(t); }
    /* insert_prev -- put it at the left of the pointer */
    void insert_prev(const T& x) {
        Lizt::insert_prev(getnewlnnk_ATTLC(T)(x)); }
    void insert_next(const T& x) {
        Lizt::insert_next(getnewlnnk_ATTLC(T)(x)); }
    /* replace_prev -- remove_prev and insert_prev */
    int replace_prev(const T& x) {
        if(at_head()) return FALSE;
        else return (((lnnk(T)*)Lizt::peek_prev())->val=(T&)x, TRUE); }
    int replace_next(const T& x) {
        if(at_end()) return FALSE;
        else return (((lnnk(T)*)Lizt::peek_next())->val=(T&)x, TRUE); }
    T& operator[](unsigned ii);
    void    sort(CMPF(T));
    inline List_generator(T) generator();
    inline List_generator(T)* new_generator();
    void	do_each(CCProcedure(T)& f) const;
    void	do_each(Proc_obj(T)&) const;
    T*	do_while(Predicate(T)& f) const;
    T*	do_while(Pred_obj(T)&) const;
    T*	do_until(Predicate(T)& f) const;
    T*	do_until(Pred_obj(T)&) const;
    inline CCList(T)	select(Predicate(T)& p) const;
    inline CCList(T)	select(Pred_obj(T)& p) const;
    inline CCList(T)	reject(Predicate(T)& p) const;
    inline CCList(T)	reject(Pred_obj(T)& p) const;
    inline CCList(T)	map(Transformer(T)& f) const;
    inline CCList(T)	map(Tran_obj(T)& f) const;
};
inline CCList(T)
CCList(T)::select(Predicate(T)& p) const
{
	return CCList(T)(p, *this, (List_select_dummy*)0);
}
inline CCList(T)
CCList(T)::select(Pred_obj(T)& p) const
{
	return CCList(T)(p, *this, (List_select_dummy*)0);
}
inline CCList(T)
CCList(T)::reject(Predicate(T)& p) const
{
	return CCList(T)(p, *this, (List_reject_dummy*)0);
}
inline CCList(T)
CCList(T)::reject(Pred_obj(T)& p) const
{
	return CCList(T)(p, *this, (List_reject_dummy*)0);
}
inline CCList(T)
CCList(T)::map(Transformer(T)& f) const
{
	return CCList(T)(f, *this, (List_map_dummy*)0);
}
inline CCList(T)
CCList(T)::map(Tran_obj(T)& f) const
{
	return CCList(T)(f, *this, (List_map_dummy*)0);
}
PT_end


PT_define Listiterdeclare_ATTLC(T)
class Listiter(T) : public Liztiter {
protected:
    T*	getAt(int i) { lnnk(T)* ll = ((lnnk(T)*)Liztiter::getAt(i));
                       if(ll) return &(ll->val); else return (T*)NULL;}
public:
    	Listiter(T)(CCList(T)& a) : Liztiter(*(Lizt*)&a) {}
    	Listiter(T)(Listiter(T)& a) : Liztiter(*(Liztiter*)&a) {}
    	~Listiter(T)() {}
    int operator==(const Listiter(T)& l) const {
        if((Liztiter&)*this==(Liztiter&)l) return 1;
        else return 0; }
    int operator!=(const Listiter(T)& l) const {
        if(!((Liztiter&)(*this)==(Liztiter&)l)) return 1;
        else return 0; }
    Listiter(T)& operator=(Listiter(T)& l) {
        return (Listiter(T)&) ((Liztiter&)*this=(Liztiter&)l); }
    int find_next(const T&);
    int find_prev(const T&);
    int next(T& t) {
        if(at_end()) return FALSE;
    	else return (t = ((lnnk(T)*)Liztiter::next())->val, TRUE); }
    int next(T*& t) {
        if(at_end()) return FALSE;
    	else return (t = &((lnnk(T)*)Liztiter::next())->val, TRUE);}
    T   next();
    int prev(T& t) {
        if(at_head()) return FALSE;
        else return (t=((lnnk(T)*)Liztiter::prev())->val, TRUE); }
    int prev(T*& t) {
        if(at_head()) return FALSE;
        else return (t= &((lnnk(T)*)Liztiter::prev())->val, TRUE); }
    T   prev();
    int step_next() { return Liztiter::next() != 0; }
    int step_prev() { return Liztiter::prev() != 0; }
    int peek_next(T&) const;
    int peek_next(T*&) const;
    T   peek_next() const;
    int peek_prev(T&) const;
    int	peek_prev(T*&) const;
    T	peek_prev() const;
    /* remove_prev -- deletes next from List */
    int remove_prev();
    int remove_next();
    int remove_prev(T&);
    int remove_next(T&);
    void insert_prev(const T& x) {
        Liztiter::insert_prev(getnewlnnk_ATTLC(T)(x)); }
    void insert_next(const T& x) {
        Liztiter::insert_next(getnewlnnk_ATTLC(T)(x)); }
    int replace_prev(const T& x) {
        if(at_head()) return FALSE;
        else
            return (((lnnk(T)*)Liztiter::peek_prev())->val=(T&)x,TRUE);}
    int replace_next(const T& x) {
        if(at_end()) return FALSE;
        else
            return (((lnnk(T)*)Liztiter::peek_next())->val=(T&)x,TRUE);}
    CCList(T)* the_list(){
	return (CCList(T)*)theLizt;
    }
};
PT_end


/* List_generatordeclare(T) should be invoked by the user if there will
   be calls to the functions CCList(T)::generator() or CCList(T)::new_generator() */
PT_define List_generatordeclare(T)
class List_generator(T) : public Gene_obj(T) {
friend CCList(T);
	Listiter(T)	it;
		List_generator(T)(CCList(T)& p) : it(p) {}
public:
	T*	operator()();
};
List_generator(T)
CCList(T)::generator()
{
	return List_generator(T)(*this);
}
List_generator(T)*
CCList(T)::new_generator()
{
	return new List_generator(T)(*this);
}
PT_end


PT_define List_of_pdeclare(T)
class List_of_p(T);
class List_of_piter(T);
class List_of_p_generator(T);
List_of_pdeclare1_ATTLC(T)
Listofpiterdec1_ATTLC(T)
PT_end

Listdeclare(voidP)

PT_define List_of_pdeclare1_ATTLC(T)
Function_minideclare(T)
class Pred_obj(T);
class Tran_obj(T);
class Gene_obj(T);
class Proc_obj(T);
typedef int     (*CMPFP(T))(const T*&, const T*&);
ostream& operator<<(ostream&,List_of_p(T)&);

class List_of_p(T) : public CCList(voidP) {
	void	make_select(Predicate(T)&, const List_of_p(T)&);
	void	make_select(Pred_obj(T)&, const List_of_p(T)&);
	void	make_reject(Predicate(T)&, const List_of_p(T)&);
	void	make_reject(Pred_obj(T)&, const List_of_p(T)&);
	void	make_map(Transformer(T)&, const List_of_p(T)&);
	void	make_map(Tran_obj(T)&, const List_of_p(T)&);
	List_of_p(T)(Predicate(T)& f, const List_of_p(T)& ps,
			List_select_dummy*) { make_select(f, ps); }
	List_of_p(T)(Pred_obj(T)& f, const List_of_p(T)& ps,
			List_select_dummy*) { make_select(f, ps); }
	List_of_p(T)(Predicate(T)& f, const List_of_p(T)& ps,
			List_reject_dummy*) { make_reject(f, ps); }
	List_of_p(T)(Pred_obj(T)& f, const List_of_p(T)& ps,
			List_reject_dummy*) { make_reject(f, ps); }
	List_of_p(T)(Transformer(T)& f, const List_of_p(T)& ps,
			List_map_dummy*) { make_map(f, ps); }
	List_of_p(T)(Tran_obj(T)& f, const List_of_p(T)& ps,
			List_map_dummy*) { make_map(f, ps); }
public:
        List_of_p(T)() {}
        List_of_p(T)(const T* x) : voidP_List((voidP) x) {}
        List_of_p(T)(const T* x, const T* y) :
            voidP_List((voidP) x, (voidP) y) {}
        List_of_p(T)(const T* x, const T* y, const T* z) :
            voidP_List((voidP)x, (voidP)y, (voidP)z) {}
        List_of_p(T)(const T* x, const T* y, const T* z, const T* w) :
            voidP_List((voidP) x, (voidP) y, (voidP) z, (voidP) w) {}
	List_of_p(T)(Gene_obj(T)&);
        List_of_p(T)(const List_of_p(T)& ll) : voidP_List((voidP_List&) ll) {}
        ~List_of_p(T)() {}
        operator void*() { return voidP_List::operator void*(); }
    int	operator==(const List_of_p(T)& ll) const { return
			    ((voidP_List&)*this == (voidP_List&)ll); }
    int	operator!=(const List_of_p(T)& ll) const { return
			     ((voidP_List&)*this != (voidP_List&)ll); }
    List_of_p(T)& operator=(const List_of_p(T)& ll) {
        return (List_of_p(T)&) ((voidP_List&)*this = (voidP_List&)ll); }
    List_of_p(T)  operator+(const List_of_p(T)&);
    List_of_p(T)  operator+(const T*);
    List_of_p(T)& put(const T *t) {
        return (List_of_p(T)&) voidP_List::put((voidP)t);}
    List_of_p(T)& put(const List_of_p(T)& ll) {
        return (List_of_p(T)&) voidP_List::put((voidP_List&)ll); }
    int	unput(T *&t) { return voidP_List::unput((voidP&)t); }
    T*  unput() { return (T*)voidP_List::unput(); }
    int get(T *&t) { return voidP_List::get((voidP&)t); }
    T*  get() { return (T*)voidP_List::get(); }
    List_of_p(T)&  unget(const T *x) {
        return (List_of_p(T)&) voidP_List::unget((voidP)x); }
    List_of_p(T)&  unget(const List_of_p(T)& ll) {
        return (List_of_p(T)&) voidP_List::unget((voidP_List&)ll); }
    T*& operator[](unsigned);
    T*  head() const { return (T*)voidP_List::head(); }
    T*  tail() const { return (T*)voidP_List::tail(); }
    int find_next(const T*& t) {
        return voidP_List::find_next((voidP&)t); }
    int find_prev(const T*& t) {
        return voidP_List::find_prev((voidP&)t); }
    int next(T *&t) { return voidP_List::next((voidP&)t); }
    int next(T **&t) { return voidP_List::next((voidP*&)t); }
    T*  next() { return (T*)voidP_List::next(); }
    int prev(T *&t) { return voidP_List::prev((voidP&)t); }
    int prev(T **&t) { return voidP_List::prev((voidP*&)t); }
    T*  prev() { return (T*)voidP_List::prev(); }
    int peek_next(T *&t) const {
	return voidP_List::peek_next((voidP&)t); }
    int peek_next(T **&t) const {
	return voidP_List::peek_next((voidP*&)t); }
    T*  peek_next() const {
	return (T*)voidP_List::peek_next(); }
    int peek_prev(T *&t) const {
	return voidP_List::peek_prev((voidP&)t); }
    int peek_prev(T **&t) const {
	return voidP_List::peek_prev((voidP*&)t); }
    T*  peek_prev() const {
	return (T*)voidP_List::peek_prev(); }
    int remove_prev() { return voidP_List::remove_prev(); }
    int remove_next() { return voidP_List::remove_next(); }
    int remove_prev(T *&x) { return voidP_List::remove_prev((voidP&)x);}
    int remove_next(T *&x) { return voidP_List::remove_next((voidP&)x);}
    void insert_prev(const T *&x) {
        voidP_List::insert_prev((voidP&)x); }
    void insert_next(const T *&x) {
        voidP_List::insert_next((voidP&)x); }
    int replace_prev(const T *x) {
        return voidP_List::replace_prev((voidP)x);}
    int replace_next(const T *x) {
        return voidP_List::replace_next((voidP)x);}
    void  sort(CMPFP(T) pf) { voidP_List::sort((CMPF(voidP))pf); }
    inline List_of_p_generator(T) generator() const;
    inline List_of_p_generator(T)* new_generator() const;
    void	do_each(CCProcedure(T)& f) const;
    void	do_each(Proc_obj(T)&) const;
    T*	do_while(Predicate(T)& f) const;
    T*	do_while(Pred_obj(T)&) const;
    T*	do_until(Predicate(T)& f) const;
    T*	do_until(Pred_obj(T)&) const;
    inline List_of_p(T)	select(Predicate(T)& p) const;
    inline List_of_p(T)	select(Pred_obj(T)& p) const;
    inline List_of_p(T)	reject(Predicate(T)& p) const;
    inline List_of_p(T)	reject(Pred_obj(T)& p) const;
    inline List_of_p(T)	map(Transformer(T)& f) const;
    inline List_of_p(T)	map(Tran_obj(T)& f) const;
};
inline List_of_p(T)
List_of_p(T)::select(Predicate(T)& p) const
{
	return List_of_p(T)(p, *this, (List_select_dummy*)0);
}
inline List_of_p(T)
List_of_p(T)::select(Pred_obj(T)& p) const
{
	return List_of_p(T)(p, *this, (List_select_dummy*)0);
}
inline List_of_p(T)
List_of_p(T)::reject(Predicate(T)& p) const
{
	return List_of_p(T)(p, *this, (List_reject_dummy*)0);
}
inline List_of_p(T)
List_of_p(T)::reject(Pred_obj(T)& p) const
{
	return List_of_p(T)(p, *this, (List_reject_dummy*)0);
}
inline List_of_p(T)
List_of_p(T)::map(Transformer(T)& f) const
{
	return List_of_p(T)(f, *this, (List_map_dummy*)0);
}
inline List_of_p(T)
List_of_p(T)::map(Tran_obj(T)& f) const
{
	return List_of_p(T)(f, *this, (List_map_dummy*)0);
}
PT_end


PT_define Listofpiterdec1_ATTLC(T)
class List_of_piter(T) : public Listiter(voidP) {
public:
        List_of_piter(T)(List_of_p(T)& l) : voidP_Listiter((voidP_List&) l) {}
        List_of_piter(T)(List_of_piter(T)& l) : voidP_Listiter((voidP_Listiter&) l){}
       ~List_of_piter(T)() {}
    int	operator==(const List_of_piter(T)& ll) const {
        return ((voidP_Listiter&)*this == (voidP_Listiter&)ll); }
    List_of_piter(T)& operator=(List_of_piter(T)& ll) {
        return (List_of_piter(T)&)
               ((voidP_Listiter&)*this = (voidP_Listiter&)ll); }
    int find_next(T*& t) { return voidP_Listiter::find_next((voidP&)t);}
    int find_prev(T*& t) { return voidP_Listiter::find_prev((voidP&)t);}
    int next(T *&t)      { return voidP_Listiter::next((voidP&)t); }
    int next(T **&t)     { return voidP_Listiter::next((voidP*&)t); }
    T*  next()           { return (T*)voidP_Listiter::next(); }
    int prev(T *&t)      { return voidP_Listiter::prev((voidP&)t); }
    int prev(T **&t)     { return voidP_Listiter::prev((voidP*&)t); }
    T*  prev()           { return (T*)voidP_Listiter::prev(); }
    int peek_next(T *&t) const {
	return voidP_Listiter::peek_next((voidP&)t); }
    int peek_next(T **&t) const {
	return voidP_Listiter::peek_next((voidP*&)t); }
    T*  peek_next() const {
	return (T*)voidP_Listiter::peek_next(); }
    int peek_prev(T *&t) const {
	return voidP_Listiter::peek_prev((voidP&)t); }
    int peek_prev(T **&t) const {
	return voidP_Listiter::peek_prev((voidP*&)t); }
    T*  peek_prev() const {
	return (T*)voidP_Listiter::peek_prev(); }
    int remove_prev() { return voidP_Listiter::remove_prev(); }
    int remove_next() { return voidP_Listiter::remove_next(); }
    int remove_prev(T *&x) {
        return voidP_Listiter::remove_prev((voidP&)x);}
    int remove_next(T *&x) {
        return voidP_Listiter::remove_next((voidP&)x);}
    void insert_prev(T *&x) { voidP_Listiter::insert_prev((voidP&)x); }
    void insert_next(T *&x) { voidP_Listiter::insert_next((voidP&)x); }
    int replace_prev(T *x) {
        return voidP_Listiter::replace_prev((voidP)x);}
    int replace_next(T *x) {
        return voidP_Listiter::replace_next((voidP)x);}
    List_of_p(T)* the_list(){
	return (List_of_p(T)*)Listiter(voidP)::the_list();
    }
};
PT_end

/* List_of_p_generatordeclare(T) should be invoked by the user if 
   there will be calls to the functions List_of_p(T)::generator() or
   List_of_p(T)::new_generator() */
PT_define List_of_p_generatordeclare(T)
class List_of_p_generator(T) : public Gene_obj(T) {
friend List_of_p(T);
	List_of_piter(T)	it;
		List_of_p_generator(T)(const List_of_p(T)&);
		~List_of_p_generator(T)();
public:
	T*	operator()();
};
List_of_p_generator(T)
List_of_p(T)::generator() const
{
	return List_of_p_generator(T)(*this);
}
List_of_p_generator(T)*
List_of_p(T)::new_generator() const
{
	return new List_of_p_generator(T)(*this);
}
PT_end

/* Listimplement(T) should be invoked exactly once per a.out -- it actually
   builds the executable code for the non-inline member functions of the
   classes CCList(T) and Listiter(T). */
PT_define Listimplement(T)
Functiondeclare(T)
#ifndef BUILDING_LIBRARY
Functionimplement(T)
#endif
Listimplement1_ATTLC(T)
Listiterimp_ATTLC(T)
List_generatordeclare(T)
List_generatorimplement(T)
PT_end

PT_define Listimplement1_ATTLC(T)
int L_error(const char*);
Objection CCList(T)::domain_error(L_error);
Pooldef_ATTLC(T)

lnk*
lnnk(T)::copy()
{
    return new lnnk(T)((T&)val);
}
int
lnnk(T)::operator==(lnk& x)
{
    return val == ((lnnk(T)*)&x)->val;
}
lnk *
getnewlnnk_ATTLC(T)(const T& x) {
    return (new lnnk(T)((T&)x));
}
void
deletelnnk_ATTLC(T)(T& t, lnnk(T)* ptr) {
    t = ptr->val;
    delete ptr;
}
CCList(T)::CCList(T)(const CCList(T)& a0, const T& t) : Lizt((Lizt&)a0)
{
    put(t);
}
CCList(T)::CCList(T)(const T& t)
{
    put(t);
}
CCList(T)::CCList(T)(const T& t, const T& u)
{
    put(t);
    put(u);
}
CCList(T)::CCList(T)(const T& t, const T& u, const T& v)
{
    put(t);
    put(u);
    put(v);
}
CCList(T)::CCList(T)(const T& t, const T& u, const T& v, const T& w)
{
    put(t);
    put(u);
    put(v);
    put(w);
}
CCList(T)::CCList(T)(Gene_obj(T)& gen)
{
    T*	pt;
    while (pt = gen())
	put(*pt);
}
CCList(T)::CCList(T)(const CCList(T)& a) : Lizt(*(Lizt*)&a) {}
T CCList(T)::unput() { lnnk(T)* ll = (lnnk(T)*)Lizt::unput();
    		 if(!ll) OBJ__ATTLC("unput: empty List",T)
                 T ans = ll->val; delete ll; return ans; }
T CCList(T)::get() { lnnk(T)* ll = (lnnk(T)*)Lizt::get();
    		if(!ll) OBJ__ATTLC("get: empty List",T)
                T ans = ll->val; delete ll; return ans; }
T&
CCList(T)::operator[](unsigned ii)
{
    if(ii > length()) OBJ__ATTLC("[] out of bounds",T)
    return (T&)*(getAt(ii));
}
T
CCList(T)::head() const
{
    lnnk(T)* ll = (lnnk(T)*)Lizt::head();
    if(!ll) OBJ__ATTLC("head: empty List",T)
    return ll->val;
}
T
CCList(T)::tail() const
{
    lnnk(T)* ll = (lnnk(T)*)Lizt::tail();
    if(!ll) OBJ__ATTLC("tail: empty List",T)
    return ll->val;
}
void
CCList(T)::make_select(Predicate(T)& f, const CCList(T)& ps)
{
	Listiter(T)	it((CCList(T)&)ps);
	/* register */ T*	tp;
	while (it.next(tp))
		if (f(tp))
			put(*tp);
}
void
CCList(T)::make_select(Pred_obj(T)& f, const CCList(T)& ps)
{
	Listiter(T)	it((CCList(T)&)ps);
	/* register */ T*	tp;
	while (it.next(tp))
		if (f(tp))
			put(*tp);
}
void
CCList(T)::make_reject(Predicate(T)& f, const CCList(T)& ps)
{
	Listiter(T)	it((CCList(T)&)ps);
	/* register */ T*	tp;
	while (it.next(tp))
		if (!f(tp))
			put(*tp);
}
void
CCList(T)::make_reject(Pred_obj(T)& f, const CCList(T)& ps)
{
	Listiter(T)	it((CCList(T)&)ps);
	/* register */ T*	tp;
	while (it.next(tp))
		if (!f(tp))
			put(*tp);
}
void
CCList(T)::make_map(Transformer(T)& f, const CCList(T)& ps)
{
	Listiter(T)	it((CCList(T)&)ps);
	/* register */ T*	tp;
	while (it.next(tp))
		put(*f(tp));
}
void
CCList(T)::make_map(Tran_obj(T)& f, const CCList(T)& ps)
{
	Listiter(T)	it((CCList(T)&)ps);
	/* register */ T*	tp;
	while (it.next(tp))
		put(*f(tp));
}
void
CCList(T)::do_each(CCProcedure(T)& f) const
{
	Listiter(T)	it(*(CCList(T)*)this);
	/* register */ T*	tp;
	while (it.next(tp))
		f(tp);
}
void
CCList(T)::do_each(Proc_obj(T)& f) const
{
	Listiter(T)	it(*(CCList(T)*)this);
	/* register */ T*	tp;
	while (it.next(tp))
		f(tp);
}
T*
CCList(T)::do_while(Predicate(T)& f) const
{
	Listiter(T)	it(*(CCList(T)*)this);
	/* register */ T*	tp;
	while (it.next(tp))
		if (!f(tp))
			return tp;
	return 0;
}
T*
CCList(T)::do_while(Pred_obj(T)& f) const
{
	Listiter(T)	it(*(CCList(T)*)this);
	/* register */ T*	tp;
	while (it.next(tp))
		if (!f(tp))
			return tp;
	return 0;
}
T*
CCList(T)::do_until(Predicate(T)& f) const
{
	Listiter(T)	it(*(CCList(T)*)this);
	/* register */ T*	tp;
	while (it.next(tp))
		if (f(tp))
			return tp;
	return 0;
}
T*
CCList(T)::do_until(Pred_obj(T)& f) const
{
	Listiter(T)	it(*(CCList(T)*)this);
	/* register */ T*	tp;
	while (it.next(tp))
		if (f(tp))
			return tp;
	return 0;
}
PT_end


PT_define Listiterimp_ATTLC(T)
T
Listiter(T)::next()
{
    lnnk(T)* ll = (lnnk(T)*)Liztiter::next();
    if(!ll) OBJ__ATTLC("next: at end of List",T)
    return ll->val;
}
T
Listiter(T)::prev()
{
    lnnk(T)* ll = (lnnk(T)*)Liztiter::prev();
    if(!ll) OBJ__ATTLC("prev: at head of List",T)
    return ll->val;
}
int
Listiter(T)::find_prev(const T& x)
{
    if(at_head()|| theLizt->length()==0) return FALSE;
    lnnk(T)* iter = (lnnk(T)*) pred->nxt;
    register int i = index;
    do {
        iter = (lnnk(T)*) iter->prv;
        if (iter->val==(T&)x) {
	    index = i;
	    pred = iter;
	    return TRUE;
	}
	i--;
    } while (i > 0);
    return FALSE;
}
int
Listiter(T)::find_next(const T& x)
{
    if(at_end()||theLizt->length()==0) return FALSE;
    lnnk(T)* iter = (lnnk(T)*) pred;
    register int i = index;
    do {
        iter = (lnnk(T)*) iter->nxt;
        if (iter->val==(T&)x) {
	    index = i;
	    pred = iter->prv;
	    return TRUE;
	}
	i++;
    } while (i < theLizt->length());
    return FALSE;
}
int
Listiter(T)::remove_prev()
{
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_prev();
    if(aLink) return (delete aLink, TRUE);
    else return FALSE;
}
int
Listiter(T)::remove_prev(T &x)
{
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_prev();
    if(aLink) return (x = aLink->val,delete aLink, TRUE);
    else return FALSE;
}
int
Listiter(T)::remove_next()
{
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_next();
    if(aLink) return (delete aLink, TRUE);
    else return FALSE;
}
int
Listiter(T)::remove_next(T &x)
{
    lnnk(T) *aLink = (lnnk(T) *)Liztiter::remove_next();
    if(aLink) return (x = aLink->val, delete aLink, TRUE);
    else return FALSE;
}
T
Listiter(T)::peek_prev() const
{
    if (at_head()) OBJ__ATTLC("peek_prev: at head of List",T)
    return ((lnnk(T)*)Liztiter::peek_prev())->val;
}
int
Listiter(T)::peek_prev(T& t) const
{
    if(at_head()) return FALSE;
    else return (t = ((lnnk(T)*)Liztiter::peek_prev())->val, TRUE);
}
int
Listiter(T)::peek_prev(T*& t) const
{
    if(at_head()) return FALSE;
    else return (t = &((lnnk(T)*)Liztiter::peek_prev())->val, TRUE);
}
T
Listiter(T)::peek_next() const
{
    if (at_end()) OBJ__ATTLC("peek_next: at end of List",T)
    return ((lnnk(T)*)Liztiter::peek_next())->val;
}
int
Listiter(T)::peek_next(T& t) const
{
    if(at_end()) return FALSE;
    else return (t = ((lnnk(T)*)Liztiter::peek_next())->val, TRUE);
}
int
Listiter(T)::peek_next(T*& t) const
{
    if(at_end()) return FALSE;
    else return (t = &((lnnk(T)*)Liztiter::peek_next())->val, TRUE);
}
void
CCList(T)::sort(CMPF(T) cmp)
{
    if (length() < 2) return;
    voidP_List_sort_internal(*(voidP_List*)this, (CMPF(voidP))cmp);
    reset_all_iters();
}
PT_end
PT_define List_generatorimplement(T)
T*
List_generator(T)::operator()()
{
	T*	p;
	return it.next(p) ? p : 0;
}
PT_end


PT_define List_of_pimplement(T)
Functiondeclare(T)
#ifndef BUILDING_LIBRARY
Functionimplement(T)
#endif
List_of_p(T)::List_of_p(T)(Gene_obj(T)& gen)
{
	T*	p;
	while (p = gen())
		put(p);
}
List_of_p(T) List_of_p(T)::operator+(const List_of_p(T)& ll) {
        return (List_of_p(T)&) ((voidP_List&)*this + (voidP_List&)ll); }
List_of_p(T) List_of_p(T)::operator+(const T* t) {
        return (List_of_p(T)&) ((voidP_List&)*this + (voidP)t); }
T*&
List_of_p(T)::operator[](unsigned ii)
{
    if(ii > length()) OBJP__ATTLC("[] out of bounds",T)
    return (T*&)(*getAt(ii));
}
void
List_of_p(T)::make_select(Predicate(T)& f, const List_of_p(T)& ps)
{
	List_of_piter(T)	it((List_of_p(T)&)ps);
	register T*	tp;
	while (it.next(tp))
		if (f(tp))
			put(tp);
}
void
List_of_p(T)::make_select(Pred_obj(T)& f, const List_of_p(T)& ps)
{
	List_of_piter(T)	it((List_of_p(T)&)ps);
	register T*	tp;
	while (it.next(tp))
		if (f(tp))
			put(tp);
}
void
List_of_p(T)::make_reject(Predicate(T)& f, const List_of_p(T)& ps)
{
	List_of_piter(T)	it((List_of_p(T)&)ps);
	register T*	tp;
	while (it.next(tp))
		if (!f(tp))
			put(tp);
}
void
List_of_p(T)::make_reject(Pred_obj(T)& f, const List_of_p(T)& ps)
{
	List_of_piter(T)	it((List_of_p(T)&)ps);
	register T*	tp;
	while (it.next(tp))
		if (!f(tp))
			put(tp);
}
void
List_of_p(T)::make_map(Transformer(T)& f, const List_of_p(T)& ps)
{
	List_of_piter(T)	it((List_of_p(T)&)ps);
	register T*	tp;
	while (it.next(tp))
		put(f(tp));
}
void
List_of_p(T)::make_map(Tran_obj(T)& f, const List_of_p(T)& ps)
{
	List_of_piter(T)	it((List_of_p(T)&)ps);
	register T*	tp;
	while (it.next(tp))
		put(f(tp));
}
void
List_of_p(T)::do_each(CCProcedure(T)& f) const
{
	List_of_piter(T)	it(*(List_of_p(T)*)this);
	register T*	tp;
	while (it.next(tp))
		f(tp);
}
void
List_of_p(T)::do_each(Proc_obj(T)& f) const
{
	List_of_piter(T)	it(*(List_of_p(T)*)this);
	register T*	tp;
	while (it.next(tp))
		f(tp);
}
T*
List_of_p(T)::do_while(Predicate(T)& f) const
{
	List_of_piter(T)	it(*(List_of_p(T)*)this);
	register T*	tp;
	while (it.next(tp))
		if (!f(tp))
			return tp;
	return 0;
}
T*
List_of_p(T)::do_while(Pred_obj(T)& f) const
{
	List_of_piter(T)	it(*(List_of_p(T)*)this);
	register T*	tp;
	while (it.next(tp))
		if (!f(tp))
			return tp;
	return 0;
}
T*
List_of_p(T)::do_until(Predicate(T)& f) const
{
	List_of_piter(T)	it(*(List_of_p(T)*)this);
	register T*	tp;
	while (it.next(tp))
		if (f(tp))
			return tp;
	return 0;
}
T*
List_of_p(T)::do_until(Pred_obj(T)& f) const
{
	List_of_piter(T)	it(*(List_of_p(T)*)this);
	register T*	tp;
	while (it.next(tp))
		if (f(tp))
			return tp;
	return 0;
}
List_of_p_generatordeclare(T)
List_of_p_generatorimplement(T)
PT_end

PT_define List_of_p_generatorimplement(T)
List_of_p_generator(T)::List_of_p_generator(T)(const List_of_p(T)& p) :
	it((List_of_p(T)&)p) {}
List_of_p_generator(T)::~List_of_p_generator(T)() {}
T*
List_of_p_generator(T)::operator()()
{
	T*	p;
	return it.next(p) ? p : 0;
}
PT_end

/* Listout(T) and List_of_pout(T) implement the iostreams output functions
   for CCList(T) objects and List_of_p(T) objects respectively. */
PT_define Listout(T)
ostream&
operator<<(ostream& oo, CCList(T)& ll)
{
    oo<<"( ";
    Listiter(T) l(ll);
    while (!l.at_end()) oo << l.next() << " ";
    oo<<")";
    return oo;
}
PT_end


PT_define List_of_pout(T)
ostream&
operator<<(ostream& oo, List_of_p(T)& ll)
{
    oo<<"( ";
    List_of_piter(T) l(ll);
    while (!l.at_end()) oo << *(l.next()) << " ";
    oo<<")";
    return oo;
}
PT_end

#endif
