#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
                    /* l_type   l_whence  l_start  l_len  l_pid   */
    struct flock filelock = {F_WRLCK, SEEK_SET,   0,      0,     0 };
    int    fd;
	char   buffer[128];
	int    res;
	//An exclusive or write lock gives a process exclusive access for writing to the specified part of the file. 
	//While a write lock is in place, no other process can lock that part of the file.
	
    filelock.l_pid = getpid();

    if (argc > 1) 
    {
    	//A shared or read lock prohibits any other process from requesting a write lock on the specified part of the file. 
    	//However, other processes can request read locks.
        printf ("Get read lock\n");
        filelock.l_type = F_RDLCK;
    }

    if ((fd = open("a", O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }
	printf ("before fcntl ....\n");
	//F_SETLKW : block 
	//F_SETLK  : not blocking 
    if (fcntl(fd, F_SETLKW, &filelock) == -1) {
        perror("fcntl");
        exit(1);
    }
    if (argc > 1)
    {
    	printf ("before read ....\n");
		res = read (fd, buffer, 128);
	}
	
    printf("got lock\n");
    printf("Press <RETURN> to release lock: ");
    getchar();

    filelock.l_type = F_UNLCK;  

    if (fcntl(fd, F_SETLK, &filelock) == -1) {
        perror("fcntl");
        exit(1);
    }

    printf("Unlocked.\n");

    close(fd);

    return 0;
}