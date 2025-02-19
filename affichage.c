#include "headers.h"


/**
 * different printing for each type of nodes
 */
void translateToPrint(char type) {
    switch (type) {
        case 'E':
            printf("  .  ");
            break;
        case 'T':
            printf("  %s%c%s  ", Brown, type, Default);
            break;
        case 'M':
        case 'B':
            printf("  %s%c%s  ", Red, type, Default);
            break;
        case 'R':
            printf("  %s%c%s  ", Blue, type, Default);
            break;
        case 'S':
        case 'W':
            printf("  %s%c%s  ", Cyan, type, Default);
            break;
        default:
            printf("  %c  ", type);
            break;
    }
}
void clearAffichageRight(int start, int end) {
    for (int i = start; i <= end; i++) {
        moveCursor(i, RIGHT_SIDE);
        clearLineFromCur();
    }
}
void clearAffichagePlayer() {
    clearAffichageRight(1, 8);
}
void clearAffichageMonster() {
    clearAffichageRight(13, 17);
}
void clearAffichageInventory(Player *p) {
    clearAffichageRight(1, 3 + countItems(p));
}
void clearAffichageSkills(Player *p) {
    clearAffichageRight(1, 3 + skillsetLength(p->skillSet));
}

/**
 * help clearing just one side of the terminal to be able to alter between different menus
 */
void clearAffichageSide(Player *p) {
    clearAffichagePlayer();
    clearAffichageInventory(p);
    clearAffichageMonster(p);
}

/**
 * display of the main parts of the UI
 */
void affichageMap(Player *player) {
    moveCursor(1, 1);
    printf("+");
    for (int i = 0; i < COLS_ROOM * 5; i++) printf("-");
    printf("+\n");
    for (int i = 0; i < ROWS_ROOM; i++) {
        printf("|");
        moveCursor(i * 2 + 2,COLS_ROOM * 5 + 2);
        printf("|\n|");
        for (int j = 0; j < COLS_ROOM; j++) {
            if (player->x == j && player->y == i) {
                printf("  %s@%s  ", Green, Default);
            } else translateToPrint(player->currMap->room[i][j]->type);
        }
        printf("|\n");
    }
    printf("|");
    moveCursor(ROWS_ROOM * 2 + 2,COLS_ROOM * 5 + 2);
    printf("|\n+");
    for (int i = 0; i < COLS_ROOM * 5; i++) printf("-");
    printf("+");
    /*
    
    moveCursor(3, 4); printf("A");
    moveCursor(3, 4 + 5); printf("B");
    moveCursor(3 + 2, 4); printf("C");
    */
}


void affichageInventory(Player *p) {
    int i = 1;
    moveCursor(i, RIGHT_SIDE);
    printf(">------------- Inventory -------------<");
    Inventory *current = p->inventory;
    i++;
    if (current == NULL) {
        moveCursor(i + 1, RIGHT_SIDE);
        printf("(Empty) ");
    }
    while (current != NULL) {
        moveCursor(++i, RIGHT_SIDE);
        printf("%s (puissance: %d) (durability: %d)", current->item->nom, current->item->puissance, current->item->durability);
        current = current->next;
    }
}
void affichageSkills(Player *p) {
    int i = 1;
    moveCursor(i, RIGHT_SIDE);
    printf(">------------- SkillSet -------------<");
    SkillSet *current = p->skillSet;
    i++;
    if (current == NULL) {
        moveCursor(i + 1, RIGHT_SIDE);
        printf("(Empty) ");
    }
    while (current != NULL) {
        moveCursor(++i, RIGHT_SIDE);
        printf("Name: %s, Class: %c", current->node->nom, current->node->class);
        current = current->next;
    }
}
void affichageMonster(Monster *monster) {
    moveCursor(13, RIGHT_SIDE);
    printf(">------------- Monster Stats ------------<");
    moveCursor(15, RIGHT_SIDE);
    printf("Monster Name: %s", monster->nom);
    moveCursor(16, RIGHT_SIDE);
    printf("HP: %d", monster->sante);
    moveCursor(17, RIGHT_SIDE);
    printf("Damage: %d", monster->puissance);
}

void affichagePlayer(Player *player) {
    moveCursor(1, RIGHT_SIDE);
    printf(">------------- Player Stats -------------<");
    /*moveCursor(2, RIGHT_SIDE);
    printf("|                                    |");*/
    moveCursor(3, RIGHT_SIDE);
    printf(" Player Name: %s", player->nom);
    moveCursor(4, RIGHT_SIDE);
    clearLineFromCur(); // solution temp
    moveCursor(4, RIGHT_SIDE);
    printf(" HP: %d", player->sante);
    moveCursor(5, RIGHT_SIDE);
    printf(" Player Position: (%d, %d)", player->x + 1, player->y + 1);
    moveCursor(6, RIGHT_SIDE);
    printf(" Level: %d", player->lvl);
    moveCursor(7, RIGHT_SIDE);
    printf(" Score: %d", player->score);
    moveCursor(8, RIGHT_SIDE);
    printf(" Class: ");
    switch(player->class) {
        case 'N':
            printf("(None)");
            break;
        case 'K':
            printf("Knight");
            break;
        case 'M':
            printf("Mage");
            break;
    }
}

void affichageHelp() {
    int start = 16;
    moveCursor(start, 1);
    printf("+- Controls -------------------------+");
    moveCursor(start + 1, 1);
    printf("|                                    |");
    moveCursor(start + 2, 1);
    printf("| [Z] UP [S] DOWN [Q] LEFT [D] RIGHT |");
    moveCursor(start + 3, 1);
    printf("| [I] Inventory   [T] SkillSet       |");
    moveCursor(start + 4, 1);
    printf("|                                    |");
    moveCursor(start + 5, 1);
    printf("+------------------------------------+");
}

void affichageGame(Player *player) {
    affichageMap(player);
    affichagePlayer(player);
    affichageHelp();
}