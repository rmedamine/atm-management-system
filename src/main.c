#include "header.h"


void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        checkUpdates(u);

        
        // student TODO : add your **Update account information** function
        // here
        break;
    case 3:
    checkAccounts(u);
        // student TODO : add your **Check the details of existing accounts** function
        // here
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
    addOrremove(u);
        // here
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
    removeAccount(u);
        // student TODO : add your **Remove existing account** function
        break;
    case 7:
    trasferData(u);
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
    // initMenu(&u);
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};

void initMenu(struct User *u)
{
    int r = 0;  //
    int option; //
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                u->id = getId(*u);
                mainMenu(*u);
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:

            loginMenu(u->name, u->password);
            if(strcmp(u->name,getUser(*u))==0){
                printf("erreur user exist  \n");
                return;
            }else {

            registerMenu(u->name, u->password);
            mainMenu(*u);
            }

            // student TODO : add your **Registration** function
            // here

            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
            exit(1);
        }
    }
};

int main()
{
    struct User u;

    initMenu(&u);
    // mainMenu(u);
    return 0;
}
