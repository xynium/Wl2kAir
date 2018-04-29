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
 Name        : PNWl2kAir.c
 Author      : Xynium
 Version     : Juillet 2016
 Copyright   : JPLathuile FM4PN
 Description : client  winlink PACTOR
 ============================================================================
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

#include "Autre.h"

#define BAUDRATE 115200

GtkBuilder * pbuilder = NULL;
GtkWidget *pBtn = NULL;
gint giTimer;
bool bJHOST;
int iDead;

static gboolean timeout_callback(gpointer data);

// Callbac du boutton
static void btnDep_clicked_cb(GtkWidget * p_wid, gpointer p_data) {
    GtkWidget *pentry = NULL;
    GtkTextIter iter;
    GtkCssProvider *css_provider = NULL;

    switch (iFlag) {
    case 0: // doit pas arriver
        break;
    case 3: // autre pression sur le btn doit stopper si connecté
        EnvCommand(0x04, 0x01, "D");     //Disconnect
        iFlag = 4;
        gtk_button_set_label(GTK_BUTTON(pBtn), "Connecte");
        css_provider = gtk_css_provider_get_default();
        strcpy(buffer, sPath);
        strcat(buffer, "PNWl2kAir.css");
        gtk_css_provider_load_from_file(css_provider, g_file_new_for_path(buffer), NULL);
        sprintf(buffer, "\r\nDisconnect\n\r");
        gtk_text_buffer_get_end_iter(tvBuffer, &iter);   // visu
        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, strlen(buffer), tagR, NULL);
        break;
    case 4:
    case 5:
        pentry = (GtkWidget *) gtk_builder_get_object(pbuilder, "eEntree"); //recup de la stat a connecter
        sStatcall = gtk_entry_get_text(GTK_ENTRY(pentry));
        iFlag = 1;
        iposr = 0;
        break;
    case 6:
        iFlag = 0;
        iDead = 0;
        break;
    default:
        iFlag = 0;
        break;
    }
}

// initialise le port serie
void init(void) {
    GtkWidget *pdialog = NULL;
    struct termios newtio;

    tty = open(sSPort, O_RDWR | O_NOCTTY | O_NONBLOCK);  //ouverture port entrée
    if (tty <= 0) {
        pdialog = gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Impossible d'ouvrir le port serie.\nLe programme termine.");
        gtk_dialog_run(GTK_DIALOG(pdialog));
        gtk_widget_destroy(pdialog);
        return;
    }
    tcgetattr(tty, &newtio);
    cfsetispeed(&newtio, BAUDRATE);
    cfsetospeed(&newtio, BAUDRATE);

    newtio.c_iflag = IGNBRK;
    newtio.c_oflag = 0;
    newtio.c_cflag = CS8 | CLOCAL | CREAD;
    newtio.c_lflag = 0;

    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;
    tcflush(tty, TCIFLUSH);
    tcsetattr(tty, TCSANOW, &newtio);

}

