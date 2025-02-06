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

    // Storing Initial Data int Output file
    FPrintAllLists();
    
    /* Testing Some Insert, Delete, Updation operations */
    Expense NewExpense;
    NewExpense = MakeExpense(100, 127, "Rent", 1300, "10/11/2024");
    AddExpense(NewExpense);

    // Delete operation
    sc = DeleteUser(286); PrintStatus("DeleteUser(286)", sc);
    sc = DeleteUser(867); PrintStatus("DeleteUser(632)", sc);
    sc = DeleteUser(671); PrintStatus("DeleteUser(671)", sc);

    // Delete Family 
    sc = DeleteFamily(409); PrintStatus("DeleteFamily(409)", sc);
    sc = DeleteFamily(400); PrintStatus("DeleteFamily(400)", sc);
    // Delete Expense 
    sc = DeleteExpense(142); PrintStatus("DeleteExpense(142)", sc);
    sc = DeleteExpense(143); PrintStatus("DeleteExpense(143)", sc);

    // Getters
    GetCategoricalExpense("Leisure", 604);
    GetHighestExpenseDay(821);
    PrintAllLists();

    return 0;
}
