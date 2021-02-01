//La librairie principale OSLib
#include <oslib/oslib.h>
#include "header.h"


//les callbacks
PSP_MODULE_INFO("Manga2PSP", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


int main()
{
	Initialisation();		
	
	//boucle principale
	while (!osl_quit)
	{
		//Permet de dessiner
		oslStartDrawing();

		if (!mangaloaded) oslClearScreen(RGB(255,255,255));	
		else oslClearScreen(RGB(0,0,0));

		Touches();
		
		Affichage();

		//Fin du dessin
		oslEndDrawing();		

		//Synchronise l'écran
		oslSyncFrame();	
	}
	
	//on quitte l'application
	oslEndGfx();
	oslQuit();
	return 0;
}


