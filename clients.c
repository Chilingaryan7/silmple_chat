#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <assert.h>
#include <string.h>

#define BUF_SIZE 1024
typedef void *(*fptr)(void *);
void * pth_foo1(void *);
void * pth_foo2(void *);
int flag = 0;
int socket_fd = -1;
pthread_t threads[2];

int main(int argc, char * argv[]){
    fptr pth_foo[2] = {pth_foo1, pth_foo2};
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
            perror("socket error: ");
            exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(atoi(argv[1]));

    if(connect(socket_fd, (struct sockaddr *)&address, (socklen_t)sizeof(address))){
        perror("connect error: ");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < 2; ++i){
        if(pthread_create(&threads[i], NULL, pth_foo[i], NULL)){
            perror("pthread_create error: ");
            exit(EXIT_FAILURE);
        }
    }
    for(int i = 0; i < 2; ++i){
        if(pthread_join(threads[i], NULL)){
            perror("pthread_join error: ");
            exit(EXIT_FAILURE);
            }
    }


        
        close(socket_fd); 
        return 0;
}

void * pth_foo1(void * arg){
char buf[BUF_SIZE] = {0};
while(1){
    fgets(buf, BUF_SIZE - 2, stdin);
    write(socket_fd, buf, strlen(buf));
    if(!strcmp(buf, "exit\n")){
        flag = 1;
        break;
    }
    memset(buf, 0 , BUF_SIZE / sizeof(int));
}

    return NULL;
}

void * pth_foo2(void * arg){
    char buf[BUF_SIZE] = {0};
    while(!flag){
        memset(buf, 0, BUF_SIZE / sizeof(int));
        ssize_t bytes_read = read(socket_fd, buf, sizeof(buf));
         if (bytes_read <= 0) {
                break;
            }
        fputs(buf, stdout);
}
    return NULL;
}