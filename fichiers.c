#include <oslib/oslib.h>
#include "header.h"


void ChargementImage()
{
	if (!preload || retour > 0) imagenumero = page;
	else imagenumero = page + 1;			

	ExtraitDuZip(mangactuel, imagenumero);

	ims = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_5551);
	if (!ims) ims = oslCreateImage(512, 700, OSL_IN_RAM, OSL_PF_5551);
}

void ChargementManga(int numero)
{
	mangaloaded = 1;
	//fabrication du chemin + nom du fichier zip à charger
	safe_strcpy(mangactuel, mangapath, strlen(tempath));
	safe_strcat(mangactuel, mangaEntry[numero+2].name, 512);
	BrowseZip(mangactuel);
}

void ChargementMP3()
{
	//check pour etre sur de charger un MP3, sinon on saute
	while (1)
	{
		sizechar = strrchr (MP3Entry[nmp3].name, '.');
		extension = (char *)malloc((5)*sizeof(char)+1);
		strncpy(extension,sizechar + 1,5);
		if(strcmp(extension,"mp3") && strcmp(extension,"MP3")) //si c'est pas un MP3
		{
			if (!smp3) //si on a appuyé droite
			{
				if (nmp3 < (MP3EntriesTotal-1)) nmp3++;
				else nmp3--;
			}
			else //si on a appuyé gauche
			{
				if (nmp3 > 2) nmp3--;
				else nmp3++;
			}
		}
		free(extension);
		sizechar = strrchr (MP3Entry[nmp3].name, '.');
		extension = (char *)malloc((5)*sizeof(char)+1);
		strncpy(extension,sizechar + 1,5);
		if(strcmp(extension,"mp3") && strcmp(extension,"MP3")) {} //si c'est toujours pas un MP3 on boucle
		else break; //sinon on se casse
		free(extension);
	}
	
	//fabrication du chemin + nom du fichier à charger
	safe_strcpy(tempath,mp3path,strlen(tempath));
	safe_strcat(tempath,MP3Entry[nmp3].name,512);
	
	//chargement du MP3
	MP3_Load(tempath);
}

void recacheMangaDir()
{
	struct SceIoDirent dir;
	memset(&dir, 0, sizeof(SceIoDirent));
	static int dfd;
	dfd = sceIoDopen(mangapath);	
	if(dfd > 0)
	{
		int f = 0;
		for(f = 0; f < MAX_ENTRIES; f++)
		{
			if(mangaEntry[f].name) free(mangaEntry[f].name);
			mangaEntry[f].name = NULL;
		}
		int count = 0;

		while(sceIoDread(dfd, &dir) > 0)
		{
			static char* name;
			name = (char*)memalign(16, 300);
			sprintf(name,"%s",dir.d_name);
			static int s = 0;
			s = strlen(name);
			mangaEntry[count].name = name;
			sizechar = strrchr (name, '.');
			if (!sizechar) mangaEntry[count].decalage = 89;
			else mangaEntry[count].decalage = 90;
			count++;
			if(count > MAX_ENTRIES - 1) count = MAX_ENTRIES - 1; 
			mangaEntriesTotal = count;
			if(mangaEntriesTotal < 0) mangaEntriesTotal = 0; 
		}	
	}
	sceIoDclose(dfd);	
} 

void recacheMP3Dir()
{
	struct SceIoDirent dir;
	memset(&dir, 0, sizeof(SceIoDirent));
	static int dfd;
	dfd = sceIoDopen(mp3path);	
	if(dfd > 0)
	{
		int f = 0;
		for(f = 0; f < MAX_ENTRIES; f++)
		{
			if(MP3Entry[f].name) free(MP3Entry[f].name);
			MP3Entry[f].name = NULL;
		}
		int count = 0;

		while(sceIoDread(dfd, &dir) > 0)
		{
			static char* name;
			name = (char*)memalign(16, 300);
			sprintf(name,"%s",dir.d_name);
			static int s = 0;
			s = strlen(name);			
			MP3Entry[count].name = name;
			count++;
			if(count > MAX_ENTRIES - 1) count = MAX_ENTRIES - 1;
			MP3EntriesTotal = count;
			if(MP3EntriesTotal < 0) MP3EntriesTotal = 0;
			else mp3present = 1;
		}	
	}
	sceIoDclose(dfd);	
} 

