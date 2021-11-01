#include <stdio.h>

#include "system.h"
#include "thread.h"

extern int userMain(int argc, char** argv);
extern int syncPrintf(const char *format, ...);

class Thread;
int main(int argc, char** argv){

	syncPrintf(" --- Poceo main ---\n");
	System::init();
	int y = userMain(argc, argv);
	int x = System::terminate();
	syncPrintf(" --- Zavrsio main ---\n");
	return 0;
}
