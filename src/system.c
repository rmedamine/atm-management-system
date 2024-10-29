#include "header.h"

const char *TMP = "./data/tmp.txt";
const char *RECORDS = "./data/records.txt";

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
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            r.id,
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

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

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
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

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
        return;
    }
    // printf("-------  %d\n", accountNbr);
    bool found = false;
    // Lire le fichier pour trouver le compte
    while (getAccountFromFile(record_file, r.name, &r))
    {
        // printf("-------  %s %d %d\n", u.name, strcmp(userName, u.name), r.accountNbr);
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

            // Mettre à jour le fichier...

            // print_record(r);
            printf("✔ Mise à jour réussie !\n");
        }
        saveAccountToFileFromRecord(tmp_file, r);
    }
    // printf("-------++++++++ %ld %ld\n", start_possition, end_possinttion);

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
}

//------------------ registration---------------

void registerMenu(char a[50], char pass[50])
{

    int id = 0;
    int currentId;
    FILE *pf = fopen(USERS, "r+");
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
    fseek(pf, 0, SEEK_END);
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
            printf("name %s \n", u.name);
            printf("name %s \n", usersforcheck);

            printf("accountnbr %d \n", accountNbr);

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

                printf("\n\n-> you will get $ %.2f as interests on day 10 of every month\n", fixed03 * r.amount);
            }
        }
    }
    if (!found)
    {
        printf("sorry  Compte non trouvé !\n");
    }
   
    fclose(files);
}

// void print_record(struct Record r)
// {
//     printf("%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
//            r.id,
//            r.id,
//            r.name,
//            r.accountNbr,
//            r.deposit.month,
//            r.deposit.day,
//            r.deposit.year,
//            r.country,
//            r.phone,
//            r.amount,
//            r.accountType);
// }