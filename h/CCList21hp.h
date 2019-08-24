/**************************************************************************

                          All Rights Reserved

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T

        The copyright notice above does not evidence any
        actual or intended publication of such source code.

Edited by: enno on 8/11/94.
Original file: /usr/add-on/C++/2.1/hp/include/List.h

Edits: substituted "List(" by "CCList(".
NOTE: don't substitute "the_list(" by "the_CCList(" etc.

***************************************************************************/
#ifndef LISTH
#define LISTH

#include <new.h>
#include <Pool.h>

class ostream;

typedef void* voidP;

PT_names
	CCList(T) 		T _List_ATTLC
	Listiter(T) 		T _Listiter_ATTLC
	Const_listiter(T)	T _cListiter_ATTLC
	List_of_piter(T) 	T _Listofpiter_ATTLC
	Const_list_of_piter(T) 	T _cListofpiter_ATTLC
	List_of_p(T) 		T P_List_ATTLC
	CMPF(T) 		T CMPFN_ATTLC
	CMPFP(T) 		T CMPFNP_ATTLC
	lnnk_ATTLC(T) 		T lnnk_ATTLC
	getnewlnnk_ATTLC(T)	T getnewlnnk_ATTLC_
	deletelnnk_ATTLC(T)	T deletelnnk_ATTLC_
PT_end

/* implements two-way pointers 
*/
struct lnk_ATTLC {
	lnk_ATTLC* nxt;
	lnk_ATTLC* prv;
	void init(lnk_ATTLC* p, lnk_ATTLC* s) { prv = p; nxt = s; }
	lnk_ATTLC() {}
	virtual ~lnk_ATTLC();
	virtual lnk_ATTLC* copy() = 0;
	virtual int operator==(lnk_ATTLC&) = 0;
};

class Lizt_ATTLC;
class CCList(voidP);

/* base class for all [Const_]Listiter(T)'s 
*/
class Liztiter_ATTLC {
private:
	friend Lizt_ATTLC;
	Liztiter_ATTLC(Lizt_ATTLC* lp) : theLizt(lp), cache(0), nextIt(0) { }
public:
	int at_head() const	{ return index == 0 ; }
protected:
	Lizt_ATTLC* theLizt;	// associated List
	Liztiter_ATTLC* nextIt;	// next on chain
	lnk_ATTLC*  cache;	// a recently retrieved link
	int cacheNo;		// its index or garbage if cache == 0
	int index;		// current position
	lnk_ATTLC* pred;	// current position
	lnk_ATTLC* getAt(int);
	lnk_ATTLC* next();
	lnk_ATTLC* prev();
	lnk_ATTLC* peek_next() const { 
		return at_end() ? (lnk_ATTLC*)0 : pred->nxt; 
	}
	lnk_ATTLC*  peek_prev() const { 
		return at_head() ? (lnk_ATTLC*)0 : pred; 
	}
	void insert_prev(lnk_ATTLC*);
	void insert_next(lnk_ATTLC*);
	lnk_ATTLC* remove_prev();
	lnk_ATTLC* remove_next();
	void reset0();
public:
	// this should be protected
	int operator==(const Liztiter_ATTLC& l) const {	
		return theLizt == l.theLizt && index == l.index; 
	}
	int operator!=(const Liztiter_ATTLC& l) const {
		return !(*this == l);
	}
	Liztiter_ATTLC&  operator=(const Liztiter_ATTLC& l);
	Liztiter_ATTLC(Lizt_ATTLC&);
	Liztiter_ATTLC(const Liztiter_ATTLC&);
	~Liztiter_ATTLC();
	int position() const { return index; }
	void reset(unsigned i=0);
	void end_reset(unsigned i=0);
	int at_end() const;
};

