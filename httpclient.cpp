#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <sstream>

#define BUFFER_SIZE 1024


void socketHttp(std::string host, std::string request)
{
	int sockfd;
	struct sockaddr_in address;
	struct hostent* server = NULL;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	server = gethostbyname(host.c_str());
	//memcpy((struct in_addr*)&address.sin_addr.s_addr, (struct in_addr* )server->h_addr, server->h_length);
	address.sin_addr =  *((struct in_addr *)server->h_addr);

	if(-1 == connect(sockfd, (struct sockaddr*)&address, sizeof(address)))
	{
		std::cout<< "connect error "<<std::endl;
		return ;
	}
	
	char file_name[512];
	bzero(file_name, 512);
	std::cout<<"file name : ";
	//std::cin>>file_name;
	strcat(file_name, "monster.jpg");
	
	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);	
	strncpy(buffer, file_name, strlen(file_name) > 1024 ? 1024 : strlen(file_name));

	write(sockfd, request.c_str(), request.size());

	FILE * fp = fopen(file_name, "w");
	if(NULL == fp)
	{
		std::cout<<"error file";
	}

	bzero(buffer, BUFFER_SIZE);
	int 	length = 0;
	bool	isDelHeader = false;
	while((length = read(sockfd, buffer, BUFFER_SIZE)) > 0)
	{
		if(!isDelHeader)
		{
			std::string httpresponse(buffer);
			std::string::size_type idx = httpresponse.find("\r\n\r\n");
			if(idx == std::string::npos)
			{
				isDelHeader = false;
			}
			else
			{
				size_t pos = httpresponse.find("\r\n\r\n");	
				char * p =  &buffer[pos + 4];
			 	if(fwrite(p , sizeof(char), (length - pos - 3), fp) !=  (length - pos -3))
				{
					std::cout << "fwrite wrong !"<<std::endl;
					return ;
				}
				isDelHeader = true;
			}
			bzero(buffer, BUFFER_SIZE);
			continue;
		}
		
		if(fwrite(buffer, sizeof(char), length, fp) != length)
		{
			std::cout<<"fwrite error";
		}
		bzero(buffer, BUFFER_SIZE);
		usleep(1);
	}

	std::cout<<"Receive File Success"<<std::endl;
	fclose(fp);
	close(sockfd);
}

void postData(std::string host, std::string path, std::string post_content)
{
	std::stringstream stream;
    	stream << "POST " << path;
    	stream << " HTTP/1.0\r\n";
    	stream << "Host: "<< host << "\r\n";
    	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
    	stream << "Content-Type:application/x-www-form-urlencoded\r\n";
    	stream << "Content-Length:" << post_content.length()<<"\r\n";
    	stream << "Connection:close\r\n\r\n";
    	stream << post_content.c_str();

    	socketHttp(host, stream.str());

}

void getData(std::string host, std::string path, std::string get_content)
{
	std::stringstream stream;
    	///stream << "GET " << path << "?" << get_content;
    	stream << "GET " << "/monster.jpg ";
		//stream<<"GET "<<"/graphics/gnu-and-penguin-color-300x276.jpg ";
    	stream << "HTTP/1.1\r\n";
    	stream << "Host: " << host << "\r\n";
    	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6) Gecko/20050225 Firefox/1.0.1\r\n";
    	stream << "Connection: Keep-Alive\r\n\r\n";

    	socketHttp(host, stream.str());
}


int main()
{
	// /www.gnu.org/graphics/gnu-and-penguin-color-300x276.jpg 
	getData("localhost", "", "");
	//getData("www.gnu.org", "", "");
	return 0;
}
