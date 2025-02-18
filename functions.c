#include "headers.h"


// ANSI functions
void hideCursor() {
    printf("\033[?25l");
}

void showCursor() {
    printf("\033[?25h");
}

void moveCursor(int x, int y) {
    printf("\033[%d;%dH", x, y);
}

void clearTerm() {
    system("cls");
    //printf("\033[2J");
}
void clearLine() {
    printf("\033[2K");
}

// helper funcitons
char *str_alloc(char *src) {
    char *dest = malloc(strlen(src) + 1);
    if (!dest) {
        printf("Memory allocation failed");
        exit(1);
    }
    strcpy(dest, src);
    return dest;
}

// les Piles

void empiler(Historique **head,Node *noeud){
    Historique *l;
    l=(Historique*)malloc(sizeof(Historique));
    l->noeud=noeud;
    l->next=*head;
    *head=l;
}
void dipiler(Historique **head){
    if(!head) return;
    Historique *tmp;
    tmp=*head;
    *head = (*head)->next;
    free(tmp);
}

Node *peek(Historique *head){
    if(head == NULL) return NULL;
    return head->noeud;
}

// Les files


// Function to initialize an empty queue
void init_queue(Queue *q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

// Function to check if the queue is empty
int is_empty(Queue *q) {
    return q->size == 0;
}

// Function to enqueue (add) a character to the queue
void enqueue(Queue *q, char data) {
    Elem *newNode = (Elem *)malloc(sizeof(Elem));
    if (!newNode) {
        perror("erreur memoire");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (is_empty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

// Function to dequeue (remove) a character from the queue
char dequeue(Queue *q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return '\0';
    }

    Elem *temp = q->front;
    char data = temp->data;

    q->front = q->front->next; // Move front pointer to the next node
    free(temp);

    if (!q->front) { // If queue becomes empty, update rear as well
        q->rear = NULL;
    }

    q->size--;
    return data;
}
// delete queue
void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
}

char peekq(Queue *q) {
    if (is_empty(q)) {
        return '\0';
    }
    return q->front->data;
}

// options function
/**
 * shows option menu and return index of choosen option by pressing enter
 */
int optionMenu(char *options[], int size) {
    int k = 0;
    moveCursor(OPTION_LINE, 1);
    printf("---->");
    for (int i = 0; i < size; i++) {
        moveCursor(OPTION_LINE + i, 7);
        printf("%s", options[i]);
    }
    while (1) {
        if (kbhit()) {
            char input = getch();
            //moveCursor(5, 1);
            //printf("%c %d", input, input);
            if (input == 0 || input == -32) {
                switch(getch()) {
                    case 72:
                        
                        if (k > 0) {
                            moveCursor(OPTION_LINE + k, 1);
                            printf("     ");
                            k--;
                            moveCursor(OPTION_LINE + k, 1);
                            printf("---->");
                        }
                        break;
                    case 80:
                        if (k < size - 1) {
                            moveCursor(OPTION_LINE + k, 1);
                            printf("     ");
                            k++;
                            moveCursor(OPTION_LINE + k, 1);
                            printf("---->");
                        }
                        break;
                }
            } else if (input == '\r') {
                for (int i = OPTION_LINE; i < OPTION_LINE + size; i++) {
                    moveCursor(i, 1);
                    clearLine();
                }
                return k;
            }
        }
    }
    
}


// tree functions

SkillNode *createSkillNode(int puissance, int maxsante, int armor, char *name, char *desc, char class) {
    SkillNode *node = (SkillNode *)malloc(sizeof(SkillNode));
    if (node == NULL) {
        printf("Failed to allocate");
        exit(1);
    }
    node->puissance = puissance;
    node->santemax = maxsante;
    node->armor = armor;
    node->nom = str_alloc(name);
    node->desc = str_alloc(desc);
    node->class = class;
    return node;
}

SkillTree *createTree(int puissance, int maxsante, int armor, char *name, char *desc, char class) {
    SkillTree *tree = (SkillTree *)malloc(sizeof(SkillTree));
    if (tree == NULL) {
        printf("Failed to allocate");
        exit(1);
    }
    SkillNode *node = createSkillNode(puissance, maxsante, armor, name, desc, class);
    tree->node = node;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

int addSkill(Player *player, SkillNode *skill) {
    SkillSet *current = player->skillSet;
    SkillSet *newSkill = (SkillSet *) malloc(sizeof(SkillSet));
    if (newSkill == NULL) return 0;
    newSkill->node = skill;
    newSkill->next = NULL;
    if (current == NULL) {
        player->skillSet = newSkill;
    } else {
        while (current->next) current = current->next;
        current->next = newSkill;
    }
    return 1;
}

int skillsetLength(SkillSet *skillSet) {
    if (skillSet == NULL) return 0;
    SkillSet *curr = skillSet;
    int length = 0;
    while (curr) {
        curr = curr->next;
        length++;
    }
    return length;
}