/* base class for all CCList(T)'s 
*/
class Lizt_ATTLC {
protected:
	friend Liztiter_ATTLC;
	lnk_ATTLC* t;			// tail
	int  sz;			// number of elements
	Liztiter_ATTLC myit;		// current position pointer
	void  delete_all();		// used by dtor and operator=()
	void init_all(const Lizt_ATTLC&);	// used by ctor and operator=()
	void init_all_to_empty();   // used by make_empty()
	void add_a_link(lnk_ATTLC*);	// used by put() and unget()
	lnk_ATTLC*  tail() const	{ return t; }
	lnk_ATTLC*  head() const	{ return t ? t->nxt : 0; }
	Lizt_ATTLC();
	Lizt_ATTLC(const Lizt_ATTLC& x) : myit(this) { init_all(x); }
	Lizt_ATTLC(const Lizt_ATTLC&, const Lizt_ATTLC&);
	~Lizt_ATTLC();
	operator void*()	{ return sz ? this : 0; }
	Lizt_ATTLC& unget(lnk_ATTLC*);
	Lizt_ATTLC& unget(const Lizt_ATTLC&);
	Lizt_ATTLC& put(lnk_ATTLC*);
	Lizt_ATTLC& put(const Lizt_ATTLC&);
	lnk_ATTLC* get();
	lnk_ATTLC* unput();
	lnk_ATTLC* getAt(int i)		{ return myit.getAt(i); }
	lnk_ATTLC* next()		{ return myit.next(); }
	lnk_ATTLC* prev()		{ return myit.prev(); }
	lnk_ATTLC* peek_next() const	{ return myit.peek_next(); }
	lnk_ATTLC* peek_prev() const	{ return myit.peek_prev(); }
	void insert_prev(lnk_ATTLC* l)	{ myit.insert_prev(l); }
	void insert_next(lnk_ATTLC* l)	{ myit.insert_next(l); }
	lnk_ATTLC* remove_prev()	{ return myit.remove_prev(); }
	lnk_ATTLC* remove_next()	{ return myit.remove_next(); }
public:
	int operator==(const Lizt_ATTLC&) const;
	int operator!=(const Lizt_ATTLC&) const;
	Lizt_ATTLC& operator=(const Lizt_ATTLC&);
	void make_empty();
	int length() const	{ return sz; }
	int position() const	{ return myit.index; }
	void reset(unsigned i=0){ myit.reset(i); }
	void end_reset(unsigned i=0) { myit.end_reset(i); }
	int at_end() const	{return myit.at_end(); }
	int at_head() const	{ return myit.at_head(); }
	void reset_all_iters();
};

PT_define Listdeclare(T)

Listdeclare1_ATTLC(T)
lnnkdeclare_ATTLC(T)
Listiterdeclare_ATTLC(T)
Listdeclare2_ATTLC(T)

PT_end

PT_define Listdeclare1_ATTLC(T)

class lnnk_ATTLC(T);
class CCList(T);
class Listiter(T);
class Const_listiter(T);

typedef int (*CMPF(T))(const T&, const T&);

ostream& operator<<(ostream&,const CCList(T)&);

PT_end

PT_define lnnkdeclare_ATTLC(T)

class lnnk_ATTLC(T) : public lnk_ATTLC {
	friend CCList(T);
	friend Const_listiter(T);
	friend Listiter(T);
	friend void voidP_List_sort_internal(CCList(voidP)&, CMPF(voidP));
	static Pool* pool;
	T val;
	lnk_ATTLC* copy();
	int operator==(lnk_ATTLC&);
	lnnk_ATTLC(T)(const T& pp) : val(pp) {}
	~lnnk_ATTLC(T)();
public:
	void* operator new(size_t);
	void operator delete(void* l) {
		pool->free(l); 
	}
	// should be called by List constructors
	static void init_pool() {
		if (pool == 0)
			pool = new Pool(sizeof(lnnk_ATTLC(T)));
	}
	friend lnk_ATTLC* getnewlnnk_ATTLC(T)(const T&);
	friend void deletelnnk_ATTLC(T)(T&, lnnk_ATTLC(T)*);
};

PT_end

PT_define Listdeclare2_ATTLC(T)

