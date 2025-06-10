#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FileHandling.h"
#include "FormatHandling.h"

void addCategory() {
    char filename[] = "categories.txt"; // File to store categories
    char categoryID[MAX_ID_LENGTH]; // Buffer for category ID
    char name[MAX_NAME_LENGTH]; // Buffer for category name
    char description[MAX_DESCRIPTION_LENGTH];   // Buffer for category description
    char continueChoice;    // Variable to check if user wants to continue adding categories

    do {
        printf("\n--- Add Category ---\n"); // Prompt user to add a category
        printf("Enter Category ID: ");  // Prompt for category ID
        scanf("%s", categoryID);
        while (getchar() != '\n');

        printf("Enter Category Name: ");    // Prompt for category name
        fgets(name, MAX_NAME_LENGTH, stdin);
        name[strcspn(name, "\n")] = '\0';

        printf("Enter Category Description: "); // Prompt for category description
        fgets(description, MAX_DESCRIPTION_LENGTH, stdin);
        description[strcspn(description, "\n")] = '\0';

        addDataCategory2(filename, categoryID, name, description); // Call function to add category data to file

        printf("Do you want to add another category? (y/n): "); // Ask user if they want to add another category
        scanf(" %c", &continueChoice);  // Read user's choice
    } while (continueChoice == 'y' || continueChoice == 'Y');   // Continue loop if user chooses 'y' or 'Y'
}

void addSupplier() {
    char filename[] = "suppliers.txt";  // File to store suppliers
    char supplierID[MAX_ID_LENGTH]; // Buffer for supplier ID
    char name[MAX_NAME_LENGTH]; // Buffer for supplier name
    char address[MAX_ADDRESS_LENGTH];   // Buffer for supplier address
    char email[MAX_EMAIL_LENGTH];   // Buffer for supplier email
    char contactNumber[MAX_CONTACT_LENGTH]; // Buffer for supplier contact number
    char continueChoice;    // Variable to check if user wants to continue adding suppliers

    do {
        while (getchar() != '\n');
        printf("\n--- Add Supplier ---\n");     
        printf("Enter Supplier ID: ");          
        fgets(supplierID, MAX_ID_LENGTH, stdin); // Prompt for supplier ID
        supplierID[strcspn(supplierID, "\n")] = '\0'; // Remove newline character from input

        printf("Enter Supplier Name: ");
        fgets(name, MAX_NAME_LENGTH, stdin); // Prompt for supplier name
        name[strcspn(name, "\n")] = '\0'; // Remove newline character from input

        printf("Enter Supplier Address: ");
        fgets(address, MAX_ADDRESS_LENGTH, stdin); // Prompt for supplier address
        address[strcspn(address, "\n")] = '\0'; // Remove newline character from input

        printf("Enter Supplier Email: ");
        scanf("%s", email);

        printf("Enter Supplier Contact Number: ");
        scanf("%s", contactNumber);

        addDataSupplier2(filename, supplierID, name, address, email, contactNumber); // Call function to add supplier data to file

        printf("Do you want to add another supplier? (y/n): ");
        scanf(" %c", &continueChoice);
    } while (continueChoice == 'y' || continueChoice == 'Y');
}

void updateCategory() {
    char filename[] = "categories.txt"; // File to store categories
    char categoryID[MAX_ID_LENGTH]; // Buffer for category ID
    char attribute[20]; // Buffer for attribute to update
    char newValue[MAX_DESCRIPTION_LENGTH];  // Buffer for new value
    char continueChoice;    // Variable to check if user wants to continue updating categories

    do {    // Prompt user to update a category and loop until they choose to stop
        while (getchar() != '\n');
        printf("\n--- Update Category ---\n");
        printf("Enter Category ID to update: ");
        scanf("%s", categoryID);
        while (getchar() != '\n'); // Clear input buffer

        printf("Enter attribute to update (name/description): ");
        scanf("%s", attribute);
        while (getchar() != '\n');

        printf("Enter new data: ");
        fgets(newValue, MAX_DESCRIPTION_LENGTH, stdin); // Prompt for new value
        newValue[strcspn(newValue, "\n")] = '\0'; // Remove newline character from input

        updateDataCategory(filename, attribute, categoryID, newValue); // Call function to update category data in file

        printf("Do you want to update another category? (y/n): ");
        scanf(" %c", &continueChoice);
    } while (continueChoice == 'y' || continueChoice == 'Y');
}

