/*
 * microsort - sort int array of 3 elements 
 * 
 * cc micro.c measure.c -o micro
 * 
 */
#include <stdio.h>
#include "measure.h"



#define TESTME 

void	sort3(int *a)	/* assumes a is array of 3 int */
{
	int	swap_tmp;
	/* possible outcomes now
	 * A0,A1,A2
	 * A0,A2,A1
	 * A1,A0,A2
	 * A1,A2,A0
	 * A2,A0,A1
	 * A2,A1,A0
	 */
	msr_inc_comp();
	if(a[0] < a[1])
	{
		/* possible outcomes
		 * A0,A1,A2
		 * A0,A2,A1
		 * A2,A0,A1
		 */
		msr_inc_comp();
		if(a[1] > a[2])
		{
			/* possible outcomes
			 * A0,A2,A1
			 * A2,A0,A1
			 *
			 * A1 is the greatest now, only need to decide 
			 * between A0 and A2 
			 */
			msr_inc_comp();
			if(a[0] > a[2])
			{
				/* outcome A2,A0,A1, 
				 * long swap - all 3 elements 
				 * 
				 */
				
				msr_inc_asgn(4);
				
				swap_tmp = a[0];
				a[0] = a[2];
				a[2] = a[1];
				a[1] = swap_tmp;
				return;	/* 3 comparisons and long swap */
			}
			/*
			 * if A0 == A2: 2 possible outcomes [A0,A2,A1] and [A2,A0,A1]
			 * if A0 < A2: only one outcome [A0,A2,A1]
			 * outcome [A0,A2,A1] is acceptable in both cases
			 * Moreover it keeps existing order 
			 * and requires less swaps
			 */
			msr_inc_asgn(3);
			
			swap_tmp = a[1];
			a[1] = a[2];
			a[2] = swap_tmp;
			return; /* 3 comparisons and swap */
		}
		
		/* A1 <= A2
		 * possible outcomes
		 * A0,A1,A2
		 * A0,A2,A1
		 * A2,A0,A1
		 *
		 * if A1 < A2: outcome A0,A1,A2 - no swaps required
		 * if A1 == A2: possible outcomes when they stay together - A0 cannot be between equal values as A0 < A1:
		 *		 A0,A1,A2 and A0,A2,A1
		 * outcome A0,A1,A2 is acceptable in both cases
		 * moreover it does not require swaps and keeps existing oreder
		 */

		return; /* 2 comparisons and no swaps */
	}
	
	/* possible outcomes now - still all possible outcome as all
	 * can be equal, but we know A0 >= A1
	 * A0,A1,A2
	 * A0,A2,A1
	 * A1,A0,A2
	 * A1,A2,A0
	 * A2,A0,A1
	 * A2,A1,A0
	 */
	msr_inc_comp();
	if(a[0] > a[1])
	{
		/* possible outcomes now
		 * A1,A0,A2
		 * A1,A2,A0
		 * A2,A1,A0
		 */
		msr_inc_comp();
		if(a[0] > a[2])
		{
			/* possible outcomes now
			 * A1,A2,A0
			 * A2,A1,A0
			 */
			msr_inc_comp();
			if(a[1] < a[2])
			{
				/* possible outcomes now
				 * A1,A2,A0
				 */
				msr_inc_asgn(4);

				swap_tmp = a[0];
				a[0] = a[1];
				a[1] = a[2];
				a[2] = swap_tmp;

				return; /* 4 comparisons, long swap */
			}
			/* possible outcomes now
			 * A1,A2,A0
			 * A2,A1,A0
			 * 
			 * if A1>A2: A2,A1,A0
			 * if A1==A2: any of (A1,A2,A0; A2,A1,A0)
			 * 
			 * choose A2,A1,A0 as it
			 * - matches both > & ==
			 * - keeps order
			 * - requires only 1 swap
			 */
			msr_inc_asgn(3);

			swap_tmp = a[0];
			a[0] = a[2];
			a[2] = swap_tmp;
			
			return; /* 4 comparisons, long swap */
		}
		
		/* possible outcomes now
		 * A1,A0,A2
		 * A1,A2,A0
		 * A2,A1,A0
		 */
		msr_inc_comp();
		if(a[0] < a[2])
		{
			/* possible outcomes now
			 * A1,A0,A2
			 */
			msr_inc_asgn(3);
			
			swap_tmp = a[0];
			a[0] = a[1];
			a[1] = swap_tmp;
			return; /* 4 comparisons 1 swap */
		}

		/* A0 == A2
		 * possible outcomes now
		 * A1,A0,A2	keeps order 1 swap
		 * A1,A2,A0	long swap does not keep order
		 * 
		 * A2,A1,A0 - not valid as A1 != A0 and cannot be between equal elements
		 * 
		 * chose is A1,A0,A2
		 */
		msr_inc_asgn(3);
		
		swap_tmp = a[0];
		a[0] = a[1];
		a[1] = swap_tmp;
		return; /* 5 comparisons, 1 swap */
	}
	
	/* possible outcomes now - still all possible outcome as all A#
	 * can be equal, but we know for sure A0 == A1
	 * A0,A1,A2
	 * A0,A2,A1
	 * A1,A0,A2	does not keep order A0,A1
	 * A1,A2,A0	does not keep order A0,A1
	 * A2,A0,A1
	 * A2,A1,A0	does not keep order A0,A1
	 */
	/* just need to check if A2 greater or lesser than both A0,A1 */
	msr_inc_comp();
	if(a[0] > a[2])
	{
		/* possible outcomes, with A0 == A1
		 * A2,A0,A1
		 * A2,A1,A0	does not keep order A0,A1
		 * 
		 * A1,A2,A0 - not valid anymore as A0==A1 sequence cannot 
		 *		be with different inside
		 * to minimize swaps we accept 
		 * With INT as argument we do not care of the initial order 
		 * that A0 preceeded A1 
		 * only swap A0 - A2 is required for INT array
		 */
		msr_inc_asgn(3);
		
		swap_tmp = a[0];
		a[0] = a[2];
		a[2] = swap_tmp;
		return; /* 3 comparisons and swap */
	}
	/* 2 other cases
	 * A2 > A0 (and > A1 too)
	 *	possible outcomes
	 *	A0,A1,A2	choose this: no swaps, keep order
	 *	A1,A0,A2
	 * A2 == A0 (and == A1 too) 
	 *	all possible outcomes
	 *	A0,A1,A2	choose this: no swaps, keep order
	 *	A0,A2,A1
	 *	A1,A0,A2
	 *	A1,A2,A0
	 *	A2,A0,A1
	 *	A2,A1,A0
	 */
	return; /* 3 comparisons and no swaps */
}

