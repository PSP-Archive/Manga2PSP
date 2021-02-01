#include <oslib/oslib.h>
#include "header.h"


void NouvellePage()
{
	zoom = 0;

	//la résolution de l'image
	reso = image1->sizeY;
	
	oslClearImage(image2,RGB(0,0,0));
	
	//remplissage du bas de l'image (image2)
	oslUnswizzleImage(image1);
	oslUnswizzleImage(image2);
	oslUncacheImage(image1);
	for (dy=0; dy<(reso-512);dy++) for (dx=0;dx<512;dx++) oslSetImagePixel(image2,dx,dy,oslGetImagePixel(image1,dx,(512+dy)));	
	oslUncacheImage(image2);
	oslSwizzleImage(image1);
	oslSwizzleImage(image2);
	
	if (!doublep)
	{
		milieu = 479;
		image1->stretchX = 480;
		image1->stretchY = (reso*480)/512;
		image2->stretchX = 480;
		image2->stretchY = 480;
	}
	else
	{
		milieu = 239;
		image1->stretchX = 240;
		image1->stretchY = (image1->sizeY*240)/512;
		image2->stretchX = 240;
		image2->stretchY = 240;
		imp2->stretchX = 240;
		imp2->stretchY = 240;	
		image1->y = 0;
		imp->y = 0;
		imc->y = 0;
		if (retour > 0 || cache == 1)
		{
			imc->stretchX = 240;
			imc->stretchY = (imc->sizeY*240)/512;			
			if (!sens) //sens japonais
			{
				imc->x = 0;	
				imp2->x = 0;
				image1->x = 240;
			}
			else //ou occidental
			{
				imc->x = 240;	
				imp2->x = 240;
				image1->x = 0;
			}
		}
		else
		{			
			imp->stretchX = 240;
			imp->stretchY = (imp->sizeY*240)/512;			
			if (!sens) //sens japonais
			{
				imp->x = 240;				
				imp2->x = 240;
				image1->x = 0;
			}
			else //ou occidental
			{
				imp->x = 0;				
				imp2->x = 0;
				image1->x = 240;
			}
		}
	}
}


void DoublePage()
{
	if (!doublep)
	{
		zoom = 0;
		doublep = 1;
		milieu = 239;
		image1->stretchX = 240;
		image1->stretchY = (image1->sizeY*240)/512;
		image2->stretchX = 240;
		image2->stretchY = 240;
		imp2->stretchX = 240;
		imp2->stretchY = 240;	
		image1->y = 0;
		imp->y = 0;
		imc->y = 0;
		if (retour > 0 || cache == 1)
		{
			imc->stretchX = 240;
			imc->stretchY = (imc->sizeY*240)/512;			
			if (!sens) //sens japonais
			{
				imc->x = 0;	
				imp2->x = 0;
				image1->x = 240;
			}
			else //ou occidental
			{
				imc->x = 240;	
				imp2->x = 240;
				image1->x = 0;
			}
		}
		else
		{			
			imp->stretchX = 240;
			imp->stretchY = (imp->sizeY*240)/512;			
			if (!sens) //sens japonais
			{
				imp->x = 240;				
				imp2->x = 240;
				image1->x = 0;
			}
			else //ou occidental
			{
				imp->x = 0;				
				imp2->x = 0;
				image1->x = 240;
			}
		}
	}
	else
	{
		zoom = 0;
		doublep = 0;
		milieu = 479;
		image1->stretchX = 480;
		image1->stretchY = (image1->sizeY*480)/512;
		image2->stretchX = 480;
		image2->stretchY = 480;
		imp2->stretchX = 480;
		imp2->stretchY = 480;
		image1->y = 0;
		imp->y = 0;
		imc->y = 0;
		image1->x = 0;
		imc->stretchX = 480;
		imc->stretchY = (imc->sizeY*480)/512;			
		imc->x = 0;	
		imp2->x = 0;
		imp->stretchX = 480;
		imp->stretchY = (imp->sizeY*480)/512;			
		imp->x = 0;				
		imp2->x = 0;		
	}
}

void ZoomImage()
{
	if (!zoom)
	{
		zoom = 1;
		milieu = 512;
		image1->stretchX = 512;
		image1->stretchY = reso;
		image2->stretchX = 512;
		image2->stretchY = 512;
		if (doublep > 0)
		{			
			imp2->stretchX = 512;			
			imp2->stretchY = 512;
			if (retour > 0 || cache == 1)
			{
				imc->stretchX = 512;
				imc->stretchY = imc->sizeY;			
				if (!sens) //sens japonais
				{
					imc->x = -272;	
					imp2->x = -272;
					image1->x = 240;
				}
				else //ou occidental
				{
					imc->x = 240;	
					imp2->x = 240;
					image1->x = -272;
				}
			}
			else
			{			
				imp->stretchX = 512;
				imp->stretchY = imp->sizeY;			
				if (!sens) //sens japonais
				{
					imp->x = 240;				
					imp2->x = 240;
					image1->x = -272;
				}
				else //ou occidental
				{
					imp->x = -272;				
					imp2->x = -272;
					image1->x = 240;
				}
			}
		}
	}
	else
	{
		zoom = 0;			
		if (doublep > 0)
		{
			milieu = 239;
			image1->stretchX = 240;
			image1->stretchY = (image1->sizeY*240)/512;
			image2->stretchX = 240;
			image2->stretchY = 240;
			imp2->stretchX = 240;
			imp2->stretchY = 240;
			image1->y = 0;
			imp->y = 0;
			imc->y = 0;
			if (retour > 0 || cache == 1)
			{
				imc->stretchX = 240;
				imc->stretchY = (imc->sizeY*240)/512;			
				if (!sens) //sens japonais
				{
					imc->x = 0;	
					imp2->x = 0;
					image1->x = 240;
				}
				else //ou occidental
				{
					imc->x = 240;	
					imp2->x = 240;
					image1->x = 0;
				}
			}
			else
			{			
				imp->stretchX = 240;
				imp->stretchY = (imp->sizeY*240)/512;			
				if (!sens) //sens japonais
				{
					imp->x = 240;				
					imp2->x = 240;
					image1->x = 0;
				}
				else //ou occidental
				{
					imp->x = 0;				
					imp2->x = 0;
					image1->x = 240;
				}
			}
		}
		else
		{
			milieu = 479;
			image1->stretchX = 480;
			image1->stretchY = (reso*480)/512;
			image2->stretchX = 480;
			image2->stretchY = 480;
			image1->x = 0;			
		}
	}
}

//Comparaison entre une chaîne à casse basse et mixte
int strlwrmixcmp(const char *lower, const char *mix)			
{
	while(*lower || *mix)		{
		char c = *mix;
		if (c >= 'A' && c <= 'Z')
			c += 32;
		if (c != *lower)
			return -1;
		lower++, mix++;
	}
	return 0;
}

//Strcpy mais avec une taille max ^^
void safe_strcpy(char *dst, char *src, u32 maxlen)		
{
	while (--maxlen && *src)
		*dst++ = *src++;
	*dst++ = 0;
}

//Strcat mais avec une taille max
void safe_strcat(char *dst, char *src, u32 maxlen)		
{
	while (maxlen && *dst)
		dst++, maxlen--;
	while (--maxlen && *src)
		*dst++ = *src++;
	*dst++ = 0;
}




