#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>
#include <stdbool.h>
#define MAX 500
#define DATAFILE "E:\\FILE.txt"
#define DIRECTORY "E:\\"
#define SLASH"|"

typedef struct account
{
    char accno[6];
    char name[31];
    char bday[10];
    char phone[13];
    int initial;
    char pin[8];
} REC;

typedef struct list
{
    REC costumer[MAX];
    int last;
} LIST;

LIST L;

REC user;
REC user2;

char pin[8];
char accno[31];

int main()
{
    insertcard();
    retrieve();
    int choice;
    system("cls"); system("COLOR B4");
    gotoxy(41,3); printf("*****************************************");
    gotoxy(43,4); printf("=== SWIFT BANK OF THE PHILIPINNES ===");
    gotoxy(52,6); printf("[1] Registration");
    gotoxy(52,8); printf("[2] Transaction");
    gotoxy(49,10); printf("Enter your choice: [ ]");
    gotoxy(41,11); printf("*****************************************");
    gotoxy(69,10); scanf("%d", &choice);
    if (choice==1)
    {
         registration();
    }
    else if (choice == 2)
    {
         transaction();
    }
    else
    {
        printf("[CHOICE INVALID, PLEASE TRY AGAIN]");
        system("pause");
        return main();
    }
}


void transaction_log()
{
    system("cls"); system("COLOR 74");
    gotoxy(35,1); printf("**************************************************");
    gotoxy(53,2); printf("=== LOG-IN ===");
    gotoxy(35,3); printf("**************************************************");

    gotoxy(35,4); printf("__________________________________________________");
    gotoxy(37,6); printf("Enter your account number: "); scanf("%s",&accno);
    gotoxy(37,9); printf("Enter your pin: ");
    int index = 0;
    char ch;
    while ((ch = getch()) != 13 && index < 5)
    {
        if (index < 0)
        {
            index = 0;
        }

        if (ch == 8)
        {
            putch('\b');
            putch(' ');
            putch('\b');
            index--;
            continue;
        }

        if (isdigit(ch))
        {
            pin[index++] = ch;
            putch('*');
        }
    }
    printf("\n");
    index = -1;
    for (int i = 0; i <= L.last; i++)
    {
        if (strcmp(L.costumer[i].accno, accno) == 0)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        gotoxy(48,12); printf("Account number not found.\n");
        gotoxy(48,14); system("pause");
        return;
    }
    gotoxy(35,10); printf("__________________________________________________");
    if (strcmp(L.costumer[index].pin, pin) != 0)
    {
        gotoxy(48,12); printf("Incorrect pin.\n");
        gotoxy(48,14); printf("%s %s\n", L.costumer[index].pin, pin);
        return;
    }
    user = L.costumer[index];
    gotoxy(48,12); system("pause");
}

void encrypt()
{
    int i = 0;
    while (pin[i] != '\0')
    {
        pin[i] = pin[i] + 77;
        i++;
    }
}

void decrypt()
{
    int i = 0;
    while (pin[i] != '\0')
    {
        pin[i] = pin[i] - 77;
        i++;
    }
}

void insertcard()
{
    FILE *fp;
    do
    {
        system("cls");
        gotoxy(48,12); printf("Please insert card...");
        fp = fopen(DATAFILE, "r");
        usleep(500);
    } while (fp == NULL);
    fclose(fp);

}

void insertcard1()
{
    FILE *fp;
    do
    {
        system("cls");
        gotoxy(48,12); printf("Please insert card...");
        fp = fopen(DATAFILE, "r");
        usleep(500);
    } while (fp == NULL);
    fclose(fp);
    transaction_log();
}

void makenull()
{
    L.last = -1;
}

int isEmpty()
{
    return (L.last == -1);
}

int isFull()
{
    return (L.last == MAX - 1);
}

void add(REC r)
{
    if (isFull())
    {
        gotoxy(48,12); printf("List is full.\n");
        gotoxy(48,13); system("pause");
    }
    else
    {
        L.last++;
        L.costumer[L.last] = r;
    }
}

void retrieve()
{
    makenull();
    FILE *fp = fopen(DATAFILE, "r");
    if (fp != NULL)
    {
        char line[1024];
        while (fgets(line, 1024, fp))
        {
            REC latest_data;
            char *token = strtok(line, SLASH);
            int i = 0;
            while (token != NULL)
            {
                switch (i)
                {
                case 0:
                    strcpy(latest_data.accno, token);
                    break;
                case 1:
                    strcpy(latest_data.name, token);
                    break;
                case 2:
                    strcpy(latest_data.bday, token);
                    break;
                case 3:
                    strcpy(latest_data.phone, token);
                    break;
                case 4:
                    latest_data.initial = atof(token);
                    break;
                case 5:
                    strcpy(pin, token);
                    decrypt();
                    strcpy(latest_data.pin, pin);
                    break;
                }
                token = strtok(NULL, SLASH);
                i++;
            }
            add(latest_data);
        }
    }
    strcpy(pin, "");
}

