#include "headers.h"

char translateToPrint(char type) {
    switch (type) {
        case 'E':
            return '.';
        default:
            return type;
    }
}
/*int checkDoor(Map *map, int r, int c) {
    int rows[4] = {(ROWS_ROOM - 1) / 2, 0, (ROWS_ROOM - 1) / 2, ROWS_ROOM - 1};
    int cols[4] = {0, (COLS_ROOM - 1) / 2, COLS_ROOM - 1, (COLS_ROOM - 1) / 2};

    for (int i = 0; i < 4; i++) {
        if (c == cols[i] && r == rows[i] && map->voisins[i]) {
            return 1;
        }
    }
    return 0;
}*/
void affichageMap(Player *player) {
    moveCursor(1, 1);
    for (int i = 0; i < ROWS_ROOM; i++) {
        for (int j = 0; j < COLS_ROOM; j++) {
            if (player->x == j && player->y == i) {
                printf(" %s@%s", Green, White);
            } else printf(" %c", translateToPrint(player->currMap->room[i][j]->type));
        }
        printf(" \n");
        }
    }



void affichageMonster(Monster *monster) {
    moveCursor(8, 60);
    printf("Monster Name: %s", monster->nom);
    moveCursor(9, 60);
    printf("HP: %d", monster->sante);
    moveCursor(10, 60);
    printf("Damage: %d", monster->puissance);
}

void clearMonsterAffichage() {
    moveCursor(8, 60);
    clearLine();
    moveCursor(9, 60);
    clearLine();
    moveCursor(10, 60);
    clearLine();
}
void affichagePlayer(Player *player) {
    moveCursor(2, 60);
    printf("Player Name: %s", player->nom);
    moveCursor(3, 60);
    printf("                        "); // solution temp
    moveCursor(3, 60);
    printf("HP: %d", player->sante);
    moveCursor(4, 60);
    printf("Player Position: (%d, %d)\n", player->x + 1, player->y + 1);
    moveCursor(5, 60);
    printf("Lvl: %d, Score: %d", player->lvl, player->score);
    moveCursor(6, 60);
    printf("Inventaire : ");
    Inventory *current = player->inventory;
    if (current == NULL) printf("(Empty) ");
    int i = 7;
    while (current != NULL) {
        moveCursor(i++, 60);
        printf("%s (puissance: %d) (durability: %d)", current->item->nom, current->item->puissance, current->item->durability);
        current = current->next;
    }
    printf("\n");

}

void affichageGame(Player *player) {
    /*int width, height;
    getTerminalSize(&width, &height);*/
    affichageMap(player);
    affichagePlayer(player);
}