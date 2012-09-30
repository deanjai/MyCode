#include <stdio.h>


int main()
{
	FILE *res;
	unsigned char buffer[128];
	int val,nBytes;
	
	val = 1;
	sprintf (buffer,"test= %d",val);
	// the pointer string "buffer" given to putenv() is used.
	// In particular, this string becomes part of the environment; changing it (buffer)later will change the environment.
	putenv(buffer);
	memset (buffer,0,128);
	res = popen("./test.sh ", "r");
	
	nBytes = fread(buffer, sizeof(char), sizeof(buffer), res);
	printf ("%s\n",buffer);
	fclose(res);
	return 0;	
}