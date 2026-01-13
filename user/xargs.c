#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

// 在子进程里面执行 xargs 后面指定的程序
void run(char* program, char** argv) {
    if (fork() == 0) {
        // 子进程执行命令
        exec(program, argv);
        // 如果 exec 失败
        fprintf(2, "exec %s failed\n", program);
        exit(1);
    } else {
        // 父进程等待子进程完成
        wait(0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs <command> [args...]\n");
        exit(1);
    }

    char buf[2048];         // 输入缓冲区
    char* argsbuf[MAXARG];  // 参数指针数组
    int argindex = 0;       // 当前参数索引
    
    // 复制命令行参数到 argsbuf (从 argv[1] 开始)
    for (int i = 1; i < argc; i++) {
        argsbuf[argindex++] = argv[i];
    }
    
    int baseargc = argindex;  // 记录基础参数数量
    char* p = buf;            // 当前写入位置
    char* arg_start = buf;    // 当前参数的起始位置
    
    while (read(0, p, 1) == 1) {
        if (*p == ' ' || *p == '\n') {
            // 遇到分隔符
            char sep = *p;  // 保存分隔符类型
            *p = '\0';      // 终止当前参数字符串
            
            // 如果有内容，添加到参数列表
            if (arg_start < p) {
                argsbuf[argindex++] = arg_start;
            }
            
            // 如果是换行符，执行命令
            if (sep == '\n') {
                argsbuf[argindex] = 0;  // NULL 结尾
                run(argv[1], argsbuf);
                
                // 重置状态，准备下一行
                argindex = baseargc;
                p = buf;
                arg_start = buf;
                continue;
            }
            
            // 更新下一个参数的起始位置
            arg_start = p + 1;
        }
        p++;
    }
    
    // 处理最后一行（如果有的话）
    if (arg_start < p) {
        *p = '\0';
        argsbuf[argindex++] = arg_start;
        argsbuf[argindex] = 0;
        run(argv[1], argsbuf);
    }
    
    exit(0);
}

