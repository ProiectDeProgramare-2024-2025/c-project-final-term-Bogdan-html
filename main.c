#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
    #define COLOR_RED     "\x1b[31m"
    #define COLOR_GREEN   "\x1b[32m"
    #define COLOR_YELLOW  "\x1b[33m"
    #define COLOR_BLUE    "\x1b[34m"
    #define COLOR_MAGENTA "\x1b[35m"
    #define COLOR_CYAN    "\x1b[36m"
    #define COLOR_RESET   "\x1b[0m"
#else
    #define CLEAR_SCREEN "clear"
    #define COLOR_RED     "\033[31m"
    #define COLOR_GREEN   "\033[32m"
    #define COLOR_YELLOW  "\033[33m"
    #define COLOR_BLUE    "\033[34m"
    #define COLOR_MAGENTA "\033[35m"
    #define COLOR_CYAN    "\033[36m"
    #define COLOR_RESET   "\033[0m"
#endif

#define MAX_JOCURI 100
#define MAX_UTILIZATORI 50
#define MAX_LUNGIME 100
#define MAX_JOCURI_UTILIZATOR 20

typedef struct {
    char titlu[MAX_LUNGIME];
    int dimensiune;
    int numarDescarcari;
} Joc;

typedef struct {
    char numeUtilizator[MAX_LUNGIME];
    char parola[MAX_LUNGIME];
    int spatiuStocare;
    char jocuriDescarcate[MAX_JOCURI_UTILIZATOR][MAX_LUNGIME];
    int numarJocuriDescarcate;
    int blocat;
} Utilizator;

Joc jocuri[MAX_JOCURI];
int numarJocuri = 0;

void adminMenu();
void userMenu(Utilizator utilizator);
void adaugaJocMenu();
void stergeJocMenu();
void actualizeazaJocMenu();
void vizualizeazaStatisticiMenu();
void monitorizareUtilizatoriMenu();
void blocheazaUtilizatoriMenu();
void vizualizareJocuriMenu(Utilizator utilizator);
void descarcaJocMenu(Utilizator utilizator);
void stergeJocInstalatMenu(Utilizator utilizator);
void inregistrareUtilizator();
void logareUtilizator(Utilizator *utilizator);

void incarcaDate();
void salveazaDate();
Utilizator incarcaUtilizator(char numeUtilizator[]);
void salveazaUtilizator(Utilizator utilizator);
int findGameIndex(const char* title);

int main() {
    incarcaDate();

    int choice;
    while (1) {
        system(CLEAR_SCREEN);
        printf(COLOR_CYAN "=== GAME MANAGER ===\n" COLOR_RESET);
        printf("1. Administrator\n");
        printf("2. Utilizator\n");
        printf("3. Iesire\n");
        printf("Alege o optiune: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                int incercari = 3;
                char parola[MAX_LUNGIME];
                while (incercari > 0) {
                    system(CLEAR_SCREEN);
                    printf(COLOR_YELLOW "Parola administrator: " COLOR_RESET);
                    scanf("%s", parola);
                    getchar();
                    if (strcmp(parola, "000") == 0) {
                        adminMenu();
                        break;
                    } else {
                        incercari--;
                        printf(COLOR_RED "Parola incorecta. Mai aveti %d incercari.\n" COLOR_RESET, incercari);
                        getchar();
                    }
                }
                if (incercari == 0) {
                    printf(COLOR_RED "Prea multe incercari esuate. Iesire.\n" COLOR_RESET);
                    return 0;
                }
                break;
            }
            case 2: {
                int actiune;
                system(CLEAR_SCREEN);
                printf("1. Logare\n");
                printf("2. Inregistrare\n");
                printf("Alege o optiune: ");
                scanf("%d", &actiune);
                getchar();

                if (actiune == 1) {
                    Utilizator utilizator;
                    logareUtilizator(&utilizator);
                    if (strcmp(utilizator.numeUtilizator, "") != 0)
                        userMenu(utilizator);
                } else if (actiune == 2) {
                    inregistrareUtilizator();
                } else {
                    printf(COLOR_RED "Optiune invalida.\n" COLOR_RESET);
                    getchar();
                }
                break;
            }
            case 3:
                salveazaDate();
                exit(0);
            default:
                printf(COLOR_RED "Optiune invalida!\nApasa enter pentru a continua..." COLOR_RESET);
                getchar();
        }
    }
    return 0;
}

