#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define RIGHT 0
#define LEFT 1
#define UP_DOWN 2

typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

//funkce pro zjištění, jestli je na dané straně zeď
bool is_border(Map *map, int r, int c, int border) {
    // Získání hodnoty buňky na daných souřadnicích (-1 protože indexujeme od 0)
    char cell = map->cells[(r - 1) * map->cols + (c - 1)];
    
    // Převedení hodnoty buňky na binární číslo
    int binary = cell - '0';

    // Pokud je bit na pozici border ve binárním zápisu hodnoty buňky (binary) nastaven na 1,
    // znamená to, že na dané straně buňky existuje zeď.
    if (binary & (1 << border)) {
        return true;
    } else {
        return false;
    }
}

//funkce pro zjištění, jestli je na dané straně vstup
// leftright: -1 pro pravou ruku, 1 pro levou ruku
int start_border(Map *map, int r, int c, int leftright)
{
    int border = -1;

    // Pokud vstoupíme do bludiště z horního nebo dolního konce mapy a není hranice na levé straně
    if ( (r == 1 && (c != 1 && c != map->cols)) || (r == map->rows && (c != 1 && c != map->cols))) {
        if (!is_border(map, r, c, 2)) {
            if (leftright == -1) {
                border = LEFT; // Pohyb doleva (pro pravou ruku)
            } else {
                border = RIGHT; // Pohyb doprava (pro levou ruku)
            }
        }
    // Pokud vstoupíme z levého nebo pravého konce mapy a aktuální pozice není levou hranicí,
    // určíme, zda se pohybujeme nahoru nebo dolů, nebo směrem k levé hranici.
    } else if (c == 1) {
        if (r % 2 == 1) {
            if (!is_border(map, r, c, 0) && leftright == -1) {
                border = UP_DOWN; // Pohyb nahoru pro pravou ruku
            } else {
                border = LEFT; // Pohyb doleva 
            }
        } else {
            if (!is_border(map, r, c, 0) && leftright == 1) {
                border = UP_DOWN; // Pohyb nahoru (pro levou ruku) 
            } else {
                border = LEFT; // Pohyb doleva 
            }
        }

    // Pokud vstoupíme z pravého konce mapy a aktuální pozice není pravou hranicí,
    // určíme, zda se pohybujeme nahoru nebo dolů, nebo směrem k pravé hranici.
    } else if (c == map->cols) {
        if (map->cols % 2 == 1) {
            if (!is_border(map, r, c, 1) && leftright == -1) {
                border = RIGHT; // Pohyb doprava (pro pravou ruku)
            } else { 
                border = UP_DOWN; // Pohyb nahoru nebo dolů (pro pravou ruku)
            }
        } else {
            if (!is_border(map, r, c, 1) && leftright == 1) {
                border = UP_DOWN; // Pohyb nahoru nebo dolů (pro levou ruku)
            } else {
                border = RIGHT; // Pohyb doprava (pro levou ruku)
            }
        }
    }

    //pokud chceme vstoupit do bludiště z buňky, která má hranice (trojuhelnik je převrácený)
    if ((r == 1 || r == map->rows) && (c != 1 && c != map->cols && c % 2 == 0)) {
        border = -1;
    }

    return border;
}

