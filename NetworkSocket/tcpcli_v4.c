#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define DEST_PORT 1111
#define SndSize    1000
#define TestCounter 1024

int main(int argc, char *argv[])
{
      int sockfd;
      /* will hold the destination addr */
      struct sockaddr_in dest_addr;
      int i;
      int val = 1, nBytes;
      unsigned char p_Buf[1600];
      int size = sizeof(dest_addr);
      int dw_Counter = 0;

      if (argc != 2)
      {
              printf("Please input IP address!\n");
              return;
      }

      sockfd = socket(AF_INET, SOCK_STREAM, 0);

      if (sockfd == -1)
      {
              perror("Client-socket()");
              exit(1);
      }

      bzero(&dest_addr, sizeof(dest_addr));
      /* host byte order */
      dest_addr.sin_family = AF_INET;
      dest_addr.sin_port = htons(DEST_PORT);
      dest_addr.sin_addr.s_addr = inet_addr(argv[1]);

      printf("target IP:%s\n", argv[1]);
      memset(&(dest_addr.sin_zero), 0, 8);

      if (connect
          (sockfd, (struct sockaddr *) &dest_addr,
           sizeof(struct sockaddr)) == -1)
      {
              perror("Client-connect()");
              exit(1);
      }
      //while (dw_Counter++  < TestCounter )
      while (1)
      {

              for (i = 0; i < SndSize; i++)
                      p_Buf[i] = (dw_Counter + 1) % 256;

              if (0 >
                  sendto(sockfd, p_Buf, SndSize, 0,
                         (struct sockaddr *) &dest_addr, (socklen_t) size))
              {
                      printf("SND ERROR!\n");
                      break;
              } else
              {
                      printf("%d SND OK!\n", dw_Counter);

                      memset(p_Buf, 0, SndSize);

                      nBytes =
                          recvfrom(sockfd, p_Buf, SndSize, 0,
                                   (struct sockaddr *) &dest_addr,
                                   (socklen_t *) & size);

                      if (nBytes < 0)
                      {
                              perror("recvfrom fail!\n");
                      }

                      if (p_Buf[0] != ((dw_Counter + 1) % 256))
                      {
                              printf("not match !!\n");
                              break;
                      }
                      for (i = 0; i < 5; i++)
                      {
                              printf("%x ", p_Buf[i]);
                      }
                      printf("\n");
              }

              dw_Counter++;
      }

      return 0;
}