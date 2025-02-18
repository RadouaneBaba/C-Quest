#include "headers.h"


void undoMovement(Player *player, Historique **head) {
    Node *old = peek(*head);
    player->x = old->y;
    player->y = old->x;
    dipiler(head);
}

void handleLvlUp(Player *player, int xp) {
    player->score += xp;
    int lvlup = player->score / 100;
    if (lvlup > player->lvl) {
        moveCursor(OPTION_LINE, 2);
        printf("+ Level Up");
        sleep(1);
        clearLine();
        //moveCursor(OPTION_LINE - 1, 1);
        //printf("Choose a Skill");
        char *options[] = {player->skillTree->left->node->desc, player->skillTree->right->node->desc};
        int res = optionMenu(options, 2);
        if (res == 0) {
            player->skillTree = player->skillTree->left;
        } else {
            player->skillTree = player->skillTree->right;
        }
        SkillNode *newskill = player->skillTree->node;
        addSkill(player, newskill);
        player->armor += newskill->armor;
        player->puissance += newskill->puissance;
        player->sante += newskill->santemax;
        if (lvlup == 1) {
            char *nom = newskill->nom;
            if (strcmp(nom, "Heavy Strike") == 0) player->class = 'K';
            else player->class = 'M';
        }
    }
    player->lvl = lvlup;
}
void handleItem(Player *player, Historique **head, char type) {
    Item *item = player->currMap->room[player->y][player->x]->item;
    char s1[20] = "Get a ";
    strcat(s1, item->nom);
    char *options[] = {s1, "Do nothing"};
    int res = optionMenu(options, 2);
    if (res == 0) {
        // different handling for different item types !!
        addItem(player, item);
        player->currMap->room[player->y][player->x]->type = 'E';
        handleLvlUp(player, item->xp);
        switch(type) {
            case 'S':
            case 'W':
                player->puissance = item->puissance;
                break;
        }
    }
}
void handleTree(Player *player, Historique **head) {
    char *options[] = {"Cut wood from tree", "Do nothing"};
    int res = optionMenu(options, 2);
    if (res == 1) {
        undoMovement(player, head);
    } else {
        Item *item = player->currMap->room[player->y][player->x]->item;
        addItem(player, item);
        player->currMap->room[player->y][player->x]->type = 'E';
        handleLvlUp(player, item->xp);
    }
}

void handleRiver(Player *player, Historique **head) {
    Item *item = searchItem(player, "Wood");
    if (item) {

        char *options[] = {"Build a bridge to cross the river", "Do nothing"};
        int res = optionMenu(options, 2);
        if (res == 1) {
            undoMovement(player, head);
        } else {
            player->currMap->room[player->y][player->x]->type = '#';
            deleteItem(player, "Wood");
            affichagePlayer(player);
        }
    } else {
        char *options[] = {"You can't cross the river unless you've got wood from trees!"};
        optionMenu(options, 1);
        undoMovement(player, head);
    }
}

void handleCombat(Player *player, Monster *monster) {
    Queue *turns = (Queue *) malloc(sizeof(Queue));
    init_queue(turns);
    enqueue(turns, 'P');
    enqueue(turns, 'M');

    SkillSet *curr = player->skillSet;
    int length = skillsetLength(curr);
    /*if (length > 0) curr = curr->next; // skip first skill (First skill is not usable skill)
    length--;*/
    char *poptions[length];
    SkillNode *skills[length];

    // add skills info to both arrays
    for (int i = 0; i < length && curr; i++) {
        skills[i] = curr->node;
        poptions[i] = curr->node->nom;
        curr = curr->next;
    }

    affichageMonster(monster);
    while (!is_empty(turns) && player->sante > 0 && monster->sante > 0) {
        char turn = dequeue(turns);
        if (turn == 'P') {
            //char *options[] = {"Attack", "Do nothing"};
            int res = optionMenu(poptions, length);
            switch(skills[res]->class) {
                case 'A':
                    monster->sante -= skills[res]->puissance;
                    break;
            }
            enqueue(turns, 'P');
        } else {
            player->sante -= monster->puissance;
            /*moveCursor(40, 5);
            printf("player %d", player->sante);*/
            enqueue(turns, 'M');
        }
        affichagePlayer(player);
        clearMonsterAffichage();
        affichageMonster(monster);
    }
    moveCursor(30, 1);
    if (player->sante <= 0) {
        printf("You died !");
        exit(0);
    } else if (monster->sante <= 0) {
        printf("You Killed monster!");
        sleep(1);
        clearLine();
        handleLvlUp(player, monster->xp);
    }
}
void handleMonster(Player *player, Historique **head) {
    Monster *monster = player->currMap->room[player->y][player->x]->monster;
    char s1[20] = "Fight ";
    strcat(s1, monster->nom);
    char *options[] = {s1, "Escape"};
    int res = optionMenu(options, 2);
    if (res == 1) {
        undoMovement(player, head);
    } else {
        handleCombat(player,monster);
        clearMonsterAffichage();
        player->currMap->room[player->y][player->x]->type = 'E';
    }
}

void handleBoss(Player *player, Historique **head) {
    if (player->currMap->hasBoss && player->currMap->lvlLock > player->lvl) {
        char *options[] = {"You are not ready yet to meet the Boss!"};
        optionMenu(options, 1);
        undoMovement(player, head);
    } else {
        int i = 0;
        Riddle *riddles[3];
        initRiddles(riddles);
        moveCursor(OPTION_LINE - 2, 1);
        printf("Welcome hero here is my riddle for you %s", riddles[i]->question);
        int res = optionMenu(riddles[i]->opts, 3);
        moveCursor(30, 1);
        if (strcmp(riddles[i]->opts[res], riddles[i]->reponse) == 0) {
            printf("You won!");
        } else printf("You Failed!");
        exit(0);
    }
}

void interaction(Player *player, Historique **head) {
    int i = player->y;
    int j = player->x;
    Node *old = peek(*head);
    Node *curr = player->currMap->room[i][j];
    char c = curr->type; 

    switch (c) {
        case 'T':  // Arbre
            //get wood or go back
            handleTree(player, head);
            break;
        case 'R':  // riviere
            //printf("-> Une riviere bloque le passage.");
            handleRiver(player, head);
            break;
        case 'S':  // sword
        case 'W':
            // fonction pour récupérer l'item
            handleItem(player, head, c);
            break;
        case 'M':
            handleMonster(player, head);
            break;
        case 'B':
            handleBoss(player, head);
            break;
    }
}