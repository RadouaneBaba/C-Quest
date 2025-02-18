#include "headers.h"

void startMenu() {
    printf("The game has started !\n");
    printf("Press [S] to start or [Q] to quit...");
    char c;
    do {
        c = toupper(_getch());
        sleep(0.01);
    } while (c != 'S' && c != 'Q');

    if (c == 'Q') exit(0);
    clearTerm();
}

void gameOver() {
    exit(0);
}


int checkHitDoor(Player *player) {
    // doors: 2 0: left, 0 2: top, 2 4: right, 4 2: bot
    // Rows - 1 / 2: 0 ..
    int rows[4] = {(ROWS_ROOM - 1) / 2, 0, (ROWS_ROOM - 1) / 2, ROWS_ROOM - 1};
    int cols[4] = {0, (COLS_ROOM - 1) / 2, COLS_ROOM - 1, (COLS_ROOM - 1) / 2};

    for (int i = 0; i < 4; i++) {
        if (player->x == cols[i] && player->y == rows[i]) {
            if (player->currMap->voisins[i]) {
                player->currMap = player->currMap->voisins[i];
                player->currMap->visited = 1;
                player->x = cols[directionBack(i)];
                player->y = rows[directionBack(i)];
                affichageGame(player);
                return 1;
            }
        }
    }
    /*if (player->currMap->room[player->y][player->x]->type == 'D') {
        player->currMap = player->currMap->voisins[i];
        player->currMap->visited = 1;
        player->x = cols[directionBack(i)];
        player->y = rows[directionBack(i)];
        affichageGame(player);
        return 1;
    }*/
    return 0;
}

void deplacementplayer(Player *player, char input, Historique **head){
    empiler(head,player->currMap->room[player->y][player->x]);

    moveCursor(27, 3);
    clearLine();
    moveCursor(27, 3);

    switch(input){
        case 'D'://right
        if(player->x < COLS_ROOM - 1){
            player->x++;
        }
        else printf("-> You reached the edge of the room ");
        break;
        case 'Q'://left
        if(player->x > 0){ 
            player->x--;
        }
        else printf("-> You reached the edge of the room ");
        break;
        case 'Z'://up
        if(player->y > 0){
            player->y--;
        }
        else printf("-> You reached the edge of the room ");
        break;
        case 'S'://down
        if(player->y < ROWS_ROOM - 1){
            player->y++;
        }
        else printf("-> You reached the edge of the room ");
        break;
    }
    if (checkHitDoor(player)) empiler(head,player->currMap->room[player->y][player->x]);
    else interaction(player, head);
}


void playerCommands(Player *player, Historique **head) {
    int started = 0;
    char *options[] = {"Get 100 XP to help you in your quest hero"};
    
    while (1) {
        sleep(0.01);
        if (kbhit()) {
            char input = getch();
            if (!started) {
                optionMenu(options, 1);
                handleLvlUp(player, 100); // to start skill tree and pick a class
                started = 1;
            }
            deplacementplayer(player, toupper(input), head);
            Node *old = peek(*head);
            if (old) {
                /*moveCursor(30, 1);
                /*printf("%d %d", old->x, old->y);*/
                moveCursor(1 + old->x, 2 + old->y * 2);
                printf("\033[45m%c", translateToPrint(old->type));
                printf("\033[0m");
            }
            moveCursor(1 + player->y, 2 + player->x * 2);
            printf("\033[1m");
            printf("%s@", Green);
            printf("\033[21m%s", White);
            affichagePlayer(player);
        }
    }
}

void playGame() {
    //printf("%c%c", (char) 201, (char) 205);
    char name[20];
    int size;
    Historique *head = NULL;
    Map *map[5];
    Player *p = (Player *)malloc(sizeof(Player));

    startMenu();
    printf("What's the size of  map you want ? ");
    scanf("%d", &size);
    printf("What's your name hero ? ");
    scanf("%s", name);
    clearTerm();
    initGame(p, name, map, size);
    hideCursor();
    affichageGame(p);
    playerCommands(p, &head);
}