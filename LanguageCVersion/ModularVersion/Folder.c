#ifndef FOLDER_C
#define FOLDER_C

#include "Diary.h"
#include "UserLogSign.c"

void CurrentDirectory(char *CDirectory, FILE *file){
    if (getcwd(CDirectory, PATH_MAX) != NULL)
    {
        // printf("Current directory: %s\n", CDirectory);
    }
    else
    {
        perror("Problem of autorization, please fix it in settings");
    }
}

void CodeImplementation(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath){
    WelcomeDiary();
    printf("Do you already use this diary ? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    if (strcmp(answer, "no") == 0 || strcmp(answer, "NO") == 0)
    {
        FILE *file = fopen("password.txt", "a"); // create bc doesn't exist
        printf("Go to create an account!\n");
        SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file, SourcePath, DestinationPath);
    }
    else
    {
        FILE *file = fopen("password.txt", "r"); // on pourrait mettre "a" par précaution mais ça ecraserait le précédent, s'il a déjà des données..
        if (file != NULL)
        {   
            printf("So... we can start it !\n");
            LogInUser(file, CDirectory, TempUser, TempPage, NbPage, PageToDelete, SourcePath, DestinationPath);
            fclose(file);
        }
        else
        {
            printf("Apparently no ! Please create a new account to join us ;)\n");
            SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file, SourcePath, DestinationPath);
        }
    }
}

void VerificationCreationPasswordtxt(FILE *file, char *CDirectory){
    char *PathPassword[PATH_MAX];
    snprintf(*PathPassword, PATH_MAX, "%s/password.txt", CDirectory);
    if (*PathPassword == NULL)
    {
        printf("Failed to create password repository.\n");
    }
    else
    {
        fclose(file);
    }
}


void FolderCreation(char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath){
    char TempUserFoldPath[PATH_MAX];
    char TempUserFoldPathbis[PATH_MAX];
    snprintf(TempUserFoldPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);
    if (CREATE_DIR(TempUserFoldPath) == 0)
    {
        // printf("User folder successfully created: %s\n", TempUserFoldPath);
        char PassFilePath[PATH_MAX];
        snprintf(PassFilePath, PATH_MAX, "%s/password.txt", CDirectory);
        FILE *Afile = fopen(PassFilePath, "a");
        if (Afile == NULL)
        {
            perror("Error opening password.txt");
            return;
        }
        fprintf(Afile, "%s %s\n", TempUser->UId, TempUser->UPass);
        fclose(Afile);
        printf("User account successfully created.\n");
        // Creation répertoire des pages bloqués en même temps que creation du compte
        snprintf(TempUserFoldPathbis, PATH_MAX, "%s/%s/BlockedPages", CDirectory, TempUser->UId);
        //FILE *Bfile = fopen(TempUserFoldPathbis, "a");
        if (CREATE_DIR(TempUserFoldPathbis) == 0)
        {
            perror("Error opening BlockedPages");
            return;
        }
        menu(TempUser, TempPage, NbPage, CDirectory, SourcePath, DestinationPath, PageToDelete);
    }
    else
    {
        perror("Error creating account");
        LogInUser(file, CDirectory, TempUser, TempPage, NbPage, PageToDelete, SourcePath, DestinationPath);
    }
}

#endif
