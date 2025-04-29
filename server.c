#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENT_COUNT 16
void* pth_foo(void *);
pthread_mutex_t mtx = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
int clinet_sockets_fd[MAX_CLIENT_COUNT] = {0};
pthread_t client_threads[MAX_CLIENT_COUNT];
int client_index = 0;
int clinets_count = 0;
//int flag = 0;
int main(int argc, char * argv[]){
    char buffer[BUFFER_SIZE];
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(socket_fd > 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));

    if(bind(socket_fd, (struct sockaddr *) &address,(socklen_t) sizeof(address))){
            perror("bind error: ");
            exit(EXIT_FAILURE);
    }

    if(listen(socket_fd, MAX_CLIENT_COUNT)){
        perror("listen error : ");
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting...\nPort number -> %d\n", atoi(argv[1]));
    size_t size_addr = sizeof(struct sockaddr_in);
    while(1){
    int * ind = (int *) calloc(sizeof(int), 1);
    assert (ind != NULL);

    *ind = -1;

    for(int i = 0; i < MAX_CLIENT_COUNT; ++i){
        if(clinet_sockets_fd[i] == -1){
            *ind = i;
            break;
        }
    }

    if(*ind < 0){
        *ind = client_index++;
    }

    clinet_sockets_fd[*ind] = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&size_addr);
        if(clinet_sockets_fd[*ind] < 0){
            perror("accept error: ");
            exit(EXIT_FAILURE);   
        }
        pthread_mutex_lock(&mtx);
        ++clinets_count; 
        pthread_mutex_unlock(&mtx);   
        printf("server connect with user\n");
        if(pthread_create(&client_threads[*ind], NULL, pth_foo, (void*)ind)){
            perror("pthread_create error: ");
            exit(EXIT_FAILURE);
        }
    pthread_detach(client_threads[*ind]);
    }
    close(socket_fd);
    for(int i = 0; i < clinets_count; ++i){
        close(clinet_sockets_fd[i]);
    }
    return 0;
}
void * pth_foo(void *arg){
    char buf[BUFFER_SIZE] = {0};
    int * index = (int *) arg;

    while(1){
            int k = read(clinet_sockets_fd[*index], buf, BUFFER_SIZE);
            buf[k] = '\0';
            for(int i = 0; i < clinets_count; ++i){
                if(i == *index || clinet_sockets_fd[i] <= 0){
                    continue;
                }

            write(clinet_sockets_fd[i], buf, strlen(buf));
            }
            if(!strcmp(buf, "exit\n")){
                    break;
             memset(buf, 0, BUFFER_SIZE / sizeof(int));
            }
    }
            
    close(clinet_sockets_fd[*index]);
    clinet_sockets_fd[*index] = -1;
    free(index);
    index = NULL;
    pthread_cancel(pthread_self());

    return NULL;
}