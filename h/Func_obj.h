#ifndef FUNC_OBJ_DOT_H
#define FUNC_OBJ_DOT_H


typedef int*	ALIGN;

typedef ALIGN	GENERATOR();
typedef void	PROCEDURE(ALIGN);
typedef int	PREDICATE(ALIGN);
typedef ALIGN	TRANSFORMER(ALIGN);
typedef ALIGN	RELATION(ALIGN, ALIGN);

PT_names
	Generator(T) 		T _generator
	CCProcedure(T) 		T _procedure
	Predicate(T) 		T _predicate
	Transformer(T) 		T _transformer
	Relation(T) 		T _relation
	Gene_obj(T) 		T _gene_obj
	Proc_obj(T) 		T _proc_obj
	Pred_obj(T) 		T _pred_obj
	Tran_obj(T) 		T _tran_obj
	Rela_obj(T) 		T _rela_obj
	Genf_obj(T) 		T _genf_obj
	Prof_obj(T) 		T _prof_obj
	Pref_obj(T) 		T _pref_obj
	Traf_obj(T) 		T _traf_obj
	Relf_obj(T) 		T _relf_obj
	Pred_or_Pred_obj(T) 	T pred__or__pred_obj
	Pred_or_Pref_obj(T) 	T pred__or__pref_obj
	Pref_or_Pred_obj(T) 	T pref__or__pred_obj
	Pred_and_Pred_obj(T) 	T pred__and__pred_obj
	Pred_and_Pref_obj(T) 	T pred__and__pref_obj
	Pref_and_Pred_obj(T) 	T pref__and__pred_obj
	Tran_Tran_obj(T) 	T tran__tran_obj
	Tran_Traf_obj(T) 	T tran__traf_obj
	Traf_Tran_obj(T) 	T traf__tran_obj
	Tran_Gene_obj(T) 	T tran__gene_obj
	Tran_Genf_obj(T) 	T tran__genf_obj
	Traf_Gene_obj(T) 	T traf__gene_obj
	Proc_Tran_obj(T) 	T proc__tran_obj
	Proc_Traf_obj(T) 	T proc__traf_obj
	Prof_Tran_obj(T) 	T prof__tran_obj
	Proc_Pred_obj(T) 	T proc__pred_obj
	Proc_Pref_obj(T) 	T proc__pref_obj
	Prof_Pred_obj(T) 	T prof__pred_obj
	Pred_Gene_obj(T) 	T pred__gene_obj
	Pred_Genf_obj(T) 	T pred__genf_obj
	Pref_Gene_obj(T) 	T pref__gene_obj
	Tran_Rela_obj(T) 	T tran__rela_obj
PT_end

PT_define Function_minideclare(T)
typedef T*	Generator(T)();
typedef void	CCProcedure(T)(T*);
typedef int	Predicate(T)(T*);
typedef T*	Transformer(T)(T*);
typedef int	Relation(T)(T*, T*);
PT_end

