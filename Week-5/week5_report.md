# Week 5 Report – System Calls

**Name:** Ayushman Soni

This week I studied how a system call travels from a user program into the xv6 kernel and back to user space. Instead of only using system calls like `getpid()`, I followed the complete execution path by reading the relevant source files in the xv6 kernel.

For this week's work, I completed **Problem 1 (Trace a System Call)**. I explored the files responsible for handling a system call, including `user/usys.pl`, `user/usys.S`, `kernel/trampoline.S`, `kernel/trap.c`, `kernel/syscall.c`, and `kernel/sysproc.c`.

The most interesting part was understanding that a simple function call like `getpid()` actually involves switching the processor from user mode to supervisor mode using the `ecall` instruction. I also learned how xv6 saves the user registers, identifies the requested system call, executes the corresponding kernel function, and safely returns to user mode.

One thing that surprised me was how many different components are involved in processing even a very small system call. Before reading the source code, I expected `getpid()` to be a direct function call, but it actually passes through multiple assembly and kernel routines before returning the result.

The main challenge was understanding the interaction between the assembly code in the trampoline and the C code inside the kernel. Reading the execution flow step by step made the overall design much easier to understand.

Overall, this week's assignment gave me a much better understanding of the boundary between user space and kernel space and how xv6 implements system calls internally.
