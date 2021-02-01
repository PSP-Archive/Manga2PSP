#include <oslib/oslib.h>
#include "header.h"


void Touches()
{
	if (!menu) {

	//Lit les touches
	oslReadKeys();

	if (((osl_keys->pressed.L && !profil) || (osl_keys->pressed.R && profil)) && page > saut) //   || test < 0
	{
		//test = 0;
		page--;
		//if (page <= 0) testsens = 1;
		
		if (cache == 1 || retour > 0) 
		{
			if (!retour) {retour = 1;}
			cache = 0;

			if (imc) oslDeleteImage(imc);
			imc = oslCreateImageCopy(image1, OSL_IN_RAM);
			oslImageIsSwizzledSet(imc, oslImageIsSwizzled(image1));
									
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImageCopy(image2, OSL_IN_RAM); //stockage de l'ancienne image
			oslImageIsSwizzledSet(imp2, oslImageIsSwizzled(image2));
			
			if (ims) oslDeleteImage(ims); //libère la mémoire
			ChargementImage();			
			if (image1) oslDeleteImage(image1);
			image1 = oslCreateImageCopy(ims, OSL_IN_RAM);
			oslImageIsSwizzledSet(image1, oslImageIsSwizzled(ims));
						
			if (ims) oslDeleteImage(ims);
			ims = oslCreateImageCopy(imc, OSL_IN_RAM);
			oslImageIsSwizzledSet(ims, oslImageIsSwizzled(imc));
		}

		else if (!cache || cache == 2) // si c'est la 1ere pression sur L
		{
			if (image1) oslDeleteImage(image1);
			image1 = oslCreateImageCopy(imp, OSL_IN_RAM); //on affiche l'image précedente
			oslImageIsSwizzledSet(image1, oslImageIsSwizzled(imp));
			
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImageCopy(image2, OSL_IN_RAM); //stockage de l'ancienne image
			oslImageIsSwizzledSet(imp2, oslImageIsSwizzled(image2));	
			
			cache = 1; //veut dire qu'on a pressé une fois sur L, c'est dire qu'on affiche l'image en cache			
		}

		NouvellePage();
	}
	if (( ((osl_keys->pressed.R && !profil) || (osl_keys->pressed.L && profil)) && (page - saut) == nbrpages - 1))
	{
		tmpinfo = 1;
		txtinfo = fintome;
		txtalpha = 191;
	}
	if (((osl_keys->pressed.R && !profil) || (osl_keys->pressed.L && profil)) && (page - saut) < nbrpages - 1)
	{
		//test = 0;
		page++;
		//if (page >= nbrpages) testsens = -1; 

		//l'image en cache n'est pas affichée et ne l'a jamais été encore
		if (!cache)
		{
			if (imp) oslDeleteImage(imp);
			imp = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'ancienne image	
			oslImageIsSwizzledSet(imp, oslImageIsSwizzled(image1));
			
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImageCopy(image2, OSL_IN_RAM); //stockage de l'ancienne image
			oslImageIsSwizzledSet(imp2, oslImageIsSwizzled(image2));			
			
			if (image1) oslDeleteImage(image1);
			image1 = oslCreateImageCopy(ims, OSL_IN_RAM); //on affiche l'image suivante
			oslImageIsSwizzledSet(image1, oslImageIsSwizzled(ims));
			
			if (imc) oslDeleteImage(imc);
			imc = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'image actuelle		
			oslImageIsSwizzledSet(imc, oslImageIsSwizzled(image1));
			
			if (ims) oslDeleteImage(ims);
									
			retour = 0; //variable debug car j'ai la flemme de trop reflechir au code
			preload = 0; //prépare à lancer le préchargement de l'image suivante	
						
		}
		
		//l'image en cache est affichée
		else if (cache == 1)
		{
			if (image1) oslDeleteImage(image1);
			image1 = oslCreateImageCopy(imc, OSL_IN_RAM);	
			oslImageIsSwizzledSet(image1, oslImageIsSwizzled(imc));
						
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImageCopy(image2, OSL_IN_RAM); //stockage de l'ancienne image
			oslImageIsSwizzledSet(imp2, oslImageIsSwizzled(image2));
			
			cache = 2;		
		}

		//l'image en cache a été affichée
		else if (cache == 2)
		{
			if (imp) oslDeleteImage(imp); //libere la mémoire
			imp = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'ancienne image
			oslImageIsSwizzledSet(imp, oslImageIsSwizzled(image1));
						
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImageCopy(image2, OSL_IN_RAM); //stockage de l'ancienne image
			oslImageIsSwizzledSet(imp2, oslImageIsSwizzled(image2));
			
			oslDeleteImage(image1);			
			image1 = oslCreateImageCopy(ims, OSL_IN_RAM); //on affiche l'image suivante
			oslImageIsSwizzledSet(image1, oslImageIsSwizzled(ims));

			if (imc) oslDeleteImage(imc);
			imc = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'image actuelle
			oslImageIsSwizzledSet(imc, oslImageIsSwizzled(image1));
						
			cache = 0;
			preload = 0; //prépare à lancer le préchargement de l'image suivante
			
			if (ims) oslDeleteImage(ims);			
		}

		NouvellePage();
	}

	//lecteur MP3
	if ( ((osl_keys->pressed.up && !profil) || (osl_keys->pressed.triangle && profil)) && mp3present > 0) 
	{
		if (!mp3play)
		{
			scePowerSetClockFrequency (222, 222, 111);
			cpu = 222;
			MP3_Init(1); 
			ChargementMP3(); 
			MP3_Play();
		}
		else MP3_Pause();
	}
	if (mp3play > 0)
	{
		if (((osl_keys->pressed.down && !profil) || (osl_keys->pressed.cross && profil))) MP3_End(); 
		if (((osl_keys->pressed.left && !profil) || (osl_keys->pressed.square && profil)) && nmp3 > 2) {smp3 = 1; nmp3--; MP3_End(); MP3_Init(1); ChargementMP3(); MP3_Play();}
		if (((osl_keys->pressed.right && !profil) || (osl_keys->pressed.circle && profil)) && nmp3 < (MP3EntriesTotal-1)) {smp3 = 0; nmp3++; MP3_End(); MP3_Init(1); ChargementMP3(); MP3_Play();}
	}
	
	//gestion image
	if (((osl_keys->pressed.triangle && !profil) || (osl_keys->pressed.up && profil))) DoublePage();	
	if (((osl_keys->pressed.square && !profil) || (osl_keys->pressed.left && profil))) ZoomImage();	
	if (((osl_keys->pressed.cross && !profil) || (osl_keys->pressed.down && profil))) 
	{
		oslWriteImageFilePNG(OSL_SECONDARY_BUFFER, "screenshot.png", 0);
		tmpinfo = 1;
		txtinfo = screenshot;
		txtalpha = 191;
	}
	if (((osl_keys->pressed.circle && !profil) || (osl_keys->pressed.right && profil))) 
	{
		SauverMarquePage(); 
		SauveConfig();
		tmpinfo = 1;
		txtinfo = bookmark;
		txtalpha = 191;
	}
	if (osl_keys->pressed.start && !menu) 
	{
		menu = 1; //affiche le menu
		menu_select = 0; 
		menu_position = 170; 
		menu_position2 = 0;
		liste_mangas = 0;
		feuilleter = 0;
	}

	//stick
	for (i=32;i<=120;i+=48)
	{	
		if (osl_keys->analogY > i) 
		{
			image1->y -=4; 
			if ((doublep > 0) && (retour > 0 || cache == 1)) imc->y -=4; 
			else {imp->y -=4;} 			
		}
		if (osl_keys->analogY< -i) 
		{
			image1->y +=4; 
			if ((doublep > 0) && (retour > 0 || cache == 1)) imc->y +=4; 
			else {imp->y +=4;}
		}
		if (osl_keys->analogX > i) 
		{
			if (doublep > 0 && !zoom) {} 
			else {image1->x -=4;} 
		}
		if (osl_keys->analogX< -i) 
		{
			if (doublep > 0 && !zoom) {} 
			else {image1->x +=4;}
		}			
	}

	//if (osl_keys->pressed.home) oslQuit();
}}