class CCList(T) : public Lizt_ATTLC {
private:
	CCList(T)(const CCList(T)& a0, const CCList(T)& a1)
		: Lizt_ATTLC((const Lizt_ATTLC&)a0, (const Lizt_ATTLC&)a1) {}
	CCList(T)(const CCList(T)& a0, const T& t);
protected:
	T* getAt(int i) { 
		lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Lizt_ATTLC::getAt(i);
		if(ll) return &(ll->val); 
		else return (T*)0;
	}
public:
	friend void voidP_List_sort_internal(CCList(voidP)&, CMPF(voidP));
	CCList(T)();
	CCList(T)(const CCList(T)&);
	CCList(T)(const T& x);
	CCList(T)(const T& x, const T& y);
	CCList(T)(const T& x, const T& y, const T& z);
	CCList(T)(const T& x, const T& y, const T& z, const T& w);
	~CCList(T)() {}
	operator void*() { return Lizt_ATTLC::operator void*(); }
	int operator==(const CCList(T)& l) const {
		return (const Lizt_ATTLC&)*this == (const Lizt_ATTLC&)l;
	}
	int operator!=(const CCList(T)& l) const {
		return !(*this == l);
	}
	CCList(T) operator+(const CCList(T)& ll) { return CCList(T)(*this, ll); }
	CCList(T) operator+(const T& t) { return CCList(T)(*this, t); }
	CCList(T)& operator=(const CCList(T)& a) {
		return (CCList(T)&) (*(Lizt_ATTLC*)this = *(const Lizt_ATTLC*)&a); 
	}
	CCList(T)& put(const T& x) {
		return (CCList(T)&) Lizt_ATTLC::put(getnewlnnk_ATTLC(T)(x)); 
	}
	CCList(T)& put(const CCList(T)& ll) {
		return (CCList(T)&) Lizt_ATTLC::put((const Lizt_ATTLC&) ll); 
	}
	T unput();
	int unput(T& t) {
		lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Lizt_ATTLC::unput();
		if(ll) { 
			deletelnnk_ATTLC(T)(t, ll); return 1; 
		}
		else	return 0; 
	}
	T get();
	int get(T& t) { 
		lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Lizt_ATTLC::get();
		if(ll) { 
			deletelnnk_ATTLC(T)(t, ll); return 1; 
		}
		else return 0; 
	}
	CCList(T)&  unget(const T& x) { 
		return (CCList(T)&)Lizt_ATTLC::unget(getnewlnnk_ATTLC(T)(x)); 
	}
	CCList(T)&  unget(const CCList(T)& ll) { 
		return (CCList(T)&)Lizt_ATTLC::unget((const Lizt_ATTLC&) ll); 
	}
	T head() const;
	T tail() const;
	int find_next(const T& t) {
		return ((Listiter(T)*)(&myit))->find_next(t);
	}
	int find_prev(const T& t) {
		return ((Listiter(T)*)(&myit))->find_prev(t);
	}
	int next(T& t)	{ return ((Listiter(T)*)(&myit))->next(t); }
	int next(T*& t)	{ return ((Listiter(T)*)(&myit))->next(t); }
	T next()	{ return ((Listiter(T)*)(&myit))->next(); }
	int prev(T& t)	{ return ((Listiter(T)*)(&myit))->prev(t); }
	int prev(T*& t)	{ return ((Listiter(T)*)(&myit))->prev(t); }
	T prev()	{ return ((Listiter(T)*)(&myit))->prev(); }
	int step_next() { return Lizt_ATTLC::next() != 0; }
	int step_prev() { return Lizt_ATTLC::prev() != 0; }

	int peek_next(T& t) const {
		return ((Const_listiter(T)*)(&((CCList(T)*)this)->myit))->peek_next(t); 
	}
	int peek_next(T*& t) const {
		return ((Const_listiter(T)*)(&((CCList(T)*)this)->myit))->peek_next(t); 
	}
	T peek_next() const {
		return ((Const_listiter(T)*)(&((CCList(T)*)this)->myit))->peek_next(); 
	}
	int peek_prev(T& t) const {
		return ((Const_listiter(T)*)(&((CCList(T)*)this)->myit))->peek_prev(t); 
	}
	int peek_prev(T*& t) const {
		return ((Const_listiter(T)*)(&((CCList(T)*)this)->myit))->peek_prev(t); 
	}
	T peek_prev() const {
		return ((Const_listiter(T)*)(&((CCList(T)*)this)->myit))->peek_prev(); 
	}
	int remove_prev() { return ((Listiter(T)*)(&myit))->remove_prev(); }
	int remove_next() { return ((Listiter(T)*)(&myit))->remove_next(); }
	int remove_prev(T& t) {
		return ((Listiter(T)*)(&myit))->remove_prev(t); 
	}
	int remove_next(T& t) {
		return ((Listiter(T)*)(&myit))->remove_next(t); 
	}
	void insert_prev(const T& x) {
		Lizt_ATTLC::insert_prev(getnewlnnk_ATTLC(T)(x)); 
	}
	void insert_next(const T& x) {
		Lizt_ATTLC::insert_next(getnewlnnk_ATTLC(T)(x)); 
	}
	int replace_prev(const T& x) {
		if(at_head()) return 0;
		else return (((lnnk_ATTLC(T)*)Lizt_ATTLC::peek_prev())->val=x, 1); 
	}
	int replace_next(const T& x) {
		if(at_end()) return 0;
		else return (((lnnk_ATTLC(T)*)Lizt_ATTLC::peek_next())->val=x, 1); 
	}
	T& operator[](unsigned ii);
	const T& operator[](unsigned ii) const;
	void sort(CMPF(T));
};
PT_end

