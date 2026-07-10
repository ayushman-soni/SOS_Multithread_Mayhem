# Week 4 Report – xv6 Setup & Codebase Exploration

Name:Ayushman Soni
Roll no: 24B0679

This week, I started working with xv6 and got an introduction to how a small operating system is organized. I completed Problem 1 (Hello xv6) and spent some time understanding the overall structure of the xv6 source tree and the purpose of different directories and files.

While reading the source code, I explored files such as `kernel/proc.h`, `kernel/proc.c`, `kernel/main.c`, `kernel/syscall.c`, and `user/sh.c`. It was interesting to see how system calls and processes are handled inside the kernel instead of just using them from user programs.

The most interesting thing I found was the implementation of `fork()`. Earlier, I only knew that `fork()` creates a new process, but after reading the xv6 source code, I got a better idea of how the kernel creates a new process and schedules it for execution.

One thing that surprised me was how small and readable xv6 is. Before this assignment, I assumed that an operating system would have a very large and complex codebase. Seeing that the important components are organized into a relatively small number of files made it much easier to understand the overall design.

Since I mainly focused on Problem 1 this week, I did not run the fork chain program yet. I plan to complete the remaining problems after finishing the xv6 setup and testing everything inside QEMU.

The biggest challenge for me was setting up the development environment on Windows. I realized that configuring WSL, the RISC-V toolchain, and QEMU is an important first step before testing any user programs.

Overall, this week's assignment gave me a good introduction to xv6 and helped me understand how user programs communicate with the operating system through system calls. I am looking forward to exploring more of the kernel and working on the upcoming assignments.