#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <net/if.h>
#include <ifaddrs.h>

void
print_addr(struct sockaddr *sa)
{
     int err;
     char host[NI_MAXHOST];

     if (sa == NULL) {
             printf("\n");
             return;
     }

     switch (sa->sa_family) {
     case AF_INET:
             err = getnameinfo(sa, sizeof(struct sockaddr_in),
                               host, sizeof(host), NULL, 0, NI_NUMERICHOST);
             if (err < 0) {
                     perror("getnameinfo");
                     exit(EXIT_FAILURE);
             }
             break;
     case AF_INET6:
             err = getnameinfo(sa, sizeof(struct sockaddr_in6),
                               host, sizeof(host), NULL, 0, NI_NUMERICHOST);
             if (err < 0) {
                     perror("getnameinfo");
                     exit(EXIT_FAILURE);
             }
             break;
        default:
             printf("\n");
             return;
     }

     printf("[%s]\n", host);
}

int
main(int argc, char **argv)
{
     struct ifaddrs *ifap0, *ifap;
     char buf[BUFSIZ];

     memset(buf, 0, sizeof(buf));

     if (getifaddrs(&ifap0)) {
             printf("err\n");
             goto fin;
     }

     for (ifap = ifap0; ifap; ifap=ifap->ifa_next) {
             if (ifap->ifa_addr == NULL)
                     continue;

             if (ifap->ifa_addr->sa_family != AF_INET &&
                 ifap->ifa_addr->sa_family != AF_INET6)
                     continue;
             printf("Interface: %s\n", ifap->ifa_name);                 
             printf("\tAddress: ");
             print_addr(ifap->ifa_addr);
             printf("\tNetmask: ");
             print_addr(ifap->ifa_netmask);
             printf("\tBcast: ");
             print_addr(ifap->ifa_broadaddr);
     }             

fin:
     freeifaddrs(ifap);

     return 0;
}