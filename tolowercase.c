#include <stdio.h>

 void StrToLowercase(unsigned char *str)
 {
	int i,len;
	len = strlen(str);
	for (i =0 ;i < len; i++,str++)
		if (*str > 'A' && *str < 'Z' )
			*str = tolower(*str);
}
int main ()
{
	int i ;
	unsigned char arr[] = "SMC.test.COM";

	StrToLowercase(arr);
	printf ("%s\n",arr);
	return 0;


}
