/* This code is an updated version of the sample code from "Computer Networks: A Systems
 * Approach," 5th Edition by Larry L. Peterson and Bruce S. Davis. Some code comes from
 * man pages, mostly getaddrinfo(3). */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <dirent.h>


/*
 * Lookup a host IP address and connect to it using service. Arguments match the first two
 * arguments to getaddrinfo(3).
 *
 * Returns a connected socket descriptor or -1 on error. Caller is responsible for closing
 * the returned socket.
 */
int lookup_and_connect( const char *host, const char *service );

int sendall(int s, char *buf, int *len) {
	int total = 0;
	int bytesleft = *len;
	int n;
	while(total < *len) {
		n = send(s, buf+total, bytesleft, 0);
		if(n == -1){ break;}
		total += n;
		bytesleft -= n;
	}
	*len = total;
	return n==-1?-1:0;
}

int main( int argc , char *argv[]) {
	int s;
	char *host = argv[1];
	char *port = argv[2];
	char *ID = argv[3];
	char option[100];
	bool keepGoing = true;

	/* Lookup IP and connect to server */
	if ( ( s = lookup_and_connect( host, port ) ) < 0 ) {
		exit( 1 );
	}

	while(keepGoing == true){
		printf("Enter a command: ");
		option[0] = '\0';
		scanf("%s", option);
		for(int i = 0; i < sizeof(option) - 1; i++) {
			option[i] = toupper(option[i]);
		}
		
		if(strcmp(option, "EXIT") == 0){
			break;
		}else if(strcmp(option, "JOIN") == 0){
			char action[2];
			action[0] = '0';
			unsigned int id;
			sscanf(ID, "%i", &id);
			id = htonl(id);
			char packet[50];
			memcpy(packet, &action, strlen(action) + 1);
			memcpy(packet + sizeof(action), &id, sizeof(id));
			int len = sizeof(action) + sizeof(id);
			int worked;
			if((worked = sendall(s, packet, &len)) == -1){
				printf("Error Sending");
			}
			
		}else if(strcmp(option, "PUBLISH") == 0){
			struct dirent *de;
			char packet[50];
			DIR *dr = opendir("./SharedFiles");
			
					if (dr == NULL)
			{
				printf("Could not open current directory" );
				return 0;
			}
			int temp;
			while ((de = readdir(dr)) != NULL){
				if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){
					memcpy(packet + temp, de->d_name, strlen(de->d_name)+1);
					temp += strlen(de->d_name)+1;
					printf("%s\n", de->d_name);
				}
				
			}
			printf("%s",packet);
					
		
			closedir(dr);  
		}
	}
	

	
	close( s );

	return 0;
}

int lookup_and_connect( const char *host, const char *service ) {
	struct addrinfo hints;
	struct addrinfo *rp, *result;
	int s;

	/* Translate host name into peer's IP address */
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if ( ( s = getaddrinfo( host, service, &hints, &result ) ) != 0 ) {
		fprintf( stderr, "stream-talk-client: getaddrinfo: %s\n", gai_strerror( s ) );
		return -1;
	}

	/* Iterate through the address list and try to connect */
	for ( rp = result; rp != NULL; rp = rp->ai_next ) {
		if ( ( s = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 ) {
			continue;
		}

		if ( connect( s, rp->ai_addr, rp->ai_addrlen ) != -1 ) {
			break;
		}

		close( s );
	}
	if ( rp == NULL ) {
		perror( "stream-talk-client: connect" );
		return -1;
	}
	freeaddrinfo( result );

	return s;
}
