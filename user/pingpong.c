#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);

    if (fork() != 0) {
        // 父进程
        // 关闭不需要的管道端
        close(p2c[0]);  // 关闭父到子管道的读端
        close(c2p[1]);  // 关闭子到父管道的写端
        
        // 向子进程发送字节
        write(p2c[1], ".", 1);
        close(p2c[1]);  // 发送完成后关闭写端

        // 从子进程读取字节
        char buf;
        read(c2p[0], &buf, 1);
        printf("%d: received pong\n", getpid());
        
        close(c2p[0]);  // 关闭读端
        wait(0);        // 等待子进程结束
    } else {
        // 子进程
        // 关闭不需要的管道端
        close(p2c[1]);  // 关闭父到子管道的写端
        close(c2p[0]);  // 关闭子到父管道的读端
        
        // 从父进程读取字节
        char buf;
        read(p2c[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        
        // 向父进程发送字节
        write(c2p[1], &buf, 1);
        close(c2p[1]);  // 发送完成后关闭写端
        close(p2c[0]);  // 关闭读端
    }
    
    exit(0);
}