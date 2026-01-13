#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // 打开路径（可能是文件或目录）
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // 获取文件/目录的状态信息
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
    case T_FILE:
        // 情况1: 如果是普通文件，检查文件名是否匹配
        // 从完整路径中提取文件名
        p = path + strlen(path);
        while(p >= path && *p != '/')
            p--;
        p++;  // p 现在指向文件名部分
        
        // 比较文件名和目标名称
        if (strcmp(p, target) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        // 情况2: 如果是目录，遍历目录内的所有项
        // 检查路径长度是否会溢出
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
            printf("find: path too long\n");
            break;
        }

        // 构造基础路径 "path/"
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        
        // 读取目录中的每一个目录项
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            // 跳过空目录项
            if (de.inum == 0)
                continue;
            
            // 跳过 "." 和 ".."，避免无限递归
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            
            // 构造完整路径 "path/filename"
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            
            // 递归查找子路径
            find(buf, target);
        }
        break;
    }

    // 关闭文件描述符
    close(fd);
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }

    // argv[1] 是搜索路径，argv[2] 是要查找的文件名
    find(argv[1], argv[2]);
    exit(0);
}