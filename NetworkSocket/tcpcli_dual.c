/* tcpclient.c
* Sample program to connect to a TCP server.
* Instruction:
* tcpclient [option] host
* [option]:
*      -f [46]: specify address family to use
*             : (not specified by default)
*      -p     : specify service name or port number
*             : (port 9999 by default)
*      -d     : detailed output
*      -h     : show instructions
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#define DEFAULT_PORT "9999"

/* structure to include options */

struct s_opt {
      sa_family_t family;   /* PF_UNSPEC, PF_INET, PF_INET6 */
      int         debug;    /* display verbosely */
      char        *host;    /* nodename for getaddrinfo(3) */
      char        *service; /* service name: "pop", "110" */
};

void
print_help(char *pname)
{
      fprintf(stderr, "%s [option] host\n"
              " -f [46]: specify family\n"
              " -p : specify port (default 9999)\n"
              " -d : display verbosely\n"
              " -h : help\n",
              pname);
}


int
main(int argc, char **argv)
{
      struct s_opt sopt;
      int c,n;
      struct addrinfo hints, *res, *res0;
      int err;
      int s;
      char host[NI_MAXHOST];
      char serv[NI_MAXSERV];
      char rbuf[BUFSIZ];
      char wbuf[BUFSIZ];

      memset(&sopt, 0, sizeof(sopt));

      sopt.family = PF_UNSPEC;

      while ((c = getopt(argc, argv, "p:f:dh")) != EOF) {
              switch (c) {
              case 'p':
                      sopt.service = (char *)malloc(strlen(optarg)+1);
                      strcpy(sopt.service, optarg);
                      break;
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
              case 'd':
                      sopt.debug=1;
                      break;
              case 'h':
                default:
                      print_help(argv[0]);
                      exit(EXIT_SUCCESS);
              }
      }

      if (optind < argc) {
              sopt.host = (char *)malloc(strlen(argv[optind])+1);
              strcpy(sopt.host, argv[optind]);
      } else {
              print_help(argv[0]);
              exit(EXIT_FAILURE);
      }
    
      if (sopt.service == NULL) {
              sopt.service = (char *)malloc(strlen(DEFAULT_PORT)+1);
              strcpy(sopt.service, DEFAULT_PORT);
      }


      /* hints is used after zero cleared */
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = sopt.family;
      hints.ai_socktype = SOCK_STREAM;

      err = getaddrinfo(sopt.host, sopt.service, &hints, &res0);
      if (err) {
              fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
              exit(EXIT_FAILURE);
      }


      /* attempt to connect using getaddrinfo(3) results */
      for (res=res0 ; res ;  res = res->ai_next) {
              s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
              if (s < 0) {
                      fprintf(stderr, "socket() faild: %d\n", s);
                      continue;
              }

              if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
                      perror("connect");
                      close(s);
                      s = -1;
                      continue;
              }

              if (sopt.debug) {
                      getnameinfo(res->ai_addr, res->ai_addrlen,
                                  host, sizeof(host),
                                  serv, sizeof(serv),
                                  NI_NUMERICHOST|NI_NUMERICSERV);
                      fprintf(stderr, "connect: [%s]:%s\n", host, serv);
              }

              break;
      }

      if (s < 0) {
              fprintf(stderr, "can't connect %s\n", sopt.host);
              exit(EXIT_FAILURE);
      }

      n = -1;
      c = 0;
      while (fgets(wbuf, sizeof(wbuf), stdin) != NULL) {
              n = strlen(wbuf);
              if (n < 0 ) {
                      fprintf(stderr, "can't read");
                      exit(EXIT_FAILURE);
              }
              do {
                      n = write(s, wbuf, n+1);
              } while (n < 0 && errno==EINTR);

              do {
                      n = read(s, rbuf, sizeof(rbuf)-1);
                      c += n;
              } while (errno == EINTR && n <= 0);
              write(STDOUT_FILENO, rbuf, c);
      }

      if (sopt.service) free(sopt.service);
      if (sopt.host) free(sopt.service);
      freeaddrinfo(res0);
      return 0;
}