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

void initSkillTree(SkillTree *root) {
    if (root == NULL) {
        printf("Error: cannot initialize tree");
        exit(1);
    }
    //for mage
    root->right = createTree(10, 0, 0, "Fireball", "Class: Mage + Skill: Fireball", 'A');//Launches a small fireball that deals moderate damage.
    root->right->left = createTree(10, 0, 0, "Freeze", "Freeze: make the enemie unable to attack (skip his turn)", 'A');////make the enemie unable to attack (skip his turn)
    root->right->right = createTree(0, 20, 5, "Arcane Shield", "Arcane Shield: Temporarily increases defense", 'A');//Temporarily increases defense
    root->right->left->left = createTree(20, 0, 0, "Flame Burst", "Flame Burst: Creates an explosion around the player, damaging nearby enemies", 'A');//Creates an explosion around the player, damaging nearby enemies.
    root->right->left->right = createTree(0, 10, 10, "Boost spell", "Boost spell: temporary increase armor and health", 'A');//temporary increase armor and health
    root->right->right->left = createTree(15, 10, 0, "Mana Surge", "Mana Surge: Boosts magic power for a short time, increasing spell damage", 'A');//Boosts magic power for a short time, increasing spell damage.
    root->right->right->right = createTree(0, 0, 20, "Arcane Cataclysm", "Arcane Cataclysm: Unleashes a devastating wave of energy, hitting all enemies", 'A');//Unleashes a devastating wave of energy, hitting all enemies.

    //for knight
    root->left = createTree(10, 0, 5,"Heavy Strike", "Class: Knight + Skill: Heavy Strike", 'A');//attack with good damage
    root->left->left = createTree(10, 0, 5, "Stun", "Stun: Make enemy unable to attack (skip his turn)", 'A');//make the enemie unable to attack (skip his turn)
    root->left->right = createTree(0, 20, 10, "Iron Defense", "Iron Defense: Temporarily increases armor, reducing damage taken", 'A');//Temporarily increases armor, reducing damage taken.
    root->left->left->left = createTree(15, 10, 0, "Space cleave", "Space cleave: Launch devastating sword attack that can hit multiple enemies at once", 'A');//Launch devastating sword attack that can hit multiple enemies at once
    root->left->left->right = createTree(5, 0, 20, "Parry", "Parry: Reflect the enemies attak back to it", 'A');//reflect the enemies attak back to it
    root->left->right->left = createTree(30, 0, 0, "Berserker Mode", "Berserker Mode: increase power attack signficantly but with each attack you lose health", 'A');//increase power attack signficantly but with each attack you lose health
    root->left->right->right = createTree(0, 0, 30, "Death Blade", "Death Blade: poison the enemie(-10 hp till fight ends)", 'A');//poison the enemie(-10 hp till fight ends)
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