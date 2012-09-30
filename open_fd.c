#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define R_SIZE 0x10000 //64K
typedef  unsigned char BYTE;
int main()
{
	int  fd,r_len;
	BYTE *tmp_buf;
	BYTE *ptr;
	int  length = 0; 
	int   count = 0;
	//O_RDONLY,O_RDWR,O_CREAT
	if( (fd = open("biosout.bin", O_RDWR)) < 0)
    {
        fprintf(stderr, "[Error] can't open file : \n" );
        return 0;
    }
	
	if(lseek(fd, 0, SEEK_SET) < 0)
	{
		close(fd);
		return 0;
	}
	if ((tmp_buf= malloc(8 * 0x100000)) < 0)
	{
		printf ("malloc error\n");
		close (fd);
		return 0;	
	}
	
	ptr = tmp_buf;
	while((r_len = read(fd, ptr,R_SIZE )) > 0)
	{
		printf (" %x ptr %x r_len %d\n",ptr,r_len);
		
		ptr 	+= r_len;
		length  += r_len;
	}
	printf ("length %x\n",length);
	close(fd);
	return 0;	
}