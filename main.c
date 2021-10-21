//
// Created by xkalen07 on 29.09.2021.
//

//TODO make data structs to make passing of arguments easier
//FIXME rule 2 broke?, check param and the function itself

// including standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// defining some constants
#define MAX_STRING_SIZE 102
#define STATS_LEN 8
#define STATS 7

typedef struct {
    int acceptance;
    int acceptance1_lowercase;
    int acceptance1_uppercase;
    int accept_rule2_3;
    //char chains[100];
    int ekv;
} Acceptance;

typedef struct {
    bool stats;
    int NCHARS;
    int min;
    float avgc;
    float total;
    int print;
    float sum;
    int count;
} Stats;

/*
 * declaring multiple functions
 */
int control(const char *, char **, long *, long *);
int control_length(const char *);
int rule1(const char *, Acceptance *);
int rule2(const char *, long, Acceptance *);
int rule2_3(const char *, Acceptance *);
int rule2_4(const char *, Acceptance *);
int rule3(const char *, long, Acceptance *);
int rule4(const char *, long, Acceptance *);
int rule4_control(const char *, long);
void sub_maker(const char *, long, int, char *);
void rule4_loop(const char *, Acceptance *, long, const char *);
int print_call(const char *, long, long, Acceptance *);
int stats(const char *, Stats *, bool []);
void stats1(const char *, Stats *, bool []);
void stats2(const char *, Stats *);
int password_browser(char ** argv, Stats *);
int stats_decide(char **, Stats *, int);
int bonus(int, char **, Stats *);
int bonus_control(char **, int);
int bonus_parse(int, char **, int *, int *, Stats *);
int bonus_decide (int, int, Stats *, char **, const int *);



int main(int argc, char *argv[]) {
    Stats stat;

    int bns = bonus(argc, argv, &stat);
    if (bns > 0) {
        return bns;
    }
    /*int sd = stats_decide(argc, argv, count, bns, &stat);
    if (sd != false) {
        return sd;
    }*/
    return 0;
}

/*
 * function control() checks number of arguments, their correctness and calls for a function control_length()
 */
int control(const char *buffer, char **argv, long *PARAM, long *LEVEL) {
    char *b;
    char *c;
    /*if (argc < 3){
        fprintf(stderr ,"Error 1: You entered less than 2 required parameters\n");
        return true;
    } else if (argc > 4 && bonus == -2 ) {
        fprintf(stderr, "Error 9: You have entered more than 3 possible parameters\n");
        return 9;
    }*/
    *LEVEL = strtol((const char *) argv[1], &b, 10);
    *PARAM = strtol((const char *) argv[2], &c, 10);

    if (buffer[0] == '\n'){
        return false;
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
    } else if (control_length(buffer) > false) {
        return 3;
    }

    return false;
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

    return false;
}

/*
 * rule 1 dictates that the password has to contain at least 1 lowercase and 1 uppercase letter
 * function rule1() is called by print_call() and loops through buffer to decide about correctness of the password under rule 1
 */
int rule1(const char* buffer, Acceptance *accept) {
    accept->acceptance1_lowercase = 0;
    accept->acceptance1_uppercase = 0;

    for (int i = 0; buffer[i] != '\0'; ++i) { //loop checking for presence of a lowercase letter
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            accept->acceptance1_lowercase++;
        }
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') { //loop checking for presence of an uppercase letter
            accept->acceptance1_uppercase++;
        }
    }

    if (accept->acceptance1_uppercase > 0 && accept->acceptance1_lowercase > 0) { //decision about overall acceptance of the password under rule 1
        accept->acceptance = true;
    } else {
        accept->acceptance = false;
    }

    return accept->acceptance;
}

/*
 * rule 2 dictates that the password has to contain at least one symbol from X groups (there are 4 groups)
 * function rule2() is called by print_call() and uses the value of PARAM to decide how many groups have to be checked for
 * accordance to rule 2 up to PARAM value of 2 is decided by the return value of rule1()
 */
