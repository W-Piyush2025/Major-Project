#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank_vault.h"
#define FILE_NAME "bank_database.txt"
// Just a header design
void printTitle() {
    printf("\n===================================\n");
    printf("     CITY BANK SECURE SYSTEM       \n");
    printf("===================================\n");
}
// SECURITY LOGIC: Shift Cipher (+3)
// Example: 'A' becomes 'D'
void shiftEncrypt(char *realPass, char *codedPass) {
    int i = 0;
    while (realPass[i] != '\0') {
        codedPass[i] = realPass[i] + 3; 
        i++;
    }
    codedPass[i] = '\0';
}
// SECURITY LOGIC: Decrypt (-3)
// Example: 'D' becomes 'A'
void shiftDecrypt(char *codedPass, char *realPass) {
    int i = 0;
    while (codedPass[i] != '\0') {
        realPass[i] = codedPass[i] - 3;
        i++;
    }
    realPass[i] = '\0';
}
void createAccount() {
    FILE *fp = fopen(FILE_NAME, "a"); // 'a' means append (add to end)
    Account user;
    char rawPass[50];
    if (fp == NULL) {
        printf("Error: Could not open database file.\n");
        return;
    }
    printf("\n--- OPEN NEW ACCOUNT ---\n");
    printf("Enter Account Number (e.g., 101): ");
    scanf("%d", &user.accNumber);
    printf("Enter Name: ");
    scanf("%s", user.name); 
    printf("Create a Password: ");
    scanf("%s", rawPass);
    printf("Enter Initial Deposit Amount: ");
    scanf("%f", &user.balance);
    // Encrypt password before saving
    shiftEncrypt(rawPass, user.encryptedPass);
    // VISUALIZATION: Show that we are securing it
    printf("\n[SYSTEM]: Encrypting password...\n");
    printf("   Input: %s  --->  Saved as: %s\n", rawPass, user.encryptedPass);
    // Save to file
    fprintf(fp, "%d %s %s %.2f\n", user.accNumber, user.name, user.encryptedPass, user.balance);
    fclose(fp);
    printf(">> Account created successfully!\n");
}
void loginAndBank() {
    int inputAcc, found = 0;
    char inputPass[50], decodedPass[50];
    Account user;
    FILE *fp = fopen(FILE_NAME, "r"); // 'r' means read
    if(fp == NULL) {
        printf("Error: No accounts exist yet!\n");
        return;
    }
    printf("\n--- CUSTOMER LOGIN ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &inputAcc);
    printf("Enter Password: ");
    scanf("%s", inputPass);
    // Loop through file to find the user
    while(fscanf(fp, "%d %s %s %f", &user.accNumber, user.name, user.encryptedPass, &user.balance) != EOF) {
        if(user.accNumber == inputAcc) {
            found = 1;
            // Decrypt the file password to check against input
            shiftDecrypt(user.encryptedPass, decodedPass);   
            if(strcmp(inputPass, decodedPass) == 0) {
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
    FILE *tempFp = fopen("temp.txt", "w"); // Temporary file to write updates   
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
    // Copy data from Old File -> New File
    while(fscanf(fp, "%d %s %s %f", &user.accNumber, user.name, user.encryptedPass, &user.balance) != EOF) {
        if (user.accNumber == targetAcc) {
            // Check if old password matches
            shiftDecrypt(user.encryptedPass, checkDecoded);
            if (strcmp(oldPass, checkDecoded) == 0) {
                found = 1;
                printf(">> Identity Verified.\n");
                printf("Enter NEW Password: ");
                scanf("%s", newPass);
                // Encrypt the new password
                shiftEncrypt(newPass, newEncrypted);               
                // Write NEW details to temp file
                fprintf(tempFp, "%d %s %s %.2f\n", user.accNumber, user.name, newEncrypted, user.balance);
                printf(">> Password Updated Successfully!\n");
            } else {
                // Wrong password, keep old data
                printf(">> Error: Old Password Incorrect. Changes cancelled.\n");
                fprintf(tempFp, "%d %s %s %.2f\n", user.accNumber, user.name, user.encryptedPass, user.balance);
            }
        } else {
            // Not the user? Just copy them as is.
            fprintf(tempFp, "%d %s %s %.2f\n", user.accNumber, user.name, user.encryptedPass, user.balance);
        }
    }
    fclose(fp);
    fclose(tempFp);
    // Swap the files
    if (found) {
        remove(FILE_NAME);
        rename("temp.txt", FILE_NAME);
    } else {
        remove("temp.txt");
        if(found == 0) printf(">> Account not found.\n");
    }
}