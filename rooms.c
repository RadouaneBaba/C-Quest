#include "headers.h"
#define POWER 5
#define HP 10
#define DURABILITY 100
#define INITIAL_XP 50
// Create a new item
Item* createItem(char *name, char type, int power, int durability, char *desc, int xp) {
    Item *newItem = (Item*)malloc(sizeof(Item));
    newItem->nom = name;
    newItem->type = type;
    newItem->puissance = power;
    newItem->durability = durability;
    newItem->desc = desc;
    newItem->xp = xp;
    return newItem;
}

// Create a new monster
Monster* createMonster(char *name, char type, int lvl, int power, int health, char key, int xp) {
    Monster *newMonster = (Monster*)malloc(sizeof(Monster));
    newMonster->nom = name;
    newMonster->type = type;
    newMonster->lvl = lvl;
    newMonster->puissance = power;
    newMonster->sante = health;
    newMonster->key = key;
    newMonster->xp = xp;
    return newMonster;
}

Node *createNode(int x, int y, int hasBoss, char type, Monster *monster, Item *item) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->hasBoss = hasBoss;
    newNode->type = type;
    newNode->monster = monster;
    newNode->item = item;
    return newNode;
}
// Initialize the 5x5 matrix of rooms
Map *loadRoom(char *filepath, int difficulty) {
    Map *newMap = (Map*)malloc(sizeof(Map));
    for (int i = 0; i < MAX_EDGES; i++) {
        newMap->voisins[i] = NULL;
    }
    FILE *file = fopen(filepath, "r");
    char line[COLS_ROOM + 2];
    int i = 0;
    int xp = 0;
    while (fgets(line, sizeof(line), file) != NULL && i < ROWS_ROOM) {
        for (int j = 0; j < ROWS_ROOM; j++) {
          int hasBoss = 0;
          Monster *monster = NULL;
          Item *item = NULL;
          switch (line[j]) {
            case 'T':
                item = createItem("Wood", 'H', 0, DURABILITY, "wood to help make a bridge", INITIAL_XP / 2);
                xp += INITIAL_XP / 2;
                break;
            case 'M':
                monster = createMonster("org", 'A', difficulty, POWER * difficulty, HP * difficulty, 'A', INITIAL_XP * difficulty);
                xp += INITIAL_XP;
                break;
            case 'S':
                item = createItem("Sword", 'A', 25, DURABILITY, "Magic Sword", INITIAL_XP);
                xp += INITIAL_XP;
                break;
            case 'W':
                item = createItem("Wand", 'A', 25, DURABILITY, "Magic Wand", INITIAL_XP);
                xp += INITIAL_XP;
                break;
            case 'B':
                hasBoss = 1;
                break;
          }
          if (hasBoss) {
            newMap->hasBoss = 1;
            newMap->lvlLock = xp / 100;
          }
          newMap->room[i][j] = createNode(i, j, hasBoss, line[j], monster, item);
        }
        i++;
    }
    fclose(file);
    return newMap;
}
void initRooms(Map *map[]) {
    map[0] = loadRoom("rooms/lobby.txt", 1);
    map[1] = loadRoom("rooms/river_tree.txt", 2);
    map[2] = loadRoom("rooms/empty.txt", 1);
    map[3] = loadRoom("rooms/monster.txt", 3);
    map[4] = loadRoom("rooms/boss.txt", 4);
}