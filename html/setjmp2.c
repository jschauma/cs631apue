#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>


int proc_4(jmp_buf env, int i)
{
	if (i == 0) longjmp(env, 1);
	return 14 % i;
}

int proc_3(jmp_buf env, int i, int j)
{
	return proc_4(env, i) + proc_4(env, j);
}

int proc_2(jmp_buf env, int i, int j, int k)
{
	return proc_3(env, i, k) + proc_3(env, j, k);
}

int proc_1(jmp_buf env, int i, int j, int k, int l)
{
	return proc_2(env, i, j, k+1);
}

int
main(int argc, char **argv)
{
	jmp_buf env;
	int sj;
	int i, j, k, l;

	if (argc != 5) {
		fprintf(stderr, "usage: sj2 i j k l\n");
		exit(1);
	}

	sj = setjmp(env);
	if (sj != 0) {
		printf("Error -- bad value of i (%d), j (%d), k (%d), l (%d)\n",
				i, j, k, l);
		exit(0);
	}

	i = atoi(argv[1]);
	j = atoi(argv[2]);
	k = atoi(argv[3]);
	l = atoi(argv[4]);

	printf("proc_1(%d, %d, %d, %d) = %d\n", i, j, k, l, proc_1(env, i, j, k, l));
	return 0;
}
