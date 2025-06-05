#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 101 // Maximum number of records
#define MAX_ID_LENGTH 11 // Maximum length for IDs
#define MAX_NAME_LENGTH 51 // Maximum length for names
#define MAX_ADDRESS_LENGTH 101 // Maximum length for addresses
#define MAX_EMAIL_LENGTH 51 // Maximum length for email addresses
#define MAX_CONTACT_LENGTH 21 // Maximum length for contact numbers
#define MAX_DESCRIPTION_LENGTH 201 // Maximum length for descriptions
#define MAX_DATE_LENGTH 11 // Maximum length for date
#define MAX_PASSWORD_LENGTH 21 // Maximum length for passwords

// Structure definitions

typedef struct {
    char adminID[MAX_ID_LENGTH];    // PK
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char contactNumber[MAX_CONTACT_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Admin;

typedef struct {
    char customerID[MAX_ID_LENGTH];    // PK
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char contactNumber[MAX_CONTACT_LENGTH];
} Customer;

typedef struct {
    char deliveryID[MAX_ID_LENGTH];    // PK
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char contactNumber[MAX_CONTACT_LENGTH];
} Logistic;

typedef struct {
    char categoryID[MAX_ID_LENGTH];    // PK
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
} Category;

typedef struct {
    char productID[MAX_ID_LENGTH];    // PK
    char categoryID[MAX_ID_LENGTH];    // FK
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
    char description[MAX_DESCRIPTION_LENGTH];
} Inventory;

typedef struct {
    char supplierID[MAX_ID_LENGTH];    // PK
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char contactNumber[MAX_CONTACT_LENGTH];
} Supplier;

typedef struct {    // Purchase transaction
    char transactionID[MAX_ID_LENGTH];   // PK
    char adminID[MAX_ID_LENGTH];    // FK
    char productID[MAX_ID_LENGTH];   // FK
    char supplierID[MAX_ID_LENGTH];  // FK
    char date[MAX_DATE_LENGTH];
    int quantity;
    float totalAmount;
} Transaction;

typedef struct {    // Sales transaction
    char orderID[MAX_ID_LENGTH];    // PK
    char adminID[MAX_ID_LENGTH];    // FK
    char customerID[MAX_ID_LENGTH]; // FK
    char productID[MAX_ID_LENGTH];  // FK
    char deliveryID[MAX_ID_LENGTH]; // FK
    char date[MAX_DATE_LENGTH];
    int quantity;
    float totalAmount;
} Order;

// Maximum records for each structure

Admin admins[MAX_RECORDS];
Customer customers[MAX_RECORDS];
Logistic logistics[MAX_RECORDS];
Category categories[MAX_RECORDS];
Inventory inventories[MAX_RECORDS];
Supplier suppliers[MAX_RECORDS];
Transaction transactions[MAX_RECORDS];
Order orders[MAX_RECORDS];

// File operations

void checkFileExist(const char *filename){ // Function to check if a file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file) { // File exists
        fclose(file);   // Close file
    } 
    else {
        file = fopen(filename, "w");  // Open file with filename in write mode
        if (file) { // If file opened successfully
            fclose(file);   // Close file
        } 
        else {
            printf("Error creating file: %s\n", filename);  // Print error message
            exit(EXIT_FAILURE); // Exit program with failure status
        }
    }
}

// Admin functions

void addDataAdmin(const char *filename, const char *adminID, const char *name, const char *address, const char *email, const char *contactNumber, const char *password){  // Function to add data to admin file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Admin newAdmin; // Create new admin structure
    strncpy(newAdmin.adminID, adminID, MAX_ID_LENGTH - 2);  // Copy value to adminID
    newAdmin.adminID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newAdmin.name, name, MAX_NAME_LENGTH - 2);  // Copy value to name
    newAdmin.name[MAX_NAME_LENGTH - 1] = '\0';  // Null terminate the string
    strncpy(newAdmin.address, address, MAX_ADDRESS_LENGTH - 2); // Copy value to address
    newAdmin.address[MAX_ADDRESS_LENGTH - 1] = '\0';    // Null terminate the string
    strncpy(newAdmin.email, email, MAX_EMAIL_LENGTH - 2);   // Copy value to email
    newAdmin.email[MAX_EMAIL_LENGTH - 1] = '\0';    // Null terminate the string
    strncpy(newAdmin.contactNumber, contactNumber, MAX_CONTACT_LENGTH - 2); // Copy value to contactNumber
    newAdmin.contactNumber[MAX_CONTACT_LENGTH - 1] = '\0';  // Null terminate the string
    strncpy(newAdmin.password, password, MAX_PASSWORD_LENGTH - 2); // Copy value to password
    newAdmin.password[MAX_PASSWORD_LENGTH - 1] = '\0'; // Null terminate the string

    fprintf(file, "%s,%s,%s,%s,%s,%s\n", newAdmin.adminID, newAdmin.name, newAdmin.address, newAdmin.email, newAdmin.contactNumber, newAdmin.password);   // Write new admin data to file
    fclose(file);   // Close file
    printf("Admin data added successfully.\n");
}

void readDataAdmin(const char *filename, const char *attribute, const char *adminID){ // Function to read data from admin file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if admin is found
    int attributeFound = 1; // Flag to check if attribute is found
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", admins[i].adminID, admins[i].name, admins[i].address, admins[i].email, admins[i].contactNumber, admins[i].password);   // Split record's content and read admin data from file
        if (admins[i].adminID[0] == '\0') { // If adminID is empty, break the loop
            break;
        }
        if (strcmp(admins[i].adminID, adminID) == 0) {    // Compare adminID with value
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                printf("Name: %s\n", admins[i].name); // Print name
            }
            else if (strcmp(attribute, "address") == 0) {   // Compare attribute with address
                printf("Address: %s\n", admins[i].address); // Print address
            }
            else if (strcmp(attribute, "email") == 0) { // Compare attribute with email
                printf("Email: %s\n", admins[i].email); // Print email
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                printf("Contact Number: %s\n", admins[i].contactNumber);    // Print contact number
            }
            else if (strcmp(attribute, "password") == 0) { // Compare attribute with password
                printf("Password: %s\n", admins[i].password);    // Print password
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If admin is found and attribute is found
        printf("Admin data read successfully.\n");
    } 
    else {
        printf("Admin with adminID %s not found.\n", adminID);
    }
}