int rule2 (const char* buffer, long PARAM, Acceptance *accept) {
    if (accept->acceptance == false) {
        return false;
    }
    int r23;
    int r24;

    if (PARAM == 1 || PARAM == 2) { //even if password would contain only lowercase letters, such password would be stopped by rule1()
        return true;
    }

    if (PARAM == 3) { //checking password for presence of at least 3 groups of requirements by looking for both special symbols and numbers
        r23 = rule2_3(buffer, accept);
        r24 = rule2_4(buffer, accept);
        if (r23 == true || r24 == true) {
            accept->acceptance = true;
        }
    }

    if (PARAM >= 4 ) { //checking password for presence of signs from all 4 groups
        r23 = rule2_3(buffer, accept);
        r24 = rule2_4(buffer, accept);
        if (!(r23 == true && r24 == true)) {
            accept->acceptance = false;
        }
    }

    return accept->acceptance;
}

/*
 * rule2_3() looks for numbers in a password
 */
int rule2_3 (const char *buffer, Acceptance *accept) {
    accept->acceptance = false;
    for (int i = 0; buffer[i] != '\0' && accept->acceptance == false; ++i) {
        if (buffer[i] >= '0' && buffer[i] <= '9') { //checking password for numbers
            accept->acceptance = true;
        }
    }

    return accept->acceptance;
}

/*
 * rule2_4() looks for special ASCII signs in a password
 */
int rule2_4 (const char *buffer, Acceptance *accept) {
    accept->acceptance = false;
    for (int i = 0; buffer[i] != '\n' && accept->acceptance == false; i++) { // checks for ASCII signs under codes 33-47, 58-67, 91-96, 123-126
        if (buffer[i] >= ' ' && buffer[i] <= '/') {
            accept->acceptance = true;
        }
        if (buffer[i] >= ':' && buffer[i] <= '@') {
            accept->acceptance = true;
        }
        if (buffer[i] >= '[' && buffer[i] <= '`') {
            accept->acceptance = true;
        }
        if (buffer[i] >= '{' && buffer[i] <= '~') {
            accept->acceptance = true;
        }
    }

    return accept->acceptance;
}

/*
 * rule 3 forbids passwords which contain a sequence of the same letters, length of this sequence is dictated by value of PARAM
 * function rule3() is called by print_call()
 */
int rule3(const char* buffer, long PARAM, Acceptance *accept) {
    if (accept->acceptance == false) {
        return false;
    }
    accept->acceptance = true;
    int letter_count = 1;

    for (int i = 0; buffer[i] != '\0'; ++i) {
        if (i > 0 && buffer[i] == buffer[i-1]) {
            letter_count++;
        } else if (letter_count >= PARAM){
            accept->acceptance = false;
            return accept->acceptance;
        } else {
            letter_count = 1;
        }
    }

    return accept->acceptance;
}

/*
 * rule 4 forbids passwords which contain a 2 or more instances of any substring of a length set by the value PARAM
 * function rule3() is called by print_call()
 */
int rule4(const char* buffer, long PARAM, Acceptance *accept) {
    char chains[102] = {0};
    if (accept->acceptance == false) {
        return false;
    }
    long string_len = PARAM;
    accept->ekv = 0;

    if (rule4_control(buffer, string_len) == true) {
        return true;
    }

    for (int u = 0; buffer[u] != '\n' && accept->ekv < 2; u++) {
        sub_maker(buffer, string_len, u, chains);
        accept->ekv = 0;
        rule4_loop(buffer, accept, string_len, chains);
    }

    if (accept->ekv < 2) {
        return true;
    } else {
        return false;
    }
}

/*
 * checks whether the password isn't shorter than wanted length of rule 4 substring
 * rule4_control() is called by rule4()
 */
int rule4_control(const char * buffer, long string_len) {
    for (int max  = 0; buffer[max] != '\0' && max < string_len; max++) {
        if (max < string_len && buffer[max] == '\n')
            return true;
    }

    return false;
}

/*
 * a function with a loop for creation of a substring for comparison within rule4()
 * sub_maker() is called by rule4() every time it requires a new substring
 */
void sub_maker(const char * buffer, long string_len, int u, char * chains) {
    for (int i = 0; string_len > i; i++) {
        if (buffer[i + u] != '\n') {
            chains[i] = buffer[i + u];
        } else {
            chains[0] = '\0';
        }
    }
}

