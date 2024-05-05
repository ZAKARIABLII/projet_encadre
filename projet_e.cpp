#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure de donn�es pour les auteurs
typedef struct {
    char nom[10];
    char prenom[10];
    char cin[5];
    int age;
    char nationalite[30];
} Auteur;

// Structure de donn�es pour les livres
typedef struct {
    char titre[50];
    size_t cote;
    int annee_de_edition;
    char categorie[20];
    Auteur auteur;
    char disponibilite[4]; 
    char description[100]; 
} Livre;

size_t numLivres = 0; // Nombre de livres dans la biblioth�que

// Prototypes des fonctions
void menu(Livre *tableauLivres);
void ajouterLivre(Livre *tableauLivres);
void supprimerLivre(Livre *tableauLivres);
void afficherBibliotheque(Livre *tableauLivres);
void afficherLivres(Livre *tableauLivres);
void auteurPlusDeLivres(Livre *tableauLivres);
void rechercherLivre(Livre *tableauLivres);
void trierLivres(Livre *tableauLivres);
void quitter(Livre *tableauLivres);
void afficherLivre(Livre livre);
void afficherAuteur(Auteur auteur);
void lireLigne(char *str, size_t tailleStr);
int chercherAuteur(Livre *tableauLivres, char *cin);
void libererMemoire(Livre *tableauLivres);
void sauvegarderLivresCSV(Livre *tableauLivres, const char *fichier);


// Fonction pour afficher un livre donn�
void afficherLivre(Livre livre) {
    printf("******************\n");
    printf("Titre : %s\n", livre.titre);
    printf("Cote : %zu\n", livre.cote);
    printf("Ann�e d'�dition : %d\n", livre.annee_de_edition);
    printf("Cat�gorie : %s\n", livre.categorie);
    printf("Disponibilit� : %s\n", livre.disponibilite);
    printf("Description : %s\n", livre.description);
    afficherAuteur(livre.auteur);
}

// Fonction pour afficher un auteur donn�
void afficherAuteur(Auteur auteur) {
    printf("Auteur : %s %s\n", auteur.nom, auteur.prenom);
    printf("CIN : %s\n", auteur.cin);
    printf("Age : %d\n", auteur.age);
    printf("Nationalit� : %s\n", auteur.nationalite);
}

