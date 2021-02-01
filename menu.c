#include <oslib/oslib.h>
#include "header.h"

void Menu()
{
	oslReadKeys();

	//if (osl_keys->pressed.home) oslQuit();
	if (mangaloaded && (osl_keys->pressed.start || (osl_keys->pressed.circle && !menu_position2 && !liste_mangas && !feuilleter))) menu = 0;
	if (osl_keys->pressed.up) {if (!menu_position2) menu_select--; else menu_select2--;}
	if (osl_keys->pressed.down) {if (!menu_position2) menu_select++; else menu_select2++;}
	
	//naviguer dans les menus
	if (!menu_position2)
	{
		if (menu_select < 0) menu_select = 6;
		if (menu_select > 6) menu_select = 0;
	}
	else if (menu_position2 == menu_mangas)
	{
		if (menu_select2 < 0) menu_select2 = 3;
		if (menu_select2 > 3) menu_select2 = 0;
	}
	else if (menu_position2 == menu_bookmark)
	{
		if (menu_select2 < 0) menu_select2 = 1;
		if (menu_select2 > 1) menu_select2 = 0;
	}
	else if (menu_position2 == menu_profils)
	{
		if (menu_select2 < 0) menu_select2 = 1;
		if (menu_select2 > 1) menu_select2 = 0;
	}
	else if (menu_position2 == menu_mp3)
		{
		if (menu_select2 < 0) menu_select2 = 4;
		if (menu_select2 > 4) menu_select2 = 0;
	}
	else if (menu_position2 == menu_options)
	{
		if (menu_select2 < 0) menu_select2 = 3;
		if (menu_select2 > 3) menu_select2 = 0;
	}
	
	//menus secondaires
	if (osl_keys->pressed.right || (osl_pad.released.cross && !menu_position2 && !liste_mangas && !feuilleter)) //affichage des menus secondaires
	{		
		if (menu_select == 0) {menu_select2 = 0; menu_position = 110; menu_position2 = menu_mangas;}
		else if (menu_select == 1) {menu_select2 = 0; menu_position = 110; menu_position2 = menu_bookmark;}
		else if (menu_select == 2) {menu_select2 = 0; menu_position = 110; menu_position2 = menu_profils;}
		else if (menu_select == 3) {menu_select2 = 0; menu_position = 110; menu_position2 = menu_mp3;}
		else if (menu_select == 4) {menu_select2 = 0; menu_position = 110; menu_position2 = menu_touches;}
		else if (menu_select == 5) {menu_select2 = 0; menu_position = 110; menu_position2 = menu_options;}
	}
	else if (osl_pad.released.cross && menu_position2 > 0 && !liste_mangas && !feuilleter) //quand on selectionne un truc dans les menus secondaires
	{
		if (menu_position2 == menu_mangas) 
		{
			if (menu_select2 == 0) 
			{
				liste_mangas = 1; 
				select = numanga + 2;
				sel_f = ((2-select)+6)*16+4;
				temp_prev = 1;
			}			
			if (menu_select2 == 1 && numanga < mangaEntriesTotal && mangaloaded) //manga suivant
			{
				nextmanga = 1;
				prevmanga = 0;
				wft2 = 1;
			}
			if (menu_select2 == 2 && numanga > 0 && mangaloaded) //manga precedent
			{
				prevmanga = 1;
				nextmanga = 0;
				wft2 = 1;
			}
			if (menu_select2 == 3 && mangaloaded) 
			{
				feuilleter = 1; 
				select = page - saut; 
				sel_f = ((2-select)+6)*16+4;
				temp_prev = 1;	
			}	
		}
		if (menu_position2 == menu_bookmark) 
		{
			if (menu_select2 == 0 && mangaloaded) 
			{
				SauverMarquePage(); 
				SauveConfig();
				menu_position2 = 0; 
				menu_select = 1; 
				menu_position = 170;
			}
			if (menu_select2 == 1) {wft2 = 1; loadbookmark = 1;}			
		}
		if (menu_position2 == menu_profils) 
		{
			if (menu_select2 == 0) profil = 0;
			if (menu_select2 == 1) profil = 1;
			menu_select = 2; 
			menu_position2 = 0; 		
			menu_position = 170;
			SauveConfig();
		}
		if (menu_position2 == menu_mp3) 
		{
			if (menu_select2 == 0 && !mp3play && mp3present > 0) //MP3 Start
			{
				scePowerSetClockFrequency (222, 222, 111); 
				cpu = 222; 
				MP3_Init(1); 
				ChargementMP3(); 
				MP3_Play();
			}
			if (menu_select2 == 1 && mp3present > 0) MP3_Pause();
			if (mp3play > 0)
			{
				if (menu_select2 == 2) MP3_End();
				if (menu_select2 == 4 && nmp3 > 2) {smp3 = 1; nmp3--; MP3_End(); MP3_Init(1); ChargementMP3(); MP3_Play();}
				if (menu_select2 == 3 && nmp3 < (MP3EntriesTotal-1)) {smp3 = 0; nmp3++; MP3_End(); MP3_Init(1); ChargementMP3(); MP3_Play();}
			}
		}
		if (menu_position2 == menu_touches) {menu_position2 = 0; menu_select = 4; menu_position = 170;}
		if (menu_position2 == menu_options) 
		{
			modif = 1; //faudra sauvegarder tout a l'heure

			if (menu_select2 == 0) 
			{
				if (!sens) sens = 1; 
				else sens = 0;
				if (zoom) ZoomImage();
				if (retour > 0 || cache == 1) {doublep = 1; DoublePage();}
			}
			if (menu_select2 == 1) {if (!langue) langue = 1; else langue = 0;}
			if (menu_select2 == 2 && (strlen(marquepage) > 0)) 
			{
				if (!boot) boot = bookmark; 
				else boot = menup;
			}
			if (menu_select2 == 3) 
			{
				if (cpu == 222)
				{
					if (!mp3play) {cpu = 96; scePowerSetClockFrequency (96, 96, 48);}
				}
				else {cpu = 222; scePowerSetClockFrequency (222, 222, 111);}
			}
		}		
	}

	//retour au menu principal
	if (osl_keys->pressed.left || (osl_keys->pressed.circle && menu_position2 > 0))
	{
		if (modif > 0) SauveConfig(); //on ne sauvegarde que si on a fait une modif
		if (menu_position2 == menu_mangas) {menu_position2 = 0; menu_select = 0; menu_position = 170;}
		if (menu_position2 == menu_bookmark) {menu_position2 = 0; menu_select = 1; menu_position = 170;}
		if (menu_position2 == menu_profils) {menu_position2 = 0; menu_select = 2; menu_position = 170;}
		if (menu_position2 == menu_mp3) {menu_position2 = 0; menu_select = 3; menu_position = 170;}
		if (menu_position2 == menu_touches) {menu_position2 = 0; menu_select = 4; menu_position = 170;}
		if (menu_position2 == menu_options) {menu_position2 = 0; menu_select = 5; menu_position = 170;}
	}

	//fond du menu (noir transparent)
	oslDrawFillRect(0,0,480,272,RGBA(0,0,0,210));
	oslSetBkColor(RGBA(0,0,0,0));
	oslSetTextColor(RGB(255,255,255));

	
	//oslPrintf_xy(10,30,"cache:%d",cache);
	//oslPrintf_xy(10,50,"retour:%d",retour);
	//oslPrintf_xy(10,70,"numanga:%d",numanga);
	//oslPrintf_xy(10,70,"image1->x:%d",image1->x);

	
	//AfficheTime();	
	
	//affichage du menu
	if (!liste_mangas && !feuilleter && !wft2)
	{		
		//petites infos
		if (mp3play > 0) oslPrintf_xy(10,25,"%s",MP3Entry[nmp3].name);
		if (mangaloaded)
		{
			oslPrintf_xy(10,5,"%s", mangaEntry[numanga+2].name);	
			oslPrintf_xy(372,5,"Page : %d/%d",(page+1-saut), (nbrzipfiles-saut));
		}

		if (langue == francais)
		{			
			//menu principal
			if (menu_select == 0) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,70,"Gestion mangas ->");
			else oslPrintf_xy(menu_position,70,"Gestion mangas  <-");
			if (menu_select == 1) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,90,"Marque-page ->");
			else oslPrintf_xy(menu_position,90,"Marque-page  <-");
			if (menu_select == 2) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,110,"Profil %d ->",(profil+1));
			else oslPrintf_xy(menu_position,110,"Profil %d  <-",(profil+1));
			if (menu_select == 3) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,130,"Lecteur MP3 ->");
			else oslPrintf_xy(menu_position,130,"Lecteur MP3  <-");
			if (menu_select == 4) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,150,"Afficher touches ->");
			else oslPrintf_xy(menu_position,150,"Afficher touches <-");
			if (menu_select == 5) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,170,"Modifier options ->");
			else oslPrintf_xy(menu_position,170,"Modifier options  <-");
			if (menu_select == 6) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			oslPrintf_xy(menu_position,190,"Quitter l'application");

			//menus secondaires
			if (menu_position2 == menu_mangas)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),50,"Liste");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),70,"Suivant");
				if (menu_select2 == 2) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),90,"Precedent");
				if (menu_select2 == 3) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),110,"Feuilleter");
			}
			else if (menu_position2 == menu_bookmark)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),80,"Sauvegarder");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),100,"Charger");	
			}
			else if (menu_position2 == menu_profils)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),100,"Profil 1");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),120,"Profil 2");		
			}
			else if (menu_position2 == menu_mp3)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),90,"Demarrer");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),110,"Pause/Lecture");
				if (menu_select2 == 2) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),130,"Stopper");
				if (menu_select2 == 3) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),150,"Piste suivante");
				if (menu_select2 == 4) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),170,"Piste precedente");
			}
			else if (menu_position2 == menu_touches)
			{
				if (!profil)
				{
					oslPrintf_xy((menu_position+160),40,"Haut : MP3 Lecture/pause");
					oslPrintf_xy((menu_position+160),60,"Bas : MP3 Stop");
					oslPrintf_xy((menu_position+160),80,"Gauche : MP3 Piste precedente");
					oslPrintf_xy((menu_position+160),100,"Droite : MP3 Piste suivante");
					oslPrintf_xy((menu_position+160),120,"Triangle : Mode double page");
					oslPrintf_xy((menu_position+160),140,"Croix : Screenshot");
					oslPrintf_xy((menu_position+160),160,"Carre : Zoom");
					oslPrintf_xy((menu_position+160),180,"Cercle : Sauver marque-page");
					oslPrintf_xy((menu_position+160),200,"L : Page precedente");
					oslPrintf_xy((menu_position+160),220,"R : Page suivante");
				}
				else
				{
					oslPrintf_xy((menu_position+160),40,"Haut : Mode double page");
					oslPrintf_xy((menu_position+160),60,"Bas : Screenshot");
					oslPrintf_xy((menu_position+160),80,"Gauche : Zoom");
					oslPrintf_xy((menu_position+160),100,"Droite : Sauver marque-page");
					oslPrintf_xy((menu_position+160),120,"Triangle : MP3 Lecture/pause");
					oslPrintf_xy((menu_position+160),140,"Croix : MP3 Stop");
					oslPrintf_xy((menu_position+160),160,"Carre : MP3 Piste precedente");
					oslPrintf_xy((menu_position+160),180,"Cercle : MP3 Piste suivante");
					oslPrintf_xy((menu_position+160),200,"L : Page suivante");
					oslPrintf_xy((menu_position+160),220,"R : Page precedente");
				}
			}
			else if (menu_position2 == menu_options)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				if (sens == japonais) oslPrintf_xy((menu_position+160),140,"Sens lecture : Japonais");
				else oslPrintf_xy((menu_position+160),140,"Sens lecture : Occidental");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				if (langue == anglais) oslPrintf_xy((menu_position+160),160,"Langue : Anglais");
				else oslPrintf_xy((menu_position+160),160,"Langue : Francais");
				if (menu_select2 == 2) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				if (boot == menup) oslPrintf_xy((menu_position+160),180,"Demarrage : Menu");
				else oslPrintf_xy((menu_position+160),180,"Demarrage : Marque-page");
				if (menu_select2 == 3) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),201,"Vitesse CPU : %dMHz",cpu);			
			}
		}
		else //si on choisit anglais
		{
			//menu principal
			if (menu_select == 0) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,70,"Manage mangas ->");
			else oslPrintf_xy(menu_position,70,"Manage mangas  <-");
			if (menu_select == 1) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,90,"Bookmark ->");
			else oslPrintf_xy(menu_position,90,"Bookmark  <-");
			if (menu_select == 2) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,110,"Profile %d ->",(profil+1));
			else oslPrintf_xy(menu_position,110,"Profile %d  <-",(profil+1));
			if (menu_select == 3) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,130,"MP3 Player ->");
			else oslPrintf_xy(menu_position,130,"MP3 Player  <-");
			if (menu_select == 4) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,150,"Show control ->");
			else oslPrintf_xy(menu_position,150,"Show control <-");
			if (menu_select == 5) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (!menu_position2) oslPrintf_xy(menu_position,170,"Modify options ->");
			else oslPrintf_xy(menu_position,170,"Modify options  <-");
			if (menu_select == 6) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			oslPrintf_xy(menu_position,190,"Quit application");

			//menus secondaires
			if (menu_position2 == menu_mangas)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),50,"Browse");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),70,"Next");
				if (menu_select2 == 2) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),90,"Previous");
				if (menu_select2 == 3) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),110,"Leaf through");
			}
			else if (menu_position2 == menu_bookmark)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),80,"Save");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),100,"Load");	
			}
			else if (menu_position2 == menu_profils)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),100,"Profile 1");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),120,"Profile 2");		
			}
			else if (menu_position2 == menu_mp3)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),90,"Start");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),110,"Pause/Play");
				if (menu_select2 == 2) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),130,"Stop");
				if (menu_select2 == 3) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),150,"Next song");
				if (menu_select2 == 4) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),170,"Previous song");
			}
			else if (menu_position2 == menu_touches)
			{
				if (!profil)
				{
					oslPrintf_xy((menu_position+160),40,"Up : MP3 Play/pause");
					oslPrintf_xy((menu_position+160),60,"Down : MP3 Stop");
					oslPrintf_xy((menu_position+160),80,"Left : MP3 Previous song");
					oslPrintf_xy((menu_position+160),100,"Right : MP3 Next song");
					oslPrintf_xy((menu_position+160),120,"Triangle : Double page mode");
					oslPrintf_xy((menu_position+160),140,"Cross : Screenshot");
					oslPrintf_xy((menu_position+160),160,"Square : Zoom");
					oslPrintf_xy((menu_position+160),180,"Circle : Save bookmark");
					oslPrintf_xy((menu_position+160),200,"L : Previous page");
					oslPrintf_xy((menu_position+160),220,"R : Next page");
				}
				else
				{
					oslPrintf_xy((menu_position+160),40,"Up : Double page mode");
					oslPrintf_xy((menu_position+160),60,"Down : Screenshot");
					oslPrintf_xy((menu_position+160),80,"Left : Zoom");
					oslPrintf_xy((menu_position+160),100,"Right : Save bookmark");
					oslPrintf_xy((menu_position+160),120,"Triangle : MP3 Play/pause");
					oslPrintf_xy((menu_position+160),140,"Cross : MP3 Stop");
					oslPrintf_xy((menu_position+160),160,"Square : MP3 Previous song");
					oslPrintf_xy((menu_position+160),180,"Circle : MP3 Next song");
					oslPrintf_xy((menu_position+160),200,"L : Next page");
					oslPrintf_xy((menu_position+160),220,"R : Previous page");
				}
			}
			else if (menu_position2 == menu_options)
			{
				if (menu_select2 == 0) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				if (sens == japonais) oslPrintf_xy((menu_position+160),140,"Read order : Japanese");
				else oslPrintf_xy((menu_position+160),140,"Read order : Westerner");
				if (menu_select2 == 1) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				if (langue == anglais) oslPrintf_xy((menu_position+160),160,"Language : English");
				else oslPrintf_xy((menu_position+160),160,"Language : French");
				if (menu_select2 == 2) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				if (boot == menup) oslPrintf_xy((menu_position+160),180,"Startup : Menu");
				else oslPrintf_xy((menu_position+160),180,"Startup : Bookmark");
				if (menu_select2 == 3) oslSetTextColor(RGB(100,250,150));
				else oslSetTextColor(RGB(255,255,255));
				oslPrintf_xy((menu_position+160),201,"CPU Speed : %dMHz",cpu);			
			}
		}			
	}
	else if (liste_mangas && !feuilleter) //affichage de la liste des mangas
	{
		Browser();
		if (osl_keys->pressed.circle && !sousdoss) //retour au menu
		{
			liste_mangas = 0; 
			menu_position = 110; 
			menu_position2 = menu_mangas; 
			menu_select2 = 0;
		}
	}
	else if (!liste_mangas && feuilleter) //affichage du mode feuilleter
	{
		Feuilleter();
		if (osl_keys->pressed.circle) //retour au menu
		{
			feuilleter = 0; 
			menu_position = 110; 
			menu_position2 = menu_mangas; 
			menu_select2 = 3; 			
		}
	}
	else
	{
		if (langue == francais) oslPrintf_xy(205,135,"Chargement...");
		else oslPrintf_xy(216,135,"Loading...");
	}	

	if (wft2) wft2++;
	if (wft2 > 3)
	{
		if (nextmanga) MangaSuivant();
		else if (prevmanga) MangaPrecedent();
		else if (loadbookmark) {RestaureConfig(); recacheMangaDir(); ChargerMarquePage();} //charger le marque page
	}

	oslSetTextColor(RGB(200,200,200));
	oslPrintf_xy(10,255,"Manga2PSP v1.0 by Yodajr");
	oslSetTextColor(RGB(5,5,255));
	oslPrintf_xy(350,255,"×");
	oslSetTextColor(RGB(255,255,255));
	oslPrintf_xy(365,255,": Ok");
	oslSetTextColor(RGB(255,5,5));
	oslPrintf_xy(407,255,"Ö");
	oslSetTextColor(RGB(255,255,255));
	if (langue == francais) oslPrintf_xy(420,255,": Annuler");
	else oslPrintf_xy(420,255,": Cancel");

	if (osl_keys->pressed.cross)
	{
		if (menu_select == 6) oslQuit();
	}	
}

