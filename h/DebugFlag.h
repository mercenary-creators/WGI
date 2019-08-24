#ifndef _DEBUGFLAG_H_INCLUDED
#define _DEBUGFLAG_H_INCLUDED

#include <String.h>
#include <Map.h>
#include <stdio.h>

class DebugFlag;
typedef DebugFlag* DebugFlagP;

#ifdef CC30
typedef Map<String,DebugFlagP> DebugFlagMap;
typedef Mapiter<String,DebugFlagP> DebugFlagMapIter;
#else
Mapdeclare(String,DebugFlagP)
typedef Map(String,DebugFlagP) DebugFlagMap;
typedef Mapiter(String,DebugFlagP) DebugFlagMapIter;
#endif

class DebugFlag
{
 private:

  String priv_name;
  int priv_value;
  String priv_file;
  int priv_line;

  void add ();
  void remove ();

  static DebugFlagMap& map();

 public:

  DebugFlag (const char* name, const char* fileName, int lineNumber,
	int initial_value = 0)
  : priv_value (initial_value), priv_name (name), priv_file (fileName),
    priv_line (lineNumber)
  { add(); }
  ~DebugFlag () { remove(); }

  const String& name () const { return priv_name; }
  int value () const { return priv_value; }
  const String& file () const { return priv_file; }
  int line () const { return priv_line; }
  operator int () const { return value(); }

  void set (int value = 1) { priv_value = value; }

  static DebugFlag* find (const char* name);
	// Returns the flag with that name, or NULL if it does not exist.
  static void set (const char* flag_list, const char* sep = ",");
	// Syntax:
	//	flag_list : <entry> ( <sep> <entry> ...)
	//	entry : <name>
	//	      | <name> '=' ( '1' | '0')
	// E.g. "bla,foo=0,bar=1", will set bla and bar to 1 and foo to 0.
	// NOTE: no spaces allowed in the list.

  static void printFlags (FILE* fp = stderr);
	// Will print a list of the DebugFlags and their values.

  static int cardinality ()
  { return map().size(); }
	// Returns the number of existing DebugFlags.
  static DebugFlagMapIter iter ()
  { return DebugFlagMapIter(map()); }
	// Returns a Map iterator for iterating over all the
	//  existing DebugFlags.
};

#define DEBUG_FLAG(v,s) static DebugFlag v (s, __FILE__, __LINE__)
#define DEBUG_PRINT(v,str) if (v) fprintf(stderr, str)

extern void printDebugFlags (FILE* fp = stderr);
	// Calls DebugFlag::printFlags.
#endif