/*
 * a loop going through the password looking for the same substring
 */
void rule4_loop(const char *buffer, Acceptance *accept, long string_len, const char * chains) {
    //printf("%s\n", accept->chains);
    for (long o = string_len - 1; buffer[o] != '\n'; o++) { //looping over the password
        int match = 0;

        if (chains[string_len - 1] == buffer[o]) { //checks the location o for the presence of the last letter of the substring
            long d = string_len - 1; //for parsing through the substring

            for (long c = o; d > -1; c--) { // checks other letters in the substring
                if (chains[d] == buffer[c]) {
                    d--;
                    match++;
                } else {
                    match = 0;
                    d--;
                }
                if (match == string_len) {
                    accept->ekv++;
                }
            }
        }
    }
}

/*
 * calls of individual security levels and their accompanied rules
 */
int print_call(const char *buffer, long LEVEL, long PARAM, Acceptance *acceptance) {
    int r1 = true;
    int r2 = true;
    int r3 = true;
    int r4 = true;

    if (LEVEL == 1) {
        r1 = rule1(buffer, acceptance);
        if (r1 == true) {
            printf("%s", buffer);
        }
    } //level 1

    if (LEVEL == 2) {
        r1 = rule1(buffer, acceptance);
        r2 = rule2(buffer, PARAM, acceptance);
        if (r1 == true && r2 == true) {
            printf("%s", buffer);
        }

    } //level 2

    if (LEVEL == 3) {
        r1 = rule1(buffer, acceptance);
        r2 = rule2(buffer, PARAM, acceptance);
        r3 = rule3(buffer, PARAM, acceptance);
        if (r1 == true && r2 == true && r3 == true) {
            printf("%s", buffer);
        }
    } //level 3

    if (LEVEL == 4) {
        r1 = rule1(buffer, acceptance);
        r2 = rule2(buffer, PARAM, acceptance);
        r3 = rule3(buffer, PARAM, acceptance);
        r4 = rule4(buffer, PARAM, acceptance);
        if (r1 == true && r2 == true && r3 == true && r4 == true){
            printf("%s", buffer);
        }
    } //level 4

    return 0;
}

/*
 * stats() is taking care of statistics when --stats is called
 * print value decides when the statistics should be printed, value 1 is given when all passwords are parsed through
 */
int stats(const char *buffer, Stats *stat, bool chars[]) {
    stats1(buffer, stat, chars);
    stats2(buffer, stat);

    if (stat->print == true) {
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", stat->NCHARS);
        printf("Minimalni delka: %d\n", stat->min);
        printf("Prumerna delka: %0.1f\n", stat->sum);
    }

    return  0;
}

/*
 * stats1() records number of unique characters among all passwords by modifying the chars bool array
 */
void stats1(const char *buffer, Stats *stat, bool *chars) {

    if (stat->print == true) {
        for (int o = 32; o < 127; ++o) {
            if (chars[o] == true) {
                stat->NCHARS++;
            }
        }
    } else {
        for (int i = 0; buffer[i] != '\0'; ++i) {
            int value = (int) buffer[i] ;
            if (chars[value] == false) {
                chars[value] = true;
            }
        }
    }
}

/*
 * stats2() counts number of characters in a password and compares it with the current minimum
 * if the current password is shorter, the min value is overwritten
 * it also saves the total number of letters in each password into stat->total
 * and then uses stat->total and stat->avgc to calculate average length of all passwords
 */
void stats2(const char *buffer, Stats *stat) {
    int i = 0;

    if (stat->print == true) {
        stat->sum = stat->total / stat->avgc;
    }

    for (; buffer[i] != '\n'; i++){
        stat->total += 1;
    }

    stat->avgc += 1;
    if (i < stat->min){
        stat->min = i;
    }
}

int password_browser(char ** argv, Stats *stat) {
    char buffer[MAX_STRING_SIZE];
    long PARAM;
    long LEVEL;
    bool chars['~'] = {0};
    stat->min = 100;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) { //going through each password in stdin
        int c = control(buffer, argv, &PARAM, &LEVEL);

        if (c >= true) {
            return c;
        }

        if (stat->count == STATS) { //2nd layer of checking for --stats
            stats(buffer, stat, chars);
        }
        Acceptance acceptance;
        print_call(buffer, LEVEL, PARAM, &acceptance);
    }

    if (stat->count == STATS) { //prints stats after all passwords are parsed through
        stat->print = true;
        stats(buffer, stat, chars);
    }

    return 0;
}