void Feuilleter()
{
	oslSetTextColor(RGB(192,192,192));
	oslPrintf_xy(130,25,"^");
	oslPrintf_xy(130,239,"_");
	oslSetTextColor(RGB(255,255,255));
	oslPrintf_xy(10,5,"%s", mangaEntry[numanga+2].name);
	oslPrintf_xy(10,25,"Page : %d/%d",(select+1), nbrpages);
	
	//oslPrintf_xy(10,55,"temp_prev:%d",temp_prev);
	
	//affichage du contenu du manga (zip)
	if (!wft)
	{
		for (i=0; i<nbrpages; i++)
		{
			if (i == select) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (((i*16 + sel_f) > 25) && ((i*16 + sel_f) < 239)) oslPrintf_xy(110,(i*16 + sel_f),pageEntry[i].name);
		}
	}
	else
	{
		if (langue == francais) oslPrintf_xy(145,135,"Chargement...");
		else oslPrintf_xy(156,135,"Loading...");
	}

	//touches
	if (osl_keys->pressed.up || osl_keys->pressed.left || osl_keys->analogY < -70 || osl_keys->analogX < -70) {temp_prev = 1; select--; sel_f += 16;}
	if (osl_keys->pressed.down || osl_keys->pressed.right || osl_keys->analogY > 70 || osl_keys->analogX > 70) {temp_prev = 1; select++; sel_f -= 16;}
	if (osl_keys->analogY < -70 || osl_keys->analogY > 70 || osl_keys->analogX < -70 || osl_keys->analogX > 70)
	{
		if (select < 0) {select = 0; sel_f = 132;}
		if (select > nbrpages - 1) {select = nbrpages - 1; sel_f = -((nbrpages-9)*16) + 4;} 
	}
	else
	{
		if (select < 0) {select = nbrpages - 1; sel_f = -((nbrpages-9)*16) + 4;}
		if (select > nbrpages - 1) {select = 0; sel_f = 132;}
	}

	//le wft c'est du bricolage pour wait for text, pour qu'on ai le temps de lire le "loading"
	if (wft) wft++;
	if (osl_pad.pressed.cross && prev1ld) wft = 1;	
	if (wft >= 3 && temp_prev >= 30)
	{
		if (select > 0) //on charge une image que si on ne selectionne pas la toute 1ere
		{
			//chargement de l'image precedente
			ExtraitDuZip(mangactuel, (select - 1 + saut));		
			if (imp) oslDeleteImage(imp);
			imp = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM, OSL_PF_5551);
			oslUnswizzleImage(imp2);
			oslUncacheImage(imp);
			for (dy=0; dy<(imp->sizeY-512);dy++) for (dx=0;dx<512;dx++) oslSetImagePixel(imp2,dx,dy,oslGetImagePixel(imp,dx,(512+dy)));	
			oslUncacheImage(imp2);			
		}
		else //sinon on crée une image noire
		{
			if (imp) oslDeleteImage(imp);
			imp = oslCreateImage(512, 700, OSL_IN_RAM, OSL_PF_5551);
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
			oslClearImage(imp,RGB(0,0,0));
			oslClearImage(imp2,RGB(0,0,0));			
		}		
		oslSwizzleImage(imp);
		oslSwizzleImage(imp2);

		if (image1) oslDeleteImage(image1);
		image1 = oslCreateSwizzledImage(prev1, OSL_IN_RAM);
		
		if (imc) oslDeleteImage(imc);
		imc = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'image actuelle
		oslImageIsSwizzledSet(imc, oslImageIsSwizzled(image1));
		
		if (ims) oslDeleteImage(ims);
		NouvellePage();
		doublep = 1;
		DoublePage();
		cache = 0;
		retour = 0; //variable debug car j'ai la flemme de trop reflechir au code
		preload = 0; //prépare à lancer le préchargement de l'image suivante	
		menu = 0;
		page = select + saut;
		wft = 0;
	}

	if (temp_prev) temp_prev++;
	if (temp_prev == 30)
	{
		//chargement de l'image de prev
		ExtraitDuZip(mangactuel, (select + saut));		
		if (prev1) oslDeleteImage(prev1);
		prev1 = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM, OSL_PF_5551);
		if (!prev1)
		{
			prev1ld = 0;
		}
		else
		{
			prev1ld = 1;
			oslClearImage(prev2,RGBA(0,0,0,0));
			oslUncacheImage(prev1);
			for (dy=0; dy<(prev1->sizeY-512);dy++) for (dx=0;dx<512;dx++) oslSetImagePixel(prev2,dx,dy,oslGetImagePixel(prev1,dx,(512+dy)));	
			oslUncacheImage(prev2);
			//affichage de l'image de prev
			prev1->stretchY = 220;
			prev1->stretchX = (prev1->stretchY * prev1->sizeX)/prev1->sizeY;
			prev1->x = 460 - prev1->stretchX;
			prev1->y = 20;
			prev2->stretchX = prev1->stretchX;
			prev2->stretchY = (prev1->stretchY * 512)/prev1->sizeY;
			prev2->x = prev1->x;
			prev2->y = 20 + round((float)(prev1->stretchY * 512)/prev1->sizeY);
		}
	}
	if (temp_prev > 30)
	{
		temp_prev = 31;	
		if (prev1ld)
		{
			oslDrawImage(prev1);
			oslDrawImage(prev2);
		}
	}
	else
	{		
		oslSetTextColor(RGB(128,128,128));
		if (langue == francais) oslPrintf_xy(355,115,"Chargement...");
		else oslPrintf_xy(366,115,"Loading...");
	}
}


