#include "unisort.h"


int	default_greater(void* i1, void *i2);
void	default_copy(void* dst, void *src);

sort_rule_t	default_rule = 
{
	sizeof(char),
	default_greater,
	default_copy
};

sort_rule_t	*p_current_rule = &default_rule;

sort_rule_t*	set_sort_rule(sort_rule_t *p_new_rule)
{
	sort_rule_t *p_return_rule;
	p_return_rule = p_current_rule;
	p_current_rule = p_new_rule;
	return p_return_rule;
}
 int	default_greater(void* i1, void *i2)
{
	if(*((char*)i1) > *((char*)i2))
	{
		return 1;
	}
	return 0;
}

void	default_copy(void* dst, void *src)
{
	*((char*)dst) = *((char*)src);
}

sort_rule_t*	get_sort_rule(void)
{
	return p_current_rule;
}
