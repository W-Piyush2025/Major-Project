#include <stdio.h>
#include <stdlib.h>
#include "bank_vault.h"
int main() {
    int choice;
    while(1) {
        printTitle();
        printf("1. Open New Account\n");
        printf("2. Login to Check Balance\n");
        printf("3. Change Password\n");
        printf("4. Exit Bank\n");
        printf("\nSelect Option: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                loginAndBank();
                break;
            case 3:
                changePassword();
                break;
            case 4:
                printf("Exiting System... Stay Secure!\n");
                exit(0);
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}