//
// Created by xkalen07 on 29.09.2021.
//

// including standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// defining some constants
#define MAX_STRING_SIZE 102
#define TRUE 1
#define FALSE 0
#define STATS_NUMBER 3
#define STATS_LEN 8
#define STATS 7

/*
 * declaring multiple functions
 */
int bonus(int, char **);
int bonus_control(int);
int bonus_parse(int, char **, int *, int *, bool *);
int bonus_decide (int, int, bool, char **);
int control(const char *, int, char **, int);
int rule1(const char *);
int rule2(const char *, const char *);
int rule3(const char *, const char *);
int rule4(const char *, const char *);
int print_call(const char *, char, const char *);
int stats(const char *, int, bool [], int, int);
int stats1(const char *, int, bool [], int);
int stats2(const char *, int);
float stats3(const char *, float, float, int );
int password_browser(int argc, char ** argv, int, int);
int stats_decide(int, char **, int, int);
int control_length(const char *);
int rule4_control(const char *, int);
char sub_maker(const char *, char *, int, int);
int rule4_loop(const char *, const char *, int, int);

// initialisation of global variables required for --stats
int min = 100;
float avgc = 0;
float total = 0;

int main(int argc, char *argv[]) {
    int count = 0;
    int bns = bonus(argc, argv);
    if (bns == TRUE || bns == 10 || bns == 11) {
        return bns;
    }
    int sd = stats_decide(argc, argv, count, bns);
    if (sd != FALSE) {
        return sd;
    }
    return 0;
}

/*
 * function control() checks number of arguments, their correctness and calls for a function control_length()
 */
int control(const char *buffer, int argc, char *argv[], int bonus) {
    char *b;
    char *c;
    if (argc < 3){
        fprintf(stderr ,"Error 1: You entered less than 2 required parameters\n");
        return TRUE;
    } else if (argc > 4 && bonus == 3 ) { //&&bonus != FALSE
        fprintf(stderr, "Error 9: You have entered more than 3 possible parameters\n");
        return 9;
    }
    strtol((const char *) argv[1], &b, 10);
    strtol((const char *) argv[2], &c, 10);

    if (buffer[0] == '\n'){
        return FALSE;
    } else if (strtol(argv[1],NULL,10) > 4 ||*argv[1] < '1') {
        fprintf(stderr,"Error 2: The first parameter can be of values 1-4, you have entered %c\n", *argv[1]);
        return 2;
    } else if (*argv[2] < '1') {
        fprintf(stderr, "Error 4: The second parameter has to be a positive number and not contain additional symbols other than numbers\n");
        return 4;
    } else if (c[0] != '\0') {
        fprintf(stderr, "Error 5: The second parameter contains other symbols than numbers\n");
        return 5;
    } else if (b[0] != '\0') {
        fprintf(stderr, "Error 6: The first parameter contains other symbols than only numbers\n");
        return 6;
    } else if (control_length(buffer) > FALSE) {
        return 3;
    }

    return FALSE;
}

/*
 * This function is called by control() to check whether the password contained in buffer isn't longer than 100 symbols
 */
int control_length(const char *buffer) {
    for (int i = 0; buffer[i] != '\0'; ++i) { //loop used to find out the actual length of the content of the buffer
        char a = buffer[i];
        if (i == 100 && a != '\n') { // checking for password longer than 100 symbols
            fprintf(stderr, "Error 3: Password <%s...> is longer than 100 symbols\n", buffer);
            return 3;
        }
    }
    return FALSE;
}

/*
 * rule 1 dictates that the password has to contain at least 1 lowercase and 1 uppercase letter
 * function rule1() is called by print_call() and loops through buffer to decide about correctness of the password under rule 1
 */
