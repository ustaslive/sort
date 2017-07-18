#include <stdio.h>
#include <stdlib.h>
#include "measure.h"
#include "unisort.h"

void*	sort_bubble_item(void *void_src, void *void_dst, int len)
{
	long	el_i = 0;	/* element ID */
	long	right_i = 0;	/* element ID */
	long	ii = 0;		/* stating byte of i'th element in array */
	long	ii_next = 0;
	char	*tmp = NULL;
	long	swap_cnt=0;
	sort_rule_t	*p_rule = get_sort_rule();
	char	*p_src = (char*) void_src;
	char	*p_dst = (char*) void_dst;
	
	printf("executing %s()\n", __func__);
	
	tmp = malloc(p_rule->item_size_bytes);
	if(NULL == tmp)
	{
		fprintf(stderr, "failed to alloc memory for item");
		exit(EXIT_FAILURE);
	}
	
	for(el_i=0; el_i < len; el_i++)
	{
		ii = el_i * p_rule->item_size_bytes;
		p_rule->f_copy(&p_dst[ii], &p_src[ii]);
	}
	
	
	for(right_i=len-1; right_i > 0; --right_i)
	{
		swap_cnt = 0;
		msr_inc_comp();
		for(el_i=0; el_i < right_i; el_i++)
		{
			msr_inc_comp(); /* FOR */
			msr_inc_comp(); /* IF */
			
			ii = el_i * p_rule->item_size_bytes;
			ii_next = (el_i+1) * p_rule->item_size_bytes;
			if(p_rule->f_greater(&p_dst[ii], &p_dst[ii_next]))
			{
				msr_inc_swap();
				
				p_rule->f_copy((void*)tmp, &p_dst[ii]);
				p_rule->f_copy(&p_dst[ii], &p_dst[ii_next]);
				p_rule->f_copy(&p_dst[ii_next], (void*)tmp);
				swap_cnt++;
			}
		}
		if(0 == swap_cnt)
		{
			break;
		}
	}
	
	if(NULL != tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	return p_dst;
}

void* sort_item_wrap(void *src, void *dst, int len)
{
	void	*return_dst = NULL;
	
	msr_init();
	return_dst = sort_bubble_item(src, dst, len);

	return return_dst;
}
