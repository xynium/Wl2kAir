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
Name        : Autre.c
 Author      : Xynium  
 Version     :
 Copyright   : FM4PN JPLathuile
 Description : Annexe po wl2k version air
 *                    Les fonction de buffer proviennent de Paclink unix
 ============================================================================
 */
#include "Autre.h"

unsigned char sBuff[276];

//-----------------------------------------------------------------------------------------------------------------------------------------
// FCS table de consultation calculée par le générateur de table.
//-----------------------------------------------------------------------------------------------------------------------------------------
short int fcstab[] = { 0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
            0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5, 0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 0x4204,
            0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 0x6306, 0x728f,
            0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 0x8408, 0x9581, 0xa71a,
            0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 0xa50a, 0xb483, 0x8618, 0x9791,
            0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028,
            0x91a1, 0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3,
            0x8238, 0x93b1, 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78 };

//init du  mode JHOST
//TODO Faire une dialog box po les valeurs

void InitPTC() {
    char sTmp[50];

    bR = false;
    EnvCommand(0xFF, 0x41, "G");
    usleep(2000);
    EnvCommand(0x04, 0x01, "#TONES 4");      // force le max pactor
    usleep(2000);
    EnvCommand(0x04, 0x01, "#FSKA 180");  // ALC = 0
    usleep(2000);
    EnvCommand(0x04, 0x01, "#PSKA 190");  //  ALC=0
    usleep(2000);
    sprintf(sTmp, "#MYCALL %s", sMycall);
    EnvCommand(0x04, 0x01, sTmp);

}

// Retrouve le texte du sujet.
// Le nom sous lequel est envoyé le fichier devient le sujet c bizzare
// la reponse est limité a 70char
char * FindSujet(const char *FName) {
    int iCpc;
    int iOut;
    FILE *fp;
    int c;

    if ((fp = fopen(FName, "rb")) == NULL) {
        return NULL;
    }
    iCpc = 0;
    iOut = 0;
    sBuff[70] = 0;
    while ((c = fgetc(fp)) != EOF) {
        sBuff[iCpc++] = c;
        if ((c == '\n') || (c == '\r') || (iCpc > 69)) {
            if (iOut == 1) {
                sBuff[--iCpc] = 0;
                fclose(fp);
                return sBuff;
            }
            iCpc = 0;
        }
        if (strncmp(sBuff, "Subject: ", 9) == 0) {
            iCpc = 0;
            iOut = 1;
            sBuff[7]='x'; // si la premiere lettre du sujet est un S revalide le if et saute le S
        }
    }
    fclose(fp);
    return NULL;
}

void calchalrep(char *challenge, char *response, char *password) {
    char *hash_input;
    unsigned char hash_sig[16];
    unsigned int m, n;
    int i, pr;
    char pr_str[20];

    m = strlen(challenge) + strlen(password);
    n = m + sizeof(sl_salt);
    hash_input = (char*) malloc(n);
    strcpy(hash_input, challenge);
    strcat(hash_input, password);
    strupper(hash_input);
    memcpy(hash_input + m, sl_salt, sizeof(sl_salt));
    md5_buffer(hash_input, n, hash_sig);
    free(hash_input);

    pr = hash_sig[3] & 0x3f;
    for (i = 2; i >= 0; i--)
        pr = (pr << 8) | hash_sig[i];

    sprintf(pr_str, "%08d", pr);
    n = strlen(pr_str);
    if (n > 8)
        strcpy(response, pr_str + (n - 8));
    else
        strcpy(response, pr_str);
}

char *strupper(char *s) {
    unsigned char *cp;

    if (s == NULL) {
        return NULL;
    }
    for (cp = (unsigned char *) s; *cp; cp++) {
        if (islower(*cp)) {
            *cp = toupper(*cp);
        }
    }
    return s;
}

struct qzbuffer * buffer_readfile(const char *path) {
    FILE *fp;
    int c;
    struct qzbuffer *buf;

    if ((fp = fopen(path, "rb")) == NULL) {
        return NULL;
    }
    if ((buf = buffer_new()) == NULL) {
        fclose(fp);
        return NULL;
    }
    while ((c = fgetc(fp)) != EOF) {
        if (buffer_addchar(buf, c) == -1) {
            fclose(fp);
            buffer_free(buf);
            return NULL;
        }
    }
    if (fclose(fp) != 0) {
        buffer_free(buf);
        return NULL;
    }
    return buf;
}