int rule1(const char* buffer) {
    int i;
    int acceptance; //variable holding overall acceptance of the password according to rule1
    int acceptance_small = 0; //variable storing information whether the password contains a lowercase letter
    int acceptance_big = 0; //variable storing information whether the password contains an uppercase letter
    for (i = 0; buffer[i] != '\0'; ++i) { //loop checking for presence of a lowercase letter
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            acceptance_small++;
        }
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') { //loop checking for presence of an uppercase letter
            acceptance_big++;
        }
    }
    if (acceptance_big > 0 && acceptance_small > 0) { //decision about overall acceptance of the password under rule 1
        acceptance = TRUE;
    } else {
        acceptance = FALSE;
    }

    return acceptance;
}

/*
 * rule 2 dictates that the password has to contain at least one symbol from X groups (there are 4 groups)
 * function rule2() is called by print_call() and uses the value of PARAM to decide how many groups have to be checked for
 * accordance to rule 2 up to PARAM value of 2 is decided by the return value of rule1()
 */
int rule2 (const char* buffer, const char * PARAM) {
    int a3 = FALSE;
    int a4 = FALSE;
    int acceptance = FALSE;
    if (*PARAM == 50 || *PARAM == 49) {
        return TRUE;
    }

    if (*PARAM == 51 || *PARAM == '4') { //checking password for presence of at least 3 groups of requirements
        for (int i = 0; buffer[i] != '\0' && a3 == 0; ++i) {
            if (buffer[i] >= 48 && buffer[i] <= 57) { //checking password for numbers
                ++a3;
            }
        }
        if (a3 != FALSE) {
            acceptance++;
        }
    }

    if (*PARAM == 52 || *PARAM == 51) { //checking password for presence of signs from all 4 groups
        for (int it = 0;
             buffer[it] != '\n' && a4 == 0; ++it) { // checks for ASCII signs under codes 33-47, 58-67, 91-96, 123-126
            if (buffer[it] >= 32 && buffer[it] <= 47) {
                ++a4;
            }
            if (buffer[it] >= 58 && buffer[it] <= 64) {
                ++a4;
            }
            if (buffer[it] >= 91 && buffer[it] <= 96) {
                ++a4;
            }
            if (buffer[it] >= 123 && buffer[it] <= 126) {
                ++a4;
            }
        }
        if (a4 != FALSE && a3 != FALSE) {
            acceptance++;
        }
    }

    return acceptance;
}

/*
 * rule 3 forbids passwords which contain a sequence of the same letters, length of this sequence is dictated by value of PARAM
 * function rule3() is called by print_call()
 */
int rule3(const char* buffer, const char * PARAM) {

    int acceptance = TRUE;
    int letter_count = 1;

    for (int i = 0; buffer[i] != '\0'; ++i) {
        if (i > 0 && buffer[i] == buffer[i-1]) {
            letter_count++;
        } else if (letter_count >= (strtol(PARAM, NULL, 10))){
            acceptance = FALSE;
            return acceptance;

        }
    }
    return acceptance;
}

/*
 * rule 4 forbids passwords which contain a 2 or more instances of any substring of a length set by the value PARAM
 * function rule3() is called by print_call()
 */
