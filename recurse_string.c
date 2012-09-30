#include <stdio.h>


char string[]= "hello my dog!";

void reverse(int b,int e,char *str)
{

    char tmp;


    if  ( b > e)
        return ;
        
    tmp = *(str+b);
    *(str+b) = *(str+e);
    *(str+e) = tmp;
     e--;
     b++;

   reverse (b,e,str);
   return ;
}

int sum( int a)
{

    if (a == 0)
        return 0;
    else 
        return sum(a-1) + 1;

}
int main()
{

    reverse (0,strlen(string)-1,string);
    printf ("%s\n",string);
    printf ("sum %d\n",sum(10));
    return 0;

}

