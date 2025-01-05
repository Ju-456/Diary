#ifndef USERLOGSIGN_C
#define USERLOGSIGN_C

#include "Diary.h"

void LogInUser(FILE *file, char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *SourcePath, char *DestinationPath)
{
    char answer[4];
    char VTempUserUId[15], VTempUserUPass[15]; // regeneralize if true
    char PathUser[80];
    printf("Enter your user ID: ");
    scanf("%14s", VTempUserUId);
    snprintf(PathUser, PATH_MAX, "%s/%s", CDirectory, VTempUserUId);
    // printf("%s",PathUser); chemin correct

    if (access(PathUser, F_OK) != 0)
    {
        printf("This ID doesn't exist.\nDo you want to create an account? (yes/no): ");
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
        {
            printf("Creating account...\n");
            SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file, SourcePath, DestinationPath);
        }
        else
        {
            ByeJournal(TempUser);
        }
    }
    else
    {
        printf("Enter the password: ");
        scanf("%14s", VTempUserUPass);

        snprintf(PathUser, PATH_MAX, "%s/%s/AccountPassword.txt", CDirectory, VTempUserUId);
        FILE *Afile = fopen(PathUser, "r");
        if (Afile == NULL)
        {
            perror("Error opening AccountPassword.txt");
            return;
        }

        int TooLate = 0;
        while (fscanf(Afile, "%14s %14s", TempUser->UId, TempUser->UPass) != EOF)
        {
            while (strcmp(VTempUserUPass, TempUser->UPass) != 0 && TooLate < 3)
            {
                TooLate++;
                printf("Wrong password. You only have %d attempt(s) left.\n", 3 - TooLate);
                if (TooLate < 3)
                {
                    printf("Enter the password again: ");
                    scanf("%14s", VTempUserUPass);
                }
                else
                {
                    printf("Access blocked. Contact support.\n");
                    fclose(Afile);
                    exit(0);
                }
            }

            if (strcmp(VTempUserUId, TempUser->UId) == 0 && strcmp(VTempUserUPass, TempUser->UPass) == 0)
            {
                printf("*Authorized access.*\n");
                PersonalAccess();
                menu(TempUser, TempPage, NbPage, CDirectory, SourcePath, DestinationPath, PageToDelete);
                fclose(Afile);
                return;
            }
        }

        printf("Invalid user ID or password.\n");
        fclose(Afile);
    }
}

void SignInUser(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath)
{
    char UPass2[15];
    PersonalAccess();
    printf("Choose a user id: ");
    scanf("%14s", TempUser->UId);
    // Just in case
    if (TempUser == NULL)
    {
        printf("Error: TempUser is not allocated.\n");
        return;
    }
    printf("Enter a password: ");
    scanf("%14s", TempUser->UPass);
    printf("Confirm the password: ");
    scanf("%14s", UPass2);
    printf("\n");
    if (strcmp(TempUser->UPass, UPass2) == 0)
    {
        FolderCreation(CDirectory, TempUser, TempPage, NbPage, PageToDelete, file, SourcePath, DestinationPath);
    }
    else
    {
        printf("Passwords don't match.\nPlease try again.\n");
        SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file, SourcePath, DestinationPath);
    }
}

#endif