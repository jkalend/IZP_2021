//
// Created by xkalen07 on 29.09.2021.
//

//TODO make data structs to make passing of arguments easier
//FIXME -l 2 -p throws an error

// including standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// defining some constants
#define MAX_STRING_SIZE 102
#define STATS_NUMBER 3
#define STATS_LEN 8
#define STATS 7

typedef struct {
    int acceptance;
    int acceptance1_lowercase;
    int acceptance1_uppercase;
    int accept_rule2_3;
    char chains[4];
    int ekv;
} Acceptance;


/*
 * declaring multiple functions
 */
int bonus(int, char **);
int bonus_control(int);
int bonus_parse(int, char **, int *, int *, bool *);
int bonus_decide (int, int, bool, char **);
int control(const char *, int, char **, int, long *, long *);
int rule1(const char *);
int rule2(const char *, long);
int rule3(const char *, long);
int rule4(const char *, long);
int print_call(const char *, long, long);
int stats(const char *, int, bool [], int *, float *, float *, int);
int stats1(const char *, int, bool [], int);
int stats2(const char *, int);
float stats3(const char *, float *, float *, int );
int password_browser(int argc, char ** argv, int, int);
int stats_decide(int, char **, int, int);
int control_length(const char *);
int rule4_control(const char *, long);
void sub_maker(const char *, Acceptance *, long, int);
void rule4_loop(const char *, Acceptance *, long);
int rule2_3(const char *, int *);
int rule2_4(const char *, int *);


int main(int argc, char *argv[]) {
    int count = 0;
    int bns = bonus(argc, argv);
    if (bns == true || bns == 10 || bns == 11) {
        return bns;
    }
    int sd = stats_decide(argc, argv, count, bns);
    if (sd != false) {
        return sd;
    }
    return 0;
}

/*
 * function control() checks number of arguments, their correctness and calls for a function control_length()
 */
int control(const char *buffer, int argc, char *argv[], int bonus, long *PARAM, long *LEVEL) {
    char *b;
    char *c;
    if (argc < 3){
        fprintf(stderr ,"Error 1: You entered less than 2 required parameters\n");
        return true;
    } else if (argc > 4 && bonus == 3 ) {
        fprintf(stderr, "Error 9: You have entered more than 3 possible parameters\n");
        return 9;
    }
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
int rule1(const char* buffer) {
    Acceptance accept;
    accept.acceptance1_lowercase = 0;
    accept.acceptance1_uppercase = 0;

    for (int i = 0; buffer[i] != '\0'; ++i) { //loop checking for presence of a lowercase letter
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            accept.acceptance1_lowercase++;
        }
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') { //loop checking for presence of an uppercase letter
            accept.acceptance1_uppercase++;
        }
    }

    if (accept.acceptance1_uppercase > 0 && accept.acceptance1_lowercase > 0) { //decision about overall acceptance of the password under rule 1
        accept.acceptance = true;
    } else {
        accept.acceptance = false;
    }

    return accept.acceptance;
}

/*
 * rule 2 dictates that the password has to contain at least one symbol from X groups (there are 4 groups)
 * function rule2() is called by print_call() and uses the value of PARAM to decide how many groups have to be checked for
 * accordance to rule 2 up to PARAM value of 2 is decided by the return value of rule1()
 */
int rule2 (const char* buffer, long PARAM) {
    Acceptance accept;

    if (PARAM == 1 || PARAM == 2) { //even if password would contain only lowercase letters, such password would be stopped by rule1()
        return true;
    }

    if (PARAM == 3) { //checking password for presence of at least 3 groups of requirements by looking for both special symbols and numbers
        rule2_3(buffer, &accept.acceptance);
        rule2_4(buffer, &accept.acceptance);
    }

    if (PARAM == 4) { //checking password for presence of signs from all 4 groups
        int r23 = rule2_3(buffer, &accept.acceptance);
        int r24 = rule2_4(buffer, &accept.acceptance);
        if (!(r23 == true && r24 == true)) {
            accept.acceptance = false;
        }
    }

    return accept.acceptance;
}

