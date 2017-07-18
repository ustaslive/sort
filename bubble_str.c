#include <stdio.h>
#include <string.h>
#include "measure.h"

/* improvement: sorting stops once we had not a single swap*/
char**	sort_bubble_str(char *src[], char *dst[], int len)
{
	int i = 0;
	int right_i = 0;
	char *tmp;
	int swaps_cnt = 0;
	printf("executing %s()\n", __func__);
	for(i=0; i<len; i++)
	{
		dst[i] = src[i];
	}
	
	for(right_i=len-1; right_i > 0; --right_i)
	{
		msr_inc_comp();
		swaps_cnt = 0;
		for(i=0; i < right_i; i++)
		{
			msr_inc_comp(); /* FOR */
			msr_inc_comp(); /* IF */
			
			if(strcmp(dst[i], dst[i+1]) > 0)
			{
				msr_inc_swap();
				
				tmp = dst[i];
				dst[i] = dst[i+1];
				dst[i+1] = tmp;
				swaps_cnt++;
			}
		}
		if(0 == swaps_cnt)
		{
			break;
		}
	}
	return dst;
}

char	**sort_str_wrap(char *src[], char *dst[], int len)
{
	char	**return_dst = NULL;
	
	msr_init();
	return_dst = sort_bubble_str(src, dst, len);

	return return_dst;
}
