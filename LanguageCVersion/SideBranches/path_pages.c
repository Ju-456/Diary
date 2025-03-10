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
void menu(User *pUser, Page **page1, int *nb_pages, int PageToDelete);
void AgainJournal(User *pUser,Page **page1, int *nb_pages, int PageToDelete);

// User gestion part
void FolderCreation(char *FolderName, User *pUser, Page **page1,int *nb_pages, int PageToDelete);
void FolderExist(char *FolderName, User *pUser, Page **page1,int *nb_pages, int PageToDelete);
void DataUser(User *pUser, char *FolderName, Page **page1, int *nb_pages, int PageToDelete);

// Page gestion part
void CreatePage(User *pUser, Page **page1, int *nb_pages);
void DeleteProcess(Page **page1, int *nb_pages, User *pUser);
void ConsultPage(Page **page1, int PageToDelete, int nb_pages, User *pUser);

// Auxiliary functions
void ConsultPagepB(User *pUser, int *nb_pages, Page **page1, int PageToDelete);
void EnterPassword(Page *page1, int page_index, int *nb_pages, int PageToDelete, User *pUser);
void WriteInPage(Page *page1, int page_index);

// Invisible functions
void SaveToFile(int *nb_pages, Page **page1,User *pUser);
void BlockedAccesPage(Page **page1, int *nb_pages, int PageToDelete);

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

void FolderCreation(char *FolderName, User *pUser, Page **page1, int *nb_pages, int PageToDelete ) {
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", pUser->UIdentifiant, pUser->UPassword);
    //stocker dans le fichier UserData pUser->UIdentifiant et pUser->UPassword
    #ifdef _WIN32
        if (_mkdir(FolderName) == 0) {
            printf("Folder créé avec succès : %s\n", FolderName);
        } else {
            perror("Erreur lors de la création du dossier");
        }
    #else
        if (mkdir(FolderName, 777) == 0) {
            printf("Hello %s\n", FolderName);
            AgainJournal(pUser, page1, nb_pages, PageToDelete);
        } else {
            printf("You already have an account, please login:\n");
            //DataUser
        }
    #endif
}

void FolderExist(char *FolderName, User *pUser, Page **page1,int *nb_pages, int PageToDelete) {
    if (access(FolderName, F_OK) == 0) {
        printf("Account exists, logging in...\n");
        //LogInUser();
    } else {
        printf("So you don't have an account\n");
        printf("Do you want to create one? (yes/no): ");
        char response[4];
        scanf("%3s", response);
        if (strcmp(response, "yes") == 0) {
            FolderCreation(FolderName, pUser, page1, nb_pages, PageToDelete);
        } else {
            printf("Goodbye!\n");
            exit(0); 
        }
    }
}

void DataUser(User *pUser, char *FolderName, Page **page1,int *nb_pages, int PageToDelete) {
    char UPassword2[15];

    printf("User id: ");
    scanf("%14s", pUser->UIdentifiant);

    printf("Password: ");
    scanf("%14s", pUser->UPassword);

    printf("Confirm the password: ");
    scanf("%14s", UPassword2);

    if (strcmp(pUser->UPassword, UPassword2) == 0) {
        EnterJournal();
        FolderCreation(FolderName, pUser, page1, nb_pages, PageToDelete);
    } else {
        printf("Passwords don't match or maybe you don't have an account.\nPlease try again.\n");
        DataUser(pUser, FolderName, page1, nb_pages, PageToDelete);
    }
}



void CreatePage(User *pUser, Page **page1, int *nb_pages) {
    *page1 = realloc(*page1, (*nb_pages + 1) * sizeof(Page));
    if (*page1 == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }

    printf("Enter the date (format DD/MM/YYYY) : ");
    scanf("%10s", (*page1)[*nb_pages].date);

    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*page1)[*nb_pages].password);

    printf("Enter note for this page : ");
    getchar();
    fgets((*page1)[*nb_pages].note, SizeMaxPage, stdin);

    (*nb_pages)++;
    printf("The page was created successfully.\n");
    printf("You have just created page number %d and the current date is %s\n", *nb_pages, (*page1)[*nb_pages - 1].date);
    printf("\n");

    SaveToFile(nb_pages, page1,pUser);
}