void RestaureConfig()
{
	save = VirtualFileOpen("Manga2PSP.ini", 0, VF_AUTO, VF_O_READWRITE);
	if (save)
	{
		//Je met le fichier de config en RAM
		texte = oslReadEntireFileToMemory(save, &taille);

		//et je le lit
		for (i = 0; i < taille; i++)
		{
			if (texte[i] == 'R' && texte[i+11] == '=') //ordre de lecture
			{
				if (texte[i+13] == 'W') sens = occidental; else sens = japonais;
			}
			if (texte[i] == 'L' && texte[i+9] == '=') //langue
			{
				if (texte[i+11] == 'F') langue = francais; else langue = anglais;
			}
			if (texte[i] == 'S' && texte[i+8] == '=') //affichage au démarrage
			{
				if (texte[i+11] == 'o') boot = bookmark;
				else boot = menup;
			}
			if (texte[i] == 'C' && texte[i+10] == '=') //vitesse CPU
			{
				if (texte[i+12] == '9') cpu = 96; else cpu = 222;
			}
			if (texte[i] == 'P' && texte[i+8] == '=') //profil
			{
				if (texte[i+10] == '2') profil = 1; else profil = 0;
			}
			if (texte[i] == 'B' && texte[i+9] == '=') //marque page
			{
				safe_strcpy(marquepage, "", 1);
				j = i + 11;
				while (1) 
				{					
					sprintf(textemp2, "%c", texte[j]);
					safe_strcat(marquepage, textemp2, 512);
					j++;
					if (texte[j] == '\r') break;
				}
				safe_strcpy(mangapath, "", 1);
				safe_strcpy(manganame, "", 1);
				safe_strcpy(pagetexte, "", 1);
				j = i + 11;
				while (1) 
				{					
					sprintf(textemp2, "%c", texte[j]);
					safe_strcat(mangapath, textemp2, 512);
					j++;
					if (texte[j] == '>') break;
				}
				j++;
				while (1) 
				{					
					sprintf(textemp2, "%c", texte[j]);
					safe_strcat(manganame, textemp2, 512);
					j++;
					if (texte[j] == '>') break;
				}
				j++;
				while (1) 
				{					
					sprintf(textemp2, "%c", texte[j]);
					safe_strcat(pagetexte, textemp2, 512);
					j++;
					if (texte[j] == '\r') break;
				}
			}
			if (texte[i] == 'M' && texte[i+14] == '=') //repertoire mangas
			{
				safe_strcpy(mangapatho, "", 1);
				j = i + 16;
				while (1) 
				{					
					sprintf(textemp2, "%c", texte[j]);
					safe_strcat(mangapatho, textemp2, 512);
					j++;
					if (texte[j] == '\r') break;
				}
			}
			if (texte[i] == 'M' && texte[i+11] == '=') //repertoire MP3
			{
				safe_strcpy(mp3path, "", 1);
				j = i + 13;
				while (texte[j] != '\r') 
				{					
					sprintf(textemp2, "%c", texte[j]);
					safe_strcat(mp3path, textemp2, 512);
					j++;
				}
			}			
		}
		VirtualFileClose(save);
	}	
	else //valeurs par defaut si fichier .ini absent
	{
		safe_strcpy(mangapatho, "ms0:/MANGA/", 13);
		safe_strcpy(mangapath, "ms0:/MANGA/", 13);
		safe_strcpy(mp3path, "ms0:/MUSIC/", 13);
		cpu = 222;
	}
}

