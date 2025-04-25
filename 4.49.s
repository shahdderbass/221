/*
4.49 
Modify the code you wrote for Problem 4.47 to implement the test and swap in the
bubblesort function (lines 6–11) using no jumps and just one conditional move.

Assuming %r10 = *i
         %r11 = *(i+1)
*/

    rrmovq  %r10, %r12       # r12 = *i
    rrmovq  %r11, %r10       # *i  = *(i+1)
    rrmovq  %r12, %r11       # *(i+1) = *i

    andq    %r10, %r11       # compare *(i+1) with *i
    cmovle  %r12, %r10       # if *(i+1) <= *i

    rmmovq  %r10, (%rcx)     # store *i back in register
    rmmovq  %r11, (%rdx)     # store *(i+1) back in register