int rule4(const char* buffer, const char * PARAM) {
    int string_len = *PARAM - '0';
    char chains[6];
    int ekv = 0;

    if (rule4_control(buffer, string_len) == FALSE) {
        return FALSE;
    }

    for (int u = 0; buffer[u] != '\n' && ekv < 2; u++) {
        sub_maker(buffer, chains, string_len, u);
        ekv = 0;
        ekv = rule4_loop(buffer, chains, ekv, string_len);
    }
    if (ekv < 2) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * checks whether the password isn't shorter than wanted length of rule 4 substring
 * rule4_control() is called by rule4()
 */
int rule4_control(const char * buffer, int string_len) {
    for (int max  = 0; buffer[max] != '\0' && max < string_len; max++) {
        if (max < string_len && buffer[max] == '\n')
            return FALSE;
    }
    return TRUE;
}

/*
 * a function with a loop for creation of a substring for comparison within rule4()
 * sub_maker() is called by rule4() every time it requires a new substring
 */
char sub_maker(const char * buffer, char * chains, int string_len, int u) {
    for (int i = 0; string_len > i; i++) {
        if (buffer[i + u] != '\n') {
            chains[i] = buffer[i + u];
        } else {
            chains[0] = '\0';
        }
    }
    return *chains;
}

/*
 * a loop going through the
 */
int rule4_loop(const char *buffer, const char *chains, int ekv, int string_len) {
    for (int o = string_len - 1; buffer[o] != '\n'; o++) { //looping over the password
        int match = 0;
        if (chains[string_len - 1] == buffer[o]) { //checks the location o for the presence of the last letter of the substring
            int d = string_len - 1; //for parsing through the substring
            for (int c = o; d > -1; --c) { // checks other letters in the substring
                if (chains[d] == buffer[c]) {
                    --d;
                    ++match;
                } else {
                    match = 0;
                    --d;
                }
                if (match == string_len) {
                    ++ekv;
                }
            }
        }
    }
    return ekv;
}

int print_call(const char *buffer, char LEVEL, const char * PARAM) {
    if (LEVEL == '1') {
        int r1 = rule1(buffer);//calling of the function rule1, must be inside the loop to take the current buffer
        if (r1 == TRUE) { // checking whether password is acceptable or not
            printf("%s", buffer);
        }
    } //rule 1
    if (LEVEL == '2' && strtol(PARAM, NULL, 10) <= 4) {
        int r1 = rule1(buffer);
        int r2 = rule2(buffer, PARAM);
        if (r1 == TRUE && r2 == TRUE) {
            printf("%s", buffer);
        }

    } //rule 2
    if (LEVEL == '3' && strtol(PARAM, NULL, 10) <= 4) {
        int r1 = rule1(buffer);
        int r2 = rule2(buffer, PARAM);
        int r3 = rule3(buffer, PARAM);
        if (r1 == TRUE && r2 == TRUE && r3 == TRUE) {
            printf("%s", buffer);
        }
    } //rule 3
    if (LEVEL == '4' && strtol((const char *) PARAM, NULL, 10) <= 4) {
        int r1 = rule1(buffer);
        int r2 = rule2(buffer, PARAM);
        int r3 = rule3(buffer, PARAM);
        int r4 = rule4(buffer, PARAM);
        if (r1 == TRUE && r2 == TRUE && r3 == TRUE && r4 == TRUE){
            printf("%s", buffer);
        }
    } //rule 4
    return 0;
}

int stats(const char *buffer, int NCHARS, bool chars[], int mi, int print) {
    int s1 = stats1(buffer, NCHARS, chars, print);
    int s2 = stats2(buffer, min);
    float s3 = stats3(buffer, total, avgc, print);
    if (s2 < mi) {
        min = s2;
    }
    if (print == 1) {
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", s1);
        printf("Minimalni delka: %d\n", min);
        printf("Prumerna delka: %0.1f\n", s3);
    }
    return  0;
}

int stats1(const char *buffer, int NCHARS, bool *chars, int print){
    for (int i = 0; buffer[i] != '\0'; ++i) {
        int placeholder = (int) buffer[i];
        if (chars[placeholder] == FALSE) {
            chars[placeholder] = TRUE;
        }

    }
    if (print == TRUE) {
        for (int o = 32; o < 127; ++o) {
            if (chars[o] == TRUE) {
                NCHARS++;
            }
        }
    }
    return  NCHARS;
}

int stats2(const char *buffer, int mi){
    int i;
    for (i = 0; buffer[i] != '\n'; ++i){}
    if (i < mi){
        mi = i;
    }
    return mi;
}

float stats3(const char *buffer, float TOTAL, float AVGC, int print){
    float SUM = 0;
    avgc++;
    for (int i = 0; buffer[i] != '\n'; ++i){
        total++;
    }
    if (print == 1) {
        SUM = TOTAL/AVGC;
    }
    return SUM;
}

int password_browser(int argc, char ** argv, int count, int bonus) {
    char buffer[MAX_STRING_SIZE];
    char *PARAM;
    char LEVEL;
    bool chars['~'];
    int NCHARS = 0;

    for (int t = 0; t != '~'; ++t) {
        chars[t] = FALSE;
    }

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) { //going through each password in stdin
        int c = control(buffer, argc, argv, bonus);
        if (c >= TRUE) {
            return c;
        }
            PARAM = argv[2];
            LEVEL = *argv[1];
        if (count == STATS) {
            stats(buffer, NCHARS, chars, min, FALSE);
        }

        print_call(buffer, LEVEL, PARAM);
    }
    if (count == STATS) { //prints stats after all passwords are parsed through
        stats(buffer, NCHARS, chars, min, TRUE);
    }
    return 0;
}

