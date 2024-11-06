#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // Objet sys : (fichiers, répertoires, processus).
#include <sys/stat.h> // Fonctions : stat(), mkdir(), obt info file/folder + create repertory 
#include <unistd.h>  // manipulation files and folder
#include <dirent.h> // read and browse the folders

#define SizeMaxPage 256

typedef struct {
    char UIdentifiant[15];
    char UPassword[15];
} User;

typedef struct {
    // point de vie 3
    char date[11]; // JJ/MM/AAAA
    char password[11]; // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

// Global function
void menu(User *TempUser, Page **TempPage, int *NbPages, int PageToDelete);
void BoucleJournal(User *TempUser,Page **TempPage, int *NbPages, int PageToDelete);

// User gestion part
void DataUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPages, int PageToDelete);
void LogInUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPages, int PageToDelete);
void CreateAnUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPages, int PageToDelete);

void FolderCreation(char *FolderName, User *TempUser, Page **TempPage,int *NbPages, int PageToDelete);
void FolderExist(char *FolderName, User *TempUser, Page **TempPage,int *NbPages, int PageToDelete);

// Page gestion part
void CreatePage(User *TempUser, Page **TempPage, int *NbPages);
void DeleteProcess(Page **TempPage, int *NbPages, User *TempUser);
void ConsultPage(Page **TempPage, int PageToDelete, int NbPages, User *TempUser);

// Auxiliary functions
void ConsultPagepB(User *TempUser, int *NbPages, Page **TempPage, int PageToDelete);
void EnterPassword(Page *TempPage, int PageIndex, int *NbPages, int PageToDelete, User *TempUser);
void WriteInPage(Page *TempPage, int PageIndex);

// Invisible functions
void SaveToFile(int *NbPages, Page **TempPage,User *TempUser);
void BlockedAccesPage(Page **TempPage, int *NbPages, int PageToDelete);

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
  exit(0);

}

void DataUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPages, int PageToDelete) {

    printf("User id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Password: ");
    scanf("%49s", TempUser->UPassword);
    
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword);
    if (access(FolderName, F_OK) == 0) {
        printf("You already have an account!\nPlease log in here:\n");
        LogInUser(TempUser, FolderName, TempPage, NbPages, PageToDelete);// si compte identification
    } else {
        printf("This identifiant doesn't exist..\nDo you want to create an account(yes/no) ? ");
        char reponse1[4];
        scanf("%3s", reponse1);
        if (strcmp(reponse1, "yes") == 0) {
            printf("Go to create an account!\n");
            CreateAnUser(TempUser, FolderName, TempPage, NbPages, PageToDelete);// Si pas de compte, créer un compte
        } else {
            ByeJournal();
        }
    }
}

void LogInUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPages, int PageToDelete){

    printf("User id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Password: ");
    scanf("%49s", TempUser->UPassword);

    // Correction: Formatage avec snprintf et utilisation de access()
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword);
    if (access(FolderName, F_OK) == 0) {
        BoucleJournal(TempUser, TempPage, NbPages, PageToDelete);
        EnterJournal();
    } else {
        int TooLate = 0;
        TooLate++;
        if (TooLate < 3) {
            printf("Wrong password. Be careful, you only have %d attempt(s) left.\n", 3 - TooLate);
            LogInUser(TempUser, FolderName, TempPage, NbPages, PageToDelete);
        } else {
            printf("You can't access this account, this account was blocked, please contact the support.\n");
            printf("\n");
            //BlockedAccesUser() à créér
            //début programme
        }
    }
}

void CreateAnUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPages, int PageToDelete){

    char UPassword2[50];

    printf("Choose an user id: ");
    scanf("%49s", TempUser->UIdentifiant);

    printf("Enter a password: ");
    scanf("%49s", TempUser->UPassword);

    printf("Confirm the password: ");
    scanf("%49s", UPassword2);

    if (strcmp(TempUser->UPassword, UPassword2) == 0) {
        EnterJournal();
        FolderCreation(FolderName, TempUser, TempPage, NbPages, PageToDelete);
    } else {
        printf("Passwords don't match.\nPlease try again.\n");
        CreateAnUser(TempUser, FolderName, TempPage, NbPages, PageToDelete);
    }
}

void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPages, int PageToDelete ) {
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", TempUser->UIdentifiant, TempUser->UPassword);
    //stocker dans le fichier UserData TempUser->UIdentifiant et TempUser->UPassword
    #ifdef _WIN32
        if (_mkdir(FolderName) == 0) {
            printf("Folder créé avec succès : %s\n", FolderName);
        } else {
            perror("Erreur lors de la création du dossier");
        }
    #else
        if (mkdir(FolderName, 777) == 0) {
            printf("Hello %s\n", TempUser->UIdentifiant);
            BoucleJournal(TempUser, TempPage, NbPages, PageToDelete);
        } else {
            printf("You already have an account, please login:\n");
            DataUser(TempUser, FolderName, TempPage, NbPages, PageToDelete);
        }
    #endif
}