void SauveConfig()
{
	CreerFichierConfig();
	VirtualFilePuts("# Manga2PSP Configuration file\r\n", save);
	
	//save du sens de lecture
	VirtualFilePuts("\r\n\r\nRead order = ", save);
	if (!sens) safe_strcpy(textemp, "Japanese", sizeof(textemp));
	else safe_strcpy(textemp, "Westerner", sizeof(textemp));
	VirtualFilePuts(textemp, save);
	//save du language
	VirtualFilePuts("\r\n\r\nLanguage = ", save);
	if (!langue) safe_strcpy(textemp, "English", sizeof(textemp));
	else safe_strcpy(textemp, "French", sizeof(textemp));
	VirtualFilePuts(textemp, save);	
	//save de l'affichage au démarrage
	VirtualFilePuts("\r\n\r\nStartup = ", save);
	if (!boot) safe_strcpy(textemp, "Menu", sizeof(textemp));
	else safe_strcpy(textemp, "Bookmark", sizeof(textemp));
	VirtualFilePuts(textemp, save);
	//save du CPU
	VirtualFilePuts("\r\n\r\nCPU Speed = ", save);
	if (cpu == 222) safe_strcpy(textemp, "222", sizeof(textemp));
	else safe_strcpy(textemp, "96", sizeof(textemp));
	VirtualFilePuts(textemp, save);	
	//save du profil
	VirtualFilePuts("\r\n\r\nProfile = ", save);
	sprintf(textemp2, "%c", (profil+49));
	safe_strcpy(textemp, textemp2, sizeof(textemp));
	VirtualFilePuts(textemp, save);
	//save du marque page
	VirtualFilePuts("\r\n\r\nBookmark = ", save);
	safe_strcpy(textemp, marquepage, sizeof(textemp));
	VirtualFilePuts(textemp, save);
	//save du repertoire mangas
	VirtualFilePuts("\r\n\r\nMangas folder = ", save);
	safe_strcpy(textemp, mangapatho, sizeof(textemp));
	VirtualFilePuts(textemp, save);
	//save du repertoire MP3
	VirtualFilePuts("\r\n\r\nMP3 folder = ", save);
	safe_strcpy(textemp, mp3path, sizeof(textemp));	
	safe_strcat(textemp, "\r", sizeof(textemp));
	VirtualFilePuts(textemp, save);

	VirtualFileClose(save);	
}

void CreerFichierConfig()
{
	file = fopen("Manga2PSP.ini", "w");
	//j'efface le fichier si il existe pour eviter les bugs
	if (file) {fclose(file); sceIoRemove("Manga2PSP.ini"); file = fopen("Manga2PSP.ini", "w");}
	fwrite(&boot, sizeof(boot), 1, file);
	fclose(file);
	save = VirtualFileOpen("Manga2PSP.ini", 0, VF_AUTO, VF_O_READWRITE);
}