//funkce pro nalezení cesty. leftright - směr hledání cesty (1 - levá ruka, -1 - pravá ruka)
int find_way(Map *map, int r, int c, int leftright) {
    //odkud začínáme
    int border = start_border(map, r, c, leftright);

    //pokud není vstup (je na všech stranách zeď)
    if (border == -1) {
        return -1;
    }

    int smer;
    //pokud řádek a sloupec jsou oba sudé nebo oba liché, tak se pohybujeme doprava, jinak doleva
    if (r % 2 == c % 2) {
        smer = 1;
    } else {
        smer = -1;
    }

    //vypíšeme souřadnice
    while (r > 0 && r <= map->rows && c > 0 && c <= map->cols) {
        printf("%d,%d\n", r, c);

        int prekrocena_hranice = -1;

        int i = 0;
        while (i < 3 && prekrocena_hranice < 0) {
            
            // current_border - vypočítá aktuální hranu, ke které se algoritmus pohybuje,
            // s ohledem na základní hranu (border), směr pohybu (smer) a orientaci (leftright).
            int current_border = (border + (i * smer * leftright) + 3) % 3;

            // Pokud aktuální pozice leží mezi dvěma hranicemi, znamená to, že došlo k překročení hranice.
            // Podle toho, na kterou stranu směřuje pohyb, rozhodneme, jestli se máme otočit doleva nebo doprava.
            // V tomto případě vypisujeme informaci o aktuálních souřadnicích.
            if (!is_border(map, r, c, current_border)) {
                prekrocena_hranice = current_border;
            }
            i++;
        }

        //pokud řádek a sloupec jsou oba sudé nebo oba liché, tak nastavíme směr pohybu doprava, jinak doleva pro další iteraci
        if (r % 2 == c % 2) {
            smer = 1;
        } else {
            smer = -1;
        }

        if (prekrocena_hranice == RIGHT) {
            //překročena pravá hranice, pohybujeme se doleva
            border = LEFT;  // Nastavení nové hrany na levou
            c += -1;   // Pohyb ve sloupci (doleva)
        } else if (prekrocena_hranice == LEFT) {
            //překročena levá hranice, pohybujeme se doprava
            border = RIGHT;  // Nastavení nové hrany na pravou 
            c += 1;   // Pohyb ve sloupci (doprava)
        } else if (prekrocena_hranice == UP_DOWN) {
            //překročena horní nebo dolní hranice
            if (r % 2 == c % 2) {
                r += -1; // Pohyb v řádku (nahoru)
            } else {
                r += 1;   // Pohyb v řádku (dolů)
            }
            border = UP_DOWN;  // Nastavení nové hrany na dolní nebo horní
        }  

        //nastaení nové hrany pro další iteraci
        border = (border + smer * leftright + 3) % 3;
    }

    return 0;
}

//initializace mapy
int inicializace(Map *map, int rows, int cols){
    //nastavení řádků a sloupců z daných hodnot
    map->rows = rows;
    map->cols = cols;
    //alokace paměti pro pole buněk
    map->cells = malloc((rows * cols + 1) * sizeof(unsigned char));
    //pokud se nepodaří alokovat paměť
    if (map->cells == NULL) {
        return 1; 
    }
    return 0;
}

//načtení mapy
int load_map(Map *mapa, const char *filename){
    FILE *matrix = fopen(filename, "r");
    if (matrix == NULL)
        return 1;

    // Čtení počtu řádků a sloupců ze souboru v prvním řádku
    int rows, cols;
    fscanf(matrix, "%d %d", &rows, &cols);

    // Inicializace mapy s daným počtem řádků a sloupců
    inicializace(mapa, rows, cols);
 
    // Čtení znaků ze souboru a ukládání do pole buněk mapy
    char znak;
    int index = 0;

    while ((znak = fgetc(matrix)) != EOF)
    {
        //pokud není bílý znak (mezera, tabulátor, nový řádek) uložíme do pole buněk
        if (!isspace(znak)){
            mapa->cells[index++] = znak;
        }
    }

    // Uzavření souboru
    fclose(matrix);
    return 0;
}

int free_matrix(Map *map) {
    free(map->cells);
    map->rows = 0;
    map->cols = 0;

    return 0;
}

//funkce, která zkontroluje, jestli je mapa validní
int test(char *filename) {
    FILE *matrix = fopen(filename, "r");
    // chyba při otevírání souboru
    if (matrix == NULL) {
        return 1; 
    }

    // načtení prvního řádku se souřadnicemi
    int rows, cols;
    if (fscanf(matrix, "%d %d", &rows, &cols) != 2 || cols <= 0 || rows <= 0) {
        fclose(matrix);
        return 1;
    }

    // Alokace paměti pro 2D pole buněk 
    unsigned char **map = malloc(rows * sizeof(unsigned char *));
    for (int i = 0; i < rows; ++i) {
        // Alokace paměti pro řádek
        map[i] = malloc(cols * sizeof(unsigned char));
    }

    // Načtení buněk ze souboru
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (fscanf(matrix, " %hhu", &map[i][j]) != 1 || (map[i][j] > 7)) {
                // Pokud se nenačetla buňka, uvolníme paměť a uzavřeme soubor
                for (int k = 0; k < rows; ++k) {
                    free(map[k]);
                }
                free(map);
                fclose(matrix);
                return 1;
            }
        }
    }

    //Zkontrolujeme hrany 
    // 100 levá strana
    // 010 pravá strana
    // 001 horní (pokud je řádek sudý)/dolní strana
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            unsigned char current_num = map[i][j];

            //Zkontrolujeme levou a pravou stranu
            //Pokud current_num má 1 na levé straně, tak musí mít 2 buňka vedle něj 1 na pravé straně
            if (j < cols - 1) {
                if ((current_num & 0x02) == 0x02) {
                    unsigned char next = map[i][j + 1];
                    if ((next & 0x01) != 0x01) {
                        //Pokud se neshodují, tak uvolníme paměť a uzavřeme soubor
                        for (int k = 0; k < rows; ++k) {
                            free(map[k]);
                        }
                        free(map);
                        fclose(matrix);
                        return 1;
                    }
                }
            }

            //Zkontrolujeme horní a dolní stranu
            //Pokud je řádek sudý, tak zkontrolujeme horní stranu, jinak dolní
            //Next je buňka nad nebo pod aktuální buňkou
            if (i > 0 && i < rows - 1 && (i + j) % 2 == 0) {
                if ((current_num & 0x04) == 0x04) {
                    unsigned char next = map[i - 1][j];
                    if ((next & 0x04) != 0x04) {
                       //uvolnění paměti a uzavření souboru
                        for (int k = 0; k < rows; ++k) {
                            free(map[k]);
                        }
                        free(map);
                        fclose(matrix);
                        return 1;
                    }
                }
            }
        }
    }

    //uvolnění paměti a uzavření souboru
    for (int k = 0; k < rows; ++k) {
        free(map[k]);
    }
    free(map);
    fclose(matrix);
    return 0;
}