void updateSupplier() {    // Function to update supplier data
    char filename[] = "suppliers.txt";
    char supplierID[MAX_ID_LENGTH];
    char attribute[20];
    char newValue[MAX_ADDRESS_LENGTH];
    char continueChoice;

    do {    // Prompt user to update a supplier and loop until they choose to stop
        while (getchar() != '\n');
        printf("\n--- Update Supplier ---\n");
        printf("Enter Supplier ID to update: ");
        scanf("%s", supplierID);
        while (getchar() != '\n');

        printf("Enter attribute to update (name/address/email/contactNumber): ");
        scanf("%s", attribute);
        while (getchar() != '\n');

        printf("Enter new data: ");
        fgets(newValue, MAX_DESCRIPTION_LENGTH, stdin);
        newValue[strcspn(newValue, "\n")] = '\0';

        updateDataSupplier(filename, attribute, supplierID, newValue); // Call function to update supplier data in file

        printf("Do you want to update another supplier? (y/n): ");
        scanf(" %c", &continueChoice);
    } while (continueChoice == 'y' || continueChoice == 'Y');
}

void deleteCategory() {   // Function to delete a category
    char filename[] = "categories.txt";
    char categoryID[MAX_ID_LENGTH]; // Buffer for category ID
    char continueChoice;

    do {    // Prompt user to delete a category and loop until they choose to stop
        printf("\n--- Delete Category ---\n");
        printf("Enter Category ID to delete: ");
        scanf("%s", categoryID);   

        deleteDataCategory(filename, categoryID);  // Call function to delete category data from file

        printf("Do you want to delete another category? (y/n): ");
        scanf(" %c", &continueChoice);
    } while (continueChoice == 'y' || continueChoice == 'Y');
}

void deleteSupplier() { // Function to delete a supplier
    char filename[] = "suppliers.txt";
    char supplierID[MAX_ID_LENGTH]; // Buffer for supplier ID
    char continueChoice;

    do {
        printf("\n--- Delete Supplier ---\n");
        printf("Enter Supplier ID to delete: ");
        scanf("%s", supplierID);

        deleteDataSupplier(filename, supplierID);   // Call function to delete supplier data from file

        printf("Do you want to delete another supplier? (y/n): ");
        scanf(" %c", &continueChoice);
    } while (continueChoice == 'y' || continueChoice == 'Y');
}

void filterSuppliersByName() {
    Supplier supplierList[MAX_RECORDS];
    int supplierCount = 0;

    FILE *supplierFile = fopen("suppliers.txt", "r"); // Open suppliers.txt for reading
    if (!supplierFile) {
        printf("Error opening suppliers.txt\n"); // If file cannot be opened, print error message
        return;
    }

    while (fscanf(supplierFile, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", // Read supplier data from file
                  supplierList[supplierCount].supplierID,
                  supplierList[supplierCount].name,
                  supplierList[supplierCount].address,
                  supplierList[supplierCount].email,
                  supplierList[supplierCount].contactNumber) == 5) {
        supplierCount++; // Increment supplier count for each successful read
    }
    fclose(supplierFile); // Close the file after reading

    char nameFilter[MAX_NAME_LENGTH];
    printf("\n--- Filter Suppliers by Name ---\n");
    printf("Enter supplier name to filter: ");
    scanf(" %[^\n]", nameFilter); // Read name filter from user input

    printf("\n--- Matching Suppliers ---\n");
    int found = 0; // Flag to check if any suppliers match the filter
    for (int i = 0; i < supplierCount; i++) { // Loop through the supplier list
        if (strstr(supplierList[i].name, nameFilter)) { // Check if the supplier name contains the filter string
            printf("ID: %s | Name: %s | Address: %s | Email: %s | Contact: %s\n", // Print matching supplier details
                   supplierList[i].supplierID, supplierList[i].name,
                   supplierList[i].address, supplierList[i].email,
                   supplierList[i].contactNumber);
            found = 1; 
        }
    }
    if (!found) {
        printf("No suppliers matched your filter.\n"); // If no suppliers matched the filter, print a message
    }
}