PT_define Listiterdeclare_ATTLC(T)

class Const_listiter(T) : public Liztiter_ATTLC {
protected:
	T* getAt(int i) { 
		lnnk_ATTLC(T)* ll = ((lnnk_ATTLC(T)*)Liztiter_ATTLC::getAt(i));
		if(ll) return &(ll->val); else return (T*)0;
	}
public:
	Const_listiter(T)(const CCList(T)& a);
	Const_listiter(T)(const Const_listiter(T)& a);
	~Const_listiter(T)() {}
	int operator==(const Const_listiter(T)& l) const {
		return (const Liztiter_ATTLC&)*this == (const Liztiter_ATTLC&)l;
	}
	int operator!=(const Const_listiter(T)& l) const {
		return !(*this == l);
	}
	Const_listiter(T)& operator=(const Const_listiter(T)& l) {
		return (Const_listiter(T)&) ((Liztiter_ATTLC&)*this=(const Liztiter_ATTLC&)l); 
	}
	int find_next(const T&);
	int find_prev(const T&);
	int next(T& t) {
		if(at_end()) return 0;
		else return (t = ((lnnk_ATTLC(T)*)Liztiter_ATTLC::next())->val, 1); 
	}
	int next(T*& t) {
		if(at_end()) return 0;
		else return (t = &((lnnk_ATTLC(T)*)Liztiter_ATTLC::next())->val, 1);
	}
	T next();
	int prev(T& t) {
		if(at_head()) return 0;
		else return (t=((lnnk_ATTLC(T)*)Liztiter_ATTLC::prev())->val, 1); 
	}
	int prev(T*& t) {
		if(at_head()) return 0;
		else return (t= &((lnnk_ATTLC(T)*)Liztiter_ATTLC::prev())->val, 1); 
	}
	T prev();
	int step_next() { return Liztiter_ATTLC::next() != 0; }
	int step_prev() { return Liztiter_ATTLC::prev() != 0; }
	int peek_next(T&) const;
	int peek_next(T*&) const;
	T peek_next() const;
	int peek_prev(T&) const;
	int peek_prev(T*&) const;
	T peek_prev() const;
	const CCList(T)* the_list() {
		return (const CCList(T)*)theLizt;
	}
};

