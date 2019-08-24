#include "DebugFlag.h"
#include <string.h>

#ifndef CC30
Mapimplement(String,DebugFlagP)
#endif

void
DebugFlag::add ()
{
    DebugFlagMap& m = map();
    if (m.element(name()))
    {
	DebugFlag* other = m[name()];
	fprintf(stderr, "ERROR: DebugFlag '%s' already exists"
		" this=(%s/%d) other=(%s/%d)\n", (const char*) name(), 
		(const char*) file(), line(),
		(const char*) other->file(), other->line());
    }
    else
    {
	m[name()] = this;
    }
}

void
DebugFlag::remove ()
{
    DebugFlagMap& m = map();
    m[name()] = NULL;
}

/* static */
DebugFlagMap&
DebugFlag::map()
{
    static DebugFlagMap m;
    return m;
}

/* static */
DebugFlag*
DebugFlag::find (const char* name)
{
    DebugFlagMap& m = map();
    String n (name);
    if (m.element(n))
	return m[n];
    else
	return NULL;
}

/* static */
void
DebugFlag::set (const char* flag_list, const char* sep /*= "," */)
{
    String list = flag_list;
    for (const char* tok = strtok ((char*) (const char*) list, sep);
	tok != NULL;
	tok = strtok (NULL, sep))
    {
	int value = 1;
	char name[100];
	sscanf(tok, "%[^=]=%d", name, &value);
	DebugFlag* flag = find(name);

	if (flag)
	{
	    flag->set(value);
	}
	else
	{
	    fprintf (stderr, "ERROR in DebugFlag::set(%s) : "
		"flag '%s' not found\n", flag_list, name);
	}
    }
}

/* static */
void
DebugFlag::printFlags (FILE* fp /*= stderr */)
{
    int i = 0;
    DebugFlagMapIter iter(map());
    while (++iter)
    {
	DebugFlag* f = iter.value();
	if (f == NULL) continue;

	fprintf (fp, "%s=%d (%s/%d)\n", (const char*)
		f->name(), f->value(), (const char*) f->file(), f->line());
	i++;
    }
    if (i == 0)
    {
	fprintf(fp, "There are no DebugFlags\n");
    }
}

void printDebugFlags (FILE* fp /*= stderr */)
{
    DebugFlag::printFlags(fp);
}
