#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

typedef struct perso
{
    int imgcourante;
    float x, y;          // coordonnée (du coin sup. gauche)
    float precx, precy;  // coordonnées précédent le déplacement
    float dx, dy;        // vecteur deplacement
    int vivant; // 0 mort, 1 vivant
    float angle;
    int victoire;
    float max_x;
    int color;
    BITMAP* hitbox;
    int tx,ty;
    double angle_collision;
    int timer_col;
    int aile;
} t_perso;

typedef struct listePerso
{
    int nbperso;
    t_perso** perso;
}t_listePerso;

void actualiserPerso(t_perso *perso,int test,int aile,int contact,float v_scroll);
int actualiserListePerso(t_listePerso *lp,int* compt,float v_scroll);
t_listePerso* creer_liste_perso();
t_perso * creerPerso(int x,int y,int couleurblindee);
t_perso * ajouterPerso(t_listePerso *lp,int x,int y);
int contactPerso(t_perso*perso,int*contactGauche,int* contactDroit,int* contactHaut,int*contactBas);
void tuer_Perso(t_listePerso *lp);
int obtention_couleurblindee(t_listePerso *lp);
int testperdu(t_listePerso *lp);

#endif // PERSO_H_INCLUDED
