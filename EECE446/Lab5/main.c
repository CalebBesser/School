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
#include <fcntl.h>
#include <ctype.h>

/*
 * Lookup a host IP address and connect to it using service. Arguments match the first two
 * arguments to getaddrinfo(3).
 *
 * Returns a connected socket descriptor or -1 on error. Caller is responsible for closing
 * the returned socket.
 */
int lookup_and_connect( const char *host, const char *service );

void io(){
    char str[1000];
    str[999] = '\0';
	int file = open("testText.txt", O_RDWR);
    int data = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(file == -1){
        printf("Error opening file");
        close(file);
    }
    if(data == -1){
        printf("Error opening data");
        close(data);
    }
    int nread = read(file,&str, sizeof(str));
    
    for(int i = 0; str[i] != '\0'; i++){
        str[i] = toupper(str[i]);
    }
    write(1, &str, nread);
    write(data, &str, nread);
    close(file);
    close(data);
}

void http(int s){ 
    char str[60] = "GET /~kkredo/reset_instructions.pdf HTTP/1.0\r\n\r\n";
	char buf[1000000];

    int data = open("local_file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	memcpy(buf, str, strlen(str));

    if( send(s, str, strlen(str), 0) < 0){
        printf("Error, Did not send");
    }

    while(recv(s, buf, 1, 0) > 0){

        write(data, &buf, 1);
    }

    printf("%d", total);
    close(data);
}

int main( ) {
    int s;
    const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";
    //io();
	

	
    
	/* Lookup IP and connect to server */
	if ( ( s = lookup_and_connect( host, port ) ) < 0 ) {
		exit( 1 );
	}
    http(s);
   
    
	
	


	//printf("%s", buff);

	/* Modify the program so it
	 *
	 * 1) connects to www.ecst.csuchico.edu on port 80 (mostly done above)
	 * 2) sends "GET /~kkredo/file.html HTTP/1.0\r\n\r\n" to the server
	 * 3) receives all the data sent by the server (HINT: "orderly shutdown" in recv(2))
	 * 4) prints the total number of bytes received
	 *
	 * */

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
