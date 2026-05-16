#include <stdio.h>
#include <stdlib.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void deposit(FILE *fPtr);
void withdraw(FILE *fPtr);

int main()
{
    FILE *cfPtr;

    // open file or create if not exists
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        struct clientData blank = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        }
    }

    unsigned int choice;

    while ((choice = enterChoice()) != 9)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 5:
            displayAll(cfPtr);
            break;
        case 6:
            searchAccount(cfPtr);
            break;
        case 7:
            deposit(cfPtr);
            break;
        case 8:
            withdraw(cfPtr);
            break;
        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1 - Export to text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Display all accounts\n");
    printf("6 - Search account\n");
    printf("7 - Deposit\n");
    printf("8 - Withdraw\n");
    printf("9 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}

// display all accounts
void displayAll(FILE *fPtr)
{
    struct clientData client;
    rewind(fPtr);

    printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d%-16s%-11s%10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// search account
void searchAccount(FILE *fPtr)
{
    unsigned int acc;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    if (acc < 1 || acc > 100)
    {
        printf("Invalid account number\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Account not found\n");
    else
        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);
}

// deposit
void deposit(FILE *fPtr)
{
    unsigned int acc;
    double amount;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Enter amount: ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Deposit successful\n");
}

// withdraw
void withdraw(FILE *fPtr)
{
    unsigned int acc;
    double amount;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (client.balance < amount)
    {
        printf("Insufficient balance\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Withdrawal successful\n");
}

// create new record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int acc;

    printf("Enter account number (1-100): ");
    scanf("%u", &acc);

    if (acc < 1 || acc > 100)
    {
        printf("Invalid account number\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists\n");
        return;
    }

    printf("Enter lastname firstname balance: ");
    scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);

    client.acctNum = acc;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully\n");
}

// update record
void updateRecord(FILE *fPtr)
{
    unsigned int acc;
    double amount;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Current balance: %.2f\n", client.balance);

    printf("Enter amount (+credit / -debit): ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated balance: %.2f\n", client.balance);
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0};
    unsigned int acc;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted\n");
}

// export to text file
void textFile(FILE *readPtr)
{
    FILE *writePtr = fopen("accounts.txt", "w");
    struct clientData client;

    if (writePtr == NULL)
    {
        printf("File error\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    printf("Data exported to accounts.txt\n");
}