int stats_decide(char ** argv, Stats *stat, int i) {
    char sts[STATS_LEN] = "--stats";
    stat->count = 0;

    if (stat->stats == true) {
        for (int pp = 0; argv[i][pp] != '\0'; ++pp) {
            if (sts[pp] == argv[i][pp]) {
                stat->count += 1;
            } else {
                fprintf(stderr, "Error 8:<%s> is not a correct statistics call, please use <--stats>\n", argv[i]);
                return 8;
            }
        }
    }
/*
    int p = password_browser(argc, argv, stat->count, bns, stat);

    if (p > 0) {
        return p;
    }
*/
    return 0;
}

int bonus(int argc, char **argv, Stats *stat) {
    int level = false;
    int param = false;
    stat->stats = false;

    int bp = bonus_parse(argc, argv, &level, &param, stat);
    if (bp != false) {
        return bp;
    }

    int bd = bonus_decide(level, param, stat, argv, &argc);

    if (bd != false) {
        return bd;
    }

    return 0;
}

int bonus_control (char **argv, int i) {
    int count = 0;
    for (int o = 0; argv[i][o] != '\0'; o++) {
         count++;
    }

    if (count > 2) {
        fprintf(stderr, "Error 10: You have entered a nonexistent switch %s\n", argv[i]);
        return 10;
    }

    return 0;
}

int bonus_parse (int argc, char **argv, int *level, int *param, Stats *stats) { //FIXME more functions please!!
    for (int i = 0; i < argc; i++) {
        if (i > 0) {

            for (int d = 0; argv[i][d] != '\0'; d++) {
                if (argv[i][d] == '-') {

                    if (argv[i][d+1] == 'l') {
                        if (i+1 == argc) {
                            fprintf(stderr, "Error 14: You have entered an -l switch without a value\n");
                            return 14;
                        }
                        if (argv[i+1][d] == '-') {
                            fprintf(stderr, "Error 11: You have entered a switch after a switch\n");
                            return 11;
                        }
                        int bc = bonus_control(argv, i);
                        if (bc != false) {
                            return bc;
                        }
                        *level = i+1;
                        break;

                    } else if (argv[i][d+1] == 'p' ) {
                        if (i+1 == argc) {
                            fprintf(stderr, "Error 12: You have entered a -p switch without a value\n");
                            return 12;
                        }
                        if (argv[i+1][d] == '-') {
                            fprintf(stderr, "Error 11: You have entered a switch after a switch\n");
                            return 11;
                        }
                        int bc = bonus_control(argv, i);
                        if (bc != false) {
                            return bc;
                        }
                        *param = i+1;
                        break;

                    } else if (argv[i][d+1] == '-' && argv[i][d+2] == 's'){
                        stats->stats = true;
                        int sd = stats_decide(argv, stats, i);
                        if (sd != false) {
                            return sd;
                        }
                        break;
                    } else {
                        fprintf(stderr, "Error 10: You have entered a nonexistent switch %s\n", argv[i]);
                        return 10;
                    }
                } else if ((argv[i][0] != '-' && argv[i-1][0] != '-' && argc > 4) || argv[i-1][1] == '-') {
                    fprintf(stderr ,"Error 13: You have entered a number without a switch\n");
                    return 13;
                }
            }
        }
    } // loop parsing arguments for -l - p
    return 0;
}

int bonus_decide (int level, int param, Stats *stats, char **argv, const int *argc) {
    if (level > 0) {
        argv[1] = argv[level];
    } else if (*argc <= 2 || param) {
        argv[1] = "1";
    }

    if (param > 0) {
        argv[2] = argv[param];
    } else if (*argc <= 2 || level) {
        argv[2] = "1";
    }

    int pb = password_browser(argv, stats);
    if (pb != false) {
        return pb;
    }

    /*
    if (stats->stats == true) {
        argv[3] = "--stats"; //call stats
    }*/

    return 0; //password_browse
}
