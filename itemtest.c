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
 * this version is MONOLITIC
 */

/* 
 *	cc itemtest.c bubble_item.c measure.c unisort.c -o itemtest -Wall -pedantic
 *	./itemtest
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

extern void* sort_item_wrap(void *src, void *dst, int len);
int	compare_item_greater_int(void *item1, void *item2);
int	compare_item_greater_long(void *item1, void *item2);
int	compare_item_greater_char(void *item1, void *item2);
int	compare_item_greater_str(void *item1, void *item2);
void	copy_item(void *dst, void *src);

sort_rule_t	srule [] = 
{
	{sizeof(item_t), compare_item_greater_int, copy_item},	/* SRULE_INT */
	{sizeof(item_t), compare_item_greater_long, copy_item},	/* SRULE_LONG */
	{sizeof(item_t), compare_item_greater_char, copy_item},	/* SRULE_CHAR */
	{sizeof(item_t), compare_item_greater_str, copy_item}	/* SRULE_STR */
};


item_t	arr_item_1[] =
{
	{4, 444, 'd', "bbbb"},
	{2, 222, 'z', "aaaa"},
	{1, 333, 'a', "iiii"},
	{3, 111, 'u', "rrrr"}
};

item_t	res_item_1_int[] =
{
	{1, 333, 'a', "iiii"},
	{2, 222, 'z', "aaaa"},
	{3, 111, 'u', "rrrr"},
	{4, 444, 'd', "bbbb"}
};

item_t	res_item_2_long[] =
{
	{3, 111, 'u', "rrrr"},
	{2, 222, 'z', "aaaa"},
	{1, 333, 'a', "iiii"},
	{4, 444, 'd', "bbbb"}
};

item_t	res_item_3_char[] =
{
	{1, 333, 'a', "iiii"},
	{4, 444, 'd', "bbbb"},
	{3, 111, 'u', "rrrr"},
	{2, 222, 'z', "aaaa"}
};

item_t	res_item_4_str[] =
{
	{2, 222, 'z', "aaaa"},
	{4, 444, 'd', "bbbb"},
	{1, 333, 'a', "iiii"},
	{3, 111, 'u', "rrrr"}
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

int	compare_item_arrs(item_t *arr1, item_t *arr2, int len)
{
	int i=0;
	for(i=0; i< len; i++)
	{
		if(!compare_equal(&arr1[i], &arr2[i]))
		{
			return 0;
		}
	}
	return 1;
}

void dump_item_arr(char *name, item_t *arr, int arr_len)
{
	int i=0;
	char *comma = "";
	printf("%s [%d]: {", name, arr_len);
	for(i=0; i < arr_len; i++)
	{
		printf("%s {%d,%ld,'%c',\"%s\"}", comma, arr[i].i, arr[i].l, arr[i].c, arr[i].s);
		comma = ",";
	}
	printf(" }\n");
}

void copy_item(void *dst, void *src)
{
	item_t *item_dst = (item_t*) dst;
	item_t *item_src = (item_t*) src;
	item_dst->i = item_src->i;
	item_dst->l = item_src->l;
	item_dst->c = item_src->c;
	strcpy(item_dst->s, item_src->s);
	msr_inc_copy();
	
}

void test_item_sort(char *test_name, void* (*sort_func)(void *src, void *dst, int len), item_t *arr_to_sort, item_t *expected_arr, int len, sort_rule_t *p_srule)
{
	item_t	*sorted_arr = NULL;
	sort_rule_t	*p_old_srule = NULL;
	
	printf("*** Start test '%s' ***\n", test_name);
	
	dump_item_arr("source", arr_to_sort, len);
	sorted_arr = malloc(len*sizeof(arr_to_sort[0]));
	if(NULL == sorted_arr)
	{
		fprintf(stderr, "failed to allocate memory for array\n");
		exit(EXIT_FAILURE);
	}
	
	p_old_srule = set_sort_rule(p_srule);
	sort_func((void*)arr_to_sort, (void*)sorted_arr, len);
	set_sort_rule(p_old_srule);
	
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

void test_item(void)
{
	test_item_sort("item test1 (by int)", sort_item_wrap, arr_item_1, res_item_1_int, ARR_SIZE(arr_item_1), &srule[SRULE_INT]);
	test_item_sort("item test2 (by long)", sort_item_wrap, arr_item_1, res_item_2_long, ARR_SIZE(arr_item_1), &srule[SRULE_LONG]);
	test_item_sort("item test3 (by char)", sort_item_wrap, arr_item_1, res_item_3_char, ARR_SIZE(arr_item_1), &srule[SRULE_CHAR]);
	test_item_sort("item test4 (by str)", sort_item_wrap, arr_item_1, res_item_4_str, ARR_SIZE(arr_item_1), &srule[SRULE_STR]);
	
}

int main(void)
{
	test_item();
	return(EXIT_SUCCESS);
}

