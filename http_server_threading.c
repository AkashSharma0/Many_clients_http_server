// server
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <json/json.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <time.h>
#include <pthread.h> 
#define PORT 8000 
#define MAX_THREADS 5



void *response(void *socket)
{	
            int sock = *(int*)socket;
	    char read_buffer[2048];
	char send_buffer[2048];

		// HTTP Response Headers

	char test_buffer[1024] = "HTTP/1.1 200 OK\nAccept: application/json\nContent-Type:application/json\nContent-Length: 1024\n\n 	{server:says, hello:to you}"; 

	sprintf(send_buffer,"%s { server: %d}",test_buffer,sock);

  
while(1)
	{

    	    

	   read(sock, read_buffer, 2048);

	   printf("%s",read_buffer );

	   bzero((char *) &read_buffer, sizeof(read_buffer));

	   send(sock , send_buffer , strlen(send_buffer),0); 

	   bzero((char *) &send_buffer, sizeof(send_buffer));

	}
}

int main(int argc, char const *argv[]) 

{ 
	//while(1)
	{
	    int server_fd, new_socket, valread; 
	    struct sockaddr_in address; 
	    int opt = 1; 
	    int addrlen = sizeof(address); 
	    int t=0;
            pthread_t threads[MAX_THREADS];

	       
	    // Creating socket file descriptor 

	    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	    { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	    } 
	       
	    // Forcefully attaching socket to the port  

	    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	    { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	    } 
	    address.sin_family = AF_INET; 
	    address.sin_addr.s_addr = INADDR_ANY; 
	    address.sin_port = htons( PORT ); 
	       
	    // Forcefully attaching socket to the port  

	    if (bind(server_fd, (struct sockaddr *)&address,  
		                         sizeof(address))<0) 
	    { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	    } 

	    // Listening requests from client

	    if (listen(server_fd, MAX_THREADS) < 0) 
	    { 
		perror("listen"); 
	    } 

	while(1)
	{

	    // Assign new socket to the connected client

	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
		               (socklen_t*)&addrlen))>0) 
	    { 
		perror("accept"); 

    		if(t<MAX_THREADS)
		{

	//create thread			

      		 pthread_create(&threads[t], NULL, response, (void*) &new_socket);
		 pthread_detach(threads[t]);
		 t++;
	         //close(new_socket);
		}
		else
		{
		printf("Server is busy, please try later");
		pthread_detach(threads[t]);
	        //close(new_socket);
		}	
	    }
	 }

	  close(new_socket);
	  close(server_fd);	   
	    //return 0; 
	}
}


















