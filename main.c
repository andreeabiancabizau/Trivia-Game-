#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ESC "\033"


#define MAX_QUESTIONS 10
#define MAX_NAME 50
#define MAX_TOTAL_QUESTIONS 100
#define MAX_PLAYERS 100

typedef struct {
    // Struct for storing each quiz question, its 4 options, answer, difficulty, and hint
    char question[256];
    char options[4][100];
    int correct_answer;
    int difficulty;
    char hint[256];
} Question;

typedef struct {
    // Struct for storing player's name and final score
    char name[MAX_NAME];
    int score;
} PlayerScore;

char name[MAX_NAME];
int score;
int questions; // numărul întrebărilor rezolvate
int v[3][10];  // pentru a reține ID-urile întrebărilor în funcție de dificultate


void set_color(int color_code) { //general function for all the colors needed
    printf(ESC "[38;5;%dm", color_code);
}

void reset() { //function used in order to reset the color to the normal, boring one
    printf(ESC "[0m");
}

void red () { // decided to do two types a function, one where I have to define the 033 code
    // Example of using ANSI escape code directly for red text (method 1)
    printf("\033[1;31m");
}

void yellow (){
    printf("\033[1;33m");
}
void green() {
    printf("\033[1;32m");
}
// Example using helper function and ESC macro for 256-color pastel pink (method 2)
void pastel_pink()    { set_color(218); }  // Light pink //and the other one where I just define ESC at the beginning of the code, in order to make my life easier
void pastel_blue()    { set_color(153); }  // Baby blue
void pastel_green()   { set_color(151); }  // Mint green
void pastel_yellow()  { set_color(229); }  // Soft yellow
void pastel_purple()  { set_color(183); }  // Lavender
void pastel_orange()  { set_color(215); }  // Peach

void clear_screen() {
    printf("CLEAR\n");
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void wait_for_enter() { //// function that waits for user to press enter before continuing
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("\nPress Enter to return to the main menu...");
    getchar();
    clear_screen();
}

void show_rankings() { //function for ranking system
    clear_screen(); //clean the screen
    FILE *f = fopen("ranking.txt", "r"); //open the ranking.txt file
    if (!f) { // if we do not find anything
        printf("No ranking data found.\n");
        wait_for_enter();
        return;
    }

    PlayerScore players[MAX_PLAYERS];
    int count = 0;

    while (fscanf(f, "%s %d", players[count].name, &players[count].score) == 2) {
        count++;
        if (count >= MAX_PLAYERS) break;
    }
    fclose(f);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[i].score < players[j].score) {
                PlayerScore temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    printf("=== 🏆 RANKING 🏆 ===\n");
    for (int i = 0; i < count; i++) {
        printf("%2d. %-15s %3d points\n", i + 1, players[i].name, players[i].score);
    }

    wait_for_enter();
}

void display_question(Question q, int index, int hints_left) {
    printf("\nQuestion %d: %s\n", index + 1, q.question);
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, q.options[i]);
    }
    if (hints_left > 0) {
        printf("Type 0 to use a hint (%d left).\n", hints_left);
    }
    printf("Select an answer: ");
}

