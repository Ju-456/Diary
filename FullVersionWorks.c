#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <linux/limits.h>

#define SizeMaxPage 256

typedef struct {
    char UId[15];
    char UPass[15];
} User;

typedef struct {
    char password[11];  // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

// Global function
void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory);
void menu(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory);

// User gestion part
void CurrentDirectory(char *CDirectory);
void CodeImplementation(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete);

void SignInUser(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete);
void LogInUser(FILE *file, char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);

void FolderCreation(char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);

// Page gestion part
void CreatePage(User *TempUser, Page **TempPage, int *NbPage,char *CDirectory);
void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser, char *CDirectory);
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, char *CDirectory);
void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, char *CDirectory);
void BlockedAccesPage(Page **TempPage, int *NbPage, int PageToDelete);

// Auxiliary functions
void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage, int PageToDelete,char *CDirectory);
void EnterPassword(Page *TempPage, int page_index, int *NbPage, User *TempUser, char *CDirectory);
void WriteInPage(Page *TempPage, int page_index);

// Decoration functions
void WelcomeJournal();
void PersonalAccess();
void ByeJournal();

void WelcomeJournal(){
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***_______the_______***\n***_____journal_____***\n*****_____:D______*****\n*******_________*******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}

void PersonalAccess(){
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***______your_______***\n***____personal_____***\n****_____space_____****\n*****_____^^_____******\n********_______********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}

void ByeJournal() {
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_______See_______***\n***_______you_______***\n***______later______***\n***_________________***\n*****_____Bye______****\n******____<3_____******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    exit(0);
}

void CurrentDirectory(char *CDirectory){
    if (getcwd(CDirectory, PATH_MAX) != NULL) {
       //printf("Current directory: %s\n", CDirectory);
   } else {
       perror("Problem of autorization, please fix it");
   }
    FILE *file = fopen("password.txt", "a");
    if (file == NULL) {
        printf("Failed to create password repository.\n");
    } else {
        fclose(file); 
    }
}

void CodeImplementation(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete){
    printf("Do you have an account? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);

    if (strcmp(answer, "no") == 0) {
        printf("Go to create an account!\n");
        SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete);
    } else {
        FILE *file = fopen("password.txt", "r");
        if (file != NULL) {
            LogInUser(file, CDirectory, TempUser, TempPage, NbPage, PageToDelete);
            fclose(file);
        } else {
            printf("Account file not found. Please create a new account.\n");
            SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete);
        }
    }
}

void LogInUser(FILE *file, char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete) {
    char answer[4];
    char VTempUserUId[15], VTempUserUPass[15];
    char PathUser[80];
    printf("Enter your user ID: ");
    scanf("%14s", VTempUserUId); 

    snprintf(PathUser, PATH_MAX, "%s/%s",CDirectory, VTempUserUId);
    
    if (access(PathUser, F_OK) != 0) { 
        printf("This ID doesn't exist.\nDo you want to create an account? (yes/no): ");
        scanf("%3s", answer); 

        if (strcmp(answer, "yes") == 0) {
            printf("Creating account...\n");
            SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete);
        } else {
            ByeJournal();
        }
    } else {
        printf("Enter the password: ");
        scanf("%14s", VTempUserUPass); 

        if (fscanf(file, "%14s %14s", TempUser->UId, TempUser->UPass) != EOF) {
            int TooLate = 0;

            while (strcmp(VTempUserUPass, TempUser->UPass) != 0 && TooLate < 3) {
                TooLate++;
                printf("Wrong password. You only have %d attempt(s) left.\n", 3 - TooLate);

                if (TooLate < 3) {
                    printf("Enter the password again: ");
                    scanf("%14s", VTempUserUPass);
                } else {
                    printf("Access blocked. Contact support.\n");
                    return; 
                }
            }

            if (strcmp(VTempUserUId, TempUser->UId) == 0 && strcmp(VTempUserUPass, TempUser->UPass) == 0) {
                PersonalAccess();
                menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory); 
            }
        }
    }
}

