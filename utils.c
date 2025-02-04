/*
    utils definition file
*/

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// helper functions
// to be written
float GetMonthlyUserExpense(int UserID, int month) {
    float ret_val = 0;
    extern ExpenseNode* elptr;
    ExpenseNode* eptr = elptr; Boolean done = FALSE; Boolean found = FALSE;
    while( eptr != NULL && !done ) {
        if( (eptr->expense).user_id == UserID ) {
            found = TRUE;
            done = TRUE;
        }
        else if( (eptr->expense).user_id > UserID ) {
            done = TRUE;
        }
        else {
            eptr = eptr->next;
        }
    }
    if( found ) {
        done = FALSE;
        while( eptr != NULL && !done ) {
            if( (eptr->expense).user_id > UserID) {
                done = TRUE;
            }
            else {
                char month_str[3];
                for(int i = 0; i < 2; i++) month_str[i] = (eptr->expense).date_of_expense[3+i];
                if( string_to_int(month_str) == month ) {
                    ret_val = ret_val + (eptr->expense).expense_amount;
                }
                eptr = eptr->next;
            }
        }
    }
    return ret_val;
}
// Similar to e1 - e2 convention: -ve if e1 < e1, 0 if e1 = e2 and +ve if e2 > e2
int CompareExpenses(Expense e1, Expense e2) {
    int ret_val;
    if( e1.user_id < e2.user_id ) {
        ret_val = -1;
    }
    else if( e1.user_id > e2.user_id ) {
        ret_val = 1;
    }
    else { /* e1.user_id == e1.user_id */
        if( e1.expense_id < e2.expense_id ) {
            ret_val = -1;
        }
        else if( e1.expense_id > e2.expense_id ) {
            ret_val = 1;
        }
        else {
            ret_val = 0;
        }
    }
    return ret_val;
}

// display:
void PrintTotalMonthlyFamilyExpense(Family family) {
    int month = 1; float sum = 0;
    while( month <= 12 ) {
        sum = 0;
        for( int i = 0; i < 4; i++ ) {
            sum += GetMonthlyUserExpense(family.members[i], month);
        }
        printf("\t\tMonth %d: %f\n", month, sum);
        month = month + 1;
    }
}


