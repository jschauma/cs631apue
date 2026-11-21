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