struct qzbuffer * buffer_new(void) {
    struct qzbuffer *b;

    if ((b = malloc(sizeof(struct qzbuffer))) == NULL) {
        return NULL;
    }
    b->alen = 1;
    b->dlen = 0;
    b->i = 0;
    if ((b->data = malloc(b->alen * sizeof(unsigned char))) == NULL) {
        return NULL;
    }
    return b;
}

int buffer_addchar(struct qzbuffer *b, int c) {
    unsigned char *d;
    unsigned long newlen;

    if (b->dlen == b->alen) {
        newlen = b->alen * 2;
        if ((d = realloc(b->data, newlen * sizeof(unsigned char))) == NULL) {
            return -1;
        }
        b->data = d;
        b->alen = newlen;
    }
    b->data[b->dlen++] = (unsigned char) c;
    return 0;
}

void buffer_free(struct qzbuffer *b) {

    if (b->data) {
        free(b->data);
    }
    free(b);
}

void buffer_rewind(struct qzbuffer *b) {

    b->i = 0;
}

int buffer_iterchar(struct qzbuffer *b) {

    if (b->i >= b->dlen) {
        return EOF;
    }
    return b->data[b->i++];
}

/*
 *   ***envoie le fichier compressé***
 *   dans le cas ou la demande se fait a partir d'un offset offst est non nul (averifier le fonctionnement)
 *  remet a zero le compteur d'octet
 *  Normalment le buffer est largement plus grand que ce qu'il est raisonnable d'envoyer. Il n'y a pas de verif
 *  de place dans le buffer
 */
int putcompressed(char *titre, long int offst, struct qzbuffer *buf) {
    unsigned int len;
    char title[81];
    unsigned char offset[7];
    unsigned char *cp;
    unsigned long rem;
    unsigned char msglen;
    int cksum; // le 17/08 etait globale

    cksum = 0;
    bNoAff = true;
    EnvCommand(0x04, 0x01, "%T 0");  // reset du compteur d'octet
    sprintf(title, "%s", titre);
    snprintf((char *) offset, sizeof(offset), "%lu", offst);
    len = strlen((const char *) title) + strlen((const char *) offset) + 2;
    sprintf(slgnbuffer, "%c%c%s%c%s%c", CHRSOH, len, title, CHRNUL, offset, CHRNUL);
    EnvData(0x04, len + 2, slgnbuffer); // Send header
    iNToSend+=len+2;

    rem = buf->dlen;
    cp = buf->data;
    if (rem < 6)
        return -2;
    cp += offst;
    rem -= (long) offst;
    if (rem < 0)
        return -3;

    while (rem > 0) {
        if (rem > 250)   // le 28/08 etait 246
            msglen = 250;
        else
            msglen = (unsigned char) rem;
        sprintf(slgnbuffer, "%c%c", CHRSTX, msglen);

        len = 0;
        while (msglen--) {
            cksum = (cksum + *cp) % 256;
            slgnbuffer[2 + len++] = *cp++;
            rem--;
        }
        EnvData(0x04, 2 + len, slgnbuffer);
        iNToSend+=len+2;
    }
    cksum = -cksum & 0xff;   // Send checksum
    sprintf(slgnbuffer, "%c%c", CHREOT, cksum);
    EnvData(0x04, 2, slgnbuffer);
    iNToSend+=2;
    bNoAff = false;
    return 0;
}

int buffer_writefile(const char *path, struct qzbuffer *buf) {
    FILE *fp;
    int c;

    if ((fp = fopen(path, "wb")) == NULL) {
        return -1;
    }
    buffer_rewind(buf);
    while ((c = buffer_iterchar(buf)) != EOF) {
        if (fputc(c, fp) == EOF) {
            fclose(fp);
            return -1;
        }
    }
    if (fclose(fp) != 0) {
        return -1;
    }
    return 0;
}

