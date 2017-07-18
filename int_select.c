#include <stdio.h>
#include <limits.h>
#include "measure.h"

int*	sort_select_int(const int *src, int *dst, long len)
{
	long i = 0;
	long right_i = 0;
	long left_i = 0;
	int min_el = 0;
	long min_i = 0;
	int max_el = INT_MAX;
	long max_i = 0;

	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	msr_start_timer();
	for(left_i = 0, right_i=len-1; left_i < right_i;)
	{
		/* printf("left_i/right_i: [%ld/%ld]\n", left_i, right_i); */
		
		max_i = left_i;
		max_el = dst[max_i];
		msr_inc_swap();	

		for(i=left_i+1; i <= right_i; i++)
		{
			msr_inc_comp();		/* IF */
			if(max_el < dst[i])
			{
				msr_inc_swap();
				max_i = i;
				max_el = dst[max_i];
				/* printf("idx[%ld]: new max_el/max_i: [%d/%ld]\n", i, max_el, max_i); */
			}
		}
		
		if(max_i != right_i)
		{
			/* printf("swap max_el/max_i with dst[right_i]: [%d/%ld - %d/%ld]\n", max_el, max_i, dst[right_i], right_i); */
			dst[max_i] = dst[right_i];
			dst[right_i] = max_el;
			msr_inc_swap();
		}

		right_i--;
		
		min_i = left_i;
		min_el = dst[min_i];
		msr_inc_swap();
		
		for(i=left_i+1; i <= right_i; i++)
		{
			msr_inc_comp();		/* IF */
			if(min_el > dst[i])
			{
				min_i = i;
				min_el = dst[min_i];
				msr_inc_swap();
				/* printf("idx[%ld]: new min_el/min_i: [%d/%ld]\n", i, min_el, min_i); */
			}
		}
		if(min_i != left_i)
		{
			/* printf("swap min_el/min_i with dst[left_i]: [%d/%ld - %d/%ld]\n", min_el, min_i, dst[left_i], left_i); */
			dst[min_i] = dst[left_i];
			dst[left_i] = min_el;
			msr_inc_swap();
		}

		left_i++;
	}
	msr_stop_timer();
	
	return dst;
}

int*	sort_select_int_short(const int *src, int *dst, long len)
{
	long i = 0;
	long right_i = 0;
	long left_i = 0;
	int max_el = INT_MAX;
	long max_i = 0;

	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	msr_start_timer();
	for(left_i = 0, right_i=len-1; left_i < right_i;)
	{
		max_i = left_i;
		max_el = dst[max_i];
#if 0
		msr_inc_swap();	
#else
		msr_inc_asgn(1);
#endif		

		for(i=left_i+1; i <= right_i; i++)
		{
			msr_inc_comp();		/* IF */
			if(max_el < dst[i])
			{
#if 0
				msr_inc_swap();	
#else
				msr_inc_asgn(2);
#endif
				max_i = i;
				max_el = dst[max_i];
			}
		}
		
		if(max_i != right_i)
		{
			dst[max_i] = dst[right_i];
			dst[right_i] = max_el;
#if 0
			msr_inc_swap();	
#else
			msr_inc_asgn(2);
#endif
		}

		right_i--;	
	}
	msr_stop_timer();
	
	return dst;
}

int*	sort_select_int_classic(const int *src, int *dst, long len)
{
	long i = 0;
	long bottom_i = 0;
	long top_i = len-1;
	long min_i = 0;
	int	tmp;

	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	msr_start_timer();
	for(bottom_i = 0; bottom_i < top_i; bottom_i++)
	{
		min_i = bottom_i;
#if 0
		msr_inc_swap();	
#else
		msr_inc_asgn(1);
#endif		

		for(i=bottom_i+1; i <= top_i; i++)
		{
			msr_inc_comp();		/* IF */
			if(dst[min_i] > dst[i])
			{
#if 0
				msr_inc_swap();	
#else
				msr_inc_asgn(1);
#endif
				min_i = i;
			}
		}
		
		if(min_i != bottom_i)
		{
			tmp = dst[min_i];
			dst[min_i] = dst[bottom_i];
			dst[bottom_i] = tmp;
#if 0
			msr_inc_swap();	
#else
			msr_inc_asgn(3);
#endif
		}
	}
	msr_stop_timer();
	return dst;
}

int* sort_int_wrap_s(const int *src, int *dst, long len)
{
	int	*return_dst = NULL;
	
	msr_init();
	return_dst = sort_select_int_classic(src, dst, len);

	return return_dst;
}
