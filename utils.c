/*
    utils definition file
*/

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern UserNode* uptr;

UserNode* CreateUserNode(User NewUser) {
    UserNode* nptr;
    nptr = (UserNode*)malloc(sizeof(UserNode));
    if( nptr != NULL ) {
        nptr->user = NewUser;
    }
    return nptr;
}

status_code InsertUserAfter(UserNode* prev, User NewUser) {
    extern UserNode* uptr;
    status_code sc = SUCCESS;
    UserNode *nptr;
    nptr = CreateUserNode(NewUser);
    if( nptr == NULL ) sc = FAILURE;
    else {  
        if( prev != NULL ) {
            nptr->next = prev->next;
            prev->next = nptr;
        }
        else { // nptr is the new first node
            nptr->next = uptr;
            uptr = nptr;
        }
    } // end of conditional: nptr != NULL
    return sc;
}

status_code AddUser(User NewUser) {
    extern UserNode* uptr;
    status_code sc = SUCCESS;

    // searching 
    UserNode *nptr, *prev;
    nptr = uptr; prev = NULL;
    Boolean found = FALSE, done = FALSE;
    while( (nptr != NULL) && !done ) {
        if( (nptr->user).user_id <= NewUser.user_id ) {
            prev = nptr;
            nptr = nptr->next;
        }
        else if( (nptr->user).user_id > NewUser.user_id ) {
            done = TRUE;
        }
    } // done searching
    
    // checking if user exists
    if( prev == NULL ) {
        found = FALSE;
    }
    else if( (prev->user).user_id == NewUser.user_id ) {
        found = TRUE;
    }
    // if not found, insert, else raise error
    if( !found ) {
        /* insert into User List */
        sc = InsertUserAfter(prev, NewUser);
        /* update family list */

    }
    else {
        sc = FAILURE;
    }
    return sc;
}

void PrintUserList() {
    extern UserNode* uptr;
    UserNode* nuptr = uptr; User user;
    printf("User List:\n");
    int i = 0;
    while( nuptr != NULL ) {
        user = nuptr->user;
        printf("User position at position %d:\n\tUserID: %d\n\tUser Name: %s\n\tIncome: %f\n", i, user.user_id, user.user_name, user.income);
        nuptr = nuptr->next; i = i + 1;
    }
}
status_code ReadUserData() {
    extern UserNode* uptr;
    status_code sc = SUCCESS;
    FILE *file_ptr = fopen("user_data.txt", "r");

    if( file_ptr == NULL ) {
        printf("\nError: file cannot be opened");
        sc = FAILURE;
    }
    else {
        User NewUser;
        char UserId[10], UserName[100], Income[50], FamilyIDstr[10], FamilyName[100];
        int FamilyID;
        while( fscanf(file_ptr, "%s %s %s %s", UserId, UserName, Income, FamilyIDstr) != EOF ) {

            // storing fields in NewUser
            FamilyName[0] = '\0'; // family name will be decided in AddUser() function
            NewUser.user_id = string_to_int(UserId);
            strcpy(NewUser.user_name, UserName);
            NewUser.income = string_to_float(Income);
            FamilyID = string_to_int(FamilyIDstr);

            sc = AddUser(NewUser);
        }
    }
    return sc;
}

// reading data
int string_to_int(char s[]) {
    int i = 0;
    int ans = 0;
    while (s[i] != '\0') {
        ans = 10*ans + (int)(s[i] - 48);
        i = i + 1;
    }
    return ans;
}
float string_to_float(char s[]) {
    // pithole: saves with some extra digits after decimal point following the least significant digits
    int i = 0;
    float whole = 0;
    float fraction = 0;
    float p_10 = 10;
    int decimal_point_flag = 0;
    if( s[0] == '.' ) {
        decimal_point_flag = 1;
        i = i + 1;
    }
    while(s[i] != '\0') {
        if( decimal_point_flag == 1 ) {
            fraction = fraction + (float)(s[i] - 48)/p_10;
            p_10 = p_10*10;
        }
        else if( s[i] == '.' ) {
            decimal_point_flag = 1;
        }
        else {
            whole = 10*whole + (float)(s[i] - 48);
        }
        i = i + 1;
    }
    return (whole+fraction);
}
