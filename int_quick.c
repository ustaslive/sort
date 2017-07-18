#include <stdio.h>
#include <limits.h>
#include "measure.h"

int	get_pivot(const int *arr, long low_i, long high_i)
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
long	partition(int *arr, long low_i, long high_i)
{
	int pivot = get_pivot(arr, low_i, high_i);	/* pivot value */
	long i_right_item = low_i-1;	/* at the moment of swap this is the index 
				 * of the element which should move to the 
				 * right side of pivot (>pivot) but still in 
				 *    */
	int	swap_tmp;
	long	i_left_item;
	
	msr_inc_call();
	/*
	printf("'%s' called (arr, %ld, %ld)\n", __func__, low_i, high_i);
	*/ 

	/*
	 * Terms
	 *	LEFT item - the item which should go to the left side of PIVOT value
	 *	RIGHT item - the item which should go to the right side of PIVOT value
	 *	CHECKED RIGHT items/series - '8,6,7' block i example below
	 * 
	 *	array in pre-sort state can be divided to
	 * 
	 *	1) checked area
	 *		LEFT items checked '3,1,2'
	 *		RIGHT items checked '8,6,7'
	 *	2) unchecked area '4,9'
	 * 
	 *	first element in unchecked area which should belong to LEFT side
	 *	is a candidate to exchange with left-most element of CHECKED RIGHT series
	 * 
	 * Example:
	 *	pivot = 5
	 *	3,1,2,8,6,7,4,9
	 *	arranged part: 3,1,2
	 *	block of RIGHT items checked: 8,6,7
	 * 
	 *	in this case at the swap point
	 *	i_left will point at '4'
	 *	i_right will point at '8'
	 *	after exchange
	 *	3,1,2,4,6,7,8,9
	 * 
	 *	So, in the block of series of CHECKED RIGHT items the left-most
	 *	one item will be exchanged with first LEFT item found
	 *	at the right side of this checked-RIGHT series of items
	 */	
	for(i_left_item=low_i; i_left_item <= high_i; i_left_item++)
	{
	
		msr_inc_comp(); /* IF */
		if(arr[i_left_item] <= pivot)
		{
			/* found an element which should belong to the left 
			 * side of the array
			 * 2 possible cases
			 * - there still no element/index found which 
			 *	if greater than pivot
			 * - we already have 
			 */
			i_right_item++;
			if(i != i_right_item)
			{
				msr_inc_asgn(3);
				
				swap_tmp = arr[i_left_item];
				arr[i_left_item] = arr[i_right_item];
				arr[i_right_item] = swap_tmp;
			}
		}
		/* 
		 * ELSE
		 *	we found new item which should belong to right side
		 *	or may be several consequent RIGHT item
		 *	i_left_item increases skip all them, looking for first occurence of
		 *	left item
		 *	once found i_right_item will be increased and will match
		 *	the first (left-most) item in series of RIGHT items
		 *	i_left will match first LEFT belong item which is on the right side of
		 *	series of RIGHT items
		 *	they will be exchanged 
		 */

	}
	/* printf("returning pivot ID[%ld], value[%d]\n", pivot_i, arr[pivot_i]); */
	return pivot_i;
}


void quick_sort(int* arr, long low_i, long high_i)
{
	long	pivot_i;
	msr_inc_depth();
	msr_inc_call();
	/* printf("'%s' called (arr, %ld, %ld)\n", __func__, low_i, high_i); */
	if(high_i <= low_i)
		return;
	pivot_i = partition(arr, low_i, high_i);
	quick_sort(arr, low_i, pivot_i-1);
	quick_sort(arr, pivot_i+1, high_i);
	msr_dec_depth();
}

int*	sort_quick_int(const int *src, int *dst, long len)
{
	long i = 0;

	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	msr_start_timer();
	quick_sort(dst, 0, len-1);
	msr_stop_timer();
	
	return dst;
}


	
int* sort_int_wrap_q(const int *src, int *dst, long len)
{
	int	*return_dst = NULL;
	
	msr_init();
	return_dst = sort_quick_int(src, dst, len);

	return return_dst;
}
