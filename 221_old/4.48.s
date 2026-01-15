/*4.48 
Modify the code you wrote for Problem 4.47 to implement the test and swap in
the bubblesort function (lines 6–11) using no jumps and at most three conditional
moves.
*/
/*
Assuming %r10 = *i
	 %r11 = *(i+1)
*/

    rrmovq  %r10, %rax      # rax = *i
    andq    %r11, %rax      # Compare *i with *(i+1)

    cmovg   %r10, %r12      # r12 = *i if *i > *(i+1)
    cmovg   %r11, %r10      # *i = *(i+1) if *i > *(i+1)
    cmovg   %r12, %r11      # *(i+1) = *i if *i > *(i+1)

    rmmovq  %r10, (%rcx)    # store *i back in register
    rmmovq  %r11, (%rdx)    # store *(i+1) back in register

