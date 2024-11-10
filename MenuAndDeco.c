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
    
    printf("***********************\n");
    printf("***********_***********\n");
    printf("*********_____*********\n");
    printf("*******_________*******\n");
    printf("*****_____________*****\n");
    printf("***_____Welcome_____***\n");
    printf("***_______to________***\n");
    printf("***_______the_______***\n");
    printf("***_____journal_____***\n");
    printf("*****_____:D______*****\n");
    printf("*******_________*******\n");
    printf("*********_____*********\n");
    printf("**********___**********\n");
    printf("***********_***********\n");
    printf("***********************\n");

    printf("\n");

}

void PersonalAccess(){

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

    printf("\n");

}

void ByeJournal() {

  printf("***********************\n");
  printf("***********_***********\n");
  printf("*********_____*********\n");
  printf("*******_________*******\n");
  printf("*****_____________*****\n");
  printf("***_______See_______***\n");
  printf("***_______you_______***\n");
  printf("***______later______***\n");
  printf("***_________________***\n");
  printf("*****_____Bye______****\n");
  printf("******____<3_____******\n");
  printf("*********_____*********\n");
  printf("**********___**********\n");
  printf("***********_***********\n");
  printf("***********************\n");

  printf("\n");

}