void SaveToFile(int *nb_pages, Page **page1,User *pUser) {
    char filename[64];
    snprintf(filename, sizeof(filename), "/home/ju456/%s_%s/Page%d.txt", pUser->UIdentifiant, pUser->UPassword, *nb_pages);
    //snprintf(filename, sizeof(filename), "Page%d.txt", *nb_pages);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        // affiche ça 
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < *nb_pages; i++) {
        fprintf(file, "Date: %s\n", (*page1)[i].date);
        fprintf(file, "Note: %s\n", (*page1)[i].note); 
    }
    fclose(file);
}

void DeleteProcess(Page **page1, int *nb_pages, User *pUser) {
    int PageToDelete;

    printf("Which page do you want to delete? (number) : ");
    scanf("%d", &PageToDelete);

    if (PageToDelete > *nb_pages || PageToDelete <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    PageToDelete--;

    ConsultPagepB(pUser, nb_pages, page1, PageToDelete); 
}

void ConsultPagepB(User *pUser, int *nb_pages, Page **page1, int PageToDelete) {
    char reponse[4];

    if (strcmp((*page1)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(pUser,page1, nb_pages, PageToDelete);
        return;
    }

    EnterPassword(*page1, PageToDelete, nb_pages, PageToDelete, pUser);

    printf("Are you sure you want to delete page number %d (yes/no) : ", PageToDelete + 1);
    scanf("%s", reponse);
    if (strcmp(reponse, "yes") == 0) {
        for (int i = PageToDelete; i < *nb_pages - 1; i++) {
            (*page1)[i] = (*page1)[i + 1];
        }
        *page1 = realloc(*page1, (*nb_pages - 1) * sizeof(Page));
        if (*page1 == NULL && *nb_pages > 1) {
            printf("Error allocating memory.\n");
            exit(1);
        }
        (*nb_pages)--;
        printf("Page deleted successfully.\n");
    } else { 
        printf("Do you prefer to change the note? (yes/no) : ");
        scanf("%s", reponse);
        if (strcmp(reponse, "yes") == 0) {
            int page_index = PageToDelete;
            WriteInPage(*page1, page_index);
        } else {
            printf("Right, returning to the menu.\n");
            menu(pUser,page1, nb_pages, PageToDelete);
        }
    }
}

// A optimiser
void BlockedAccesPage(Page **page1, int *nb_pages, int PageToDelete) {
    char CopyBlockedPass[11];
    char CopyBlockedDate[11];
    char CopyBlockedNote[SizeMaxPage];

    // Copy intern of page's information
    // strncpy(source, destination, n);
    strncpy(CopyBlockedPass, (*page1)[PageToDelete].password, sizeof((*page1)[PageToDelete].password));
    strncpy(CopyBlockedDate, (*page1)[PageToDelete].date, sizeof((*page1)[PageToDelete].date));
    strncpy(CopyBlockedNote, (*page1)[PageToDelete].note, sizeof((*page1)[PageToDelete].note));

    // Page reset 
    // memset(source, remplacement, n); 
    strncpy((*page1)[PageToDelete].password, "MdpAdminOnly2104", sizeof((*page1)[PageToDelete].password)); // supprime
    memset((*page1)[PageToDelete].date, 0, sizeof((*page1)[PageToDelete].date)); 
    memset((*page1)[PageToDelete].note, 0, sizeof((*page1)[PageToDelete].note)); 

    printf("%s",CopyBlockedPass); // ça n'a pas marché 
}

void EnterPassword(Page *page1, int page_index, int *nb_pages, int PageToDelete, User *pUser) {
    char password[11];
    int TooLate = 0;
    printf("Enter the password for this page (be careful, you only have 3 attempts) : ");
    while (TooLate < 3) {
        printf("Try again : ");
        scanf("%10s", password);

        if (strcmp(page1[page_index].password, password) == 0) {
            printf("*Authorized access.*\n");
            printf("Date : %s\n", page1[page_index].date);
            printf("Note : %s\n", page1[page_index].note);
            return;
        } else {
            TooLate++;
            if (TooLate < 3) {
                printf("Wrong password. Be careful, you only have %d attempt(s) left.\n", 3 - TooLate);
            } else {
                printf("You can't access this page, this page was blocked\n");
                printf("\n");
                BlockedAccesPage(&page1, nb_pages, page_index);
                printf("Returning to the menu.\n");
                menu(pUser,&page1,nb_pages,PageToDelete);
            }
        }
    }
}
// a tester + tard
void ConsultPage(Page **page1, int PageToDelete, int nb_pages, User *pUser) {
    int page_requested;
    // ne rentre pas 
    if (strcmp((*page1)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(pUser,page1, &nb_pages, PageToDelete);
        return;
    }

    printf("Which page do you want to consult? (number) : ");
    scanf("%d", &page_requested);

    if (page_requested > nb_pages || page_requested <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }

    EnterPassword(*page1, page_requested - 1, &nb_pages, PageToDelete, pUser);
    printf("Do you want to write in this page? (yes/no) : ");
    char reponse[4];
    scanf("%s", reponse);
    if (strcmp(reponse, "yes") == 0) {
        WriteInPage(*page1, page_requested - 1);
    }
}

void WriteInPage(Page *page1, int page_index) {
    printf("Enter a new note for this page : ");
    getchar();
    fgets(page1[page_index].note, SizeMaxPage, stdin);
    printf("The note was changed successfully.\n");
}

void AgainJournal(User *pUser,Page **page1, int *nb_pages, int PageToDelete){
    while (1) {
        menu(pUser,page1, nb_pages, PageToDelete);

        printf("Do you want to do something else? (yes/no) : ");
        char again[4];
        scanf("%s", again);
        printf("\n");
        if (strcmp(again, "no") == 0) {
            ByeJournal();
        }
    }
}

void menu(User *pUser, Page **page1, int *nb_pages, int PageToDelete) {
    char reponse[4];

    printf("A - Create a page\n");
    printf("B - Delete a page\n");
    printf("C - Consult an existing page\n");
    printf("\n");

    printf("Enter your choice : ");
    scanf("%s", reponse);
    printf("\n");

    if (strcmp(reponse, "A") == 0) {
        CreatePage(pUser, page1, nb_pages);
    } else if (strcmp(reponse, "B") == 0) {
        DeleteProcess(page1, nb_pages, pUser);
    } else if (strcmp(reponse, "C") == 0) {
        ConsultPage(page1, PageToDelete, *nb_pages, pUser);
    } /*else if (strcmp(reponse, "IAmTheAdmin") == 0){
        AdminSpace(); Boléen accès page à créer plus tard
    }  */else {
        printf("Invalid option.\n");
    }
}

int main() {
    User pUser;
    char reponse1[4];
    Page *page1 = NULL;
    int nb_pages = 0;
    int PageToDelete = 0;
    char FolderName[1024];

// Tout à refaire voir interface user
    WelcomeJournal();
    while (1) {
       printf("Do you have an account? (yes/no): ");
    scanf("%3s", reponse1);
    if (strcmp(reponse1, "yes") == 0) {
        printf("Authentification...");
        DataUser(&pUser, FolderName, &page1, &nb_pages, PageToDelete);
    } else if (strcmp(reponse1, "no") == 0) {
        printf("Do you want to create an account? (yes/no): ");
            scanf("%3s", reponse1);{
            if (strcmp(reponse1, "yes") == 0){
                printf("Go to create an account !");
            } else {
                ByeJournal();
            }
        }
        FolderExist(FolderName, &pUser, &page1, &nb_pages, PageToDelete);
        //jsplus
    } else {
        printf("Invalid option.\n");
    }
    }

    free(page1);
    return 0;
}

