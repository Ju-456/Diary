#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MenuAndDeco.h"
#include "page.h"

#define SizeMaxPage 256

void CreatePage(User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file) {
    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }

    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*TempPage)[*NbPage].password);

    printf("Enter note for this page : ");
    getchar();
    fgets((*TempPage)[*NbPage].note, SizeMaxPage, stdin);

    (*NbPage)++;
    printf("The page was created successfully.\n");
    DateAndHour(file, NbPage);
    printf("\n");

    SaveToFile(filename, file, NbPage, TempPage, TempUser);
}

void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser, char *filename, FILE *file) {
    int PageToDelete;

    printf("Which page do you want to delete? (number) : ");
    scanf("%d", &PageToDelete);

    if (PageToDelete > *NbPage || PageToDelete <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    PageToDelete--;

    ConsultPagepB(TempUser, NbPage, TempPage, filename, file, PageToDelete);
}


void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, char *filename, FILE *file, User *TempUser) {
    int page_requested;
    /* if (strcmp((*TempPage)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(TempUser, TempPage, &NbPage, filename, file, PageToDelete);
        return;
    }*/
   
    printf("Which page do you want to consult? (number) : ");
    scanf("%d", &page_requested);

    if (page_requested > NbPage || page_requested <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    EnterPassword(*TempPage, page_requested - 1, &NbPage, filename, file, PageToDelete, TempUser);
    printf("Do you want to write in this page? (yes/no) : ");
    char answer[4];
    scanf("%s", answer);
    if (strcmp(answer, "yes") == 0) {
        WriteInPage(*TempPage, page_requested - 1);
    }
}

// Auxiliary functions for gestion of page 
void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage,char *filename, FILE *file, int PageToDelete) {
    char answer[4];

    if (strcmp((*TempPage)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(TempUser, TempPage, NbPage, filename, file, PageToDelete);
        return;
    }
    EnterPassword(*TempPage, PageToDelete, NbPage, filename, file, PageToDelete, TempUser);

    printf("Are you sure you want to delete page number %d (yes/no) : ", PageToDelete + 1);
    scanf("%s", answer);
    if (strcmp(answer, "yes") == 0) {
        for (int i = PageToDelete; i < *NbPage - 1; i++) {
            (*TempPage)[i] = (*TempPage)[i + 1];
        }
        *TempPage = realloc(*TempPage, (*NbPage - 1) * sizeof(Page));
        if (*TempPage == NULL && *NbPage > 1) {
            printf("Error allocating memory.\n");
            exit(1);
        }
        (*NbPage)--;
        printf("Page deleted successfully.\n");
    } else { 
        printf("Do you prefer to change the note? (yes/no) : ");
        scanf("%s", answer);
        if (strcmp(answer, "yes") == 0) {
            int page_index = PageToDelete;
            WriteInPage(*TempPage, page_index);
        } else {
            printf("Right, returning to the menu.\n");
            menu(TempUser, TempPage, NbPage, filename, file, PageToDelete);
        }
    }
}

void WriteInPage(Page *TempPage, int page_index) {
    printf("Enter a new note for this page : ");
    getchar();
    fgets(TempPage[page_index].note, SizeMaxPage, stdin);
    printf("The note was changed successfully.\n");
}

void EnterPassword(Page *TempPage, int page_index, int *NbPage, char *filename, FILE *file, int PageToDelete, User *TempUser) {
    char password[11];
    int TooLate = 0;
    printf("Enter the password for this page (be careful, you only have 3 attempts) : ");
    while (TooLate < 3) {
        printf("Try answer : ");
        scanf("%10s", password);

        if (strcmp(TempPage[page_index].password, password) == 0) {
            printf("*Authorized access.*\n");
            // printf("Date : %s\n", TempPage[page_index].date); **time.h**
            printf("Note : %s\n", TempPage[page_index].note);
            return;
        } else {
            TooLate++;
            if (TooLate < 3) {
                printf("Wrong password. Be careful, you only have %d attempt(s) left.\n", 3 - TooLate);
            } else {
                printf("You can't access this page, this page was blocked\n");
                printf("\n");
                //BlockedAccessPage(&TempPage, page_index);
                printf("Returning to the menu.\n");
                menu(TempUser, &TempPage, NbPage, filename, file, PageToDelete);
            }
        }
    }
}

/* A optimiser ou même changer le raisonnement bouléen
void BlockedAccessPage (Page **TempPage, int PageToDelete) {
    
    char CopyBlockedPass[11];
    char CopyBlockedNote[SizeMaxPage];

    // Copy intern of page's information
    // strncpy(source, destination, n);
    strncpy(CopyBlockedPass, (*TempPage)[PageToDelete].password, sizeof((*TempPage)[PageToDelete].password));
    strncpy(CopyBlockedNote, (*TempPage)[PageToDelete].note, sizeof((*TempPage)[PageToDelete].note));

    // Page reset 
    // memset(source, remplacement, n); 
    strncpy((*TempPage)[PageToDelete].password, "MdpAdminOnly2104", sizeof((*TempPage)[PageToDelete].password)); // supprime
    memset((*TempPage)[PageToDelete].note, 0, sizeof((*TempPage)[PageToDelete].note)); 

    printf("%s",CopyBlockedPass); // ça n'a pas marché 
    
}*/

void DateAndHour(FILE *file, int *NbPage){
    time_t timestamp = time(NULL);
    struct tm *FrenchHour = localtime(&timestamp);
    fprintf(file, "Page%d\nDate : %02d/%02d/%04d\nAt : %02dh%02d:%02d\n ",
        *NbPage, FrenchHour->tm_mday, FrenchHour->tm_mon + 1, 1900 + FrenchHour->tm_year,
        FrenchHour->tm_hour, FrenchHour->tm_min, FrenchHour->tm_sec);
}