void adminMenu() {
    int choice;
    while (1) {
        system(CLEAR_SCREEN);
        printf(COLOR_CYAN "=== ADMIN MENU ===\n" COLOR_RESET);
        printf("1. Adauga joc\n");
        printf("2. Sterge joc\n");
        printf("3. Actualizeaza joc\n");
        printf("4. Vizualizare statistici descarcari\n");
        printf("5. Monitorizare utilizatori\n");
        printf("6. Blocare utilizatori\n");
        printf("7. Inapoi\n");
        printf("Alege o optiune: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: adaugaJocMenu(); break;
            case 2: stergeJocMenu(); break;
            case 3: actualizeazaJocMenu(); break;
            case 4: vizualizeazaStatisticiMenu(); break;
            case 5: monitorizareUtilizatoriMenu(); break;
            case 6: blocheazaUtilizatoriMenu(); break;
            case 7: return;
            default: printf(COLOR_RED "Optiune invalida!\nApasa enter pentru a continua..." COLOR_RESET); getchar();
        }
    }
}

void userMenu(Utilizator utilizator) {
    int choice;
    while (1) {
        system(CLEAR_SCREEN);
        printf(COLOR_CYAN "=== USER MENU ===\n" COLOR_RESET);
        printf(COLOR_GREEN "Utilizator: %s\n" COLOR_RESET, utilizator.numeUtilizator);
        printf("Spatiu disponibil: %d GB\n", utilizator.spatiuStocare);
        printf("1. Vizualizare jocuri\n");
        printf("2. Descarca joc\n");
        printf("3. Sterge joc instalat\n");
        printf("4. Inapoi\n");
        printf("Alege o optiune: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: vizualizareJocuriMenu(utilizator); break;
            case 2: descarcaJocMenu(utilizator); break;
            case 3: stergeJocInstalatMenu(utilizator); break;
            case 4: return;
            default: printf(COLOR_RED "Optiune invalida!\nApasa enter pentru a continua..." COLOR_RESET); getchar();
        }
    }
}

void adaugaJocMenu() {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== ADAUGARE JOC ===\n" COLOR_RESET);
    Joc jocNou;
    printf("Titlu: ");
    scanf(" %[^\n]s", jocNou.titlu);
    printf("Dimensiune (GB): ");
    if (scanf("%d", &jocNou.dimensiune) != 1) {
        printf(COLOR_RED "Dimensiune invalida. Introduceti un numar.\n" COLOR_RESET);
        getchar();
        return;
    }
    getchar();
    jocNou.numarDescarcari = 0;
    jocuri[numarJocuri++] = jocNou;
    printf(COLOR_GREEN "Joc adaugat cu succes!\n" COLOR_RESET);
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
    salveazaDate();
}