static gboolean timeout_callback(gpointer data) {
    int iRa;
    GtkTextIter iter;
    GtkWidget *pscroll = NULL;
    GtkAdjustment *verticalAdjust;
    GError * p_err = NULL;
    GtkCssProvider *css_provider = NULL;
    char sStatus[100];

    int iTmp;
    GFile *file = NULL;

    switch (iFlag) {
    case 0: {      // init du ptc reste en mode console
        if (iDead > 50) {
            sprintf(sStatus, "Probleme Reinitialise le PTC\r\n");
            iFlag = 6;
            gtk_text_buffer_get_end_iter(tvBuffer, &iter);
            gtk_text_buffer_insert_with_tags(tvBuffer, &iter, sStatus, strlen(sStatus), tagR, NULL);
            gtk_button_set_label(GTK_BUTTON(pBtn), "Connecte");
            css_provider = gtk_css_provider_get_default();
            strcpy(buffer, sPath);
            strcat(buffer, "PNWl2kAirT.css");
            file = g_file_new_for_path(buffer);
            gtk_css_provider_load_from_file(css_provider, file, &p_err);
        }
        do {
            iRa = read(tty, buffer, MAXBUFFLON); //premier sens
            if (iRa == -1) {
                if (iDead % 15 == 0) {
                    sprintf(sStatus, " RESTART \r");
                    sStatus[0] = 27;
                    write(tty, sStatus, strlen(sStatus));
                    gtk_text_buffer_get_start_iter(tvBuffer, &iter);
                    //  sprintf(sStatus, "RE ");
                    //  gtk_text_buffer_insert_with_tags(tvBuffer, &iter, sStatus, strlen(sStatus), tagR, NULL);
                }
                iDead++;
            } else {
                for (int it = 0; it < iRa; it++) {
                    if (buffer[it] != '\r')
                        sStatus[iposr++] = buffer[it];
                    if (buffer[it] == '\n') { //fin de lgn
                        if (--iposr > 0)
                            sStatus[iposr] = 0;
                        iposr = 0;
                    }
                    if ((strncmp(sStatus, "cmd: ", 5) == 0) && (iposr > 1)) { // Attend une commande prepare po mode du cat passe sur la freq et moniteur
                        sprintf(buffer, "TRX ty y 4800\n\r");
                        write(tty, buffer, strlen(buffer));
                        usleep(100000);
                        sprintf(buffer, "TRX yt 6\n\r");    // po yaesu
                        write(tty, buffer, strlen(buffer));
                        usleep(100000);
                        sprintf(buffer, "TRX T 0A 00 00 00 07\n\r");  // mod dig
                        write(tty, buffer, strlen(buffer));
                        if ((iFreq > 1000) && (iFreq < 300000)) {
                            setlocale(LC_NUMERIC, "en_US.UTF-8");    // po  prendre le point po virgule
                            sprintf(buffer, "TRX F %0.1f\n\r", (((float) iFreq / 10.0) - 1.5));
                            write(tty, buffer, strlen(buffer));
                        }
                        sprintf(buffer, "L 1\n\r");
                        write(tty, buffer, strlen(buffer));
                        sprintf(sStatus, "Pret...\n\r ");
                        iFlag = 5;
                        gtk_text_buffer_get_start_iter(tvBuffer, &iter);  // recopie a l'ecran
                        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, sStatus, strlen(sStatus), tagJ, NULL);
                        css_provider = gtk_css_provider_get_default();
                        strcpy(buffer, sPath);
                        strcat(buffer, "PNWl2kAir.css");
                        file = g_file_new_for_path(buffer);
                        gtk_css_provider_load_from_file(css_provider, file, &p_err);
                    }
                }
            }
        } while (iRa == MAXBUFFLON);
        return TRUE;
    }

    case 1: // Test l'ouverture du PTC avant init
    {
        gtk_button_set_label(GTK_BUTTON(pBtn), "Deconnecte"); //Change texte button
        css_provider = gtk_css_provider_get_default();
        strcpy(buffer, sPath);
        strcat(buffer, "PNWl2kAirT.css");
        file = g_file_new_for_path(buffer);
        gtk_css_provider_load_from_file(css_provider, file, &p_err);

        buffer[0] = 0x11;
        buffer[1] = 0x18;
        buffer[2] = 0x1B;
        buffer[3] = 'J';
        buffer[4] = 'H';
        buffer[5] = 'O';
        buffer[6] = 'S';
        buffer[7] = 'T';
        buffer[8] = '5';
        buffer[9] = 0x0D;
        if (write(tty, buffer, 10) != 10) {
            sprintf(sStatus, "ERREUR ecriture\r\n ");
                gtk_text_buffer_get_end_iter(tvBuffer, &iter);  // recopie a l'ecran
        gtk_text_buffer_insert_with_tags(tvBuffer, &iter, sStatus, strlen(sStatus), tagR, NULL);
            iFlag = 5;
        } else {
    //        sprintf(sStatus, "En mode JHOST, demande connection...\r\n ");
            iFlag = 2;
        }
           iposr = 0;
        return TRUE;
    }

    case 2:    // Connect en pactor
         InitPTC();
        iFlag = 3;
        bJHOST = true;
        sprintf(sStatus, "C %s", sStatcall);
        EnvCommand(0x04, 0x01, sStatus);
        iTranState = 0;
        iposr = 0;
        bDispCpt = false;
        sprintf(sStatus, "En mode JHOST, demande connection...\r\n ");
       gtk_text_buffer_set_text (tvBuffer,sStatus,strlen(sStatus));     
        return TRUE;

    case 3:   // phase de fonctionnement normal QSO
    {
        if (PollStatus(sStatus)) { // met a jour le status
            css_provider = gtk_css_provider_get_default();
            strcpy(buffer, sPath);
            strcat(buffer, "PNWl2kAirD.css");
            file = g_file_new_for_path(buffer);
             gtk_css_provider_load_from_file(css_provider, file, &p_err);
        }
        GtkWidget *pentry = (GtkWidget *) gtk_builder_get_object(pbuilder, "labStatus");
        gtk_label_set_text(GTK_LABEL(pentry), sStatus);

        bR = !bR;   // Demande d'etat du qso
        buffer[0] = 0x04;  // canal du pactor
        buffer[1] = 0x01;
        buffer[2] = 0x00;
        buffer[3] = 'G';
        if (bR)
            buffer[1] = 0x81;
        if ((iTmp = EnvTram(buffer, 4)) == 0) {
            if (cRep[1] == 0)
                return TRUE;  // rien a faire
            if (((cRep[1] & 0x7) > 0) && ((cRep[1] & 0x7) < 6)) { // lareponse est une chaine et donne l'etat
                DEDExtraitC();
                gtk_text_buffer_get_end_iter(tvBuffer, &iter);  // recopie a l'ecran
                gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, strlen(buffer), tagJ, NULL);
            } else {
                if (cRep[0] == 0x04)   // Verif qu'on s'adresse bien au canal pactor
                    DEDExtraitD(pBtn);
            }
            pscroll = (GtkWidget *) gtk_builder_get_object(pbuilder, "scrolledwindow1");
            verticalAdjust = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(pscroll));
            gtk_adjustment_set_value(verticalAdjust, gtk_adjustment_get_upper(verticalAdjust));
            gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(pscroll), verticalAdjust);
        } else {
            sprintf(sStatus, "Re %d", iTmp);
             if (iTmp != 1)
                bR = !bR;
        }
        return TRUE;
    }
    case 4:    // Stand by en mode JHOST apres deconnection Pactor
                    return TRUE;

    case 5:    // Stand by en mode Console init / affiche le resultat du Monitoring
        iRa = read(tty, buffer, MAXBUFFLON); //premier sens
        if (iRa > 0) {
            gtk_text_buffer_get_end_iter(tvBuffer, &iter);  // recopie a l'ecran
            gtk_text_buffer_insert_with_tags(tvBuffer, &iter, buffer, iRa, tagJ, NULL);
            pscroll = (GtkWidget *) gtk_builder_get_object(pbuilder, "scrolledwindow1");
            verticalAdjust = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(pscroll));
            gtk_adjustment_set_value(verticalAdjust, gtk_adjustment_get_upper(verticalAdjust));
            gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(pscroll), verticalAdjust);
        }
        return TRUE;

    case 6:    // Stand by en attente reset du ptc
        return TRUE;

    case 7:    //Post deconnexion scrolle puis passe en stand by etat 4
                  pscroll = (GtkWidget *) gtk_builder_get_object(pbuilder, "scrolledwindow1");
                  verticalAdjust = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(pscroll));
                  gtk_adjustment_set_value(verticalAdjust, gtk_adjustment_get_upper(verticalAdjust));
                  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(pscroll), verticalAdjust);
                  iFlag=4;
          return TRUE;

    default:
        return TRUE;

    }
    return TRUE;  // Normalment doit etre sortis avant
}