void shuffle(Question *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void show_instructions() {
    clear_screen();
     pastel_green(); printf("=== ");
    pastel_pink();  printf("HOW ");
    pastel_blue();  printf("TO ");
    pastel_green(); printf("PLAY ");
    pastel_pink();  printf("===\n");
    reset();
    pastel_blue();  printf("- ");
    pastel_green(); printf("Choose ");
    pastel_pink();  printf("a ");
    pastel_blue();  printf("difficulty ");
    pastel_green(); printf("level ");
    pastel_pink();  printf("(1 ");
    pastel_blue();  printf("= ");
    pastel_green(); printf("easy, ");
    pastel_pink();  printf("2 ");
    pastel_blue();  printf("= ");
    pastel_green(); printf("medium, ");
    pastel_pink();  printf("3 ");
    pastel_blue();  printf("= ");
    pastel_green(); printf("hard).\n");
    reset();
    pastel_blue();  printf("- ");
    pastel_pink();  printf("You’ll ");
    pastel_green(); printf("get ");
    pastel_blue();  printf("10 ");
    pastel_pink();  printf("questions ");
    pastel_green(); printf("based ");
    pastel_blue();  printf("on ");
    pastel_pink();  printf("that ");
    pastel_green(); printf("difficulty.\n");
    reset();
    pastel_blue();  printf("- ");
    pastel_green(); printf("Each ");
    pastel_pink();  printf("correct ");
    pastel_blue();  printf("answer ");
    pastel_green(); printf("gives ");
    pastel_pink();  printf("you ");
    pastel_blue();  printf("points ");
    pastel_green(); printf("equal ");
    pastel_pink();  printf("to ");
    pastel_blue();  printf("the ");
    pastel_green(); printf("difficulty ");
    pastel_pink();  printf("level.\n");
    reset();
    pastel_blue();  printf("- ");
    pastel_green(); printf("You ");
    pastel_pink();  printf("can ");
    pastel_blue();  printf("use ");
    pastel_green(); printf("2 ");
    pastel_pink();  printf("hints ");
    pastel_blue();  printf("per ");
    pastel_green(); printf("game. ");
    pastel_pink();  printf("Enter ");
    pastel_blue();  printf("0 ");
    pastel_green(); printf("when ");
    pastel_pink();  printf("answering ");
    pastel_blue();  printf("to ");
    pastel_green(); printf("use ");
    pastel_pink();  printf("one.\n");
    reset();
    pastel_pink();  printf("Enter ");
    pastel_blue();  printf("to ");
    pastel_green(); printf("return ");
    pastel_pink();  printf("to ");
    pastel_blue();  printf("the ");
    pastel_green(); printf("main ");
    pastel_pink();  printf("menu....\n");
    wait_for_enter();
}
void save_score(const char *name, int score) {
    FILE *f = fopen("ranking.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s %d\n", name, score);
        fclose(f);
    }
}

int load_questions(Question *questions) {
    FILE *file = fopen("questions.txt", "r");
    if (!file) {
        printf("Error opening questions file.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%d|%255[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|%d|%255[^\n]",
        //looks scary, took me a while to think about it but basically "d " reads the difficulty, 255 reads a string with maximum 255 charcaters,99 for the options, d for the correct answer, 255 for the hint, using | to stop it from reading other elements, prevents buffer overflow
                  &questions[count].difficulty,
                  questions[count].question,
                  questions[count].options[0],
                  questions[count].options[1],
                  questions[count].options[2],
                  questions[count].options[3],
                  &questions[count].correct_answer,
                  questions[count].hint) == 8) {
        count++;
        if (count >= MAX_TOTAL_QUESTIONS) break;
    }
    fclose(file);
    return count;
}
void save_progress(int i) {
    // Saves current progress (name, score, answered questions) to "current.txt"
    // Used for resume functionality
    FILE *file = fopen("current.txt", "w");
    if (!file) return;

    fprintf(file, "%s\n", name);
    fprintf(file, "%d\n", score);
    fprintf(file, "%d\n", i);

    if (i <= 3) {
        for (int j = 0; j < i; j++)
            fprintf(file, "%d\n", v[0][j]);
    } else if (i <= 8) {
        for (int j = 0; j < 3; j++)
            fprintf(file, "%d\n", v[0][j]);
        for (int j = 0; j < i - 3; j++)
            fprintf(file, "%d\n", v[1][j]);
    } else {
        for (int j = 0; j < 3; j++)
            fprintf(file, "%d\n", v[0][j]);
        for (int j = 0; j < 5; j++)
            fprintf(file, "%d\n", v[1][j]);
        for (int j = 0; j < i - 8; j++)
            fprintf(file, "%d\n", v[2][j]);
    }

    fclose(file);
}

void remove_progress() {
    FILE *file = fopen("current.txt", "w");
    if (file) {
        fprintf(file, "NULL");
        fclose(file);
    }
}

void new_game_continued(int start_index, Question *selected, int total) {
    int hints_left = 2;

    for (int i = start_index; i < total; i++) {
        int answer;
        while (1) {
            display_question(selected[i], i, hints_left);
            scanf("%d", &answer);
            if (answer == 0 && hints_left > 0) {
                printf("Hint: %s\n", selected[i].hint);
                hints_left--;
            } else if (answer >= 1 && answer <= 4) {
                break;
            } else {
                printf("Invalid choice. Try again.\n");
            }
        }

        if (answer == selected[i].correct_answer) {
            green();
            printf("Correct!\n");
            reset();
            score += selected[i].difficulty;
        } else {
            red();
            printf("Wrong! ");
            reset();
            printf( "Correct answer was %d.\n", selected[i].correct_answer);
        }

        if (i < total - 1)
            save_progress(i + 1);
    }

    printf("\nFinal score: %d points. Good job, %s!\n", score, name);
    save_score(name, score);
    remove_progress();
    wait_for_enter();
}

