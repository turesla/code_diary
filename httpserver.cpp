#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <time.h>
#include<iostream>

#define ISspace(x) isspace((int)x)


/*
 * create socket 
 * bind socket port 
 * listen port
 */
int startup(int* port)
{
	int httpd = 0;
	struct sockaddr_in name;

	httpd = socket(AF_INET, SOCK_STREAM, 0);
	if(httpd == -1)
	{
		printf("socket error ! \n");
		return -1;
	}
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
	{
		printf("bind error !!! \n");	
		return -1;
	}
	if(*port == 0) //dynamically alloc port
	{
		u_int namelen = sizeof(name);	
		if(getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
		{
			printf("getsockname error");	
			return -1; 
		}
		*port = ntohs(name.sin_port);	
	}
	if(listen(httpd, 5) < 0)
	{
		printf("listen error");	
	}

	return httpd;
}

void AddResHeader(int sock);

void* accept_request(void * client)
{
	int* client_sock = (int *)client;
	//char buf[1024];
	//int numchars;
	//char method[255];
	//char url[255];
	//char path[512];
	//size_t i, j;
	//struct stat st;
	//int cgi = 0;
//
//	char* query_string = NULL;
//	
//	numchars = get_line(client, buf, sizeof(buf));
//	i = 0; j = 0;
//	while( !ISspace(buf[j] && i < sizeof(method) - 1 ))
//	{
//		method[i] = buf[j];	
//		i++; j++;
//	}
//	method[i] = '\0';
//
//	if(strcasecmp(method, "GET") && strcasecmp(method, "POST"))
//	{
//		return ;	
//	}
//
//	if(strcasecmp(method, "POST") == 0)
//	{
//		cgi = 1;	
//	}
	
	char buf_recv[1024];
	bzero(buf_recv, 1024);
	char file_path[1024];
	bzero(file_path,1024);
	file_path[0] = '.';
	read(*client_sock, buf_recv, strlen(buf_recv));

	strcat(buf_recv, "GET /monster.jpg ");
	std::string str_recv(buf_recv);
	std::string::size_type idx = str_recv.find("GET ");

	if(idx == std::string::npos)
	{
		return NULL;
	}
	else
	{
		if((idx + 4) < strlen(buf_recv))		
		{
			char* p = &buf_recv[idx + 4];	
		}
		size_t i = 1;
		size_t j = idx + 4;
		while(buf_recv[j] != ' ')
		{
			 file_path[i] = buf_recv[j];	
			 i++; j++;
		}
		file_path[i] = '\0';
		
		//send(client_sock, file_path, strlen(file_path), 0);

	}
	
	AddResHeader(*client_sock);

	//send file
	FILE* file_pointer = fopen(file_path, "rb");
	if(file_pointer == NULL)
	{
		printf("not found open !");
		return NULL;	
	}

	//FILE* ptest = fopen("test1.jpg", "w");
	//if(!ptest)
	//{
	//	return NULL;	
	//}

	char file_buf[1024];
	memset(file_buf, 0, sizeof(file_buf));
	//fread(file_buf, sizeof(file_buf), 1, file_pointer);
	while(!feof(file_pointer))
	{
		int size = fread(file_buf, sizeof(char), 1024,file_pointer);
		//fwrite(file_buf, sizeof(char), size, ptest);
		if(send(*client_sock, file_buf, size, 0) != size)
		{
			printf("send error");	
		}
		usleep(1);
	}
	//fclose(ptest);
	

	//int size = fread(file_buf, sizeof(file_buf), 1, file_pointer);
	//send(*client_sock, file_buf, size, 0);
	//while(!feof(file_pointer))
	//{
	//	size = fread(file_buf, sizeof(file_buf), 1, file_pointer);
	//	send(*client_sock, file_buf, size, 0);
	//}





	fclose(file_pointer);
	close(*client_sock);

		
}


int get_line(int sock, char* buf, int size)
{
	int i = 0;
	char c = '\0';
	int n;
	
	while((i < size - 1) && c != '\n')
	{
		n = recv(sock, &c, 1, 0);
		if(n > 0)
		{
			if(c == '\r')	
			{
				n = recv(sock, &c, 1, MSG_PEEK);
				if(n > 0 && c == '\n')
				{
					recv(sock, &c, 1, 0);		
				}
				else
				{
					c = '\n';
				}
			}
			buf[i] = c;
			i++;
		}
		else
		{
			c = '\n';	
		}
	}
	buf[i] = '\0';

	return (i);	
}

std::string getTime();

void AddResHeader(int sock)
{
	std::stringstream ss;
	ss.str("");
	ss<<"HTTP/1.1 200 OK\r\n";
	ss<<"Date: Fri, 07 Sep 2018 "<<getTime()<<" GMT\r\n";
	ss<<"Last-Modified: Tue, 13 Feb 2001 01:23:17 GMT\r\n";
	ss<<"ETag: 'baca-37d2612286b40'\r\n";
	ss<<"Accept-Ranges: bytes\r\n";
	ss<<"Content-Length: 47818\r\n";
	ss<<"Cache-Control: max-age=259200\r\n";
	ss<<"Expires: Sun, 07 Oct 2018 07:27:37 GMT\r\n";
	ss<<"Keep-Alive: timeout=3, max=100\r\n";
	ss<<"Connection: Keep-Alive\r\n";
	ss<<"Content-Type: image/jpeg\r\n";
	ss<<"Content-Language: non-html\r\n";
	ss<<"\r\n"; 
	
	send(sock, ss.str().c_str(), ss.str().size(), 0);
}

std::string getTime()
{
	time_t timep;
	time (&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%H:%M:%S",localtime(&timep) );
	return tmp;
}

int main(void)
{
	int server_sock = -1;
	int port = 8080;
	int client_sock = -1;
	struct sockaddr_in client_name;
	uint32_t client_name_len = sizeof(client_name);
	pthread_t newthread;

	server_sock = startup(&port);
	printf("httpd running on port , %d \n", port);
	
	while(1)
	{
		client_sock = accept(server_sock, 
							(struct sockaddr *)&client_name, 
							&client_name_len);	
		if(client_sock == -1)
		{
			printf("client sock error");	
			return 0; 
		}
		
		//if(pthread_create(&newthread, NULL, accept_request, (void*)&client_sock) != 0)
		//{
		//	printf("pthread_create error");	
	//		return 0;
	//	}
		accept_request(&client_sock);
	}

	close(server_sock);

	return 0;
}
