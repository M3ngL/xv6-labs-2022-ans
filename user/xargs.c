#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void run(char *path, char **argv){
    if(fork() == 0){
        exec(path, argv);
        exit(1);
    }else{
        wait((int *) 0);
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("usage: xargs command [params].\n");
    }

    char *argsbuf[128];
    char **args_ponit = argsbuf;
    // 将argv先写入new_argv
    for(int i = 1; i < argc; i++){
        *args_ponit++ = argv[i];
    }
    // 获取console的输入并整合到new_argv
    char s, buf[512];
    char *p = buf;
    while(read(0, &s, sizeof(char)) == 1){
        if(s == '\n'){
            *p++ = '\0'; // 分割字符串
            *args_ponit++ = buf;
            run(argv[1], argsbuf);
            p = buf; // 重置指针
            args_ponit--; // 重置指针
            continue;
        }
        *p++ = s; // 获取console的连贯输入作为单个字符串
    }
    *p++ = '\0';
    *args_ponit++ = buf;
    run(argv[1], argsbuf);

    while (wait(0) != -1);
    exit(0);
}