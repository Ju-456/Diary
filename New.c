#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <linux/limits.h>

#define SizeMaxPage 256

typedef struct {
    char UId[15];
    char UPassword[15];
} User;

typedef struct {
    char password[11];  // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

// Global function
void menu(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);
void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);

// User gestion part
void DataUser(User *TempUser, char *FolderName, Page **TempPage, int *NbPage, int PageToDelete);
void SignInUser(User *TempUser, char *FolderName, Page **TempPage, int *NbPage, int PageToDelete);
void LogInUser(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);
void SignIn(User *TempUser, char *FolderName, Page **TempPage, int *NbPage, int PageToDelete);

// Folder gestion part
void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);
void FolderExist(char *FolderName, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);

// Page gestion part
void CreatePage(User *TempUser, Page **TempPage, int *NbPage);
void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser);
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser);
void DateAndHour(FILE *file, int *NbPage);

// Auxiliary functions
void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage, int PageToDelete);
void EnterPassword(Page *TempPage, int page_index, int *NbPage, int PageToDelete, User *TempUser);
void WriteInPage(Page *TempPage, int page_index);

// Invisible functions
void SaveToFile(User *TempUser, int *NbPage, Page **TempPage);
void BlockedAccesPage(Page **TempPage, int *NbPage, int PageToDelete);

// Decoration functions
void WelcomeJournal();
void PersonalAccess();
void ByeJournal();

void WelcomeJournal(){
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***_______the_______***\n***_____journal_____***\n*****_____:D______*****\n*******_________*******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}

void PersonalAccess(){

    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***______your_______***\n***____personal_____***\n****_____space_____****\n*****_____^^_____******\n********_______********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");

}

void ByeJournal() {
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_______See_______***\n***_______you_______***\n***______later______***\n***_________________***\n*****_____Bye______****\n******____<3_____******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}

void DataUser(){

    printf("Is it the first time you use this programm ?\n");
    if (strcmp(answer, "yes") == 0){
        // a créer dans le répertoire de travail
        FILE *file = fopen("password.txt", "w");
        if (file == NULL) {
        printf("Error permission.\n");
    }
    }

    printf("Do you have an account?\n");
    char answer[4];
    if (strcmp(answer, "yes") == 0) {
            printf("Please Log in :\n");
            LogInUser();
        } else {
            SignIn(TempUser, FolderName, TempPage, NbPage, PageToDelete);
            exit(0);
        }

}

void LogInUser(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete) {

    FILE *file = fopen("password.txt", "r");
    if (file == NULL) {
        printf("Error, can't open file\n");
        return;
    }

    int found = 0;
    printf("Enter your id: ");
    scanf("%49s", TempUser->UId);

    printf("Enter the password: ");
    scanf("%49s", TempUser->UPassword);

    char LogInId[15],LogInPass[15]
    while (fscanf(file, "%s %s", TempUser->UId, TempUser->UPassword) != EOF) {
        if (strcmp(LogInId, TempUser->UId) == 0 && strcmp(LogInPass, TempUser->UPassword) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        PersonalAccess();
        BoucleJournal(TempUser, TempPage, NbPage, PageToDelete);
        } else {
        printf("Your account doesn't exist or maybe the password is wrong...\nDo you want to try again (T) or create an account(C) ?(C/T)");
        if (strcmp(answer, "T") == 0) {
            //TryAgain();
            } else if (strcmp(answer, "C") == 0) {
            //SignIn(User *TempUser, char *FolderName, Page **TempPage, int *NbPage, int PageToDelete);
            } else {
            printf("Invalid option.\n");
    }
    }

}

void SignIn(User *TempUser, char *FolderName, Page **TempPage, int *NbPage, int PageToDelete){
    char UPassword2[50];

    printf("Choose an user id: ");
    scanf("%49s", TempUser->UId);

    printf("Enter a password: ");
    scanf("%49s", TempUser->UPassword);

    printf("Confirm the password: ");
    scanf("%49s", UPassword2);
    printf("\n");

    if (strcmp(TempUser->UPassword, UPassword2) == 0) {
        FolderCreation(FolderName, TempUser, TempPage, NbPage, PageToDelete);
    } else {
        printf("Passwords don't match.\nPlease try again.\n");
        SignIn(TempUser, FolderName, TempPage, NbPage, PageToDelete);
    }
}

void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete) {
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UId, TempUser->UPassword);
    if (mkdir(FolderName, 0777) == 0) { 
        BoucleJournal(TempUser, TempPage, NbPage, PageToDelete);
    } else {
        printf("You already have an account, please login:\n");
        SignInUser(TempUser, FolderName, TempPage, NbPage, PageToDelete);
    }
}