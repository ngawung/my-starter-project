#include <iostream>
#include <nds.h>


int main()
{
	nocashMessage("Hello world!");

	consoleDemoInit();
	std::cout << "Hello World!";

	while(true)
	{
		swiWaitForVBlank();
	}

	return 0;

}