void updateDataAdmin(const char *filename, const char *attribute, const char *adminID, const char *newValue){   // Function to update data in admin file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if admin is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", admins[i].adminID, admins[i].name, admins[i].address, admins[i].email, admins[i].contactNumber, admins[i].password);   // Split record's content and read admin data from file
        if (admins[i].adminID[0] == '\0') { // If adminID is empty, break the loop
            break;
        }
        if (strcmp(admins[i].adminID, adminID) == 0) {  // Compare adminID with adminID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                strncpy(admins[i].name, newValue, MAX_NAME_LENGTH - 2); // Copy new value to name
                admins[i].name[MAX_NAME_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "address") == 0) {   // Compare attribute with address
                strncpy(admins[i].address, newValue, MAX_ADDRESS_LENGTH - 2); // Copy new value to address
                admins[i].address[MAX_ADDRESS_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "email") == 0) {  // Compare attribute with email
                strncpy(admins[i].email, newValue, MAX_EMAIL_LENGTH - 2); // Copy new value to email
                admins[i].email[MAX_EMAIL_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                strncpy(admins[i].contactNumber, newValue, MAX_CONTACT_LENGTH - 2); // Copy new value to contactNumber
                admins[i].contactNumber[MAX_CONTACT_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "password") == 0) { // Compare attribute with password
                strncpy(admins[i].password, newValue, MAX_PASSWORD_LENGTH - 2); // Copy new value to password
                admins[i].password[MAX_PASSWORD_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s,%s\n", admins[i].adminID, admins[i].name, admins[i].address, admins[i].email, admins[i].contactNumber, admins[i].password);  // Write updated admin data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If admin is found and attribute is found
        printf("Admin data updated successfully.\n");
    }
    else {
        printf("Admin with adminID %s not found.\n", adminID);
    }
}

void deleteDataAdmin(const char *filename, const char *adminID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if admin is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", admins[i].adminID, admins[i].name, admins[i].address, admins[i].email, admins[i].contactNumber, admins[i].password);   // Split record's content and read admin data from file
        if (admins[i].adminID[0] == '\0') { // If adminID is empty, break the loop
            break;
        }
        if (strcmp(admins[i].adminID, adminID) == 0) {  // Compare adminID with adminID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s,%s\n", admins[i].adminID, admins[i].name, admins[i].address, admins[i].email, admins[i].contactNumber, admins[i].password);  // Write admin data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If admin is found
        printf("Admin data deleted successfully.\n");
    } 
    else {
        printf("Admin with adminID %s not found.\n", adminID);
    }
}

// Customer functions

void addDataCustomer(const char *filename, const char *customerID, const char *name, const char *address, const char *email, const char *contactNumber){  // Function to add data to customer file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Customer newCustomer; // Create new customer structure
    strncpy(newCustomer.customerID, customerID, MAX_ID_LENGTH - 2);  // Copy value to customerID
    newCustomer.customerID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newCustomer.name, name, MAX_NAME_LENGTH - 2);  // Copy value to name
    newCustomer.name[MAX_NAME_LENGTH - 1] = '\0';  // Null terminate the string
    strncpy(newCustomer.address, address, MAX_ADDRESS_LENGTH - 2); // Copy value to address
    newCustomer.address[MAX_ADDRESS_LENGTH - 1] = '\0';    // Null terminate the string
    strncpy(newCustomer.email, email, MAX_EMAIL_LENGTH - 2);   // Copy value to email
    newCustomer.email[MAX_EMAIL_LENGTH - 1] = '\0';    // Null terminate the string
    strncpy(newCustomer.contactNumber, contactNumber, MAX_CONTACT_LENGTH - 2); // Copy value to contactNumber
    newCustomer.contactNumber[MAX_CONTACT_LENGTH - 1] = '\0';  // Null terminate the string
    fprintf(file, "%s,%s,%s,%s,%s\n", newCustomer.customerID, newCustomer.name, newCustomer.address, newCustomer.email, newCustomer.contactNumber);   // Write new customer data to file
    fclose(file);   // Close file
    printf("Customer data added successfully.\n");
}

void readDataCustomer(const char *filename, const char *attribute, const char *customerID){ // Function to read data from customer file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if customer is found
    int attributeFound = 1; // Flag to check if attribute is found
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", customers[i].customerID, customers[i].name, customers[i].address, customers[i].email, customers[i].contactNumber);   // Split record's content and read customer data from file
        if (customers[i].customerID[0] == '\0') { // If customerID is empty, break the loop
            break;
        }
        if (strcmp(customers[i].customerID, customerID) == 0) {    // Compare customerID with value
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                printf("Name: %s\n", customers[i].name); // Print name
            }
            else if (strcmp(attribute, "address") == 0) {   // Compare attribute with address
                printf("Address: %s\n", customers[i].address); // Print address
            }
            else if (strcmp(attribute, "email") == 0) { // Compare attribute with email
                printf("Email: %s\n", customers[i].email); // Print email
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                printf("Contact Number: %s\n", customers[i].contactNumber);    // Print contact number
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If admin is found and attribute is found
        printf("Admin data read successfully.\n");
    } 
    else {
        printf("Customer with customerID %s not found.\n", customerID);
    }
}

void updateDataCustomer(const char *filename, const char *attribute, const char *customerID, const char *newValue){   // Function to update data in customer file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if customer is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", customers[i].customerID, customers[i].name, customers[i].address, customers[i].email, customers[i].contactNumber);   // Split record's content and read customer data from file
        if (customers[i].customerID[0] == '\0') { // If customerID is empty, break the loop
            break;
        }
        if (strcmp(customers[i].customerID, customerID) == 0) {  // Compare customerID with customerID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                strncpy(customers[i].name, newValue, MAX_NAME_LENGTH - 2); // Copy new value to name
                customers[i].name[MAX_NAME_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "address") == 0) {   // Compare attribute with address
                strncpy(customers[i].address, newValue, MAX_ADDRESS_LENGTH - 2); // Copy new value to address
                customers[i].address[MAX_ADDRESS_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "email") == 0) {  // Compare attribute with email
                strncpy(customers[i].email, newValue, MAX_EMAIL_LENGTH - 2); // Copy new value to email
                customers[i].email[MAX_EMAIL_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                strncpy(customers[i].contactNumber, newValue, MAX_CONTACT_LENGTH - 2); // Copy new value to contactNumber
                customers[i].contactNumber[MAX_CONTACT_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s\n", customers[i].customerID, customers[i].name, customers[i].address, customers[i].email, customers[i].contactNumber);  // Write updated customer data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If customer is found and attribute is found
        printf("Customer data updated successfully.\n");
    }
    else {
        printf("Customer with customerID %s not found.\n", customerID);
    }
}

void deleteDataCustomer(const char *filename, const char *customerID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if customer is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", customers[i].customerID, customers[i].name, customers[i].address, customers[i].email, customers[i].contactNumber);   // Split record's content and read customer data from file
        if (customers[i].customerID[0] == '\0') { // If customerID is empty, break the loop
            break;
        }
        if (strcmp(customers[i].customerID, customerID) == 0) {  // Compare customerID with customerID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s\n", customers[i].customerID, customers[i].name, customers[i].address, customers[i].email, customers[i].contactNumber);  // Write customer data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If customer is found
        printf("Customer data deleted successfully.\n");
    } 
    else {
        printf("Customer with customerID %s not found.\n", customerID);
    }
}

// Logistic functions

void addDataLogistic(const char *filename, const char *deliveryID, const char *name, const char *email, const char *contactNumber){  // Function to add data to logistic file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Logistic newLogistic; // Create new logistic structure
    strncpy(newLogistic.deliveryID, deliveryID, MAX_ID_LENGTH - 2);  // Copy value to deliveryID
    newLogistic.deliveryID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newLogistic.name, name, MAX_NAME_LENGTH - 2);  // Copy value to name
    newLogistic.name[MAX_NAME_LENGTH - 1] = '\0';  // Null terminate the string
    strncpy(newLogistic.email, email, MAX_EMAIL_LENGTH - 2);   // Copy value to email
    newLogistic.email[MAX_EMAIL_LENGTH - 1] = '\0';    // Null terminate the string
    strncpy(newLogistic.contactNumber, contactNumber, MAX_CONTACT_LENGTH - 2); // Copy value to contactNumber
    newLogistic.contactNumber[MAX_CONTACT_LENGTH - 1] = '\0';  // Null terminate the string
    fprintf(file, "%s,%s,%s,%s\n", newLogistic.deliveryID, newLogistic.name, newLogistic.email, newLogistic.contactNumber);   // Write new logistic data to file
    fclose(file);   // Close file
    printf("Logistic data added successfully.\n");
}

void readDataLogistic(const char *filename, const char *attribute, const char *deliveryID){ // Function to read data from logistic file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if logistic is found
    int attributeFound = 1; // Flag to check if attribute is found
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n", logistics[i].deliveryID, logistics[i].name, logistics[i].email, logistics[i].contactNumber);   // Split record's content and read logistic data from file
        if (logistics[i].deliveryID[0] == '\0') { // If deliveryID is empty, break the loop
            break;
        }
        if (strcmp(logistics[i].deliveryID, deliveryID) == 0) {    // Compare deliveryID with value
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                printf("Name: %s\n", logistics[i].name); // Print name
            }
            else if (strcmp(attribute, "email") == 0) { // Compare attribute with email
                printf("Email: %s\n", logistics[i].email); // Print email
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                printf("Contact Number: %s\n", logistics[i].contactNumber);    // Print contact number
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If deliveryID is found and attribute is found
        printf("Logistic data read successfully.\n");
    } 
    else {
        printf("Logistic with deliveryID %s not found.\n", deliveryID);
    }
}

void updateDataLogistic(const char *filename, const char *attribute, const char *deliveryID, const char *newValue){   // Function to update data in logistic file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if logistic is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n", logistics[i].deliveryID, logistics[i].name, logistics[i].email, logistics[i].contactNumber);   // Split record's content and read logistic data from file
        if (logistics[i].deliveryID[0] == '\0') { // If deliveryID is empty, break the loop
            break;
        }
        if (strcmp(logistics[i].deliveryID, deliveryID) == 0) {  // Compare deliveryID with deliveryID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                strncpy(logistics[i].name, newValue, MAX_NAME_LENGTH - 2); // Copy new value to name
                logistics[i].name[MAX_NAME_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "email") == 0) {  // Compare attribute with email
                strncpy(logistics[i].email, newValue, MAX_EMAIL_LENGTH - 2); // Copy new value to email
                logistics[i].email[MAX_EMAIL_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                strncpy(logistics[i].contactNumber, newValue, MAX_CONTACT_LENGTH - 2); // Copy new value to contactNumber
                logistics[i].contactNumber[MAX_CONTACT_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%s\n", logistics[i].deliveryID, logistics[i].name, logistics[i].email, logistics[i].contactNumber);  // Write updated logistic data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If logistic is found and attribute is found
        printf("Logistic data updated successfully.\n");
    }
    else {
        printf("Logistic with deliveryID %s not found.\n", deliveryID);
    }
}

void deleteDataLogistic(const char *filename, const char *deliveryID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if logistic is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n", logistics[i].deliveryID, logistics[i].name, logistics[i].email, logistics[i].contactNumber);   // Split record's content and read logistic data from file
        if (logistics[i].deliveryID[0] == '\0') { // If deliveryID is empty, break the loop
            break;
        }
        if (strcmp(logistics[i].deliveryID, deliveryID) == 0) {  // Compare deliveryID with deliveryID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s,%s\n", logistics[i].deliveryID, logistics[i].name, logistics[i].email, logistics[i].contactNumber);  // Write logistic data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If logistic is found
        printf("Logistic data deleted successfully.\n");
    } 
    else {
        printf("Logistic with deliveryID %s not found.\n", deliveryID);
    }
}

// Category functions

void addDataCategory(const char *filename, const char *categoryID, const char *name, const char *description){  // Function to add data to Category file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Category newCategory; // Create new Category structure
    strncpy(newCategory.categoryID, categoryID, MAX_ID_LENGTH - 2);  // Copy value to categoryID
    newCategory.categoryID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newCategory.name, name, MAX_NAME_LENGTH - 2);  // Copy value to name
    newCategory.name[MAX_NAME_LENGTH - 1] = '\0';  // Null terminate the string
    strncpy(newCategory.description, description, MAX_DESCRIPTION_LENGTH - 2);   // Copy value to description
    newCategory.description[MAX_DESCRIPTION_LENGTH - 1] = '\0';    // Null terminate the string
    fprintf(file, "%s,%s,%s\n", newCategory.categoryID, newCategory.name, newCategory.description);   // Write new category data to file
    fclose(file);   // Close file
    printf("Category data added successfully.\n");
}

void readDataCategory(const char *filename, const char *attribute, const char *categoryID){ // Function to read data from category file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if category is found
    int attributeFound = 1; // Flag to check if attribute is found
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^\n]\n", categories[i].categoryID, categories[i].name, categories[i].description);   // Split record's content and read category data from file
        if (categories[i].categoryID[0] == '\0') { // If categoryID is empty, break the loop
            break;
        }
        if (strcmp(categories[i].categoryID, categoryID) == 0) {    // Compare categoryID with value
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                printf("Name: %s\n", categories[i].name); // Print name
            }
            else if (strcmp(attribute, "description") == 0) {   // Compare attribute with description
                printf("Description: %s\n", categories[i].description); // Print description
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If category is found and attribute is found
        printf("Category data read successfully.\n");
    } 
    else {
        printf("Category with categoryID %s not found.\n", categoryID);
    }
}

void updateDataCategory(const char *filename, const char *attribute, const char *categoryID, const char *newValue){   // Function to update data in category file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if customer is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^\n]\n", categories[i].categoryID, categories[i].name, categories[i].description);   // Split record's content and read category data from file
        if (categories[i].categoryID[0] == '\0') { // If categoryID is empty, break the loop
            break;
        }
        if (strcmp(categories[i].categoryID, categoryID) == 0) {  // Compare categoryID with categoryID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                strncpy(categories[i].name, newValue, MAX_NAME_LENGTH - 2); // Copy new value to name
                categories[i].name[MAX_NAME_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "description") == 0) {   // Compare attribute with description
                strncpy(categories[i].description, newValue, MAX_DESCRIPTION_LENGTH - 2); // Copy new value to description
                categories[i].description[MAX_DESCRIPTION_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s\n", categories[i].categoryID, categories[i].name, categories[i].description);  // Write updated category data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If category is found and attribute is found
        printf("Category data updated successfully.\n");
    }
    else {
        printf("Category with categoryID %s not found.\n", categoryID);
    }
}

void deleteDataCategory(const char *filename, const char *categoryID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if category is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^\n]\n", categories[i].categoryID, categories[i].name, categories[i].description);   // Split record's content and read category data from file
        if (categories[i].categoryID[0] == '\0') { // If categoryID is empty, break the loop
            break;
        }
        if (strcmp(categories[i].categoryID, categoryID) == 0) {  // Compare categoryID with categoryID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s\n", categories[i].categoryID, categories[i].name, categories[i].description);  // Write category data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If category is found
        printf("Category data deleted successfully.\n");
    } 
    else {
        printf("Category with categoryID %s not found.\n", categoryID);
    }
}

// Inventory functions

void addDataInventory(const char *filename, const char *productID, const char *categoryID, const char *name, float price, int quantity, const char *description){  // Function to add data to inventory file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Inventory newInventory; // Create new inventory item structure
    strncpy(newInventory.productID, productID, MAX_ID_LENGTH - 2);  // Copy value to productID
    newInventory.productID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newInventory.categoryID, categoryID, MAX_ID_LENGTH - 2);  // Copy value to categoryID
    newInventory.categoryID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newInventory.name, name, MAX_NAME_LENGTH - 2);  // Copy value to name
    newInventory.name[MAX_NAME_LENGTH - 1] = '\0';  // Null terminate the string
    newInventory.price = price;   // Copy value to price
    newInventory.quantity = quantity;   // Copy value to quantity
    strncpy(newInventory.description, description, MAX_DESCRIPTION_LENGTH - 2);   // Copy value to description
    newInventory.description[MAX_DESCRIPTION_LENGTH - 1] = '\0';    // Null terminate the string
    fprintf(file, "%s,%s,%s,%.2f,%d,%s\n", newInventory.productID, newInventory.categoryID, newInventory.name, newInventory.price, newInventory.quantity, newInventory.description);   // Write new inventory data to file
    fclose(file);   // Close file
    printf("Inventory data added successfully.\n");
}