void ExtraitDuZip(char *filename, int number)				
{
	unzFile fichier_zip;
	unz_file_info unzinfo;
	
	//ouvre le fichier zip
	fichier_zip = unzOpen(filename);

	//si le fichier zip est là
	if (fichier_zip) 
	{
		unz_global_info pglobal_info;		

		//récupère les infos du zip
		unzGetGlobalInfo(fichier_zip,&pglobal_info);
		nbrzipfiles = pglobal_info.number_entry; //bien utile comme info, le nombre de fichiers dans le zip

		//ouvre le premier fichier du zip si !number
		unzGoToFirstFile(fichier_zip);
		while (number--) unzGoToNextFile(fichier_zip);

		//recupère les infos du fichier pointé
		unzGetCurrentFileInfo(fichier_zip, &unzinfo, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

		while (!unzinfo.uncompressed_size) {unzGoToNextFile(fichier_zip); unzGetCurrentFileInfo(fichier_zip, &unzinfo, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);}
		
		//ouvre le fichier pointé dans le zip
		unzOpenCurrentFile(fichier_zip);

		//petite allocation, le unzinfo.uncompressed_size est la taille en octets du fichier pointé
		if (buf) free(buf);
		buf = (void*)malloc(unzinfo.uncompressed_size);

		//extrait le fichier pointé dans le zip
		err = unzReadCurrentFile(fichier_zip, buf, unzinfo.uncompressed_size);
		if (!err) oslQuit(); //radical, mais faudrait mettre plutot un message d'erreur
		
		//ecriture dans le fichier temporaire en ram
		oslSetTempFileData(buf, unzinfo.uncompressed_size, &VF_MEMORY);			

		//on oublie pas de fermer le fichier pointé puis le zip lui même
		unzCloseCurrentFile(fichier_zip);
		unzClose(fichier_zip);			
	}	
}

void SauverMarquePage()
{
	safe_strcpy(marquepage, mangapath, sizeof(marquepage));
	safe_strcat(marquepage, ">", sizeof(marquepage));
	safe_strcat(marquepage, mangaEntry[numanga+2].name, sizeof(marquepage));
	safe_strcat(marquepage, ">", sizeof(marquepage));
	if (((page+1-saut)/1000) > 0) {sprintf(textemp2, "%c", ((page+1-saut)/1000)+48); safe_strcat(marquepage, textemp2, 512);}
	if ((((page+1-saut)%1000)/100) > 0) {sprintf(textemp2, "%c", (((page+1-saut)%1000)/100)+48); safe_strcat(marquepage, textemp2, 512);}
	if ((((page+1-saut)%100)/10) > 0) {sprintf(textemp2, "%c", (((page+1-saut)%100)/10)+48); safe_strcat(marquepage, textemp2, 512);}
	if (((page+1-saut)%10) > 0) {sprintf(textemp2, "%c", ((page+1-saut)%10)+48); safe_strcat(marquepage, textemp2, 512);}
}

void ChargerMarquePage()
{
	filefound = 0;
	for (i=2; i<mangaEntriesTotal; i++)
	{
		if(strcmp(mangaEntry[i].name,manganame) && strlwrmixcmp(manganame,mangaEntry[i].name)) {}
		else {numanga = i - 2; i = mangaEntriesTotal; filefound = 1;}
	}

	if (filefound) //selement si on a réussi à retrouver le manga dans la liste
	{
		k = 0;
		while (pagetexte[k] > 0) k++;
		page = pagetexte[k-1] - 48;
		if (k-2 >= 0) page += 10*(pagetexte[k-2] - 48);
		if (k-3 >= 0) page += 100*(pagetexte[k-3] - 48);
		if (k-4 >= 0) page += 1000*(pagetexte[k-4] - 48);

		if (strcmp(mangapath,mangapatho)) sousdoss = 1; //indique que le dossier du marque page n'est pas celui de base

		saut = 0;
		ChargementManga(numanga);
		page += saut-1;	
		ExtraitDuZip(mangactuel, page);	
									
		if (image1) oslDeleteImage(image1);
		image1 = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_5551);
		if (imc) oslDeleteImage(imc);
		imc = oslCreateImageCopy(image1, OSL_IN_RAM); //stockage de l'image actuelle
		oslImageIsSwizzledSet(imc, oslImageIsSwizzled(image1));
		if (ims) oslDeleteImage(ims);
		
		if (page > 1) //on charge une image que si on ne selectionne pas la toute 1ere
		{
			ExtraitDuZip(mangactuel, page-1);
			if (imp) oslDeleteImage(imp);
			imp = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM, OSL_PF_5551);
			oslUncacheImage(imp);
			oslUnswizzleImage(imp2);
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

		NouvellePage();		
		
		doublep = 1;
		DoublePage();
		cache = 0;
		retour = 0; //variable debug car j'ai la flemme de trop reflechir au code
		preload = 0; //prépare à lancer le préchargement de l'image suivante	
		menu = 0;
		loadbookmark = 0;
		wft2 = 0;
	}
	else 
	{
		safe_strcpy(mangapath, mangapatho, sizeof(mangapath));
		recacheMangaDir();		
		menu = 1;	
		preload = 1;
	}
}

void BrowseZip(char *filename)
{
	unzFile fichier_zip;
	unz_file_info unzinfo;
	unz_global_info pglobal_info;
	int doss = 0;
	
	fichier_zip = unzOpen(filename);			
	unzGetGlobalInfo(fichier_zip,&pglobal_info);	
	unzGoToFirstFile(fichier_zip);
	for(i=0; i<pglobal_info.number_entry; i++)
	{
		unzGetCurrentFileInfo(fichier_zip, &unzinfo, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
		while (!unzinfo.uncompressed_size) {doss++; unzGoToNextFile(fichier_zip); unzGetCurrentFileInfo(fichier_zip, &unzinfo, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);} //ici je ne compte pas les dossiers
		
		//ici je met comme nom que le nom du fichier, sans le nom de dossier si il y a un dossier dans le zip
		sizechar = strrchr (filename_inzip, '/');
		if (sizechar)
		{
			strncpy(nominzip,sizechar + 1,512);		
			safe_strcpy(pageEntry[i].name, nominzip, sizeof(pageEntry[i].name));
		}
		else safe_strcpy(pageEntry[i].name, filename_inzip, sizeof(pageEntry[i].name));
		
		unzGoToNextFile(fichier_zip);
	}
	nbrpages = pglobal_info.number_entry - doss;
	saut = doss;
	unzClose(fichier_zip);
}







