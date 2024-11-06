#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // Objet sys : (fichiers, répertoires, processus).
#include <sys/stat.h> // Fonctions : stat(), mkdir(), obt info file/folder + create repertory 
#include <unistd.h>  // manipulation files and folder
#include <dirent.h> // read and browse the folders
#include <time.h>

/*
Error opening file for writing. when cretate page ? 

*/
#define SizeMaxPage 256

typedef struct {
    char UIdentifiant[15];
    char UPassword[15];
} User;

typedef struct {
    // point de vie 3
    char password[11]; // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

// Global function
void menu(User *TempUser, Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete);
void BoucleJournal(User *TempUser,Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete);

// User gestion part
void DataUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete);
void LogInUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete);
void CreateAnUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete);

void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file, int PageToDelete);
void FolderExist(char *FolderName, User *TempUser, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete);

// Page gestion part
void CreatePage(User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file);
void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser, char *filename, FILE *file);
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, char *filename, FILE *file, User *TempUser);
void DateAndHour(FILE *file, int *NbPage);

// Auxiliary functions
void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage,char *filename, FILE *file, int PageToDelete);
void EnterPassword(Page *TempPage, int page_index, int *NbPage, char *filename, FILE *file, int PageToDelete, User *TempUser);
void WriteInPage(Page *TempPage, int page_index);

// Invisible functions
void SaveToFile(char *filename, FILE *file, int *NbPage, Page **TempPage,User *TempUser);
void BlockedAccesPage(Page **TempPage, int *NbPage, int PageToDelete);

// Decoration functions
void WelcomeJournal();
void PersonalAcces();
void ByeJournal();

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

void EnterJournal(){

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

void DataUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete) {

    printf("Enter an user id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Enter the password ");
    scanf("%49s", TempUser->UPassword);

    printf("\n");
    
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword); // changer mettre le mdp dans un dossier nommer mdp dans fichier
    if (access(FolderName, F_OK) == 0) { 
        printf("You already have an account!\nPlease log in here:");
        printf("\n");
        LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);// si compte identification
    } else {
        printf("This identifiant doesn't exist..\nDo you want to create an account (yes/no) ? ");
        char answer1[4];
        scanf("%3s", answer1);
        if (strcmp(answer1, "yes") == 0) {
            printf("Go to create an account!\n");
            CreateAnUser(TempUser, FolderName, TempPage, NbPage, filename, file, PageToDelete);// Si pas de compte, créer un compte
        } else {
            ByeJournal();
            exit(0);
        }
    }
}

void LogInUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete){

    printf("Enter your id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Enter your password: ");
    scanf("%49s", TempUser->UPassword);

    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword);
    if (access(FolderName, F_OK) == 0) {
        BoucleJournal(TempUser, TempPage, NbPage, filename, file, PageToDelete);
        EnterJournal();
    } else {
        int TooLate = 0;
        TooLate++;
        if (TooLate < 3) {
            printf("Wrong password. Be careful, you only have %d attempt(s) left.\n", 3 - TooLate);
            LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);
        } else {
            printf("You can't access this account, this account was blocked, please contact the support.\n");
            printf("\n");
            //BlockedAccesUser() à créér
            //début programme
        }
    }
}

void CreateAnUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete){

    char UPassword2[50];

    printf("Choose an user id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Enter a password: ");
    scanf("%49s", TempUser->UPassword);

    printf("Confirm the password: ");
    scanf("%49s", UPassword2);
    printf("\n");

    if (strcmp(TempUser->UPassword, UPassword2) == 0) {
        EnterJournal();
        FolderCreation(FolderName, TempUser, TempPage, NbPage, filename, file, PageToDelete);
    } else {
        printf("Passwords don't match.\nPlease try answer.\n");
        CreateAnUser(TempUser, FolderName, TempPage, NbPage, filename, file, PageToDelete);
    }
}

void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file, int PageToDelete ) {
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword);
    //stocker dans le fichier UserData TempUser->UIdentifiant et TempUser->UPassword
    #ifdef _WIN32
        if (_mkdir(FolderName) == 0) {
            printf("Folder créé avec succès : %s\n", FolderName);
        } else {
            perror("Erreur lors de la création du dossier");
        }
    #else
        if (mkdir(FolderName, 0777) == 0) { //766 pour empecher balade fichier 
            printf("Hello");
            //DateAndHour(); à l1 puis user écrit à l2
            BoucleJournal(TempUser, TempPage, NbPage, filename, file, PageToDelete);
        } else {
            printf("You already have an account, please login:\n");
            LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);
        }
    #endif
}

