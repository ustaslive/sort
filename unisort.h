/*
 * Elements for assisting with sorting complex items
 */


#ifndef UNISORT_H
#define UNISORT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	size_t	item_size_bytes;	/* use sizeof() */
	int	(*f_greater)(void* item1, void* item2); /* 1 - item1 > item2
						      *	0 - item1 <= item2 
						      */
	void	(*f_copy)(void *to_item, void *from_item);
} sort_rule_t;

extern	sort_rule_t*	set_sort_rule(sort_rule_t *rule);
extern	sort_rule_t*	get_sort_rule(void);
	
#ifdef __cplusplus
}
#endif

#endif /* UNISORT_H */

