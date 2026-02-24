.data                                      # Data section

format: .asciz "|%s|\n"
format_ans: .asciz "%d\n"
format_test: .asciz "%c"
msg_err: .asciz "Wrong args (stdout)\n" # Message string for write

#define MSG_LEN 20

.text                                      # Text (code) section
.global main                               # Exportable object (label)

main:
    addi sp, sp, -22
    mv a0, sp            # First argument - format string
    call gets               # Call printf (external symbol)
    
    mv t0, sp
    li t1, 0

    li t4, 0 # num 2 start

while:
    lb t2, 0(t0)

    beq t2, x0, end
    
    li t3, 10
    beq t3, t2, end_n

    li t3, 32
    bne t2, t3, continue

    beq t1, x0, failure
    bne t4, x0, failure 

    mv t4, t1
    addi t4, t4, 1

continue:
    addi t0, t0, 1
    addi t1, t1, 1

    j while

end_n: # \n
    sb x0, 0(t0)    

end:
    beq t1, t4, failure # второй аргумент пуст
    beq x0, t4, failure # только первый аргумент
        
    la a0, format    
    mv a1, sp
    call printf

    mv t5, t4  # num2 start - 1
    addi t5, t5, -1

    mv t0, sp 
    add t0, t0, t5
    sb x0, 0(t0)

    mv a0, sp
    call atoi
    
    mv t1, a0

    mv a0, sp
    add a0, a0, t4
    call atoi

    mv t2, a0
        
    add t3, t1, t2

    la a0, format_ans
    mv a1, t3
    call printf

    li a7, 2
    li a0, 0
    ecall
        
failure:

    li a7, 16                 # Write syscall number
    li a0, 2                  # stderr fd (2)
    la a1, msg_err            # Error message string address 
    li a2, 20                 # Error message string length
    ecall                     # Syscall

    li a7, 2                  # Exit system call
    li a0, 1                  # Exit status (failure)
    ecall                     # Syscall