/*
 * rule2_3() looks for numbers in a password
 */
int rule2_3 (const char *buffer, int *acceptance) {
    int r23 = false;
    for (int i = 0; buffer[i] != '\0' && r23 == false; ++i) {
        if (buffer[i] >= '0' && buffer[i] <= '9') { //checking password for numbers
            r23 = true;
        }
    }
    if (r23 != false) {
        *acceptance = true;
    }
    return *acceptance;
}

/*
 * rule2_4() looks for special ASCII signs in a password
 */
int rule2_4 (const char *buffer, int *acceptance) {
    int r24 = false;
    for (int i = 0; buffer[i] != '\n' && r24 == false; i++) { // checks for ASCII signs under codes 33-47, 58-67, 91-96, 123-126
        if (buffer[i] >= ' ' && buffer[i] <= '/') {
            r24 = true;
        }
        if (buffer[i] >= ':' && buffer[i] <= '@') {
            r24 = true;
        }
        if (buffer[i] >= '[' && buffer[i] <= '`') {
            r24 = true;
        }
        if (buffer[i] >= '{' && buffer[i] <= '~') {
            r24 = true;
        }
    }

    if (r24 != false ) {
        *acceptance = true;
    }

    return *acceptance;
}

/*
 * rule 3 forbids passwords which contain a sequence of the same letters, length of this sequence is dictated by value of PARAM
 * function rule3() is called by print_call()
 */
int rule3(const char* buffer, long PARAM) {
    Acceptance accept;
    accept.acceptance = true;
    int letter_count = 1;

    for (int i = 0; buffer[i] != '\0'; ++i) {
        if (i > 0 && buffer[i] == buffer[i-1]) {
            letter_count++;
        } else if (letter_count >= PARAM){
            accept.acceptance = false;
            return accept.acceptance;

        }
    }

    return accept.acceptance;
}

/*
 * rule 4 forbids passwords which contain a 2 or more instances of any substring of a length set by the value PARAM
 * function rule3() is called by print_call()
 */
int rule4(const char* buffer, long PARAM) {
    long string_len = PARAM;
    Acceptance accept;
    accept.ekv = 0;


    if (rule4_control(buffer, string_len) == false) {
        return false;
    }

    for (int u = 0; buffer[u] != '\n' && accept.ekv < 2; u++) {
        sub_maker(buffer, &accept, string_len, u);
        accept.ekv = 0;
        rule4_loop(buffer, &accept, string_len);
    }

    if (accept.ekv < 2) {
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
            return false;
    }

    return true;
}

/*
 * a function with a loop for creation of a substring for comparison within rule4()
 * sub_maker() is called by rule4() every time it requires a new substring
 */
void sub_maker(const char * buffer, Acceptance *accept, long string_len, int u) {
    for (int i = 0; string_len > i; i++) {
        if (buffer[i + u] != '\n') {
            accept->chains[i] = buffer[i + u];
        } else {
            accept->chains[0] = '\0';
        }
    }
}

/*
 * a loop going through the password looking for the same substring
 */