void readDataInventory(const char *filename, const char *attribute, const char *productID){ // Function to read data from inventory file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if product is found
    char tempPrice[20];
    char tempQuantity[20];
    int attributeFound = 1; // Flag to check if attribute is found
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", inventories[i].productID, inventories[i].categoryID, inventories[i].name, tempPrice, tempQuantity, inventories[i].description);   // Split record's content and read product data from file
        if (inventories[i].productID[0] == '\0') { // If productID is empty, break the loop
            break;
        }
        inventories[i].price = atof(tempPrice); // Convert price string to float
        inventories[i].quantity = atoi(tempQuantity); // Convert quantity string to int
        if (strcmp(inventories[i].productID, productID) == 0) {    // Compare productID with value
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "categoryID") == 0) {   // Compare attribute with categoryID
                printf("Category ID: %s\n", inventories[i].categoryID); // Print category ID
            }
            else if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                printf("Name: %s\n", inventories[i].name); // Print name
            }
            else if (strcmp(attribute, "price") == 0) { // Compare attribute with price
                printf("Price: %.2f\n", inventories[i].price); // Print price
            }
            else if (strcmp(attribute, "quantity") == 0) { // Compare attribute with quantity
                printf("Quantity: %d\n", inventories[i].quantity);    // Print quantity
            }
            else if (strcmp(attribute, "description") == 0) { // Compare attribute with description
                printf("Description: %s\n", inventories[i].description);    // Print description
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If product is found and attribute is found
        printf("Product data read successfully.\n");
    } 
    else {
        printf("Product with productID %s not found.\n", productID);
    }
}

