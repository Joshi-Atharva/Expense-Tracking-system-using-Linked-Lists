/*
    driver file for expense tracking system
    Author: Atharva Joshi
    BT23CSE020
*/

#include "utils.h"
#include <stdio.h>

// global variable declarations:
UserNode* ulptr;
FamilyNode* flptr;
ExpenseNode* elptr;

int main() {

    // Null initialisation of linked list pointers
    ulptr = NULL; flptr = NULL; elptr = NULL;

    status_code sc;
    sc = ReadUserData(); PrintStatus("ReadUserData()", sc);
    sc = ReadExpenseData(); PrintStatus("ReadExpenseData()", sc);

    printf("Printing User List:\n"); PrintUserList();
    printf("Printing Expense List:\n"); PrintExpenseList();
    printf("Printing Family List:\n"); PrintFamilyList();
    FPrintAllLists();
    
    /* Testing Some Insert, Delete, Updation operations */
    Expense NewExpense;
    NewExpense = MakeExpense(100, 127, "Rent", 1300, "10/11/2024");
    AddExpense(NewExpense);
    printf("Printing Expense List:\n"); PrintExpenseList();
    printf("Printing Family List:\n"); PrintFamilyList();

    // Delete operation
    sc = DeleteUser(286); PrintStatus("DeleteUser(286)", sc);
    sc = DeleteUser(867); PrintStatus("DeleteUser(632)", sc);
    sc = DeleteUser(671); PrintStatus("DeleteUser(671)", sc);
    PrintAllLists();

    return 0;
}
