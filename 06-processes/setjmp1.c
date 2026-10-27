/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * This file is in the public domain.
 *
 * Conceptually based on the code examples from
 * https://en.wikipedia.org/wiki/Setjmp.h
 *
 * You don't have to, but if you feel like
 * acknowledging where you got this code, you may
 * reference me by name, email address, or point
 * people to the course website:
 * https://stevens.netmeister.org/631/
 *
 * This program illustrates the functionality of the
 * setjmp(3)/longjmp(3) functions.  Compile with
 * debugging symbols, then
 *
 * - break near the setjmp call as well as the
 *   following else
 * - p &stack
 * - disassemble to show 'stack' being passed
 * - i r rbp rsp # to show current registers
 * - si # to step into __setjmp14
 * - i r rbp rsp # to show registers that will end up in jmp_buf
 * - finish
 * - p/x stack # to show value of registers in jmp_buf
 * - step though and into __longjmp14 to see addresses being restored
 *
 * See also:
 * /usr/include/machine/setjmp.h
 * /usr/src/lib/libc/arch/x86_64/gen/__setjmp14.S
 */

#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

static jmp_buf stack;

void func2() {
	int i;
	printf("Entering func2() near 0x%12lX.\n", (unsigned long)&i);
	longjmp(stack, 1);
	printf("Leaving func2().\n");
}

void func1() {
	int i;
	printf("Entering func1() near 0x%12lX.\n", (unsigned long)&i);
	func2();
	printf("Leaving func1().\n");
}

int main()
{
	int i;
	printf("Entering main()  near 0x%12lX.\n", (unsigned long)&i);
	if (setjmp(stack) == 0) {
		func1();
	} else {
		printf("Returning directly from func2().\n");
	}

	printf("Leaving main().\n");

	return EXIT_SUCCESS;
}
