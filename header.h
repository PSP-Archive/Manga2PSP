#include <oslib/oslib.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include "mp3player.h"
#include "zlib.h"
#include "unzip.h"

//definition des pointeurs
OSL_IMAGE *imc, *imp, *ims, *image1, *image2, *imp2, *prev1, *prev2;
OSL_FONT *maFonte;
FILE *file;
VIRTUAL_FILE *save;

//defines
#define MAX_ENTRIES 1000
#define menu_mangas 1
#define menu_bookmark 2
#define menu_profils 3
#define menu_mp3 4
#define menu_touches 5
#define menu_options 6
#define japonais 0
#define occidental 1
#define anglais 0
#define francais 1
#define menup 0
#define bookmark 2
#define screenshot 1
#define fintome 3


//variables
int boot;
int cache;
int cpu;
int doublep;
int dx;
int dy;
int feuilleter;
int filefound;
int imagenumero;
int langue;
int liste_mangas;
int loadbookmark;
int menu;
int menu_select;
int menu_select2;
int menu_position;
int menu_position2;
int milieu;
int modif;
int mangaloaded;
int mp3play;
int mp3present;
int nextmanga;
int numanga;
int nmp3;
int numero;
int number;
int nbrpages;
int nbrzipfiles;
int page;
int prev1ld;
int preload;
int prevmanga;
int profil;
int reso;
int retour;
int saut;
int select;
int sel_f;
int sens;
int smp3;
int sousdoss;
int taille;
int temp;
int temp_prev;
int txtalpha;
int txtinfo;
int tmpinfo;
int isSwizzled;
int i;
int j;
int k;
int wft;
int wft2;
int zoom;

char filename_inzip[256];
void* buf;
int err;
int number;

//int testsens;
//int testtemp;
//int test;
//int test2;
//int echec;

char *texte;
char *sizechar;
char *extension;
char nominzip[512];
char manganame[512];
char tempath[512];
char mangapath[512];
char mangapatho[512];
char mp3path[512];
char marquepage[512];
char textemp[512];
char textemp2[512];
char mangactuel[512];
char pagetexte[5];

int mangaEntriesTotal;
int mangaDirSelected;
int MP3EntriesTotal;
int MP3DirSelected;

#define WIDTH 480
#define HEIGHT 272
#define min(x, y) (((x)>(y))?(y):(x))
#define BUFSIZE 65536


//stuctures
struct mangaEntry
{
	char *name;
	int decalage;
};
struct mangaEntry mangaEntry[MAX_ENTRIES];

struct MP3Entry
{
	char *name;
};
struct MP3Entry MP3Entry[MAX_ENTRIES];

struct pageEntry
{
	char name[512];	
};
struct pageEntry pageEntry[MAX_ENTRIES];


//prototypes
void Affichage();
void AfficheInfos();
void Browser();
void BrowseZip(char *filename);
void ChargementImage();
void ChargementManga(int numero);
void ChargementMP3();
void ChargerMarquePage();
void CreerFichierConfig();
void DoublePage();
void ExtraitDuZip(char *filename, int numero);
void Feuilleter();
void Initialisation();
//void JustForTest();
void MangaSuivant();
void MangaPrecedent();
void Menu();
void NouvellePage();
void recacheMangaDir();
void recacheMP3Dir();
void RestaureConfig();
void safe_strcat(char *dst, char *src, u32 maxlen);
void safe_strcpy(char *dst, char *src, u32 maxlen);
void SauveConfig();
void SauverMarquePage();
void Touches();
void ZoomImage();

int strlwrmixcmp(const char *lower, const char *mix);
int logoAffiche();



