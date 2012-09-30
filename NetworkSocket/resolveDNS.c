#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <libxml/parser.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <sys/socket.h>


int LookUpIPadress(const char *dns_name,char *ip_str)
{
	int err;
	struct addrinfo *res,*res1;
	struct sockaddr *addr;
	
	err = getaddrinfo(dns_name, NULL, NULL, &res);
	if (err != 0)
	{	
        dg_printf ("error getaddrinfo\n");
        return  -1;
    }
	
	for (res1 = res; res1 != NULL; res1 = res1->ai_next)
    {   
    	addr = res->ai_addr;
    	switch (addr->sa_family)
    	{
    		case AF_INET:
    			inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr),ip_str,INET_ADDRSTRLEN );
    			break;
    		case AF_INET6:
    			inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)addr)->sin6_addr),ip_str, INET6_ADDRSTRLEN);
    			break;
    		default :
    			break;
    	}
    	
    }	
	freeaddrinfo(res);
	return 0;
}
