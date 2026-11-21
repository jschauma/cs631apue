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

/* This simple program can be used to illustrate how shared libraries
 * work.  We use function prototypes to forward-declare the 'ldtest'
 * functions, which are implemented in the ldtest[12].c files.
 *
 * Compiling this file alone works, but linking it will fail.
 *
 * Compiling main.c and ldtest[12].c in one step will work.
 *
 * Compiling main.c and ldtest[12].c into object files, then linking them
 * together into an executable will work.
 *
 * You can then also create an archive of the ldtest[12].o files and
 * create a dynamically linked executable from that; likewise, you can
 * then create a statically linked binary using the archive.
 *
 * Finally, you can build a shared library from the ldtest[12].o files and
 * link this program against it.
 */

#include <stdio.h>

void ldtest0(const char *);
void ldtest1(const char *);
void ldtest2(const char *);

int main() {
	ldtest0("Hello world!");
	ldtest1("Hello world!");
	ldtest2("Hello world!");
	return 0;
}
