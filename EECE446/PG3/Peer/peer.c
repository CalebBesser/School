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
#include <arpa/inet.h>


/*
 * Lookup a host IP address and connect to it using service. Arguments match the first two
 * arguments to getaddrinfo(3).
 *
 * Returns a connected socket descriptor or -1 on error. Caller is responsible for closing
 * the returned socket.
 */
int lookup_and_connect( const char *host, const char *service );

// This is the Beej's Guide sendall code, for sending the whole packet
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

// This is a receiveall function i wrote that was based on teh Beej's sendall function, it
//keeps receiving till it receives buflen amount
int receiveall(int s, char *buf, int buflen) {
    int bytes_received = 0;
    int total_received = 0;

    while (total_received < buflen) {
        bytes_received = recv(s, buf + total_received, buflen - total_received, 0);
		// < 0 means error, == 0 means the data is done sending.
        if (bytes_received < 0) {
            // error receiving data
            return -1;
        } else if (bytes_received == 0) {
            // connection closed by peer
            break;
        }

        total_received += bytes_received;
    }

    return total_received;
}

// This is for receiving and putting the the data into a file. This one took me a while. Cause i was confused about how
//to keep receving even though we dont know the file size. 

//Im fairly certain i used functions you didnt want me to. But i dont know how to do it without them, like memove.
int recv_file(int sockfd, char* filename) {
    int total = 0;
    size_t MAX_RECV_LEN = 1024;
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }
    
    char buffer[MAX_RECV_LEN];
    int num_bytes = 0;
    int header_received = 0;
    while ((num_bytes = recv(sockfd, buffer, MAX_RECV_LEN, 0)) > 0) {
        if (!header_received) {
            // Skip the first byte (header)
            num_bytes--;
            memmove(buffer, buffer + 1, num_bytes);
            header_received = 1;
        }
        int bytes_written = fwrite(buffer, sizeof(char), num_bytes, fp);
        if (bytes_written < num_bytes) {
            perror("Error writing file");
            fclose(fp);
            return -1;
        }
        total += bytes_written;
    }
    fclose(fp);
	close(sockfd);
    return total;
}

void Join(int s, char* ID){
	#define PACKETJ_LENGTH 5
	char packetJ[PACKETJ_LENGTH] = {0};

	// Set the action field
	packetJ[0] = 0x00;

	// Parse the ID as an unsigned int
	unsigned int id;
	sscanf(ID, "%u", &id);

	// Convert the ID to network byte order
	id = htonl(id);

	// Copy the ID into the packet
	memcpy(&packetJ[1], &id, sizeof(id));

	int temp = PACKETJ_LENGTH;
	int bytes_sent = sendall(s, packetJ, &temp);
	if (bytes_sent == -1) {
		perror("Error sending join packet\n");
	}
}

void Publish(int s){
	//Create action header
	char actionP[1];
	actionP[0] = 0x01;

	//Create Packet
	char packetP[1600] = {0};

	//Keep track of offset for memcpy, so data doesnt get overridden
	int packetP_offset = sizeof(actionP);

	//Put action into packet
	memcpy(packetP, actionP, packetP_offset);

	//This part took me a while, and i still dont entirely get it. It uses <dirent.h> to search
	//through a directory for all files, and counts them
	DIR *dirr;
	struct dirent *entt;
	unsigned int count = 0;
	if ((dirr = opendir("./SharedFiles")) != NULL) {
		while ((entt = readdir(dirr)) != NULL) {
			if (strcmp(entt->d_name, ".") != 0 && strcmp(entt->d_name, "..") != 0) {
				count++;
			}
		}
		closedir(dirr);
	} else {
		perror("Error: Unable to open directory\n");
	}

	//Takes the count and puts it in network byte order then adds it to the packet
	unsigned int count_n = htonl(count);
	memcpy(packetP + packetP_offset, &count_n, sizeof(count_n));

	//update packet offset
	packetP_offset += sizeof(count_n);

	//I could probably do this step with the other one, but i kept getting errors every time
	//This one goes through the directory again, and puts all the file names into the packet
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir("./SharedFiles")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				int len = strlen(ent->d_name) + 1;
				memcpy(packetP + packetP_offset, ent->d_name, len);
				packetP_offset += len;
			}
		}
		closedir(dir);
	} else {
		perror("Error: Unable to open directory\n");
	}

	int len = packetP_offset;

	int worked;

	//Send the packet
	if ((worked = sendall(s, packetP, &len)) == -1) {
		perror("Error Sending");

	}	

}

