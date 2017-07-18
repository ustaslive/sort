#include <stdio.h>
#include "measure.h"

int*	sort_bubble_int1(const int *src, int *dst, long len)
{
	int i = 0;
	int right_i = 0;
	int tmp;
	/* printf("executing %s()\n", __func__); */
	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	msr_start_timer();
	for(right_i=len-1; right_i > 0; --right_i)
	{
		for(i=0; i < right_i; i++)
		{
			msr_inc_comp(); /* IF */
			if(dst[i]>dst[i+1])
			{
				msr_inc_swap();
				
				tmp = dst[i];
				dst[i] = dst[i+1];
				dst[i+1] = tmp;
			}
		}
	}
	msr_stop_timer();
	
	return dst;
}

/* improvement: sorting stops once we had not a single swap*/
int*	sort_bubble_int2(const int *src, int *dst, long len)
{
	long i = 0;
	long right_i = 0;
	int tmp;
	long swaps_cnt = 0;
	/* printf("executing %s()\n", __func__); */
	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	msr_start_timer();
	for(right_i=len-1; right_i > 0; --right_i)
	{
		msr_inc_comp();
		swaps_cnt = 0;
		for(i=0; i < right_i; i++)
		{
			msr_inc_comp(); /* FOR */
			msr_inc_comp(); /* IF */
			
			if(dst[i]>dst[i+1])
			{
#if 0
				msr_inc_swap();	
#else
				msr_inc_asgn(3);
#endif
				tmp = dst[i];
				dst[i] = dst[i+1];
				dst[i+1] = tmp;
				swaps_cnt++;
			}
		}
		if(0 == swaps_cnt)
		{
			break;
		}
	}
	msr_stop_timer();
	
	return dst;
}

int* sort_int_wrap_b(const int *src, int *dst, long len)
{
	int	*return_dst = NULL;
	
	msr_init();
	return_dst = sort_bubble_int2(src, dst, len);

	return return_dst;
}
