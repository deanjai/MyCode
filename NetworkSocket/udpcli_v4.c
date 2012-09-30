#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SndSize    30
#define TestCounter 1024
int main(int argc, char *argv[])
{
       int fd, i;
       struct sockaddr_in addr;
       int val = 1, nBytes;
       unsigned char p_Buf[256];
       int size = sizeof(addr);
       int dw_Counter = 0;
       fd = socket(AF_INET, SOCK_DGRAM, 0);
       if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) < 0)
       {
               printf("setsockopt");
       }
       bzero(&addr, sizeof(struct sockaddr_in));
       addr.sin_family = AF_INET;
       addr.sin_port = htons(1234);
       addr.sin_addr.s_addr = INADDR_ANY;
       if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) != 0)

       {
               printf("Error!!\n");
       }
       if (argc != 2)

       {
               printf("Please input IP address!\n");
               return;
       }
       bzero(&addr, sizeof(addr));
       addr.sin_family = AF_INET;
       addr.sin_port = htons(55);
       if (argc < 2)
               printf("please input IP address!\n");
       addr.sin_addr.s_addr = inet_addr(argv[1]);
       printf("target IP:%s\n", argv[1]);

       while (1)

       {
               for (i = 0; i < 128; i++)
                       p_Buf[i] = (dw_Counter + 1) % 256;
               if (0 >
                   sendto(fd, p_Buf, SndSize, 0, (struct sockaddr *) &addr,
                          (socklen_t) size))

               {
                       printf("SND ERROR!\n");
               }

               else

               {
                       printf("%d SND OK!\n", dw_Counter);
                       nBytes =
                           recvfrom(fd, p_Buf, 256, 0,
                                    (struct sockaddr *) &addr,
                                    (socklen_t *) & size);

                       printf(" RECV OK!\n");
                       for (i = 0; i < nBytes; i++)

                       {
                               printf("%x ", p_Buf[i]);
                       }
                       printf("\n");
                       if (p_Buf[0] != ((dw_Counter + 1) % 256))

                       {
                               printf("not match !!\n");
                               break;
                       }
               }
               dw_Counter++;
       }
}