#ifndef DECORSMENU_C
#define DECORSMENU_C

#include "Diary.h"
#include "PageGestion.c"

void WelcomeDiary(){
    printf(
        "***********************\n"
        "***********_***********\n"
        "*********_____*********\n"
        "*******_________*******\n"
        "*****_____________*****\n"
        "***_____Welcome_____***\n"
        "***_______to________***\n"
        "***_______the_______***\n"
        "***______Diary______***\n"
        "*****_____:D______*****\n"
        "*******_________*******\n"
        "*********_____*********\n"
        "**********___**********\n"
        "***********_***********\n"
        "***********************\n");
    printf("\n");
}

void PersonalAccess(){
    printf(
        "***********************\n"
        "***********_***********\n"
        "*********_____*********\n"
        "*******_________*******\n"
        "*****_____________*****\n"
        "***_____Welcome_____***\n"
        "***_______to________***\n"
        "***______your_______***\n"
        "***____personal_____***\n"
        "****_____space_____****\n"
        "*****_____^^______*****\n"
        "*******_________*******\n"
        "*********_____*********\n"
        "**********___**********\n"
        "***********_***********\n"
        "***********************\n");
    printf("\n");
}

void ByeJournal(User *TempUser){
    printf(
        "***********************\n"
        "***********_***********\n"
        "*********_____*********\n"
        "*******_________*******\n"
        "*****_____________*****\n"
        "***_______See_______***\n"
        "***_______you_______***\n"
        "***______later______***\n"
        "***___ '%s' ___***\n"
        "*****_____Bye______*****\n"
        "******____<3______******\n"
        "*******_________*******\n"
        "*********_____*********\n"
        "**********___**********\n"
        "***********_***********\n"
        "***********************\n",
        TempUser->UId);
    exit(0);
}


void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory, char *SourcePath, char *DestinationPath){
    while (1)
    {
        menu(TempUser, TempPage, NbPage, CDirectory, SourcePath, DestinationPath, PageToDelete);
        printf("Do you want to do something else? (yes/no) : ");
        char answer[4];
        scanf("%s", answer);
        printf("\n");
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
        {
            menu(TempUser, TempPage, NbPage, CDirectory, SourcePath, DestinationPath, PageToDelete);
        }
        else
        {
            ByeJournal(TempUser);
        }
    }
}

void menu(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory, char *SourcePath, char *DestinationPath, int PageToDelete){
    int choice = 0;
    printf(
        "\n1. Create Page\n"
        "2. Delete Page\n"
        "3. Consult Page\n"
        "4. Quit\n"
        "\nChoose: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        CreatePage(TempUser, TempPage, NbPage, CDirectory);
        break;
    case 2:
        DeletePageProcess(TempPage, NbPage, TempUser, CDirectory, SourcePath, DestinationPath);
        break;
    case 3:
        ConsultPage(TempPage, PageToDelete, *NbPage, TempUser, CDirectory, SourcePath, DestinationPath);
        break;
    case 4:
        ByeJournal(TempUser);
        break;
    default:
        printf("Invalid choice.\n");
    }
}

#endif