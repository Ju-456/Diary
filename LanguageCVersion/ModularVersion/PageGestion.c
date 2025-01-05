#ifndef GESTIONPAGE_C
#define GESTIONPAGE_C

#include "Diary.h"

void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, char *CDirectory)
{
    char userFolderPath[PATH_MAX];
    snprintf(userFolderPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);
    if (chdir(userFolderPath) != 0)
    {
        printf("Error changing to user directory.\n");
        return;
    }
    for (int i = 0; i < *NbPage; i++)
    {
        char PageFilePath[PATH_MAX];
        snprintf(PageFilePath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, i + 1);
        FILE *Wfile = fopen(PageFilePath, "w");
        if (Wfile == NULL)
        {
            printf("Error opening file for writing page %d.\n", i + 1);
            continue;
        }
        time_t timestamp = time(NULL);
        struct tm *FrenchHour = localtime(&timestamp);
        fprintf(Wfile, "Page%d\nDate: %02d/%02d/%04d\nAt: %02dh%02d:%02d\n\n",
                i + 1, FrenchHour->tm_mday, FrenchHour->tm_mon + 1,
                1900 + FrenchHour->tm_year, FrenchHour->tm_hour, FrenchHour->tm_min, FrenchHour->tm_sec);
        fprintf(Wfile, "Content:\n%s\n\n", TempPage[i].note);
        fclose(Wfile);
    }
    printf("The page is saved successfully.\n");
}

// to generalize
int BlockedAccessPage(char *SourcePath, char *DestinationPath, User *TempUser, char *CDirectory, int PageToDelete, int *NbPage, Page **TempPage)
{
    char TempUserFoldPath[PATH_MAX];
    snprintf(SourcePath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, *NbPage);
    FILE *Source = fopen(SourcePath, "rb");
    if (Source == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier source");
        return 1;
    }
    snprintf(DestinationPath, PATH_MAX, "%s/%s/BlockedPages/Page%d.txt", CDirectory, TempUser->UId, *NbPage);
    FILE *destination = fopen(DestinationPath, "wb");
    if (destination == NULL)
    {
        perror("Erreur lors de la création du fichier destination");
        fclose(Source);
        return 1;
    }
    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), Source)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destination);
    }
    fclose(Source);
    fclose(destination);
    printf("Fichier copié avec succès de %s vers %s\n", SourcePath, DestinationPath);
    if (BlockedAccessPage(SourcePath, DestinationPath, TempUser, CDirectory, PageToDelete, NbPage, TempPage) == 0)
    {
        for (int i = PageToDelete; i < *NbPage - 1; i++)
        {
            (*TempPage)[i] = (*TempPage)[i + 1];
        }
        *TempPage = realloc(*TempPage, (*NbPage - 1) * sizeof(Page));
        if (*TempPage == NULL && *NbPage > 1)
        {
            printf("Error allocating memory.\n");
            exit(1);
        }
        snprintf(TempUserFoldPath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, PageToDelete + 1);
        if (remove(TempUserFoldPath) == 0)
        {
            printf("Page%d deleted successfully from disk.\n", PageToDelete + 1);
        }
        else
        {
            perror("Error during file deletion");
        }
        (*NbPage)--;
        printf("Page deleted successfully.\n");
        printf("Copy fonctionnal\n");
    }
    else
    {
        printf("Error Copy\n");
    }
}

void EnterPasswordPage(Page *TempPage, int page_index, int *NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath, int PageToDelete)
{
    char password[11];
    int TooLate = 0;

    printf("Enter the password for this page (you have 3 attempts):\n");

    while (TooLate < 3)
    {
        printf("Attempt %d: ", TooLate + 1);
        scanf("%10s", password);

        char PagesPasswordPath[PATH_MAX];
        snprintf(PagesPasswordPath, PATH_MAX, "%s/%s/PagesPassword.txt", CDirectory, TempUser->UId);

        FILE *Bfile = fopen(PagesPasswordPath, "r");
        if (Bfile == NULL)
        {
            perror("Error opening PagesPassword.txt");
            return;
        }

        char line[256];
        int found = 0;
        char target[256];

        // Prepare the target identifier for the requested page
        snprintf(target, sizeof(target), "mdp%d :", page_index + 1);

        while (fgets(line, sizeof(line), Bfile))
        {
            if (strncmp(line, target, strlen(target)) == 0)
            {
                // Extract the password stored in the file to verify compatibility
                char *stored_password = strchr(line, ':');
                if (stored_password != NULL)
                {
                    stored_password += 2; // Skip the colon and the space
                    stored_password[strcspn(stored_password, "\n")] = '\0';

                    if (strcmp(stored_password, password) == 0)
                    {
                        found = 1;
                        break;
                    }
                }
            }
        }
        fclose(Bfile);

        if (found)
        {
            printf("*Authorized access.*\n");
            printf("Note: %s\n", TempPage[page_index].note);
            return;
        }
        else
        {
            TooLate++;
            if (TooLate < 3)
            {
                printf("Wrong password. You have %d attempt(s) left.\n", 3 - TooLate);
            }
            else
            {
                printf("Access to this page is blocked.\n");
                BlockedAccessPage(SourcePath, DestinationPath, TempUser, CDirectory, PageToDelete, NbPage, &TempPage);
                printf("Returning to the menu.\n");
                menu(TempUser, &TempPage, NbPage, CDirectory, SourcePath, DestinationPath, page_index);
            }
        }
    }
}

void WriteInPage(Page *TempPage, int page_index)
{
    time_t timestamp = time(NULL);
    struct tm *FrenchHour = localtime(&timestamp);

    printf("Enter a new note for this page: ");
    getchar();
    fgets(TempPage[page_index].note, SizeMaxPage, stdin);
    char date_time[64];
    snprintf(date_time, sizeof(date_time), "Page%d\nDate: %02d/%02d/%04d\nAt: %02dh%02d:%02d\n\n",
             page_index + 1, FrenchHour->tm_mday, FrenchHour->tm_mon + 1,
             1900 + FrenchHour->tm_year, FrenchHour->tm_hour, FrenchHour->tm_min, FrenchHour->tm_sec);

    char UpdatedNote[SizeMaxPage + 64];
    snprintf(UpdatedNote, sizeof(UpdatedNote), "%s%s", date_time, TempPage[page_index].note);
    strncpy(TempPage[page_index].note, UpdatedNote, SizeMaxPage);

    printf("The note was changed successfully.\n");
}

#endif
