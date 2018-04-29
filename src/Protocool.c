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
 Name        : Protocool.c
 Author      : Xynium
 Version     : juillet 2016
 Copyright   : FM4PN JPLathuile
 Description : Annexe po wl2k
 *                    protocole wl2k FBB
 ============================================================================
 */

/* TODO
 *  supprimer l'erreur 34 qui arrive souvent doit y avoir moyen de demander retransmission du block
 */

#include "Autre.h"
//#include <gtk/gtk.h>

int iNQTC;
int iRQTC;
int iaMstR[MAXRECPROP];
char saNam[MAXRECPROP][15];  // normalment contient le mid limité a 12octets
struct message mmsg[MAXRECPROP];   // tableau des proposals en emission



// Principal du protocole wl2K
// appellé du cas 3 du switch   static gboolean timeout_callback(gpointer data)
void DEDExtraitD(GtkWidget *pBtn) {
    int ina;
    static char sCod[20];
    char sCodR[20];
    GtkTextIter iter, iter2;
    int irt;
    static int iGetStat;
    static int cksum;
    static int ipos;
    static int inbrR;
    static unsigned int len;
    struct qzbuffer *ubuf;

    ina = (int) (((cRep[1] & 0x30) << 4) + cRep[2]);  // Nombre d'octet a lire -1

    if (((cRep[1] & 0x06) == 0x06) && (ina < 1024)) { // la reponse est ina données
        for (int inb = 3; inb < ina + 4; inb++) {

            switch (iTranState) {  // Traitement protocole
            case 0:    // repere le PQ et passe a l'etat 1 a reception de >
                if (cRep[inb] != '\n')
                    slgnbuffer[iposr++] = cRep[inb];
                if (cRep[inb] == '\r') { //fin de lgn
                    if (--iposr > 1) {
                        slgnbuffer[iposr] = 0;
                        strcpy(buffer, slgnbuffer);
                        strcat(buffer, "\r\n");
                        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // Affiche la ligne reçue
                        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, iposr + 2, tagB, NULL);
                    }
                    if ((iposr > 3) && (slgnbuffer[iposr - 1] == '>') && (slgnbuffer[iposr - 2] == ' ')) { // repere > po commencer envoie
                        sprintf(slgnbuffer, "[TuxLINK-0.5.1.0-B2FHM$]\r");
                        EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
                        if (strlen(sCod) != 8)
                            TermineAvecErreur(10, pBtn); // si PQ n'est pas repere terminer
                        calchalrep(sCod, sCodR, password);
                        sprintf(slgnbuffer, ";PR: %s\r", sCodR);
                        EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
                        sprintf(slgnbuffer, ";%s de %s %s\r", sStatcall, sMycall, locator);
                        EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
                        MesProposals();
                        iTranState = 2;   // fin de l'envoie de la reponse
                        iposr = 0;
                    }
                    iposr = 0;
                    if ((strncmp(slgnbuffer, ";PQ:", 4)) == 0) { // cherche si ligne PQ
                        for (int jt = 5; jt < strlen(slgnbuffer); jt++)
                            sCod[iposr++] = slgnbuffer[jt];
                        sCod[iposr] = 0;
                        iposr = 0;
                    }
                }
                break;   // fin case 0

            case 2:    // gere les reponse a mes proposals envoie de mes message
                if (cRep[inb] != '\n')
                    slgnbuffer[iposr++] = cRep[inb];
                printf("%x ", cRep[inb]);    //debug
                if (cRep[inb] == '\r') { //fin de lgn
                    if (--iposr > 1) {
                        slgnbuffer[iposr] = 0;
                        strcpy(buffer, slgnbuffer);
                        strcat(buffer, "\r\n");
                        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // Affiche la ligne reçue
                        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, iposr + 2, tagB, NULL);
                        bDispCpt = false;
                    }
                    iposr = 0;
                    if (strncmp(slgnbuffer, "FS ", 3) == 0) {     //proposal acceptage

                        for (int jt = 3; (jt < strlen(slgnbuffer)) && (jt < MAXRECPROP + 3); jt++)
                            if ((slgnbuffer[jt] == 'Y') && (mmsg[jt - 3].ietat == 1))
                                mmsg[jt - 3].ietat = 2;

                        iNToSend = 0;
                        bDispCpt = true;  //ligne status affiche le compteur d'envoie

                        for (int jt = 0; jt < iNQTC; jt++) {
                            if (mmsg[jt].ietat == 2) {
                                sprintf(buffer, "Envoie %s %ld Octets.\r\n", mmsg[jt].title, mmsg[jt].cbuf->dlen);
                                gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                                gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, strlen(buffer), tagJ, NULL);
                                if ((irt = putcompressed(mmsg[jt].title, 0, mmsg[jt].cbuf)) != 0) {
                                    sprintf(buffer, "Erreur d'ecriture %d\r\n", irt);    //Erreur
                                    gtk_text_buffer_get_end_iter(tvBuffer, &iter);
                                    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, strlen(buffer), tagR, NULL);
                                    TermineAvecErreur(26, pBtn);
                                }
                                mmsg[jt].ietat = 3;
                            } else {
                                sprintf(buffer, "Refusé %s %ld Octets.\r\n", mmsg[jt].title, mmsg[jt].cbuf->dlen);
                                gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                                gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, strlen(buffer), tagJ, NULL);

                            }
                            if (mmsg[jt].cbuf)
                                buffer_free(mmsg[jt].cbuf);
                        }
                        iNQTC = 0;
                        iposr = 0;
                    }
                    if (strcmp(slgnbuffer, "FF") == 0) {     //ack deconnecte
                        printf("On deconnecte.\r\n");  //debug
                        sprintf(slgnbuffer, "On deconnecte.\r\n");
                        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagJ, NULL);
                        EnvoieFQ(pBtn);
                    }
                    if (strncmp(slgnbuffer, "FC", 2) == 0) {     //Retrouve dans la lgnProposal la longueur et met dans un tableau TODO tester sur FC EM
                        iaMstR[iNQTC] = FCLongParse(slgnbuffer);  // splite et retourne longueur
                        strcpy(saNam[iNQTC], slgnbuffer);  // la fonction a modilfié pointer
                        iNQTC++;
                    }
                    if (strncmp(slgnbuffer, "FQ", 2) == 0) {     //rien a recevoir on termine
                        printf("La station deconnecte.\r\n");  //debug
                        sprintf(slgnbuffer, "La station deconnecte.\r\n");
                        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagJ, NULL);
                        EnvoieFQ(pBtn);
                    }
                    if (strncmp(slgnbuffer, "F> ", 3) == 0) {     //il y a a recevoir TODO Verifier le checksum
                        sprintf(slgnbuffer, "FS ");
                        for (int it = 0; it < iNQTC; it++)
                            slgnbuffer[3 + it] = 'Y';
                        slgnbuffer[3 + iNQTC] = '\r';
                        slgnbuffer[4 + iNQTC] = 0;
                        EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
                        ipos = 0;  // init po reception
                        iRQTC = 0;
                        iTranState = 3;
                        bNoAff = true;
                        iGetStat = 0;
                        printf("\r\n Verif inb= %d   ina= %d", inb, ina);  // debug inb doit etre = ina +3

                    }
                }
                break;   // fin case 2

            case 3:   // recoit les messages
                switch (iGetStat) {
                case 0: // charge le header
                    printf("\r\n Dans 3 inb = %d cRep[inb] = %X", inb, cRep[inb]);  //debug
                    if (cRep[inb] == CHRSOH) {      // ((inb == 3) && (cRep[inb] == CHRSOH))
                        iGetStat++;
                        if ((cbuf = buffer_new()) == NULL)
                            TermineAvecErreur(300, pBtn);                        // Termine il y une erreur le buffer ne peut etre alloue
                    } else {  //sort erreur
                        inb = MAXBUFFLON;
                        TermineAvecErreur(301, pBtn);
                    }
                    break;
                case 1: // charge la longueur mais sert a rien
                    len = cRep[inb];
                    iGetStat++;
                    break;
                case 2: // charge le titre
                    mmsg[0].title[ipos++] = cRep[inb];
                    if (cRep[inb] == 0) {
                        ipos = 0;
                        iGetStat++;
                        inbrR = 0;
                        sprintf(slgnbuffer, "Recoit : %s    0%%", mmsg[0].title);
                        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagJ, NULL);
                    }
                    if (ipos > 179) { //04/2018 was 79 but some title are longuer    secu depassement
                        inb = MAXBUFFLON;
                        TermineAvecErreur(32, pBtn);
                    }
                    break;
                case 3: // charge offset
                    mmsg[0].offset[ipos++] = cRep[inb];
                    if (cRep[inb] == 0) {
                        ipos = 0;
                        iGetStat++;
                        cksum = 0;
                        if (strcmp((const char *) mmsg[0].offset, "0") != 0) { // du code original n'exploite pas offset
                            inb = MAXBUFFLON;
                            TermineAvecErreur(33, pBtn);
                        }
                    }
                    if (ipos > 6) {
                        inb = MAXBUFFLON;
                        TermineAvecErreur(331, pBtn);
                    }
                    break;
                case 4: // debut charge payload verifie un CHRSTX
                    printf("\r\nDans 4  %d ", cRep[inb]);  // debug
                    if (cRep[inb] == CHRSTX)       //CHRSTX= 2
                        iGetStat = 5;
                    if (cRep[inb] == CHREOT)      //CHREOT= 4
                        iGetStat = 7;
                    if (iGetStat == 4) {
                        inb = MAXBUFFLON;
                        TermineAvecErreur(34, pBtn); // le block n'est pas valide doit y avoir un moyen de demander retransmission
                    }
                    break;
                case 5: // Charge la longueur
                    len = (unsigned int) cRep[inb];
                    if (len == 0)
                        len = 256;
                    iGetStat = 6;
                    printf("\r\nDans 5  %d ", len);  // debug
                    break;
                case 6: // Charge la vrai payload len char
                    len--;
                    if (buffer_addchar(cbuf, cRep[inb]) != 0) {
                        inb = MAXBUFFLON;
                        TermineAvecErreur(36, pBtn);
                    }
                    cksum = (cksum + cRep[inb]) % 256;
                    if (len == 0)
                        iGetStat = 4;
                    // indic de progression
                    inbrR++;
                    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // recule de 3char
                    gtk_text_buffer_get_end_iter(tvBuffer, &iter2);
                    gtk_text_iter_backward_chars(&iter2, 3);
                    gtk_text_buffer_delete(tvBuffer, &iter2, &iter);
                    sprintf(slgnbuffer, "%2d%%", (int) (100 * inbrR / iaMstR[iRQTC]));
                    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagJ, NULL);   // fin indic progress
                    break;
                case 7: //Verif check sum
                    printf("\r\nDans 7");  //debug
                    cksum = (cksum + cRep[inb]) % 256;
                    if (cksum != 0) { //sort erreur
                        TermineAvecErreur(371, pBtn);
                    }
                    if ((ubuf = version_1_Decode(cbuf)) == NULL) {
                        printf("version_1_Decode() - %s", strerror(errno));
                        TermineAvecErreur(372, pBtn);
                    }
                    sprintf(slgnbuffer, "%s/PNMail/InBox/%s.msg", getenv("HOME"), saNam[iRQTC++]);   // forme le nom du fichier
                    printf("\n\r Ckeck  %d  name  %s\r\n", cksum, slgnbuffer);  // debug
                    if (buffer_writefile(slgnbuffer, ubuf) != 0) {                               //stoque  en fichier le msg reçu
                        printf("buffer_writefile - %s", strerror(errno));
                        TermineAvecErreur(373, pBtn);
                    }
                    sprintf(slgnbuffer, "Recu \r\n");
                    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagJ, NULL);
                    if (ubuf)
                        buffer_free(ubuf);
                    if (iRQTC >= iNQTC) {  // tout recu ?
                        bNoAff = false;
                        printf("On deconnecte.\r\n");  //debug
                        sprintf(slgnbuffer, "On deconnecte.\r\n");
                        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagJ, NULL);
                        sprintf(slgnbuffer, "FF\r");   // TODO Verifier si necessaire
                        EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
                        EnvoieFQ(pBtn);
                    } else {  // po les autres messages
                        iGetStat = 0;
                    }
                    break;
                }
                break; //fin case 3
            }  //fin switch
        }
    }
}

