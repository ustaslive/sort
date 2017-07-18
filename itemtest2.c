#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "measure.h"
#include "unisort.h"

/* 
 * Sorting items of complex nature. item is a structure object
 * 2 types of how list/array of items can be organized
 * - monolitic array of items, arr[i] is i-th element and internals can be accesses with arr[i].internal
 * - array of pointers to items. item ** arr. arr[i] = &item; memory allocated for item is outside
 * 
 * 
 * This version is 'array of pointers'
 */

/* 
 *	cc itemtest2.c bubble_item2.c measure.c unisort.c -o itemtest2 -Wall -pedantic
 *	./itemtest2 > ../out/itemtest2.log
 *
 */

#define	ARR_SIZE(array)	(sizeof(array)/sizeof(array[0]))
#define	SRULE_INT	0
#define	SRULE_LONG	1
#define	SRULE_CHAR	2
#define	SRULE_STR	3

typedef struct
{
	int	i;
	long	l;
	char	c;
	char	s[10];
} item_t;

extern void* sort_item_wrap2(void *src, void *dst, int len);
int	compare_item_greater_int(void *item1, void *item2);
int	compare_item_greater_long(void *item1, void *item2);
int	compare_item_greater_char(void *item1, void *item2);
int	compare_item_greater_str(void *item1, void *item2);
void	copy_item(void *dst, void *src);

sort_rule_t	srule [] = 
{
	{sizeof(item_t*), compare_item_greater_int, copy_item},	/* SRULE_INT */
	{sizeof(item_t*), compare_item_greater_long, copy_item},	/* SRULE_LONG */
	{sizeof(item_t*), compare_item_greater_char, copy_item},	/* SRULE_CHAR */
	{sizeof(item_t*), compare_item_greater_str, copy_item}	/* SRULE_STR */
};

item_t	item1 = {1, 333, 'a', "iiii"};
item_t	item2 = {2, 222, 'z', "aaaa"};
item_t	item3 = {3, 111, 'u', "rrrr"};
item_t	item4 = {4, 444, 'd', "bbbb"};


item_t	*arr_item_1[] =
{
	&item4, &item2, &item1, &item3
};

item_t	*res_item_1_int[] =
{
	&item1, &item2, &item3, &item4
};

item_t	*res_item_2_long[] =
{
	&item3, &item2, &item1, &item4
};

item_t	*res_item_3_char[] =
{
	&item1, &item4, &item3, &item2
};

item_t	*res_item_4_str[] =
{
	&item2, &item4, &item1, &item3
};

/* Test sort functions */
int	compare_item_greater_int(void *item1, void *item2)
{
	item_t *i1 = (item_t*) item1;
	item_t *i2 = (item_t*) item2;
	if(i1->i > i2->i)
	{
		return 1;
	}
	return 0;
}

int	compare_item_greater_long(void *item1, void *item2)
{
	item_t *i1 = (item_t*) item1;
	item_t *i2 = (item_t*) item2;
	PRINT_PTR(item1);
	PRINT_PTR(item2);
	PRINT_PTR(i1);
	PRINT_PTR(i2);
	
	if(i1->l > i2->l)
	{
		return 1;
	}
	return 0;
}

int	compare_item_greater_char(void *item1, void *item2)
{
	item_t *i1 = (item_t*) item1;
	item_t *i2 = (item_t*) item2;
	if(i1->c > i2->c)
	{
		return 1;
	}
	return 0;
}

int	compare_item_greater_str(void *item1, void *item2)
{
	item_t *i1 = (item_t*) item1;
	item_t *i2 = (item_t*) item2;
	if(strcmp(i1->s, i2->s) > 0)
	{
		return 1;
	}
	return 0;
}

int	compare_equal(item_t *item1, item_t *item2)
{
	if(	item1->i == item2->i &&
		item1->l == item2->l &&	
		item1->c == item2->c &&
		0 == strcmp(item1->s, item2->s))
	{
		return 1;
	}
	return 0;
}

int	compare_item_arrs(item_t **arr1, item_t **arr2, int len)
{
	int i=0;
	for(i=0; i< len; i++)
	{
		if(!compare_equal(arr1[i], arr2[i]))
		{
			return 0;
		}
	}
	return 1;
}

