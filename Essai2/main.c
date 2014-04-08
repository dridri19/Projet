#include "projet.h"

/* INDISPENSABLE:

easy:
faire toutes les images:crédits,regles,différents bonus,les niveaux,les images de fond,la victoire,menu de choix du niveau

moyen:
plusieurs niveaux
différents menus

dur:
collision
3 types de bonus (aimantés?)
blindage du spawn pour les clones pour pas qu'ils spawn sur un mur/perso

*/

/*Ensuite:

sauvegarde de progression dans fichier
sons (bruitages et musique de fond)=> flapflap,1 musique d'ambiance par niveau, bruit de mort, bruit de floup d'attraction du bonus

*/

/*autres idées qu'on aura surement pas le temps de faire:
checkpointsq
timer
éléments destructibles par poids du perso (gros perso ou plein de persos font casser un tuyau qui bloquait le passage en sautant dessus)
ventilateurs
interrupteurs
téléporteurs
systeme d'étoiles
éléments dynamiques (scies,roues,pistons,pierres qui tombent)
*/

//// DERNIERE VERSION EN DATE soir du 08/04 à 20h48

typedef struct graphmenu
{
    BITMAP *img_menu, *cursor, *img_boutons[NIMGMENU], *img_perso[NIMGPERSO], *img_son[NIMGSON];
    SAMPLE *music, *flap;
    int boutons_x[NIMGMENU/2];
    int boutons_y[NIMGMENU/2];
} t_graphmenu;

void affMenu(t_graphmenu gm);
void chargerContenuMenu(t_graphmenu* pgm);
int menuChoixNiveau();
void gestionniveau(int choixniveau);
int jouerNiv(int niv);
void actualiser_bitmap_collision(float*mapx,t_listePerso* lp);
void affichage(float*mapx,t_listePerso* lp);
void scroll(float * mapx,float* v_scroll);
int victoire (float * mapx,float* v_scroll,t_listePerso *lp);
void chargementBitmapGlobales();
int score(t_listePerso *lp);

BITMAP *page;
BITMAP* collision;
BITMAP* niveau;
BITMAP* oiseau[NIMAGE];
BITMAP* fond;

void chargementBitmapGlobales()
{
    collision=create_bitmap(800,600);
    fond= chargerImage("files/images/fond.bmp");
    niveau= chargerImage("files/images/map.bmp");
    charger_images(oiseau);
}


#define ZBOUB=502