void Browser()
{
	oslSetTextColor(RGB(192,192,192));
	oslPrintf_xy(120,25,"^");
	oslPrintf_xy(120,239,"_");
	oslSetTextColor(RGB(255,255,255));
	oslPrintf_xy(10,5,"%s",mangapath);

	//affichage de la liste
	if (!wft)
	{
		for (i=2; i<mangaEntriesTotal; i++)
		{
			if (i == select) oslSetTextColor(RGB(100,250,150));
			else oslSetTextColor(RGB(255,255,255));
			if (((i*16 + sel_f) > 25) && ((i*16 + sel_f) < 239)) oslPrintf_xy(mangaEntry[i].decalage,(i*16 + sel_f),mangaEntry[i].name); 		
		}	
	}
	else
	{
		if (langue == francais) oslPrintf_xy(145,135,"Chargement...");
		else oslPrintf_xy(156,135,"Loading...");
	}

	//touches
	if (osl_keys->pressed.up || osl_keys->pressed.left || osl_keys->analogY < -70 || osl_keys->analogX < -70) {temp_prev = 1; select--; sel_f += 16;}
	if (osl_keys->pressed.down || osl_keys->pressed.right || osl_keys->analogY > 70 || osl_keys->analogX > 70) {temp_prev = 1; select++; sel_f -= 16;}
	if (osl_keys->analogY < -70 || osl_keys->analogY > 70 || osl_keys->analogX < -70 || osl_keys->analogX > 70)
	{
		if (select < 2) {select = 2; sel_f = 100;}
		if (select > mangaEntriesTotal-1) {select = mangaEntriesTotal-1; sel_f = -((mangaEntriesTotal-9)*16) + 4;} 
	}
	else
	{
		if (select < 2) {select = mangaEntriesTotal-1; sel_f = -((mangaEntriesTotal-9)*16) + 4;}
		if (select > mangaEntriesTotal-1) {select = 2; sel_f = 100;}
	}

	//appui croix : si c'est un dossier, on entre dans le répertoire
	if (osl_pad.pressed.cross && mangaEntry[select].decalage != 90)
	{
		safe_strcat(mangapath, mangaEntry[select].name, sizeof(mangapath));
		safe_strcat(mangapath, "/", sizeof(mangapath));
		recacheMangaDir();
		sousdoss++;
		select = 2;
		sel_f = 6*16+4;
	}
	else if (osl_pad.pressed.cross && mangaEntry[select].decalage == 90 && prev1ld) wft = 1;

	//on quitte le mode browser ou on reviens à la racine du dossier manga 
	if (osl_pad.released.circle && sousdoss)
	{
		sousdoss = 0;

		save = VirtualFileOpen("Manga2PSP.ini", 0, VF_AUTO, VF_O_READWRITE);
		if (save)
		{
			texte = oslReadEntireFileToMemory(save, &taille);
			for (i = 0; i < taille; i++)
			{
				if (texte[i] == 'M' && texte[i+14] == '=') //repertoire mangas
				{
					safe_strcpy(mangapath, "", 1);
					j = i + 16;
					while (1) 
					{					
						sprintf(textemp2, "%c", texte[j]);
						safe_strcat(mangapath, textemp2, 512);
						j++;
						if (texte[j] == '\r') break;
					}
				}				
			}
			VirtualFileClose(save);
		}
		else safe_strcpy(mangapath, "ms0:/MANGA/", 13);
		recacheMangaDir();
		select = 2;
		sel_f = 6*16+4;
	}

	//le wft c'est du bricolage pour wait for text, pour qu'on ai le temps de lire le "loading"
	if (wft) wft++;		
	if (wft >= 3 && temp_prev >= 30)
	{			
		numanga = select - 2;
		ChargementManga(numanga);
		page = saut;
		if (imp) oslDeleteImage(imp);
		imp = oslCreateImage(512, 700, OSL_IN_RAM, OSL_PF_5551);
		if (imp2) oslDeleteImage(imp2);
		imp2 = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
		oslClearImage(imp,RGB(0,0,0));
		oslClearImage(imp2,RGB(0,0,0));
		oslSwizzleImage(imp);
		oslSwizzleImage(imp2);
				
		if (image1) oslDeleteImage(image1);
		image1 = oslCreateSwizzledImage(prev1, OSL_IN_RAM);
		
		if (imc) oslDeleteImage(imc);
		imc = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'image actuelle
		oslImageIsSwizzledSet(imc, oslImageIsSwizzled(image1));
		
		if (ims) oslDeleteImage(ims);
		NouvellePage();
		doublep = 1;
		DoublePage();
		cache = 0;
		retour = 0; //variable debug car j'ai la flemme de trop reflechir au code
		preload = 0; //prépare à lancer le préchargement de l'image suivante	
		menu = 0;
		wft = 0;		
	}

	/*oslPrintf_xy(10,50,"mangactuel:%s",mangactuel);
	oslPrintf_xy(10,70,"mangapath:%s",mangapath);
	oslPrintf_xy(10,90,"select:%d",select);
	oslPrintf_xy(10,110,"saut:%d",saut);*/
	
	if (temp_prev)
	{
		if (mangaEntry[select].decalage == 90) temp_prev++;
		else temp_prev = 1;
	}
	if (temp_prev == 30)
	{
		//chargement de l'image de prev
		safe_strcpy(tempath,mangapath,strlen(tempath));
		safe_strcat(tempath,mangaEntry[select].name,512);

		//check pour etre sur de charger un png, sinon on charge rien
		sizechar = strrchr (tempath, '.');
		extension = (char *)malloc((5)*sizeof(char)+1);
		strncpy(extension,sizechar + 1,5);
		if(strcmp(extension,"zip") && strcmp(extension,"ZIP") && strcmp(extension,"cbz") && strcmp(extension,"CBZ")) //si c'est pas un zip
		{
			if (prev1ld) {oslDeleteImage(prev1); prev1ld = 0;}			
		} 
		else 
		{
			saut = 0;
			ExtraitDuZip(tempath, 0); //on extrait juste la 1ere page
			if (prev1ld) {oslDeleteImage(prev1); prev1ld = 0;}
			prev1 = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM, OSL_PF_5551);
			prev1ld = 1;
			oslClearImage(prev2,RGBA(0,0,0,0));
			oslUncacheImage(prev1);
			for (dy=0; dy<(prev1->sizeY-512);dy++) for (dx=0;dx<512;dx++) oslSetImagePixel(prev2,dx,dy,oslGetImagePixel(prev1,dx,(512+dy)));	
			oslUncacheImage(prev2);
			//affichage de l'image de prev
			prev1->stretchY = 220;
			prev1->stretchX = (prev1->stretchY * prev1->sizeX)/prev1->sizeY;
			prev1->x = 460 - prev1->stretchX;
			prev1->y = 20;
			prev2->stretchX = prev1->stretchX;
			prev2->stretchY = (prev1->stretchY * 512)/prev1->sizeY;
			prev2->x = prev1->x;
			prev2->y = 20 + round((float)(prev1->stretchY * 512)/prev1->sizeY);		
		}
		free(extension);
		
	}
	if (temp_prev > 30)
	{
		temp_prev = 31;			
		if (prev1ld) 
		{
			oslDrawImage(prev1);
			oslDrawImage(prev2);
		}
	}
	else
	{		
		oslSetTextColor(RGB(128,128,128));
		if (langue == francais) 
		{
			if (mangaEntry[select].decalage == 90) oslPrintf_xy(355,115,"Chargement...");
			else oslPrintf_xy(375,115,"Dossier");
		}
		else 
		{
			if (mangaEntry[select].decalage == 90) oslPrintf_xy(366,115,"Loading...");
			else oslPrintf_xy(379,116,"Folder");
		}
	}	
}

