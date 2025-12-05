#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank_vault.h"

#define FILE_NAME "bank_database.txt"


void printTitle() {
    printf("\n===================================\n");
    printf("     CITY BANK SECURE SYSTEM       \n");
    printf("===================================\n");
}

// Check if an ID already exists to prevent duplicates
int isAccountExists(int id) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) return 0; // File doesn't exist yet, so account doesn't exist
    
    Account temp;
    while(fscanf(fp, "%d %[^\n]s %s %f", &temp.accNumber, temp.name, temp.encryptedPass, &temp.balance) != EOF) {
        if (temp.accNumber == id) {
            fclose(fp);
            return 1; // Found duplicate
        }
    }
    fclose(fp);
    return 0; // Not found
}

void shiftEncrypt(char *realPass, char *codedPass) {
    int i = 0;
    while (realPass[i] != '\0') {
        codedPass[i] = realPass[i] + 3; 
        i++;
    }
    codedPass[i] = '\0';
}

void shiftDecrypt(char *codedPass, char *realPass) {
    int i = 0;
    while (codedPass[i] != '\0') {
        realPass[i] = codedPass[i] - 3;
        i++;
    }
    realPass[i] = '\0';
}

void createAccount() {
    Account user;
    char rawPass[50];

    printf("\n--- OPEN NEW ACCOUNT ---\n");
    printf("Enter Account Number (e.g., 101): ");
    scanf("%d", &user.accNumber);

    if (isAccountExists(user.accNumber)) {
        printf(">> [ERROR]: Account %d already exists! Try again.\n", user.accNumber);
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]s", user.name); 

    printf("Create a Password: ");
    scanf("%s", rawPass);

    printf("Enter Initial Deposit Amount: ");
    scanf("%f", &user.balance);

    // Encrypt password
    shiftEncrypt(rawPass, user.encryptedPass);

    // VISUALIZATION
    printf("\n[SYSTEM]: Encrypting password...\n");
    printf("   Input: %s  --->  Saved as: %s\n", rawPass, user.encryptedPass);

    // Save to file
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL) {
        printf("Error: Could not open database file.\n");
        return;
    }
    
    for(int j=0; user.name[j]; j++) {
        if(user.name[j] == ' ') user.name[j] = '_';
    }

    fprintf(fp, "%d %s %s %.2f\n", user.accNumber, user.name, user.encryptedPass, user.balance);
    fclose(fp);
    printf(">> Account created successfully!\n");
}

void loginAndBank() {
    int inputAcc, found = 0;
    char inputPass[50], decodedPass[50];
    Account user;
    
    FILE *fp = fopen(FILE_NAME, "r");
    if(fp == NULL) {
        printf("Error: No accounts exist yet!\n");
        return;
    }

    printf("\n--- CUSTOMER LOGIN ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &inputAcc);
    printf("Enter Password: ");
    scanf("%s", inputPass);

    while(fscanf(fp, "%d %s %s %f", &user.accNumber, user.name, user.encryptedPass, &user.balance) != EOF) {
        if(user.accNumber == inputAcc) {
            found = 1;
            shiftDecrypt(user.encryptedPass, decodedPass);   
            
            if(strcmp(inputPass, decodedPass) == 0) {
                // Formatting name back to spaces for display
                for(int j=0; user.name[j]; j++) {
                    if(user.name[j] == '_') user.name[j] = ' ';
                }
                printf("\n[SYSTEM]: Password Verified. Access Granted.\n");
                printf("\nWelcome, %s!\n", user.name);
                printf("Your Current Balance: $%.2f\n", user.balance);
            } else {
                printf("\n[ERROR]: Wrong Password!\n");
            }
            break; 
        }
    }

    if(!found) {
        printf("\n[ERROR]: Account Number not found.\n");
    }
    fclose(fp);
}

void changePassword() {
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *tempFp = fopen("temp.txt", "w");   
    int targetAcc, found = 0;
    char oldPass[50], newPass[50], checkDecoded[50], newEncrypted[50];
    Account user;

    if (fp == NULL || tempFp == NULL) {
        printf("Error accessing system files.\n");
        return;
    }

    printf("\n--- CHANGE PASSWORD ---\n");
    printf("Confirm Account Number: ");
    scanf("%d", &targetAcc);
    printf("Enter Old Password: ");
    scanf("%s", oldPass);

    while(fscanf(fp, "%d %s %s %f", &user.accNumber, user.name, user.encryptedPass, &user.balance) != EOF) {
        if (user.accNumber == targetAcc) {
            shiftDecrypt(user.encryptedPass, checkDecoded);
            if (strcmp(oldPass, checkDecoded) == 0) {
                found = 1;
                printf(">> Identity Verified.\n");
                printf("Enter NEW Password: ");
                scanf("%s", newPass);
                
                shiftEncrypt(newPass, newEncrypted);               
                fprintf(tempFp, "%d %s %s %.2f\n", user.accNumber, user.name, newEncrypted, user.balance);
                printf(">> Password Updated Successfully!\n");
            } else {
                printf(">> Error: Old Password Incorrect. Changes cancelled.\n");
                fprintf(tempFp, "%d %s %s %.2f\n", user.accNumber, user.name, user.encryptedPass, user.balance);
            }
        } else {
            fprintf(tempFp, "%d %s %s %.2f\n", user.accNumber, user.name, user.encryptedPass, user.balance);
        }
    }
    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove(FILE_NAME);
        rename("temp.txt", FILE_NAME);
    } else {
        remove("temp.txt");
        if(found == 0) printf(">> Account not found or Password Incorrect.\n");
    }
}

// Main menu

int main() {
    int choice;
    printTitle();
    
    while(1) {
        printf("\n1. Create Account");
        printf("\n2. Login");
        printf("\n3. Change Password");
        printf("\n4. Exit");
        printf("\n>> Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: createAccount(); break;
            case 2: loginAndBank(); break;
            case 3: changePassword(); break;
            case 4: 
                printf("Exiting... Goodbye!\n");
                exit(0);
            default: printf("Invalid Choice.\n");
        }
    }
    return 0;
}