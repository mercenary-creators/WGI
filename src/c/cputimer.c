/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  A Couple Of Utility Routines Used To Measure Real And CPU Time For   --*/
/*--          Benchmarking Improvements In The Graphics Routines           --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>

#define TICKS_PER_SEC	60
#define TIME_NONE	-1

static struct tms s_cpu;
static struct tms e_cpu;

static long s_time = TIME_NONE;
static long e_time = TIME_NONE;

static char line[512];

extern long times();

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--         Set The CPU Timer And Remember The Current Time Values        --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void set_cputimer()
{
	time(&s_time);
	times(&s_cpu);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--   Stop The CPU Timer And Derive The Time Interval By Calculating The  --*/
/*--           Delta Between The Current Time And The Previous Time        --*/
/*--                                                                       --*/
/*--     Producer: Dean Jones      Inspected: 01/91     Coverage: 000%     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

char *end_cputimer(show)
int show;
{
	long real;
	long tcpu;
	long mcpu;
	float scpu;
	float totl;

	if (s_time != TIME_NONE)
	{

		time(&e_time);

		real = e_time - s_time;

		times(&e_cpu);

		tcpu = (e_cpu.tms_utime + e_cpu.tms_stime) -
			(s_cpu.tms_utime + s_cpu.tms_stime);

		mcpu = tcpu / TICKS_PER_SEC / TICKS_PER_SEC;

		scpu = ((float) tcpu - (float) mcpu *
			(float) TICKS_PER_SEC * (float) TICKS_PER_SEC) /
			(float) TICKS_PER_SEC;

		totl = (float) tcpu / (float) TICKS_PER_SEC;

		sprintf(line,
			"Real: %d:%02d = %d sec; CPU: %d:%05.2f = %.2f sec",
			real / TICKS_PER_SEC, real % TICKS_PER_SEC,
			real, mcpu, scpu, totl);

		if (show)
			fprintf(stderr, "\n%s\n", line);
	}
	s_time = TIME_NONE;
	return (line);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