// Extrait la chaine depuis cRep[] dans sC
// appellé du cas 3 du switch   static gboolean timeout_callback(gpointer data)
void DEDExtraitC(void) {
    int iT;

    iT = 1;
    buffer[0] = '\r';
    buffer[1] = '\n';
    do {
        iT++;
        buffer[iT] = cRep[iT];
    } while ((cRep[iT] != 0) && (iT < 512));
    buffer[iT] = '\r';
    buffer[iT + 1] = '\n';
    buffer[iT + 2] = 0;
}

//Cherche et envoie ligne proposal
// le buffer est pret a etre envoyé
void MesProposals(void) {
    GtkTextIter iter;
    struct dirent *ent;
    DIR *dir;
    char sCoT[20];
    char sCoR[200];
    int cksum;
    struct qzbuffer *ubuf;

    iNQTC = 0;    // envoie mes proposal
    cksum = 0;
    sprintf(slgnbuffer, "%s/PNMail/OutBox", getenv("HOME"));
    if ((dir = opendir(slgnbuffer)) != NULL) {     // cherche le nombre de message a Tx
        while ((ent = readdir(dir)) != NULL) {
            if ((ent->d_type == DT_REG) && (ent->d_name[0] != '*')) {
                iNQTC++;
                if (iNQTC > MAXRECPROP) {
                    printf("Trop de message pour ce QSO");
                    break;
                }
                sprintf(slgnbuffer, "%s/PNMail/OutBox/%s", getenv("HOME"), ent->d_name);
                strcpy(mmsg[iNQTC - 1].title, FindSujet(slgnbuffer));
                if ((ubuf = buffer_readfile(slgnbuffer)) == NULL) {     // lit le fichier
                    sprintf(buffer, "%s - %s", ent->d_name, strerror(errno));   //Erreur
                    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, strlen(buffer), tagR, NULL);
                    return;
                }
                if ((mmsg[iNQTC - 1].cbuf = version_1_Encode(ubuf)) == NULL) {   // Compresse le buffer
                    sprintf(buffer, "Erreur dans la compression du fichier.");    //Erreur
                    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
                    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, strlen(buffer), tagR, NULL);
                    return;
                }
                strcpy(sCoT, ent->d_name);   //Renome le fichier avec une * TODO changer de place le faire une fois envoie confirme
                sCoT[0] = '*';
                sprintf(sCoR, "%s/PNMail/OutBox/%s", getenv("HOME"), sCoT);
                rename(slgnbuffer, sCoR);
                ent->d_name[strlen(ent->d_name) - 4] = 0;  // tronque extension
                sprintf(slgnbuffer, "FC EM %s %ld %ld 0\r", ent->d_name, ubuf->dlen, mmsg[iNQTC - 1].cbuf->dlen);
                EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
                mmsg[iNQTC - 1].ietat = 1;

                for (int it = 0; it < strlen(slgnbuffer); it++) { // check sum   doit y avoir \r a la fin
                    cksum += (unsigned char) slgnbuffer[it];
                }
                if (ubuf)
                    buffer_free(ubuf);
            }
        }
           closedir(dir);
    }

    if (iNQTC == 0) { // pas de message a envoyer
        sprintf(slgnbuffer, "FF\r");
        EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
    }
    else{
        cksum = -cksum & 0xff;
              sprintf(slgnbuffer, "F> %X\r", cksum);
              EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
    }
    iposr = 0;
}

