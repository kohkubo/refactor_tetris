#include <unistd.h>
#include <stdio.h>

int main()
{
	usleep(-1);
	perror("a");
}