void stergeJocMenu() {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== STERGERE JOC ===\n" COLOR_RESET);
    printf("Titlu joc de sters: ");
    char titluSterge[MAX_LUNGIME];
    scanf(" %[^\n]s", titluSterge);
    int gasit = 0;
    for (int i = 0; i < numarJocuri; i++) {
        if (strcmp(jocuri[i].titlu, titluSterge) == 0) {
            for (int j = i; j < numarJocuri - 1; j++) {
                jocuri[j] = jocuri[j + 1];
            }
            numarJocuri--;
            gasit = 1;
            printf(COLOR_GREEN "Joc sters cu succes!\n" COLOR_RESET);
            break;
        }
    }
    if (!gasit) {
        printf(COLOR_RED "Jocul nu a fost gasit.\n" COLOR_RESET);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
    salveazaDate();
}

void actualizeazaJocMenu() {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== ACTUALIZARE JOC ===\n" COLOR_RESET);
    printf("Titlu joc de actualizat: ");
    char titluActualizare[MAX_LUNGIME];
    scanf(" %[^\n]s", titluActualizare);
    int gasit = 0;
    for (int i = 0; i < numarJocuri; i++) {
        if (strcmp(jocuri[i].titlu, titluActualizare) == 0) {
            printf("Noua dimensiune (GB): ");
            if (scanf("%d", &jocuri[i].dimensiune) != 1) {
                printf(COLOR_RED "Dimensiune invalida. Introduceti un numar.\n" COLOR_RESET);
                getchar();
                return;
            }
            getchar();
            gasit = 1;
            printf(COLOR_GREEN "Joc actualizat cu succes!\n" COLOR_RESET);
            break;
        }
    }
    if (!gasit) {
        printf(COLOR_RED "Jocul nu a fost gasit.\n" COLOR_RESET);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
    salveazaDate();
}

void vizualizeazaStatisticiMenu() {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== VIZUALIZARE STATISTICI DESCARCARI ===\n" COLOR_RESET);
    for (int i = 0; i < numarJocuri; i++) {
        printf("%s - %d descarcari\n", jocuri[i].titlu, jocuri[i].numarDescarcari);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
}

void monitorizareUtilizatoriMenu() {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== MONITORIZARE UTILIZATORI ===\n" COLOR_RESET);
    printf("Introduceti " COLOR_YELLOW "numele utilizatorului" COLOR_RESET " pentru a vedea detalii: ");
    char numeUtilizator[MAX_LUNGIME];
    scanf("%s", numeUtilizator);
    getchar();
    Utilizator utilizator = incarcaUtilizator(numeUtilizator);
    if (strcmp(utilizator.numeUtilizator, numeUtilizator) == 0) {
        int spatiuLiber = 100 - (100 - utilizator.spatiuStocare);
        printf(COLOR_GREEN "Detalii utilizator: %s\n" COLOR_RESET, utilizator.numeUtilizator);
        printf("Spatiu liber: %d GB\n", spatiuLiber);
        printf("Jocuri descarcate:\n");
        if (utilizator.numarJocuriDescarcate > 0) {
            for (int i = 0; i < utilizator.numarJocuriDescarcate; i++) {
                printf("- %s\n", utilizator.jocuriDescarcate[i]);
            }
        } else {
            printf(COLOR_BLUE "Niciun joc descarcat.\n" COLOR_RESET);
        }
        printf("Blocat: %s\n", utilizator.blocat ? COLOR_RED "Da" COLOR_RESET : COLOR_GREEN "Nu" COLOR_RESET);
    } else {
        printf(COLOR_RED "Utilizatorul nu a fost gasit.\n" COLOR_RESET);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
}

void blocheazaUtilizatoriMenu() {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== BLOCARE UTILIZATORI ===\n" COLOR_RESET);
    printf("Nume utilizator de blocat: ");
    char numeBlocare[MAX_LUNGIME];
    scanf("%s", numeBlocare);
    getchar();

    Utilizator utilizator = incarcaUtilizator(numeBlocare);
    if (strcmp(utilizator.numeUtilizator, numeBlocare) == 0) {
        if (utilizator.blocat == 0) {
            utilizator.blocat = 1;
            salveazaUtilizator(utilizator);
            printf(COLOR_GREEN "Utilizator blocat cu succes!\n" COLOR_RESET);
        } else {
            printf(COLOR_YELLOW "Utilizatorul este deja blocat.\n" COLOR_RESET);
        }
    } else {
        printf(COLOR_RED "Utilizatorul nu a fost gasit.\n" COLOR_RESET);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
}

void vizualizareJocuriMenu(Utilizator utilizator) {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== VIZUALIZARE JOCURI ===\n" COLOR_RESET);
    for (int i = 0; i < numarJocuri; i++) {
        int descarcat = 0;
        for (int j = 0; j < utilizator.numarJocuriDescarcate; j++) {
            if (strcmp(jocuri[i].titlu, utilizator.jocuriDescarcate[j]) == 0) {
                descarcat = 1;
                break;
            }
        }
        printf(COLOR_BLUE "%s" COLOR_RESET " (%d GB) - %s\n", jocuri[i].titlu, jocuri[i].dimensiune, descarcat ? COLOR_GREEN "Descarcat" COLOR_RESET : "Disponibil");
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
}

void descarcaJocMenu(Utilizator utilizator) {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== DESCARCA JOC ===\n" COLOR_RESET);
    printf("Titlu joc de descarcat: ");
    char titluDescarcare[MAX_LUNGIME];
    scanf(" %[^\n]s", titluDescarcare);
    getchar();

    int jocIndex = findGameIndex(titluDescarcare);
    if (jocIndex != -1) {
        if (utilizator.spatiuStocare >= jocuri[jocIndex].dimensiune) {
            strcpy(utilizator.jocuriDescarcate[utilizator.numarJocuriDescarcate], titluDescarcare);
            utilizator.numarJocuriDescarcate++;
            utilizator.spatiuStocare -= jocuri[jocIndex].dimensiune;
            jocuri[jocIndex].numarDescarcari++;
            salveazaUtilizator(utilizator);
            salveazaDate();
            printf(COLOR_GREEN "Joc descarcat cu succes!\n" COLOR_RESET);
        } else {
            printf(COLOR_RED "Spatiu insuficient.\n" COLOR_RESET);
        }
    } else {
        printf(COLOR_RED "Jocul nu a fost gasit.\n" COLOR_RESET);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
}

void stergeJocInstalatMenu(Utilizator utilizator) {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== STERGE JOC INSTALAT ===\n" COLOR_RESET);
    printf("Titlu joc de sters: ");
    char titluSterge[MAX_LUNGIME];
    scanf(" %[^\n]s", titluSterge);
    getchar();

    int jocIndex = findGameIndex(titluSterge);
    int gasit = 0;
    for (int i = 0; i < utilizator.numarJocuriDescarcate; i++) {
        if (strcmp(utilizator.jocuriDescarcate[i], titluSterge) == 0) {
            for (int j = i; j < utilizator.numarJocuriDescarcate - 1; j++) {
                strcpy(utilizator.jocuriDescarcate[j], utilizator.jocuriDescarcate[j + 1]);
            }
            utilizator.numarJocuriDescarcate--;
            if(jocIndex != -1)
                utilizator.spatiuStocare += jocuri[jocIndex].dimensiune;
            salveazaUtilizator(utilizator);
            gasit = 1;
            printf(COLOR_GREEN "Joc sters cu succes!\n" COLOR_RESET);
            break;
        }
    }
    if (!gasit) {
        printf(COLOR_RED "Jocul nu a fost gasit.\n" COLOR_RESET);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
}

void inregistrareUtilizator() {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== INREGISTRARE UTILIZATOR ===\n" COLOR_RESET);
    Utilizator utilizatorNou;
    printf("Nume utilizator: ");
    scanf("%s", utilizatorNou.numeUtilizator);
    getchar();
    printf("Parola: ");
    scanf("%s", utilizatorNou.parola);
    getchar();
    utilizatorNou.spatiuStocare = 100;
    utilizatorNou.numarJocuriDescarcate = 0;
    utilizatorNou.blocat = 0;

    if (incarcaUtilizator(utilizatorNou.numeUtilizator).numeUtilizator[0] == '\0') {
        salveazaUtilizator(utilizatorNou);
        printf(COLOR_GREEN "Utilizator inregistrat cu succes!\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "Numele de utilizator exista deja!\n" COLOR_RESET);
    }
    getchar();
    printf("Apasa enter pentru a reveni..."); getchar();
}

void logareUtilizator(Utilizator *utilizator) {
    system(CLEAR_SCREEN);
    printf(COLOR_CYAN "=== LOGARE UTILIZATOR ===\n" COLOR_RESET);
    char numeUtilizator[MAX_LUNGIME];
    printf("Nume utilizator: ");
    scanf("%s", numeUtilizator);
    getchar();
    char parola[MAX_LUNGIME];
    printf("Parola: ");
    scanf("%s", parola);
    getchar();

    *utilizator = incarcaUtilizator(numeUtilizator);
    if (strcmp(utilizator->numeUtilizator, numeUtilizator) == 0) {
        if (utilizator->blocat == 1) {
            printf(COLOR_RED "Acest utilizator este blocat.\n" COLOR_RESET);
            getchar();
            strcpy(utilizator->numeUtilizator, "");
        } else if (strcmp(parola, utilizator->parola) == 0) {
            printf(COLOR_GREEN "Logare cu succes!\n" COLOR_RESET);
            getchar();
        } else {
            printf(COLOR_RED "Parola incorecta.\n" COLOR_RESET);
            getchar();
            strcpy(utilizator->numeUtilizator, "");
        }
    } else {
        printf(COLOR_RED "Utilizator inexistent.\n" COLOR_RESET);
        getchar();
        strcpy(utilizator->numeUtilizator, "");
    }
}


void incarcaDate() {
    FILE *fJocuri = fopen("jocuri.txt", "r");
    if (fJocuri != NULL) {
        while (fscanf(fJocuri, "%[^|]|%d|%d\n", jocuri[numarJocuri].titlu, &jocuri[numarJocuri].dimensiune, &jocuri[numarJocuri].numarDescarcari) == 3) {
            numarJocuri++;
        }
        fclose(fJocuri);
    }
}

void salveazaDate() {
    FILE *fJocuri = fopen("jocuri.txt", "w");
    if (fJocuri != NULL) {
        for (int i = 0; i < numarJocuri; i++) {
            fprintf(fJocuri, "%s|%d|%d\n", jocuri[i].titlu, jocuri[i].dimensiune, jocuri[i].numarDescarcari);
        }
        fclose(fJocuri);
    }
}

Utilizator incarcaUtilizator(char numeUtilizator[]) {
    char numeFisier[MAX_LUNGIME + 5];
    sprintf(numeFisier, "%s.txt", numeUtilizator);
    FILE *fUtilizator = fopen(numeFisier, "r");
    Utilizator utilizator;
    strcpy(utilizator.numeUtilizator, "");
    if (fUtilizator != NULL) {
        if (fscanf(fUtilizator, "%[^|]|%[^|]|%d|%d\n", utilizator.numeUtilizator, utilizator.parola, &utilizator.spatiuStocare, &utilizator.blocat) == 4) {
            utilizator.numarJocuriDescarcate = 0;
            char jocDescarcat[MAX_LUNGIME];
            while (fscanf(fUtilizator, "%[^|]|", jocDescarcat) == 1) {
                strcpy(utilizator.jocuriDescarcate[utilizator.numarJocuriDescarcate++], jocDescarcat);
            }
        }
        fclose(fUtilizator);
    }
    return utilizator;
}

void salveazaUtilizator(Utilizator utilizator) {
    char numeFisier[MAX_LUNGIME + 5];
    sprintf(numeFisier, "%s.txt", utilizator.numeUtilizator);
    FILE *fUtilizator = fopen(numeFisier, "w");
    if (fUtilizator != NULL) {
        fprintf(fUtilizator, "%s|%s|%d|%d\n", utilizator.numeUtilizator, utilizator.parola, utilizator.spatiuStocare, utilizator.blocat);
        for (int j = 0; j < utilizator.numarJocuriDescarcate; j++) {
            fprintf(fUtilizator, "%s|", utilizator.jocuriDescarcate[j]);
        }
        fprintf(fUtilizator, "\n");
        fclose(fUtilizator);
    }
}

int findGameIndex(const char* title) {
    for (int i = 0; i < numarJocuri; i++) {
        if (strcmp(jocuri[i].titlu, title) == 0) {
            return i;
        }
    }
    return -1;
}