int compareSuppliersAsc(const void *a, const void *b) { // Function to compare two suppliers in ascending order based on name
    return strcmp(((Supplier *)a)->name, ((Supplier *)b)->name); // Compare names of suppliers
}

int compareSuppliersDesc(const void *a, const void *b) { // Function to compare two suppliers in descending order based on name
    return strcmp(((Supplier *)b)->name, ((Supplier *)a)->name); // Compare names of suppliers
}

void sortSuppliersByName() { // Function to sort suppliers by name
    Supplier supplierList[MAX_RECORDS]; // Array to hold supplier data
    int supplierCount = 0;

    FILE *supplierFile = fopen("suppliers.txt", "r"); // Open suppliers.txt for reading
    if (!supplierFile) { // If file cannot be opened, print error message
        printf("Error opening suppliers.txt\n");
        return;
    }

    while (fscanf(supplierFile, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", // Read supplier data from file
                  supplierList[supplierCount].supplierID,
                  supplierList[supplierCount].name,
                  supplierList[supplierCount].address,
                  supplierList[supplierCount].email,
                  supplierList[supplierCount].contactNumber) == 5) {
        supplierCount++; // Increment supplier count for each successful read
    }
    fclose(supplierFile);

    int sortOrder;
    printf("\n--- Sort Suppliers by Name ---\n");
    printf("1. Ascending\n2. Descending\nHow would you like to sort?: ");
    scanf("%d", &sortOrder);

    if (sortOrder == 1) { 
        qsort(supplierList, supplierCount, sizeof(Supplier), compareSuppliersAsc); // Sort suppliers in ascending order
    } else if (sortOrder == 2) {
        qsort(supplierList, supplierCount, sizeof(Supplier), compareSuppliersDesc); // Sort suppliers in descending order
    } else {
        printf("Invalid sort option.\n");
        return;
    }

    printf("\n--- Sorted Suppliers ---\n");
    for (int i = 0; i < supplierCount; i++) { // Loop through sorted supplier list
        printf("ID: %s | Name: %s | Address: %s | Email: %s | Contact: %s\n",
               supplierList[i].supplierID, supplierList[i].name,
               supplierList[i].address, supplierList[i].email,
               supplierList[i].contactNumber);
    }
}

void filterCategoriesByName() {
    Category categoryList[MAX_RECORDS];
    int categoryCount = 0;

    FILE *categoryFile = fopen("categories.txt", "r");
    if (!categoryFile) {
        printf("Error opening categories.txt\n");
        return;
    }

    while (fscanf(categoryFile, "%[^,],%[^,],%[^\n]\n",
                  categoryList[categoryCount].categoryID,
                  categoryList[categoryCount].name,
                  categoryList[categoryCount].description) == 3) {
        categoryCount++;
    }
    fclose(categoryFile);

    char nameFilter[MAX_NAME_LENGTH];
    printf("\n--- Filter Categories by Name ---\n");
    printf("Enter category name to filter: ");
    scanf(" %[^\n]", nameFilter);

    printf("\n--- Matching Categories ---\n");
    int found = 0;
    for (int i = 0; i < categoryCount; i++) { // Loop through the category list
        // Check if the category name contains the filter string
        if (strstr(categoryList[i].name, nameFilter)) {
            printf("ID: %s | Name: %s | Description: %s\n",
                   categoryList[i].categoryID, categoryList[i].name,
                   categoryList[i].description);
            found = 1;
        }
    }
    if (!found) {
        printf("No categories matched your filter.\n");
    }

}

int compareCategoriesAsc(const void *a, const void *b) { // Function to compare two categories in ascending order based on name
    return strcmp(((Category *)a)->name, ((Category *)b)->name);
}

int compareCategoriesDesc(const void *a, const void *b) { // Function to compare two categories in descending order based on name
    return strcmp(((Category *)b)->name, ((Category *)a)->name);
}

