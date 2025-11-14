#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int pingpong[2];
    if(pipe(pingpong) < 0){
        printf("pingpong: pipe failed\n");
        exit(1);
    }
    // printf("%d, %d", pingpong[0], pingpong[1]);

    int pid = fork();
    if(pid == 0){
        int child = getpid();
        char msg[4];
        if(read(pingpong[0], msg, 4) != -1){
            printf("%d: received %s\n", child, msg);
            strcpy(msg, "pong");
            write(pingpong[1], msg, 4);
            exit(0);
        }
    }else{
        int parent = getpid();
        char msg[4] = "ping";
        write(pingpong[1], msg, 4);
        if(read(pingpong[0], msg, 4) != -1){
            printf("%d: received %s\n", parent, msg);
        }
    }
    exit(0);
}