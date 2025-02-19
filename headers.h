#ifndef HEADERS_H
#define HEADERS_H

// libraries necessaires
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

// ANSI escape codes
#define Red "\033[0;31m"
#define Green "\033[0;32m"
#define Yellow "\033[0;33m"
#define Blue "\033[0;34m"
#define Magenta "\033[0;35m"
#define Cyan "\033[0;36m"
#define White "\033[0;37m"
#define Brown   "\033[38;5;130m"

#define Default "\033[0m"

// Definitions

#define ROWS_ROOM 5
#define COLS_ROOM 5
#define MAX_EDGES 4
#define OPTION_LINE 25
#define NUM_RIDDLES 3
#define RIGHT_SIDE 58

// Definition des structures

/** 
 * Items collected by player
 * Item has special properties
*/
typedef struct item_t {
    char *nom;
    char type;
    int puissance;
    int durability;
    int xp;
    char *desc;
} Item;

/**
 * Inventaire infinie pour collections des items
 */
typedef struct inventory_t {
    Item *item;
    struct inventory_t *next;
} Inventory;


/**
 * ennemies of main character
 */
typedef struct monster_t {
    char *nom;
    char type;
    int lvl;
    int xp;
    int puissance;
    int sante;
    char key;
} Monster;

/**
 * Last boss riddles struct
 */
typedef struct riddle_t {
    char *question;
    char *opts[3];
    char *reponse;
} Riddle;

/**
 * node of a room (2x2) array
 * node might be: vide, monstre, arbre ... (type)
 * type: 'T' = tree 'B' = Boss 'M' = Monster, 'R' = River, 'E' = Empty, 'S' = Sword, 'W' = Wand
 */
typedef struct node_t {
    int x, y;
    int hasBoss;
    char type;
    Monster *monster;
    Item *item;
} Node;

/**
 * Map that is a random combination of rooms
 * difficulty: 0-?
 */
typedef struct map_t {
    Node *room[ROWS_ROOM][COLS_ROOM];
    int difficulty;
    int visited;
    int hasBoss;
    int lvlLock;
    struct map_t *voisins[MAX_EDGES]; // left up right down
} Map;

// Les Piles

/**
 * historique des mouvements de joueur
 */
typedef struct Historique{
    Node *noeud;
    struct Historique *next;
}Historique;

void empiler(Historique **head,Node *noeud);
void dipiler(Historique **head);
Node *peek(Historique *head);

// Les files
typedef struct elem_t {
    char data;
    struct elem_t *next; 
} Elem;
// Queue structure
typedef struct {
    Elem *front;
    Elem *rear;
    int size;
} Queue;

void init_queue(Queue *q);
int is_empty(Queue *q);
void enqueue(Queue *q, char data);
char dequeue(Queue *q);
void free_queue(Queue *q);
char peekq(Queue *q);


// Les arbres binaires
/**
 * tree node properties
 */
typedef struct skillNode_t {
    char *nom;
    int puissance;
    int santemax;
    int armor;
    char class;
    char *desc;
} SkillNode;
/**
 * predefined tree of skills
 */
typedef struct skillTree_t {
    SkillNode *node;
    struct skillTree_t *left;
    struct skillTree_t *right;
} SkillTree;

/**
 * player skills linked list
 */
typedef struct skillSet_t {
    SkillNode *node;
    struct skillSet_t *next;
} SkillSet;

/**
 * Main character properties
 */
typedef struct player_t {
    char *nom;
    int lvl;
    int score;
    int sante;
    int puissance;
    int x, y;
    Map *currMap;
    int armor;
    Inventory *inventory;
    SkillSet *skillSet;
    SkillTree *skillTree;
    char class;
    Item *quickbar[4];
} Player;

void hideCursor();
void showCursor();
void moveCursor(int x, int y);
void clearTerm();
void clearLine();
void clearLineFromCur();

// Definitions des fonctions
void initRooms(Map *map[]);
void initGame(Player *p, char *name, Map *map[], int size);
void affichageGame(Player *player);
void playGame();
void translateToPrint(char type);
int directionBack(int direction);
void affichagePlayer(Player *player);
void interaction(Player *player, Historique **head);
int optionMenu(char *options[], int size, char *context);
void handleLvlUp(Player *player, int xp);
int countItems(Player *player);
int addItem(Player *player, Item *item);
Item *searchItem(Player *player, char *nom_item);
void deleteItem(Player *player, char *nom);
void affichageMonster(Monster *monster);
void affichageInventory(Player *p);
void affichageSkills(Player *p);
void clearAffichageMonster();
/*void clearAffichagePlayer();
void clearAffichageInventory(Player *p);
void clearAffichageSkills(Player *p);*/
void clearAffichageSide(Player *p);
void initRiddles(Riddle *riddles[]);
char *str_alloc(char *src);
SkillTree *createTree(int puissance, int maxsante, int armor, char *name, char *desc, char class);
int addSkill(Player *player, SkillNode *skill);
int skillsetLength(SkillSet *skillSet);
//void playSoundTrack();
void startMenu();
void gameOver(int won);

#endif