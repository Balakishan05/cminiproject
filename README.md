# cminiproject
This Bank Account Management System is developed in C using binary file handling. It stores customer details such as account number, name, and balance in a file named `credit.dat`. The program supports operations like adding, updating, searching, deleting, depositing, and withdrawing accounts using a simple menu-driven interface.
# Bank Account Management System in C

## Overview
This project is a simple Bank Account Management System developed in C using binary file handling. The program allows users to create, update, search, delete, deposit, and withdraw account records using random-access file processing.

---

## Features

- Add new bank accounts
- Update account balance
- Delete accounts
- Search account by account number
- Deposit money
- Withdraw money
- Display all accounts
- Export account details to a text file

---

## Technologies Used

- C Programming Language
- File Handling
- Structures
- Random Access Files

---

## File Used

### credit.dat
Binary file used to store account records permanently.

### accounts.txt
Text file generated when exporting records.

---

## Structure Used

```c
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};