void dump_item_arr(char *name, item_t **arr, int arr_len)
{
	int i=0;
	char *comma = "";
	printf("%s [%d]: {", name, arr_len);
	for(i=0; i < arr_len; i++)
	{
		printf("%s {%d,%ld,'%c',\"%s\"}", comma, arr[i]->i, arr[i]->l, arr[i]->c, arr[i]->s);
		comma = ",";
	}
	printf(" }\n");
}

void copy_item(void *void_p_dst, void *void_p_src)
{
	/* we do not copy data here
	   we copy pointers
	 * 
	 * 
	 * both incoming parameters are pointers to some space where
	 * addresses of item_t variables are stored
	 * pointers to pointers
	 * 
	 * the goal is to copy SRC pointer of one item_t
	 * to the place where DST 
	 * 
	 * REAL item_t - 'item'[xxxx]
	 * address of item_t is stores in p_item_src = &item 
	 *	item_t *p_item_src = &item;
	 * 
	 * also destination
	 *	item_t *p_item_dst;
	 * to set new value to p_item_dst we need to pass address/pointer of p_item_dst
	 * variable to this func - &p_item_dst
	 * so here we have void **void_p_dst
	 * 
	 * we could only pass p_item_src (not &p_item_src) here
	 * but for consistency we pass address.
	 * 
	 *
	 */
	
	item_t **item_dst = (item_t**) void_p_dst;
	item_t **item_src = (item_t**) void_p_src;
	
	*item_dst = *item_src;

	msr_inc_copy();
}

void test_item_sort2(char *test_name, void* (*sort_func)(void *src, void *dst, int len), item_t **arr_to_sort, item_t **expected_arr, int len, sort_rule_t *p_srule)
{
	item_t	**sorted_arr = NULL;
	sort_rule_t	*p_old_srule = NULL;
	
	printf("*** Start test '%s' ***\n", test_name);
	
	dump_item_arr("source", arr_to_sort, len);
	sorted_arr = malloc(len*sizeof(arr_to_sort[0]));
	PRINT_PTR(sorted_arr);
	PRINT_PTR(*sorted_arr);
	if(NULL == sorted_arr)
	{
		fprintf(stderr, "failed to allocate memory for array\n");
		exit(EXIT_FAILURE);
	}
	
	p_old_srule = set_sort_rule(p_srule);
	PRINT_PTR(arr_to_sort);
	PRINT_PTR(*arr_to_sort);
	sort_func((void*)arr_to_sort, (void*)sorted_arr, len);
	set_sort_rule(p_old_srule);
	
	
	PRINT_PTR(expected_arr);
	PRINT_PTR(sorted_arr);
	PRINT_PTR(*sorted_arr);
	if(1 == compare_item_arrs(expected_arr, sorted_arr, len))
	{
		printf(">>>>>>>>>>>>> arrays match\n");
		msr_dump();
		dump_item_arr("result", sorted_arr, len);
	
	}
	else
	{
		printf(">>>>>>>>>>>>> arrays do not match\n");
		dump_item_arr("result", sorted_arr, len);
		dump_item_arr("expected", expected_arr, len);
	}
	
	if(NULL != sorted_arr)
	{	
		free(sorted_arr);
		sorted_arr = NULL;
	}
	
	printf("*** End of test '%s' ***\n", test_name);
}

void test_item2(void)
{
	test_item_sort2("item test1 (by int)", sort_item_wrap2, arr_item_1, res_item_1_int, ARR_SIZE(arr_item_1), &srule[SRULE_INT]);
	PRINT_PTR(&item1);
	PRINT_PTR(&item2);
	PRINT_PTR(&item3);
	PRINT_PTR(&item4);
	
	/* PRINT_PTR(sort_item_wrap2); */	
	PRINT_PTR(arr_item_1);
	PRINT_PTR(res_item_2_long);
	PRINT_PTR(&srule[SRULE_LONG]);
	
	test_item_sort2("item test2 (by long)", sort_item_wrap2, arr_item_1, res_item_2_long, ARR_SIZE(arr_item_1), &srule[SRULE_LONG]);
	test_item_sort2("item test3 (by char)", sort_item_wrap2, arr_item_1, res_item_3_char, ARR_SIZE(arr_item_1), &srule[SRULE_CHAR]);
	test_item_sort2("item test4 (by str)", sort_item_wrap2, arr_item_1, res_item_4_str, ARR_SIZE(arr_item_1), &srule[SRULE_STR]);
}

int main(void)
{
	test_item2();
	return(EXIT_SUCCESS);
}