int stats_decide(int argc, char ** argv, int count, int bns){
    char sts[STATS_LEN] = "--stats";

    if (argc == 4 || bns == 2) {
        for (int pp = 0; argv[STATS_NUMBER][pp] != '\0'; ++pp) {
            if (sts[pp] == argv[STATS_NUMBER][pp]) {
                count++;
            } else {
                fprintf(stderr, "Error 8:<%s> is not a correct statistics call, please use <--stats>\n", argv[3]);
                return 8;
            }
        }
    }
    int p = password_browser(argc, argv, count, bns);
    if(p > 0){
        return p;
    }
    return 0;
}

int bonus(int argc, char **argv) {
    int level = FALSE;
    int *plevel = &level;
    int param = FALSE;
    int *pparam = &param;
    bool stats = FALSE;
    bool *pstats = &stats;
    int con = bonus_control(argc);
    if (con == TRUE) {
        return con;
    }
    int bss = bonus_parse(argc, argv, plevel, pparam, pstats);
    if (bss != FALSE) {
        return bss;
    }

    if (level == FALSE && param == FALSE) {
        return 3;
    }
    bonus_decide(level, param, stats, argv);
    if (stats == TRUE) {
        return 2;
    }
    return 0;
}

int bonus_control(int argc) {
    if (argc < 3) {
        fprintf(stderr, "Error 1: You entered less than 2 required parameters\n");
        return TRUE;
    }
    return 0;
}

int bonus_parse (int argc, char **argv, int *level, int *param, bool *stats) {
    for (int i = 0; i < argc; i++) {

        if (i > 0) {
            for (int d = 0; argv[i][d] != '\0'; d++) {
                if (argv[i][d] == '-') {
                    if (argv[i][d+1] == 'l') {
                        if (argc < 4) {
                            fprintf(stderr, "Error 11: You have entered a switch after a switch\n");
                            return 11;
                        }
                        *level = i+1;
                    } else if (argv[i][d+1] == 'p' ) {
                        if (argc < 4) {
                            fprintf(stderr, "Error 11: You have entered a switch after a switch\n");
                            return 11;
                        }
                        *param = i+1;
                    } else if (argv[i][d+1] == '-' && argv[i][d+2] == 's'){
                        *stats = TRUE;
                        break;
                    } else {
                        fprintf(stderr, "Error 10: You have entered a nonexistent switch\n");
                        return 10;
                    }
                }
            }
        }
    } // loop parsing arguments for -l - p
    return 0;
}

int bonus_decide (int level, int param, bool stats, char **argv) {
    if (level > 0) { // && *argv[level] >= '1' && *argv[level] <= '4'
        argv[1] = argv[level];
    } else {
        argv[1] = "1";
    }
    if (param > 0) { //&& strtol((const char *) argv[param], NULL, 10) >= 1
        argv[2] = argv[param];
    } else {
        argv[2] = "1";
    }
    if (stats == TRUE) {
        argv[3] = "--stats"; //call stats
    }
    return 0;
}