void affMenu(t_graphmenu gm)
{
    int mx, my, i;

    int niveauchoisi;

    int decalPerso[10];
    decalPerso[0] = -6;
    decalPerso[1] = decalPerso[9] = -4;
    decalPerso[2] = decalPerso[8] = -2;
    decalPerso[3] = decalPerso[7] = 0;
    decalPerso[4] = decalPerso[6] = 2;
    decalPerso[5] = 4;

    int click=0, compteur=0, compteur2=0, soundOn=1;

    while (!key[KEY_ESC])
    {
        mx=mouse_x;
        my=mouse_y;
        draw_sprite(page, gm.img_menu, 0,0);
        //AFFICHAGE
        for (i=0; i<NIMGMENU/2; i++)
        {
            if (mx>=gm.boutons_x[i] && mx<=(gm.boutons_x[i]+gm.img_boutons[i]->w) && my>=gm.boutons_y[i] && my<=(gm.boutons_y[i]+gm.img_boutons[i]->h))
                draw_sprite(page, gm.img_boutons[i+NIMGMENU/2], gm.boutons_x[i], gm.boutons_y[i]);
            else
                draw_sprite(page, gm.img_boutons[i], gm.boutons_x[i], gm.boutons_y[i]);
        }
        if (mx>=640 && mx<=640+gm.img_son[0]->w && my>=150 && my<=150+gm.img_son[0]->h)
        {
            if (soundOn)
                draw_sprite(page, gm.img_son[2], 640, 150);
            else
                draw_sprite(page, gm.img_son[3], 640, 150);
        }
        else
        {
            if (soundOn)
                draw_sprite(page, gm.img_son[0], 640, 150);
            else
                draw_sprite(page, gm.img_son[1], 640, 150);
        }
        draw_sprite(page, gm.img_perso[compteur], SCREEN_W/2-gm.img_perso[0]->w/2, 280+decalPerso[compteur]);
        draw_sprite(page, gm.cursor, mx, my);
        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);

        //CLICK
        if (mouse_b==1 && click==0)
        {
            if (mx>=gm.boutons_x[0] && mx<=(gm.boutons_x[0]+gm.img_boutons[0]->w) && my>=gm.boutons_y[0] && my<=(gm.boutons_y[0]+gm.img_boutons[0]->h))
            {
                //JOUER
                printf("jouer\n");
                /// on lance le ssprog jouerpartie
                niveauchoisi=menuChoixNiveau();
                gestionniveau(niveauchoisi);
            }
            if (mx>=gm.boutons_x[1] && mx<=(gm.boutons_x[1]+gm.img_boutons[1]->w) && my>=gm.boutons_y[1] && my<=(gm.boutons_y[1]+gm.img_boutons[1]->h))
            {
                //QUITTER
                break;
            }
            if (mx>=gm.boutons_x[2] && mx<=(gm.boutons_x[2]+gm.img_boutons[2]->w) && my>=gm.boutons_y[2] && my<=(gm.boutons_y[2]+gm.img_boutons[2]->h))
            {
                //REGLES
                printf("regles\n");
            }
            if (mx>=gm.boutons_x[3] && mx<=(gm.boutons_x[3]+gm.img_boutons[3]->w) && my>=gm.boutons_y[3] && my<=(gm.boutons_y[3]+gm.img_boutons[3]->h))
            {
                //CREDITS
                printf("credits\n");
            }
            if (mx>=640 && mx<=640+gm.img_son[0]->w && my>=150 && my<=150+gm.img_son[0]->h)
            {
                if (soundOn)
                {
                    soundOn=0;
                    stop_sample(gm.music);
                }
                else
                {
                    soundOn=1;
                    play_sample(gm.music, 255, 127, 1000, 1);
                }
            }
            click=1;
        }
        if (mouse_b==0 && click==1)
        {
            click = 0;
        }
        compteur2++;
        if (compteur2%6==0)
        {
            compteur++;
            if (compteur==2)
                play_sample(gm.flap, 30, 127, 1000, 0);
        }

        if (compteur==10)
        {
            compteur=0;
        }
        rest(10);
    }
}

