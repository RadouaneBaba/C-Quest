#include "headers.h"

/**
 * drawings of different possible menus
 */
void printTitle() {
    hideCursor();
    printf("\n");
    printf("\033[1;34m"); // Bleu
    moveCursor(3,50);
    printf("   ____    ____     _    __  __   _____ \n");
    sleep(1); // Délai de 1 seconde
    moveCursor(4,50);
    printf("  / ___|  / ___|   / \\  |  \\/  | | ____|\n");
    sleep(1);
    printf("\033[1;32m"); // Vert
    moveCursor(5,50);
    printf(" | |     | |  _   / _ \\ | |\\/| | |  _|  \n");
    sleep(1);
    printf("\033[1;33m"); // Jaune
    moveCursor(6,50);
    printf(" | |___  | |_| | / ___ \\| |  | | | |___ \n");
    sleep(1);
    printf("\033[1;31m"); // Rouge
    moveCursor(7,50);
    printf("  \\____|  \\____|/_/   \\_\\_|  |_| |_____|\n");
    sleep(1);
    printf("\n");
    printf("\033[0m"); // Réinitialiser la couleur
    moveCursor(9,54);
}

void printYouWon() {
    printf("\n");
    printf("\033[1;32m"); // Vert
    moveCursor(4,35);
    printf(" _     _ ______  _      _    _             _ ______  _    _  \n");
    sleep(1); // Délai de 1 seconde
    moveCursor(5,35);
    printf(" \\\\   //| ____ || |    | |  \\ \\    __    / /| ____ || \\  | |           \n");
    sleep(1); // Délai de 1 seconde
    moveCursor(6,35);
    printf("  \\\\_// ||    ||| |    | |   \\ \\  /  \\  / / ||    |||  \\ | |              \n");
    sleep(1); // Délai de 1 seconde
    moveCursor(7,35);
    printf("   | |  ||____||| |____| |    \\ \\/ /\\ \\/ /  ||____||| |\\\\| |                 \n");
    sleep(1); // Délai de 1 seconde
    moveCursor(8,35);
    printf("   |_|  |______||________|     \\_ /  \\_ /   |______||_| \\__|             \n");
    printf("\n");
    printf("\033[0m"); // Réinitialiser la couleur
    moveCursor(9,57);
    exit(0);
}


void printGameOver() {
    printf("\n");
    printf("\033[1;31m"); // Rouge
    moveCursor(3,35);
    printf("  ____     _    __  __  _____    ______ _     _  _____  _____ ");
    sleep(1); // Délai de 1 seconde
    moveCursor(4,35);
    printf(" / ___|   / \\  |  \\/  || ____|  |  __  |\\ \\  / /| ____||     |");
    sleep(1); // Délai de 1 seconde
    moveCursor(5,35);
    printf("| |  _   / _ \\ | |\\/| || __|    | |  | | \\ \\/ / |  _|  |__ __|");
    sleep(1); // Délai de 1 seconde
    moveCursor(6,35);
    printf("| |_| | / ___ \\| |  | |||___    | |__| |  \\  /  | |___ | | \\ \\");
    sleep(1); // Délai de 1 seconde
    moveCursor(7,35);
    printf(" \\____|/_/   \\_\\_|  |_||_____|  |______|   \\/   |_____||_|  \\_\\");
    printf("\n");
    printf("\033[0m"); // Réinitialiser la couleur
    moveCursor(9,57);
    hideCursor();
    sleep(2);
    exit(0);
}

/**
 * display menus and returning everything to default
 */

void startMenu() {
    clearTerm();
    printTitle();
    printf("Press [S] to start or [Q] to quit...");
    char c;
    do {
        c = toupper(_getch());
        sleep(0.01);
    } while (c != 'S' && c != 'Q');

    if (c == 'Q') exit(0);
    clearTerm();
}
void gameOver(int won) {
    clearTerm();
    if (won) printYouWon();
    else printGameOver();
    showCursor();
    exit(0);
}