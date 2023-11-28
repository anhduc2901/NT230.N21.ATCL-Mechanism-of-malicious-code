#include<string.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
//#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define NAME_SIZE 2048

int handling(int c)
{
  char buffer[BUFFER_SIZE], name[NAME_SIZE];
  int bytes;
  int i;
  printf("address %p\n",buffer);
  strcpy(buffer, "My name is: ");
  bytes = send(c, buffer, strlen(buffer), 0);
  if (bytes == -1)
    return -1;
  bytes = recv(c, name, sizeof(name), 0);
  if (bytes == -1)
    return -1;
   name[bytes-2] = 0;    
  sprintf(buffer, "Hello :%s, welcome to our site", name);  
  bytes = send(c, buffer, strlen(buffer), 0);
  if (bytes == -1)
    return -1;
  return 0;
}

int main(int argc, char *argv[])
{
  int s, c;
  socklen_t cli_size;
  struct sockaddr_in srv, cli;
  cli_size = sizeof(cli);
  if (argc != 2)
    {
      fprintf(stderr, "usage: %s port\n", argv[0]);
      return 1;
    }
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1)
    {
      perror("socket() failed");
      return 2;
    }
  srv.sin_addr.s_addr = INADDR_ANY;
  srv.sin_port = htons( (unsigned short int) atol(argv[1]));
  srv.sin_family = AF_INET;
  if (bind(s, (struct sockaddr *)&srv, sizeof(srv)) == -1)
    {
      perror("bind() failed");
      return 3;
    }
  if (listen(s, 3) == -1)
    {
      perror("listen() failed");
      return 4;
    }
  for(;;)
    {
      c = accept(s, (struct sockaddr *)&cli, &cli_size);
      if (c == -1)
	{
	  perror("accept() failed");
	  return 5;
	}
      printf("client from %s", inet_ntoa(cli.sin_addr));
      if (handling(c) == -1)
	fprintf(stderr, "%s: handling() failed", argv[0]);
      close(c);
    }
  
  return 0;
}
