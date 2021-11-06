# My first IZP project

The purprose of this code is to filter passwords according to 4 rules

Rule 1 - The password has to contain at least 1 uppercase and 1 lowercase letter
Rule 2 - Contains 4 groups, group 1: lowercase letters, group 2: uppercase letters, group 3: numbers, group 4: special ASCII characters
Rule 3 - The password can not contain X same consecutive characters
Rule 4 - The password can not contain any substring of length X more than once

Started by: `$ ./pwcheck [-l LEVEL] [-p PARAM] [--stats] <passwords.txt`

LEVEL = an integer from intereval 1-4
PARAM = a positive integer
