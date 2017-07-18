#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "measure.h"

/* 
 *	cc inttest.c int_bubble.c measure.c -o inttest -Wall -pedantic
 *	cc inttest.c int_bubble.c int_select.c measure.c -o inttest -Wall -pedantic
 *	cc inttest.c int_bubble.c int_select.c int_quick.c measure.c -o inttest -Wall -pedantic
 *	cc inttest.c int_bubble.c int_select.c int_quick.c int_quick2.c measure.c -o inttest -Wall -pedantic
 *	cc inttest.c int_bubble.c int_select.c int_quick.c int_quick2.c int_quick3.c int_quick4.c micro.c measure.c -o inttest -Wall -pedantic
 *	./inttest
 *
 */

#define	ARR_SIZE(array)	(sizeof(array)/sizeof(array[0]))
#define	TEST_QUICK
#define	TEST_TEST
#define	ALGORITHM_COMPARE
#define DETAILED_OUTPUT

#undef	TEST_TEST
#undef	DETAILED_OUTPUT



extern int* sort_int_wrap_b(const int *src, int *dst, long len);
extern int* sort_int_wrap_s(const int *src, int *dst, long len);
extern int* sort_int_wrap_q(const int *src, int *dst, long len);
extern int* sort_int_wrap_q2(const int *src, int *dst, long len);	/* alternate partition() and get_pivot() */
extern int* sort_int_wrap_q3(const int *src, int *dst, long len);	/* sort_int_wrap_q2 with sort3() */
extern int* sort_int_wrap_q4(const int *src, int *dst, long len);	/* sort_int_wrap_q with sort3() */

int	arr_int_01[] = {3,1,2};
int	arr_int_02[] = {2,4,3,1};

int	arr_int_1[] = {1,2,3,4,5,6,7,8,9,10};
int	arr_int_2[] = {10,9,8,7,6,5,4,3,2,1};
int	arr_int_3[] = {1,2,3,4,1,2,3,4,1,2,3,4};
int	arr_int_4[] = {1};
int	arr_int_5[] = {2,1};
int	*arr_int_dyn = NULL;

int	res_int_01[] = {1,2,3};
int	res_int_02[] = {1,2,3,4};

int	res_int_1[] = {1,2,3,4,5,6,7,8,9,10};
int	res_int_3[] = {1,1,1,2,2,2,3,3,3,4,4,4};
int	res_int_4[] = {1};
int	res_int_5[] = {1,2};
int	*res_int_dyn = NULL;


int	get_rand(int max)
{
static	int first_time = 1;
	if(first_time)
	{
		srand(time(NULL));
		first_time = 0;
	}
	return rand()%max;	
}

int * gen_sorted_array(long size)
{
	int *allocated = malloc(size*sizeof(int));
	long i=0;
	int value = 0;
	if(NULL == allocated)
	{
		fprintf(stderr, "failed to allocate [%ld] bytes", size*sizeof(int));
		exit(-1);
	}
	for(i=0; i<size; i++)
	{
		value += get_rand(3);
		allocated[i] = value;
	}
	return allocated;
}

int*	duplicate_array(int *existing_arr, long size)
{
	int	*new_arr = malloc(size*sizeof(int));
	long	i=0;
	if(NULL == new_arr)
	{
		fprintf(stderr, "failed to allocate [%ld] bytes", size*sizeof(int));
		exit(-1);
	}
	for(i = 0; i < size; i++)
	{
		new_arr[i] = existing_arr[i];
	}
	return new_arr;
}

void	copy_array(int* dst, const int* src, long size)
{
	int	i=0;
	for(i=0; i < size; i++)
	{
		dst[i] = src[i];
	}
	return;
}

void	shuffle_array(int *arr, long size)
{
	long	i=0;
	long	random_i;
	int	temp;
	for(i = 0; i < size; i++)
	{
		do
		{
			random_i = get_rand(size);
		} while(i == random_i);
		temp = arr[i];
		arr[i] = arr[random_i];
		arr[random_i] = temp;
	}
}

void dump_int_arr(const char *name, const int *arr, long arr_len)
{
	long i=0;
	char *comma = "";
	printf("%s [%ld]: {", name, arr_len);
	for(i=0; i < arr_len; i++)
	{
		printf("%s %d",  comma, arr[i]);
		comma = ",";
	}
	printf(" }\n");
}

