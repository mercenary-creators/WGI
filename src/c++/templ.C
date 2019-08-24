#include "StringList.h"
#include "Wgi.h"
#include "WgiRubber.h"
#include "DebugFlag.h"
#include "WgiDevice.h"

StringList l1;
StringListIter l1_iter (l1);

WgiSegListMap l2;
WgiSegListMapIter l2_iter (l2);

SegprimList l3;
SegprimListIter l3_iter (l3);

WgiRubberSeqList l4;
WgiRubberSeqListIter l4_iter (l4);

WgiRubberManagerList l5;
WgiRubberManagerListIter l5_iter (l5);

WgiRubberBandList l6;
WgiRubberBandListIter l6_iter (l6);

WgiRubberSeqList l7;
WgiRubberSeqListIter l7_iter (l7);

DebugFlagMap l8;
DebugFlagMapIter l8_iter (l8);

WgiViewPortMap l9;
WgiViewPortMapIter l9_iter (l9);

extern "C"
{
    
void *ewgi_malloc(size_t size);
void *ewgi_realloc(void *mptr,size_t size);
void ewgi_free(void *mptr);

};

void *ewgi_malloc(size_t size)
{
    return NULL;
}

void *ewgi_realloc(void *mptr,size_t size)
{
    return NULL;
}

void ewgi_free(void *mptr)
{
    
}

int main ()
{
    
}