void resume_game(Question *questions_list, int total_questions) {
    FILE *file = fopen("current.txt", "r");
    if (!file) {
        printf("No saved game found.\n");
        wait_for_enter();
        return;
    }

    fgets(name, sizeof(name), file);
    strtok(name, "\n");

    if (strcmp(name, "NULL") == 0) {
        printf("No saved game to resume.\n");
        fclose(file);
        wait_for_enter();
        return;
    }

    fscanf(file, "%d\n", &score);
    fscanf(file, "%d\n", &questions);

    if (questions <= 3) {
        for (int i = 0; i < questions; i++)
            fscanf(file, "%d\n", &v[0][i]);
    } else if (questions <= 8) {
        for (int i = 0; i < 3; i++)
            fscanf(file, "%d\n", &v[0][i]);
        for (int i = 0; i < questions - 3; i++)
            fscanf(file, "%d\n", &v[1][i]);
    } else {
        for (int i = 0; i < 3; i++)
            fscanf(file, "%d\n", &v[0][i]);
        for (int i = 0; i < 5; i++)
            fscanf(file, "%d\n", &v[1][i]);
        for (int i = 0; i < questions - 8; i++)
            fscanf(file, "%d\n", &v[2][i]);
    }

    fclose(file);

    int difficulty = questions <= 3 ? 1 : (questions <= 8 ? 2 : 3);
    Question selected[MAX_QUESTIONS];
    int count = 0;

    for (int i = 0; i < total_questions && count < MAX_QUESTIONS; i++) {
        if (questions_list[i].difficulty == difficulty)
            selected[count++] = questions_list[i];
    }

    shuffle(selected, count);
    new_game_continued(questions, selected, count);
}

void start_game(Question *questions_list, int total_questions) {
    clear_screen();
    score = 0;

    // validate name
    while (1) {
        printf("Enter your name (max %d characters): ", MAX_NAME - 1);
        fgets(name, sizeof(name), stdin);
        strtok(name, "\n");
        if (strlen(name) > 0) break;
        pastel_green();
        printf("Name cannot be empty. Try again.\n");
    }

    // validate date
    int day, month, year;
    while (1) {
        printf("Enter today\'s date in format DD MM YYYY: "); // the date does not have any purpose in this code, it was just made to show the validating function for it!
        int result = scanf("%d %d %d", &day, &month, &year);
        getchar(); // consume newline
        if (result == 3 && day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 1900 && year <= 2100) {
            break;
        } else {
            printf("Invalid date. Please enter a valid date in format DD MM YYYY.\n");
        }
    }

    // validate difficulty
    int difficulty;
    while (1) {
        printf("Choose difficulty (1 = easy, 2 = medium, 3 = hard): ");
        int result = scanf("%d", &difficulty);
        getchar(); // consume newline
        if (result == 1 && difficulty >= 1 && difficulty <= 3) {
            break;
        } else {
            printf("Invalid input. Please enter 1, 2, or 3.\n");
        }
    }

    Question selected[MAX_QUESTIONS];
    int count = 0;
    for (int i = 0; i < total_questions && count < MAX_QUESTIONS; i++) {
        if (questions_list[i].difficulty == difficulty)
            selected[count++] = questions_list[i];
    }

    if (count == 0) {
        printf("No questions found for this difficulty.\n");
        wait_for_enter();
        return;
    }

    shuffle(selected, count);

    int hints_left = 2;
    for (int i = 0; i < count; i++) {
        int answer;
        while (1) {
            display_question(selected[i], i, hints_left);
            int result = scanf("%d", &answer);
            if (result != 1) {
                printf("Invalid input. Please enter a number between 1 and 4, or 0 for a hint.\n");
                while (getchar() != '\n'); // flush
                continue;
            }
            if (answer == 0 && hints_left > 0) {
                printf("Hint: %s\n", selected[i].hint);
                hints_left--;
            } else if (answer >= 1 && answer <= 4) {
                break;
            } else {
                printf("Invalid choice. Try again.\n");
            }
        }

        if (answer == selected[i].correct_answer) {
            green();
            printf("Correct!\n");
            reset();
            score += selected[i].difficulty;
        } else {
            red();
            printf("Wrong! ");
            reset();
            printf( "Correct answer was %d.\n", selected[i].correct_answer);
        }

        if (i < count - 1)
            save_progress(i + 1);
    }
    pastel_green();
    printf("\nAwesome job, %s! Final score: %d\n", name, score);
    reset();
    save_score(name, score);
    remove_progress();
    wait_for_enter();
}