PT_define Functiondeclare(T)
Function_minideclare(T)
class Gene_obj(T) {
	Gene_obj(T)(const Gene_obj(T)&);  /* declared but not defined */
	Gene_obj(T)&	operator=(const Gene_obj(T)&);   /* ditto */
public:
			Gene_obj(T)() {}
	virtual		~Gene_obj(T)();
	virtual T*	operator()() = 0;
};
class Proc_obj(T) {
	Proc_obj(T)(const Proc_obj(T)&);  /* declared but not defined */
	Proc_obj(T)&	operator=(const Proc_obj(T)&);   /* ditto */
public:
			Proc_obj(T)() {}
	virtual		~Proc_obj(T)();
	virtual void	operator()(T*) = 0;
};
class Pred_obj(T) {
	Pred_obj(T)(const Pred_obj(T)&);  /* declared but not defined */
	Pred_obj(T)&	operator=(const Pred_obj(T)&);   /* ditto */
public:
			Pred_obj(T)() {}
	virtual		~Pred_obj(T)();
	virtual int	operator()(T*) = 0;
};
class Tran_obj(T) {
	Tran_obj(T)(const Tran_obj(T)&);  /* declared but not defined */
	Tran_obj(T)&	operator=(const Tran_obj(T)&);   /* ditto */
public:
			Tran_obj(T)() {}
	virtual		~Tran_obj(T)();
	virtual T*	operator()(T*) = 0;
};
class Rela_obj(T) {
	Rela_obj(T)(const Rela_obj(T)&);  /* declared but not defined */
	Rela_obj(T)&	operator=(const Rela_obj(T)&);   /* ditto */
public:
			Rela_obj(T)() {}
	virtual		~Rela_obj(T)();
	virtual int	operator()(T*, T*) = 0;
};
class Genf_obj(T) : public Gene_obj(T) {
friend  Genf_obj(T) objectify(Generator(T)& p);
	Generator(T)&	f;
		Genf_obj(T)(Generator(T)& p) : f(p) {}
public:
	T*	operator()();
};
inline Genf_obj(T)
objectify(Generator(T)& p)
{
	return Genf_obj(T)(p);
}
class Prof_obj(T) : public Proc_obj(T) {
friend  Prof_obj(T) objectify(CCProcedure(T)& p);
	CCProcedure(T)&	f;
		Prof_obj(T)(CCProcedure(T)& p) : f(p) {}
public:
	void	operator()(T*);
};
inline Prof_obj(T)
objectify(CCProcedure(T)& p)
{
	return Prof_obj(T)(p);
}
class Pref_obj(T) : public Pred_obj(T) {
friend  Pref_obj(T) objectify(Predicate(T)& p);
	Predicate(T)&	f;
		Pref_obj(T)(Predicate(T)& p) : f(p) {}
public:
	int	operator()(T*);
};
inline Pref_obj(T)
objectify(Predicate(T)& p)
{
	return Pref_obj(T)(p);
}
class Traf_obj(T) : public Tran_obj(T) {
friend  Traf_obj(T) objectify(Transformer(T)& p);
	Transformer(T)&	f;
		Traf_obj(T)(Transformer(T)& p) : f(p) {}
public:
	T*	operator()(T*);
};
inline Traf_obj(T)
objectify(Transformer(T)& p)
{
	return Traf_obj(T)(p);
}
class Relf_obj(T) : public Rela_obj(T) {
friend  Relf_obj(T) objectify(Relation(T)& p);
	Relation(T)&	f;
		Relf_obj(T)(Relation(T)& p) : f(p) {}
public:
	int	operator()(T*, T*);
};
inline Relf_obj(T)
objectify(Relation(T)& p)
{
	return Relf_obj(T)(p);
}
class Pred_or_Pred_obj(T) : public Pred_obj(T) {
friend  Pred_or_Pred_obj(T) operator||(Pred_obj(T)& p, Pred_obj(T)& q);
	Pred_obj(T)&	pred1;
	Pred_obj(T)&	pred2;
		Pred_or_Pred_obj(T)(Pred_obj(T)& p, Pred_obj(T)& q) :
				pred1(p), pred2(q) {}
public:
	int	operator()(T*);
};
inline Pred_or_Pred_obj(T)
operator||(Pred_obj(T)& p, Pred_obj(T)& q)
{
	return Pred_or_Pred_obj(T)(p, q);
}
class Pred_or_Pref_obj(T) : public Pred_obj(T) {
friend  Pred_or_Pref_obj(T) operator||(Pred_obj(T)& p, Predicate(T)& q);
	Pred_obj(T)&	pred1;
	Predicate(T)&	pred2;
		Pred_or_Pref_obj(T)(Pred_obj(T)& p, Predicate(T)& q) :
				pred1(p), pred2(q) {}
public:
	int	operator()(T*);
};
inline Pred_or_Pref_obj(T)
operator||(Pred_obj(T)& p, Predicate(T)& q)
{
	return Pred_or_Pref_obj(T)(p, q);
}
class Pref_or_Pred_obj(T) : public Pred_obj(T) {
friend  Pref_or_Pred_obj(T) operator||(Predicate(T)& p, Pred_obj(T)& q);
	Predicate(T)&	pred1;
	Pred_obj(T)&	pred2;
		Pref_or_Pred_obj(T)(Predicate(T)& p, Pred_obj(T)& q) :
				pred1(p), pred2(q) {}
public:
	int	operator()(T*);
};
inline Pref_or_Pred_obj(T)
operator||(Predicate(T)& p, Pred_obj(T)& q)
{
	return Pref_or_Pred_obj(T)(p, q);
}
class Pred_and_Pred_obj(T) : public Pred_obj(T) {
friend  Pred_and_Pred_obj(T) operator&&(Pred_obj(T)& p, Pred_obj(T)& q);
	Pred_obj(T)&	pred1;
	Pred_obj(T)&	pred2;
		Pred_and_Pred_obj(T)(Pred_obj(T)& p, Pred_obj(T)& q) :
				pred1(p), pred2(q) {}
public:
	int	operator()(T*);
};
inline Pred_and_Pred_obj(T)
operator&&(Pred_obj(T)& p, Pred_obj(T)& q)
{
	return Pred_and_Pred_obj(T)(p, q);
}
class Pred_and_Pref_obj(T) : public Pred_obj(T) {
friend  Pred_and_Pref_obj(T) operator&&(Pred_obj(T)& p, Predicate(T)& q);
	Pred_obj(T)&	pred1;
	Predicate(T)&	pred2;
		Pred_and_Pref_obj(T)(Pred_obj(T)& p, Predicate(T)& q) :
				pred1(p), pred2(q) {}
public:
	int	operator()(T*);
};
inline Pred_and_Pref_obj(T)
operator&&(Pred_obj(T)& p, Predicate(T)& q)
{
	return Pred_and_Pref_obj(T)(p, q);
}
class Pref_and_Pred_obj(T) : public Pred_obj(T) {
friend  Pref_and_Pred_obj(T) operator&&(Predicate(T)& p, Pred_obj(T)& q);
	Predicate(T)&	pred1;
	Pred_obj(T)&	pred2;
		Pref_and_Pred_obj(T)(Predicate(T)& p, Pred_obj(T)& q) :
				pred1(p), pred2(q) {}
public:
	int	operator()(T*);
};
inline Pref_and_Pred_obj(T)
operator&&(Predicate(T)& p, Pred_obj(T)& q)
{
	return Pref_and_Pred_obj(T)(p, q);
}
class Tran_Tran_obj(T) : public Tran_obj(T) {
friend  Tran_Tran_obj(T) operator*(Tran_obj(T)& p, Tran_obj(T)& q);
	Tran_obj(T)&	tran1;
	Tran_obj(T)&	tran2;
		Tran_Tran_obj(T)(Tran_obj(T)& p, Tran_obj(T)& q) :
				tran1(p), tran2(q) {}
public:
	T*	operator()(T*);
};
inline Tran_Tran_obj(T)
operator*(Tran_obj(T)& p, Tran_obj(T)& q)
{
	return Tran_Tran_obj(T)(p, q);
}
class Tran_Traf_obj(T) : public Tran_obj(T) {
friend  Tran_Traf_obj(T) operator*(Tran_obj(T)& p, Transformer(T)& q);
	Tran_obj(T)&	tran1;
	Transformer(T)&	tran2;
		Tran_Traf_obj(T)(Tran_obj(T)& p, Transformer(T)& q) :
				tran1(p), tran2(q) {}
public:
	T*	operator()(T*);
};
inline Tran_Traf_obj(T)
operator*(Tran_obj(T)& p, Transformer(T)& q)
{
	return Tran_Traf_obj(T)(p, q);
}
class Traf_Tran_obj(T) : public Tran_obj(T) {
friend  Traf_Tran_obj(T) operator*(Transformer(T)& p, Tran_obj(T)& q);
	Transformer(T)&	tran1;
	Tran_obj(T)&	tran2;
		Traf_Tran_obj(T)(Transformer(T)& p, Tran_obj(T)& q) :
				tran1(p), tran2(q) {}
public:
	T*	operator()(T*);
};
inline Traf_Tran_obj(T)
operator*(Transformer(T)& p, Tran_obj(T)& q)
{
	return Traf_Tran_obj(T)(p, q);
}
class Tran_Gene_obj(T) : public Gene_obj(T) {
friend  Tran_Gene_obj(T) operator*(Tran_obj(T)& p, Gene_obj(T)& q);
	Tran_obj(T)&	tran;
	Gene_obj(T)&	gene;
		Tran_Gene_obj(T)(Tran_obj(T)& p, Gene_obj(T)& q) :
				tran(p), gene(q) {}
public:
	T*	operator()();
};
inline Tran_Gene_obj(T)
operator*(Tran_obj(T)& p, Gene_obj(T)& q)
{
	return Tran_Gene_obj(T)(p, q);
}
class Tran_Genf_obj(T) : public Gene_obj(T) {
friend  Tran_Genf_obj(T) operator*(Tran_obj(T)& p, Generator(T)& q);
	Tran_obj(T)&	tran;
	Generator(T)&	gene;
		Tran_Genf_obj(T)(Tran_obj(T)& p, Generator(T)& q) :
				tran(p), gene(q) {}
public:
	T*	operator()();
};
inline Tran_Genf_obj(T)
operator*(Tran_obj(T)& p, Generator(T)& q)
{
	return Tran_Genf_obj(T)(p, q);
}
class Traf_Gene_obj(T) : public Gene_obj(T) {
friend  Traf_Gene_obj(T) operator*(Transformer(T)& p, Gene_obj(T)& q);
	Transformer(T)&	tran;
	Gene_obj(T)&	gene;
		Traf_Gene_obj(T)(Transformer(T)& p, Gene_obj(T)& q) :
				tran(p), gene(q) {}
public:
	T*	operator()();
};
inline Traf_Gene_obj(T)
operator*(Transformer(T)& p, Gene_obj(T)& q)
{
	return Traf_Gene_obj(T)(p, q);
}
class Proc_Tran_obj(T) : public Proc_obj(T) {
friend  Proc_Tran_obj(T) operator*(Proc_obj(T)& p, Tran_obj(T)& q);
	Proc_obj(T)&	proc;
	Tran_obj(T)&	tran;
		Proc_Tran_obj(T)(Proc_obj(T)& p, Tran_obj(T)& q) :
				proc(p), tran(q) {}
public:
	void	operator()(T*);
};
inline Proc_Tran_obj(T)
operator*(Proc_obj(T)& p, Tran_obj(T)& q)
{
	return Proc_Tran_obj(T)(p, q);
}
class Proc_Traf_obj(T) : public Proc_obj(T) {
friend  Proc_Traf_obj(T) operator*(Proc_obj(T)& p, Transformer(T)& q);
	Proc_obj(T)&	proc;
	Transformer(T)&	tran;
		Proc_Traf_obj(T)(Proc_obj(T)& p, Transformer(T)& q) :
				proc(p), tran(q) {}
public:
	void	operator()(T*);
};
inline Proc_Traf_obj(T)
operator*(Proc_obj(T)& p, Transformer(T)& q)
{
	return Proc_Traf_obj(T)(p, q);
}
class Prof_Tran_obj(T) : public Proc_obj(T) {
friend  Prof_Tran_obj(T) operator*(CCProcedure(T)& p, Tran_obj(T)& q);
	CCProcedure(T)&	proc;
	Tran_obj(T)&	tran;
		Prof_Tran_obj(T)(CCProcedure(T)& p, Tran_obj(T)& q) :
				proc(p), tran(q) {}
public:
	void	operator()(T*);
};
inline Prof_Tran_obj(T)
operator*(CCProcedure(T)& p, Tran_obj(T)& q)
{
	return Prof_Tran_obj(T)(p, q);
}
class Proc_Pred_obj(T) : public Proc_obj(T) {
friend  Proc_Pred_obj(T) operator*(Proc_obj(T)& p, Pred_obj(T)& q);
	Proc_obj(T)&	proc;
	Pred_obj(T)&	pred;
		Proc_Pred_obj(T)(Proc_obj(T)& p, Pred_obj(T)& q) :
				proc(p), pred(q) {}
public:
	void	operator()(T*);
};
inline Proc_Pred_obj(T)
operator*(Proc_obj(T)& p, Pred_obj(T)& q)
{
	return Proc_Pred_obj(T)(p, q);
}
class Proc_Pref_obj(T) : public Proc_obj(T) {
friend  Proc_Pref_obj(T) operator*(Proc_obj(T)& p, Predicate(T)& q);
	Proc_obj(T)&	proc;
	Predicate(T)&	pred;
		Proc_Pref_obj(T)(Proc_obj(T)& p, Predicate(T)& q) :
				proc(p), pred(q) {}
public:
	void	operator()(T*);
};
inline Proc_Pref_obj(T)
operator*(Proc_obj(T)& p, Predicate(T)& q)
{
	return Proc_Pref_obj(T)(p, q);
}
class Prof_Pred_obj(T) : public Proc_obj(T) {
friend  Prof_Pred_obj(T) operator*(CCProcedure(T)& p, Pred_obj(T)& q);
	CCProcedure(T)&	proc;
	Pred_obj(T)&	pred;
		Prof_Pred_obj(T)(CCProcedure(T)& p, Pred_obj(T)& q) :
				proc(p), pred(q) {}
public:
	void	operator()(T*);
};
inline Prof_Pred_obj(T)
operator*(CCProcedure(T)& p, Pred_obj(T)& q)
{
	return Prof_Pred_obj(T)(p, q);
}
class Pred_Gene_obj(T) : public Gene_obj(T) {
friend  Pred_Gene_obj(T) operator*(Pred_obj(T)& p, Gene_obj(T)& g);
	Pred_obj(T)&	pred;
	Gene_obj(T)&	gene;
		Pred_Gene_obj(T)(Pred_obj(T)& p, Gene_obj(T)& g) :
				pred(p), gene(g) {}
public:
	T*	operator()();
};
inline Pred_Gene_obj(T)
operator*(Pred_obj(T)& p, Gene_obj(T)& g)
{
	return Pred_Gene_obj(T)(p, g);
}
class Pred_Genf_obj(T) : public Gene_obj(T) {
friend  Pred_Genf_obj(T) operator*(Pred_obj(T)& p, Generator(T)& g);
	Pred_obj(T)&	pred;
	Generator(T)&	gene;
		Pred_Genf_obj(T)(Pred_obj(T)& p, Generator(T)& g) :
				pred(p), gene(g) {}
public:
	T*	operator()();
};
inline Pred_Genf_obj(T)
operator*(Pred_obj(T)& p, Generator(T)& g)
{
	return Pred_Genf_obj(T)(p, g);
}
class Pref_Gene_obj(T) : public Gene_obj(T) {
friend  Pref_Gene_obj(T) operator*(Predicate(T)& p, Gene_obj(T)& g);
	Predicate(T)&	pred;
	Gene_obj(T)&	gene;
		Pref_Gene_obj(T)(Predicate(T)& p, Gene_obj(T)& g) :
				pred(p), gene(g) {}
public:
	T*	operator()();
};
inline Pref_Gene_obj(T)
operator*(Predicate(T)& p, Gene_obj(T)& g)
{
	return Pref_Gene_obj(T)(p, g);
}
class Tran_Rela_obj(T) : public Rela_obj(T) {
friend  Tran_Rela_obj(T) operator*(Tran_obj(T)& t, Rela_obj(T)& r);
	Tran_obj(T)&	tran;
	Rela_obj(T)&	rela;
		Tran_Rela_obj(T)(Tran_obj(T)& t, Rela_obj(T)& r) :
				tran(t), rela(r) {}
public:
	int	operator()(T*, T*);
};
inline Tran_Rela_obj(T)
operator*(Tran_obj(T)& t, Rela_obj(T)& r)
{
	return Tran_Rela_obj(T)(t, r);
}
PT_end