void FolderExist(char *FolderName, User *TempUser, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete) {
    if (access(FolderName, F_OK) == 0) {
        printf("Account exists, logging in...\n");
        LogInUser(TempUser, FolderName, TempPage, NbPage,filename, file, PageToDelete);
    } else {
        printf("So you don't have an account\n");
        printf("Do you want to create one? (yes/no): ");
        char response[4];
        scanf("%3s", response);
        if (strcmp(response, "yes") == 0) {
            FolderCreation(FolderName, TempUser, TempPage, NbPage, filename, file, PageToDelete);
        } else {
            printf("Goodbye!\n");
            exit(0); 
        }
    }
}

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

void SaveToFile(char *filename, FILE *file, int *NbPage, Page **TempPage,User *TempUser) {
    snprintf(filename, sizeof(filename), "/home/ju456/%s_%s/Page%d.txt", TempUser->UIdentifiant, TempUser->UPassword, *NbPage);
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < *NbPage; i++) {
        DateAndHour(file,NbPage);
        printf("Enter your note :");
        fprintf(file, "%s\n", (*TempPage)[i].note); 
    }
    fclose(file);
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

void DateAndHour(FILE *file, int *NbPage){
    time_t timestamp = time(NULL);
    struct tm *FrenchHour = localtime(&timestamp);
    fprintf(file, "Page%d\nDate : %02d/%02d/%04d\nAt : %02dh%02d:%02d\n ",
        *NbPage, FrenchHour->tm_mday, FrenchHour->tm_mon + 1, 1900 + FrenchHour->tm_year,
        FrenchHour->tm_hour, FrenchHour->tm_min, FrenchHour->tm_sec);
}

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

// A optimiser ou même changer le raisonnement bouléen
void BlockedAccesPage(Page **TempPage, int *NbPage, int PageToDelete) {
    char CopyBlockedPass[11];
    char CopyBlockedDate[11];
    char CopyBlockedNote[SizeMaxPage];

    // Copy intern of page's information
    // strncpy(source, destination, n);
    strncpy(CopyBlockedPass, (*TempPage)[PageToDelete].password, sizeof((*TempPage)[PageToDelete].password));
    //strncpy(CopyBlockedDate, (*TempPage)[PageToDelete].date, sizeof((*TempPage)[PageToDelete].date)); **time.h**
    strncpy(CopyBlockedNote, (*TempPage)[PageToDelete].note, sizeof((*TempPage)[PageToDelete].note));

    // Page reset 
    // memset(source, remplacement, n); 
    strncpy((*TempPage)[PageToDelete].password, "MdpAdminOnly2104", sizeof((*TempPage)[PageToDelete].password)); // supprime
    //memset((*TempPage)[PageToDelete].date, 0, sizeof((*TempPage)[PageToDelete].date)); **time.h**
    memset((*TempPage)[PageToDelete].note, 0, sizeof((*TempPage)[PageToDelete].note)); 

    printf("%s",CopyBlockedPass); // ça n'a pas marché 
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
                BlockedAccesPage(&TempPage, NbPage, page_index);
                printf("Returning to the menu.\n");
                menu(TempUser, &TempPage, NbPage, filename, file, PageToDelete);
            }
        }
    }
}
// a tester + tard
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, char *filename, FILE *file, User *TempUser) {
    int page_requested;
    // ne rentre pas 
    if (strcmp((*TempPage)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(TempUser, TempPage, &NbPage, filename, file, PageToDelete);
        return;
    }

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

void WriteInPage(Page *TempPage, int page_index) {
    printf("Enter a new note for this page : ");
    getchar();
    fgets(TempPage[page_index].note, SizeMaxPage, stdin);
    printf("The note was changed successfully.\n");
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

int main() {
    User TempUser;
    char answer1[4];
    Page *TempPage = NULL;
    int NbPage = 0;
    int PageToDelete = 0;
    char FolderName[1024];
    char filename[80];
    FILE *file = fopen(filename, "w");

    WelcomeJournal();
    while(1){
        DataUser(&TempUser, FolderName, &TempPage, &NbPage,filename, file, PageToDelete);
    }

    free(TempPage);
    return 0;
}