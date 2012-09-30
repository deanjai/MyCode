#include <stdio.h>

typedef struct St_Var
{
 char name[16];
 int  i;	
 void (*func)();
} St_VarTag;

//St_VarTag Var[2];

void a()
{
	printf ("a");
};

void b()
{
	printf ("b");
};

St_VarTag var[2] =
{
	{
		.name = "aa",
		.i 	  = 1 ,
		.func = a	
	}
	,
	{
		.name = "bb",
		.i 	  = 2,
		.func = b
	}	
};




int main()
{
	int i;
	
	for (i = 0 ;i < 2 ; i++)
	{
		//printf ("%s i = %x\n ",var[i].name,var[i].i);
		var[i].func();
	}
	
	
	return 0;
}

