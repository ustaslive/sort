#include <stdio.h>
#include <string.h>
#include <time.h>
#include "measure.h"

measure_t	stat;
clock_t		start_time;

void	msr_init(void)
{
	memset(&stat, 0, sizeof(stat));
}

void	msr_inc_depth(void)
{
	stat.curr_depth++;
	if(stat.curr_depth > stat.depth)
		stat.depth = stat.curr_depth;
}

void	msr_dec_depth(void)
{
	stat.curr_depth--;
}

void	msr_inc_comp(void)
{
	stat.comp++;
}

void	msr_inc_swap(void)
{
	stat.swap++;
}

void	msr_inc_asgn(int inc)
{
	stat.asgn += inc;
}

void	msr_inc_copy(void)
{
	stat.copy++;
}

void	msr_inc_call(void)
{
	stat.call++;
}

void	msr_start_timer(void)
{
	start_time = clock();
}

void	msr_stop_timer(void)
{
	clock_t stop_time = clock();
	stat.time = ((double) (stop_time - start_time)) / CLOCKS_PER_SEC;
}

void	msr_dump(void)
{
#if LONG_OUTPUT
	printf("Execution statistics\n");
	printf("Comparisons: %lu\n", stat.comp);
	printf("Swaps: %lu\n", stat.swap);
	printf("Copies: %lu\n", stat.copy);
#else
#if PRINT_SWAPS
	printf("swap/comp/call/depth/time:\t[%10lu %10lu %10lu %10lu %f]\n", stat.swap, stat.comp, stat.call, stat.depth, stat.time);
#else
	printf("asgn/comp/call/depth/time:\t[%10lu %10lu %10lu %10lu %f]\n", stat.asgn, stat.comp, stat.call, stat.depth, stat.time);
#endif
#endif
}