int	compare_int_arrs(const int *arr1, const int *arr2, int len)
{
	int i=0;
	for(i=0; i< len; i++)
	{
		if(arr1[i] != arr2[i])
		{
			return 0;
		}
	}
	return 1;
}

void test_int_sort(const char *test_name, int* (*sort_func)(const int *src, int *dst, long len), const int *arr_to_sort, const int* expected_arr, long len)
{
	int	*sorted_arr = NULL;
#ifdef	DETAILED_OUTPUT
	printf("*** Start test '%s' ***\n", test_name); */
	
	dump_int_arr("source", arr_to_sort, len);
#endif
	sorted_arr = malloc(len*sizeof(arr_to_sort[0]));
	if(NULL == sorted_arr)
	{
		fprintf(stderr, "failed to allocate memory for array\n");
		exit(EXIT_FAILURE);
	}
	
	sort_func(arr_to_sort, sorted_arr, len);
	
	if(1 == compare_int_arrs(expected_arr, sorted_arr, len))
	{
#ifdef DETAILED_OUTPUT
		printf(">>>>>>>>>>>>> arrays match\n");
#endif
		printf("'%s' sort, array length [%lu]", test_name, len);

		msr_dump();
#ifdef DETAILED_OUTPUT
		dump_int_arr("result", sorted_arr, len); 
#endif
	}
	else
	{
		printf(">>>>>>>>>>>>> arrays do not match\n");
		dump_int_arr("result", sorted_arr, len);
		dump_int_arr("expected", expected_arr, len);
	}
	
	if(NULL != sorted_arr)
	{	
		free(sorted_arr);
		sorted_arr = NULL;
	}
	
#ifdef DETAILED_OUTPUT
	printf("*** End of test '%s' ***\n", test_name);
#endif
}


void	sort_comparison(const char *test_name, const int* arr_to_sort, const int* expected_arr, long arr_size)
{
	printf("***** Test '%s' *****\n", test_name);
	/*
	test_int_sort("bubble", sort_int_wrap_b, arr_to_sort, expected_arr, arr_size);
	test_int_sort("select", sort_int_wrap_s, arr_to_sort, expected_arr, arr_size);
	 * */
	test_int_sort("quick", sort_int_wrap_q, arr_to_sort, expected_arr, arr_size);
	test_int_sort("micro", sort_int_wrap_q4, arr_to_sort, expected_arr, arr_size);
	test_int_sort("quick2", sort_int_wrap_q2, arr_to_sort, expected_arr, arr_size);
	test_int_sort("micro2", sort_int_wrap_q3, arr_to_sort, expected_arr, arr_size);
}


void	compare_algorithms(void)
{
	int i=0;
	long arr_size = 100;
/*	
	sort_comparison("test1",arr_int_1, res_int_1, ARR_SIZE(arr_int_1));
	sort_comparison("test2",arr_int_2, res_int_1, ARR_SIZE(arr_int_2));
	sort_comparison("test3",arr_int_3, res_int_3, ARR_SIZE(arr_int_3));
	sort_comparison("test4",arr_int_4, res_int_4, ARR_SIZE(arr_int_4));
*/
	sort_comparison("small",arr_int_01, res_int_01, ARR_SIZE(arr_int_01));
	
#define	DYN_ARR_SIZE	100
	
	for(i=0, arr_size=20000; i<5; i++, arr_size*=4)
	{
		res_int_dyn = gen_sorted_array(arr_size);
		arr_int_dyn = duplicate_array(res_int_dyn, arr_size);
		shuffle_array(arr_int_dyn, arr_size);
		
		sort_comparison("QUICK", arr_int_dyn, res_int_dyn, arr_size);
		
		free(res_int_dyn);
		res_int_dyn = NULL;
		free(arr_int_dyn);
		arr_int_dyn = NULL;
	}
}

