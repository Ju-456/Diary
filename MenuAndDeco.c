#include "MenuAndDeco.h"

void menu(User *TempUser, Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete) {
    char answer[4];

    printf("A - Create a page\n");
    printf("B - Delete a page\n");
    printf("C - Consult an existing page\n");
    printf("\n");

    printf("Enter your choice : ");
    scanf("%s", answer);
    printf("\n");

    if (strcmp(answer, "A") == 0) {
        CreatePage(TempUser, TempPage, NbPage, filename, file);
    } else if (strcmp(answer, "B") == 0) {
        DeleteProcess(TempPage, NbPage, TempUser, filename, file);
    } else if (strcmp(answer, "C") == 0) {
        ConsultPage(TempPage, PageToDelete, *NbPage,filename, file, TempUser);
    } /*else if (strcmp(answer, "IAmTheAdmin") == 0){
        AdminSpace(); Boléen accès page à créer plus tard
    }  */else {
        printf("Invalid option.\n");
    }
}

void BoucleJournal(User *TempUser,Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete){
    while (1) {
        menu(TempUser, TempPage, NbPage, filename, file, PageToDelete);

        printf("Do you want to do something else? (yes/no) : ");
        char answer[4];
        scanf("%s", answer);
        printf("\n");
        if (strcmp(answer, "yes") == 0) {
            menu(TempUser, TempPage, NbPage, filename, file, PageToDelete);
        } else {
            ByeJournal();
        }
    }
}

void WelcomeJournal(){
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***_______the_______***\n***_____journal_____***\n*****_____:D______*****\n*******_________*******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}

void PersonalAccess(){
<<<<<<< HEAD

    printf("***********************\n");
    printf("***********_***********\n");
    printf("*********_____*********\n");
    printf("*******_________*******\n");
    printf("*****_____________*****\n");
    printf("***_____Welcome_____***\n");
    printf("***_______to________***\n");
    printf("***______your_______***\n");
    printf("***____personal_____***\n");
    printf("****_____space_____****\n");
    printf("*****_____^^_____******\n");
    printf("********_______********\n");
    printf("**********___**********\n");
    printf("***********_***********\n");
    printf("***********************\n");
=======
>>>>>>> 2cde619d4590a0f7bc451cb70a228689f87e4e4c

    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***______your_______***\n***____personal_____***\n****_____space_____****\n*****_____^^_____******\n********_______********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");

}

void ByeJournal() {
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_______See_______***\n***_______you_______***\n***______later______***\n***_________________***\n*****_____Bye______****\n******____<3_____******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}
