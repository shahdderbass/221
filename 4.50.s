/*
4.50
Implement switchv in Y86-64 using a jump table. Although the Y86-64 instruction set does not include an indirect jump instruction, you can get the same effect by pushing a computed address onto the stack and then executing the ret instruction. Implement test code similar to what is shown in C to demonstrate that your implementation of switchv will handle both the cases handled explicitly as well as those that trigger the default case.
*/

switchv:
    irmovq  $5, %rsi      
    subq    %rdi, %rsi      # rsi = 5 - idx
    jbe     default         # if idx > 5 or idx < 0 jumps to default

    leaq    jt(%rip), %rdx  # rdx = address of jump table
    mrmovq  (%rdx,%rdi,8), %rax  # rax = offset from jump table
    addq    %rdx, %rax      
    pushq   %rax            
    ret                     

case0:
    irmovq  $0xaaa, %rax
    jmp     return

case2:
case5:
    irmovq  $0xbbb, %rax
    jmp     return

case3:
    irmovq  $0xccc, %rax
    jmp     return

default:
    irmovq  $0xddd, %rax

return:
    ret

jt:
    .quad case0-jt      # idx 0
    .quad other-jt      # idx 1
    .quad case2-jt      # idx 2
    .quad case3-jt      # idx 3
    .quad other-jt      # idx 4
    .quad case5-jt      # idx 5

