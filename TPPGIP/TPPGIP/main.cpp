
#include "Context.h"

int main(int argc, char **argv)
{
	Context context;

	context.Init();
	context.mainLoop();

	return 1;
}