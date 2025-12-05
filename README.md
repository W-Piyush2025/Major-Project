# Major-Project
========================================================================
PROJECT TITLE:   Bank Management System with Password Encryption
LANGUAGE:        C Programming
AUTHOR:          [Your Name Here]
DATE:            December 2025
========================================================================

## 1. PROJECT DESCRIPTION
This project is a secure Bank Account Management System built in C. 
Unlike standard management systems, this project focuses on DATA SECURITY. 
It implements a custom encryption algorithm to ensure that user passwords 
stored in the database file are not readable by unauthorized persons.

## 2. KEY FEATURES
- **Secure Account Creation:** Users can create an account with a name and password.
- **Password Encryption:** Passwords are encrypted before being saved to the text file.
- **Login Verification:** Users can only access their balance if the decrypted password matches.
- **Change Password:** A secure method to update passwords by modifying records in the file.
- **Data Persistence:** All data is saved in 'bank_database.txt'.

## 3. FILE STRUCTURE
The project is modular and consists of 3 main files:
1. main.c        -> Contains the main menu and program loop.
2. bank_vault.c  -> Contains the logic for encryption, file handling, and banking functions.
3. bank_vault.h  -> The header file containing structure definitions and function prototypes.

## 4. HOW TO COMPILE AND RUN
Since the project uses multiple files, you must compile the C files together.

Option A: Using Command Prompt / Terminal (GCC)
   1. Open the terminal in the project folder.
   2. Type the following command:
      gcc main.c bank_vault.c -o bank_app
   3. Run the application:
      ./bank_app

Option B: Using an IDE (Dev-C++, CodeBlocks, VS Code)
   1. Create a new C Project.
   2. Add 'main.c', 'bank_vault.c', and 'bank_vault.h' to the project.
   3. Click "Build and Run".

## 5. ENCRYPTION LOGIC USED
This project uses a "Shift Cipher" (a variation of the Caesar Cipher) for simplicity and speed.

- **Encryption:**
  We shift every character of the password forward by 3 positions in the ASCII table.
  Example: 'A' -> 'D', 'B' -> 'E'
  
- **Decryption:**
  When reading from the file, we shift the character backward by 3 positions to restore the original password.

## 6. PROJECT OUTPUT (Example)
[SYSTEM]: Encrypting password...
   Input: apple  --->  Saved as: dssoh

This ensures that if someone opens 'bank_database.txt', they see "dssoh" instead of "apple".

========================================================================