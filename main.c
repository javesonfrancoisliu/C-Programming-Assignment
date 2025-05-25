#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "FileHandling.h"

#define PRODUCT_FILE "inventory.txt"
#define ADMIN_FILE "admins.txt"

// --- Function Prototypes ---
void mainSystemMenu(int *loggedInStatus, int *keepRunningApp); // Modified
void otherManagementSystemsMenu();                             // Kept for structure, can be removed if not grouping 3,4,5 anymore
                                                               // For this request, we will list them directly in mainSystemMenu

void productManagementMenu();
void addProduct();
void updateProduct();
void deleteProduct();
void viewSpecificProductDetails();
void viewAllProducts();

void inventoryManagementMenu();
void stockManagementMenu();
void categoryManagementMenu(); // New
void supplierManagementMenu(); // New
void userTransactionManagementMenu();

int handleLogin();
int verifyAdminCredentials(const char *adminID, const char *password);

void getStringInput(const char *prompt, char *buffer, int buffer_size);
float getFloatInput(const char *prompt);
int getIntInput(const char *prompt);
int productExists(const char *productID);
void clearInputBuffer();

int main()
{
    checkFileExist(ADMIN_FILE);
    checkFileExist(PRODUCT_FILE);

    int loggedIn = 0;
    int keepRunningApp = 1; // Flag to control the main application loop

    do
    {
        if (!loggedIn)
        { // If not currently logged in (e.g., first run or after logout without exit)
            loggedIn = handleLogin();
            if (!loggedIn)
            {                       // Login failed after max attempts
                keepRunningApp = 0; // Signal to exit the application
                                    // handleLogin already prints "Maximum login attempts reached."
            }
        }

        if (loggedIn && keepRunningApp)
        {                                               // Only show main menu if logged in and app is still running
            mainSystemMenu(&loggedIn, &keepRunningApp); // Pass loggedIn to be reset on logout
                                                        // Pass keepRunningApp to be set if user wants to exit
        }
        // If user logged out from mainSystemMenu but didn't exit (keepRunningApp is 1, loggedIn is 0),
        // the loop will continue, and !loggedIn will be true, prompting login again.
        // If user chose to exit from mainSystemMenu (keepRunningApp is 0), the loop terminates.
        // If login failed definitively (keepRunningApp is 0), the loop terminates.

    } while (keepRunningApp);

    printf("\nSystem shutting down. Thank you!\n");
    return 0;
}

