#ifndef BANK_VAULT_H
#define BANK_VAULT_H
// This structure defines what a single user looks like
typedef struct {
    int accNumber;
    char name[50];
    char encryptedPass[50]; // We store the secret code here
    float balance;
} Account;
// Function Declarations
void printTitle();
void shiftEncrypt(char *realPass, char *codedPass);
void shiftDecrypt(char *codedPass, char *realPass);
// Main features
void createAccount();
void loginAndBank();
void changePassword();
#endif