void updateDataInventory(const char *filename, const char *attribute, const char *productID, const char *newValue){   // Function to update data in product file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if product is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Inventory currentInventory; // Create new inventory item structure
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        int parsed = fscanf(file, "%[^,],%[^,],%[^,],%f,%d,%[^\n]\n", currentInventory.productID, currentInventory.categoryID, currentInventory.name, &currentInventory.price, &currentInventory.quantity, currentInventory.description);   // Split record's content and read product data from file
        if (parsed != 6) { // If not all fields are read successfully
            break;
        }
        if (strcmp(currentInventory.productID, productID) == 0) {  // Compare productID with productID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "categoryID") == 0) {   // Compare attribute with categoryID
                strncpy(currentInventory.categoryID, newValue, MAX_ID_LENGTH - 2); // Copy new value to categoryID
                currentInventory.categoryID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                strncpy(currentInventory.name, newValue, MAX_NAME_LENGTH - 2); // Copy new value to name
                currentInventory.name[MAX_NAME_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "price") == 0) {  // Compare attribute with price
                currentInventory.price = atof(newValue); // Copy new value to price
            }
            else if (strcmp(attribute, "quantity") == 0) { // Compare attribute with quantity
                currentInventory.quantity = atoi(newValue); // Copy new value to quantity
            }
            else if (strcmp(attribute, "description") == 0) { // Compare attribute with description
                strncpy(currentInventory.description, newValue, MAX_DESCRIPTION_LENGTH - 2); // Copy new value to description
                currentInventory.description[MAX_DESCRIPTION_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%.2f,%d,%s\n", currentInventory.productID, currentInventory.categoryID, currentInventory.name, currentInventory.price, currentInventory.quantity, currentInventory.description);  // Write updated product data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If product is found and attribute is found
        printf("Product data updated successfully.\n");
    }
    else {
        printf("Product with productID %s not found.\n", productID);
    }
}

void deleteDataInventory(const char *filename, const char *productID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if product is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    Inventory currentInventory; // Create new inventory item structure
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        int parsed = fscanf(file, "%[^,],%[^,],%[^,],%f,%d,%[^\n]\n", currentInventory.productID, currentInventory.categoryID, currentInventory.name, &currentInventory.price, &currentInventory.quantity, currentInventory.description);   // Split record's content and read product data from file
        if (parsed != 6) { // If not all fields are read successfully
            break;
        }
        if (strcmp(currentInventory.productID, productID) == 0) {  // Compare productID with productID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s,%.2f,%d,%s\n", currentInventory.productID, currentInventory.categoryID, currentInventory.name, currentInventory.price, currentInventory.quantity, currentInventory.description);  // Write product data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If product is found
        printf("Product data deleted successfully.\n");
    } 
    else {
        printf("Product with productID %s not found.\n", productID);
    }
}

// Supplier functions

void addDataSupplier(const char *filename, const char *supplierID, const char *name, const char *address, const char *email, const char *contactNumber){  // Function to add data to supplier file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Supplier newSupplier; // Create new supplier structure
    strncpy(newSupplier.supplierID, supplierID, MAX_ID_LENGTH - 2);  // Copy value to supplierID
    newSupplier.supplierID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newSupplier.name, name, MAX_NAME_LENGTH - 2);  // Copy value to name
    newSupplier.name[MAX_NAME_LENGTH - 1] = '\0';  // Null terminate the string
    strncpy(newSupplier.address, address, MAX_ADDRESS_LENGTH - 2);  // Copy value to address
    newSupplier.address[MAX_ADDRESS_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newSupplier.email, email, MAX_EMAIL_LENGTH - 2);  // Copy value to email
    newSupplier.email[MAX_EMAIL_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newSupplier.contactNumber, contactNumber, MAX_CONTACT_LENGTH - 2);  // Copy value to contactNumber
    newSupplier.contactNumber[MAX_CONTACT_LENGTH - 1] = '\0'; // Null terminate the string
    fprintf(file, "%s,%s,%s,%s,%s\n", newSupplier.supplierID, newSupplier.name, newSupplier.address, newSupplier.email, newSupplier.contactNumber);   // Write new supplier data to file
    fclose(file);   // Close file
    printf("Supplier data added successfully.\n");
}

