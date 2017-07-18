#ifndef __MEASURE_H
#define __MEASURE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
	unsigned long	comp;
	unsigned long	swap;
	unsigned long	copy;
	unsigned long	asgn;
	unsigned long	call;
	unsigned long	depth;		/* max recursion depth */
	unsigned long	curr_depth;	/* current depth */
	double		time;
} measure_t;
	
extern	void	msr_init(void);
extern	void	msr_inc_comp(void);
extern	void	msr_inc_swap(void);
extern	void	msr_inc_asgn(int cnt);
extern	void	msr_inc_copy(void);
extern	void	msr_inc_call(void);
extern	void	msr_start_timer(void);
extern	void	msr_stop_timer(void);
extern	void	msr_inc_depth(void);
extern	void	msr_dec_depth(void);

extern	void	msr_dump(void);

#define PRINT_PTR(p) printf( "%-30s:%30s:\t[%p]\n",__func__,#p,(void*)p)
#undef PRINT_PTR
#define PRINT_PTR(p) ;

#ifdef __cplusplus
}
#endif

#endif /* __MEASURE_H */

