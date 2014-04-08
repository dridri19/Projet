#include "projet.h"

void tuer_Perso(t_listePerso *lp)
{
    int i;
    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL) // on étudie tous les persos vivants
        {
            if (lp->perso[i]->x < -lp->perso[i]->hitbox->w) // si un perso mérite de mourir
            {
                destroy_bitmap(lp->perso[i]->hitbox);
                free(lp->perso[i]);
                lp->perso[i]=NULL;
                lp->nbperso--;
                printf("le perso est mort\n");
            }
        }
    }
}

int testperdu(t_listePerso *lp)
{
    int perdu=0;
    if (lp->nbperso == 0) /// SI IL N Y A PLUS DE PERSOS
    {
        perdu=1;
    }
    return perdu;
}

int obtention_couleurblindee(t_listePerso *lp)// ni noire ni magenta ni d'une autre couleur d'une hitbox déjà existante
{
    int couleur,probleme;
    int i;
    do
    {
        probleme=0;
        couleur=makecol(1+rand()%255,1+rand()%255,1+rand()%255);
        for (i=0; i<MAXI; i++)
        {
            if (lp->perso[i]!=NULL)
            {
                if (couleur == lp->perso[i]->color) // si la couleur qu'on crée existe déjà
                {
                    probleme=1;
                }
            }
        }
    }
    while(probleme==1);

    return couleur;
}

int contactPerso(t_perso*perso,int*contactGauche,int* contactDroit,int* contactHaut,int*contactBas)
{
    int contact=0;
    int i,j;
    double anglegrad;
    int magenta = makecol(255,0,255);
    int xc,yc;
    //xc=perso->hitbox->w/2;
    //yc=perso->hitbox->h/2;
    xc=26;
    yc=43;

    perso->angle_collision=0;
    if (perso->x>=0)
    {
        for(i=0; i<perso->hitbox->w; i++) // on parcourt tous les pixels de la hitbox
        {
            for(j=0; j<perso->hitbox->h; j++)
            {
                if (getpixel(perso->hitbox,i,j)== perso->color) // si on tombe sur un pixel de la vraie hitbox (pas du magenta)
                {
                    if ((getpixel(collision,perso->x+perso->dx+i,perso->y+perso->dy+j) != magenta) && (getpixel(collision,perso->x+perso->dx+i,perso->y+perso->dy+j) != perso->color)) // si à l'endroit où on voudrait afficher le perso il y a déjà quelquechose
                    {
                        if ((i+perso->x+perso->dx >0) && (i+perso->x+perso->dx <800))
                        {
                            contact = 1;
                            anglegrad= acos( (i-xc) / sqrt(   (i-xc)*(i-xc)+(j-yc)*(j-yc)   )  );
                            perso->angle_collision=anglegrad * 180.0 / 3.14;

                            if (perso->dy>0) perso->angle_collision=-perso->angle_collision; // fonctionne apparemment cette fois ci (c'est si dy>0 qu'on modifie!)
                            /// en fonction de cet angle, mettre les booléens contactGauche,contactDroite,contactHaut,contactBas
                            /// (reçus en passage par adresse) à 0 ou 1.
                            //printf("%lf\n",perso->angle_collision);
                            /*if ((perso->angle_collision>=-45) && (perso->angle_collision<=45))
                            {
                                *contactDroit=1;
                                printf("contact droite\n");
                            }
                            if ((perso->angle_collision>=45) && (perso->angle_collision<=135))
                            {
                                *contactHaut=1;
                                printf("contact haut\n");
                            }
                            if ((perso->angle_collision<=-45) && (perso->angle_collision>=-135))
                            {
                                *contactBas=1;
                                printf("contact bas\n");
                            }
                            if (((perso->angle_collision>=135) && (perso->angle_collision<=180)) || ((perso->angle_collision<=-135)&&(perso->angle_collision>=-180)))
                            {
                                *contactGauche=1;
                                printf("contact gauche\n");
                            }*/
                            line (screen,perso->x+xc,perso->y+yc,perso->x+xc+cos(anglegrad)*100,perso->y+yc-sin(anglegrad)*100,makecol(125,125,125));
                            // on peut faire une optimisation de malade en arretant dès que contact ==1
                        }
                    }
                }
            }
        }

    }
    else
    {
        perso->dx=-2;
    }
    /// EN PROGRAMMATION LES REPERES COMMENCENT EN HAUT A GAUCHE, VERIFIER QUE POUR NOS CALCULS ON A PRIS CA EN COMPTE
    /// ATTENTION AUX COORDONNES DU CENTRE DE LA BITMAP QUI NE SONT PAS LES COORDONNEES DU CENTRE DE LOISEAU !!!!
    return contact;
}