void FolderExist(char *FolderName, User *TempUser, Page **TempPage,int *NbPages, int PageToDelete) {
    if (access(FolderName, F_OK) == 0) {
        printf("Account exists, logging in...\n");
        LogInUser(TempUser, FolderName, TempPage, NbPages, PageToDelete);
    } else {
        printf("it doesn't work..\n");
        printf("Do you want to create one (yes/no) ? ");
        char response[4];
        scanf("%3s", response);
        if (strcmp(response, "yes") == 0) {
            FolderCreation(FolderName, TempUser, TempPage, NbPages, PageToDelete);
        } else {
            printf("Goodbye!\n");
            exit(0); 
        }
    }
}

void CreatePage(User *TempUser, Page **TempPage, int *NbPages) {
    *TempPage = realloc(*TempPage, (*NbPages + 1) * sizeof(Page));
    if (*TempPage == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }

    printf("Enter the date (format DD/MM/YYYY) : ");
    scanf("%10s", (*TempPage)[*NbPages].date);

    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*TempPage)[*NbPages].password);

    printf("Enter note for this page : ");
    getchar();
    fgets((*TempPage)[*NbPages].note, SizeMaxPage, stdin);

    (*NbPages)++;
    printf("The page was created successfully.\n");
    printf("You have just created page number %d and the current date is %s\n", *NbPages, (*TempPage)[*NbPages - 1].date);
    printf("\n");

    SaveToFile(NbPages, TempPage,TempUser);
}


void SaveToFile(int *NbPages, Page **TempPage,User *TempUser) {
    char filename[64];
    snprintf(filename, sizeof(filename), "/home/ju456/%s_%s/Page%d.txt", TempUser->UIdentifiant, TempUser->UPassword, *NbPages);
    //snprintf(filename, sizeof(filename), "Page%d.txt", *NbPages);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        // affiche ça 
        //printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < *NbPages; i++) {
        fprintf(file, "Date: %s\n", (*TempPage)[i].date);
        fprintf(file, "Note: %s\n", (*TempPage)[i].note); 
    }
    fclose(file);
}