// Parse la phrase FC po retrouver la longeur
//et met le nom dans s  
int FCLongParse(char *s) {
    gchar ** gsaPart;
    int iR;

    gsaPart = g_strsplit(s, " ", 7);
    iR = atoi(gsaPart[4]);
    strcpy(s, gsaPart[2]);
    g_strfreev(gsaPart);
    return iR;
}

//Retrouve les preferences
int LitConfig(void) {
    int iq, ir, is, pos;
    char sTmpp[20];
    FILE *fp;
    char c;
    char buffer[MAXBUFFLON];

    sprintf(buffer, "%s/PNMail/PNMail.conf", getenv("HOME"));
    if ((fp = fopen(buffer, "r")) == NULL) {
        return -1;
    }

    do { // read all lines in file
        pos = 0;
        do { // read one line
            c = fgetc(fp);
            if (c != EOF)
                buffer[pos++] = (char) c;
            if (pos >= MAXBUFFLON - 1) { // ierreur lecture trop longue
                break;
            }
        } while (c != EOF && c != '\n');
        buffer[--pos] = 0; // line is now in buffer
        iq = 0;
        for (ir = 0; ir < strlen(buffer); ir++) {
            switch (iq) {
            case 0:
                sTmpp[ir] = buffer[ir]; // charge la phrase
                if (sTmpp[ir] == ':') {
                    sTmpp[ir - 1] = 0;
                    if (strcmp(sTmpp, "CallAdress") == 0)
                        iq = 1;
                    if (strcmp(sTmpp, "PassWord") == 0)
                        iq = 2;
                    if (strcmp(sTmpp, "Locator") == 0)
                        iq = 3;
                    is = 0;
                }
                break;
            case 1:
                sMycall[is++] = buffer[ir]; // charge la phrase
                if (buffer[ir] == '@') {
                    sMycall[--is] = 0;
                    iq = 0;
                }
                break;
            case 2:
                password[is++] = buffer[ir]; // charge la phrase
                if (buffer[ir + 1] == 0) {
                    password[is] = 0;
                    iq = 0;
                }
                break;
            case 3:
                locator[is++] = buffer[ir]; // charge la phrase
                if (buffer[ir + 1] == 0) {
                    locator[is] = 0;
                    iq = 0;
                }
                break;
            }
        }
    } while (c != EOF);
    fclose(fp);
    return 0;
}