class Listiter(T) : public Const_listiter(T) {
public:
	Listiter(T)(CCList(T)& a);
	Listiter(T)(const Listiter(T)& a);
	~Listiter(T)() {}
	int operator==(const Listiter(T)& l) const {
		return (const Liztiter_ATTLC&)*this == (const Liztiter_ATTLC&)l;
	}
	int operator!=(const Listiter(T)& l) const {
		return !(*this == l);
	}
	Listiter(T)& operator=(const Listiter(T)& l) {
		return (Listiter(T)&) ((Liztiter_ATTLC&)*this=(const Liztiter_ATTLC&)l); 
	}
	CCList(T)* the_list() {
		return (CCList(T)*)theLizt;
	}
	// the following operations change the container
	int remove_prev();
	int remove_next();
	int remove_prev(T&);
	int remove_next(T&);
	void insert_prev(const T& x) {
		Liztiter_ATTLC::insert_prev(getnewlnnk_ATTLC(T)(x)); 
	}
	void insert_next(const T& x) {
		Liztiter_ATTLC::insert_next(getnewlnnk_ATTLC(T)(x)); 
	}
	int replace_prev(const T& x) {
		if(at_head()) return 0;
		else return (((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_prev())->val=x,1);
	}
	int replace_next(const T& x) {
		if(at_end()) return 0;
		else return (((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_next())->val=x,1);
	}
};

PT_end

PT_define List_of_pdeclare(T)

class List_of_p(T);
class List_of_piter(T);
class Const_list_of_piter(T);
List_of_pdeclare1_ATTLC(T)
Listofpiterdec1_ATTLC(T)

PT_end

Listdeclare(voidP)

PT_define List_of_pdeclare1_ATTLC(T)

typedef int (*CMPFP(T))(const T*&, const T*&);
ostream& operator<<(ostream&,const List_of_p(T)&);

class List_of_p(T) : public CCList(voidP) {
public:
	List_of_p(T)() {}
	List_of_p(T)(const T* x);
	List_of_p(T)(const T* x, const T* y);
	List_of_p(T)(const T* x, const T* y, const T* z);
	List_of_p(T)(const T* x, const T* y, const T* z, const T* w);
	List_of_p(T)(const List_of_p(T)& ll) : CCList(voidP)((const CCList(voidP)&) ll) {}
	~List_of_p(T)() {}
	operator void*() { return CCList(voidP)::operator void*(); }
	int operator==(const List_of_p(T)& ll) const { 
		return (const CCList(voidP)&)*this == (const CCList(voidP)&)ll;
	}
	int operator!=(const List_of_p(T)& l) const {
		return !(*this == l);
	}
	List_of_p(T)& operator=(const List_of_p(T)& ll) {
		return (List_of_p(T)&) ((CCList(voidP)&)*this = (const CCList(voidP)&)ll); 
	}
	List_of_p(T) operator+(const List_of_p(T)&);
	List_of_p(T) operator+(const T*);
	List_of_p(T)& put(const T *t) {
		return (List_of_p(T)&) CCList(voidP)::put((voidP)t);
	}
	List_of_p(T)& put(const List_of_p(T)& ll) {
		return (List_of_p(T)&) CCList(voidP)::put((const CCList(voidP)&)ll); 
	}
	int unput(T *&t) { return CCList(voidP)::unput((voidP&)t); }
	T* unput() { return (T*)CCList(voidP)::unput(); }
	int get(T *&t) { return CCList(voidP)::get((voidP&)t); }
	T* get() { return (T*)CCList(voidP)::get(); }
	List_of_p(T)& unget(const T *x) {
		return (List_of_p(T)&) CCList(voidP)::unget((voidP)x); 
	}
	List_of_p(T)& unget(const List_of_p(T)& ll) {
		return (List_of_p(T)&) CCList(voidP)::unget((const CCList(voidP)&)ll); 
	}
	T*& operator[](unsigned);
	const T*& operator[](unsigned) const;
	T* head() const { return (T*)CCList(voidP)::head(); }
	T* tail() const { return (T*)CCList(voidP)::tail(); }
	int find_next(const T*& t) {
		return CCList(voidP)::find_next((voidP&)t); 
	}
	int find_prev(const T*& t) {
		return CCList(voidP)::find_prev((voidP&)t); 
	}
	int next(T *&t) { return CCList(voidP)::next((voidP&)t); }
	int next(T **&t) { return CCList(voidP)::next((voidP*&)t); }
	T* next() { return (T*)CCList(voidP)::next(); }
	int prev(T *&t) { return CCList(voidP)::prev((voidP&)t); }
	int prev(T **&t) { return CCList(voidP)::prev((voidP*&)t); }
	T* prev() { return (T*)CCList(voidP)::prev(); }
	int peek_next(T *&t) const {
	return CCList(voidP)::peek_next((voidP&)t); }
	int peek_next(T **&t) const {
	return CCList(voidP)::peek_next((voidP*&)t); }
	T*  peek_next() const {
	return (T*)CCList(voidP)::peek_next(); }
	int peek_prev(T *&t) const {
	return CCList(voidP)::peek_prev((voidP&)t); }
	int peek_prev(T **&t) const {
	return CCList(voidP)::peek_prev((voidP*&)t); }
	T* peek_prev() const {
	return (T*)CCList(voidP)::peek_prev(); }
	int remove_prev() { return CCList(voidP)::remove_prev(); }
	int remove_next() { return CCList(voidP)::remove_next(); }
	int remove_prev(T *&x) { return CCList(voidP)::remove_prev((voidP&)x);}
	int remove_next(T *&x) { return CCList(voidP)::remove_next((voidP&)x);}
	void insert_prev(const T *&x) {
		CCList(voidP)::insert_prev((voidP&)x); 
	}
	void insert_next(const T *&x) {
		CCList(voidP)::insert_next((voidP&)x); 
	}
	int replace_prev(const T *x) {
		return CCList(voidP)::replace_prev((const voidP)x);
	}
	int replace_next(const T *x) {
		return CCList(voidP)::replace_next((const voidP)x);
	}
	void sort(CMPFP(T) pf) { CCList(voidP)::sort((CMPF(voidP))pf); }
};

PT_end

PT_define Listofpiterdec1_ATTLC(T)

class Const_list_of_piter(T) : public Listiter(voidP) {
public:
	Const_list_of_piter(T)(const List_of_p(T)& l);
	Const_list_of_piter(T)(const Const_list_of_piter(T)& l);
	~Const_list_of_piter(T)() {}
	int operator==(const Const_list_of_piter(T)& l) const {
		return (const Listiter(voidP)&)*this == ((const Listiter(voidP)&)l); 
	}
	int operator!=(const Const_list_of_piter(T)& l) const {
		return !(*this == l);
	}
	Const_list_of_piter(T)& operator=(const Const_list_of_piter(T)& ll) {
		return (Const_list_of_piter(T)&)((Listiter(voidP)&)*this = (const Listiter(voidP)&)ll); 
	}
	int find_next(T*& t) { return Listiter(voidP)::find_next((voidP&)t);}
	int find_prev(T*& t) { return Listiter(voidP)::find_prev((voidP&)t);}
	int next(T *&t)	{ return Listiter(voidP)::next((voidP&)t); }
	int next(T **&t) { return Listiter(voidP)::next((voidP*&)t); }
	T* next() { return (T*)Listiter(voidP)::next(); }
	int prev(T *&t) { return Listiter(voidP)::prev((voidP&)t); }
	int prev(T **&t) { return Listiter(voidP)::prev((voidP*&)t); }
	T* prev() { return (T*)Listiter(voidP)::prev(); }
	int peek_next(T *&t) const {
		return Listiter(voidP)::peek_next((voidP&)t); 
	}
	int peek_next(T **&t) const {
		return Listiter(voidP)::peek_next((voidP*&)t); 
	}
	T*  peek_next() const {
		return (T*)Listiter(voidP)::peek_next(); 
	}
	int peek_prev(T *&t) const {
		return Listiter(voidP)::peek_prev((voidP&)t); 
	}
	int peek_prev(T **&t) const {
		return Listiter(voidP)::peek_prev((voidP*&)t); 
	}
	T*  peek_prev() const {
		return (T*)Listiter(voidP)::peek_prev(); 
	}
	const List_of_p(T)* the_list(){
		return (const List_of_p(T)*)Listiter(voidP)::the_list();
	}
};

class List_of_piter(T) : public Const_list_of_piter(T) {
public:
	List_of_piter(T)(List_of_p(T)& l);
	List_of_piter(T)(const List_of_piter(T)& l);
	~List_of_piter(T)();
	int operator==(const List_of_piter(T)& l) const {
		return (const Const_list_of_piter(T)&)*this == ((const Const_list_of_piter(T)&)l); 
	}
	int operator!=(const List_of_piter(T)& l) const {
		return !(*this == l);
	}
	List_of_piter(T)& operator=(const List_of_piter(T)& ll) {
		return (List_of_piter(T)&)((Listiter(voidP)&)*this = (const Listiter(voidP)&)ll); 
	}
	List_of_p(T)* the_list(){
		return (List_of_p(T)*)Listiter(voidP)::the_list();
	}
	// the following operations change the container
	int remove_prev() { return Listiter(voidP)::remove_prev(); }
	int remove_next() { return Listiter(voidP)::remove_next(); }
	int remove_prev(T *&x) {
		return Listiter(voidP)::remove_prev((voidP&)x);
	}
	int remove_next(T *&x) {
		return Listiter(voidP)::remove_next((voidP&)x);
	}
	void insert_prev(T *&x) { Listiter(voidP)::insert_prev((voidP&)x); }
	void insert_next(T *&x) { Listiter(voidP)::insert_next((voidP&)x); }
	int replace_prev(T *x) {
		return Listiter(voidP)::replace_prev((voidP)x);
	}
	int replace_next(T *x) {
		return Listiter(voidP)::replace_next((voidP)x);
	}
};

PT_end

/* Note to implementers: If you change the decomposition of Listimplement(T),
*  you'll have to change voidP[0-2].c, and also ../../Path/plist[0-2].c 
*/
PT_define Listimplement(T)

Listimplement0_ATTLC(T)
Listimplement1_ATTLC(T)
Listimplement2_ATTLC(T)

PT_end

PT_define Listimplement0_ATTLC(T)

Pool*
lnnk_ATTLC(T)::pool = 0;

PT_end

PT_define Listimplement1_ATTLC(T)

lnnk_ATTLC(T)::~lnnk_ATTLC(T)() {}

lnk_ATTLC*
lnnk_ATTLC(T)::copy() {
	return new lnnk_ATTLC(T)((T&)val);
}

int
lnnk_ATTLC(T)::operator==(lnk_ATTLC& x) {
	return val == ((lnnk_ATTLC(T)*)&x)->val;
}

void* 
lnnk_ATTLC(T)::operator new(size_t) {
	return pool->alloc();
}

lnk_ATTLC *
getnewlnnk_ATTLC(T)(const T& x) {
	return (new lnnk_ATTLC(T)(x));
}
void

deletelnnk_ATTLC(T)(T& t, lnnk_ATTLC(T)* ptr) {
	t = ptr->val;
	delete ptr;
}

CCList(T)::CCList(T)(const CCList(T)& a0, const T& t) : Lizt_ATTLC((const Lizt_ATTLC&)a0) {
	put(t);
}

CCList(T)::CCList(T)() {
	lnnk_ATTLC(T)::init_pool(); 
}

CCList(T)::CCList(T)(const T& t) {
	lnnk_ATTLC(T)::init_pool();
	put(t);
}

CCList(T)::CCList(T)(const T& t, const T& u) {
	lnnk_ATTLC(T)::init_pool();
	put(t);
	put(u);
}

CCList(T)::CCList(T)(const T& t, const T& u, const T& v) {
	lnnk_ATTLC(T)::init_pool();
	put(t);
	put(u);
	put(v);
}

CCList(T)::CCList(T)(const T& t, const T& u, const T& v, const T& w) {
	lnnk_ATTLC(T)::init_pool();
	put(t);
	put(u);
	put(v);
	put(w);
}

CCList(T)::CCList(T)(const CCList(T)& a) : Lizt_ATTLC((const Lizt_ATTLC&)a) {
}

T 
CCList(T)::unput() { 
	lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Lizt_ATTLC::unput();
	T ans = ll->val; 
	delete ll; 
	return ans; 
}

T 
CCList(T)::get() { 
	lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Lizt_ATTLC::get();
	T ans = ll->val; 
	delete ll; 
	return ans; 
}

T&
CCList(T)::operator[](unsigned ii) {
	return (T&)*(getAt(ii));
}

const T&
CCList(T)::operator[](unsigned ii) const {
	return (const T&)*(((CCList(T)*)this)->getAt(ii));
}

T
CCList(T)::head() const {
	lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Lizt_ATTLC::head();
	return ll->val;
}

T
CCList(T)::tail() const {
	lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Lizt_ATTLC::tail();
	return ll->val;
}

PT_end

PT_define Listimplement2_ATTLC(T)

Const_listiter(T)::Const_listiter(T)(const CCList(T)& a) : Liztiter_ATTLC((/*const*/ Lizt_ATTLC&)a) {
}

T 
Const_listiter(T)::next() {
	lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Liztiter_ATTLC::next();
	return ll->val;
}

T 
Const_listiter(T)::prev() {
	lnnk_ATTLC(T)* ll = (lnnk_ATTLC(T)*)Liztiter_ATTLC::prev();
	return ll->val;
}

int
Const_listiter(T)::find_prev(const T& x) {
	if(at_head()|| theLizt->length()==0) return 0;
	lnnk_ATTLC(T)* iter = (lnnk_ATTLC(T)*) pred->nxt;
	register int i = index;
	do {
		iter = (lnnk_ATTLC(T)*) iter->prv;
		if (iter->val==x) {
		index = i;
		pred = iter;
		return 1;
	}
	i--;
	} while (i > 0);
	return 0;
}

int
Const_listiter(T)::find_next(const T& x) {
	if(at_end()||theLizt->length()==0) return 0;
	lnnk_ATTLC(T)* iter = (lnnk_ATTLC(T)*) pred;
	register int i = index;
	do {
		iter = (lnnk_ATTLC(T)*) iter->nxt;
		if (iter->val==x) {
		index = i;
		pred = iter->prv;
		return 1;
	}
	i++;
	} while (i < theLizt->length());
	return 0;
}

Listiter(T)::Listiter(T)(CCList(T)& a) : 
	Const_listiter(T)((const CCList(T)&)a) {
}

Listiter(T)::Listiter(T)(const Listiter(T)& a) : 
	Const_listiter(T)((const Const_listiter(T))a) {
}

int
Listiter(T)::remove_prev() {
	lnnk_ATTLC(T) *aLink = (lnnk_ATTLC(T) *)Liztiter_ATTLC::remove_prev();
	if(aLink) return (delete aLink, 1);
	else return 0;
}

int
Listiter(T)::remove_prev(T &x) {
	lnnk_ATTLC(T) *aLink = (lnnk_ATTLC(T) *)Liztiter_ATTLC::remove_prev();
	if(aLink) return (x = aLink->val,delete aLink, 1);
	else return 0;
}

int
Listiter(T)::remove_next() {
	lnnk_ATTLC(T) *aLink = (lnnk_ATTLC(T) *)Liztiter_ATTLC::remove_next();
	if(aLink) return (delete aLink, 1);
	else return 0;
}

int
Listiter(T)::remove_next(T &x) {
	lnnk_ATTLC(T) *aLink = (lnnk_ATTLC(T) *)Liztiter_ATTLC::remove_next();
	if(aLink) return (x = aLink->val, delete aLink, 1);
	else return 0;
}

Const_listiter(T)::Const_listiter(T)(const Const_listiter(T)& a) : Liztiter_ATTLC((const Liztiter_ATTLC&)a) {
}

T
Const_listiter(T)::peek_prev() const {
	return ((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_prev())->val;
}

int
Const_listiter(T)::peek_prev(T& t) const {
	if(at_head()) return 0;
	else return (t = ((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_prev())->val, 1);
}

int
Const_listiter(T)::peek_prev(T*& t) const {
	if(at_head()) return 0;
	else return (t = &((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_prev())->val, 1);
}

T
Const_listiter(T)::peek_next() const {
	return ((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_next())->val;
}

int
Const_listiter(T)::peek_next(T& t) const {
	if(at_end()) return 0;
	else return (t = ((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_next())->val, 1);
}

int
Const_listiter(T)::peek_next(T*& t) const {
	if(at_end()) return 0;
	else return (t = &((lnnk_ATTLC(T)*)Liztiter_ATTLC::peek_next())->val, 1);
}

void
CCList(T)::sort(CMPF(T) cmp) {
	if (length() < 2) return;
	voidP_List_sort_internal(*(CCList(voidP)*)this, (CMPF(voidP))cmp);
	reset_all_iters();
}

PT_end

PT_define List_of_pimplement(T)

List_of_piter(T)::List_of_piter(T)(List_of_p(T)& l) : 
	Const_list_of_piter(T)((const List_of_p(T)&)l) {
}

List_of_piter(T)::List_of_piter(T)(const List_of_piter(T)& l) : 
	Const_list_of_piter(T)((Const_list_of_piter(T))l) {
}

List_of_piter(T)::~List_of_piter(T)() {
}

List_of_p(T)::List_of_p(T)(const T* x) : CCList(voidP)((voidP) x) {
}

List_of_p(T)::List_of_p(T)(const T* x, const T* y) :
	CCList(voidP)((voidP) x, (voidP) y) {
}

List_of_p(T)::List_of_p(T)(const T* x, const T* y, const T* z) :
	CCList(voidP)((voidP)x, (voidP)y, (voidP)z) {
}

List_of_p(T)::List_of_p(T)(const T* x, const T* y, const T* z, const T* w) :
	CCList(voidP)((voidP) x, (voidP) y, (voidP) z, (voidP) w) {
}

Const_list_of_piter(T)::Const_list_of_piter(T)(const List_of_p(T)& l) : Listiter(voidP)((/*const*/ CCList(voidP)&) l) {
}

Const_list_of_piter(T)::Const_list_of_piter(T)(const Const_list_of_piter(T)& l) : Listiter(voidP)((const Listiter(voidP)&) l) {
}

List_of_p(T) List_of_p(T)::operator+(const List_of_p(T)& ll) {
	return (List_of_p(T)&) ((CCList(voidP)&)*this + (CCList(voidP)&)ll); 
}

List_of_p(T) List_of_p(T)::operator+(const T* t) {
	return (List_of_p(T)&) ((CCList(voidP)&)*this + (voidP)t); 
}

T*&
List_of_p(T)::operator[](unsigned ii) {
	return (T*&)*getAt(ii);
}

const T*&
List_of_p(T)::operator[](unsigned ii) const {
	return (const T*&)*((List_of_p(T)*)this)->getAt(ii);
}

PT_end

PT_define Listout(T)

ostream&
operator<<(ostream& oo, const CCList(T)& ll) {
	int first = 1;
	oo << "( ";
	Const_listiter(T) l(ll);
	while (!l.at_end()) {
		if (!first)
			oo << ", ";
		first = 0;
		oo << l.next();
	}
	oo << " )";
	return oo;
}

PT_end

PT_define List_of_pout(T)

ostream&
operator<<(ostream& oo, const List_of_p(T)& ll) {
	int first = 1;
	oo << "( ";
	Const_list_of_piter(T) l(ll);
	while (!l.at_end()) {
		if (!first)
			oo << ", ";
		first = 0;
		oo << *(l.next());
	}
	oo << " )";
	return oo;
}

PT_end

#endif