void rule4_loop(const char *buffer, Acceptance *accept, long string_len) {
    //printf("%s\n", accept->chains);
    for (long o = string_len - 1; buffer[o] != '\n'; o++) { //looping over the password
        int match = 0;

        if (accept->chains[string_len - 1] == buffer[o]) { //checks the location o for the presence of the last letter of the substring
            long d = string_len - 1; //for parsing through the substring

            for (long c = o; d > -1; --c) { // checks other letters in the substring
                if (accept->chains[d] == buffer[c]) {
                    --d;
                    ++match;
                } else {
                    match = 0;
                    --d;
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
int print_call(const char *buffer, long LEVEL, long PARAM) {
    int r1 = true;
    int r2 = true;
    int r3 = true;
    int r4 = true;

    if (LEVEL == 1) {
        r1 = rule1(buffer);
        if (r1 == true) {
            printf("%s", buffer);
        }
    } //level 1

    if (LEVEL == 2 && PARAM <= 4) {
        r1 = rule1(buffer);
        r2 = rule2(buffer, PARAM);
        if (r1 == true && r2 == true) {
            printf("%s", buffer);
        }

    } //level 2

    if (LEVEL == 3 && PARAM <= 4) {
        r1 = rule1(buffer);
        r2 = rule2(buffer, PARAM);
        r3 = rule3(buffer, PARAM);
        if (r1 == true && r2 == true && r3 == true) {
            printf("%s", buffer);
        }
    } //level 3

    if (LEVEL == 4 && PARAM <= 4) {
        r1 = rule1(buffer);
        r2 = rule2(buffer, PARAM);
        r3 = rule3(buffer, PARAM);
        r4 = rule4(buffer, PARAM);
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
int stats(const char *buffer, int NCHARS, bool chars[], int *min, float *avgc, float *total, int print) {
    int s1 = stats1(buffer, NCHARS, chars, print);
    int s2 = stats2(buffer, *min);
    float s3 = stats3(buffer, total, avgc, print);

    if (s2 < *min) {
        *min = s2;
    }

    if (print == true) {
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", s1);
        printf("Minimalni delka: %d\n", *min);
        printf("Prumerna delka: %0.1f\n", s3);
    }

    return  0;
}
/*
 * stats1() records number of unique characters among all passwords by modifying the chars bool array
 */
int stats1(const char *buffer, int NCHARS, bool *chars, int print){

    if (print == true) {
        for (int o = 32; o < 127; ++o) {
            if (chars[o] == true) {
                NCHARS++;
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

    return  NCHARS;
}
/*
 * stats2() counts number of characters in a password and compares it with the current minimum
 * if the current password is shorter, the min value is overwritten
 */
int stats2(const char *buffer, int min){
    int i = 0;

    for (; buffer[i] != '\n'; ++i){}
    if (i < min){
        min = i;
    }

    return min;
}

float stats3(const char *buffer, float *TOTAL, float *AVGC, int print){
    float SUM = 0;

    if (print == 1) {
        SUM = *TOTAL / *AVGC;
    } else {
        *AVGC += 1;
        for (int i = 0; buffer[i] != '\n'; ++i){
            *TOTAL += 1;
        }
    }

    return SUM;
}

int password_browser(int argc, char ** argv, int count, int bonus) {
    char buffer[MAX_STRING_SIZE];
    long PARAM;
    long LEVEL;
    bool chars['~'] = {0};
    int NCHARS = 0;
    int min = 100;
    float avgc = 0;
    float total = 0;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) { //going through each password in stdin
        int c = control(buffer, argc, argv, bonus, &PARAM, &LEVEL);

        if (c >= true) {
            return c;
        }

        if (count == STATS) {
            stats(buffer, NCHARS, chars, &min, &avgc, &total, false);
        }

        print_call(buffer, LEVEL, PARAM);
    }

    if (count == STATS) { //prints stats after all passwords are parsed through
        stats(buffer, NCHARS, chars, &min, &avgc, &total, true); //FIXME structs pls
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
    int level = false;
    int param = false;
    bool stats = false;
    int con = bonus_control(argc);

    if (con == true) {
        return con;
    }

    int bss = bonus_parse(argc, argv, &level, &param, &stats);
    if (bss != false) {
        return bss;
    }

    if (level == false && param == false) {
        return 3;
    }

    bonus_decide(level, param, stats, argv);

    if (stats == true) {
        return 2;
    }

    return 0;
}

int bonus_control(int argc) {
    if (argc < 3) {
        fprintf(stderr, "Error 1: You entered less than 2 required parameters\n");
        return true;
    }

    return 0;
}

int bonus_parse (int argc, char **argv, int *level, int *param, bool *stats) { //FIXME more functions please!!
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
                        *stats = true;
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

    if (stats == true) {
        argv[3] = "--stats"; //call stats
    }

    return 0;
}
