# Problem 1 – Trace a System Call (getpid)

## 1. User-space Stub

**File:** `user/usys.pl` (generates `user/usys.S`)

The `getpid()` function is called from a user program. The entry for `getpid` is defined in `user/usys.pl`, which generates the corresponding assembly stub in `user/usys.S`. The stub loads the system call number (`SYS_getpid`) into register `a7` and executes the `ecall` instruction to enter the kernel.

---

## 2. The ecall Instruction

The `ecall` instruction transfers execution from User Mode (U-mode) to Supervisor Mode (S-mode). During this transition, the processor saves the current program counter in `sepc`, records the trap reason in `scause`, and jumps to the address stored in `stvec`, which points to the kernel trap handler.

---

## 3. First xv6 Code After the Trap

**File:** `kernel/trampoline.S`

Execution enters the `uservec` routine. This code saves all user registers into the current process's trapframe so that the process can later resume execution exactly where it stopped.

---

## 4. Detecting the System Call

**File:** `kernel/trap.c`

The `usertrap()` function determines why the trap occurred. If the trap was caused by a system call from user mode, it advances the saved program counter and calls `syscall()` to handle the request.

---

## 5. System Call Dispatch

**File:** `kernel/syscall.c`

The `syscall()` function reads the system call number from register `a7`. It uses this number as an index into the `syscalls[]` table and invokes the corresponding kernel function. For `getpid()`, the selected function is `sys_getpid()`.

---

## 6. System Call Implementation

**File:** `kernel/sysproc.c`

The `sys_getpid()` function returns the PID of the currently running process using `myproc()->pid`. The returned value is placed in register `a0`, which is used by the RISC-V calling convention for function return values.

---

## 7. Returning to User Mode

After the system call finishes, control returns through `usertrapret()` in `kernel/trap.c`, followed by `userret` in `kernel/trampoline.S`. Finally, the `sret` instruction switches the processor back to User Mode, restores all saved registers, and resumes execution immediately after the original `ecall`.

---

## Execution Flow

```
User Program
      ↓
getpid()
      ↓
user/usys.S
      ↓
ecall
      ↓
kernel/trampoline.S (uservec)
      ↓
kernel/trap.c (usertrap)
      ↓
kernel/syscall.c (syscall)
      ↓
kernel/sysproc.c (sys_getpid)
      ↓
usertrapret()
      ↓
userret
      ↓
sret
      ↓
Back to User Program
```