void save()
{
    FILE *fp = fopen(DATAFILE, "w");
    REC r;
    for (int i = 0; i <= L.last; i++)
    {
        r = L.costumer[i];
        strcpy(pin, r.pin);
        encrypt();
        strcpy(r.pin, pin);
        fprintf(fp, "%s|%s|%s|%s|%d|%s\n", r.accno, r.name, r.bday, r.phone, r.initial, r.pin);
    }
    fclose(fp);
}

void balance_inquiry()
{
    system("cls"); system("COLOR 74");
    gotoxy(35,1); printf("**************************************************");
    gotoxy(48,2); printf("=== BALANCE INQUIRY ===");
    gotoxy(35,3); printf("**************************************************");
    gotoxy(47,5); printf("ACCOUNT NAME: %s\n", user.name);
    gotoxy(47,6); printf("ACCOUNT NUMBER: %s\n", user.accno);
    gotoxy(35,7); printf("__________________________________________________");
    gotoxy(42,9); printf("Your current balance is PHP %d\n", user.initial);
    gotoxy(35,10); printf("__________________________________________________");
    gotoxy(42,12); system("pause");
}

void withdraw()
{
    int amount;
    system("cls"); system("COLOR 74");
    gotoxy(35,1); printf("**************************************************");
    gotoxy(48,2); printf("=== WITHDRAWAL FORM ===");
    gotoxy(35,3); printf("**************************************************");
    gotoxy(47,5); printf("ACCOUNT NAME: %s\n", user.name);
    gotoxy(47,6); printf("ACCOUNT NUMBER: %s\n", user.accno);
    gotoxy(35,7); printf("__________________________________________________");
    gotoxy(42,9); printf("Enter amount to withdraw: PHP ");
    gotoxy(35,10); printf("__________________________________________________");
    gotoxy(72,9);scanf("%d", &amount);
    if (amount > user.initial)
    {
        gotoxy(42,11); printf("Insufficient initial. Please try again.\n");
        gotoxy(42,13); system("pause");
    }

    else if (amount < 100)
    {
        gotoxy(42,11); printf("[SORRY, YOU CANNOT WITHDRAW CASH LOWER THAN 100]");
        gotoxy(42,13); system("pause");
    }
    else
    {
        user.initial -= amount;
        for (int i = 0; i <= L.last; i++)
        {
            if (strcmp(L.costumer[i].accno, user.accno) == 0)
            {
                L.costumer[i] = user;
                break;
            }
        }
        save();
        gotoxy(42,12); printf("[YOU SUCCESSFULLY WITHDRAW PHP %d]",amount);
        gotoxy(46,14);system("pause");
    }
}

void deposit()
{
    system("cls"); system("COLOR 74");
    int amount;
    gotoxy(35,1); printf("**************************************************");
    gotoxy(50,2); printf("=== DEPOSIT FORM ===");
    gotoxy(35,3); printf("**************************************************");
    gotoxy(47,5); printf("ACCOUNT NAME: %s\n", user.name);
    gotoxy(47,6); printf("ACCOUNT NUMBER: %s\n", user.accno);
    gotoxy(35,7); printf("__________________________________________________");
    gotoxy(42,9); printf("Enter amount to deposit: PHP ");
    gotoxy(35,10); printf("__________________________________________________");
    gotoxy(72,9); scanf("%d", &amount);
    if (amount < 100)
    {
        printf("[SORRY, YOU CANNOT DEPOSIT PHP 100 BELOW]");
        system("pause");
    }
    user.initial += amount;
    for (int i = 0; i <= L.last; i++)
    {
        if (strcmp(L.costumer[i].accno, user.accno) == 0)
        {
            L.costumer[i] = user;
            break;
        }
    }
    gotoxy(42,12); printf("[YOU SUCCESSFULLY DEPOSITED PHP %d]",amount);
    save();
    gotoxy(46,14);system("pause");
}

void fund_transfer()
{
    system("cls"); system("COLOR 74");
    char accno[20];
    int amount;
    gotoxy(35,1); printf("**************************************************");
    gotoxy(50,2); printf("=== FUND TRANSFER ===");
    gotoxy(35,3); printf("**************************************************");
    gotoxy(47,5); printf("ACCOUNT NAME: %s\n", user.name);
    gotoxy(47,6); printf("ACCOUNT NUMBER: %s\n", user.accno);
    gotoxy(35,8); printf("__________________________________________________");
    gotoxy(42,9); printf("Enter account number to transfer: ");
    gotoxy(55,10); scanf("%s", &accno);
    gotoxy(42,12); printf("Enter amount to transfer: PHP ");
    gotoxy(35,13); printf("__________________________________________________");
    gotoxy(73,12); scanf("%d", &amount);
    if (amount > user.initial)
    {
        gotoxy(40,15); printf("Insufficient balance. Please try again.");
        gotoxy(45,17); system("pause");
    }

    else if (amount< 100)
    {
        gotoxy(40,15); printf("[SORRY, YOU CANNOT TRANSFER PHP 100 BELOW]");
        gotoxy(45,17); system("pause");
    }
    else
    {
        int index = -1;
        for (int i = 0; i <= L.last; i++)
        {
            if (strcmp(L.costumer[i].accno, accno) == 0 && strcmp(L.costumer[i].accno, user.accno) != 0)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            gotoxy(38,15); printf("[ACCOUNT NUMBER NOT FOUND, PLEASE TRY AGAIN]");
            gotoxy(45,17);system("pause");
        }
        else
        {
            user.initial -= amount;
            L.costumer[index].initial += amount;
            for (int i = 0; i <= L.last; i++)
            {
                if (strcmp(L.costumer[i].accno, user.accno) == 0)
                {
                    L.costumer[i] = user;
                    break;
                }
            }
            save();

            gotoxy(45,15); printf("[FUND TRANSFER SUCCESSFUL]");
            gotoxy(45,17);system("pause");
        }
    }
}