void readDataSupplier(const char *filename, const char *attribute, const char *supplierID){ // Function to read data from supplier file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if supplier is found
    int attributeFound = 1; // Flag to check if attribute is found
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", suppliers[i].supplierID, suppliers[i].name, suppliers[i].address, suppliers[i].email, suppliers[i].contactNumber);   // Split record's content and read supplier data from file
        if (suppliers[i].supplierID[0] == '\0') { // If supplierID is empty, break the loop
            break;
        }
        if (strcmp(suppliers[i].supplierID, supplierID) == 0) {    // Compare supplierID with value
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "supplierID") == 0) {   // Compare attribute with supplierID
                printf("Supplier ID: %s\n", suppliers[i].supplierID); // Print supplier ID
            }
            else if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                printf("Name: %s\n", suppliers[i].name); // Print name
            }
            else if (strcmp(attribute, "address") == 0) { // Compare attribute with address
                printf("Address: %s\n", suppliers[i].address); // Print address
            }
            else if (strcmp(attribute, "email") == 0) { // Compare attribute with email
                printf("Email: %s\n", suppliers[i].email); // Print email
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                printf("Contact Number: %s\n", suppliers[i].contactNumber);    // Print contact number
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If supplier is found and attribute is found
        printf("Supplier data read successfully.\n");
    } 
    else {
        printf("Supplier with supplierID %s not found.\n", supplierID);
    }
}

