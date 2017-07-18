#include <stdio.h>
#include <limits.h>
#include "measure.h"

extern	void	sort3(int *a);

int	get_pivot4(const int *arr, long low_i, long high_i)
{
	int	pivot = arr[high_i];
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
long	partition4(int *arr, long low_i, long high_i)
{
	int pivot = get_pivot4(arr, low_i, high_i);	/* pivot value */
	long pivot_i = low_i-1;	/* pivot position - to be found
				 * initially out of pre-set bounds */
	int	swap_tmp;
	long	i;
	
	msr_inc_call();
	/*
	printf("'%s' called (arr, %ld, %ld)\n", __func__, low_i, high_i);
	*/ 
	
	for(i=low_i; i <= high_i; i++)
	{
		msr_inc_comp(); /* IF */
		if(arr[i] <= pivot)
		{
			pivot_i++;
			if(i != pivot_i)
			{
				msr_inc_asgn(3);
				
				swap_tmp = arr[i];
				arr[i] = arr[pivot_i];
				arr[pivot_i] = swap_tmp;
			}
		}
	}
	/* printf("returning pivot ID[%ld], value[%d]\n", pivot_i, arr[pivot_i]); */
	return pivot_i;
}


void quick4_sort(int* arr, long low_i, long high_i)
{
	long	pivot_i;
	msr_inc_depth();
	msr_inc_call();
	/* printf("'%s' called (arr, %ld, %ld)\n", __func__, low_i, high_i); */
	if(high_i <= low_i)
		return;
	
	if(2 == high_i - low_i)	/* 3 elements left */
	{
		sort3(&arr[low_i]);
		return;
	}
	pivot_i = partition4(arr, low_i, high_i);
	quick4_sort(arr, low_i, pivot_i-1);
	quick4_sort(arr, pivot_i+1, high_i);
	msr_dec_depth();
}

int*	sort_quick4_int(const int *src, int *dst, long len)
{
	long i = 0;

	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	msr_start_timer();
	quick4_sort(dst, 0, len-1);
	msr_stop_timer();
	
	return dst;
}


	
int* sort_int_wrap_q4(const int *src, int *dst, long len)
{
	int	*return_dst = NULL;
	
	msr_init();
	return_dst = sort_quick4_int(src, dst, len);

	return return_dst;
}
