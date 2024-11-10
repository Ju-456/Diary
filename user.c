#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "MenuAndDeco.h"
#include "user.h"
#include "page.h"

void DataUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete) {

    printf("Enter an user id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Enter the password: ");
    scanf("%49s", TempUser->UPassword);

    printf("\n");
    
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword); // changer mettre le mdp dans un dossier nommer mdp dans fichier
    if (access(FolderName, F_OK) == 0) { 
        printf("You already have an account!\n");
        printf("\n");
        LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);// si compte identification
    } else {
        printf("This identifiant doesn't exist..\nDo you want to create an account (yes/no) ? ");
        char answer1[4];
        scanf("%3s", answer1);
        if (strcmp(answer1, "yes") == 0) {
            printf("Go to create an account!\n");
            CreateAnUser(TempUser, FolderName, TempPage, NbPage, filename, file, PageToDelete);// Si pas de compte, créer un compte
        } else {
            ByeJournal();
            exit(0);
        }
    }
}

void LogInUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete){

/*printf("Enter your id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Enter your password: ");
    scanf("%49s", TempUser->UPassword);*/

    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword);
    if (access(FolderName, F_OK) == 0) {
        BoucleJournal(TempUser, TempPage, NbPage, filename, file, PageToDelete);
        PersonalAccess();
    } else {
        int TooLate = 0;
        TooLate++;
        if (TooLate < 3) {
            printf("Wrong password. Be careful, you only have %d attempt(s) left.\n", 3 - TooLate);
            LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);
        } else {
            printf("You can't access this account, this account was blocked, please contact the support.\n");
            printf("\n");
            //BlockedAccesUser() à créér
            //début programme
        }
    }
}

void CreateAnUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete){

    char UPassword2[50];

    printf("Choose an user id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Enter a password: ");
    scanf("%49s", TempUser->UPassword);

    printf("Confirm the password: ");
    scanf("%49s", UPassword2);
    printf("\n");

    if (strcmp(TempUser->UPassword, UPassword2) == 0) {
        PersonalAccess();
        FolderCreation(FolderName, TempUser, TempPage, NbPage, filename, file, PageToDelete);
    } else {
        printf("Passwords don't match.\nPlease try answer.\n");
        CreateAnUser(TempUser, FolderName, TempPage, NbPage, filename, file, PageToDelete);
    }
}


void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file, int PageToDelete ) {
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword);
    //stocker dans le fichier UserData TempUser->UIdentifiant et TempUser->UPassword
    #ifdef _WIN32
        if (_mkdir(FolderName) == 0) {
            printf("Folder créé avec succès : %s\n", FolderName);
        } else {
            perror("Erreur lors de la création du dossier");
        }
    #else
        if (mkdir(FolderName, 0777) == 0) { //766 pour empecher balade fichier 
            printf("Hello\n");
            //DateAndHour(); à l1 puis user écrit à l2
            BoucleJournal(TempUser, TempPage, NbPage, filename, file, PageToDelete);
        } else {
            printf("You already have an account, please login:\n");
            LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);
        }
    #endif
}

void FolderExist(char *FolderName, User *TempUser, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete) {
    if (access(FolderName, F_OK) == 0) {
        printf("Account exists, logging in...\n");
        LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);
    } else {
        printf("So you don't have an account\n");
        printf("Do you want to create one? (yes/no): ");
        char response[4];
        scanf("%3s", response);
        if (strcmp(response, "yes") == 0) {
            FolderCreation(FolderName, TempUser, TempPage, NbPage, filename, file, PageToDelete);
        } else {
            printf("Goodbye!\n");
            exit(0); 
        }
    }
}

void SaveToFile(char *filename, FILE *file, int *NbPage, Page **TempPage,User *TempUser) {
    //snprintf(filename, sizeof(filename), "/home/ju456/%s_%s/Page%d.txt", TempUser->UIdentifiant, TempUser->UPassword, *NbPage);
    snprintf(filename, 1024, "/home/ju456/%s_%s/Page%d.txt", TempUser->UIdentifiant, TempUser->UPassword, *NbPage);
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < *NbPage; i++) {
        DateAndHour(file,NbPage);
        printf("Enter your note :");
        fprintf(file, "%s\n", (*TempPage)[i].note); 
    }
    fclose(file);
}