int main(int argc, char *argv[]) {
//pokud byl zadan spatny pocet argumentu
    if (argc<2){
        fprintf(stderr, "Zkuste ./maze --help\n");
        return 1;
    }

//vypiše nápovědu
    if (argc == 2 && strcmp( argv[1], "--help")==0) {
        printf("./maze --test soubor.txt\n "
            "(pouze zkontroluje, že soubor daný druhým argumentem programu obsahuje řádnou definici mapy bludiště.)\n\n"
            "./maze --rpath R C soubor.txt\n"
            "(hledá průchod bludištěm na vstupu na řádku R a sloupci C. Průchod hledá pomocí pravidla pravé ruky (pravá ruka vždy na zdi.)\n\n"
            "./maze --lpath R C soubor.txt\n"
            "(hledá průchod bludištěm na vstupu na řádku R a sloupci C, ale za pomoci pravidla levé ruky.) \n");
        return 0;
    }

//jestli mapa je validní nebo ne
    if (argc == 3 && strcmp(argv[1], "--test")==0){
              if (!test(argv[2]))
                printf("Valid\n");
            else
                printf("Invalid\n");
        return 0;
    }

    Map bludiste;
    //převedeme do int 
    int souradnice1=atoi(argv[2]);
    int souradnice2=atoi(argv[3]);

    if (argc == 5 && strcmp(argv[1], "--rpath")==0){

        //pokud mapa není validní
        if (test(argv[4]) == 1){
            printf("Invalid map!\n");
            return 1;
        }

        //zkusíme jestli zadané souřadnice jsou záporné
        if (souradnice1 < 0 || souradnice2 < 0){
            fprintf(stderr, "Invalid coordinates!\n");
            return 1;
        }

        //pokud se nepodaří načíst mapu
        if (load_map(&bludiste, argv[4]) == 1) {
            fprintf(stderr, "Chyba při načítání mapy\n");
            free_matrix(&bludiste);
            return 1;
        }

        //pokud se nepodaří najít cestu pro pravou ruku
        if (find_way(&bludiste, souradnice1, souradnice2, -1) == -1) {
            fprintf(stderr, "Cesta nenalezena\n");
            free_matrix(&bludiste);
            return 1;
        }

        free_matrix(&bludiste);
        return 0;
    }

    //stejné jako předchozí, jenom s levou rukou
    if (argc == 5 && strcmp(argv[1], "--lpath")==0){
        //zkusíme jestli zadané souřadnice jsou záporné
        if (souradnice1 < 0 || souradnice2 < 0){
            fprintf(stderr, "Invalid coordinates!\n");
            return 1;
        }
        
        //pokud mapa není validní
        if (test(argv[4])){
            fprintf(stderr,"Invalid map!\n");
            return 1;
        }

        //pokud se nepodaří načíst mapu
        if (load_map(&bludiste, argv[4]) == 1) {
            fprintf(stderr, "Chyba při načítání mapy\n");
            free_matrix(&bludiste);
            return 1;
        }

        //pokud se nepodaří najít cestu pomocí pravidla levé ruky
        if (find_way(&bludiste, souradnice1, souradnice2, 1) == -1) {
            fprintf(stderr, "Cesta nenalezena\n");
            free_matrix(&bludiste);
            return 1;
        }

        free_matrix(&bludiste);
        return 0;
    }

    //něco bylo špatně zadané
    else {
        printf("Zkuste znova anebo zavolejte ./maze --help\n");
        return 1;
    }
}



