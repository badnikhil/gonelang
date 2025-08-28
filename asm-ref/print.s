section .data
    msg db "Hello, World!", 0xA   ; message with newline
    len equ $ - msg               ; length of message

section .text
    global _start

_start:
    ; syscall: write(1, msg, len)
    mov rax, 1        ; syscall number for write
    mov rdi, 1        ; file descriptor (stdout)
    mov rsi, msg      ; pointer to string
    mov rdx, len      ; string length
    syscall

    ; syscall: exit(0)
    mov rax, 60       ; syscall number for exit
    xor rdi, rdi      ; return code 0
    syscall


