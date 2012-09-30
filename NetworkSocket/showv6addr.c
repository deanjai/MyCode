/* for using IPv6 address in regular format */
#define USE_REGULAR_FORMAT 1 

#ifdef USE_REGULAR_FORMAT
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#endif

/*
* /proc/netif_inet6 format:
*
* "ipv6 address" "index" " prefix length" "scope" "DAD status" "interface name"
*
* refer to: iface_proc_info() in linux/net/ipv6/addrconf.c
*/
#define PROC_IFINET6_PATH "/proc/net/if_inet6"

/* refer to: linux/include/net/ipv6.h */
#define IPV6_ADDR_ANY           0x0000U
#define IPV6_ADDR_LOOPBACK      0x0010U
#define IPV6_ADDR_LINKLOCAL     0x0020U
#define IPV6_ADDR_SITELOCAL     0x0040U
#define IPV6_ADDR_COMPATv4      0x0080U


int
main(int argc, char **argv)
{

     FILE *fp;
     char addr[8][5];
     int index, plen, scope, flags;
     char ifname[8];
     char addr6[33];
#ifdef USE_REGULAR_FORMAT
     struct addrinfo hints, *res0;
     int err;
     char host[NI_MAXHOST];
#endif

     if ((fp = fopen(PROC_IFINET6_PATH, "r")) != NULL) {
             while (fscanf(fp,
                           "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %8s\n",
                           addr[0],addr[1],addr[2],addr[3],
                           addr[4],addr[5],addr[6],addr[7],
                           &index, &plen, &scope, &flags, ifname) != EOF) {
                     sprintf(addr6, "%s:%s:%s:%s:%s:%s:%s:%s",
                             addr[0],addr[1],addr[2],addr[3],
                             addr[4],addr[5],addr[6],addr[7]);
#ifdef USE_REGULAR_FORMAT
                     memset(&hints, 0, sizeof(hints));
                     hints.ai_family = PF_INET6;
                     hints.ai_flags = AI_NUMERICHOST;
                     err = getaddrinfo(addr6, NULL, &hints, &res0);
                     if (err) {
                             fprintf(stderr, "getaddrinfo: %s\n",
                                     gai_strerror(err));
                             exit(EXIT_FAILURE);
                     }
                     if (getnameinfo(res0->ai_addr, res0->ai_addrlen,
                                     host, sizeof(host), NULL, 0, NI_NUMERICHOST)) {
                             fprintf(stderr, "getaddrinfo: %s\n",
                                     gai_strerror(err));
                             exit(EXIT_FAILURE);
                     }
                     printf("%s: %s/%d scope:", ifname, host, plen);
                     freeaddrinfo(res0);
#else
                     printf("%s: %s/%d scope:", ifname, addr6, plen);
#endif
                     switch(scope) {
                     case IPV6_ADDR_ANY: printf("global\n");break;
                     case IPV6_ADDR_LOOPBACK: printf("loop\n");break;
                     case IPV6_ADDR_LINKLOCAL: printf("linklocal\n");break;
                     case IPV6_ADDR_SITELOCAL: printf("sitelocal\n");break;
                     case IPV6_ADDR_COMPATv4: printf("compat\n");break;
                        default: printf("unknown\n");
                     }
             }
     }

     return 0;
}