void updateDataSupplier(const char *filename, const char *attribute, const char *supplierID, const char *newValue){   // Function to update data in supplier file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if supplier is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", suppliers[i].supplierID, suppliers[i].name, suppliers[i].address, suppliers[i].email, suppliers[i].contactNumber);   // Split record's content and read supplier data from file
        if (suppliers[i].supplierID[0] == '\0') { // If supplierID is empty, break the loop
            break;
        }
        if (strcmp(suppliers[i].supplierID, supplierID) == 0) {  // Compare supplierID with supplierID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "name") == 0) {   // Compare attribute with name
                strncpy(suppliers[i].name, newValue, MAX_NAME_LENGTH - 2); // Copy new value to name
                suppliers[i].name[MAX_NAME_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "address") == 0) {   // Compare attribute with address
                strncpy(suppliers[i].address, newValue, MAX_ADDRESS_LENGTH - 2); // Copy new value to address
                suppliers[i].address[MAX_ADDRESS_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "email") == 0) {  // Compare attribute with email
                strncpy(suppliers[i].email, newValue, MAX_EMAIL_LENGTH - 2); // Copy new value to email
                suppliers[i].email[MAX_EMAIL_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "contactNumber") == 0) { // Compare attribute with contactNumber
                strncpy(suppliers[i].contactNumber, newValue, MAX_CONTACT_LENGTH - 2); // Copy new value to contactNumber
                suppliers[i].contactNumber[MAX_CONTACT_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s\n", suppliers[i].supplierID, suppliers[i].name, suppliers[i].address, suppliers[i].email, suppliers[i].contactNumber);  // Write updated supplier data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If supplier is found and attribute is found
        printf("Supplier data updated successfully.\n");
    }
    else {
        printf("Supplier with supplierID %s not found.\n", supplierID);
    }
}

void deleteDataSupplier(const char *filename, const char *supplierID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if supplier is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", suppliers[i].supplierID, suppliers[i].name, suppliers[i].address, suppliers[i].email, suppliers[i].contactNumber);   // Split record's content and read supplier data from file
        if (suppliers[i].supplierID[0] == '\0') { // If supplierID is empty, break the loop
            break;
        }
        if (strcmp(suppliers[i].supplierID, supplierID) == 0) {  // Compare supplierID with supplierID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s\n", suppliers[i].supplierID, suppliers[i].name, suppliers[i].address, suppliers[i].email, suppliers[i].contactNumber);  // Write supplier data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If supplier is found
        printf("Supplier data deleted successfully.\n");
    } 
    else {
        printf("Supplier with supplierID %s not found.\n", supplierID);
    }
}

// Transaction functions

void addDataTransaction(const char *filename, const char *transactionID, const char *adminID, const char *productID, const char *supplierID, const char *date, int quantity, float totalAmount){  // Function to add data to transaction file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Transaction newTransaction; // Create new transaction structure
    strncpy(newTransaction.transactionID, transactionID, MAX_ID_LENGTH - 2);  // Copy value to transactionID
    newTransaction.transactionID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newTransaction.adminID, adminID, MAX_ID_LENGTH - 2);  // Copy value to adminID
    newTransaction.adminID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newTransaction.productID, productID, MAX_ID_LENGTH - 2);  // Copy value to productID
    newTransaction.productID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newTransaction.supplierID, supplierID, MAX_ID_LENGTH - 2);  // Copy value to supplierID
    newTransaction.supplierID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newTransaction.date, date, MAX_DATE_LENGTH - 2);  // Copy value to date
    newTransaction.date[MAX_DATE_LENGTH - 1] = '\0'; // Null terminate the string
    newTransaction.quantity = quantity;   // Copy value to quantity
    newTransaction.totalAmount = totalAmount;   // Copy value to totalAmount
    fprintf(file, "%s,%s,%s,%s,%s,%d,%.2f\n", newTransaction.transactionID, newTransaction.adminID, newTransaction.productID, newTransaction.supplierID, newTransaction.date, newTransaction.quantity, newTransaction.totalAmount);   // Write new transaction data to file
    fclose(file);   // Close file
    printf("Transaction data added successfully.\n");
}

void readDataTransaction(const char *filename, const char *attribute, const char *transactionID){ // Function to read data from transaction file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if transaction is found
    int attributeFound = 1; // Flag to check if attribute is found
    char tempTotalAmount[20];
    char tempQuantity[20];
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", transactions[i].transactionID, transactions[i].adminID, transactions[i].productID, transactions[i].supplierID, transactions[i].date, tempQuantity, tempTotalAmount);   // Split record's content and read transaction data from file
        if (transactions[i].transactionID[0] == '\0') { // If transactionID is empty, break the loop
            break;
        }
        if (strcmp(transactions[i].transactionID, transactionID) == 0) {    // Compare transactionID with value
            found = 1;  // Set found flag to 1
            transactions[i].quantity = atoi(tempQuantity); // Convert quantity string to int
            transactions[i].totalAmount = atof(tempTotalAmount); // Convert totalAmount string to float
            if (strcmp(attribute, "transactionID") == 0) {   // Compare attribute with transactionID
                printf("Transaction ID: %s\n", transactions[i].transactionID); // Print transaction ID
            }
            else if (strcmp(attribute, "adminID") == 0) {   // Compare attribute with adminID
                printf("Admin ID: %s\n", transactions[i].adminID); // Print admin ID
            }
            else if (strcmp(attribute, "productID") == 0) { // Compare attribute with productID
                printf("Product ID: %s\n", transactions[i].productID); // Print product ID
            }
            else if (strcmp(attribute, "supplierID") == 0) { // Compare attribute with supplierID
                printf("Supplier ID: %s\n", transactions[i].supplierID); // Print supplier ID
            }
            else if (strcmp(attribute, "date") == 0) { // Compare attribute with date
                printf("Date: %s\n", transactions[i].date); // Print date
            }
            else if (strcmp(attribute, "quantity") == 0) { // Compare attribute with quantity
                printf("Quantity: %d\n", transactions[i].quantity);    // Print quantity
            }
            else if (strcmp(attribute, "totalAmount") == 0) { // Compare attribute with totalAmount
                printf("Total Amount: %.2f\n", transactions[i].totalAmount); // Print total amount
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If transaction is found and attribute is found
        printf("Transaction data read successfully.\n");
    } 
    else {
        printf("Transaction with TransactionID %s not found.\n", transactionID);
    }
}

void updateDataTransaction(const char *filename, const char *attribute, const char *transactionID, const char *newValue){   // Function to update data in transaction file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if product is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Transaction currentTransaction; // Create new transaction structure
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        int parsed = fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n", currentTransaction.transactionID, currentTransaction.adminID, currentTransaction.productID, currentTransaction.supplierID, currentTransaction.date, &currentTransaction.quantity, &currentTransaction.totalAmount);   // Split record's content and read transaction data from file
        if (currentTransaction.transactionID[0] == '\0') { // If transactionID is empty, break the loop
            break;
        }
        if (parsed != 7) { // If parsing fails
            if (feof(file)) break; // Break if end of file is reached
            printf("Error reading transaction data.\n");
            return;
        }
        if (strcmp(currentTransaction.transactionID, transactionID) == 0) {  // Compare transactionID with transactionID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "adminID") == 0) {   // Compare attribute with adminID
                strncpy(currentTransaction.adminID, newValue, MAX_ID_LENGTH - 2); // Copy new value to adminID
                currentTransaction.adminID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "productID") == 0) {   // Compare attribute with productID
                strncpy(currentTransaction.productID, newValue, MAX_ID_LENGTH - 2); // Copy new value to productID
                currentTransaction.productID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "supplierID") == 0) {   // Compare attribute with supplierID
                strncpy(currentTransaction.supplierID, newValue, MAX_ID_LENGTH - 2); // Copy new value to supplierID
                currentTransaction.supplierID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "date") == 0) {   // Compare attribute with date
                strncpy(currentTransaction.date, newValue, MAX_DATE_LENGTH - 2); // Copy new value to date
                currentTransaction.date[MAX_DATE_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "quantity") == 0) { // Compare attribute with quantity
                currentTransaction.quantity = atoi(newValue); // Copy new value to quantity
            }
            else if (strcmp(attribute, "totalAmount") == 0) {  // Compare attribute with totalAmount
                currentTransaction.totalAmount = atof(newValue); // Copy new value to totalAmount
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s,%d,%.2f\n", currentTransaction.transactionID, currentTransaction.adminID, currentTransaction.productID, currentTransaction.supplierID, currentTransaction.date, currentTransaction.quantity, currentTransaction.totalAmount);  // Write updated product data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If transaction is found and attribute is found
        printf("Transaction data updated successfully.\n");
    }
    else {
        printf("Transaction with transactionID %s not found.\n", transactionID);
    }
}

