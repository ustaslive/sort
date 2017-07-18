#include <stdio.h>
#include <stdlib.h>
#include "measure.h"
#include "unisort.h"

void*	sort_bubble_item2(void *void_src, void *void_dst, int len)
{
	long	el_i = 0;	/* element ID */
	long	right_i = 0;	/* element ID */
	void	*p_tmp = NULL;
	long	swap_cnt=0;
	sort_rule_t	*p_rule = get_sort_rule();
	void	**pp_src = (void**) void_src;
	void	**pp_dst = (void**) void_dst;
	
	printf("executing %s()\n", __func__);
	
	PRINT_PTR(void_src);
	PRINT_PTR(pp_src);
	PRINT_PTR(*pp_src);
	
	PRINT_PTR(void_dst);
	PRINT_PTR(pp_dst);
	PRINT_PTR(*pp_dst);

	for(el_i=0; el_i < len; el_i++)
	{
		PRINT_PTR(pp_dst[el_i]);
		PRINT_PTR(&pp_dst[el_i]);
		PRINT_PTR(pp_src[el_i]);
		PRINT_PTR(&pp_src[el_i]);
		
		p_rule->f_copy((void*)&pp_dst[el_i], (void*)&pp_src[el_i]);
	}
	

	for(right_i=len-1; right_i > 0; --right_i)
	{
		swap_cnt = 0;
		msr_inc_comp();
		for(el_i=0; el_i < right_i; el_i++)
		{
			msr_inc_comp(); /* FOR */
			msr_inc_comp(); /* IF */
			
			if(p_rule->f_greater((void*)pp_dst[el_i], (void*)pp_dst[el_i+1]))
			{
				msr_inc_swap();
				
				PRINT_PTR(p_tmp);
				PRINT_PTR(&p_tmp);
				PRINT_PTR(pp_dst[el_i]);
				PRINT_PTR(&pp_dst[el_i]);
				
				p_rule->f_copy((void*)&p_tmp, (void*)&pp_dst[el_i]);
				
				PRINT_PTR(pp_dst[el_i]);
				PRINT_PTR(pp_dst[el_i+1]);
				
				p_rule->f_copy((void*)&pp_dst[el_i], (void*)&pp_dst[el_i+1]);
				
				PRINT_PTR(pp_dst[el_i+1]);
				PRINT_PTR(p_tmp);
				
				p_rule->f_copy((void*)&pp_dst[el_i+1], (void*)&p_tmp);
				
				PRINT_PTR(pp_dst[el_i]);
				PRINT_PTR(pp_dst[el_i+1]);
				
				swap_cnt++;
			}
		}
		if(0 == swap_cnt)
		{
			break;
		}
	}

	return (void*) pp_dst;
}

void* sort_item_wrap2(void *src, void *dst, int len)
{
	void	*return_dst = NULL;
	
	PRINT_PTR(src);
	PRINT_PTR(dst);
	
	msr_init();
	return_dst = sort_bubble_item2(src, dst, len);
	
	PRINT_PTR(return_dst);
	return return_dst;
}
