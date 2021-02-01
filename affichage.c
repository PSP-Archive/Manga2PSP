#include <oslib/oslib.h>
#include "header.h"


void Affichage()
{
	//gardes fou
	if (mangaloaded) 
	{
		if (image1->y > 0) image1->y = 0;	
		if (!sens) //sens japonais
		{
			if (retour > 0 || cache == 1) {if (image1->x > 512) image1->x = 512;}
			else {if (image1->x > 0 && image1->x != 240) image1->x = 0;}
		}
		else //ou occidental
		{
			if (retour > 0 || cache == 1) {if (image1->x > 0 && image1->x != 240) image1->x = 0;}
			else {if (image1->x > 512) image1->x = 512;}
		}
		if (imp->y > 0) imp->y = 0;
		if (imc->y > 0) imc->y = 0;
		if (doublep > 0) //mode double page
		{
			if (!zoom)
			{
				if (!sens) //sens japonais
				{
					if (retour > 0 || cache == 1) {}
					else if (image1->x != 0) image1->x = 0;
				}
				else //ou occidental
				{
					if (retour > 0 || cache == 1) {}
					else if (image1->x != 240) image1->x = 240;
				}
				if ((image1->y+((reso*240)/512)) < 273) image1->y = 273-((reso*240)/512);
				if (retour > 0 || cache == 1) {if ((imc->y+((reso*240)/512)) < 273) imc->y = 273-((reso*240)/512);}
				else {if ((imp->y+((reso*240)/512)) < 273) imp->y = 273-((reso*240)/512);}
			}
			else
			{
				if (!sens) //sens japonais
				{
					if (retour > 0 || cache == 1) {if ((image1->x+512) < 480) image1->x = -32;}
					else {if ((image1->x+1024) < 480) image1->x = -544;}
				}
				else //ou occidental
				{
					if (retour > 0 || cache == 1) {if ((image1->x+1024) < 480) image1->x = -544;}
					else {if ((image1->x+512) < 480) image1->x = -32;}
				}
				if ((image1->y+reso) < 272) image1->y = (272-reso);
				if (imp->y != image1->y) imp->y = image1->y;
				if (imc->y != image1->y) imc->y = image1->y;
			}		
		}
		else
		{
			if (zoom > 0) //zoom activé
			{
				if (image1->x > 0) image1->x = 0;
				if ((image1->x+512) < 480) image1->x = -32;				
				if ((image1->y+reso) < 272) image1->y = (272-reso);
			}
			else  //zoom desactivé
			{
				if (image1->x < 0) image1->x = 0;
				if (image1->x > 0) image1->x = 0;
				if (imc->x < 0) imc->x = 0;
				if (imc->x > 0) imc->x = 0;
				if ((image1->y+((reso*480)/512)) < 273) image1->y = 273-((reso*480)/512);
				if ((imp->y+((reso*480)/512)) < 273) imp->y = 273-((reso*480)/512);
			}
		}
	}	

	//on calle bien la 2eme image a la suite
	image2->x = image1->x;
	image2->y = image1->y + milieu;
	if (retour > 0 || cache == 1) imp2->y = imc->y + milieu;
	else imp2->y = imp->y + milieu;	
	if (doublep > 0)
	{
		if (zoom > 0)
		{
			if (!sens) //sens japonais
			{
				if (retour > 0 || cache == 1) {imc->x = image1->x - 512; imp2->x = image1->x - 512;}
				else {imp->x = image1->x + 512;	imp2->x = image1->x + 512;}
			}
			else //ou occidental
			{
				if (retour > 0 || cache == 1) {imc->x = image1->x + 512; imp2->x = image1->x + 512;}
				else {imp->x = image1->x - 512;	imp2->x = image1->x - 512;}
			}
		}
		else
		{
			if (!sens) //sens japonais
			{
				if (retour > 0 || cache == 1) {imc->x = 0; imp2->x = 0;}
				else {imp->x = 240;	imp2->x = 240;}
			}
			else //ou occidental
			{
				if (retour > 0 || cache == 1) {imc->x = 240; imp2->x = 240;}
				else {imp->x = 0;	imp2->x = 0;}
			}
		}
	}

	//dessin de la page en 2 parties
	oslDrawImage(image1);
	oslDrawImage(image2);

	//dessin de la page précédente (ou suivante) en cas de double page
	if (doublep > 0) 
	{
		if (retour > 0 || cache == 1) {oslDrawImage(imc); oslDrawImage(imp2);}
		else {oslDrawImage(imp); oslDrawImage(imp2);}		
	}	

	if (menu > 0) Menu();
	//else 
	{
		//oslSysBenchmarkDisplay(); 
		//JustForTest();
	}
	
	AfficheInfos();

	//chargement de l'image suivante		
	if (!preload)
	{
		temp++;
		if (temp > 2)
		{	
			temp = 0;
			preload = 1;
			ChargementImage();
		}
	}

	//ici je balance le mp3 suivant dès que l'actuel se finit
	if (mp3play > 0 && MP3_EndOfStream() > 0)
	{
		if (nmp3 < (MP3EntriesTotal-1)) {nmp3++; MP3_End(); MP3_Init(1); ChargementMP3(); MP3_Play();}
		else {MP3_End(); nmp3 = 2;}
	}
}

