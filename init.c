#include <oslib/oslib.h>
#include "header.h"
#include "police.h"
#include "fond.h"
#include "etoile.h"
#include "texte.h"


void Initialisation()
{
	//Initialisation de la librairie
	oslInit(0);

	//Initialisation du mode graphique
	oslInitGfx(OSL_PF_5650, 1);		//5650

	//l'audio pour la lecture MP3
	pspAudioInit();

	//quitte si jamais y'a un blem de chargement
	oslSetQuitOnLoadFailure(1);
	
	//Configure l'autorepeat des touches
	oslSetKeyAutorepeatInit(35);
	oslSetKeyAutorepeatInterval(5);

	//lissage des pixels
	oslSetBilinearFilter(1);

	//active le dithering
	oslSetDithering(1);	

	//chargement des virtual files  
	OSL_VIRTUALFILENAME ram_files[] = {
		{"ram:/police.oft", (void*)police_data, sizeof(police_data), &VF_MEMORY},
		{"ram:/etoile.png", (void*)etoile_data, sizeof(etoile_data), &VF_MEMORY},
		{"ram:/texte.png", (void*)texte_data, sizeof(texte_data), &VF_MEMORY},
		{"ram:/fond.png", (void*)fond_data, sizeof(fond_data), &VF_MEMORY}
	};
	oslAddVirtualFileList(ram_files, oslNumberof(ram_files));

	//logo neoflash, à virer en temps utile
	file = fopen("logo/neoflash.png", "r");
	if (file) {fclose(file); oslShowSplashScreen(2);}

	//affiche le logo oslib
	logoAffiche();

	//chargement de ma police perso
	maFonte = oslLoadFontFile("ram:/police.oft"); 
	if (!maFonte) {} else oslSetFont(maFonte);	
	
	//création des images noires
	image2 = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
	imp = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
	imc = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
	imp2 = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
	prev2 = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
	oslClearImage(image2,RGB(0,0,0));
	oslClearImage(imp,RGB(0,0,0));
	oslClearImage(imc,RGB(0,0,0));
	oslSwizzleImage(image2);
	oslSwizzleImage(imp);
	oslSwizzleImage(imc);
	oslSwizzleImage(imp2);
		
	//init des variables et autres trucs
	milieu = 479;
	nmp3 = 2; //on se place après les . et ..
	select = 1;
	menu_position = 170;	
	txtalpha = 191;
	oslSetBkColor(RGBA(0,0,0,0));
	oslSetTextColor(RGBA(128,128,128,150));
	image1 = oslLoadImageFilePNG("ram:/fond.png", OSL_IN_RAM, OSL_PF_5551);
	image1->x = 120;
	image1->y = 6;

	//lecture du fichier de config
	RestaureConfig();		

	//on parcourt les répertoires
	recacheMangaDir();
	recacheMP3Dir();

	//config en fonction du fichier (ou pas)
	if (cpu == 96) scePowerSetClockFrequency (96, 96, 48);
	else scePowerSetClockFrequency (222, 222, 111);

	//si y'a un marque page
	if (strlen(marquepage) > 1 && boot) 
	{
		//affichage du loading	
		oslStartDrawing();
		oslSetDrawBuffer(OSL_SECONDARY_BUFFER);
		oslClearScreen(RGB(255,255,255));
		oslDrawImage(image1);
		if (langue == francais) oslPrintf_xy(391,251,"Chargement...");	
		else oslPrintf_xy(411,251,"Loading...");
		oslSetTextColor(RGB(0,0,0));
		if (langue == francais) oslPrintf_xy(390,250,"Chargement...");	
		else oslPrintf_xy(410,250,"Loading...");
		oslEndDrawing();	

		ChargerMarquePage();
	}
	else 
	{
		safe_strcpy(mangapath, mangapatho, sizeof(mangapath));
		recacheMangaDir();		
		menu = 1;	
		preload = 1;
	}	

	//testsens = 1;
}


