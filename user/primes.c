#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int pleft[2]) {
    int p;
    // 从左侧管道读取第一个数,即当前素数。
    read(pleft[0], &p, sizeof(p));
    if(p == -1)  {
        // -1 作为哨兵，表示该分支后续没有数字。
        exit(0);
    }
    printf("prime %d\n", p);

    int pright[2];
    // 创建右侧管道，将剩余候选数传递给下一阶段。
    pipe(pright);

    if(fork() == 0) {
        // 子进程负责下一层筛选，从右侧管道读取数据。
        close(pright[1]);
        close(pleft[0]);
        sieve(pright);
        exit(0);  // 递归返回后退出
    } else {
        // 父进程过滤掉能被当前素数整除的数，并转发其余数字。
        close(pright[0]);
        int buf;
        while(read(pleft[0], &buf, sizeof(buf)) && buf != -1) {
            if(buf % p != 0) {
                write(pright[1], &buf, sizeof(buf));
            }
        }

        buf = -1;
        // 写入哨兵告知子进程结束，并等待其退出。
        write(pright[1], &buf, sizeof(buf));
        wait(0);
        exit(0);
    }    
}

int main(int argc, char* argv[]) {
    // 创建初始管道，向第一个筛选阶段提供 2~35 的数字。
    int input_pipe[2];
    pipe(input_pipe);

    if(fork() == 0) {
        // 子进程从管道读取数据并启动筛选。
        close(input_pipe[1]);
        sieve(input_pipe);
        exit(0);
    } else {
        close(input_pipe[0]);
        int i;
        // 父进程向管道写入 2~35 的候选数字。
        for (int i = 2; i <= 35; i++) {
            write(input_pipe[1], &i, sizeof(i));
        }

        i = -1;
        // 写入哨兵 -1 表示输入结束。
        write(input_pipe[1], &i, sizeof(i));
    }

    wait(0);
    exit(0);
}