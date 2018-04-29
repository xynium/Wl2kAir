//__/\\\_______/\\\__/\\\________/\\\__/\\\\\_____/\\\__/\\\\\\\\\\\__/\\\________/\\\__/\\\\____________/\\\\_
// _\///\\\___/\\\/__\///\\\____/\\\/__\/\\\\\\___\/\\\_\/////\\\///__\/\\\_______\/\\\_\/\\\\\\________/\\\\\\_
//  ___\///\\\\\\/______\///\\\/\\\/____\/\\\/\\\__\/\\\_____\/\\\_____\/\\\_______\/\\\_\/\\\//\\\____/\\\//\\\_
//   _____\//\\\\__________\///\\\/______\/\\\//\\\_\/\\\_____\/\\\_____\/\\\_______\/\\\_\/\\\\///\\\/\\\/_\/\\\_
//    ______\/\\\\____________\/\\\_______\/\\\\//\\\\/\\\_____\/\\\_____\/\\\_______\/\\\_\/\\\__\///\\\/___\/\\\_
//     ______/\\\\\\___________\/\\\_______\/\\\_\//\\\/\\\_____\/\\\_____\/\\\_______\/\\\_\/\\\____\///_____\/\\\_
//      ____/\\\////\\\_________\/\\\_______\/\\\__\//\\\\\\_____\/\\\_____\//\\\______/\\\__\/\\\_____________\/\\\_
//       __/\\\/___\///\\\_______\/\\\_______\/\\\___\//\\\\\__/\\\\\\\\\\\__\///\\\\\\\\\/___\/\\\_____________\/\\\_
//        _\///_______\///________\///________\///_____\/////__\///////////_____\/////////_____\///______________\///__
/*
 ============================================================================
 Name        : Autre.h
 Author      : Xynium  
 Version     :
 Copyright   : FM4PN JPLathuile
 Description : Annexe po wl2k
 ============================================================================
 */

#ifndef Autre
#define Autre

//Preferences
#define sPath  "/home/aura/eclipse-workspace/PNWl2kAir/src/"
#define  sSPort  "/dev/rfcomm2"
// mail ~/PNMail/OutBox/   ~/PNMail/InBox/
// config ~/PNMail/PNMail.conf

#define MAXBUFFLON 1200   // le PTC envoie maxi 1024 char compter les stuff
#define MAXRECPROP 10   // dim tableau de message de reception et emission

// Fin Pref

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <termio.h>
#include <locale.h>
#include <sys/time.h>
#include "md5.h"
#include "lzhuf_1.h"
#include <gtk/gtk.h>

typedef enum {
    false, true
} bool;

#define CHRNUL 0
#define CHRSOH 1
#define CHRSTX 2
#define CHREOT 4

struct qzbuffer {
    unsigned char *data;
    unsigned long alen;
    unsigned long dlen;
    unsigned int i;
};

struct message {
   struct qzbuffer *cbuf;
   unsigned char title[181];  //04/2018 was 81 but some title very long
   unsigned char offset[7]; // inutilisé
int ietat;    // 0 rien 1 chargé 2 autorisé envoi 3 envoye
};

/*Winlink  login */
static const unsigned char sl_salt[] = { 77, 197, 101, 206, 190, 249, 93, 200, 51, 243, 93, 237, 71, 94, 239, 138, 68, 108, 70, 185, 225, 137, 217, 16, 51, 122, 193, 48, 194, 195, 198, 175, 172, 169, 70, 84, 61, 62, 104, 186, 114, 52, 61, 168, 66, 129, 192, 208, 187, 249, 232,
            193, 41, 113, 41, 45, 240, 16, 29, 228, 208, 228, 61, 20 };

char sMycall[10];
char password[10];
char locator[10];
char saPropName[MAXRECPROP][20];
const gchar *sStatcall;

char slgnbuffer[270];
struct qzbuffer *cbuf;
unsigned char cRep[MAXBUFFLON];
char buffer[MAXBUFFLON];
GtkTextBuffer *tvBuffer;
GtkTextTag *tagG, *tagR, *tagB, *tagJ;
int tty;   // port serie
long int iFreq;  // Freq a comoposer
bool bR;     // bR est l'ack
int iTranState;   // var etat
int iposr;
int iFlag;   // var etat
bool bNoAff;     // TODO en argument de EnvData
bool bDispCpt;
int iNToSend;


// dans autre
void calchalrep(char *challenge, char *response, char *password);
char *strupper(char *s);
struct qzbuffer * buffer_readfile(const char *path);
struct qzbuffer * buffer_new(void);
int buffer_addchar(struct qzbuffer *b, int c);
void buffer_free(struct qzbuffer *b);
void buffer_rewind(struct qzbuffer *b);
int buffer_iterchar(struct qzbuffer *b);
int putcompressed(char *titre, long int offst, struct qzbuffer *buf);
int buffer_writefile(const char *path, struct qzbuffer *buf);
int FCLongParse(char *s);
int LitConfig(void);
void InitPTC(void);
void EnvCommand(unsigned char cCh, unsigned char cCmd, char * sCmd);
int EnvTram(unsigned char *cTrame, int its);
void PtcRest();
bool PollStatus(char *sPStat);
void EnvData(unsigned char cCh, unsigned int iLo, char * sCmd);
char * FindSujet(const char *FName);

//dans protocool
void  DEDExtraitD(GtkWidget *pBtn);
void  DEDExtraitC(void);
void MesProposals(void);
void TermineAvecErreur(int ierr, GtkWidget *pBtn);
void EnvoieFQ(GtkWidget *pBtn);


#endif
