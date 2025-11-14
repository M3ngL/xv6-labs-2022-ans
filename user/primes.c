#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter(int *pre_p){
    close(pre_p[1]);

    int prime;
    if(read(pre_p[0], &prime, 4) != 4){ // 递归的退出条件判断
        close(pre_p[0]);
        exit(0);
    }
    printf("prime %d\n", prime);

    // 创建管道
    int p[2];
    if(pipe(p) < 0){
        printf("error: pipe create p wrong.");
        exit(1);
    }

    // 并发运行
    if(fork() == 0){
        filter(p); // 进入递归
        exit(0);
    }else{
        close(p[0]); // 将父进程的所有管道都关闭，这样才能正常退出程序
        int num;
        while(read(pre_p[0], &num, 4) == 4){
            if(num % prime != 0){
                write(p[1], &num, 4);
            }
        }
        close(pre_p[0]);
        close(p[1]);

        wait((int*)0); // 父进程都必须wait所有的子进程
    }
}

int main(){
    int p[2];
    if(pipe(p) < 0){
        printf("error: pipe create p wrong.");
        exit(1);
    }
    for(int i = 2; i <= 35; i++){
        write(p[1], &i, 4);
    }
    filter(p);
    exit(0);
}