void SignInUser(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete){
    char UPass2[15];

    PersonalAccess();
    printf("Choose a user id: ");
    scanf("%14s", TempUser->UId);

    printf("Enter a password: ");
    scanf("%14s", TempUser->UPass);

    printf("Confirm the password: ");
    scanf("%14s", UPass2);
    printf("\n");

    if (strcmp(TempUser->UPass, UPass2) == 0) {
        FolderCreation(CDirectory, TempUser, TempPage, NbPage, PageToDelete);
    } else {
        printf("Passwords don't match.\nPlease try again.\n");
        SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete);
    }
}

#ifdef _WIN32
    #include <direct.h>
    #define CREATE_DIR(path) _mkdir(path)
#else
    #include <sys/types.h>
    #define CREATE_DIR(path) mkdir(path, 0777)
#endif

void FolderCreation(char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete) {
    char TempUserFoldPath[PATH_MAX];
    snprintf(TempUserFoldPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);

    if (CREATE_DIR(TempUserFoldPath) == 0) {
        //printf("User folder successfully created: %s\n", TempUserFoldPath);

        char passwordFilePath[PATH_MAX];
        snprintf(passwordFilePath, PATH_MAX, "%s/password.txt", CDirectory);

        FILE *file = fopen(passwordFilePath, "a");
        if (file == NULL) {
            perror("Error opening password.txt");
            return;
        }

        fprintf(file, "%s %s\n", TempUser->UId, TempUser->UPass);
        fclose(file);
        printf("User account successfully created.\n");

        menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
    } else {
        perror("Error creating user folder");
    }
}
/*
void CreatePage(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory) {
    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }

    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*TempPage)[*NbPage].password);

    printf("Write your page (max 256 characters): ");
    getchar();
    fgets((*TempPage)[*NbPage].note, SizeMaxPage, stdin);

    (*NbPage)++;
    SaveToFile(TempUser, NbPage, &TempPage, CDirectory);
}

void SaveToFile(User *TempUser, int *NbPage, Page **TempPage, char *CDirectory) {
    char userFolderPath[PATH_MAX];
    snprintf(userFolderPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);

    // if change to the user directory
    if (chdir(userFolderPath) != 0) {
        printf("Error changing to user directory.\n");
        return;
    }

    for (int i = 0; i < *NbPage; i++) {
        char PageFilePath[PATH_MAX];
        snprintf(PageFilePath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, i + 1);

        FILE *file = fopen(PageFilePath, "w");
        if (file == NULL) {
            printf("Error opening file for writing page %d.\n", i + 1);
            continue;
        }

        fprintf(file, "Page %d - \nContent:\n%s\n\n", i + 1, (*TempPage)[i].note);
        fclose(file);
    }

    printf("All pages saved successfully in individual files.\n");
}
*/

void CreatePage(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory) {
    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }

    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*TempPage)[*NbPage].password);

    printf("Write your page (max 256 characters): ");
    getchar();
    fgets((*TempPage)[*NbPage].note, SizeMaxPage, stdin);

    (*NbPage)++;
    SaveToFile(TempUser, NbPage, *TempPage, CDirectory);
}

void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, char *CDirectory) {
    char userFolderPath[PATH_MAX];
    snprintf(userFolderPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);

    if (chdir(userFolderPath) != 0) {
        printf("Error changing to user directory.\n");
        return;
    }

    for (int i = 0; i < *NbPage; i++) {
        char PageFilePath[PATH_MAX];
        snprintf(PageFilePath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, i + 1);

        FILE *file = fopen(PageFilePath, "w");
        if (file == NULL) {
            printf("Error opening file for writing page %d.\n", i + 1);
            continue;
        }

        fprintf(file, "Page %d - \nContent:\n%s\n\n", i + 1, TempPage[i].note);
        fclose(file);
    }

    printf("All pages saved successfully in individual files.\n");
}

void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser, char *CDirectory) {
    int PageToDelete;

    printf("Which page do you want to delete? (number): ");
    scanf("%d", &PageToDelete);

    if (PageToDelete > *NbPage || PageToDelete <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    PageToDelete--;

    ConsultPagepB(TempUser, NbPage, TempPage, PageToDelete, CDirectory);
}

void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage, int PageToDelete,char *CDirectory) {
    char answer[4];

    if (strcmp((*TempPage)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
        return;
    }
    EnterPassword(*TempPage, PageToDelete, NbPage, TempUser, CDirectory);

    printf("Are you sure you want to delete page number %d (yes/no): ", PageToDelete + 1);
    scanf("%3s", answer);
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
        printf("Do you prefer to change the note? (yes/no): ");
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0) {
            WriteInPage(*TempPage, PageToDelete);
        } else {
            printf("Right, returning to the menu.\n");
            menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
        }
    }
}

