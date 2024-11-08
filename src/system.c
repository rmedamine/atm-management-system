#include "header.h"
#include <ctype.h>

const char *TMP = "./data/tmp.txt";
const char *RECORDS = "./data/records.txt";
const char *TMPR = "./data/temporary.txt";
const char *USERS = "./data/users.txt";
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    // todo get last id 
    FILE *pf = fopen(RECORDS, "r");
    int id = -1;
    int currentId;


    if (pf == NULL)
    {
        perror("Failed to open file");
        return;
    }
    
    while (fscanf(pf, "%d", &currentId) == 1)
    {
        fscanf(pf, "%*[^\n]"); // Ignorer le reste de la ligne
        if (currentId > id)
        {
            id = currentId;
        }
    }
    id++;
    r.id = id;
    fclose(pf);
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void saveAccountToFileFromRecord(FILE *ptr, struct Record r)
{
     struct User u;
         fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            r.userId,
            r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

bool checkAlphabet(const char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (!isalpha(str[i]))

            return false;
        else
            return true;
    }
}

int checkDigit(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {

            return 0;
        }
    }
    return 1;
}

int is_valid_number(char *str)
{
    char *endptr;
    strtod(str, &endptr);
    if (*endptr != '\0')
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void createNewAcc(struct User u)
{
    printf("%d %s %s\n", u.id, u.name, u.password);
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");
    char tmprory[50];
   

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    if (r.deposit.day < 1 || r.deposit.day > 31 || r.deposit.month < 1 || r.deposit.month > 12 || r.deposit.year < 2000 || r.deposit.year > 2024)
    {
        printf("\ncheck date\n\n");
        return;
    }

    printf("\nEnter the account number:");
    scanf("%s", tmprory);
    if (!checkDigit(tmprory))
    {
        printf("erreur account \n\n");
        return;
    }
    r.accountNbr = atoi(tmprory);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    if (!checkAlphabet(r.country))
    {
        printf("\nmust erreur \n\n");
        return;
    }

    printf("\nEnter the phone number:");
    scanf("%s", tmprory);

    if (!checkDigit(tmprory))
    {
        printf("mal fait \n\n");
        return;
    }
    if (strlen(tmprory)>10){
        printf("erruer de phone \n\n");
        return;
    }
    r.phone = atoi(tmprory);
    printf("\nEnter amount to deposit: $");
    scanf("%s", tmprory);
    if (!is_valid_number(tmprory))
    {
        printf("\n mal erruer : \n\n");
        return;
    }
    r.amount = atof(tmprory);

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    if (!checkAlphabet(r.accountType))
    {
        printf("erreur type ! : \n\n");
        return;
    }
    char *options[5] = {"saving", "current", "fixed01", "fixed02", "fixed03"};
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(options[i], r.accountType) == 0)
        {

            break;
        }
        if (i == 4)
        {
            printf("\nerreur de choix \n\n");
            return;
        }
    }


    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void checkUpdates(struct User u)
{
    struct Record r;
    int accountNbr;
    FILE *record_file = fopen(RECORDS, "r+");
    FILE *tmp_file = fopen(TMP, "w");

    if (record_file == NULL || tmp_file == NULL)
    {
        printf("Erreur d'ouverture du fichier !\n");
        return;
    }

    printf("Entrez le numéro de compte à mettre à jour : ");
    if (scanf("%d", &accountNbr) != 1)
    {
        printf("Entrée invalide pour le numéro de compte !\n");
        fclose(record_file);
        fclose(tmp_file);
        return;
    }
    bool found = false;
    while (getAccountFromFile(record_file, r.name, &r))
    {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accountNbr)
        {
            found = true;
            printf("Choisissez ce que vous voulez mettre à jour :\n");
            printf("1 - Mettre à jour le pays\n");
            printf("2 - Mettre à jour le téléphone\n");
            int choice;
            if (scanf("%d", &choice) != 1)
            {
                printf("Entrée invalide pour le choix !\n");
                fclose(record_file);
                fclose(tmp_file);
                remove(TMP);
                return;
            }

            if (choice == 1)
            {
                printf("Entrez le nouveau pays : ");
                scanf("%s", r.country);
            }
            else if (choice == 2)
            {
                printf("Entrez le nouveau numéro de téléphone : ");
                scanf("%d", &r.phone);
            }
            else
            {
                printf("Choix invalide !\n");
                fclose(record_file);
                fclose(tmp_file);
                remove(TMP);
                return;
            }

            printf("✔ Mise à jour réussie !\n");
        }
        saveAccountToFileFromRecord(tmp_file, r);
    }

    if (!found)
    {
        printf("sorry  Compte non trouvé !\n");
    }
    else
    {
        remove(RECORDS);
        rename(TMP, RECORDS);
    }
    fclose(record_file);
    fclose(tmp_file);
    success(u);
}

