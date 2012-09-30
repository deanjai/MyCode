#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
       int fd;
       struct sockaddr_in addr;
       int val = 1, nBytes;
       char p_Buf[256];
       int size = sizeof(addr);
       int i;
       int dw_Counter = 0;
       fd = socket(AF_INET, SOCK_DGRAM, 0);
       if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) < 0)

       {
               printf("setsockopt\n");
               exit(1);
       }
       bzero(&addr, sizeof(struct sockaddr_in));
       addr.sin_family = AF_INET;
       addr.sin_port = htons(55);
       addr.sin_addr.s_addr = INADDR_ANY;
       if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) != 0)

       {
               printf("Error!!\n");
               exit(1);
       }
       while (1)

       {
               nBytes =
                   recvfrom(fd, p_Buf, 256, 0, (struct sockaddr *) &addr,
                            (socklen_t *) & size);
               if (nBytes > 0)

               {
                       printf("recv data : %x len %x!\n", p_Buf[0], nBytes);
                       if (0 >
                           sendto(fd, p_Buf, nBytes, 0,
                                  (struct sockaddr *) &addr, sizeof(addr)))
                               printf("Error send\n");

               }

               else

               {
                       printf("recv error\n");
               }
       }
}