#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//:  agruement is required after option ,like -f file.txt -o .....
//:: no agruement required after option ,like -f  -o .......
static const char *optString = "Il:o:vh?";
int main(int argc, char *argv[])
{
	int opt;
	
	opt = getopt( argc, argv, optString );
    while( opt != -1 ) {
    	printf ("got %c\n",opt);
    	printf ("optarg %s\n",optarg);
    	printf ("optind %x\n",optind);
        switch( opt ) {
            case 'I':
                break;
                
            case 'l':
                break;
                
            case 'o':
                break;
                
            case 'v':
                break;
                
            case 'h':   
            case '?':
                break;
                
            default:
                
                break;
        }
        
        opt = getopt( argc, argv, optString );
    }
	
	return 0;
}