void registerMenu(  char a[50], char pass[50])
{

    FILE *pf = fopen(USERS, "a+");
    struct User u;
    int id = -1;
    int currentId;

    if (pf == NULL)
    {
        perror("Failed to open file");
        return;
    }

    
    
    
    while (fscanf(pf, "%d", &currentId) == 1  )
    {
        fscanf(pf, "%*[^\n]"); // Ignorer le reste de la ligne
        if (currentId > id)
        {
            id = currentId;
        }
    }
    id++;
   // fseek(pf, 0, SEEK_END);
    fprintf(pf, "\n%d %s %s",
            id++,
            a,
            pass);

    fclose(pf);
}

void checkAccounts(struct User u)
{
    struct Record r;

    char usersforcheck[50];
    FILE *files = fopen(RECORDS, "r+");
    double savings = 7.0 / 100.0 / 12.0;
    double fixed01 = 4.0 / 100.0;
    double fixed02 = 5.0 / 50.0;
    double fixed03 = 3.0 * (8.0 / 100.0);

    int accountNbr;
    int found = 0;

    if (files == NULL)
    {
        printf("Erreur d'ouverture du fichier !\n");
        return;
    }
    printf("Enter the account number: ");
    if (scanf("%d", &accountNbr) != 1)
    {
        printf("Entrée invalide pour le numéro de compte !\n");
        fclose(files);
        return;
    }
    while (getAccountFromFile(files, usersforcheck, &r))
    {
        if (strcmp(usersforcheck, u.name) == 0 && r.accountNbr == accountNbr)
        {
            found = 1;

            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);

            if (strcmp(r.accountType, "current") == 0)
            {
                printf("\n\n-> you will not get interests because the account is of type current");
            }
            if (strcmp(r.accountType, "saving") == 0)
            {

                printf("\n\n-> you will get $ %.2f as interests on day 10 of every month\n", savings * r.amount);
            }
            if (strcmp(r.accountType, "fixed01") == 0)
            {

                printf("\n\n-> you will get $ %.2f as interests on day 10 of every month\n", fixed01 * r.amount);
            }
            if (strcmp(r.accountType, "fixed02") == 0)
            {

                printf("\n\n-> you will get $ %.2f as interests on day 10 of every month\n", fixed02 * r.amount);
            }
            if (strcmp(r.accountType, "fixed03") == 0)
            {

                printf("\n\n-> you will get $ %.2f as interests on %d/%d/%d\n", fixed03 * r.amount,r.deposit.day,r.deposit.month,r.deposit.year+3);
            }
        }
    }
    if (!found)
    {
        printf("sorry  Compte non trouvé !\n");
    }

    fclose(files);
    success(u);
}

void addOrremove(struct User u)
{
    struct Record r;
    FILE *record_file = fopen(RECORDS, "r+");
    FILE *temporary = fopen(TMPR, "w");

    bool found = false;
    double amount;
    int accountNbr;
    int choice;

    if (record_file == NULL || temporary == NULL)
    {
        printf("Erreur d'ouverture du fichier !\n");
        return;
    }

    printf("Entre the  account number of the customer : \n");

    if (scanf("%d", &accountNbr) != 1)
    {
        printf("Entrée invalide pour le numéro de compte !\n");
        fclose(record_file);
        fclose(temporary);

        return;
    }

    while (getAccountFromFile(record_file, r.name, &r))
    {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accountNbr)
        {
            if(strcmp(r.accountType,"fixed03")==0||strcmp(r.accountType,"fixed03")==0|| strcmp(r.accountType,"fixed01")==0){
                printf(" it is not possible to withdraw or deposit for fixed accounts?");
                fclose(record_file);
                fclose(temporary);
                return;

            }
            found = true;
            

            printf("Do you want to  :\n");
            printf("1 -> WithDraw \n");
            printf("2 -> Deposit \n");
            printf("\n\n Enter your choice \n");

            if (scanf("%d", &choice) != 1)
            {
                printf("Entrée invalide pour le choix !\n");
                fclose(record_file);
                fclose(temporary);
                remove(TMPR);
                return;
            }

            if (choice == 1)
            {

                printf("Enter the amount you what to withdraw : ");
                scanf("%lf", &amount);
                if (r.amount > amount)
                {

                    r.amount -= amount;
                }
                else
                {
                    printf("daber 3al 3a9a");
                    system("clear");
                }
            }
            else if (choice == 2)
            {
                printf("Enter the amount you what to Desposit : ");
                scanf("%lf", &amount);
                r.amount += amount;
            }
            else
            {
                printf("Choix invalide !\n");
                fclose(record_file);
                fclose(temporary);
                remove(TMPR);

                return;
            }
        }
        saveAccountToFileFromRecord(temporary, r);
    }
    if (!found)
    {
        printf("sorry  Compte non trouvé !\n");
    }

    fclose(record_file);
    fclose(temporary);

    remove(RECORDS);
    rename(TMPR, RECORDS);
    mainMenu(u);
}