void MangaSuivant()
{		
	numanga++;
	if (numanga > mangaEntriesTotal-3) numanga = mangaEntriesTotal-3;
	else
	{
		//check pour etre sur de charger un manga, sinon on charge rien
		sizechar = strrchr (mangaEntry[numanga+2].name, '.');
		extension = (char *)malloc((5)*sizeof(char)+1);
		strncpy(extension,sizechar + 1,5);
		if(strcmp(extension,"zip") && strcmp(extension,"ZIP") && strcmp(extension,"cbz") && strcmp(extension,"CBZ")) {numanga--;} //si c'est pas un zip
		else 
		{
			saut = 0;
			ChargementManga(numanga);
			page = saut;	
			ExtraitDuZip(mangactuel, page);				
								
			if (image1) oslDeleteImage(image1);
			image1 = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM, OSL_PF_5551);
			if (imc) oslDeleteImage(imc);
			imc = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'image actuelle
			oslSwizzleImage(image1);
			oslSwizzleImage(imc);
			
			if (ims) oslDeleteImage(ims);
			NouvellePage();

			if (imp) oslDeleteImage(imp);
			imp = oslCreateImage(512, 700, OSL_IN_RAM, OSL_PF_5551);
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
			oslClearImage(imp,RGB(0,0,0));
			oslClearImage(imp2,RGB(0,0,0));	
			oslSwizzleImage(imp2);

			doublep = 1;
			DoublePage();
			cache = 0;
			retour = 0; //variable debug car j'ai la flemme de trop reflechir au code
			preload = 0; //prépare à lancer le préchargement de l'image suivante	
			menu = 0;
		}
		free(extension);
	}
	wft2 = 0;
	nextmanga = 0;
}