static void destroy(GtkWidget *widget, gpointer data) {
    g_source_remove(giTimer);
    if (bJHOST)
        PtcRest();
    close(tty);
    gtk_main_quit();
}

int main(int argc, char ** argv) {
    GError * p_err = NULL;
    char sTmp[15];
    GtkCssProvider *css_provider = NULL;
    GtkStyleContext *context = NULL;
    GFile *file = NULL;
    char sStatus[100];

    iFlag = 0;
    bJHOST = false;
    bNoAff = false;
    iDead = 0;
    if (argc == 3) {
        setlocale(LC_NUMERIC, "en_US.UTF-8");    // po  prendre le point po virgule
        iFreq = atol(argv[2]); /* convert Freq  to integers  frequence *10 */
        strcpy(sTmp, argv[1]);
    } else {
        sTmp[0] = 0;
        iFreq = 0;
    }

//Initialisation de GTK+
    gtk_init(&argc, &argv);

// Creation d'un nouveau GtkBuilder
    pbuilder = gtk_builder_new();

    if (pbuilder != NULL) {
        //Chargement du XML dans p_builder
        strcpy(buffer, sPath);
        strcat(buffer, "PNWl2kAir.ui");
        gtk_builder_add_from_file(pbuilder, buffer, &p_err);  // pb car le binaire est dans un autre repertoire

        if (p_err == NULL) {
            strcpy(buffer, sPath);
            strcat(buffer, "PNWl2kAirT.css");
            file = g_file_new_for_path(buffer);
            css_provider = gtk_css_provider_get_default();
            gtk_css_provider_load_from_file(css_provider, file, &p_err);
            //   context = gtk_style_context_new();
            gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
            //po le label defini une classe
            context = gtk_widget_get_style_context(GTK_LABEL(gtk_builder_get_object(pbuilder, "labStatus")));
            gtk_style_context_add_class(context, "labSt");

            // Recuparation d'un pointeur sur la fenetre.
            GtkWidget * p_win = (GtkWidget *) gtk_builder_get_object(pbuilder, "appWin");
            g_signal_connect(p_win, "destroy", G_CALLBACK (destroy), NULL); // signal destroy

            //  Signal du bouton depart
            g_signal_connect(gtk_builder_get_object(pbuilder, "btnDep"), "clicked", G_CALLBACK (btnDep_clicked_cb), NULL);

            // log win
            GtkWidget * pentry = (GtkWidget *) gtk_builder_get_object(pbuilder, "tvLog");
            //  gtk_widget_set_size_request (pentry, 300, 350);

            if (iFreq == 0)
                sprintf(sStatus, "Ajuste Station et Frequence");
            else
                sprintf(sStatus, "Initialisé %0.1fKHz", (((float) iFreq / 10.0) - 1.5));
            pentry = (GtkWidget *) gtk_builder_get_object(pbuilder, "labStatus");
            gtk_label_set_text(GTK_LABEL(pentry), sStatus);

            // The text buffer represents the text being edited
            tvBuffer = gtk_text_buffer_new(NULL);
            GtkWidget *ptextview = (GtkWidget *) gtk_builder_get_object(pbuilder, "tvLog");
            tvBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ptextview));
            // les tag de couleurs
            tagG = gtk_text_buffer_create_tag(tvBuffer, "green_foreground", "foreground", "green", NULL);
            tagR = gtk_text_buffer_create_tag(tvBuffer, "red_foreground", "foreground", "red", NULL);
            tagB = gtk_text_buffer_create_tag(tvBuffer, "blue_foreground", "foreground", "blue", NULL);
            tagJ = gtk_text_buffer_create_tag(tvBuffer, "yellow_foreground", "foreground", "orange", NULL);

            if (LitConfig() != 0)     // recup des preferances
                printf("erreur lecture config");     // erreur

            if (sTmp[0] != 0) {
                ptextview = (GtkWidget *) gtk_builder_get_object(pbuilder, "eEntree"); //Change texte edit  Station
                gtk_entry_set_text(GTK_ENTRY(ptextview), sTmp);
            }

            pBtn = (GtkWidget *) gtk_builder_get_object(pbuilder, "btnDep");  // en var globale
            init();  // ouvre le port serie

            giTimer = g_timeout_add(300, timeout_callback, NULL);   // le 24/08 passe a 300 etait 200

            gtk_widget_show_all(p_win);
            gtk_main();
        } else {
            // Affichage du message d'erreur de GTK+
            g_error("%s", p_err->message);
            g_error_free(p_err);
        }
    }

    return EXIT_SUCCESS;
}
