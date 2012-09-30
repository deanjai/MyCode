/*
* instruction:
* tcpserv [option]
* [option]:
*      -f [46]: specify IP address family
*             : (no option by default)
*      -a     : specify address to bind(2)
*      -p     : specify service name or port number
*             : (port 9999 by fdefault)
*      -6     : use IPV6_V6ONLY socket option
*      -A     : use SO_REUSEADDR socket option
*      -l     : do not bind(2) UNIXdomain protocl
*      -d     : detailed output
*      -h     : instruction for use
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netdb.h>


#define DEFAULT_PORT "9999" /* default port number for bind(2) */
#define DEFAULT_BACKLOG 5   /* backlog */

/* structure to include options */
struct s_opt {
     sa_family_t family;       /* PF_UNSPEC, PF_INET, PF_INET6 */
     int         debug;        /* display verbosely */
     char        *host;        /* nodename for getaddrinfo(3) */
     char        *service;     /* service name: "pop", "110" */
#ifdef IPV6_V6ONLY
     int         v6only;
#endif
     int         reuseaddr;    /* SO_REUSEADDR */
     int         dontuselocal; /* don't use PF_LOCAL socket */
};

void
print_help(char *pname)
{
     fprintf(stderr, "%s [option]\n"
             " -f [46]: specify family\n"
             " -a : specify bind address\n"

             " -p : specify port (default 9999)\n"
#ifdef IPV6_V6ONLY
             " -6 : use IPV6_V6ONLY option\n"
#endif
             " -A : use SO_REUSEADDR\n"
             " -l : don't use PF_LOCAL socket\n"
             " -d : display verbosely\n"
             " -h: help\n",
             pname);
}

void
read_write(int s, int debug)
{
     char peer[NI_MAXHOST];
     struct sockaddr_storage ss;
     socklen_t salen = sizeof(ss);
     int n;
     char buf[BUFSIZ];

     if (debug) {
             if (getpeername(s, (struct sockaddr *)&ss, &salen) < 0)
                     perror("getpeername");
             if (getnameinfo((struct sockaddr *)&ss, salen,
                         peer, sizeof(peer), NULL, 0, NI_NUMERICHOST) < 0)
                     perror("getnameinfo");
     }

     while ((n = read(s, buf, sizeof(buf))) > 0 &&
            write(s, buf, n) > 0) {
             if (debug) {
                     buf[n] = '\0';
                     fprintf(stderr, "%s: %s", peer, buf);
             }
     }

     close(s);
}