void BlockedAccesPage(Page **TempPage, int *NbPage, int PageToDelete) {
    char CopyBlockedPass[11];
    char CopyBlockedNote[SizeMaxPage];

    strncpy(CopyBlockedPass, (*TempPage)[PageToDelete].password, sizeof((*TempPage)[PageToDelete].password));
    strncpy(CopyBlockedNote, (*TempPage)[PageToDelete].note, sizeof((*TempPage)[PageToDelete].note));

    strncpy((*TempPage)[PageToDelete].password, "MdpAdminOnly2104", sizeof((*TempPage)[PageToDelete].password));
    memset((*TempPage)[PageToDelete].note, 0, sizeof((*TempPage)[PageToDelete].note));

    printf("Blocked password was: %s\n", CopyBlockedPass);
}

void EnterPassword(Page *TempPage, int page_index, int *NbPage, User *TempUser, char *CDirectory) {
    char password[11];
    int TooLate = 0;
    printf("Enter the password for this page (you have 3 attempts):\n ");
    while (TooLate > 3 ) {
        printf("Wrong password. You have %d attempt(s) left.\n", 3 - TooLate);
        scanf("%10s", password);

        if (strcmp(TempPage[page_index].password, password) == 0) {
            printf("*Authorized access.*\n");
            printf("Note: %s\n", TempPage[page_index].note);
            return;
        } else {
            TooLate++;
            if (TooLate < 3) {
                printf("Access to this page is blocked.\n");
                BlockedAccesPage(&TempPage, NbPage, page_index);
                printf("Returning to the menu.\n");
                menu(TempUser, &TempPage, NbPage, page_index, CDirectory);
            }
        }
    }
}

void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, char *CDirectory) {
    int page_requested;

    printf("Which page do you want to consult? (number): ");
    scanf("%d", &page_requested);

    if (page_requested > NbPage || page_requested <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    EnterPassword(*TempPage, page_requested - 1, &NbPage, TempUser, CDirectory);

    printf("Do you want to write in this page? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    if (strcmp(answer, "yes") == 0) {
        WriteInPage(*TempPage, page_requested - 1);
    }
}

void WriteInPage(Page *TempPage, int page_index) {
    time_t timestamp = time(NULL);
    struct tm *FrenchHour = localtime(&timestamp);
    
    printf("Enter a new note for this page: ");
    getchar();  
    fgets(TempPage[page_index].note, SizeMaxPage, stdin);
    
    printf("The note was changed successfully.\n");
}

void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory){
    while (1) {
        menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);

        printf("Do you want to do something else? (yes/no) : ");
        char answer[4];
        scanf("%s", answer);
        printf("\n");
        if (strcmp(answer, "yes") == 0) {
            menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
        } else {
            ByeJournal();
        }
    }
}

void menu(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory) {
    int choice = 0;
        printf("\n1. Create Page\n2. Delete Page\n3. Consult Page\n4. Quit\n\nChoose: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                CreatePage(TempUser, TempPage, NbPage, CDirectory);
                break;
            case 2:
                DeleteProcess(TempPage, NbPage, TempUser, CDirectory);
                break;
            case 3:
                ConsultPage(TempPage, PageToDelete, *NbPage, TempUser, CDirectory);
                break;
            case 4:
                ByeJournal();
                break;
            default:
                printf("Invalid choice.\n");
    }
}

int main() {
    User TempUser;
    Page *TempPage = NULL;
    int NbPage = 0;
    int PageToDelete = 0;
    char CDirectory[PATH_MAX];

    CurrentDirectory(CDirectory); 
    CodeImplementation(&TempUser, CDirectory, &TempPage, &NbPage, PageToDelete);

    WelcomeJournal();
    while(1){
        BoucleJournal(&TempUser, &TempPage, &NbPage, PageToDelete, CDirectory);
    }
    
    free(TempPage);
    return 0;
}