void PrintFamilyList() {
    extern FamilyNode* flptr;
    FamilyNode* fnptr = flptr; Family family; float monthly_family_expense;
    printf("Family list:\n");
    int i = 0;
    while( fnptr != NULL ) {
        family = fnptr->family;
        printf("Family at position %d:\n"
            "\tFamilyID: %d\n"
            "\tFamily Name: %s\n"
            "\tMembers:\n"
                "\t\tUserID: %d\n"
                "\t\tUserID: %d\n"
                "\t\tUserID: %d\n"
                "\t\tUserID: %d\n"
            "\tTotal Family Income: %f\n"
            "\tTotal Monthly Expense:\n", i, family.family_id, family.family_name, 
            family.members[0], family.members[1], family.members[2], family.members[3], 
            family.total_family_income
        );
        PrintTotalMonthlyFamilyExpense(family);
        fnptr = fnptr->next;
        i = i + 1;
    }
}
void PrintUserList() {
    extern UserNode* ulptr;
    UserNode* unptr = ulptr; User user;
    printf("User List:\n");
    int i = 0;
    while( unptr != NULL ) {
        user = unptr->user;
        printf("User at position %d:\n"
            "\tUserID: %d\n"
            "\tUser Name: %s\n"
            "\tIncome: %f\n", i, user.user_id, user.user_name, user.income);
        unptr = unptr->next; i = i + 1;
    }
}
void PrintExpenseList() {
    extern ExpenseNode* elptr;
    ExpenseNode* eptr = elptr; Expense expense; int i = 0;
    while( eptr != NULL ) {
        expense = eptr->expense;
        printf("Expense at position %d:\n"
            "\tExpense ID: %d\n"
            "\tUser ID: %d\n"
            "\tExpense Category: %s\n"
            "\tExpense amount: %f\n"
            "\tDate of expense: %s\n", i, 
            expense.expense_id, expense.user_id, expense.expense_category, expense.expense_amount, expense.date_of_expense
        );
        eptr = eptr->next; i = i + 1;
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

// micro utilities
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
Expense MakeExpense(int exp_id, int user_id, const char* exp_cat, float exp_amt, const char* exp_date) {
    Expense NewExpense;
    NewExpense.expense_id = exp_id;
    NewExpense.user_id = user_id;
    strcpy(NewExpense.expense_category, exp_cat);
    NewExpense.expense_amount = exp_amt;
    strcpy(NewExpense.date_of_expense, exp_date);
    return NewExpense;
}

// Creation:
FamilyNode* CreateFamilyNode(User NewUser, int FamilyID) {
    extern FamilyNode* flptr;
    FamilyNode* fnptr = (FamilyNode*)malloc(sizeof(FamilyNode));

    if( FamilyID == 0 ) { // Traverses family list for LastFamilyID to generate unique family_id 
        FamilyNode* ptr = flptr; int LastFamilyID = 0;
        while( ptr != NULL ) {
            LastFamilyID = (ptr->family).family_id;
            ptr = ptr->next;
        }
        FamilyID = LastFamilyID + 1;
    }

    if( fnptr != NULL ) { // constructs a new FamilyNode
        (fnptr->family).family_id = FamilyID;
        char s[NAME_SIZE];
        FamilyNameGenerator(NewUser, s);
        strcpy((fnptr->family).family_name, s);
        (fnptr->family).members[0] = NewUser.user_id;
        for(int i = 1; i < 4; i++ ) {
            (fnptr->family).members[i] = 0;
        }
        (fnptr->family).total_family_income = NewUser.income;
        for(int i = 0; i < 12; i++ ) {
            (fnptr->family).monthly_family_expense[i] = GetMonthlyUserExpense(NewUser.user_id, i+1);
        }
        
        fnptr->next = NULL; 
    }
    return fnptr;
}
UserNode* CreateUserNode(User NewUser) {
    UserNode* unptr;
    unptr = (UserNode*)malloc(sizeof(UserNode));
    if( unptr != NULL ) {
        unptr->user = NewUser;
        unptr->next = NULL;
    }
    return unptr;
}
ExpenseNode* CreateExpenseNode(Expense NewExpense) {
    ExpenseNode* enptr = (ExpenseNode*)malloc(sizeof(ExpenseNode));
    if( enptr != NULL ) {
        enptr->expense = NewExpense; // Bitwise copying exptected
        enptr->next = NULL;
    }
    return enptr;
}

// Searching:
Boolean UserInFamily(Family family, int UserID) {
    int i = 0; Boolean found = FALSE;
    while( i < 4 && !found ) {
        if( family.members[i] == UserID ) {
            found = TRUE;
        }
        else {
            i = i + 1;
        }
    }
    return found;
}
FamilyNode* FamilyOfUser(int UserID) {
    extern FamilyNode* flptr;
    FamilyNode* ret_fptr;
    FamilyNode* fptr = flptr; Boolean found = FALSE;
    while( fptr != NULL && !found) {
        if( UserInFamily(fptr->family, UserID) ) {
            found = TRUE;
        }
        else {
            fptr = fptr->next;
        }
    }
    if( found ) {
        ret_fptr = fptr;
    }
    else {
        ret_fptr = NULL;
    }
    return ret_fptr;
}


// Insertion:
status_code InsertUserAfter(UserNode* prev, User NewUser) {
    extern UserNode* ulptr;
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
            nptr->next = ulptr;
            ulptr = nptr;
        }
    } // end of conditional: nptr != NULL
    return sc;
}
status_code InsertUserToFamily(User NewUser, int FamilyID) {
    extern FamilyNode* flptr; status_code sc = SUCCESS;

    if( FamilyID == 0 ) { // New family to be created
        FamilyNode* fnptr = flptr, *fprev = NULL;

        // traversing to find last position
        while( fnptr != NULL ) {
            fprev = fnptr;
            fnptr = fnptr->next;
        }
        if( fprev == NULL ) { // Family list is empty, flptr to be updated
            fnptr = CreateFamilyNode(NewUser, FamilyID);
            if( fnptr == NULL ) sc = FAILURE;
            else {
                flptr = fnptr;
            }
        }
        else { // fprev != NULL
            fnptr = CreateFamilyNode(NewUser, FamilyID);
            if( fnptr == NULL ) sc = FAILURE;
            fprev->next = fnptr;
        }
    }
    else { // FamilyID != 0
        // searching 
        FamilyNode* ptr = flptr, *prev = NULL;
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
                    (prev->family).monthly_family_expense[j] += GetMonthlyUserExpense(NewUser.user_id, j);
                }
            }

        }
        else { // family with given FamilyID not found - Create new Family
            FamilyNode* fnptr = CreateFamilyNode(NewUser, FamilyID);

            if( fnptr == NULL ) sc = FAILURE;
            else { // fnptr != NULL
                if( prev != NULL ) {
                    prev->next = fnptr;
                    fnptr->next = ptr;
                }
                else { // prev == NULL ie New node is to be added at first position - hence update flptr
                    fnptr->next = flptr;
                    flptr = fnptr;
                }
            }

        }

    }
    return sc;
}
status_code AddExpense(Expense NewExpense) {
    extern ExpenseNode* elptr; status_code sc = SUCCESS;
    /* searching */
    ExpenseNode *eprev, *eptr;
    Expense exp; Boolean done = FALSE;
    eprev = NULL; eptr = elptr;
    while( eptr != NULL && !done ) {
        if( CompareExpenses(eptr->expense, NewExpense) <= 0 ) {
            eprev = eptr; eptr = eptr->next;
        }
        else { /* eptr->expense > NewExpense */
            done = TRUE;
        }
    }

    /* Insertion */
    if( eprev == NULL ) { /* new node will be first node */
        ExpenseNode *enptr = CreateExpenseNode(NewExpense);
        if( enptr == NULL ) sc = FAILURE;
        else {
            enptr->next = elptr;
            elptr = enptr;
        }
    }
    else { /* eprev != NULL => either entry found or correct position of insertion after eprev */
        if( CompareExpenses(NewExpense, eprev->expense) == 0 ) { // expense already exists
            sc = FAILURE;
        }
        else { // expense doesn't exist => New expense to be inserted
            ExpenseNode *enptr = CreateExpenseNode(NewExpense);
            if( enptr == NULL ) sc = FAILURE;
            else {
                eprev->next = enptr;
                enptr->next = eptr;
            }
        }
    }
    
    /* Updation */
    FamilyNode* fptr = FamilyOfUser(NewExpense.user_id);
    if( fptr == NULL ) sc = FAILURE;
    else {
        char month_str[3];
        for( int i = 0; i < 2; i++ ) {
            month_str[i] = (NewExpense.date_of_expense)[3+i];
        }
        int month = string_to_int(month_str);
        (fptr->family).monthly_family_expense[month] += NewExpense.expense_amount;
    }
    return sc;
}


