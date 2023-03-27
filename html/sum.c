/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * This file is in the public domain.
 *
 * You don't have to, but if you feel like
 * acknowledging where you got this code, you may
 * reference me by name, email address, or point
 * people to the course website:
 * https://stevens.netmeister.org/631/
 */

/* Use this program to illustrate how the
 * call stack works by inspecting the various
 * registers using gdb(1).
 *
 * Also inspect the return values of the
 * differrent functions and how the behavior
 * changes when you exit versus return
 * from main.
 *
 * Note that you have to compile without
 * warnings since we don't (explicitly)
 * return in 'sum2'.
 *
 * cc -g sum.c
 * gdb a.out
 * display $rbp
 * display $rsp
 * display $rip
 * display $rax
 * break __start
 * break main
 * break sum
 * break sum2
 * run
 *
 * Also use this example to illustrate that
 * you can manipulate both variables and
 * registers while the program is running.
 */

#include <stdio.h>
#include <stdlib.h>

int
sum1(int a, int b) {
	int sum;
	sum = a + b;
	return sum;
}

int
sum2(int a, int b) {
	int sum = a + b;
	printf("sum2: %d\n", sum);
}

int
main() {
	int a = 1;
	int b = 2;

	a += b;
	printf("sum1 returned: %d\n", sum1(a, b));
	printf("sum2 returned: %d\n", sum2(b, a));
	return(0);
	//exit(0);
}