void submit_suggestion() {
    clear_screen();
    char name[MAX_NAME];
    char question[256];
    char answer[100];
    char hint[256];

    printf("=== Submit a Question Suggestion ===\n");
    printf("Your name: ");
    scanf("%s", name);
    getchar();

    printf("Your question: ");
    fgets(question, sizeof(question), stdin);
    strtok(question, "\n");

    printf("Suggested correct answer: ");
    fgets(answer, sizeof(answer), stdin);
    strtok(answer, "\n");

    printf("Hint: ");
    fgets(hint, sizeof(hint), stdin);
    strtok(hint, "\n");

    FILE *f = fopen("sugestii.txt", "a");
    if (f) {
        fprintf(f, "From: %s\nQuestion: %s\nAnswer: %s\nHint: %s\n---\n",
                name, question, answer, hint);
        fclose(f);
        pastel_pink();  printf("\nThanks ");
        pastel_blue();  printf("we ");
        pastel_green(); printf("will ");
        pastel_pink();  printf("consider ");
        pastel_blue();  printf("your ");
        pastel_green(); printf("suggestion");
        pastel_pink();  printf("!\n");
    } else {
        printf("Could not save your suggestion.\n");
    }

    wait_for_enter();
}



int main() {
    srand(time(NULL));
    Question questions[MAX_TOTAL_QUESTIONS];
    int total_loaded = load_questions(questions);

    if (total_loaded == 0) {
        printf("No questions loaded. Exiting...\n");
        return 1;
    }

    int choice;
    do {

        pastel_green(); printf("=== ");
        pastel_purple(); printf("MAIN ");
        pastel_orange(); printf("MENU ");
        pastel_green(); printf("===\n");
        reset();
        pastel_pink(); printf("1.");
        reset(); printf(" ");
        pastel_blue(); printf("Start");
        reset(); printf(" ");
        pastel_green(); printf("Game");
        reset(); printf("\n");
        pastel_yellow(); printf("2.");
        reset(); printf(" ");
        pastel_purple(); printf("Continue");
        reset(); printf(" ");
        pastel_pink(); printf("Saved");
        reset(); printf(" ");
        pastel_orange(); printf("Game");
        reset(); printf("\n");
        pastel_blue(); printf("3.");
        reset(); printf(" ");
        pastel_green(); printf("How");
        reset(); printf(" ");
        pastel_yellow(); printf("to");
        reset(); printf(" ");
        pastel_purple(); printf("Play");
        reset(); printf("\n");
        pastel_orange(); printf("4.");
        reset(); printf(" ");
        pastel_blue(); printf("Show");
        reset(); printf(" ");
        pastel_green(); printf("Rankings");
        reset(); printf("\n");
        pastel_pink(); printf("5.");
        reset(); printf(" ");
        pastel_yellow(); printf("Submit");
        reset(); printf(" ");
        pastel_purple(); printf("a");
        reset(); printf(" ");
        pastel_blue(); printf("Suggestion");
        reset(); printf("\n");
        pastel_green(); printf("6.");
        reset(); printf(" ");
        pastel_orange(); printf("Exit");
        reset(); printf("\n");

        printf("Select an option: ");
        reset();
        scanf("%d", &choice);
        getchar();
        clear_screen();

        switch (choice) {
            case 1:
                start_game(questions, total_loaded);
            break;
            case 2:
                resume_game(questions, total_loaded);
            break;
            case 3:
                show_instructions();
            break;
            case 4:
                show_rankings();
            break;
            case 5:
                submit_suggestion();
            break;
            case 6:
                printf("Goodbye!\n");
            break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}