status_code ReadUserData() {
    extern UserNode* ulptr;
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
        while( (sc == SUCCESS) && fscanf(file_ptr, "%s %s %s %s", UserId, UserName, Income, FamilyIDstr) != EOF ) {

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
status_code ReadExpenseData() {
    status_code sc = SUCCESS;
    FILE *file_ptr = fopen("expense_data.txt", "r");
    if( file_ptr == NULL ) {
        printf("\nError: file cannot be opened");
        sc = FAILURE;
    }
    else {
        Expense NewExpense;
        char ExpenseID[5], UserID[5], ExpenseCategory[25], ExpenseAmount[25], DateOfExpense[20];

        while( (sc == SUCCESS) && fscanf(file_ptr, "%s %s %s %s %s", ExpenseID, UserID, ExpenseCategory, ExpenseAmount, DateOfExpense) != EOF ) {
    
            NewExpense.expense_id = string_to_int(ExpenseID);
            NewExpense.user_id = string_to_int(UserID);
            strcpy(NewExpense.expense_category, ExpenseCategory);
            NewExpense.expense_amount = string_to_float(ExpenseAmount);
            strcpy(NewExpense.date_of_expense, DateOfExpense);

            sc = AddExpense(NewExpense);
        }
    }
    return sc;
}
status_code AddUser(User NewUser, int FamilyID) {
    extern UserNode* ulptr;
    status_code sc = SUCCESS;

    // searching 
    UserNode *nptr, *prev;
    nptr = ulptr; prev = NULL;
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
