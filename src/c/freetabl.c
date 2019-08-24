#include <memory.h>

#include "wgiStruct.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                Free A Range Of Elements In A Table                    --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_table_free(tptr, indx, nelm)
Table *tptr;
unsigned long indx;
unsigned long nelm;
{
	long used;
	Tnode *node;
	Tnode *back;
	Tnode *this;

	/*
		Check The Table Pointer
	*/

	if(!tptr)
	{
		WOOPS("The free table pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		A Zero Element Free Is An Error
	*/

	if(nelm == 0)
	{
		WOOPS("Asked to free zero elements");
		return (WGI_PROBLEM);
	}

	/*
		Make Sure Elements Are Within Known Range
	*/

	if((indx + nelm) > tptr->nelm)
	{
		WOOPS("The freed element extends beyond free table");
		return (WGI_PROBLEM);
	}

	/*
		Clear Out Returned Memory Block For Future Use
	*/

	if(iwgi_zero_flag)
		memset(tptr->data+(tptr->elsz*indx), 0, (tptr->elsz*nelm));

	/*
		Allocate A Node Structure For This Free Area
	*/

	if(this = WGI_MALLOC(1, Tnode))
	{
		this->indx = indx;
		this->nelm = nelm;
		this->next = NULL;
		this->back = NULL;

		back = NULL;
		node = tptr->node;

		/*
			Find The Spot In The List Where The Free Node Goes
			And Insert In Ascending Order Of Indexes
		*/

		while((node) && ((node->indx + node->nelm) <= this->indx))
		{
			back = node;
			node = node->next;
		}

		/*
			Make Sure Free Node Does Not Overlap Next Node
		*/

		if((node) && ((this->indx + this->nelm) > node->indx))
		{
			WGI_FREE(this);
			WOOPS("The free table node overlaps next node");
			return (WGI_PROBLEM);
		}

		/*
			Make Sure Free Node Does Not Overlap Back Node
		*/

		if((back) && ((back->indx + back->nelm) > this->indx))
		{
			WGI_FREE(this);
			WOOPS("The free table node overlaps back node");
			return (WGI_PROBLEM);
		}

		/*
			Insert Free Node Somewhere In The List
		*/

		if(node)
		{
			this->back = back;
			this->next = node;
			node->back = this;
			if(back)
			{
				back->next = this;
			}
			else
			{
				tptr->node = this;
			}
		}
		else if(back)
		{
			/*
				Insert Free Node At Tail Of List
			*/

			back->next = this;
			this->back = back;
		}
		else
		{
			/*
				Insert Free Node As The First In The List
			*/

			tptr->node = this;
		}

		/*
			Mark The Possible Used High Mark
		*/

		used = indx;

		/*
			Check To See If Node Can Be Merged With Next Node
		*/

		if(node = this->next)
		{
			if((this->indx + this->nelm) >= node->indx)
			{
				this->nelm = this->nelm + node->nelm;
	
				if(this->next = node->next)
				{
					this->next->back = this;
				}
				WGI_FREE(node);
			}
		}

		/*
			Check To See If Node Can Be Merged With Back Node
		*/

		if(node = this->back)
		{
			if((node->indx + node->nelm) >= this->indx)
			{
				/*
					Used High Mark Is Lowered
				*/

				used = node->indx;

				node->nelm = node->nelm + this->nelm;
	
				if(node->next = this->next)
				{
					node->next->back = node;
				}
				WGI_FREE(this);
			}
		}

		/*
			Update Used High Mark
		*/

		if((indx + nelm) == tptr->used)
		{
			tptr->used = used;
		}

		return (WGI_SUCCESS);
	}

	WOOPS("Not enough memory to allocate free table node");
	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- An Internal Routine To Find Space For A Number Of Elements In A Table --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static long wgi_table_look(tptr, indx, nelm)
Table *tptr;
unsigned long *indx;
unsigned long nelm;
{
	long stop;
	Tnode *node;
	Tnode *this;

	/*
		Check The Table Pointer
	*/

	if(!tptr)
	{
		WOOPS("The free table pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Set Conditional Variables And Look For A Matching Node
	*/

	this = NULL;
	stop = FALSE;
	for(node = tptr->node; node && !stop; node = node->next)
	{
		/*
			Node Is Big Enough
		*/

		if(node->nelm >= nelm)
		{
			/*
				Node Is Exactly Right So Stop
			*/

			if(node->nelm == nelm)
			{
				this = node;
				stop = TRUE;
			}
			else if(this)
			{
				/*
					Is Node Smaller Than The One We Have
				*/

				if(node->nelm < this->nelm)
				{
					this = node;
				}
			}
			else
			{
				/*
					This Is The First Node That Is Big Enough
				*/

				this = node;
			}
		}
	}

	/*
		If We Have A Matching Node
	*/

	if(this)
	{
		if(this->nelm == nelm)
		{
			/*
				If It Is Exact Use The Whole Thing And Delete Node
			*/

			*indx = this->indx;
			if(this->back)
			{
				this->back->next = this->next;
			}
			else
			{
				tptr->node = this->next;
			}
			if(this->next)
			{
				this->next->back = this->back;
			}
			WGI_FREE(this);
		}
		else
		{
			/*
				Use Part Of A Node And Shrink Node
			*/

			*indx = this->indx;
			this->indx = this->indx + nelm;
			this->nelm = this->nelm - nelm;
		}

		/*
			Set The Used Element Mark For The Table
		*/

		if((*indx + nelm) > tptr->used)
		{
			tptr->used = (*indx + nelm);
		}

		return (WGI_SUCCESS);
	}

	/*
		Dont Print Error Message Cause We Can Allocate More Memory
	*/

	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--     Initialize A Table By Allocating Some Starting Memory For It      --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_table_init(tptr, elsz, nelm, grow)
Table *tptr;
unsigned long elsz;
unsigned long nelm;
unsigned long grow;
{
	long rc;

	/*
		Check The Table Pointer
	*/

	if(!tptr)
	{
		WOOPS("The free table pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Initialize The Table Structure
	*/

	tptr->used = 0;
	tptr->sync = 0;
	tptr->elsz = elsz;
	tptr->grow = grow;
	tptr->nelm = nelm;
	tptr->node = NULL;

	/*
		Allocate And Free The Initial Block Of Memory
	*/

	if(tptr->data = WGI_MALLOC((elsz * nelm), char))
	{
		if((rc = wgi_table_free(tptr, 0, nelm)) != WGI_SUCCESS)
			return (rc);

		return (WGI_SUCCESS);
	}

	WOOPS("Not enough memory to allocate free table buffer");
	return (WGI_PROBLEM);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--   Find Or Allocate Enough Space For A Number Of Elements In A Table   --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_table_aloc(tptr, indx, nelm)
Table *tptr;
unsigned long *indx;
unsigned long nelm;
{
	long rc;
	char *temp;
	unsigned long last;
	unsigned long grow;

	/*
		Check The Table Pointer
	*/

	if(!tptr)
	{
		WOOPS("The free table pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Make Sure Request Is Not Zero
	*/

	if(nelm == 0)
	{
		WOOPS("Asked for zero free table elements");
		return (WGI_PROBLEM);
	}

	/*
		Make Sure Request Is Semi-Sane
	*/

	if(nelm > WGI_TABLE_MAX_CHUNK)
	{
		WOOPS("Asked for more than %u free table elements", WGI_TABLE_MAX_CHUNK);
		return (WGI_PROBLEM);
	}

	/*
		Search For An Already Free Node
	*/

	if((rc = wgi_table_look(tptr, indx, nelm)) != WGI_SUCCESS)
	{
		/*
			No Big Enough Node So Allocate More
		*/

		last = tptr->nelm;
		grow = nint((double)last * ((double)tptr->grow / 100.0));
		grow = MAX(grow, nelm);

		/*
			Extend The Table By Either The Grow Percentage Or The
			Number Of Slots To Be Allocated,  Whichever Is Bigger
		*/

		if(temp = WGI_REALLOC(tptr->data, (last+grow)*tptr->elsz, char))
		{
			tptr->data = temp;
			tptr->nelm = (last + grow);

			/*
				Free The New Memory Into The Node List
			*/

			if((rc = wgi_table_free(tptr, last, grow)) != WGI_SUCCESS)
				return (rc);

			/*
				If We Dont Have Enough Now We Have Real Problems
			*/

			if((rc = wgi_table_look(tptr, indx, nelm)) != WGI_SUCCESS)
			{
				WOOPS("Can't find a free table node after allocate");
				return (rc);
			}

			return (WGI_SUCCESS);
		}

		WOOPS("Not enough memory to extend free table buffer");
		return (WGI_PROBLEM);
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- A Utility Routine To Dump A Table On The Error File ( For Debugging ) --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_table_dump(tptr)
Table *tptr;
{
	Tnode *node;

	/*
		Check The Table Pointer
	*/

	if(!tptr)
	{
		WOOPS("The free table pointer is null");
		return (WGI_PROBLEM);
	}

	SAYIT("DUMP: used mark %u\n\n", tptr->used);

	for(node = tptr->node; node; node = node->next)
	{
		SAYIT("DUMP: %5d %5d\n", node->indx, (node->indx + node->nelm - 1));
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--             A Simple Memory Buffer Initialization Routine             --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_tbuff_init(tbuf)
Tbuff *tbuf;
{
	/*
		Check The Buffer Pointer
	*/

	if(!tbuf)
	{
		WOOPS("The buffer pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Initialize Our Variables To All Zero
	*/

	tbuf->sync = 0;
	tbuf->size = 0;
	tbuf->data = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--      A Simple Memory Buffer Expand To A Specified Size Routine        --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_tbuff_match(tbuf, size)
Tbuff *tbuf;
long size;
{
	char *data;

	/*
		Check The Buffer Pointer
	*/

	if(!tbuf)
	{
		WOOPS("The buffer pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Check If The Buffer Needs To Be Made Larger
	*/

	if(size > tbuf->size)
	{
		/*
			If There Was A Previous Chunk Of Allocated Memory
			Then Free It
		*/

		if(tbuf->data)
			WGI_FREE(tbuf->data);

		/*
			Try To Allocate The A Chunk Of Memory But Use The 
			Temporary Variable So We Dont Hoze Up The Current
			Buffer
		*/

		if(data = WGI_MALLOC(size, char))
		{
			tbuf->size = size;

			if(iwgi_zero_flag)
			{
				tbuf->data = memset(data, 0, size);
			}
			else
			{
				tbuf->data = data;
			}

			return (WGI_SUCCESS);
		}

		/*
			Oh No - Ran Out Of Memory - I Hope Everyone Above
			This Function Checks Return Codes!!!
		*/

		WOOPS("Not enough memory to allocate buffer");
		return (WGI_PROBLEM);
	}

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--              Free All The Memory Associated With A Buffer             --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

long wgi_tbuff_free(tbuf)
Tbuff *tbuf;
{
	/*
		Check The Buffer Pointer
	*/

	if(!tbuf)
	{
		WOOPS("The buffer pointer is null");
		return (WGI_PROBLEM);
	}

	/*
		Free The Chunk Of Memory Associated With The Buffer
	*/

	if(tbuf->data)
		WGI_FREE(tbuf->data);

	/*
		Re-Initialize Our Variables To All Zero
	*/

	tbuf->sync = 0;
	tbuf->size = 0;
	tbuf->data = NULL;

	return (WGI_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