void change_pin()
{
    char newpin[10];
    system("cls"); system("COLOR 74");
    gotoxy(35,1); printf("**************************************************");
    gotoxy(50,2); printf("=== CHANGE PIN ===");
    gotoxy(35,3); printf("**************************************************");

    gotoxy(47,5); printf("ACCOUNT NAME: %s\n", user.name);
    gotoxy(47,6); printf("ACCOUNT NUMBER: %s\n", user.accno);
    gotoxy(35,7); printf("__________________________________________________");
    gotoxy(42,9); printf("Enter new PIN: ");
    gotoxy(35,10); printf("__________________________________________________");
    gotoxy(57,9); scanf("%s", &newpin);
    strcpy(user.pin, newpin);
    for (int i = 0; i <= L.last; i++)
    {
        if (strcmp(L.costumer[i].accno, user.accno) == 0)
        {
            L.costumer[i] = user;
            break;
        }
    }
    save();
    gotoxy(45,12); printf("[PIN CHANGED SUCCESSFULLY]");
    gotoxy(45,14); system("pause");
}

void registration()
{
    system("cls"); system("COLOR 74");
    REC latest_data;
    gotoxy(35,1); printf("**************************************************");
    gotoxy(48,2); printf("=== REGISTRATION ===");
    gotoxy(35,3); printf("**************************************************");

    gotoxy(35,4); printf("__________________________________________________");
    gotoxy(37,6); printf("ACCOUNT NUMBER (4-6 NUMBERS:) ");
    gotoxy(68,6); scanf("%s", &latest_data.accno);
    gotoxy(37,9); printf("NAME: ");scanf(" %[^\n]s", &latest_data.name);
    gotoxy(37,12); printf("BIRTHDAY: "); scanf("%s", &latest_data.bday);
    gotoxy(37,15); printf("CONTACT NUMBER: "); scanf("%s", &latest_data.phone);
    gotoxy(37,18); printf("INTIAL DEPOSIT: "); scanf("%d",&latest_data.initial);
    gotoxy(37,21);  printf("CREATE PIN: "); scanf("%s", &latest_data.pin);
    gotoxy(35,22); printf("__________________________________________________");
    if (latest_data.initial < 5000)
    {
        gotoxy(36,23); printf("REGISTRATION FAILED | MINIMUM INITIAL DEPOSIT SHOULD BE PHP 5000");
        gotoxy(48,24); system("pause");
        registration();
    }
    else
    {
        add(latest_data);
        save();
        char dfile[50];
        strcpy(dfile, DIRECTORY);
        strcat(dfile, latest_data.accno);
        strcat(dfile, latest_data.pin);
        strcat(dfile, ".code");
        FILE *fp = fopen(dfile, "w");
        fclose(fp);
        gotoxy(48,24); printf("[REGISTRATION SUCCESSFULL]");
        gotoxy(48,25); system("pause");
    }
}

void transaction()
{
    insertcard1();
    system("cls");
    if (user.accno[0] == '\0')
    {
        return;
    }
    int choice;
    do
    {
        system("cls");
        gotoxy(35,1); printf("**************************************************");
        gotoxy(48,2); printf("=== TRANSACTION MODE ===");
        gotoxy(35,3); printf("**************************************************");
        gotoxy(47,5); printf("ACCOUNT NAME: %s\n", user.name);
        gotoxy(35,6); printf("__________________________________________________");
        gotoxy(49,8); printf("[1] Balance Inquiry\n");
        gotoxy(49,10); printf("[2] Withdraw\n");
        gotoxy(49,12); printf("[3] Deposit\n");
        gotoxy(49,14); printf("[4] Fund Transfer\n");
        gotoxy(49,16); printf("[5] Change Pin\n");
        gotoxy(49,18); printf("[6] Exit\n");
        gotoxy(49,20); printf("Enter your choice: [ ]");
        gotoxy(35,21); printf("__________________________________________________");
        gotoxy(69,20); scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            balance_inquiry();
            break;
        case 2:
            withdraw();
            break;
        case 3:
            deposit();
            break;
        case 4:
            fund_transfer();
            break;
        case 5:
            change_pin();
            break;
        case 6:
            printf("Thank you for banking us! ");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            system("pause");
        }
    } while (choice != 6);
}


void gotoxy(int x, int y)
{
    COORD a = {0,0};
    a.X=x;
    a.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