void DeleteProcess(Page **TempPage, int *NbPages, User *TempUser) {
    int PageToDelete;

    printf("Which page do you want to delete? (number) : ");
    scanf("%d", &PageToDelete);

    if (PageToDelete > *NbPages || PageToDelete <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    PageToDelete--;

    ConsultPagepB(TempUser, NbPages, TempPage, PageToDelete); 
}

void ConsultPagepB(User *TempUser, int *NbPages, Page **TempPage, int PageToDelete) {
    char reponse[4];

    if (strcmp((*TempPage)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(TempUser,TempPage, NbPages, PageToDelete);
        return;
    }

    EnterPassword(*TempPage, PageToDelete, NbPages, PageToDelete, TempUser);

    printf("Are you sure you want to delete page number %d (yes/no) : ", PageToDelete + 1);
    scanf("%s", reponse);
    if (strcmp(reponse, "yes") == 0) {
        for (int i = PageToDelete; i < *NbPages - 1; i++) {
            (*TempPage)[i] = (*TempPage)[i + 1];
        }
        *TempPage = realloc(*TempPage, (*NbPages - 1) * sizeof(Page));
        if (*TempPage == NULL && *NbPages > 1) {
            printf("Error allocating memory.\n");
            exit(1);
        }
        (*NbPages)--;
        printf("Page deleted successfully.\n");
    } else { 
        printf("Do you prefer to change the note? (yes/no) : ");
        scanf("%s", reponse);
        if (strcmp(reponse, "yes") == 0) {
            int PageIndex = PageToDelete;
            WriteInPage(*TempPage, PageIndex);
        } else {
            printf("Right, returning to the menu.\n");
            menu(TempUser,TempPage, NbPages, PageToDelete);
        }
    }
}

// A optimiser
void BlockedAccesPage(Page **TempPage, int *NbPages, int PageToDelete) {
    char CopyBlockedPass[11];
    char CopyBlockedDate[11];
    char CopyBlockedNote[SizeMaxPage];

    // Copy intern of page's information
    // strncpy(source, destination, n);
    strncpy(CopyBlockedPass, (*TempPage)[PageToDelete].password, sizeof((*TempPage)[PageToDelete].password));
    strncpy(CopyBlockedDate, (*TempPage)[PageToDelete].date, sizeof((*TempPage)[PageToDelete].date));
    strncpy(CopyBlockedNote, (*TempPage)[PageToDelete].note, sizeof((*TempPage)[PageToDelete].note));

    // Page reset 
    // memset(source, remplacement, n); 
    strncpy((*TempPage)[PageToDelete].password, "MdpAdminOnly2104", sizeof((*TempPage)[PageToDelete].password)); // supprime
    memset((*TempPage)[PageToDelete].date, 0, sizeof((*TempPage)[PageToDelete].date)); 
    memset((*TempPage)[PageToDelete].note, 0, sizeof((*TempPage)[PageToDelete].note)); 

    printf("%s",CopyBlockedPass); // ça n'a pas marché 
}

void EnterPassword(Page *TempPage, int PageIndex, int *NbPages, int PageToDelete, User *TempUser) {
    char password[11];
    int TooLate = 0;
    printf("Enter the password for this page (be careful, you only have 3 attempts) : ");
    while (TooLate < 3) {
        printf("Try again : ");
        scanf("%10s", password);

        if (strcmp(TempPage[PageIndex].password, password) == 0) {
            printf("*Authorized access.*\n");
            printf("Date : %s\n", TempPage[PageIndex].date);
            printf("Note : %s\n", TempPage[PageIndex].note);
            return;
        } else {
            TooLate++;
            if (TooLate < 3) {
                printf("Wrong password. Be careful, you only have %d attempt(s) left.\n", 3 - TooLate);
            } else {
                printf("You can't access this page, this page was blocked\n");
                printf("\n");
                BlockedAccesPage(&TempPage, NbPages, PageIndex);
                printf("Returning to the menu.\n");
                menu(TempUser,&TempPage,NbPages,PageToDelete);
            }
        }
    }
}
// a tester + tard
void ConsultPage(Page **TempPage, int PageToDelete, int NbPages, User *TempUser) {
    int page_requested;
    // ne rentre pas 
    if (strcmp((*TempPage)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(TempUser,TempPage, &NbPages, PageToDelete);
        return;
    }

    printf("Which page do you want to consult? (number) : ");
    scanf("%d", &page_requested);

    if (page_requested > NbPages || page_requested <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }

    EnterPassword(*TempPage, page_requested - 1, &NbPages, PageToDelete, TempUser);
    printf("Do you want to write in this page? (yes/no) : ");
    char reponse[4];
    scanf("%s", reponse);
    if (strcmp(reponse, "yes") == 0) {
        WriteInPage(*TempPage, page_requested - 1);
    }
}

void WriteInPage(Page *TempPage, int PageIndex) {
    printf("Enter a new note for this page : ");
    getchar();
    fgets(TempPage[PageIndex].note, SizeMaxPage, stdin);
    printf("The note was changed successfully.\n");
}

void BoucleJournal(User *TempUser,Page **TempPage, int *NbPages, int PageToDelete){
    while (1) {
        menu(TempUser, TempPage, NbPages, PageToDelete);

        printf("Do you want to do something else? (yes/no) : ");
        char again[4];
        scanf("%s", again);
        printf("\n");
        if (strcmp(again, "yes") == 0) {
            menu(TempUser, TempPage, NbPages, PageToDelete);
        } else {
            ByeJournal();
        }
    }
}

void menu(User *TempUser, Page **TempPage, int *NbPages, int PageToDelete) {
    char reponse[4];

    printf("A - Create a page\n");
    printf("B - Delete a page\n");
    printf("C - Consult an existing page\n");
    printf("\n");

    printf("Enter your choice : ");
    scanf("%s", reponse);
    printf("\n");

    if (strcmp(reponse, "A") == 0) {
        CreatePage(TempUser, TempPage, NbPages);
    } else if (strcmp(reponse, "B") == 0) {
        DeleteProcess(TempPage, NbPages, TempUser);
    } else if (strcmp(reponse, "C") == 0) {
        ConsultPage(TempPage, PageToDelete, *NbPages, TempUser);
    } /*else if (strcmp(reponse, "IAmTheAdmin") == 0){
        AdminSpace(); Boléen accès page à créer plus tard
    }  */else {
        printf("Invalid option.\n");
    }
}

int main() {
    User TempUser;
    char reponse1[4];
    Page *TempPage = NULL;
    int NbPages = 0;
    int PageToDelete = 0;
    char FolderName[1024];

    WelcomeJournal();
    while(1){
        DataUser(&TempUser, FolderName, &TempPage, &NbPages, PageToDelete);
    }

    free(TempPage);
    return 0;
}