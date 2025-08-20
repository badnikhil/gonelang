.global _start
_start:
    mov $55, %rdi    
    mov $60, %rax     
    syscall
    