void deleteDataTransaction(const char *filename, const char *transactionID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if transaction is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    Transaction currentTransaction; // Create new transaction structure
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        int parsed = fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n", currentTransaction.transactionID, currentTransaction.adminID, currentTransaction.productID, currentTransaction.supplierID, currentTransaction.date, &currentTransaction.quantity, &currentTransaction.totalAmount);   // Split record's content and read transaction data from file
        if (parsed != 7) { // If not all fields are read successfully
            break;
        }
        if (strcmp(currentTransaction.transactionID, transactionID) == 0) {  // Compare transactionID with transactionID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s,%d,%.2f\n", currentTransaction.transactionID, currentTransaction.adminID, currentTransaction.productID, currentTransaction.supplierID, currentTransaction.date, currentTransaction.quantity, currentTransaction.totalAmount);  // Write transaction data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If Transaction is found
        printf("Transaction data deleted successfully.\n");
    } 
    else {
        printf("Transaction with TransactionID %s not found.\n", transactionID);
    }
}

// Order functions

void addDataOrder(const char *filename, const char *orderID, const char *adminID, const char *customerID, const char *productID, const char *deliveryID, const char *date, int quantity, float totalAmount){  // Function to add data to order file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "a");  // Open file with filename in append mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Order newOrder; // Create new order structure
    strncpy(newOrder.orderID, orderID, MAX_ID_LENGTH - 2);  // Copy value to orderID
    newOrder.orderID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newOrder.adminID, adminID, MAX_ID_LENGTH - 2);  // Copy value to adminID
    newOrder.adminID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newOrder.customerID, customerID, MAX_ID_LENGTH - 2);  // Copy value to customerID
    newOrder.customerID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newOrder.productID, productID, MAX_ID_LENGTH - 2);  // Copy value to productID
    newOrder.productID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newOrder.deliveryID, deliveryID, MAX_ID_LENGTH - 2);  // Copy value to deliveryID
    newOrder.deliveryID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
    strncpy(newOrder.date, date, MAX_DATE_LENGTH - 2);  // Copy value to date
    newOrder.date[MAX_DATE_LENGTH - 1] = '\0'; // Null terminate the string
    newOrder.quantity = quantity;   // Copy value to quantity
    newOrder.totalAmount = totalAmount;   // Copy value to totalAmount
    fprintf(file, "%s,%s,%s,%s,%s,%s,%d,%.2f\n", newOrder.orderID, newOrder.adminID, newOrder.customerID, newOrder.productID, newOrder.deliveryID, newOrder.date, newOrder.quantity, newOrder.totalAmount);   // Write new order data to file
    fclose(file);   // Close file
    printf("Order data added successfully.\n");
}

