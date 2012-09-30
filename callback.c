#include <stdio.h>

//void (*(*fp)[2])(char *);
typedef void( *fp_t )(char *);
void  spi_read(char *data)
{
printf ("spi_read\n");
}

void  spi_write(char *data)
{
printf ("spi_write\n");
}

void  nand_read(char *data)
{
printf ("nand_read\n");
}

void  nand_write(char *data)
{
printf ("nand_write\n");
}
void (*spi_fp[2]) (char *);
void (*nand_fp[2]) (char *);
int main()
{
	int i;
 
        fp_t( *fp) [2];
	spi_fp[0] = spi_read;	
	spi_fp[1] = spi_write;	
	nand_fp[0] = nand_read;	
	nand_fp[1] = nand_write;	
	
	fp = &spi_fp;

	for (i=0;i<2;i++)
	{
		(*fp)[i] (0);
	}

	return 0;
}