void removeAccount(struct User u)
{
    struct Record r;
    FILE *tmp_file = fopen(TMPR, "w");
    FILE *files = fopen(RECORDS, "r+");
    // char usersforcheck[50];
    int accountNbr;
    int found = 0;
    int choice;

    if (files == NULL || tmp_file == NULL)
    {
        printf("Erreur d'ouverture du fichier !\n");
        if (files)
            fclose(files);
        if (tmp_file)
            fclose(tmp_file);
        return;
    }

    printf("Enter the account number: ");
    if (scanf("%d", &accountNbr) != 1)
    {
        printf("Entrée invalide pour le numéro de compte !\n");
        fclose(files);
        fclose(tmp_file);
        return;
    }

    while (getAccountFromFile(files, r.name, &r))
    {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accountNbr)
        {
            found = 1;

            system("clear");
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2f\nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            printf("\n\nAre you serious? Enter 1 to confirm, any other number to cancel: ");

            if (scanf("%d", &choice) != 1)
            {
                printf("Entrée invalide pour le choix !\n");
                fclose(files);
                fclose(tmp_file);
                return;
            }

            if (choice == 1)
            {
                printf("Account removed.\n");
                continue;
            }
            else
            {
                printf("Account removal cancelled.\n");
            }
        }
        saveAccountToFileFromRecord(tmp_file, r);
    }

    if (!found)
    {
        printf("Désolé, compte non trouvé !\n");
    }

    fclose(files);
    fclose(tmp_file);

    if (found && choice == 1)
    {
        remove(RECORDS);
        rename(TMPR, RECORDS);
    }
    else
    {
        remove(TMPR);
    }
    success(u);
}
void trasferData(struct User u)
{
    int usertotranser;
    struct Record r;
    FILE *useres = fopen(USERS, "r"); // Ouvrir en lecture pour vérifier l'existence de l'utilisateur
    FILE *recordes = fopen(RECORDS, "r");
    FILE *tmpfile = fopen(TMP, "w");
    char newusr[50]; // Pour stocker le nom du nouvel utilisateur
    int found = 0;

    if (useres == NULL || recordes == NULL || tmpfile == NULL)
    {
        printf("Erreur d'ouverture du fichier !\n");
        return;
    }

    printf("Le numéro de compte dont vous voulez transférer la propriété : \n");
    if (scanf("%d", &usertotranser) != 1)
    {
        printf("Entrée invalide pour le numéro de compte !\n");
        fclose(useres);
        fclose(recordes);
        fclose(tmpfile);
        return;
    }

    while (getAccountFromFile(recordes, r.name, &r))
    {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == usertotranser)
        {
            found = 1;

            printf("\t\t====== Transfert du compte : \n\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2f\nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);

            printf("À quel utilisateur voulez-vous transférer la propriété ? Entrez le nom d'utilisateur : ");
            scanf("%s", newusr);

            // Vérifier si l'utilisateur existe
            struct User newUser;
            int userExists = 0;

            // Réinitialiser le pointeur vers le début du fichier USERS
            rewind(useres);
            while (fscanf(useres, "%d %s %s", &newUser.id, newUser.name, newUser.password) != EOF)
            {
                if (strcmp(newusr, newUser.name) == 0)
                {
                    userExists = 1;
                    break;
                }
            }

            if (userExists)
            {
                // Mettre à jour le nom de l'utilisateur dans le record
                strcpy(r.name, newusr);
                printf("dahaba walam ya3od\n\n");
            }
            else
            {
                printf("L'utilisateur %s n'existe pas.\n", newusr);
                break;
            }
        }
        saveAccountToFileFromRecord(tmpfile, r);
    }

    if (!found)
    {
        printf("Désolé, compte non trouvé !\n");
    }

    fclose(recordes);
    fclose(useres);
    fclose(tmpfile);

    remove(RECORDS);
    rename(TMP, RECORDS);
}
