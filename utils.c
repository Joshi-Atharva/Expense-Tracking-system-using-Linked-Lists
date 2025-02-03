/*
    utils definition file
*/

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

status_code AddUser(User NewUser, int FamilyID) {
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
        sc = InsertUserToFamily(NewUser, FamilyID);

    }
    else {
        sc = FAILURE;
    }
    return sc;
}

void FamilyNameGenerator(User NewUser, char* s) {
    int i = 0;
    while( (NewUser.user_name)[i] != '\0' ) {
        s[i] = (NewUser.user_name)[i];
        i = i + 1;
    }
    char suffix[] = "'s family\0";
    int j = 0;
    while( suffix[j] != '\0' ) {
        s[i] = suffix[j];
        i = i + 1;
        j = j + 1;
    }
    s[i] = '\0';
}

// to be written
float GetMonthlyUserExpense(User user, int month) {
    float ret_val = 0;
    if( (1 <= month) && (month <= 12) ) {
    }
    else {
    }
    return ret_val;
}
        
FamilyNode* CreateFamilyNode(User NewUser, int FamilyID) {
    extern FamilyNode* fptr;
    FamilyNode* nfptr = (FamilyNode*)malloc(sizeof(FamilyNode));

    if( FamilyID == 0 ) { // Traverses family list for LastFamilyID to generate unique family_id 
        FamilyNode* ptr = fptr; int LastFamilyID = 0;
        while( ptr != NULL ) {
            LastFamilyID = (ptr->family).family_id;
            ptr = ptr->next;
        }
        FamilyID = LastFamilyID + 1;
    }

    if( nfptr != NULL ) { // constructs a new FamilyNode
        (nfptr->family).family_id = FamilyID;
        char s[NAME_SIZE];
        FamilyNameGenerator(NewUser, s);
        strcpy((nfptr->family).family_name, s);
        (nfptr->family).members[0] = NewUser.user_id;
        for(int i = 1; i < 4; i++ ) {
            (nfptr->family).members[i] = 0;
        }
        (nfptr->family).total_family_income = NewUser.income;
        for(int i = 0; i < 12; i++ ) {
            (nfptr->family).monthly_family_expense[i] = GetMonthlyUserExpense(NewUser, i+1);
        }
        
        nfptr->next = NULL; 
    }
    return nfptr;
}

        
status_code InsertUserToFamily(User NewUser, int FamilyID) {
    extern FamilyNode* fptr; status_code sc = SUCCESS;

    if( FamilyID == 0 ) { // New family to be created
        FamilyNode* nfptr = fptr, *fprev = NULL;

        // traversing to find last position
        while( nfptr != NULL ) {
            fprev = nfptr;
            nfptr = nfptr->next;
        }
        if( fprev == NULL ) { // Family list is empty, fptr to be updated
            nfptr = CreateFamilyNode(NewUser, FamilyID);
            if( nfptr == NULL ) sc = FAILURE;
            else {
                fptr = nfptr;
            }
        }
        else { // fprev != NULL
            nfptr = CreateFamilyNode(NewUser, FamilyID);
            if( nfptr == NULL ) sc = FAILURE;
            fprev->next = nfptr;
        }
    }
    else { // FamilyID != 0
        // searching 
        FamilyNode* ptr = fptr, *prev = NULL;
        Boolean done = FALSE;
        while( (ptr != NULL) && !done ) {
            if( (ptr->family).family_id <= FamilyID ) {
                prev = ptr;
                ptr = ptr->next;
            }
            else { // (ptr->family).family_id > FamilyID
                done = TRUE;
            }
        }
        Boolean found = FALSE;
        if( prev == NULL ) {
            found = FALSE;
        }
        else if( (prev->family).family_id == FamilyID ) {
            found = TRUE;
        }

        if( found ) { 
            int i = 0;
            while( (prev->family).members[i] != 0 && (i < 4)) i = i + 1;
            if( i >= 4 ) { // family full - Create new Family
                InsertUserToFamily(NewUser, 0);
            }
            else { // i < 4 hence update family
                (prev->family).members[i] = NewUser.user_id;
                (prev->family).total_family_income += NewUser.income;
                for( int j = 0; j < 12; j++ ) {
                    (prev->family).monthly_family_expense[j] += GetMonthlyUserExpense(NewUser, j);
                }
            }

        }
        else { // family with given FamilyID not found - Create new Family
            FamilyNode* nfptr = CreateFamilyNode(NewUser, FamilyID);

            if( nfptr == NULL ) sc = FAILURE;
            else { // nfptr != NULL
                if( prev != NULL ) {
                    prev->next = nfptr;
                    nfptr->next = ptr;
                }
                else { // prev == NULL ie New node is to be added at first position - hence update fptr
                    nfptr->next = fptr;
                    fptr = nfptr;
                }
            }

        }

    }
    return sc;
}

void PrintFamilyList() {
    extern FamilyNode* fptr;
    FamilyNode* nfptr = fptr; Family family;
    printf("Family list:\n");
    int i = 0;
    while( nfptr != NULL ) {
        family = nfptr->family;
        printf("Family at position %d:\n"
            "\tFamilyID: %d\n"
            "\tFamily Name: %s\n"
            "\tMembers:\n"
                "\t\tUserID: %d\n"
                "\t\tUserID: %d\n"
                "\t\tUserID: %d\n"
                "\t\tUserID: %d\n"
            "\tTotal Family Income: %f\n"
            "\tTotal Monthly Expense: \n", i, family.family_id, family.family_name, 
            family.members[0], family.members[1], family.members[2], family.members[3], 
            family.total_family_income
        );
        nfptr = nfptr->next;
        i = i + 1;
    }
}



void PrintUserList() {
    extern UserNode* uptr;
    UserNode* nuptr = uptr; User user;
    printf("User List:\n");
    int i = 0;
    while( nuptr != NULL ) {
        user = nuptr->user;
        printf("User at position %d:\n"
            "\tUserID: %d\n"
            "\tUser Name: %s\n"
            "\tIncome: %f\n", i, user.user_id, user.user_name, user.income);
        nuptr = nuptr->next; i = i + 1;
    }
}
void PrintStatus(const char* opcode, status_code sc) {
    if( sc == SUCCESS ) {
        printf("Operation: %s successfull\n", opcode);
    }
    else {
        printf("Operatin: %s failed\n", opcode);
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

            sc = AddUser(NewUser, FamilyID);
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