int
main(int argc, char **argv)
{
     int i,m;
     int c;
     int err;
  
     struct addrinfo hints, *res, *res0;
     struct sockaddr_storage ss;
     struct sockaddr *sa = (struct sockaddr *)&ss;

     int s[FD_SETSIZE];
     int smax = 0;
     int sock_max = -1;
     fd_set rfd, rfd0;
  
     char host[NI_MAXHOST];
     char serv[NI_MAXSERV];
     struct s_opt sopt;
     int backlog = DEFAULT_BACKLOG;


     socklen_t len = sizeof(ss);
  
     memset(&sopt, 0, sizeof(sopt));
  
     //default setting
     sopt.family = PF_UNSPEC;

     while ((c = getopt(argc, argv, "f:a:p:6Aldh")) != EOF) {
             switch (c) {
             case 'f':
                     if (!strncmp("4", optarg,1)) {
                             sopt.family = PF_INET;
                     } else if (!strncmp("6", optarg,1)) {
                             sopt.family = PF_INET6;
                     } else {
                             print_help(argv[0]);
                             exit(EXIT_FAILURE);
                     }
                     break;
             case 'a':
                     sopt.host = (char *)malloc(strlen(optarg)+1);
                     strcpy(sopt.host, optarg);
                     break;
             case 'p':
                     sopt.service = (char *)malloc(strlen(optarg)+1);
                     strcpy(sopt.service, optarg);
                     break;
#ifdef IPV6_V6ONLY
             case '6':
                     sopt.v6only=1;
                     break;
#endif
             case 'A':
                     sopt.reuseaddr=1;
                     break;
             case 'l':
                     sopt.dontuselocal=1;
                     break;
             case 'd':
                     sopt.debug=1;
                     break;
             case 'h':
                default:
                     print_help(argv[0]);
                     exit(EXIT_SUCCESS);
             }
     }

     if (sopt.service == NULL) {
             sopt.service = (char *)malloc(strlen(DEFAULT_PORT)+1);
             strcpy(sopt.service, DEFAULT_PORT);
     }

     /* hints is used after zero cleared */
     memset(&hints, 0, sizeof(hints));
     hints.ai_family = sopt.family;
     hints.ai_socktype = SOCK_STREAM;
     hints.ai_flags = AI_PASSIVE;   /* specified in the case of bind(2) */
  
     err =getaddrinfo(sopt.host, sopt.service, &hints, &res0);
     if (err) {
             fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
             exit(EXIT_FAILURE);
     }

     /* bind(2) with getaddrinfo(3) results */
     for (res=res0 ; res && smax < FD_SETSIZE; res = res->ai_next)
     {
             s[smax] = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
             if (s[smax] < 0) {
                     fprintf(stderr, "socket() failed: %d\n", s[smax]);
                     continue;
             }

             /* Some implementation returns UNIX domain protocol(PF_LOCAL) when hints.ai_family = PF_UNSPEC */
             if (res->ai_family == PF_LOCAL) {
                     if (sopt.dontuselocal) {
                             if (sopt.debug)
                                     fprintf(stderr, "skip: ai_family is PF_LOCAL\n");
                             continue;
                     } else {
                             struct sockaddr_un *sun;
                             sun = (struct sockaddr_un *)res->ai_addr;
                             /* deleated because you fail in bind(2) if socket file exists in the case of AF_LOCAL */
                             unlink(sun->sun_path);
                     }
             }

#ifdef IPV6_V6ONLY /* configure IPV6_V6ONLY socket option */
             if (sopt.v6only && res->ai_family == PF_INET6) {
                     int on = 1;
                     if (setsockopt(s[smax], IPPROTO_IPV6,IPV6_V6ONLY, &on, sizeof(on)) < 0) {
                             perror("setsockopt(IPV6_V6ONLY)");
                     }
             }
#endif
             if (sopt.reuseaddr) {
                     int on = 1;
                     if (setsockopt(s[smax], SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
                             perror("setsockopt(SO_REUSEADDR");
                     }
             }

             if (bind(s[smax], (struct sockaddr *)res->ai_addr, res->ai_addrlen) < 0) {
                     getnameinfo((struct sockaddr *)res->ai_addr, res->ai_addrlen,
                                 host, sizeof(host),
                                 serv, sizeof(serv),
                                 NI_NUMERICHOST|NI_NUMERICSERV);
                     fprintf(stderr, "bind(2):failed: %s: "
                             "[%s]:%s\n", strerror(errno), host, serv);
                     close(s[smax]);
                     s[smax] = -1;
                     continue;
             } else {
                     getnameinfo((struct sockaddr *)res->ai_addr, res->ai_addrlen,
                                 host, sizeof(host),
                                 serv, sizeof(serv),
                                 NI_NUMERICHOST|NI_NUMERICSERV);
                     fprintf(stderr, "bind(2):succeed: [%s]:%s\n", host, serv);
             }

             if (listen(s[smax], backlog) < 0) {
                     getnameinfo((struct sockaddr *)res->ai_addr, res->ai_addrlen,
                                 host, sizeof(host),
                                 serv, sizeof(serv),
                                 NI_NUMERICHOST|NI_NUMERICSERV);
                     fprintf(stderr, "listen(2):failed: [%s]:%s\n", host, serv);
                     close(s[smax]);
                     s[smax] = -1;
                     continue;
             } else {
                     getnameinfo((struct sockaddr *)res->ai_addr, res->ai_addrlen,
                                 host, sizeof(host),
                                 serv, sizeof(serv),
                                 NI_NUMERICHOST|NI_NUMERICSERV);
                     fprintf(stderr, "listen(2):succeed: [%s]:%s\n", host, serv);
             }

             if (s[smax] < sock_max)
                     sock_max = s[smax];
             smax++;
     }

     if (smax == 0) {
             fprintf(stderr, "no sockets\n");
             exit(EXIT_FAILURE);
     }
     freeaddrinfo(res0);


     FD_ZERO(&rfd0);
     for (i = 0; i < smax; i++)
             FD_SET(s[i], &rfd0);


     while (1) {
             rfd = rfd0;

             m  = select(sock_max+1, &rfd, NULL, NULL, NULL);
             if (m < 0) {
                     if (errno == EINTR)
                             continue;
                     perror("select");
                     exit(EXIT_FAILURE);
             }

          
             for (i=0;i<smax; i++) {
                     if (FD_ISSET(s[i], &rfd)) {
                             int cs;
                             cs = accept(s[i], sa, &len);
                             if (cs < 0) {
                                     perror("accept");
                                     exit(EXIT_FAILURE);
                             }
                             if (fork() == 0) {
                                     close(s[i]);
                                     read_write(cs, sopt.debug);
                                     close(cs);
                                     exit(EXIT_SUCCESS);
                             }
                             close(cs);
                     }
             }
     }

     return 0;
}