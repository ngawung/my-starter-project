/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/input.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touch;

	consoleDemoInit();  //setup the sub screen for printing

	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\twww.devkitpro.org");

	while(1) {

		touchRead(&touch);
		iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);
		iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

		swiWaitForVBlank();
		scanKeys();
		// if (keysDown()&KEY_START) break;
	}

	return 0;
}