void sortCategoriesByName() {
    Category categoryList[MAX_RECORDS];
    int categoryCount = 0;

    FILE *categoryFile = fopen("categories.txt", "r");
    if (!categoryFile) {
        printf("Error opening categories.txt\n");
        return;
    }

    while (fscanf(categoryFile, "%[^,],%[^,],%[^\n]\n",
                  categoryList[categoryCount].categoryID,
                  categoryList[categoryCount].name,
                  categoryList[categoryCount].description) == 3) {
        categoryCount++;
    }
    fclose(categoryFile);

    int sortOrder;
    printf("\n--- Sort Categories by Name ---\n");
    printf("1. Ascending\n2. Descending\nHow would you like to sort?: ");
    scanf("%d", &sortOrder);

    if (sortOrder == 1) {
        qsort(categoryList, categoryCount, sizeof(Category), compareCategoriesAsc); // Sort categories in ascending order
    } else if (sortOrder == 2) {
        qsort(categoryList, categoryCount, sizeof(Category), compareCategoriesDesc); // Sort categories in descending order
    } else {
        printf("Invalid sort option.\n");
        return;
    }

    printf("\n--- Sorted Categories ---\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("ID: %s | Name: %s | Description: %s\n",
               categoryList[i].categoryID, categoryList[i].name,
               categoryList[i].description);
    }
}

int main() {
    int choiceMain;  // Variables to store user choices
    
    do {
        printf("\n--- Main Menu ---\n"); // Display main menu options
        printf("1. Category Functions\n");
        printf("2. Supplier Functions\n");
        printf("3. Exit\n");
        printf("What would you like to do?: ");
        scanf("%d", &choiceMain);

        if (choiceMain == 1) { // If user chooses category functions
            int categoryChoice;
            printf("\n--- Category Menu ---\n");
            printf("1. Add Category\n");
            printf("2. Update Category\n");
            printf("3. Delete Category\n");
            printf("4. Filter Categories by Name\n");
            printf("5. Sort Categories by Name\n");
            printf("6. Back to Main Menu\n");
            printf("What would you like to do?: ");
            scanf(" %d", &categoryChoice);

            switch (categoryChoice) { // Switch case to handle category menu choices
                case 1:
                    addCategory(); // Call function to add category
                    break;
                case 2:
                    updateCategory(); // Call function to update category
                    break;
                case 3:
                    deleteCategory(); // Call function to delete category
                    break;
                case 4:
                    filterCategoriesByName(); // Call function to filter categories by name
                    break;
                case 5:
                    sortCategoriesByName(); // Call function to sort categories by name
                    break;
                case 6:
                    printf("Returning to main menu...\n");
                    break;
                default:
                    printf("Invalid choice.\n");
            }
        }
        else if (choiceMain == 2) { // If user chooses supplier functions
            int supplierChoice;
            printf("\n--- Supplier Menu ---\n");
            printf("1. Add Supplier\n");
            printf("2. Update Supplier\n");
            printf("3. Delete Supplier\n");
            printf("4. Filter Suppliers by Name\n");
            printf("5. Sort Suppliers by Name\n");
            printf("6. Back to Main Menu\n");
            printf("What would you like to do?: ");
            scanf(" %d", &supplierChoice);

            switch (supplierChoice) { // Switch case to handle supplier menu choices
                case 1:
                    addSupplier(); // Call function to add supplier
                    break;
                case 2:
                    updateSupplier(); // Call function to update supplier
                    break;
                case 3:
                    deleteSupplier(); // Call function to delete supplier
                    break;
                case 4:
                    filterSuppliersByName(); // Call function to filter suppliers by name
                    break;
                case 5:
                    sortSuppliersByName(); // Call function to sort suppliers by name
                    break;
                case 6: 
                    printf("Returning to main menu...\n");
                    break;
                default:
                    printf("Invalid choice.\n");
            }
        }
        else if (choiceMain == 3) {
            printf("Exiting...");
            return 0;
        } else {
            printf("Invalid choice.\n");
        }

    } while (choiceMain != 3);

    return 0;
}