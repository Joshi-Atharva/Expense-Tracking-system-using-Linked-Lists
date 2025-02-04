/*
    utils header file
*/
#ifndef UTILS_H
#define UTILS_H

#define NAME_SIZE 100
#define NUM_USERS 1000
#define NUM_EXPENSES 1000
#define NUM_FAMILIES 100
#define EXPENSE_CATEGORIES 11

typedef enum{FALSE, TRUE} Boolean;
typedef enum{FAILURE, SUCCESS} status_code;

// structure declarations:
typedef struct UserTag {
    int user_id;
    char user_name[NAME_SIZE];
    float income; // doubt: think about the two decimal point constraint
}User;

typedef struct FamilyTag {
    int family_id;
    char family_name[NAME_SIZE];
    int members[4];
    float total_family_income;
    float monthly_family_expense[12];
}Family;

typedef struct ExpenseTag {
    int expense_id;
    int user_id;
    char expense_category[EXPENSE_CATEGORIES];
    float expense_amount;
    char date_of_expense[11]; // in "dd/mm/yyyy\0" format
}Expense;

typedef struct UserExpTag {
    int cat;
    float exp;
}UserExp;

typedef struct UserNodeTag {
    User user;
    struct UserNodeTag* next;
}UserNode;

typedef struct FamilyNodeTag {
    Family family;
    struct FamilyNodeTag* next;
}FamilyNode;

typedef struct ExpenseNodeTag {
    Expense expense;
    struct ExpenseNodeTag* next;
}ExpenseNode;

// variable declarations:
extern UserNode* uptr;
extern FamilyNode* fptr;
extern ExpenseNode* eptr;

// --------function declarations--------
    // micro utilities:
    int string_to_int(char s[]);
    float string_to_float(char s[]);
    void FamilyNameGenerator(User NewUser, char* s);
    Expense MakeExpense(int exp_id, int user_id, const char* exp_cat, float exp_amt, const char* exp_date);

    // display:
    void PrintUserList();
    void PrintStatus(const char* opcode, status_code sc);
    void PrintFamilyList();
    void PrintExpenseList();

    // helper functions
    float GetMonthlyUserExpense(int UserID, int month);
    int CompareExpenses(Expense e1, Expense e2);

    // Searching:
    Boolean UserInFamily(Family family, int UserID);
    FamilyNode* FamilyOfUser(int UserID);

    // ----core functionalities----
        // Creation
        UserNode* CreateUserNode(User NewUser);
        FamilyNode* CreateFamilyNode(User NewUser, int FamilyID);

        // Insertion
        status_code InsertUserAfter(UserNode* prev, User NewUser);
        status_code AddUser(User NewUser, int FamilyID);
        status_code ReadUserData();
        status_code InsertUserToFamily(User NewUser, int FamilyID);
        status_code AddExpense(Expense NewExpense);
        status_code ReadExpenseData();



#endif