// envoie commande au ptc
void EnvCommand(unsigned char cCh, unsigned char cCmd, char * sCmd) {
    int iStat;
    int ina;

    ina = 4;
    bR = !bR;
    sBuff[0] = cCh;
    sBuff[1] = cCmd;
    sBuff[1] |= bR << 7;   // met le ack
    sBuff[2] = strlen(sCmd) - 1;
    for (int it = 0; it <= strlen(sCmd); it++) {
        sBuff[3 + it] = sCmd[it];
    }

    do {
        usleep(2000);
        ina--;
        if ((iStat = EnvTram(sBuff, strlen(sCmd) + 3)) != 0) {
            printf("\r\n   **Erreur command %d ; %d -> %s", iStat, ina, sBuff);  //debug
        }
    } while ((iStat != 0) && (ina > 0));
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// cas verif retourne 0x0F47 si ok ;cas calcule retourne le crc avec les octets inversé
//-----------------------------------------------------------------------------------------------------------------------------------------
int calculate_crc(unsigned char *cTab, int inn) {
    ushort fcs = 0xffff; // valeur FCS Initiale
    //  ushort PPPGOODFCS16 = 0xf0b8; // bonne valeur FCS finale
    int j, x;

    for (j = 0; j < inn; j++) {
        x = (fcs ^ (ushort) cTab[j]) & 0x00ff;
        fcs = (ushort) ((fcs >> 8) ^ fcstab[x]);
    }
    return (fcs ^ 0xFFFF);      // cas verif retourne 0x0F47 si ok ;cas calcule retourne le crc avec les octets inversé
}

// Envoie une trame rajoute entete et crc
// Puis attent et verifie la reponse
// reponse dans le tableau global cRep
// si tout vas bien retour de 0
//  stuff -> il faut faire suivre un octet AA d'un octet 0
int EnvTram(unsigned char *cTrame, int its) {
    long long lStart, lStop;
    int inz;
    bool bIsHeader, bIsStuff, bIsZeroTerm;
    int iRCrc, iRec, iTotRec;
    int iCrc;
    struct timeval tnow;
    unsigned char sStuffTram[276];
    unsigned char cree[MAXBUFFLON];

    inz = read(tty, cree, MAXBUFFLON - 1);     //vide le tampon de reception normalement y doit rien y avoir
    if (inz > 0) {  // il y a un prob
        printf("\r\nDeb : %d", inz);  //debug
    }

    sStuffTram[0] = 0xAA;
    sStuffTram[1] = 0xAA;

    inz = 2;
    for (int io = 0; io < its; io++) {
        unsigned char cx = *(cTrame + io);
        sStuffTram[inz + io] = cx;
        if (cx == 0xAA)   //   stuff si 0xAA envoie un 0 a la suite
            sStuffTram[++inz + io] = 0;
    }

    iCrc = calculate_crc(cTrame, its);
    sStuffTram[inz + its] = (unsigned char) (iCrc & 0xFF);
    if (sStuffTram[inz + its] == 0xAA)
        sStuffTram[++inz + its] = 0;
    sStuffTram[inz + its + 1] = (unsigned char) ((iCrc >> 8) & 0xFF);
    if (sStuffTram[inz + its + 1] == 0xAA)
        sStuffTram[++inz + its + 1] = 0;
    inz = write(tty, sStuffTram, inz + its + 2);

    // reponse
    gettimeofday(&tnow, NULL);  // Base delais
    lStart = 500 + tnow.tv_sec * 1000LL + tnow.tv_usec / 1000;

    bIsHeader = false;
    iRCrc = 0xFFFF;
    iTotRec = 0;
    bIsStuff = false;
    iRec = 0;
    bIsZeroTerm = false;
    inz = 0;

    do {
        usleep(500);
        if (ioctl(tty, FIONREAD, &inz) < 0)
            printf("\r\n Erreur du ioctl");
        if (((inz != 0) && (bIsHeader)) || (inz >= 4)) {
            inz = read(tty, cree, MAXBUFFLON - 1);

            for (int iy = 0; iy < inz; iy++)  //calcul crc Sort le stuffing et sort l'entete
                        {
                if (bIsHeader) {
                    if (bIsStuff) {
                        bIsStuff = false;
                        if (cree[iy] != (unsigned char) 0x00)
                            return 4;   // error le stuff n'est pas nul
                        continue;
                    }   //destuff
                    if (cree[iy] == (unsigned char) 0xAA)
                        bIsStuff = true;

                    cRep[iRec] = cree[iy];  //constitue  le paquet retour
                    iRec++;

                    if ((bIsZeroTerm) && (cree[iy] == (unsigned char) 0x00))
                        iTotRec = 2;   // paquet zero terminated rencontre de zero plus que le crc a lire

                    int x = (iRCrc ^ cree[iy]) & 0x00ff;   //calc CRC
                    iRCrc = (ushort) ((iRCrc >> 8) ^ fcstab[x]);
                    if (iTotRec == 0) {
                        if (iRCrc == 0xF0B8)
                            return 0;
                        else
                            return 2;   //err CRC
                    }
                    iTotRec--;
                } else { // repere le header
                    if ((cree[0] != (unsigned char) 0xAA) || (cree[1] != (unsigned char) 0xAA))
                        return 1;  //erreur entete non reconue
                    else {
                        bIsHeader = true;
                        iy = 1;
                        int iiz = cree[3] & 0x07;
                        if ((cree[2] == (unsigned char) 0xAA) && (cree[3] == (unsigned char) 0x85))
                            return 10; // erreur demande de renvoie
                        if (iiz == (unsigned char) 0x00)
                            iTotRec = 3;  // paquet court
                        if ((iiz > 0) && (iiz < 6)) {  // Paquet terminé par 0
                            iTotRec = 0x3FF;
                            bIsZeroTerm = true;
                        }
                        if ((iiz == 6) || (iiz == 7)) {
                            iTotRec = ((cree[3] & 0x30) << 4) + cree[4] + 5;
                        }
                    }
                }
            }
        }
        gettimeofday(&tnow, NULL);
        lStop = tnow.tv_sec * 1000LL + tnow.tv_usec / 1000; // caculate milliseconds
    } while (lStart > lStop);  // la reponse doit etre arrivé avant delais
    return 3;
}

void PtcRest() {
    cRep[0] = 0x1B;    // esc 27
    write(tty, cRep, 1);
    EnvCommand(0x1F, 0x41, "JHOST0");
}

// Check le canal 254 de status et met a jour le label
// Contraire a la doc le premier octet ne bouge pas
// si on est en envoie check le nombre d'octet envoyé
bool PollStatus(char *sPStat) {
    //unsigned char ucTm, ucTe,
    unsigned char ucTd;
    int iTF;
    static int iTotTrans;

    //  const char *csMode[] = { "STANDBY", "AMTOR-ARQ", " PACTOR-ARQ", "AMTOR-FEC", "PACTOR-FEC", "RTTY / CW", "LISTEN", "Channel-Busy" };
    //  const char *csEtat[] = { "ERROR", "REQUEST", "TRAFFIC", "IDLE", "OVER", "PHASE", "SYNCH", "IGNORE" };
    const char *csDirection[] = { "Recoit", "Envoie" };
    const char *csPactor[] = { "Pas Connecté", "PACTOR-I", "PACTOR-II", "PACTOR-III", "Erreur" };
    const int iaPactorSpid[] = { 200, 600, 1400, 2800, 3200, 3600 };

    if (bDispCpt) {  // Affiche le compteur d'octet envoyé
        EnvCommand(0x04, 0x01, "%T");  // la reponse est le nbre d'octet  envoyé
         if (cRep[1] != 0) {
            iTF = 0;
            do {
                sBuff[iTF] = cRep[iTF + 2];    // 21/08 etait 3
                iTF++;
            } while ((iTF < 6) && (cRep[iTF - 1] != 0));
            iTotTrans = atoi(sBuff);  // 21/08 etait +=
        }
        sprintf(sPStat, "%d octets envoyés sur %d", iTotTrans,    iNToSend); //  la longueur a envoyer comprend les messages +les entetes
    } else {
        EnvCommand(0xFE, 0x01, "G3");  // la reponse est 4 octets

        iTotTrans = 0;
        //   ucTm = (cRep[3] && (0x70)) >> 4;
        //   ucTe = (cRep[3] && (0x03));
        ucTd = ((cRep[3] & (0x04)) == 0x04) ? 1 : 0;
        if (cRep[4] > 3)
            cRep[4] = 4;
        iTF = (int) cRep[6];
        if (iTF > 127)
            iTF -= 128;
        if (cRep[4] == 0)
            sprintf(sPStat, "%s", csPactor[cRep[4]]);
        else
            sprintf(sPStat, "%s - %s debit: %d Bds, Ecart de Frequence: %d Hz", csDirection[ucTd], csPactor[cRep[4]], iaPactorSpid[cRep[5]], iTF);
        //sprintf(sPStat, " %s  %s  %s %s niveau: %d , Ecart de Frequence: %d Hz", csMode[ucTm], csEtat[ucTe], csDirection[ucTd], csPactor[cRep[4]], cRep[5], iTF);
        //  GtkWidget *pentry = (GtkWidget *) gtk_builder_get_object(p_builder, "labStatus");
        //  gtk_label_set_text(GTK_LABEL(pentry), sLab);
    }

    if (cRep[4] != 0)
        return true;
    else
        return false;
}

// Envoie un paquet de data
// ILo doit etre inferireur a 250
void EnvData(unsigned char cCh, unsigned int iLo, char * sCmd) {
    int iStat;
    GtkTextIter iter;
    int ina;

    if (bNoAff == false) {
        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, sCmd, iLo, tagG, NULL);
    }
    ina = 4;
    bR = !bR;
    sBuff[0] = cCh;
    sBuff[1] = bR << 7;   // met le ack
    sBuff[2] = iLo - 1;
    for (unsigned int it = 0; it < iLo; it++) {
        sBuff[3 + it] = sCmd[it];
    }

    do {
        usleep(2000);
        ina--;
        if ((iStat = EnvTram(sBuff, iLo + 3)) != 0) {
            printf("\r\n   **Erreur Data %d ; %d ->   %s", iStat, ina, sBuff);    //debug
        }
    } while ((iStat != 0) && (ina > 0));
}

