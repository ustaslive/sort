#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "measure.h"

/* 
 *	cc strtest.c bubble_str.c measure.c -o strtest -Wall -pedantic
 *	./strtest
 *
 */

#define TEST_STR
#define	ARR_SIZE(array)	(sizeof(array)/sizeof(array[0]))

extern char** sort_str_wrap(char **src, char **dst, int len);

#ifdef TEST_STR
char	*arr_str_1[] = {"z","b","c","a"};
char	*arr_str_2[] = {"below", "xyz", "above"};

char	*res_str_1[] = {"a","b","c","z"};
char	*res_str_2[] = {"above", "below", "xyz"};


void dump_str_arr(char *name, char **arr, int arr_len)
{
	int i=0;
	char *comma = "";
	printf("%s [%d]: {", name, arr_len);
	for(i=0; i < arr_len; i++)
	{
		printf("%s '%s'",  comma, arr[i]);
		comma = ",";
	}
	printf(" }\n");
}

int	compare_str_arrs(char **arr1, char **arr2, int len)
{
	int i=0;
	for(i=0; i< len; i++)
	{
		if(strcmp(arr1[i], arr2[i]))
		{
			return 0;
		}
	}
	return 1;
}

void test_str_sort(char *test_name, char** (*sort_func)(char **src, char **dst, int len), char **arr_to_sort, char** expected_arr, int len)
{
	char	**sorted_arr = NULL;
	
	printf("*** Start test '%s' ***\n", test_name);
	
	dump_str_arr("source", arr_to_sort, len);
	sorted_arr = malloc(len*sizeof(arr_to_sort[0]));
	if(NULL == sorted_arr)
	{
		fprintf(stderr, "failed to allocate memory for array\n");
		exit(EXIT_FAILURE);
	}
	
	sort_func(arr_to_sort, sorted_arr, len);
	
	if(1 == compare_str_arrs(expected_arr, sorted_arr, len))
	{
		printf(">>>>>>>>>>>>> arrays match\n");
		msr_dump();
		dump_str_arr("result", sorted_arr, len);
	}
	else
	{
		printf(">>>>>>>>>>>>> arrays do not match\n");
		dump_str_arr("result", sorted_arr, len);
		dump_str_arr("expected", expected_arr, len);
	}
	
	if(NULL != sorted_arr)
	{	
		free(sorted_arr);
		sorted_arr = NULL;
	}
	
	printf("*** End of test '%s' ***\n", test_name);
}

void test_str(void)
{
	test_str_sort("str test1", sort_str_wrap, arr_str_1, res_str_1, ARR_SIZE(arr_str_1));
	test_str_sort("str test2", sort_str_wrap, arr_str_2, res_str_2, ARR_SIZE(arr_str_2));
}
#endif


int main(void)
{

#ifdef TEST_STR
	test_str();
#endif
	return(EXIT_SUCCESS);
}