void test_int(void)
{
#ifdef TEST_QUICK
	long	i;
	long	arr_size;
	
	test_int_sort("quick test01", sort_int_wrap_q, arr_int_01, res_int_01, ARR_SIZE(arr_int_01));
	test_int_sort("quick test02", sort_int_wrap_q, arr_int_02, res_int_02, ARR_SIZE(arr_int_02));

	test_int_sort("quick test1", sort_int_wrap_q, arr_int_1, res_int_1, ARR_SIZE(arr_int_1));
	test_int_sort("quick test2", sort_int_wrap_q, arr_int_2, res_int_1, ARR_SIZE(arr_int_2));
	test_int_sort("quick test3", sort_int_wrap_q, arr_int_3, res_int_3, ARR_SIZE(arr_int_3));
	test_int_sort("quick test4", sort_int_wrap_q, arr_int_4, res_int_4, ARR_SIZE(arr_int_4));
	test_int_sort("quick test5", sort_int_wrap_q, arr_int_5, res_int_5, ARR_SIZE(arr_int_5));
	
#define	DYN_ARR_SIZE	100
	
	for(i=0, arr_size=DYN_ARR_SIZE; i<5; i++, arr_size+= 100)
	{
		res_int_dyn = gen_sorted_array(arr_size);
		arr_int_dyn = duplicate_array(res_int_dyn, arr_size);
		shuffle_array(arr_int_dyn, arr_size);
		
		test_int_sort("quick", sort_int_wrap_q, arr_int_dyn, res_int_dyn, arr_size);
		
		free(res_int_dyn);
		res_int_dyn = NULL;
		free(arr_int_dyn);
		arr_int_dyn = NULL;
	}
#endif
	
#ifdef TEST_BUBBLE
	test_int_sort("bubble test1", sort_int_wrap_b, arr_int_1, res_int_1, ARR_SIZE(arr_int_1));
	test_int_sort("bubble test2", sort_int_wrap_b, arr_int_2, res_int_1, ARR_SIZE(arr_int_2));
	test_int_sort("bubble test3", sort_int_wrap_b, arr_int_3, res_int_3, ARR_SIZE(arr_int_3));
	test_int_sort("bubble test4", sort_int_wrap_b, arr_int_4, res_int_4, ARR_SIZE(arr_int_4));
	test_int_sort("bubble test5", sort_int_wrap_b, arr_int_5, res_int_5, ARR_SIZE(arr_int_5));
 
#define	DYN_ARR_SIZE	100
	
	for(i=0, arr_size=DYN_ARR_SIZE; i<5; i++, arr_size+= 100)
	{
		res_int_dyn = gen_sorted_array(arr_size);
		arr_int_dyn = duplicate_array(res_int_dyn, arr_size);
		shuffle_array(arr_int_dyn, arr_size);
		
		test_int_sort("bubble", sort_int_wrap_b, arr_int_dyn, res_int_dyn, arr_size);
		
		free(res_int_dyn);
		res_int_dyn = NULL;
		free(arr_int_dyn);
		arr_int_dyn = NULL;
	}
#endif
	
#ifdef TEST_SELECT
	test_int_sort("select test01", sort_int_wrap_s, arr_int_01, res_int_01, ARR_SIZE(arr_int_01));
	test_int_sort("select test02", sort_int_wrap_s, arr_int_02, res_int_02, ARR_SIZE(arr_int_02));

	test_int_sort("select test1", sort_int_wrap_s, arr_int_1, res_int_1, ARR_SIZE(arr_int_1));
	test_int_sort("select test2", sort_int_wrap_s, arr_int_2, res_int_1, ARR_SIZE(arr_int_2));
	test_int_sort("select test3", sort_int_wrap_s, arr_int_3, res_int_3, ARR_SIZE(arr_int_3));
	test_int_sort("select test4", sort_int_wrap_s, arr_int_4, res_int_4, ARR_SIZE(arr_int_4));
	test_int_sort("select test5", sort_int_wrap_s, arr_int_5, res_int_5, ARR_SIZE(arr_int_5));
 
#define	DYN_ARR_SIZE	100
	
	for(i=0, arr_size=DYN_ARR_SIZE; i<5; i++, arr_size+= 100)
	{
		res_int_dyn = gen_sorted_array(arr_size);
		arr_int_dyn = duplicate_array(res_int_dyn, arr_size);
		shuffle_array(arr_int_dyn, arr_size);
		
		test_int_sort("select", sort_int_wrap_s, arr_int_dyn, res_int_dyn, arr_size);
		
		free(res_int_dyn);
		res_int_dyn = NULL;
		free(arr_int_dyn);
		arr_int_dyn = NULL;
	}
#endif

	
}

int main(void)
{
#ifdef	ALGORITHM_COMPARE
	compare_algorithms();
#endif
#ifdef	TEST_TEST
	test_int();
#endif
	return(EXIT_SUCCESS);
}