void AfficheInfos()
{
	if (tmpinfo < 290)
	{
		tmpinfo++;
		if (tmpinfo > 100) txtalpha--;
		oslSetBkColor(RGBA(0,0,0,txtalpha));
		oslSetTextColor(RGBA(255,255,255,(txtalpha+60)));
		if (txtinfo == bookmark)
		{
			if (langue == francais) oslPrintf_xy(370,257,"marque-page sauve");	
			else oslPrintf_xy(388,257," bookmark saved");
		}
		else if (txtinfo == screenshot)
		{
			if (langue == francais) oslPrintf_xy(368,257," copie d'ecran faite");	
			else oslPrintf_xy(391,257,"screenshot done");
		}
		else if (txtinfo == fintome)
		{
			if (langue == francais) oslPrintf_xy(404,257,"derniere page");	
			else oslPrintf_xy(420,257," last page");
		}
	}
}

void JustForTest()
{
	//texte de debug
	/*oslPrintf_xy(10,10,"%d",image1->sizeY);		
	oslPrintf_xy(10,20,"cache:%d",cache);		
	oslPrintf_xy(10,30,"page:%d",page);
	oslPrintf_xy(10,40,"preload:%d",preload);
	oslPrintf_xy(10,50,"retour:%d",retour);
	oslPrintf_xy(10,70,"imageEntriesTotal:%d",imageEntriesTotal);
	oslPrintf_xy(10,80,"select:%d",select);

	oslPrintf_xy(10,90,"doublep:%d",doublep);		
	oslPrintf_xy(10,100,"zoom:%d",zoom);

	
	oslPrintf_xy(10,130,"imc->x:%d",imc->x);
	oslPrintf_xy(10,140,"imp->x:%d",imp->x);
	oslPrintf_xy(10,150,"imp2->x:%d",imp2->x);

	oslPrintf_xy(10,170,"image2->y:%d",image2->y);
	oslPrintf_xy(10,180,"imp2->y:%d",imp2->y);*/

	/*oslPrintf_xy(10,180,"imageEntry[page].name:%s",imageEntry[page+2].name);
	oslPrintf_xy(10,200,"MP3_EndOfStream():%d",MP3_EndOfStream());
	if (mp3play > 0) oslPrintf_xy(10,25,"%s",MP3Entry[nmp3].name);*/

	//oslPrintf_xy(10,30,"nbrpages:%d",nbrpages);
	//oslPrintf_xy(10,50,"sens:%d",sens);
	//oslPrintf_xy(10,70,"saut:%d",saut);
	//oslPrintf_xy(10,90,"%s",oslGetTempFileName());
	
	/*oslPrintf_xy(10,10,"%s",marquepage);
	oslPrintf_xy(10,30,"%s",mangapath);
	oslPrintf_xy(10,50,"%s",manganame);
	oslPrintf_xy(10,70,"%s",pagetexte);  
	oslPrintf_xy(10,90,"%s",mangapatho);
	oslPrintf_xy(10,110,"%s",mp3path);	
	oslPrintf_xy(10,130,"%d",numanga);*/
	
	/*if (imc) oslPrintf_xy(10,30,"imc:%d",oslImageIsSwizzled(imc));
	if (imp) oslPrintf_xy(10,50,"imp:%d",oslImageIsSwizzled(imp));
	if (ims) oslPrintf_xy(10,70,"ims:%d",oslImageIsSwizzled(ims));
	if (image1) oslPrintf_xy(10,90,"image1:%d",oslImageIsSwizzled(image1));
	if (image2) oslPrintf_xy(10,110,"image2:%d",oslImageIsSwizzled(image2));
	if (imp2) oslPrintf_xy(10,130,"imp2:%d",oslImageIsSwizzled(imp2));
	if (prev1) oslPrintf_xy(10,150,"prev1:%d",oslImageIsSwizzled(prev1));
	if (prev2) oslPrintf_xy(10,170,"prev2:%d",oslImageIsSwizzled(prev2));*/

	/*testtemp++;
	if (testtemp > 30)
	{
		test = testsens;
		testtemp = 0;		
	}*/
}

