#include <stdio.h>
#include <limits.h>
#include "measure.h"

/* improved 
 *	partitioning and get_pivot
 * algorithms
 */

int	get_pivot2(const int *arr, long low_i, long high_i)
{
	int	pivot = arr[low_i];
	/* printf("'%s' called (arr, %ld, %ld)\n", __func__, low_i, high_i);
	printf("'returning pivot[%d]\n", pivot); */
	return pivot;
}

/* 
 * redistributes elements of the part of the array limited with LOW HIGH
 * around a value 'pivot':
 * - lesser - to the left side of the array
 * - bigger - to the right side of the array
 * neither left nor right sides are definitely sorted
 * the only thing warranted - position related to pivot.
 * 
 */
long	partition2(int *arr, long low_i, long high_i)
{
	int pivot = get_pivot2(arr, low_i, high_i);	/* pivot value */
	int	swap_tmp;
	long	l_i = low_i-1;
	long	h_i = high_i+1;
	
	msr_inc_call();
	/*
	printf("'%s' called (arr, %ld, %ld)\n", __func__, low_i, high_i);
	*/ 
	
	while(1)
	{
		do
		{
			l_i++;
			msr_inc_comp();
		} while(arr[l_i] < pivot);
		do
		{
			h_i--;
			msr_inc_comp();
		} while(arr[h_i] > pivot);
		
		if(l_i >= h_i)
			return h_i;
		
		msr_inc_asgn(3);
		msr_inc_swap();
				
		swap_tmp = arr[l_i];
		arr[l_i] = arr[h_i];
		arr[h_i] = swap_tmp;
	}

	/* printf("returning pivot ID[%ld], value[%d]\n", pivot_i, arr[pivot_i]); */
	return h_i;;
}


void quick2_sort(int* arr, long low_i, long high_i)
{
	long	pivot_i;
	msr_inc_depth();
	msr_inc_call();
	/* printf("'%s' called (arr, %ld, %ld)\n", __func__, low_i, high_i); */
	if(high_i <= low_i)
		return;

	pivot_i = partition2(arr, low_i, high_i);
	quick2_sort(arr, low_i, pivot_i);
	quick2_sort(arr, pivot_i+1, high_i);
	msr_dec_depth();
}

int*	sort_quick2_int(const int *src, int *dst, long len)
{
	long i = 0;

	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	msr_start_timer();
	quick2_sort(dst, 0, len-1);
	msr_stop_timer();
	
	return dst;
}


	
int* sort_int_wrap_q2(const int *src, int *dst, long len)
{
	int	*return_dst = NULL;
	
	msr_init();
	return_dst = sort_quick2_int(src, dst, len);

	return return_dst;
}
