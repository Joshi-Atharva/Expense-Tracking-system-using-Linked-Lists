/*
    driver file for expense tracking system
    Author: Atharva Joshi
    BT23CSE020
*/

#include "utils.h"
#include <stdio.h>

// global variable declarations:
UserNode* uptr;
FamilyNode* fptr;
ExpenseNode* eptr;

int main() {

    // Null initialisation of linked list pointers
    uptr = NULL; fptr = NULL; eptr = NULL;

    status_code sc;
    sc = ReadUserData(); PrintStatus("ReadUserData()", sc);

    printf("Printing User List:\n"); PrintUserList();

    printf("Printing Family List:\n"); PrintFamilyList();
    return 0;
}
