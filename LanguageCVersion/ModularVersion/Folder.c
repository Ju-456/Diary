#ifndef FOLDER_C
#define FOLDER_C

#include "Diary.h"
#include "UserLogSign.c"

void CurrentDirectory(char *CDirectory, FILE *file)
{
    if (getcwd(CDirectory, PATH_MAX) != NULL)
    {
        // printf("Current directory: %s\n", CDirectory);
    }
    else
    {
        perror("Problem of autorization, please fix it in settings");
    }
}

void CodeImplementation(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath)
{
    WelcomeDiary();
    printf("Do you already use this diary ? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    if (strcmp(answer, "no") == 0 || strcmp(answer, "NO") == 0)
    {
        FILE *file = fopen("AccountPassword.txt", "a"); // create bc doesn't exist
        printf("Go to create an account!\n");
        SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file, SourcePath, DestinationPath);
    }
    else
    {
        FILE *file = fopen("AccountPassword.txt", "r"); // on pourrait mettre "a" par précaution mais ça ecraserait le précédent, s'il a déjà des données..
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

void VerificationCreationPasswordtxt(FILE *file, char *CDirectory)
{
    char *PathPassword[PATH_MAX];
    snprintf(*PathPassword, PATH_MAX, "%s/AccountPassword.txt", CDirectory);
    if (*PathPassword == NULL)
    {
        printf("Failed to create password repository.\n");
    }
    else
    {
        fclose(file);
    }
}

void FolderCreation(char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath)
{
    // char BlockedUserAccountPath[PATH_MAX];
    char AccountPasswordPath[PATH_MAX];
    char PagesPasswordPath[PATH_MAX];
    char BlockedPagesPath[PATH_MAX];

    snprintf(AccountPasswordPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);
    if (CREATE_DIR(AccountPasswordPath) == 0)
    { /*
      // Creation of BlockedUserAccount in the same time
      snprintf(BlockedUserAccountPath, PATH_MAX, "%s/BlockedPages", CDirectory);
      if (CREATE_DIR(BlockedUserAccountPath) == 0)
      {
          perror("Error opening BlockedUserAccount");
          return;
      }
      // Idk if it's a great idea
      */
        // printf("User folder successfully created: %s\n", AccountPasswordPath);
        char PassFilePath[PATH_MAX];
        snprintf(PassFilePath, PATH_MAX, "%s/AccountPassword.txt", CDirectory);
        FILE *Afile = fopen(PassFilePath, "a");
        if (Afile == NULL)
        {
            perror("Error opening AccountPassword.txt");
            return;
        }
        fprintf(Afile, "%s %s\n", TempUser->UId, TempUser->UPass);
        fclose(Afile);
        printf("User account successfully created.\n");

        // Creation of PagesPassword.txt in the same time
        snprintf(PagesPasswordPath, PATH_MAX, "%s/%s/PagesPassword.txt", CDirectory, TempUser->UId);
        FILE *Bfile = fopen(PagesPasswordPath, "a");
        if (Bfile == NULL)
        {
            perror("Error opening PagesPassword.txt");
            return;
        }
        fclose(Bfile);

        // Creation of BlockedPages in the same time
        snprintf(BlockedPagesPath, PATH_MAX, "%s/%s/BlockedPages", CDirectory, TempUser->UId);
        if (CREATE_DIR(BlockedPagesPath) == 0)
        {
            perror("Error opening BlockedPages");
            return;
        }

        // return to the menu after do all things concerning an opening of a new account
        menu(TempUser, TempPage, NbPage, CDirectory, SourcePath, DestinationPath, PageToDelete);
    }
    else
    {
        perror("Error creating account");
        LogInUser(file, CDirectory, TempUser, TempPage, NbPage, PageToDelete, SourcePath, DestinationPath);
    }
}

#endif