void Search(int s){
	//Create action header
	char actionS[1];
	actionS[0] = 0x02;
	//Create both the send and receive packets
	char packetS[50] = {0};
	char receiveS[5000] = {0};

	//Create and update packet offset, then copy action into the packet
	int packetS_offset = sizeof(actionS);
	memcpy(packetS, actionS, packetS_offset);

	//Get file name from user
	printf("Enter a file name: ");
	char searchInput[100] = {0};
	scanf("%s", searchInput);

	//Add null character
	searchInput[strlen(searchInput)] = '\0';

	//Add it to packet and update offset
	memcpy(packetS + packetS_offset, searchInput, (strlen(searchInput)) + 1);
	packetS_offset += strlen(searchInput) + 1;

	//This was jsut for testing to see if my packet matched the hex dump in the handout
	int len = packetS_offset;
	// for (int i = 0; i < len; i++) {
	// 	printf("0x%02x ", packetS[i]);
	// }
	// printf("\n");

	int worked;
	//Send the packet
	if ((worked = sendall(s, packetS, &len)) == -1) {
		perror("Error Sending");
	}else{
		//We know the receive will be 10 bytes
		int tempsize = 10;
		
		//Call receiveall function
		int amountReceived = receiveall(s, receiveS, tempsize);

		//Error handling
		if(amountReceived < 0){
			perror("Error Receiving\n");
		}else if(amountReceived == 0){
			perror("Connection closed");
		}else if(amountReceived != tempsize){
			perror("Wrong size receive");
		}else{
			//This is for checking if the file was there, it does so by checking if the packet has 
			//anything but 0s
			bool temp = false;
			for(int i = 0; i < amountReceived; i++){
				if(receiveS[i] != 0){
					temp = true;
					break;
				}
			}
			if(temp == true){
				//create the id IP and port
				uint32_t id;
				uint8_t ip[4];
				uint16_t port;

				//get them from the packet
				memcpy(&id, &receiveS[0], sizeof(id));
				memcpy(&ip[0], &receiveS[4], sizeof(ip));
				memcpy(&port, &receiveS[8], sizeof(port));

				//Switches id and port back from network byte order, and copies all of the ip
				id = ntohl(id);
				for (int i = 0; i < 4; i++) {
					ip[i] = receiveS[i+4];
				}
				port = ntohs(port);

				//Prints out the required message
				printf("File found at\n");
				printf(" peer %u\n", id);
				printf(" %u.%u.%u.%u:%u\n", ip[0], ip[1], ip[2], ip[3], port);
			}else{
				perror("File not indexed by registry\n");
			}
		}	
	}	
}

