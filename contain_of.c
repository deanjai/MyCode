#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) );})

struct  St_Inner{
	int 				dw_a;
};

struct St_Outer {
	int 				dw_a;
	struct St_Inner		St_b;
	int 				dw_c;
};

     
int main ()
{
	struct St_Outer  src;
	struct St_Outer  *p_dest;
	struct  St_Inner *p_Inner;
	p_Inner = &src.St_b;
	
	
	p_dest = container_of (p_Inner,struct St_Outer, St_b);
	
	printf ("p_dest %x == src %x\n",p_dest,&src);
	return 0;
}

