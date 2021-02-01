#include <oslib/oslib.h>
#include "header.h"

#define min(x, y) (((x)>(y))?(y):(x))

/*
	Pour le logo!!!
	Décale la valeur du rouge vers l'alpha, pour un niveau de transparence correspondant au texte
*/
void logoDecalePixels(OSL_IMAGE *img)		{
	unsigned long *data = img->data;
	int i;
	for (i=0;i<img->totalSize>>2;i++)		{
		*data = (*data) << 24;
		data++;
	}
	oslUncacheImage(img);
}

OSL_IMAGE *logoCreeImageFond()			{
	OSL_IMAGE *fond;
	unsigned char *data;
	int i, j;
	fond = oslCreateImage(480, 272, OSL_IN_RAM, OSL_PF_8BIT);
	if (!fond)
		return 0;
	fond->palette = oslCreatePalette(256, OSL_PF_8888);
	if (!fond->palette)		{
		oslDeleteImage(fond);
		return 0;
	}
	for (j=0;j<HEIGHT;j++)			{
		data = (unsigned char*)oslGetImageLine(fond, j);
		for (i=0;i<WIDTH/2;i++)			{
			*data++ = i;
			*data++ = i;
		}
	}
	oslUncacheImage(fond);
	return fond;
}

void logoCreePaletteRotative(OSL_IMAGE *img, int angle)			{
	unsigned long *pal;
	int valeur, i;
	pal = img->palette->data;
	for (i=0;i<239;i++)			{
		angle = angle % (256 * 6);
		if (angle < 0)	angle += 256 * 6;
		valeur = angle & 255;
		if (angle < 256)					// Rouge -> jaune
			*pal++ = RGBA(255, valeur, 0, 0xff);
		else if (angle < 256 * 2)			// Jaune -> vert
			*pal++ = RGBA(255-valeur, 255, 0, 0xff);
		else if (angle < 256 * 3)			// Vert -> turquoise
			*pal++ = RGBA(0, 255, valeur, 0xff);
		else if (angle < 256 * 4)			// Turquoise -> bleu
			*pal++ = RGBA(0, 255-valeur, 255, 0xff);
		else if (angle < 256 * 5)			// Bleu -> rose
			*pal++ = RGBA(valeur, 0, 255, 0xff);
		else if (angle < 256 * 6)			// Rose -> rouge
			*pal++ = RGBA(255, 0, 255-valeur, 0xff);
		angle += 8;
	}
	oslUncachePalette(img->palette);
}

/*//Pas utilisé, servait à texturer une image d'une autre, mais maintenant on texture d'un dégradé
int logoTextureImage(OSL_IMAGE *temp)			{
	OSL_IMAGE *tex;
	unsigned long *src, *dst;
	int i;

	tex = oslLoadImageFile("fond.png", OSL_IN_RAM, OSL_PF_8888);
	if (!tex)
		return 0;
	dst = temp->data;
	src = tex->data;
	for (i=0;i<temp->totalSize>>2;i++)		{
		*dst = ((*dst) & 0xff000000) | ((*src) & 0x00ffffff);
		src++, dst++;
	}
	oslUncacheImage(temp);
	oslDeleteImage(tex);
	return 1;
}*/

