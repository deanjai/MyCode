#include   <stdlib.h>
#include   <stdio.h>
#include   <unistd.h>
#include   <sys/time.h>
#include   <sys/types.h>
#include   <string.h>
#include   <signal.h>
#include   <sys/socket.h>
#include   <netinet/in.h>
#include   <arpa/inet.h>
#include   <errno.h>

#define   MAXCLIENT   10
#define   PORT   1111
#define   BUF_SIZE   1600
#undef    max
#define   max(x,y)   ((x)   >   (y)   ?   (x)   :   (y))

int main(int argc, char *argv[])
{

       int listen_id, accep_id;
       int n;
       static int currentid = 0;
       int fd[MAXCLIENT];
       int i, j, len;
       char buffer[BUF_SIZE];
       struct sockaddr_in serveraddr, clientaddr;

       fd_set rd;

       for (i = 0; i < MAXCLIENT; i++)
       {
               fd[i] = -1;
       }

       listen_id = socket(AF_INET, SOCK_STREAM, 0);
       if (listen_id < 0)
       {
               fprintf(stderr, "Create   listen   socket   failure.\n");
               exit(1);
       }

       memset(&serveraddr, 0, sizeof(serveraddr));
       serveraddr.sin_family = AF_INET;
       serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
       serveraddr.sin_port = htons(PORT);

       if (bind(listen_id, (struct sockaddr *) &serveraddr, sizeof(serveraddr))
           < 0)
       {
               fprintf(stderr, "bind   listen   socket   failure.\n");
               exit(1);
       }

       if (listen(listen_id, MAXCLIENT) < 0)
       {
               fprintf(stderr, "listen   the   socket   failure.\n");
       }

       FD_ZERO(&rd);

       int nfds = 0;
       while (1)
       {
               int ret;

               nfds = max(nfds, listen_id);

               FD_SET(listen_id, &rd);

               ret = select(nfds + 1, &rd, NULL, NULL, NULL);

               if (ret < 0)
               {
                       if (EINTR == errno)
                       {
                               continue;
                       } else
                       {
                               fprintf(stderr, "begin   select   failure.\n");
                       }
               }

               if (FD_ISSET(listen_id, &rd))
               {
                       memset(&clientaddr, 0, sizeof(clientaddr));
                       len = sizeof(struct sockaddr);
                       fd[currentid] =
                           accept(listen_id, (struct sockaddr *) &clientaddr,
                                  &len);
                       if (fd[currentid] < 0)
                       {
                               perror
                                   ("accept   a   new   connect   failure.\n");

                               close(listen_id);
                               exit(1);
                       } else
                       {
                               FD_SET(fd[currentid], &rd);

                               printf("Accept a new connection %x\n",
                                      fd[currentid]);

                               if (fd[currentid] > nfds)
                               {
                                       nfds = fd[currentid];
                               }

                               currentid++;

                       }
               } else
               {
                       for (j = 0; j < currentid; j++)
                       {
                               if (fd[j] < 0)
                               {
                                       continue;
                               }

                               if (FD_ISSET(fd[j], &rd))
                               {
                                       int rec;
                                       memset(&buffer, 0, sizeof(buffer));
                                       rec = recv(fd[j], buffer, BUF_SIZE, 0);
                                       if (rec < 0)
                                       {
                                               fprintf(stderr,
                                                       "received   from   client:   %s   failure.\n",
                                                       inet_ntoa(clientaddr.
                                                                 sin_addr));

                                               close(fd[j]);
                                               FD_CLR(fd[j], &rd);
                                               fd[j] = -1;
                                       } else if (rec == 0)
                                       {
                                               close(fd[j]);
                                               FD_CLR(fd[j], &rd);
                                               fd[j] = -1;
                                       } else
                                       {
                                               if (send(fd[j], buffer, rec, 0)
                                                   != rec)
                                               {
                                                       fprintf(stderr,
                                                               "sento   client:   %s   ,failure.   \n",
                                                               inet_ntoa
                                                               (clientaddr.
                                                                sin_addr));

                                                       close(fd[j]);
                                                       FD_CLR(fd[j], &rd);
                                                       fd[j] = -1;
                                               }
                                       }

                                       FD_SET(fd[j], &rd);

                               }
                       }
               }
       }
}