// Fonction pour lire une ligne de texte
void lireLigne(char *str, size_t tailleStr) {
    fgets(str, tailleStr, stdin);
    // Supprime les caract�res suivant le dernier caract�re non nul
    if (str[0] != '\0' && str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

// Fonction pour v�rifier si un auteur existe d�j�
int chercherAuteur(Livre *tableauLivres, char *cin) {
    for (size_t i = 0; i < numLivres; i++) {
        if (strcmp(tableauLivres[i].auteur.cin, cin) == 0) {
            return 1; // Auteur existe
        }
    }
    return 0; // Auteur n'existe pas
}

// Fonction pour ajouter un livre dans la biblioth�que
void ajouterLivre(Livre *tableauLivres) {
    numLivres++;
    tableauLivres = (Livre *)realloc(tableauLivres, numLivres * sizeof(Livre));
    if (tableauLivres == NULL) {
        printf("Erreur de r�allocation de m�moire\n");
        exit(1);
    }

    Livre *livre = &tableauLivres[numLivres - 1];

    printf("Donner le titre du livre : ");
    lireLigne(livre->titre, sizeof(livre->titre));

    printf("Donner la cat�gorie du livre : ");
    lireLigne(livre->categorie, sizeof(livre->categorie));

    printf("Donner l'ann�e d'�dition du livre : ");
    scanf("%d", &livre->annee_de_edition);
    getchar(); // Nettoyer le tampon d'entr�e

    printf("Donner le CIN de l'auteur : ");
    char tmpCin[10];
    lireLigne(tmpCin, sizeof(tmpCin));

    // V�rifier si l'auteur existe d�j�
    if (chercherAuteur(tableauLivres, tmpCin)) {
        printf("Auteur d�j� existant\n");
        // Trouver l'auteur existant pour copier les informations
        for (size_t i = 0; i < numLivres; i++) {
            if (strcmp(tableauLivres[i].auteur.cin, tmpCin) == 0) {
                livre->auteur = tableauLivres[i].auteur;
                break;
            }
        }
    } else {
        strcpy(livre->auteur.cin, tmpCin);
        printf("Donner le nom de l'auteur : ");
        lireLigne(livre->auteur.nom, sizeof(livre->auteur.nom));

        printf("Donner le pr�nom de l'auteur : ");
        lireLigne(livre->auteur.prenom, sizeof(livre->auteur.prenom));

        printf("Donner l'�ge de l'auteur : ");
        scanf("%d", &livre->auteur.age);
        getchar();

        printf("Donner la nationalit� de l'auteur : ");
        lireLigne(livre->auteur.nationalite, sizeof(livre->auteur.nationalite));
    }

    printf("Le livre est-il disponible (oui ou non) ? ");
    lireLigne(livre->disponibilite, sizeof(livre->disponibilite));

    printf("Donnez une br�ve description du livre : ");
    lireLigne(livre->description, sizeof(livre->description));

    livre->cote = numLivres; // La cote est l'index du livre dans le tableau
    printf("Livre ajout� avec succ�s !\n");
    menu(tableauLivres);
     sauvegarderLivresCSV(tableauLivres, "livres.csv");
}


// Fonction pour supprimer un livre de la biblioth�que
void supprimerLivre(Livre *tableauLivres) {
    printf("Choisissez l'option de suppression :\n");
    printf("a. Par titre\n");
    printf("b. Par auteur (CIN)\n");
    printf("c. Par position\n");

    char choix;
    scanf(" %c", &choix);
    getchar();

    if (choix == 'a') {
        // Supprimer par titre
        printf("Donner le titre du livre � supprimer : ");
        char titre[100];
        lireLigne(titre, sizeof(titre));

        size_t index = SIZE_MAX; // Valeur maximale de size_t pour indiquer non trouv�
        for (size_t i = 0; i < numLivres; i++) {
            if (strcmp(tableauLivres[i].titre, titre) == 0) {
                index = i;
                break;
            }
        }

        if (index != SIZE_MAX) {
            // Supprimer le livre trouv�
            for (size_t i = index; i < numLivres - 1; i++) {
                tableauLivres[i] = tableauLivres[i + 1];
            }
            numLivres--;
            printf("Livre supprim� avec succ�s !\n");
        } else {
            printf("Livre non trouv�.\n");
        }

    } else if (choix == 'b') {
        // Supprimer par CIN de l'auteur
        printf("Donner le CIN de l'auteur : ");
        char cin[5];
        lireLigne(cin, sizeof(cin));

        size_t index = SIZE_MAX; // Valeur maximale de size_t pour indiquer non trouv�
        for (size_t i = 0; i < numLivres; i++) {
            if (strcmp(tableauLivres[i].auteur.cin, cin) == 0) {
                index = i;
                break;
            }
        }

        if (index != SIZE_MAX) {
            // Supprimer le livre trouv�
            for (size_t i = index; i < numLivres - 1; i++) {
                tableauLivres[i] = tableauLivres[i + 1];
            }
            numLivres--;
            printf("Livre supprim� avec succ�s !\n");
        } else {
            printf("Livre non trouv�.\n");
        }

    } else if (choix == 'c') {
        // Supprimer par position
        size_t position;
        printf("Donner la position du livre � supprimer : ");
        scanf("%zu", &position);
        getchar();

        if (position < 1 || position > numLivres) {
            printf("Position invalide.\n");
            return;
        }

        // Supprimer le livre � la position donn�e
        position--; // Passer � l'index z�ro bas� sur la position
        for (size_t i = position; i < numLivres - 1; i++) {
            tableauLivres[i] = tableauLivres[i + 1];
        }
        numLivres--;
        printf("Livre supprim� avec succ�s !\n");
    }

    menu(tableauLivres);
}

// Fonction pour afficher tous les livres de la biblioth�que
void afficherLivres(Livre *tableauLivres) {
    for (size_t i = 0; i < numLivres; i++) {
        afficherLivre(tableauLivres[i]);
    }
    printf("Fin de l'affichage des livres.\n");
}

// Fonction pour afficher l'auteur qui a publi� le plus de livres
void auteurPlusDeLivres(Livre *tableauLivres) {
    // Compter les livres par auteur
    size_t compteurLivres[100] = {0};
    Auteur auteurs[100];
    size_t numAuteurs = 0;

    for (size_t i = 0; i < numLivres; i++) {
        int trouve = 0;
        for (size_t j = 0; j < numAuteurs; j++) {
            if (strcmp(auteurs[j].cin, tableauLivres[i].auteur.cin) == 0) {
                compteurLivres[j]++;
                trouve = 1;
                break;
            }
        }
        if (!trouve) {
            auteurs[numAuteurs] = tableauLivres[i].auteur;
            compteurLivres[numAuteurs]++;
            numAuteurs++;
        }
    }

    // Trouver l'auteur avec le plus de livres
    size_t maxLivres = 0;
    size_t indexMax = 0;
    for (size_t i = 0; i < numAuteurs; i++) {
        if (compteurLivres[i] > maxLivres) {
            maxLivres = compteurLivres[i];
            indexMax = i;
        }
    }

    // Afficher l'auteur avec le plus de livres
    printf("L'auteur ayant publi� le plus de livres est :\n");
    afficherAuteur(auteurs[indexMax]);
    printf("Nombre de livres publi�s : %zu\n", maxLivres);
    menu(tableauLivres);
}

// Fonction pour rechercher un livre par titre ou auteur
void rechercherLivre(Livre *tableauLivres) {
    printf("Choisissez l'option de recherche :\n");
    printf("a. Par titre\n");
    printf("b. Par auteur (CIN)\n");

    char choix;
    scanf(" %c", &choix);
    getchar();

    if (choix == 'a') {
        // Rechercher par titre
        printf("Donner le titre du livre � rechercher : ");
        char titre[100];
        lireLigne(titre, sizeof(titre));

        for (size_t i = 0; i < numLivres; i++) {
            if (strcmp(tableauLivres[i].titre, titre) == 0) {
                afficherLivre(tableauLivres[i]);
                return;
            }
        }
        printf("Livre non trouv�.\n");

    } else if (choix == 'b') {
        // Rechercher par CIN de l'auteur
        printf("Donner le CIN de l'auteur : ");
        char cin[5];
        lireLigne(cin, sizeof(cin));

        for (size_t i = 0; i < numLivres; i++) {
            if (strcmp(tableauLivres[i].auteur.cin, cin) == 0) {
                afficherLivre(tableauLivres[i]);
                return;
            }
        }
        printf("Livre non trouv�.\n");
    }

    menu(tableauLivres);
}

// Fonction pour trier les livres par titre
void trierLivres(Livre *tableauLivres) {
    // Demander � l'utilisateur le crit�re de tri
    printf("Choisissez le crit�re de tri :\n");
    printf("a. Par titre\n");
    printf("b. Par cat�gorie\n");

    char choix;
    scanf(" %c", &choix);
    getchar(); // Nettoyer le tampon d'entr�e

    // Trier les livres en fonction du choix de l'utilisateur
    if (choix == 'a') {
        // Tri par titre
        for (size_t i = 0; i < numLivres - 1; i++) {
            for (size_t j = i + 1; j < numLivres; j++) {
                if (strcmp(tableauLivres[i].titre, tableauLivres[j].titre) > 0) {
                    // �changer les livres
                    Livre temp = tableauLivres[i];
                    tableauLivres[i] = tableauLivres[j];
                    tableauLivres[j] = temp;
                }
            }
        }
        printf("Livres tri�s par titre.\n");
    } else if (choix == 'b') {
        // Tri par cat�gorie
        for (size_t i = 0; i < numLivres - 1; i++) {
            for (size_t j = i + 1; j < numLivres; j++) {
                if (strcmp(tableauLivres[i].categorie, tableauLivres[j].categorie) > 0) {
                    // �changer les livres
                    Livre temp = tableauLivres[i];
                    tableauLivres[i] = tableauLivres[j];
                    tableauLivres[j] = temp;
                }
            }
        }
        printf("Livres tri�s par cat�gorie.\n");
    } else {
        printf("Crit�re de tri invalide.\n");
    }

    menu(tableauLivres);
}


// Fonction pour quitter le programme
void quitter(Livre *tableauLivres) {
    libererMemoire(tableauLivres);
    printf("Programme termin�.\n");
    exit(0);
}

// Fonction pour lib�rer la m�moire allou�e aux livres
void libererMemoire(Livre *tableauLivres) {
    free(tableauLivres);
}

// Menu principal
void menu(Livre *tableauLivres) {
    while (1) {
        printf("Menu principal :\n");
        printf("1. Ajouter un livre\n");
        printf("2. Supprimer un livre\n");
        printf("3. Afficher les livres\n");
        printf("4. Afficher l'auteur ayant publi� le plus de livres\n");
        printf("5. Rechercher un livre\n");
        printf("6. Trier les livres\n");
        printf("7. Quitter\n");
        printf("8. Sauvgarder les livres dans un fichier csv\n");
        
        printf("Entrez votre choix : ");

        int choix;
        scanf("%d", &choix);
        getchar(); // Nettoyer le tampon d'entr�e

        switch (choix) {
            case 1:
                ajouterLivre(tableauLivres);
                break;
            case 2:
                supprimerLivre(tableauLivres);
                break;
            case 3:
                afficherLivres(tableauLivres);
                break;
            case 4:
                auteurPlusDeLivres(tableauLivres);
                break;
            case 5:
                rechercherLivre(tableauLivres);
                break;
            case 6:
                trierLivres(tableauLivres);
                break;
            case 7:
                quitter(tableauLivres);
                break;
                case 8:
                	sauvegarderLivresCSV(tableauLivres, "livres.csv");
                break;
            default:
                printf("Choix invalide. Veuillez r�essayer.\n");
        }
    }
}

void sauvegarderLivresCSV(Livre *tableauLivres, const char *fichier) {
    FILE *fp = fopen(fichier, "w");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", fichier);
        return;
    }

    // �crire les en-t�tes CSV
    fprintf(fp, "Titre\tCote\tAnnee de Edition\tCategorie\tAuteur Nom\tAuteur Prenom\tAuteur CIN\tNationalite\tAge\tDisponibilite\tDescription\n");

    // �crire les donn�es de chaque livre dans le fichier CSV
    for (size_t i = 0; i < numLivres; i++) {
        Livre livre = tableauLivres[i];
        fprintf(fp,"%s\t%zu\t%d\t%s\t%s\t%s\t%s\t%s\t%d\t%s\t%s\n",livre.titre,livre.cote,livre.annee_de_edition,livre.categorie,livre.auteur.nom,livre.auteur.prenom,livre.auteur.cin,livre.auteur.nationalite,livre.auteur.age,livre.disponibilite,livre.description);
    }
    // Fermer le fichier apr�s l'�criture
    fclose(fp);
    printf("Les livres ont �t� sauvegard�s dans le fichier %s.\n", fichier);
}


int main() {
    Livre *tableauLivres = NULL; // Tableau dynamique de livres
    menu(tableauLivres);
    libererMemoire(tableauLivres);
    return 0;
}

