.global _start
_start:
    mov $55, %rdi     @ exit code
    mov $60, %rax     @ syscall number for exit
    syscall
    
