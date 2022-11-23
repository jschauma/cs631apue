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

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>


int a(char *s, jmp_buf env)
{
	int i;

	i = setjmp(env);
	printf("Setjmp returned -- i = %d\n", i);
	printf("In hex: %x\n", s);
	printf("%s\n", s);

	printf("s = %s\n", s);
	return i;
}

int b(int i, jmp_buf env)
{
	printf("In B: i=%d.  Calling longjmp(env, i)\n", i);

	longjmp(env, i);
}


int
main(int argc, char **argv)
{
	jmp_buf env;

	if (a("Jim", env) != 0) exit(0);
	b(3, env);
	return 0;
}