void actualiserPerso(t_perso *perso,int test,int aile,int contact,float v_scroll)
{
    float r;

    if (aile==1)
    {
        if (perso->aile==0)
        {
            perso->imgcourante++;
            if (perso->imgcourante>=NIMAGE)
                perso->aile=1;
        }
        if (perso->aile==1)
        {
            perso->imgcourante--;
            if (perso->imgcourante<=0)
                perso->aile=0;
        }

    }

    if (contact==0)
    {
        if (test==1) //touche espace enfoncée
        {
            //caractère aléatoire du saut (compris entre -1 et 1) ((RAND_MAX/2)=2)
            r = (float)rand() / (float)(RAND_MAX/2)-1;

            //saut + caractere aleatoire
            perso->dy= -2.2+r;
            perso->angle= perso->angle - 4;
        }


        if (perso->timer_col==0)
        {
            if (perso->victoire==1)
            {
                perso->dx=2;
            }
            else  if (perso->x < perso->max_x)
            {
                perso->dx=perso->dx+0.05;
                if (perso->dx>0.5)
                    perso->dx=0.5;
            }
            else if (perso->x == perso->max_x)
            {
                perso->dx = 0;
            }
            else if (perso->x > perso->max_x)
            {
                perso->dx = -0.2;
            }
            r = (float)rand() / (float)(RAND_MAX)-0.5;
            //perso->dx=perso->dx+r;

        }



        perso->x=perso->x+perso->dx;
        perso->y=perso->y+perso->dy;
        perso->dy=perso->dy + 0.07;
        perso->timer_col--;
        if (perso->timer_col<0)
            perso->timer_col=0;

    }

    if (contact==1)
    {
        perso->timer_col=20;

        if ((perso->angle_collision>=-180) && (perso->angle_collision<=-60))
        {
            perso->x=perso->x+3;
            if (test==1) //touche espace enfoncée
            {
                perso->dy= -5;
                perso->angle= perso->angle - 4;
            }
            perso->dy=(-perso->dy)/3;
            perso->y=perso->y-2;
            printf("4\n");
        }

        if (((perso->angle_collision)<0) && ((perso->angle_collision)> -70))
        {
            perso->x=perso->x-v_scroll-3;
            perso->y=perso->y-2;
            printf("2\n");
        }

        if (((perso->angle_collision)<70) && ((perso->angle_collision)>= 0))
        {
            perso->x=perso->x-v_scroll-3;
            perso->dy=perso->dy + 0.07;
            printf("1\n");
        }

    }
    if ((perso->angle_collision>=35) && (perso->angle_collision<=125))
    {
        perso->y=perso->y+4;
        perso->dy=(-perso->dy)/3;
        perso->dy=perso->dy + 0.07;
        printf("3\n");
    }

    perso->angle=perso->angle+0.4;
    if (perso->angle>=10)
        perso->angle=10;


    if (perso->angle<=-15)
        perso->angle=-15;
}

int actualiserListePerso(t_listePerso *lp,int* compt,float v_scroll)
{
    int i;
    int test=0;
    int aile=0;
    int contact=0;
    int contactGauche=0;
    int contactDroit=0;
    int contactHaut=0;
    int contactBas=0;
    int perdu=0;
    // actualiser chaque acteur
    // si un acteur n'est plus vivant, l'enlever de la liste

    perdu=testperdu(lp);
    if (perdu==1)
    {
        printf("perdu\n");
        return 0;
    }

    if (key[KEY_SPACE])
    {
        test=1;
        *compt=*compt+5;
    }

    if (*compt>10)
    {
        aile=1;
        *compt=0;
    }

    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            contact=contactPerso(lp->perso[i],&contactGauche,&contactDroit,&contactHaut,&contactBas);
            actualiserPerso(lp->perso[i],test,aile,contact,v_scroll);// on fait le déplacement
        }
    }
    return -1;
}

t_listePerso* creer_liste_perso()
{
    int i;
    t_listePerso* nouv;

    nouv=(t_listePerso *)malloc(1*sizeof(t_listePerso));

    nouv=(t_listePerso *)malloc(1*sizeof(t_listePerso));

    nouv->nbperso=0;
    nouv->perso=(t_perso **)malloc(MAXI*sizeof(t_perso*));

    for (i=0; i<MAXI; i++)
        nouv->perso[i]=NULL;

    return nouv;
}

t_perso * creerPerso(int x,int y,int couleurblindee)
{
    t_perso *nouv;
    int couleur,a,b;

    // Allouer
    nouv=(t_perso *)malloc(1*sizeof(t_perso));

    // Initialiser ...

    // ici ce qui est commun aux acteurs

    float r = (float)rand() / (float)(RAND_MAX/200);

    nouv->x=x;
    nouv->y=y;
    nouv->dx=0;
    nouv->dy=0;
    nouv->imgcourante=0;
    nouv->angle=-10;
    nouv->vivant=1;
    nouv->victoire=0;
    nouv->max_x=SCREEN_W/3+r;   //Position max d'un oiseau
    nouv->color=couleurblindee;
    nouv->hitbox= create_bitmap(oiseau[1]->w,oiseau[1]->h);
    nouv->tx=oiseau[1]->w;
    nouv->ty=oiseau[1]->h;
    nouv->angle=0;
    nouv->timer_col=0;
    nouv->aile=0;

    // création d'une bitmap hitbox à partir de l'image de l'ennemi
    for (a=0; a<oiseau[1]->w; a++)
    {
        for (b=0; b<oiseau[1]->h; b++)
        {
            couleur=getpixel(oiseau[1], a, b);
            if (couleur==makecol(255,0,255))
                putpixel(nouv->hitbox, a, b, makecol(255,0,255));
            else
                putpixel(nouv->hitbox, a, b, nouv->color);
        }
    }

    return nouv;
}

t_perso * ajouterPerso(t_listePerso *lp,int x,int y)
{
    int i;
    t_perso *perso;
    int couleur;

    // Liste pleine, on alloue rien et on retourne NULL...
    if (lp->nbperso >= MAXI)
        return NULL;

    // Allouer un acteur initialisé
    couleur=obtention_couleurblindee(lp);

    perso=creerPerso(x,y,couleur);

    // Chercher un emplacement libre
    i=0;
    while (lp->perso[i]!=NULL && i<MAXI)
        i++;

    // Si on a trouvé ...
    // (normalement oui car on a vérifié n<max)
    if (i<MAXI)
    {
        // Accrocher le acteur à l'emplacement trouvé
        lp->perso[i]=perso;
        lp->nbperso++;
    }
    // Sinon c'est qu'il y a un problème de cohérence
    else
        allegro_message("Anomalie gestion ajouterActeur : liste corrompue");

    return perso;
}
