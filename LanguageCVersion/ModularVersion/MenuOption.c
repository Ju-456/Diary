#ifndef OPTION_C
#define OPTION_C

#include "Diary.h"

void CreatePage(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory)
{
    char PagesPasswordPath[PATH_MAX];
    char tempFilePath[PATH_MAX];
    int CurrentNumberPage = 0;
    char line[256];

    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL)
    {
        printf("Error allocating memory.\n");
        exit(1);
    }

    snprintf(PagesPasswordPath, PATH_MAX, "%s/%s/PagesPassword.txt", CDirectory, TempUser->UId);
    snprintf(tempFilePath, PATH_MAX, "%s/temp.txt", CDirectory);

    FILE *Bfile = fopen(PagesPasswordPath, "r");
    FILE *tempFile = fopen(tempFilePath, "w");
    if (tempFile == NULL)
    {
        perror("Error creating temporary file");
        if (Bfile != NULL)
            fclose(Bfile);
        return;
    }

    if (Bfile != NULL)
    {
        // read first line 
        if (fgets(line, sizeof(line), Bfile) != NULL)
        {
            sscanf(line, "Current Number of Page : %d", &CurrentNumberPage);
            //printf("CurrentNumberPage : %d\n", CurrentNumberPage);
        }
    }

    *NbPage = CurrentNumberPage + 1;

    fprintf(tempFile, "Current Number of Page : %d\n", *NbPage);

    if (Bfile != NULL)
    {
        while (fgets(line, sizeof(line), Bfile) != NULL)
        {
            fprintf(tempFile, "%s", line); // Copy  every line of the original file
        }
        
        fclose(Bfile);
    }

    printf("Enter the password (max 10 characters): ");
    scanf("%10s", (*TempPage)[*NbPage].password);

    fprintf(tempFile, "mdp%d : %s\n", *NbPage, (*TempPage)[*NbPage].password); 
    printf("mdp%d : %s\n", *NbPage, (*TempPage)[*NbPage].password); // test
    fclose(tempFile);

    remove(PagesPasswordPath);
    rename(tempFilePath, PagesPasswordPath);

    printf("Write your page (max 1024 characters):\n");
    while (getchar() != '\n'); // clean the buffer
    fgets((*TempPage)[*NbPage - 1].note, SizeMaxPage, stdin);

    printf("Note actuelle : %s\n", (*TempPage)[*NbPage - 1].note); // test

    SaveToFile(TempUser, NbPage, *TempPage, CDirectory);

    printf("Page n°%d created successfully.\n", *NbPage);
}// prend pas en compte les espaces dans un mdp je crois mais prend les chiffres 
// tous les caractères spéciaux sont pris en compte dans le contenu de la page, il faut rajouter le saut de ligne et modifier l'édition
// quand on se deconnecte et qu'on recree une page, les caractères sont mal retranscris SUR la page alors que le fget contient bien le message !!!
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
        // printf("%s\n", TempUserFoldPath); good path

        FILE *file = fopen("AccountPassword.txt", "r");
        if (file)
        {
            // printf("AccountPassword.txt was open successfully to open the page %d : \n", *page_requested_ptr);
            fclose(file);
        }
        else
        {
            perror("Error opening the page");
        }
    }

    EnterPasswordPage(*TempPage, page_requested_ptr, &NbPage, TempUser, CDirectory, SourcePath, DestinationPath, &PageToDelete);

    printf("This is the "); //PageN
    char TempPageFoldPath[PATH_MAX];
    snprintf(TempPageFoldPath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, *page_requested_ptr);
    FILE *PageAcces = fopen(TempPageFoldPath, "r");
    if (PageAcces == NULL)
    {
        perror("Error opening the file");
        return; 
    }

    char line[256];
    while (fgets(line, sizeof(line), PageAcces))
    {
        printf("%s", line); // line per line
    }

    fclose(PageAcces);

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