void TermineAvecErreur(int ierr, GtkWidget *pBtn) {
    GtkTextIter iter;

    printf("\r\n ERROR %d", ierr); //debug
    bNoAff = false;
    sprintf(slgnbuffer, "\r ERROR %d\r\n", ierr);
    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagR, NULL);
    sprintf(slgnbuffer, ";%s de %s got an error\r", sStatcall, sMycall);
    EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
    EnvoieFQ(pBtn);

    sprintf(slgnbuffer, "Erreur Deconnecte.\r\n");
    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagR, NULL);
}

// Todo revenir en jhost0 si il y a des erreus de reconnections
void EnvoieFQ(GtkWidget *pBtn) {
    GtkCssProvider *css_provider = NULL;
    GtkTextIter iter;

    printf("\r\n  *** Dans FQ ***   "); //debug
    bNoAff = false;
    sprintf(slgnbuffer, ";%s de %s  Out\r", sStatcall, sMycall);
    EnvData(0x04, strlen(slgnbuffer), slgnbuffer);

    sprintf(slgnbuffer, "FQ\r");
    EnvData(0x04, strlen(slgnbuffer), slgnbuffer);
    gtk_button_set_label(GTK_BUTTON(pBtn), "Connecte");
    css_provider = gtk_css_provider_get_default();
    strcpy(slgnbuffer, sPath);
    strcat(slgnbuffer, "PNWl2kAir.css");
    gtk_css_provider_load_from_file(css_provider, g_file_new_for_path(slgnbuffer), NULL);
    EnvCommand(0x04, 0x01, "D");     //Disconnect
    sprintf(slgnbuffer, "\r\nDeconnecte.\n\r");
    gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
    gtk_text_buffer_insert_with_tags(tvBuffer, &iter, slgnbuffer, strlen(slgnbuffer), tagJ, NULL);
    iFlag = 7;  // c'est fini
}

