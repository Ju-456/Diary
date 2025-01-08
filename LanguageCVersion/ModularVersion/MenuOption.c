#ifndef OPTION_C
#define OPTION_C

#include "Diary.h"

void CreatePage(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory)
{
    char ChangeNumberPage[256];
    char PagesPasswordPath[PATH_MAX];
    int pageFound = 0;

    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL)
    {
        printf("Error allocating memory.\n");
        exit(1);
    }

    printf("Enter the password (max 10 characters): ");
    scanf("%10s", (*TempPage)[*NbPage].password);

    snprintf(PagesPasswordPath, PATH_MAX, "%s/%s/PagesPassword.txt", CDirectory, TempUser->UId);
    printf("Attempting to open: %s\n", TempUser->UId); // c'est vide !!!

    FILE *Bfile = fopen(PagesPasswordPath, "r");
    if (Bfile == NULL)
    {
        perror("Error opening PagesPassword.txt");
        return;
    }

    // Create temporary file
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        perror("Error creating temporary file");
        fclose(Bfile);
        return;
    }

    // Read PagesPassword.txt & maj "Current Number of Page"
    while (fgets(ChangeNumberPage, sizeof(ChangeNumberPage), Bfile))
    {
        if (strstr(ChangeNumberPage, "Current Number of Page") != NULL)
        {
            fprintf(tempFile, "Current Number of Page : %d\n", *NbPage + 1);
            pageFound = 1;
        }
        else
        {
            fputs(ChangeNumberPage, tempFile);
        }
    }

    if (!pageFound)
    {
        fprintf(tempFile, "Current Number of Page : %d\n", *NbPage + 1);
    }

    // Add the line for the new mdp
    fprintf(tempFile, "mdp%d : %s\n", *NbPage + 1, (*TempPage)[*NbPage].password);

    fclose(Bfile);
    fclose(tempFile);

    // Remplace PagesPassword.txt with the temporary file
    remove(PagesPasswordPath);
    rename("temp.txt", PagesPasswordPath);

    printf("Write your page (max 1024 characters):\n ");
    getchar();
    fgets((*TempPage)[*NbPage].note, SizeMaxPage, stdin);

    (*NbPage)++;

    SaveToFile(TempUser, NbPage, *TempPage, CDirectory);

    printf("Page n°%d created successfully.\n", *NbPage);
}

void DeletePageProcess(Page **TempPage, int *NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath)
{
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
    EnterPasswordPage(*TempPage, &PageToDelete, NbPage, TempUser, CDirectory, SourcePath, DestinationPath, &PageToDelete); // si le mdp est bon, on fait la suite ?
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

void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath)
{
    int page_requested;
    int *page_requested_ptr = &page_requested; // Pointer to int

    printf("Which page do you want to consult? (number): ");
    scanf("%d", page_requested_ptr); 

    if (*page_requested_ptr <= 0) // verification of the value's pointer
    {
        printf("Please, enter a number greater than zero :\n");
        scanf("%d", page_requested_ptr);
    }

    if (*page_requested_ptr > 0)
    {
        char TempUserFoldPath[PATH_MAX];
        snprintf(TempUserFoldPath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, *page_requested_ptr);
        printf("%s\n", TempUserFoldPath);

        FILE *file = fopen("AccountPassword.txt", "r");
        if (file)
        {
            //printf("AccountPassword.txt was open successfully to open the page %d : \n", *page_requested_ptr);
            fclose(file);
        }
        else
        {
            perror("Error opening the page");
        }
    }

    EnterPasswordPage(*TempPage, page_requested_ptr, &NbPage, TempUser, CDirectory, SourcePath, DestinationPath, &PageToDelete);

    printf("Do you want to write in this page? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);

    if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
    {
        WriteInPage(*TempPage, *page_requested_ptr - 1); 
    }
    else
    {
        BoucleJournal(TempUser, TempPage, &NbPage, PageToDelete, CDirectory, SourcePath, DestinationPath);
    }
}

#endif