void	sort3_v1(int *a)
{
	int	swap_tmp;
	msr_inc_comp();
	if(a[0] < a[1])
	{
		msr_inc_comp();
		if(a[1] > a[2])
		{
			msr_inc_comp();
			if(a[0] < a[2])
			{
				msr_inc_asgn(3);
				swap_tmp = a[1];
				a[1] = a[2];
				a[2] = swap_tmp;
			}
			else
			{
				msr_inc_asgn(4);
				swap_tmp = a[0];
				a[0] = a[2];
				a[2] = a[1];
				a[1] = swap_tmp;
			}
		}
		else
		{
		    /* do nothing */
			;
		}
	}
	else
	{
		msr_inc_comp();
		if(a[1] < a[2])
		{
			msr_inc_comp();
			if(a[0] < a[2])
			{
				msr_inc_asgn(3);
				swap_tmp = a[0];
				a[0] = a[1];
				a[1] = swap_tmp;
			}
			else
			{
				msr_inc_asgn(4);
				swap_tmp = a[0];
				a[0] = a[1];
				a[1] = a[2];
				a[2] = swap_tmp;
			}
		}
		else
		{
			msr_inc_asgn(3);
			swap_tmp = a[0];
			a[0] = a[2];
			a[2] = swap_tmp;
		}
	}
}

#ifdef TESTME
int	compare_micro_arr(int *arr1, const int *arr2)
{	
	int	i;
	for(i=0; i<3; i++)
	{
		if(arr1[i] != arr2[i])
			return 0;
	}
	return 1;
}

void	test_micro_sort(int *array_to_sort, const int* expected_arr)
{
	printf("{%d,%d,%d}: ", array_to_sort[0], array_to_sort[1], array_to_sort[2]);
	msr_init();
	sort3(array_to_sort);
	if(compare_micro_arr(array_to_sort, expected_arr))
	{
		printf("OK\t");
		msr_dump();
	}
	else
	{
		printf("FAILED, {%d,%d,%d}, expected {%d,%d,%d}\n", array_to_sort[0], array_to_sort[1], array_to_sort[2], expected_arr[0], expected_arr[1], expected_arr[2]);
	}
}

void	test_micro_sort_v1(int *array_to_sort, const int* expected_arr)
{
	printf("{%d,%d,%d}: ", array_to_sort[0], array_to_sort[1], array_to_sort[2]);
	msr_init();
	sort3_v1(array_to_sort);
	if(compare_micro_arr(array_to_sort, expected_arr))
	{
		printf("OK\t");
		msr_dump();
	}
	else
	{
		printf("FAILED, {%d,%d,%d}, expected {%d,%d,%d}\n", array_to_sort[0], array_to_sort[1], array_to_sort[2], expected_arr[0], expected_arr[1], expected_arr[2]);
	}
}

void	full_test(void (*process)(int *arr, const int *res))
{
	int	res_1[] = {1,2,3};
	int	arr_1_1[] = {1,2,3};
	int	arr_1_2[] = {1,3,2};
	int	arr_1_3[] = {2,1,3};
	int	arr_1_4[] = {2,3,1};
	int	arr_1_5[] = {3,2,1};
	int	arr_1_6[] = {3,1,2};

	int	res_2[] = {1,1,2};
	int	arr_2_1[] = {2,1,1};
	int	arr_2_2[] = {1,2,1};
	int	arr_2_3[] = {1,1,2};
	
	int	res_3[] = {1,1,1};
	int	arr_3_1[] = {1,1,1};
	
	process(arr_1_1, res_1);
	process(arr_1_2, res_1);
	process(arr_1_3, res_1);
	process(arr_1_4, res_1);
	process(arr_1_5, res_1);
	process(arr_1_6, res_1);
	process(arr_2_1, res_2);
	process(arr_2_2, res_2);
	process(arr_2_3, res_2);
	process(arr_3_1, res_3);
}


int main(void)
{
	full_test(test_micro_sort);
	printf("*****************\n");
	full_test(test_micro_sort_v1);
	return 0;
}
#endif