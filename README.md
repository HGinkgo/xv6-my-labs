# xv6-my-labs

这是我学习 xv6 操作系统课程实验的代码仓库，基于 MIT xv6-riscv / xv6-labs 完成。仓库记录了课程前八章相关实验内容，重点围绕用户态工具、系统调用、页表、陷入机制、惰性内存分配、写时复制以及线程切换等核心操作系统机制展开。

本仓库采用分支管理实验代码：`main` 分支作为说明页，各实验实现分别保存在对应分支中。

## 实验进度

| 分支 | 实验主题 | 主要内容 | 状态 |
| --- | --- | --- | --- |
| [`util`](https://github.com/HGinkgo/xv6-my-labs/tree/util) | 用户态工具 | `sleep`、`pingpong`、`primes`、`find`、`xargs`，理解 `fork`、`exec`、`wait`、`pipe` 等 Unix 接口 | 已完成 |
| [`syscall`](https://github.com/HGinkgo/xv6-my-labs/tree/syscall) | 系统调用 | 添加 `trace`、`sysinfo`，理解系统调用从用户态进入内核态的完整路径 | 已完成 |
| [`pgtbl`](https://github.com/HGinkgo/xv6-my-labs/tree/pgtbl) | 页表机制 | `ugetpid`、`vmprint`、`pgaccess`，理解 Sv39 页表、PTE 标志位和虚拟地址转换 | 已完成 |
| [`traps`](https://github.com/HGinkgo/xv6-my-labs/tree/traps) | 中断与异常 | `backtrace`、`alarm`，理解 trap、trapframe、trampoline 和用户态/内核态切换 | 已完成 |
| [`lazy`](https://github.com/HGinkgo/xv6-my-labs/tree/lazy) | 惰性内存分配 | 延迟 `sbrk` 的物理页分配，在 page fault 时按需分配内存 | 已完成 |
| [`cow`](https://github.com/HGinkgo/xv6-my-labs/tree/cow) | 写时复制 | 实现 Copy-on-Write fork，减少 `fork` 时的物理内存复制开销 | 已完成 |
| [`thread`](https://github.com/HGinkgo/xv6-my-labs/tree/thread) | 用户级线程 | 实现用户线程切换、寄存器保存恢复，并完成多线程同步相关实验 | 已完成 |

## 运行方式

先克隆仓库并切换到对应实验分支：

```bash
git clone https://github.com/HGinkgo/xv6-my-labs.git
cd xv6-my-labs
git checkout util
```

编译并启动 xv6：

```bash
make qemu
```

运行对应实验测试：

```bash
make grade
```

如果本地 RISC-V 工具链前缀不同，可以根据环境调整 `TOOLPREFIX`：

```bash
make TOOLPREFIX=riscv64-linux-gnu- qemu
```

## 环境依赖

推荐在 Linux 或 WSL 环境下运行，需要准备：

- RISC-V 交叉编译工具链
- QEMU RISC-V 模拟器
- `make`
- `python3`
- `gdb-multiarch`，用于调试

Ubuntu / WSL 可参考安装：

```bash
sudo apt update
sudo apt install -y build-essential git make python3 gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```

## 分支说明

实验代码不合并到 `main`，每个实验分支保留该实验对应的最终实现。查看某个实验时，直接切换到对应分支即可：

```bash
git branch -r
git checkout syscall
git checkout pgtbl
git checkout traps
git checkout lazy
git checkout cow
git checkout thread
```

## 参考

- [MIT 6.S081: Operating System Engineering](https://pdos.csail.mit.edu/6.S081/)
- [xv6: a simple, Unix-like teaching operating system](https://github.com/mit-pdos/xv6-riscv)

本仓库主要用于课程实验记录和操作系统学习总结。