/*
	Affiche le logo OSLib
	Ne cherchez pas à comprendre cette fonction si vous débutez :p
*/
int logoAffiche()			
{
	OSL_IMAGE *texte, *temp, *temp2, *etoile;
	OSL_IMAGE *fond;
	int skip=0;
	int x, y;
	int angle, nFrame, val;
	float dist;
	int pousNb;
	float pousX[100], pousY[100];
	int i, imgAngle;
	float tblVitPous[8] = {0.2, 0.3, 0.15, 0.2, 0.35, 0.2, 0.15, 0.3};
	int fade = 0;

	temp = oslLoadImageFile("ram:/texte.png", OSL_IN_VRAM, OSL_PF_8888);
	if (!temp)
		return 0;
	logoDecalePixels(temp);
	oslStartDrawing();
	oslSetDrawBuffer(temp);
	oslDrawGradientRect(0,0, WIDTH,HEIGHT, RGB(0,0,0), RGB(0,0,128), RGB(0,0,128), RGB(0,0,0));
	oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
	oslEndDrawing();
	temp2 = oslCreateImageCopy(temp, OSL_IN_RAM);
	oslDeleteImage(temp);
	texte = oslCreateSwizzledImage(temp2, OSL_IN_VRAM);
	if (!texte)		{
		return 0;
	}
	oslDeleteImage(temp2);

	fond = logoCreeImageFond();
	if (!fond)		{
		oslDeleteImage(texte);
		return 0;
	}
	
	oslSetTransparentColor(RGB(255,0,254));
	etoile = oslLoadImageFile("ram:/etoile.png", OSL_IN_RAM, OSL_PF_5551);
	if (!etoile)		{
		oslDeleteImage(texte);
		oslDeleteImage(fond);
		return 0;
	}
	oslDisableTransparentColor();

	texte->centerX = texte->sizeX / 2;
	texte->centerY = texte->sizeY / 2;

//recommence:
	angle = 270;
	dist = 160.0f;
	nFrame = 0;
	pousNb=0;
	imgAngle = 78;

	while (!osl_quit)
	{
		if (!skip)			
		{
			oslStartDrawing();
			logoCreePaletteRotative(fond, 2000-nFrame*8);
			oslDrawImage(fond);
			x = WIDTH/2;
			y = HEIGHT/2;
			if (dist > 0)			{
				x += oslCos(angle, dist);
				y -= oslSin(angle, dist);
				texte->stretchX = texte->sizeX * (1+dist*0.03f);
				texte->stretchY = texte->sizeY * (1+dist*0.03f);
				texte->angle = imgAngle;
			}
			else		{
				texte->stretchX = texte->sizeX;
				texte->stretchY = texte->sizeY;
				texte->angle = 0;
			}
			//oslSetBilinearFilter(1);
			//Effet spécial pour le texturage des nuages
			if (nFrame >= 50)		{
				val = (nFrame - 50) * 6;
				if (val > 255)
					val = 255;
				oslSetAlpha(OSL_FX_ALPHA|OSL_FX_COLOR, RGBA(val,val,val,0xff));
			}
			else
				oslSetAlpha(OSL_FX_ALPHA|OSL_FX_COLOR, RGBA(0,0,0,0xff));
			oslDrawImageXY(texte, x, y);

			oslSetImageTileSize(etoile, 0,0, 16,16);
			oslCorrectImageHalfBorder(etoile);
			oslImageSetRotCenter(etoile);
			etoile->angle = 360 - ((nFrame * 3) % 360);
			etoile->stretchX = 16;
			etoile->stretchY = 16;
			oslSetAlpha(OSL_FX_ALPHA|OSL_FX_COLOR, RGBA(0xff,0,0,0xff));
			oslDrawImageXY(etoile, 480-9, 272-9);
			
			//oslPrintf_xy(10,130,"%d",nFrame);
			
			//Dessine l'étoile
			if (nFrame >= 140)// && nFrame < 240)	
			if (nFrame < 240)
			{
				val = nFrame - 140;
				angle = (val * 5) % 360;
				i = nFrame - (240 - 256/12);
				if (val*16 < 255)
					oslSetAlpha(OSL_FX_ALPHA, val*16);
				else if (i > 0)
					oslSetAlpha(OSL_FX_ALPHA, 255-i*12);
				else
					oslSetAlpha(OSL_FX_RGBA, 0);
				etoile->angle = (val * 8) % 360;
				etoile->x = WIDTH/2 + oslCos(angle, 120);
				etoile->y = HEIGHT/2 - oslSin(angle, 50);
				etoile->stretchX = 16 * 2;
				etoile->stretchY = 16 * 2;
				oslDrawImage(etoile);
				oslResetImageHalfBorder(etoile);
			}
			
			//Restaure l'environnement
			//oslSetBilinearFilter(0);
			//Dessine les poussières
			oslSetAlpha(OSL_FX_ADD, 0xff);
			oslSetImageTileSize(etoile, 0,16, 8,8);
			oslImageSetRotCenter(etoile);
			for (i=0;i<pousNb;i++)
				oslDrawImageXY(etoile, pousX[i], pousY[i]);
			

			oslSetAlpha(OSL_FX_RGBA, 0);
//			oslSysBenchmarkDisplay();
			//Fade
			if (nFrame < 32)
				oslDrawFillRect(0,0, WIDTH,HEIGHT, RGBA(0,0,0,255-(nFrame<<3)));
			if (fade > 0)		{
				if (fade >= 31)
					fade = 31;
				oslDrawFillRect(0,0, WIDTH,HEIGHT, RGBA(0,0,0,(fade<<3)));
			}
			oslEndDrawing();
		}
		nFrame++;
		//L'étoile tourne sur le logo
		if (nFrame >= 140)// && nFrame < 240)	
		if (nFrame < 240)
		{
			val = nFrame - 140;
			angle = (val * 5) % 360;
			if (angle >= 200 && angle <= 340)		
			{
				pousX[pousNb] = WIDTH/2 + oslCos(angle, 120);
				pousY[pousNb] = HEIGHT/2 - oslSin(angle, 50);
				pousNb++;
			}
		}
		if (fade > 0)
			fade++;
		if (nFrame > 290 && fade == 0)
			fade = 1;
		for (i=0;i<pousNb;i++)		{
			pousY[i] += tblVitPous[i%8];
		}

		dist -= 2.0f;
		angle -= 4;
		imgAngle -= 1;
		if (angle < 0)
			angle += 360;
		skip = oslSyncFrameEx(1,4,0);
		//Lecture des touches
		oslReadKeys();
		if ((osl_keys->pressed.start || osl_keys->pressed.cross || osl_keys->pressed.circle || osl_keys->pressed.square) && fade == 0)
			fade = 1;
/*		if (osl_keys->pressed.L)
			oslWriteImageFile(OSL_SECONDARY_BUFFER, "screen.png", 0);
		if (osl_keys->pressed.start || osl_keys->held.R)			{
			if (osl_keys->held.L)
				oslWriteImageFile(OSL_SECONDARY_BUFFER, "screen.png", 0);
			do	{
				oslReadKeys();
			} while (!osl_keys->pressed.start && !osl_keys->pressed.select && !osl_keys->pressed.R && !osl_quit);
			oslSwapBuffers();
			oslSyncFrameEx(0,0,0);
			skip = 0;
		}
		if (osl_keys->pressed.select)
			goto recommence;*/
		if (/*nFrame >= 290+32 ||*/ fade >= 32)
			break;
	}
	oslSetAlpha(OSL_FX_RGBA, 0);
	oslDeleteImage(texte);
	oslDeleteImage(fond);
	return 1;
}
