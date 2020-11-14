#include "wcnet.h"

int main()
{
    unsigned long mode = -1;
    unsigned long disable = 0;
    SOCKET fd = Socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);

    if (Bind(fd,777) < 0)
        WSAstrerror(WSAGetLastError());
    
    if (Listen(fd,5) < 0)
        WSAstrerror(WSAGetLastError());
    
    SOCKET nfd = Accept(fd);
    char buffer[100] = {0};
    char buf[1024] = {0};
    fd_set rdfs;

    while (1)
    {
        FD_ZERO (&rdfs);
        FD_SET(_fileno(stdin),&rdfs);
        FD_SET(nfd,&rdfs);

        if (select(nfd, &rdfs,NULL,NULL,NULL) == -1)
        {
            perror("select()");
            strerror(errno);
            exit(errno);
        }

        if (FD_ISSET(_fileno(stdin),&rdfs))
        {
            fgets(buffer,99,stdin);
            char *p = NULL;
            p = strstr(buffer,"\n");

            if (p != NULL)
                *p = 0;
            else
                buffer[99] = 0;
            
            if (send(nfd,buffer,99,0) < 0)
                fprintf(stderr,"[-] Error send !\n");
        } else if (FD_ISSET(nfd,&rdfs))
        {
            int n = 0;

            if ((n = recv(nfd,buf,1023,0)) < 0)
                perror("recv()");
            
            buf[n] = 0;

            printf("%s",buf);
        }
    }


    closesocket(fd);
    return (0);
}