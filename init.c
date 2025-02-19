#include "headers.h"

// init joueur init map init *
int directionBack(int direction) {
    switch (direction) {
        case 0:
            return 2;
        case 1:
            return 3;
        case 2:
            return 0;
        case 3:
            return 1;
    }
}

void initMap(int size, Map *map[]) {
    srand(time(NULL));
    initRooms(map);
    Map* cur = map[0];
    // make doors in rooms
    int rows[4] = {(ROWS_ROOM - 1) / 2, 0, (ROWS_ROOM - 1) / 2, ROWS_ROOM - 1};
    int cols[4] = {0, (COLS_ROOM - 1) / 2, COLS_ROOM - 1, (COLS_ROOM - 1) / 2};
    size = 5; // for now
    for (int i = 1; i < size; i++) {
        int randDirection;
        do {
            randDirection = rand() % (MAX_EDGES - 1);
            //printf("randDirection %d\n", randDirection);
        } while (cur->voisins[randDirection] != NULL);

        cur->room[rows[randDirection]][cols[randDirection]]->type = 'D';
        cur->voisins[randDirection] = map[i];
        cur->voisins[randDirection]->room[rows[directionBack(randDirection)]][cols[directionBack(randDirection)]]->type = 'D';
        cur->voisins[randDirection]->voisins[directionBack(randDirection)] = cur;
        cur = cur->voisins[randDirection];
    }
    /*
    // left up right down
    int c = 0; // created cartes
    Map *cur = spawn;
    while (c < size) {
        int randNum = rand() % (MAX_EDGES - 1) + 1;
        for (int i = 0; i < randNum; i++) {
            int randDirection = rand() % (MAX_EDGES - 1);
            cur->voisins[randDirection] = (Map *) malloc(sizeof(Map));
            cur->voisins[randDirection]->voisins[directionBack(randDirection)] = cur;
            cur = cur->voisins[randDirection];
        
        }
    }
    */
}

//initializing predefined skill tree

void initSkillTree(SkillTree *root) {
    if (root == NULL) {
        printf("Error: cannot initialize tree");
        exit(1);
    }
    //for mage
    root->right = createTree(10, 0, 0, "Fireball", "Class: Mage + Skill: Fireball", 'A');
    root->right->left = createTree(0, 0, 0, "Freeze", "Freeze: make the enemie unable to attack for one turn", 'S');
    root->right->right = createTree(0, 20, 5, "Arcane Shield", "Arcane Shield: increases defense", 'D');
    root->right->left->left = createTree(20, 0, 0, "Black flash", "Black flash: Creates an explosion around the player,dealing decent damage", 'A');
    root->right->left->right = createTree(0, 0, 0, "Parry", "Parry: Reflect the enemies attak back to it", 'E');
    root->right->right->left = createTree(15, 10, -5, "Mana Surge", "Mana Surge: Boosts magic power and health, increasing spell damage,but reducing armor", 'B');
    root->right->right->right = createTree(100, 0, 0, "Hollow purple", "Hollow purple: Unleashes a devastating wave of energythat can kill enemy in one hit(1/3 hit chance)", 'U');

    //for knight
    root->left = createTree(15, 0, 0,"Heavy Strike", "Class: Knight + Skill: Heavy Strike", 'A');
    root->left->left = createTree(0, 0, 0, "Stun", "Stun: make the enemie unable to attack for one turn", 'S');
    root->left->right = createTree(0, 20, 10, "Iron Defense", "Iron Defense: increases armor, reducing damage taken", 'D');
    root->left->left->left = createTree(100,0, 0, "Cleave", "Cleave: Launch devastating sword attack that can kill enemy in one hit(1/4 hit chance)", 'U');
    root->left->left->right = createTree(0, 0, 0, "Parry", "Parry: Reflect the enemies attak back to it", 'E');
    root->left->right->left = createTree(30, -5, -5, "Berserker", "Berserker: increase power attack signficantly but reducing health", 'B');
    root->left->right->right = createTree(10, 0, 0, "Death Blade", "Death Blade: poison the enemie(-10 hp every turn)", 'E');
}

// Function to initialize nerd C riddles
void initRiddles(Riddle *riddles[]) {
    for (int i = 0; i < NUM_RIDDLES; i++) {
        riddles[i] = malloc(sizeof(Riddle));
        if (!riddles[i]) {
            printf("Memory allocation failed");
            exit(1);
        }
    }

    // Riddle 1
    riddles[0]->question = str_alloc("Why did the C pointer break up with the array?");
    riddles[0]->opts[0] = str_alloc("(A) It wanted more space");
    riddles[0]->opts[1] = str_alloc("(B) It couldn't handle the address");
    riddles[0]->opts[2] = str_alloc("(C) It lost its reference");
    riddles[0]->reponse = str_alloc("(A) It wanted more space");

    // Riddle 2
    riddles[1]->question = str_alloc("Why do C programmers prefer the dark?");
    riddles[1]->opts[0] = str_alloc("(A) Because they hate bugs");
    riddles[1]->opts[1] = str_alloc("(B) Because they love pointers");
    riddles[1]->opts[2] = str_alloc("(C) Because they can't C in the light");
    riddles[1]->reponse = str_alloc("(C) Because they can't C in the light");

    // Riddle 3
    riddles[2]->question = str_alloc("What happens if you dereference a NULL pointer?");
    riddles[2]->opts[0] = str_alloc("(A) You summon a segmentation fault");
    riddles[2]->opts[1] = str_alloc("(B) It prints NULL");
    riddles[2]->opts[2] = str_alloc("(C) Undefined behavior");
    riddles[2]->reponse = str_alloc("(A) You summon a segmentation fault");
}

void initPlayer(Player *p, char *name, Map *lobby) {
    p->nom = (char *) malloc(strlen(name) * sizeof(name[0]));
    strcpy(p->nom, name);
    p->lvl = 0;
    p->score = 0;
    p->sante = 100;
    p->puissance = 0;
    p->x = 2;
    p->y = 1;
    p->currMap = lobby;
    p->armor = 0;
    p->currMap->visited = 1;
    p->inventory = NULL;
    p->skillSet = NULL;
    p->skillTree = createTree(0, 100, 0, "Beginning", "Spawning to the world", '-');
    p->class = 'N';
}
// scan player name, map size, init map, init player, afficher (map, stats, narrative, inventory, quick bar)

void initGame(Player *p, char *name, Map *map[], int size) {
    initMap(size, map);
    initPlayer(p, name, map[0]);
    initSkillTree(p->skillTree);
}