PT_define Functionimplement(T)
Gene_obj(T)::~Gene_obj(T)() {}
Proc_obj(T)::~Proc_obj(T)() {}
Pred_obj(T)::~Pred_obj(T)() {}
Tran_obj(T)::~Tran_obj(T)() {}
Rela_obj(T)::~Rela_obj(T)() {}
T*
Genf_obj(T)::operator()()
{
	return f();
}
void
Prof_obj(T)::operator()(T* tp)
{
	f(tp);
}
int
Pref_obj(T)::operator()(T* tp)
{
	return f(tp);
}
T*
Traf_obj(T)::operator()(T* tp)
{
	return f(tp);
}
int
Relf_obj(T)::operator()(T* tp1, T* tp2)
{
	return f(tp1, tp2);
}
int
Pred_or_Pred_obj(T)::operator()(T* tp)
{
	return pred1(tp) ? 1 : pred2(tp);
}
int
Pred_or_Pref_obj(T)::operator()(T* tp)
{
	return pred1(tp) ? 1 : pred2(tp);
}
int
Pref_or_Pred_obj(T)::operator()(T* tp)
{
	return pred1(tp) ? 1 : pred2(tp);
}
int
Pred_and_Pred_obj(T)::operator()(T* tp)
{
	return pred1(tp) ? pred2(tp) : 0;
}
int
Pred_and_Pref_obj(T)::operator()(T* tp)
{
	return pred1(tp) ? pred2(tp) : 0;
}
int
Pref_and_Pred_obj(T)::operator()(T* tp)
{
	return pred1(tp) ? pred2(tp) : 0;
}
T*
Tran_Tran_obj(T)::operator()(T* tp)
{
	T*	tp1 = tran2(tp);
	return tp1 ? tran1(tp1) : 0;
}
T*
Tran_Traf_obj(T)::operator()(T* tp)
{
	T*	tp1 = tran2(tp);
	return tp1 ? tran1(tp1) : 0;
}
T*
Traf_Tran_obj(T)::operator()(T* tp)
{
	T*	tp1 = tran2(tp);
	return tp1 ? tran1(tp1) : 0;
}
T*
Tran_Gene_obj(T)::operator()()
{
	T*	tp1 = gene();
	return tp1 ? tran(tp1) : 0;
}
T*
Tran_Genf_obj(T)::operator()()
{
	T*	tp1 = gene();
	return tp1 ? tran(tp1) : 0;
}
T*
Traf_Gene_obj(T)::operator()()
{
	T*	tp1 = gene();
	return tp1 ? tran(tp1) : 0;
}
void
Proc_Tran_obj(T)::operator()(T* tp)
{
	T*	tp1 = tran(tp);
	if(tp1) proc(tp1);
}
void
Proc_Traf_obj(T)::operator()(T* tp)
{
	T*	tp1 = tran(tp);
	if(tp1) proc(tp1);
}
void
Prof_Tran_obj(T)::operator()(T* tp)
{
	T*	tp1 = tran(tp);
	if(tp1) proc(tp1);
}
void
Proc_Pred_obj(T)::operator()(T* tp)
{
	if (pred(tp))
		proc(tp);
}
void
Proc_Pref_obj(T)::operator()(T* tp)
{
	if (pred(tp))
		proc(tp);
}
void
Prof_Pred_obj(T)::operator()(T* tp)
{
	if (pred(tp))
		proc(tp);
}
T*
Pred_Gene_obj(T)::operator()()
{
	for (;;) {
		register T*	ans = gene();
		if (ans == 0)
			return 0;
		if (pred(ans))
			return ans;
	}
}
T*
Pred_Genf_obj(T)::operator()()
{
	for (;;) {
		register T*	ans = gene();
		if (ans == 0)
			return 0;
		if (pred(ans))
			return ans;
	}
}
T*
Pref_Gene_obj(T)::operator()()
{
	for (;;) {
		register T*	ans = gene();
		if (ans == 0)
			return 0;
		if (pred(ans))
			return ans;
	}
}
int
Tran_Rela_obj(T)::operator()(T* tp1, T* tp2)
{
	return rela(tran(tp1), tp2);
}
PT_end

#endif
