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
        if (player->skillTree->left == NULL && player->skillTree->right == NULL) {
            return;
        }
        char *options[] = {player->skillTree->left->node->desc, player->skillTree->right->node->desc};
        int res = optionMenu(options, 2, "Choose a Skill");
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
    int res = optionMenu(options, 2, "");
    if (res == 0) {
        // different handling for different item types !!
        
        switch(type) {
            case 'S':
                if (player->class == 'K') {
                    addItem(player, item);
                    player->currMap->room[player->y][player->x]->type = 'E';
                    handleLvlUp(player, item->xp);
                    player->puissance = item->puissance;
                }
                else {
                    char *unable[] = {"You can't use a Sword as a mage"};
                    optionMenu(unable, 1, "");
                }
                break;
            case 'W':
                if (player->class == 'M') {
                    addItem(player, item);
                    player->currMap->room[player->y][player->x]->type = 'E';
                    handleLvlUp(player, item->xp);
                    player->puissance = item->puissance;
                }
                else {
                    char *unable[] = {"You can't use a Wand as a Knight"};
                    optionMenu(unable, 1, "");
                }
            break;
        }
    }
}
void handleTree(Player *player, Historique **head) {
    char *options[] = {"Cut wood from tree", "Do nothing"};
    int res = optionMenu(options, 2, "");
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
        int res = optionMenu(options, 2, "");
        if (res == 1) {
            undoMovement(player, head);
        } else {
            player->currMap->room[player->y][player->x]->type = '#';
            deleteItem(player, "Wood");
            affichagePlayer(player);
        }
    } else {
        char *options[] = {"You can't cross the river unless you've got wood from trees!"};
        optionMenu(options, 1, "");
        undoMovement(player, head);
    }
}

void skillOnCooldown(Queue *turns) {
    moveCursor(OPTION_LINE, 1);
    printf("skill on cooldown");
    sleep(1);
    clearLine();
    dequeue(turns);
    enqueue(turns, 'P');
    enqueue(turns, 'M');
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
    int poison = 0;
    int reflect = 0;
    int q = 1;
    int defense = 0;
    int boost = 0 ;
    int skip = 0;
    int i = 5;
    int addedefense,addedpuissace,addedarmor,addedsante;
    while (!is_empty(turns) && player->sante > 0 && monster->sante > 0) {
        if(!q) q = 1;
        char turn = dequeue(turns);
        if (turn == 'P') {
            //char *options[] = {"Attack", "Do nothing"};
            int res = optionMenu(poptions, length, "Your Turn");
            switch(skills[res]->class) {
                case 'A':
                    monster->sante -= (skills[res]->puissance) + (player->puissance);
                    break;
                case 'D':
                    if(defense == 0 ){
                      player->armor += skills[res]->armor;
                      addedefense = skills[res]->armor;
                      defense = 1;
                    }
                    else skillOnCooldown(turns);
                    break;
                case 'S':
                    if(skip == 0){
                    dequeue(turns);
                    enqueue(turns, 'P');
                    enqueue(turns, 'M');
                     q = 0;
                     skip = 3;
                    }
                    else skillOnCooldown(turns);
                    break;
                case 'B':
                    if(boost ==0 ){
                    player->puissance += skills[res]->puissance;
                    addedpuissace = skills[res]->puissance ;
                    player->armor += skills[res]->armor;
                    addedarmor = skills[res]->armor;
                    player->sante += skills[res]->santemax;
                    addedsante = skills[res]->santemax; 
                    boost = 1;
                    }
                    else skillOnCooldown(turns);
                    break;
                case 'U':
                    int hitchance = rand() % 4;
                        if(!hitchance)
                            monster->sante -= skills[res]->puissance; 
                        else {
                            moveCursor(OPTION_LINE, 1);
                            printf("You missed the enemy");
                        }
                        break;
                case 'E':
                        if(strcmp(poptions[res],"Parry") == 0){
                            reflect = 1;
                        }
                        else if(strcmp(poptions[res],"Death blade") == 0)
                                poison = 3;
                        break;
            }
            if(q) enqueue(turns, 'P');
        } else {
            if(poison > 0){
                monster->sante -= 10;
                poison--;
            }
             if(!reflect){
                if(skip > 0) skip --;
                player->sante -= monster->puissance;
            }
            else {
                monster->sante -= (monster->puissance - i);
                reflect = 0;
                i += 2;
            }
            /*moveCursor(40, 5);
            printf("player %d", player->sante);*/
            enqueue(turns, 'M');
        }
        affichagePlayer(player);
        clearAffichageMonster();
        affichageMonster(monster);
    }
    moveCursor(30, 1);
    if (player->sante <= 0) {
        gameOver(0);
        exit(0);
    } else if (monster->sante <= 0) {
        if(defense == 1) player->armor -=  addedefense;
    if(boost == 1){
        player->puissance -= addedpuissace;
        player->armor -= addedarmor;
        player->sante -= addedsante;
    }
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
    int res = optionMenu(options, 2, "You meet a Monster!");
    if (res == 1) {
        undoMovement(player, head);
    } else {
        handleCombat(player,monster);
        clearAffichageMonster();
        player->currMap->room[player->y][player->x]->type = 'E';
    }
}

void handleBoss(Player *player, Historique **head) {
    if (player->currMap->hasBoss && player->currMap->lvlLock > player->lvl) {
        char *options[] = {"You are not ready yet to meet the Boss!"};
        optionMenu(options, 1, "");
        undoMovement(player, head);
    } else {
        int i = rand() % NUM_RIDDLES;
        Riddle *riddles[3];
        initRiddles(riddles);
        moveCursor(OPTION_LINE - 2, 1);
        printf("Welcome hero we finally met here is my riddle for you");
        int res = optionMenu(riddles[i]->opts, 3, riddles[i]->question);
        moveCursor(OPTION_LINE - 2, 1);
        clearLine();
        moveCursor(30, 1);
        if (strcmp(riddles[i]->opts[res], riddles[i]->reponse) == 0) {
            gameOver(1);
        } else gameOver(0);
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