#ifndef OPTION_C
#define OPTION_C

#include "Diary.h"

void CreatePage(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory){
    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL)
    {
        printf("Error allocating memory.\n");
        exit(1);
    }
    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*TempPage)[*NbPage].password);
    printf("Write your page (max 1024 characters):\n ");
    getchar();
    fgets((*TempPage)[*NbPage].note, SizeMaxPage, stdin);
    (*NbPage)++;
    SaveToFile(TempUser, NbPage, *TempPage, CDirectory);
}

void DeletePageProcess(Page **TempPage, int *NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath){
    int PageToDelete;
    char answer[4];
    char TempUserFoldPath[PATH_MAX];

    printf("Which page do you want to delete? (number): ");
    scanf("%d", &PageToDelete);
    if (PageToDelete > *NbPage || PageToDelete <= 0)
    {
        printf("This page doesn't exist.\n");
        return;
    }
    PageToDelete--;
    EnterPasswordPage(*TempPage, PageToDelete, NbPage, TempUser, CDirectory, SourcePath, DestinationPath, PageToDelete); // si le mdp est bon, on fait la suite ?
    printf("Are you sure you want to delete page number %d (yes/no): ", PageToDelete + 1);
    scanf("%3s", answer);
    if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
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
            snprintf(TempUserFoldPath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, PageToDelete + 1);
            if (remove(TempUserFoldPath) == 0)
            {
                printf("Page%d deleted successfully.\n", PageToDelete + 1);
            }
            else
            {
                perror("Error during suppression of page");
            }
            (*NbPage)--;
            printf("Page deleted successfully.\n");
        }
    }
    else
    {
        printf("Do you prefer to change the note? (yes/no): ");
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
        {
            WriteInPage(*TempPage, PageToDelete);
        }
        else
        {
            printf("Right, returning to the menu.\n");
            menu(TempUser, TempPage, NbPage, CDirectory, SourcePath, DestinationPath, PageToDelete);
        }
    }
}

void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath){
    int page_requested;
    printf("Which page do you want to consult? (number): ");
    scanf("%d", &page_requested);
    if (page_requested = 0)
    {
        printf("Please, enter a number greater than zero :\n");
        scanf("%d", &page_requested);
    }
    else if (page_requested != 0)
    {   
        char TempUserFoldPath[PATH_MAX];
        snprintf(TempUserFoldPath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, page_requested);
            FILE *file = fopen("password.txt", "r");
            if (file) {
            printf("The Page %d was open successfully : \n", page_requested);
            
            fclose(file); // Fermer le fichier une fois terminé
        } else {
            perror("Error opening the page");
        }
    }
    EnterPasswordPage(*TempPage, page_requested - 1, &NbPage, TempUser, CDirectory, SourcePath, DestinationPath, PageToDelete);
    printf("Do you want to write in this page? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
    {
        WriteInPage(*TempPage, page_requested - 1);
    }
    else
    {
        BoucleJournal(TempUser, TempPage, &NbPage, PageToDelete, CDirectory, SourcePath, DestinationPath);
    }
}

#endif