#include "headers.h"

int countItems(Player *player) {
    Inventory *current = player->inventory;
    int compteur=0;
    while(current != NULL){
        compteur += 1;
        current = current->next;
    }
    return compteur;
}
// ajout item
int addItem(Player *player, Item *item) {
    Inventory *current = player->inventory;
    Inventory *newItem = (Inventory *) malloc(sizeof(Inventory));
    if (newItem == NULL) return 0;
    newItem->item = item;
    newItem->next = NULL;
    if (current == NULL) {
        player->inventory = newItem;
    } else {
        while (current->next) current = current->next;
        current->next = newItem;
    }
    return 1;
}
//chercher item
Item *searchItem(Player *player, char *nom_item) {
    Inventory *current = player->inventory;
    while(current != NULL){
        if(strcmp(current->item->nom, nom_item) == 0){
            return current->item;
        }
        current = current->next;
    }
    return NULL;
}
// suppression item
void deleteItem(Player *player, char *nom) {
    Inventory *current = player->inventory;
    Inventory *precedent = NULL;
    Inventory *temp = NULL;
    while (current != NULL) {
        if (strcmp(current->item->nom, nom) == 0) {
            if (precedent == NULL) {
                // Supprimer la première arme
                temp = current;
                player->inventory = current->next;
            } else {
                // Supprimer une arme au milieu ou à la fin
                precedent->next = current->next;
                temp = current;
            }
            free(temp);
            return;
        }
        precedent = current;
        current = current->next;
    }
}