void Fetch(int s, int s2){

	//This whole part up till the next comment is exactly the same as search
	char actionF[1];
	actionF[0] = 0x02;
	char packetF[50] = {0};

	int packetF_offset = sizeof(actionF);
	memcpy(packetF, actionF, packetF_offset);
	printf("Enter a file name: ");
	char searchInput[100] = {0};
	scanf("%s", searchInput);
	searchInput[strlen(searchInput)] = '\0';
	memcpy(packetF + packetF_offset, searchInput, (strlen(searchInput)) + 1);
	packetF_offset += strlen(searchInput) + 1;

	int len = packetF_offset;
	// for (int i = 0; i < len; i++) {
	// 	printf("0x%02x ", packetF[i]);
	// }
	// printf("\n");
	char receiveF[5000] = {0};
	int worked;
	if ((worked = sendall(s, packetF, &len)) == -1) {
		perror("Error Sending\n");
	}else{
		int tempsize = 10;
		int amountReceived = receiveall(s, receiveF, tempsize);
		if(amountReceived < 0){
			perror("Error Receiving: Did not receive file\n");
		}else if(amountReceived == 0){
			perror("Connection closed: No file\n");
		}else if(amountReceived != tempsize){
			perror("Wrong size received on file receive\n");
		}else{
			bool temp = false;
			for(int i = 0; i < amountReceived; i++){
				if(receiveF[i] != 0){
					temp = true;
					break;
				}
			}
			if(temp == true){
				uint32_t id;
				uint8_t ip[4];
				uint16_t port;

				


				memcpy(&id, &receiveF[0], sizeof(id));
				memcpy(&ip[0], &receiveF[4], sizeof(ip));
				memcpy(&port, &receiveF[8], sizeof(port));

				id = ntohl(id);
				for (int i = 0; i < 4; i++) {
					ip[i] = receiveF[i+4];
				}
				port = ntohs(port);

				//This is where the actual fetch begins, first i create char pointers for IP and port
				//Since the lookup_and_connect function requires char pointers
				char *charIP = malloc(INET_ADDRSTRLEN);
				inet_ntop(AF_INET, ip, charIP, INET_ADDRSTRLEN);
				char charPort[6];
				sprintf(charPort, "%d", port);


				/* Lookup IP and connect to server */
				if ( ( s2 = lookup_and_connect( charIP, charPort ) ) < 0 ) {
					exit( 1 );
				}

				//Create the action 
				char actionF2[1];
				actionF2[0] = 0x03;

				//Create the Packet
				char packetF2[200] = {0};
				
				//Create and update packet offset

				//Also id like to point out that i use different variable names each time even though
				//the scopes are different. I was running into problems with different actions interferring.
				//I don't think this is how it works. But i wanted to be safe
				int packetF2_offset = sizeof(actionF2);
				memcpy(packetF2, actionF2, packetF2_offset);

				//Here i just use the searchInput from before as the file name
				memcpy(packetF2 + packetF2_offset, searchInput, (strlen(searchInput)) + 1);
				packetF2_offset += strlen(searchInput) + 1;

				//Sendall the packet
				int len = packetF2_offset;
				if ((worked = sendall(s2, packetF2, &len)) == -1) {
					perror("Error Sending\n");
				}else{
					//use the recv_file function to both receive the whole packet and put it into a file
					int total;
					if((total = recv_file(s2, searchInput)) <= 0){
						perror("Error receiving");
					}
				}
				//free the memory used for the char pointer
				free(charIP);
			}else{
				printf("File not indexed by registry\n");
			}
		}
	}
}

int main( int argc , char *argv[]) {
	int s, s2 = 0;
	char *host = argv[1];
	char *port = argv[2];
	char *ID = argv[3];
	char option[100];
	bool keepGoing = true;

	/* Lookup IP and connect to server */
	if ( ( s = lookup_and_connect( host, port ) ) < 0 ) {
		exit( 1 );
	}

	// Main while loop to keep code running
	while(keepGoing == true){

		//get user input
		printf("Enter a command: ");
		option[0] = '\0';
		scanf("%s", option);

		//Main if/else
		//Just runs whatever function corresponds to the command typed
		if(strcmp(option, "EXIT") == 0){
			break;
		}else if(strcmp(option, "JOIN") == 0){
			Join(s, ID);
		}else if(strcmp(option, "PUBLISH") == 0){
			Publish(s);
		}else if(strcmp(option, "SEARCH") == 0){
			Search(s);
		}else if(strcmp(option, "FETCH") == 0){
			Fetch(s, s2);
		}else{
			continue;
		}
	}
	//There is a path through the code that leads to s2 not closing, so i added this
	if(s2){
		close( s2 );
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