void readDataOrder(const char *filename, const char *attribute, const char *orderID){ // Function to read data from order file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    if (file == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    int found = 0;  // Flag to check if order is found
    int attributeFound = 1; // Flag to check if attribute is found
    char tempQuantity[20];
    char tempTotalAmount[20];
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", orders[i].orderID, orders[i].adminID, orders[i].customerID, orders[i].productID, orders[i].deliveryID, orders[i].date, tempQuantity, tempTotalAmount);   // Split record's content and read order data from file
        if (orders[i].orderID[0] == '\0') { // If orderID is empty, break the loop
            break;
        }
        if (strcmp(orders[i].orderID, orderID) == 0) {    // Compare orderID with value
            found = 1;  // Set found flag to 1
            orders[i].quantity = atoi(tempQuantity); // Convert quantity string to int
            orders[i].totalAmount = atof(tempTotalAmount); // Convert totalAmount string to float
            if (strcmp(attribute, "orderID") == 0) {   // Compare attribute with orderID
                printf("Order ID: %s\n", orders[i].orderID); // Print order ID
            }
            else if (strcmp(attribute, "adminID") == 0) {   // Compare attribute with adminID
                printf("Admin ID: %s\n", orders[i].adminID); // Print admin ID
            }
            else if (strcmp(attribute, "customerID") == 0) { // Compare attribute with customerID
                printf("Customer ID: %s\n", orders[i].customerID); // Print customer ID
            }
            else if (strcmp(attribute, "productID") == 0) { // Compare attribute with productID
                printf("Product ID: %s\n", orders[i].productID); // Print product ID
            }
            else if (strcmp(attribute, "deliveryID") == 0) { // Compare attribute with deliveryID
                printf("Delivery ID: %s\n", orders[i].deliveryID); // Print delivery ID
            }
            else if (strcmp(attribute, "date") == 0) { // Compare attribute with date
                printf("Date: %s\n", orders[i].date); // Print date
            }
            else if (strcmp(attribute, "quantity") == 0) { // Compare attribute with quantity
                printf("Quantity: %d\n", orders[i].quantity);    // Print quantity
            }
            else if (strcmp(attribute, "totalAmount") == 0) { // Compare attribute with totalAmount
                printf("Total Amount: %.2f\n", orders[i].totalAmount); // Print total amount
            }
            else {  // If attribute does not match any of the above
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
    }
    fclose(file);   // Close file
    if (found && attributeFound) {    // If order is found and attribute is found
        printf("Order data read successfully.\n");
    } 
    else {
        printf("Order with OrderID %s not found.\n", orderID);
    }
}

void updateDataOrder(const char *filename, const char *attribute, const char *orderID, const char *newValue){   // Function to update data in order file
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if order is found
    int attributeFound = 1; // Flag to check if attribute is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");    // Print error message
        return;
    }
    Order currentOrder; // Create new order structure
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        int parsed = fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n", currentOrder.orderID, currentOrder.adminID, currentOrder.customerID, currentOrder.productID, currentOrder.deliveryID, currentOrder.date, &currentOrder.quantity, &currentOrder.totalAmount);   // Split record's content and read order data from file
        if (currentOrder.orderID[0] == '\0') { // If orderID is empty, break the loop
            break;
        }
        if (parsed != 8) {
            if (feof(file)) break; // End of file
            printf("Error reading record %d\n", i+1);
            continue;
        }
        if (strcmp(currentOrder.orderID, orderID) == 0) {  // Compare orderID with orderID
            found = 1;  // Set found flag to 1
            if (strcmp(attribute, "adminID") == 0) {   // Compare attribute with adminID
                strncpy(currentOrder.adminID, newValue, MAX_ID_LENGTH - 2); // Copy new value to adminID
                currentOrder.adminID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "customerID") == 0) {   // Compare attribute with customerID
                strncpy(currentOrder.customerID, newValue, MAX_ID_LENGTH - 2); // Copy new value to customerID
                currentOrder.customerID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "productID") == 0) {   // Compare attribute with productID
                strncpy(currentOrder.productID, newValue, MAX_ID_LENGTH - 2); // Copy new value to productID
                currentOrder.productID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "deliveryID") == 0) {  // Compare attribute with deliveryID
                strncpy(currentOrder.deliveryID, newValue, MAX_ID_LENGTH - 2); // Copy new value to deliveryID
                currentOrder.deliveryID[MAX_ID_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "date") == 0) { // Compare attribute with date
                strncpy(currentOrder.date, newValue, MAX_DATE_LENGTH - 2); // Copy new value to date
                currentOrder.date[MAX_DATE_LENGTH - 1] = '\0'; // Null terminate the string
            }
            else if (strcmp(attribute, "quantity") == 0) { // Compare attribute with quantity
                currentOrder.quantity = atoi(newValue); // Copy new value to quantity
            } 
            else if (strcmp(attribute, "totalAmount") == 0) { // Compare attribute with totalAmount
                currentOrder.totalAmount = atof(newValue); // Copy new value to totalAmount
            }
            else {
                printf("Invalid attribute\n");  // Print error message
                fclose(file);   // Close file
                fclose(tempFile);   // Close temp file
                attributeFound = 0; // Set attributeFound flag to 0
                return;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s,%s,%d,%.2f\n", currentOrder.orderID, currentOrder.adminID, currentOrder.customerID, currentOrder.productID, currentOrder.deliveryID, currentOrder.date, currentOrder.quantity, currentOrder.totalAmount);  // Write updated order data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found && attributeFound) { // If order is found and attribute is found
        printf("Order data updated successfully.\n");
    }
    else {
        printf("Order with OrderID %s not found.\n", orderID);
    }
}

void deleteDataOrder(const char *filename, const char *orderID){
    checkFileExist(filename);   // Check if file exists
    FILE *file = fopen(filename, "r");  // Open file with filename in read mode
    FILE *tempFile = fopen("temp.txt", "w");    // Open temp.txt as tempFile in write mode
    int found = 0;  // Flag to check if order is found
    if (file == NULL || tempFile == NULL) { // If file cannot be opened
        printf("Error opening file.\n");
        return;
    }
    Order currentOrder; // Create new order structure
    for (int i = 0; i < MAX_RECORDS; i++) { // Loop through each record
        int parsed = fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n", currentOrder.orderID, currentOrder.adminID, currentOrder.customerID, currentOrder.productID, currentOrder.deliveryID, currentOrder.date, &currentOrder.quantity, &currentOrder.totalAmount);   // Split record's content and read order data from file
        if (parsed != 8) { // If not all fields are read successfully
            break;
        }
        if (strcmp(currentOrder.orderID, orderID) == 0) {  // Compare orderID with orderID
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s,%s,%d,%.2f\n", currentOrder.orderID, currentOrder.adminID, currentOrder.customerID, currentOrder.productID, currentOrder.deliveryID, currentOrder.date, currentOrder.quantity, currentOrder.totalAmount);  // Write order data to tempFile
    }
    fclose(file);   // Close file
    fclose(tempFile);   // Close temp file
    remove(filename);   // Remove original file
    rename("temp.txt", filename);   // Rename temp file to original file name
    if (found) {    // If Order is found
        printf("Order data deleted successfully.\n");
    } 
    else {
        printf("Order with OrderID %s not found.\n", orderID);
    }
}

#endif