void chargerContenuMenu(t_graphmenu* pgm)
{
    int i;
    char filename[100];
    FILE *boutons_xy;
    boutons_xy = fopen("files/menu/boutons_xy.txt", "r");
    pgm->music = load_sample("files/menu/music.wav");
    pgm->flap = load_sample("files/menu/flap.wav");
    if (boutons_xy==NULL)
    {
        allegro_message("Erreur chargement du fichier \"boutons_xy.txt\"");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    rewind(boutons_xy);
    for(i=0; i<NIMGMENU/2; i++)
    {
        fscanf(boutons_xy, "%d", &pgm->boutons_x[i]);
        fscanf(boutons_xy, "%d", &pgm->boutons_y[i]);
    }
    pgm->img_menu = chargerImage("files/menu/menu.bmp");
    pgm->cursor = chargerImage("files/menu/cursor.bmp");
    for (i=0; i<NIMGMENU; i++)
    {
        sprintf(filename, "files/menu/bouton%d.bmp", i);
        pgm->img_boutons[i] = chargerImage(filename);
    }
    for (i=0; i<NIMGSON; i++)
    {
        sprintf(filename, "files/menu/son%d.bmp", i);
        pgm->img_son[i] = chargerImage(filename);
    }
    for (i=0; i<6; i++)
    {
        sprintf(filename, "files/menu/bird%d.bmp", i);
        pgm->img_perso[i] = chargerImage(filename);
    }

    for (i=0; i<4; i++)
    {
        pgm->img_perso[i+6] = pgm->img_perso[4-i];
    }
}

int menuChoixNiveau()
{
    int niveau;
    niveau=0;
    return niveau;
}

void gestionniveau(int choixniveau)
{
    int fini,niv;
    niv=choixniveau;
    while(!key[KEY_ESC] && fini!=2 && niv!=3) /// car 3 le nombre de niveaux existants
    {
        fini=jouerNiv(niv);
        if(fini==1) niv++;
        if (niv==3) printf("merci bien joué");
    }
}

int jouerNiv(int niv)
{
    t_listePerso* liste;
    liste=creer_liste_perso();
    float mapx=0;
    float v_scroll=2;
    int test_appui=0;
    int compt=0;
    int fini=-1;

    ajouterPerso(liste,50,SCREEN_H/2);

    while (!key[KEY_ESC])
    {
        compt++;
        if (key[KEY_ENTER] && test_appui==0)
        {
            ajouterPerso(liste,0,SCREEN_H/2);
            test_appui=1;
        }
        if (!key[KEY_ENTER] && test_appui==1)
        {
            test_appui =0;
        }
        fini=victoire(&mapx,&v_scroll,liste);
        if (fini!=-1)
        {
            return fini;
        }
        score(liste);
        scroll(&mapx,&v_scroll);
        fini=actualiserListePerso(liste,&compt,v_scroll);
        if (fini!=-1)
        {
            return fini;
        }
        tuer_Perso(liste);
        affichage(&mapx,liste);
        actualiser_bitmap_collision(&mapx,liste);
        rest(8);

    }
    return fini;
}

void actualiser_bitmap_collision(float*mapx,t_listePerso* lp)
{
    int i;
    clear_to_color(collision,makecol(255,0,255));
    masked_blit(niveau,collision,*mapx,0,0,0,SCREEN_W,SCREEN_H);

    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            rotate_sprite(collision,lp->perso[i]->hitbox,lp->perso[i]->x,lp->perso[i]->y,ftofix(lp->perso[i]->angle));
        }
    }
}

void affichage(float*mapx,t_listePerso* lp)
{
    int i;
    blit(fond,page,*mapx/10,0,0,0,SCREEN_W,SCREEN_H);
    masked_blit(niveau,page,*mapx,0,0,0,SCREEN_W,SCREEN_H);

    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            rotate_sprite(page,oiseau[lp->perso[i]->imgcourante],lp->perso[i]->x,lp->perso[i]->y,ftofix(lp->perso[i]->angle));
        }
    }

    // affichage sur le buffer
    blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
}

void scroll(float * mapx,float* v_scroll)
{
    (*mapx)= (*mapx)+(*v_scroll);
}

int victoire (float * mapx,float* v_scroll,t_listePerso *lp)
{
    int i=0;
    int j=0;
    int pos=SCREEN_W;
    if (*mapx >= niveau->w-SCREEN_W)
    {
        *v_scroll=0;
        for (i=0; i<MAXI; i++)
        {
            if (lp->perso[i]!=NULL)
            {
                lp->perso[i]->victoire=1;
            }
        }
        for (i=0; i<MAXI; i++)
        {
            if (lp->perso[i]!=NULL)
            {
                if (lp->perso[i]->victoire==1)
                {
                    j++;
                }
            }
        }
        if (j == lp->nbperso)
        {
            for (i=0; i<MAXI; i++)
            {
                if (lp->perso[i]!=NULL)
                {
                    if (lp->perso[i]->victoire==1)
                    {
                        pos = min (pos,lp->perso[i]->x);
                        if (pos==SCREEN_W)
                        {
                            printf("victoire\n");
                            return 0 ; // mettre a 1 quand il y aura pls niveau
                        }

                    }
                }
            }
        }
    }
    return -1;
}

int score(t_listePerso *lp)
{
    int i=0;
    int score=0;
    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            if (lp->perso[i]->victoire==1) score++;
        }
    }
    return score;
}

int main()
{
    lancerToutAllegro(800,600);

    srand(time(NULL));

    chargementBitmapGlobales();

    t_perso* perso[MAXI];
    t_graphmenu gm;

    chargerContenuMenu(&gm);

    play_sample(gm.music, 255, 127, 1000, 1);
    affMenu(gm);

    return 0;
}
END_OF_MAIN();