// --- Main System Menu Function ---
void mainSystemMenu(int *loggedInStatus, int *keepRunningApp)
{
    int choice;
    do
    {
        printf("\n--- Main System Menu ---\n");
        printf("1. Product Management\n");
        printf("2. Inventory Management\n");
        printf("3. Stock Management\n");
        printf("4. Category Management\n");             // Changed
        printf("5. Supplier Management\n");             // New
        printf("6. User and Transaction Management\n"); // Renumbered
        printf("0. Logout\n");                          // Changed
        printf("--------------------------\n");
        printf("Enter your choice: ");

        choice = getIntInput("");

        switch (choice)
        {
        case 1:
            productManagementMenu();
            break;
        case 2:
            inventoryManagementMenu();
            break;
        case 3:
            stockManagementMenu();
            break;
        case 4:
            categoryManagementMenu(); // New call
            break;
        case 5:
            supplierManagementMenu(); // New call
            break;
        case 6:
            userTransactionManagementMenu(); // Renumbered call
            break;
        case 0: // Logout
            printf("Logging out...\n");
            *loggedInStatus = 0; // Set loggedIn to 0 immediately

            char exitChoiceBuffer[10];
            getStringInput("Do you want to exit the system completely? (y/n): ", exitChoiceBuffer, sizeof(exitChoiceBuffer));
            if (exitChoiceBuffer[0] == 'y' || exitChoiceBuffer[0] == 'Y')
            {
                *keepRunningApp = 0; // Signal main loop to terminate
            }
            // If 'n' or anything else, keepRunningApp remains 1.
            // The mainSystemMenu loop will terminate because choice is 0.
            // The main() loop will then see loggedInStatus is 0 and keepRunningApp is 1, prompting login.
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0 && *keepRunningApp); // Also exit menu if app is shutting down
}

// --- Product Management Menu Function ---
void productManagementMenu()
{
    int choice;
    do
    {
        printf("\n--- Product Management Menu ---\n");
        printf("1. Add New Product\n");
        printf("2. Update Product Information\n");
        printf("3. Delete Product\n");
        printf("4. View Specific Product Details\n");
        printf("5. View All Products\n");
        printf("0. Return to Main System Menu\n");
        printf("-----------------------------------\n");
        printf("Enter your choice: ");

        choice = getIntInput("");

        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            updateProduct();
            break;
        case 3:
            deleteProduct();
            break;
        case 4:
            viewSpecificProductDetails();
            break;
        case 5:
            viewAllProducts();
            break;
        case 0:
            printf("Returning to Main System Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

// --- Placeholder Menu Functions for Other Roles ---
void inventoryManagementMenu()
{
    printf("\n--- Inventory Management Menu ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
    getchar();
}

void stockManagementMenu()
{
    printf("\n--- Stock Management Menu ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
    getchar();
}

void categoryManagementMenu() // New Placeholder
{
    printf("\n--- Category Management Menu ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
    getchar();
}

void supplierManagementMenu() // New Placeholder
{
    printf("\n--- Supplier Management Menu ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
    getchar();
}

void userTransactionManagementMenu()
{
    printf("\n--- User and Transaction Management Menu ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
    getchar();
}

// --- Login Function Implementations ---
int verifyAdminCredentials(const char *adminID, const char *password)
{
    FILE *file = fopen(ADMIN_FILE, "r");
    if (file == NULL)
    {
        printf("Error: Could not open admin credentials file '%s'.\n", ADMIN_FILE);
        printf("Please ensure '%s' exists and has admin data.\n", ADMIN_FILE);
        return 0;
    }

    Admin tempAdmin;
    char line_buffer[512];

    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';

        tempAdmin.adminID[0] = '\0';
        tempAdmin.name[0] = '\0';
        tempAdmin.address[0] = '\0';
        tempAdmin.email[0] = '\0';
        tempAdmin.contactNumber[0] = '\0';
        tempAdmin.password[0] = '\0';

        int itemsScanned = sscanf(line_buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
                                  tempAdmin.adminID, tempAdmin.name, tempAdmin.address,
                                  tempAdmin.email, tempAdmin.contactNumber, tempAdmin.password);

        if (itemsScanned == 6)
        {
            if (strcmp(tempAdmin.adminID, adminID) == 0 && strcmp(tempAdmin.password, password) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

int handleLogin()
{
    char adminID[MAX_ID_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int loginAttempts = 0;
    const int MAX_LOGIN_ATTEMPTS = 3;

    printf("\n--- System Login ---\n");
    while (loginAttempts < MAX_LOGIN_ATTEMPTS)
    {
        getStringInput("Enter Admin ID: ", adminID, MAX_ID_LENGTH);
        if (strlen(adminID) == 0)
        {
            printf("Admin ID cannot be empty.\n");
            loginAttempts++;
            printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - loginAttempts);
            if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
            {
                // The main loop will print the "Exiting system" message if login fails definitively
                return 0;
            }
            continue;
        }
        getStringInput("Enter Password: ", password, MAX_PASSWORD_LENGTH);
        if (strlen(password) == 0)
        {
            printf("Password cannot be empty.\n");
            loginAttempts++;
            printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - loginAttempts);
            if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
            {
                return 0;
            }
            continue;
        }

        if (verifyAdminCredentials(adminID, password))
        {
            FILE *file = fopen(ADMIN_FILE, "r");
            char adminName[MAX_NAME_LENGTH] = "Admin";
            if (file != NULL)
            {
                Admin tempAdminRead;
                char line_buffer_login[512];
                while (fgets(line_buffer_login, sizeof(line_buffer_login), file) != NULL)
                {
                    line_buffer_login[strcspn(line_buffer_login, "\n")] = '\0';
                    tempAdminRead.adminID[0] = '\0';
                    tempAdminRead.name[0] = '\0';
                    sscanf(line_buffer_login, "%[^,],%[^,],%*[^,],%*[^,],%*[^,],%*s",
                           tempAdminRead.adminID, tempAdminRead.name);
                    if (tempAdminRead.adminID[0] != '\0' && strcmp(tempAdminRead.adminID, adminID) == 0)
                    {
                        if (tempAdminRead.name[0] != '\0')
                            strcpy(adminName, tempAdminRead.name);
                        break;
                    }
                }
                fclose(file);
            }
            printf("Login successful! Welcome, %s.\n", adminName);
            return 1;
        }
        else
        {
            printf("Invalid Admin ID or Password.\n");
            loginAttempts++;
            printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - loginAttempts);
            if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
            {
                return 0;
            }
        }
    }
    // This part should ideally not be reached if MAX_LOGIN_ATTEMPTS >= 1
    // because the loop condition or returns inside should handle termination.
    // However, to be safe and ensure a return path:
    if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
    {
        printf("Maximum login attempts reached.\n"); // This message will be followed by main's exit message
    }
    return 0;
}

// --- Helper Function Implementations ---
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void getStringInput(const char *prompt, char *buffer, int buffer_size)
{
    printf("%s", prompt);
    if (fgets(buffer, buffer_size, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    else
    {
        buffer[0] = '\0';
        if (!feof(stdin) && !ferror(stdin) && strlen(buffer) == (size_t)buffer_size - 1)
        {
            clearInputBuffer();
        }
        else if (ferror(stdin))
        {
            perror("fgets error");
        }
    }
}

float getFloatInput(const char *prompt)
{
    char buffer[100];
    float value;
    int validInput;
    char *endptr;
    do
    {
        validInput = 1;
        getStringInput(prompt, buffer, sizeof(buffer));
        if (buffer[0] == '\0')
        {
            printf("Input cannot be empty. Please enter a non-negative number (e.g., 10.99).\n");
            validInput = 0;
            continue;
        }
        value = strtof(buffer, &endptr);
        if (*endptr != '\0' || endptr == buffer || value < 0)
        {
            printf("Invalid input. Please enter a non-negative number (e.g., 10.99).\n");
            validInput = 0;
        }
    } while (!validInput);
    return value;
}

int getIntInput(const char *prompt)
{
    char buffer[100];
    long value_long;
    int value_int;
    int validInput;
    char *endptr;
    do
    {
        validInput = 1;
        getStringInput(prompt, buffer, sizeof(buffer));
        if (buffer[0] == '\0')
        {
            if (strlen(prompt) > 0)
            {
                printf("Input cannot be empty. Please enter an integer.\n");
            }
            validInput = 0;
            if (strlen(prompt) == 0)
                return -999; // Indicate invalid menu input
            continue;
        }
        value_long = strtol(buffer, &endptr, 10);

        if (*endptr != '\0' || endptr == buffer)
        {
            if (strlen(prompt) > 0)
            {
                printf("Invalid input. Please enter a valid integer.\n");
            }
            else
            {
                return -999; // A distinct invalid menu choice
            }
            validInput = 0;
        }
        else if (value_long < -2147483648L || value_long > 2147483647L)
        {
            printf("Input out of integer range. Please enter a valid integer.\n");
            validInput = 0;
        }
        else
        {
            value_int = (int)value_long;
        }
    } while (!validInput);
    return value_int;
}

int productExists(const char *productID_to_check)
{
    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL)
    {
        return 0;
    }
    Inventory tempProduct;
    char line_buffer[512];
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        tempProduct.productID[0] = '\0';
        sscanf(line_buffer, "%[^,]", tempProduct.productID);
        if (tempProduct.productID[0] != '\0' && strcmp(tempProduct.productID, productID_to_check) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// --- Product Management Function Implementations ---
void addProduct()
{
    printf("\n--- Add New Product ---\n");
    Inventory newProduct;

    do
    {
        getStringInput("Enter Product ID (e.g., P001): ", newProduct.productID, MAX_ID_LENGTH);
        if (strlen(newProduct.productID) == 0)
        {
            printf("Product ID cannot be empty. Please try again.\n");
            newProduct.productID[0] = '\0';
            continue;
        }
        if (productExists(newProduct.productID))
        {
            printf("Product ID '%s' already exists. Please use a different ID.\n", newProduct.productID);
            newProduct.productID[0] = '\0';
        }
    } while (strlen(newProduct.productID) == 0);

    getStringInput("Enter Category ID (FK - e.g., C01): ", newProduct.categoryID, MAX_ID_LENGTH);
    if (strlen(newProduct.categoryID) == 0)
    {
        printf("Category ID is empty. Using 'N/A' as default.\n");
        strcpy(newProduct.categoryID, "N/A");
    }
    // Future enhancement: Validate if Category ID exists in a categories.txt file.

    getStringInput("Enter Product Name: ", newProduct.name, MAX_NAME_LENGTH);
    if (strlen(newProduct.name) == 0)
    {
        printf("Product Name is empty. Using 'Unknown Product' as default.\n");
        strcpy(newProduct.name, "Unknown Product");
    }

    newProduct.price = getFloatInput("Enter Product Price: ");
    newProduct.quantity = getIntInput("Enter Product Quantity (non-negative): ");
    while (newProduct.quantity < 0)
    {
        printf("Quantity cannot be negative. Please enter a non-negative value.\n");
        newProduct.quantity = getIntInput("Enter Product Quantity (non-negative): ");
    }

    getStringInput("Enter Product Description: ", newProduct.description, MAX_DESCRIPTION_LENGTH);
    if (strlen(newProduct.description) == 0)
    {
        strcpy(newProduct.description, "No description available.");
    }

    addDataInventory2(PRODUCT_FILE, newProduct.productID, newProduct.categoryID,
                      newProduct.name, newProduct.price, newProduct.quantity, newProduct.description);
}

void updateProduct()
{
    printf("\n--- Update Product Information ---\n");
    char productID_to_update[MAX_ID_LENGTH];
    char attribute_name[30];
    char newValue_str[MAX_DESCRIPTION_LENGTH];
    int choice;

    getStringInput("Enter Product ID of the product to update: ", productID_to_update, MAX_ID_LENGTH);
    if (strlen(productID_to_update) == 0)
    {
        printf("Product ID cannot be empty.\n");
        return;
    }

    if (!productExists(productID_to_update))
    {
        printf("Product with ID '%s' not found. Cannot update.\n", productID_to_update);
        return;
    }

    printf("\nWhich information do you want to update for Product ID '%s'?\n", productID_to_update);
    printf("1. Category ID (FK)\n");
    printf("2. Name\n");
    printf("3. Price\n");
    printf("4. Quantity\n");
    printf("5. Description\n");
    printf("0. Cancel Update\n");
    printf("Enter your choice: ");
    choice = getIntInput("");

    switch (choice)
    {
    case 1:
        strcpy(attribute_name, "categoryID");
        getStringInput("Enter new Category ID (FK): ", newValue_str, MAX_ID_LENGTH);
        if (strlen(newValue_str) == 0)
        {
            printf("Category ID cannot be empty for update. Update cancelled.\n");
            return;
        }
        // Future enhancement: Validate if new Category ID exists.
        break;
    case 2:
        strcpy(attribute_name, "name");
        getStringInput("Enter new Product Name: ", newValue_str, MAX_NAME_LENGTH);
        if (strlen(newValue_str) == 0)
        {
            printf("Product Name cannot be empty for update. Update cancelled.\n");
            return;
        }
        break;
    case 3:
    {
        strcpy(attribute_name, "price");
        float new_price = getFloatInput("Enter new Price: ");
        sprintf(newValue_str, "%.2f", new_price);
        break;
    }
    case 4:
    {
        strcpy(attribute_name, "quantity");
        int new_quantity = getIntInput("Enter new Quantity (non-negative): ");
        while (new_quantity < 0)
        {
            printf("Quantity cannot be negative. Please enter a non-negative value.\n");
            new_quantity = getIntInput("Enter new Quantity (non-negative): ");
        }
        sprintf(newValue_str, "%d", new_quantity);
        break;
    }
    case 5:
        strcpy(attribute_name, "description");
        getStringInput("Enter new Description: ", newValue_str, MAX_DESCRIPTION_LENGTH);
        break;
    case 0:
        printf("Update cancelled.\n");
        return;
    default:
        printf("Invalid choice for attribute. Update cancelled.\n");
        return;
    }
    updateDataInventory(PRODUCT_FILE, attribute_name, productID_to_update, newValue_str);
}

void deleteProduct()
{
    printf("\n--- Delete Product ---\n");
    char productID_to_delete[MAX_ID_LENGTH];
    char confirmation_buffer[10];

    getStringInput("Enter Product ID of the product to delete: ", productID_to_delete, MAX_ID_LENGTH);
    if (strlen(productID_to_delete) == 0)
    {
        printf("Product ID cannot be empty.\n");
        return;
    }

    if (!productExists(productID_to_delete))
    {
        printf("Product with ID '%s' not found. Cannot delete.\n", productID_to_delete);
        return;
    }

    getStringInput("Are you sure you want to delete product ID (y/n): ", confirmation_buffer, sizeof(confirmation_buffer));

    if (confirmation_buffer[0] == 'y' || confirmation_buffer[0] == 'Y')
    {
        deleteDataInventory(PRODUCT_FILE, productID_to_delete);
    }
    else
    {
        printf("Product deletion cancelled.\n");
    }
}

void viewSpecificProductDetails()
{
    printf("\n--- View Specific Product Details ---\n");
    char productID_to_view[MAX_ID_LENGTH];
    Inventory product;
    int found = 0;

    getStringInput("Enter Product ID to view: ", productID_to_view, MAX_ID_LENGTH);
    if (strlen(productID_to_view) == 0)
    {
        printf("Product ID cannot be empty.\n");
        return;
    }

    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL)
    {
        printf("Error: Could not open product file '%s'.\n", PRODUCT_FILE);
        return;
    }
    char line_buffer[1024];
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        product.productID[0] = '\0';
        product.categoryID[0] = '\0';
        product.name[0] = '\0';
        product.description[0] = '\0';
        product.price = 0.0f;
        product.quantity = 0;

        int itemsScanned = sscanf(line_buffer, "%[^,],%[^,],%[^,],%f,%d,%[^\n]",
                                  product.productID, product.categoryID, product.name,
                                  &product.price, &product.quantity, product.description);

        if (product.productID[0] != '\0' && strcmp(product.productID, productID_to_view) == 0)
        {
            found = 1;
            printf("\n--- Product Details for ID: %s (PK) ---\n", product.productID);
            printf("Category ID : %s (FK)\n", product.categoryID[0] == '\0' ? "N/A" : product.categoryID);
            printf("Name        : %s\n", product.name[0] == '\0' ? "N/A" : product.name);
            printf("Price       : %.2f\n", product.price);
            printf("Quantity    : %d\n", product.quantity);
            printf("Description : %s\n", (itemsScanned < 6 || product.description[0] == '\0') ? "(empty)" : product.description);
            printf("-----------------------------------\n");
            break;
        }
    }
    fclose(file);
    if (!found)
    {
        printf("Product with ID '%s' not found.\n", productID_to_view);
    }
}

void viewAllProducts()
{
    printf("\n--- All Products in Inventory ---\n");
    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL)
    {
        printf("Error: Could not open product file '%s' or it is empty.\n", PRODUCT_FILE);
        return;
    }
    Inventory product;
    int count = 0;
    char line_buffer[1024];

    printf("+------------+---------------+--------------------------------+------------+----------+------------------------------------+\n");
    printf("| Product ID | Category ID   | Name                           | Price      | Quantity | Description (first 34 chars)       |\n");
    printf("| (PK)       | (FK)          |                                |            |          |                                    |\n");
    printf("+------------+---------------+--------------------------------+------------+----------+------------------------------------+\n");

    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        product.productID[0] = '\0';
        product.categoryID[0] = '\0';
        product.name[0] = '\0';
        product.description[0] = '\0';
        product.price = 0.0f;
        product.quantity = 0;

        int itemsScanned = sscanf(line_buffer, "%[^,],%[^,],%[^,],%f,%d,%[^\n]",
                                  product.productID, product.categoryID, product.name,
                                  &product.price, &product.quantity, product.description);

        if (itemsScanned >= 5 && product.productID[0] != '\0')
        {
            printf("| %-10.10s | %-13.13s | %-30.30s | %-10.2f | %-8d | %-34.34s |\n",
                   product.productID,
                   product.categoryID[0] == '\0' ? "N/A" : product.categoryID,
                   product.name[0] == '\0' ? "N/A" : product.name,
                   product.price,
                   product.quantity,
                   (itemsScanned < 6 || product.description[0] == '\0') ? "(empty)" : product.description);
            count++;
        }
        else if (strlen(line_buffer) > 0 && line_buffer[0] != '\n' && line_buffer[0] != EOF && itemsScanned < 5)
        {
            // This condition helps to identify lines that are not empty but failed to parse enough fields.
            // It's optional to print this, as it might indicate data corruption or an empty line with just spaces.
            // printf("Skipping malformed or incomplete line in product file: %s\n", line_buffer);
        }
    }
    printf("+------------+---------------+--------------------------------+------------+----------+------------------------------------+\n");
    fclose(file);
    if (count == 0)
    {
        printf("No products found in the inventory.\n");
    }
    else
    {
        printf("Total products listed: %d\n", count);
    }
}