void MangaPrecedent()
{		
	numanga--; 
	if (numanga < 0) numanga = 0;
	else
	{
		//check pour etre sur de charger un manga, sinon on charge rien
		sizechar = strrchr (mangaEntry[numanga+2].name, '.');
		extension = (char *)malloc((5)*sizeof(char)+1);
		strncpy(extension,sizechar + 1,5);
		if(strcmp(extension,"zip") && strcmp(extension,"ZIP") && strcmp(extension,"cbz") && strcmp(extension,"CBZ")) {numanga++;} //si c'est pas un zip
		else 
		{
			saut = 0;
			ChargementManga(numanga);
			page = saut;	
			ExtraitDuZip(mangactuel, page);			
								
			if (image1) oslDeleteImage(image1);
			image1 = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM, OSL_PF_5551);
			if (imc) oslDeleteImage(imc);
			imc = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'image actuelle
			oslSwizzleImage(image1);
			oslSwizzleImage(imc);
			
			if (ims) oslDeleteImage(ims);
			NouvellePage();

			if (imp) oslDeleteImage(imp);
			imp = oslCreateImage(512, 700, OSL_IN_RAM, OSL_PF_5551);
			if (imp2) oslDeleteImage(imp2);
			imp2 = oslCreateImage(512, 512, OSL_IN_RAM, OSL_PF_5551);
			oslClearImage(imp,RGB(0,0,0));
			oslClearImage(imp2,RGB(0,0,0));	
			oslSwizzleImage(imp2);

			doublep = 1;
			DoublePage();
			cache = 0;
			retour = 0; //variable debug car j'ai la flemme de trop reflechir au code
			preload = 0; //prépare à lancer le préchargement de l'image suivante	
			menu = 0;
		}
		free(extension);
	}
	wft2 = 0;
	prevmanga = 0;
}

