#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "FileHandling.h"

#define PRODUCT_FILE "inventory.txt"
#define ADMIN_FILE "admins.txt"

// --- Function Prototypes ---
void mainSystemMenu(int *loggedInStatus, int *keepRunningApp);

void productManagementMenu();
void addProduct();
void updateProduct();
void deleteProduct();
void viewSpecificProductDetails();
void viewAllProducts();

void inventoryManagementMenu();
void stockManagementMenu();
void categoryManagementMenu();
void supplierManagementMenu();
void userTransactionManagementMenu();

int handleLogin();
int verifyAdminCredentials(const char *adminID, const char *password);

void getStringInput(const char *prompt, char *buffer, int buffer_size);
float getFloatInput(const char *prompt);
int getIntInput(const char *prompt);
int productExists(const char *productID);
void clearInputBuffer();
void generateNewProductID(char *newIDBuffer, int bufferSize);

int main()
{
    checkFileExist(ADMIN_FILE);
    checkFileExist(PRODUCT_FILE);

    int loggedIn = 0;
    int keepRunningApp = 1;

    do
    {
        if (!loggedIn)
        {
            loggedIn = handleLogin();
            if (!loggedIn)
            {
                keepRunningApp = 0;
            }
        }

        if (loggedIn && keepRunningApp)
        {
            mainSystemMenu(&loggedIn, &keepRunningApp);
        }

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
        printf("4. Category Management\n");
        printf("5. Supplier Management\n");
        printf("6. User and Transaction Management\n");
        printf("0. Logout\n");
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
            categoryManagementMenu();
            break;
        case 5:
            supplierManagementMenu();
            break;
        case 6:
            userTransactionManagementMenu();
            break;
        case 0: // Logout
            printf("Logging out...\n");
            *loggedInStatus = 0;

            char exitChoiceBuffer[10];
            getStringInput("Do you want to exit the system completely? (y/n): ", exitChoiceBuffer, sizeof(exitChoiceBuffer));
            if (exitChoiceBuffer[0] == 'y' || exitChoiceBuffer[0] == 'Y')
            {
                *keepRunningApp = 0;
            }
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0 && *keepRunningApp);
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

void categoryManagementMenu()
{
    printf("\n--- Category Management Menu ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
    getchar();
}

void supplierManagementMenu()
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
                printf("Maximum login attempts reached.\n");
                return 0;
            }
        }
    }
    if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
    {
        printf("Maximum login attempts reached.\n");
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
                return -999;
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
                return -999;
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

// Function to trim leading and trailing whitespace from a string
void trimWhitespace(char *str)
{
    if (str == NULL)
        return;
    char *start = str;
    char *end = str + strlen(str) - 1;

    // Trim leading space
    while (isspace((unsigned char)*start))
    {
        start++;
    }

    // Trim trailing space
    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }
    // Write new null terminator
    *(end + 1) = '\0';

    // Shift string to the left if leading spaces were trimmed
    if (start != str)
    {
        memmove(str, start, strlen(start) + 1);
    }
}

int productExists(const char *productID_to_check)
{
    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL)
    {
        return 0;
    }
    char line_buffer[512];
    char id_from_file[MAX_ID_LENGTH];

    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        id_from_file[0] = '\0';

        if (sscanf(line_buffer, "%[^,]", id_from_file) == 1)
        {
            trimWhitespace(id_from_file);
            if (strcmp(id_from_file, productID_to_check) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

void generateNewProductID(char *newIDBuffer, int bufferSize)
{
    FILE *file = fopen(PRODUCT_FILE, "r");
    char currentLine[1024];
    int maxNumericID = 0;

    if (file != NULL)
    {
        char tempID_from_file[MAX_ID_LENGTH];
        while (fgets(currentLine, sizeof(currentLine), file) != NULL)
        {
            tempID_from_file[0] = '\0';
            if (sscanf(currentLine, "%[^,]", tempID_from_file) == 1)
            {
                trimWhitespace(tempID_from_file);

                if (tempID_from_file[0] == 'P')
                {
                    int all_digits_after_P = 1;
                    if (tempID_from_file[1] == '\0')
                    {
                        all_digits_after_P = 0;
                    }
                    else
                    {
                        for (int k = 1; tempID_from_file[k] != '\0'; ++k)
                        {
                            if (!isdigit((unsigned char)tempID_from_file[k]))
                            {
                                all_digits_after_P = 0;
                                break;
                            }
                        }
                    }

                    if (all_digits_after_P && tempID_from_file[1] != '\0')
                    {
                        int numericPart = atoi(tempID_from_file + 1);
                        if (numericPart > maxNumericID)
                        {
                            maxNumericID = numericPart;
                        }
                    }
                }
            }
        }
        fclose(file);
    }

    maxNumericID++;

    snprintf(newIDBuffer, bufferSize, "P%03d", maxNumericID);

    while (productExists(newIDBuffer))
    {
        maxNumericID++;
        snprintf(newIDBuffer, bufferSize, "P%03d", maxNumericID);
    }
    printf("Auto-generated Product ID: %s\n", newIDBuffer);
}

// --- Product Management Function Implementations ---
void addProduct()
{
    printf("\n--- Add New Product ---\n");
    Inventory newProduct;
    char idChoice[10];

    printf("Do you want to enter Product ID manually or auto-generate? (manual/auto): ");
    getStringInput("", idChoice, sizeof(idChoice));

    for (int i = 0; idChoice[i]; i++)
    {
        idChoice[i] = tolower(idChoice[i]);
    }

    if (strcmp(idChoice, "auto") == 0)
    {
        generateNewProductID(newProduct.productID, MAX_ID_LENGTH);
    }
    else if (strcmp(idChoice, "manual") == 0)
    {
        do
        {
            getStringInput("Enter Product ID (e.g., P001): ", newProduct.productID, MAX_ID_LENGTH);
            if (strlen(newProduct.productID) == 0)
            {
                printf("Product ID cannot be empty. Please try again.\n");
                newProduct.productID[0] = '\0';
                continue;
            }
            trimWhitespace(newProduct.productID);
            if (strlen(newProduct.productID) == 0)
            {
                printf("Product ID cannot be effectively empty after trimming. Please try again.\n");
                continue;
            }

            if (productExists(newProduct.productID))
            {
                printf("Product ID '%s' already exists. Please use a different ID.\n", newProduct.productID);
                newProduct.productID[0] = '\0';
            }
        } while (strlen(newProduct.productID) == 0);
    }
    else
    {
        printf("Invalid choice for ID input. Defaulting to manual ID entry.\n");
        do
        {
            getStringInput("Enter Product ID (e.g., P001): ", newProduct.productID, MAX_ID_LENGTH);
            if (strlen(newProduct.productID) == 0)
            {
                printf("Product ID cannot be empty. Please try again.\n");
                newProduct.productID[0] = '\0';
                continue;
            }
            trimWhitespace(newProduct.productID);
            if (strlen(newProduct.productID) == 0)
            {
                printf("Product ID cannot be effectively empty after trimming. Please try again.\n");
                continue;
            }

            if (productExists(newProduct.productID))
            {
                printf("Product ID '%s' already exists. Please use a different ID.\n", newProduct.productID);
                newProduct.productID[0] = '\0';
            }
        } while (strlen(newProduct.productID) == 0);
    }

    getStringInput("Enter Category ID (FK - e.g., CT01): ", newProduct.categoryID, MAX_ID_LENGTH);
    trimWhitespace(newProduct.categoryID);
    if (strlen(newProduct.categoryID) == 0)
    {
        printf("Category ID is empty. Using 'N/A' as default.\n");
        strcpy(newProduct.categoryID, "N/A");
    }

    getStringInput("Enter Product Name: ", newProduct.name, MAX_NAME_LENGTH);
    trimWhitespace(newProduct.name);
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
    trimWhitespace(newProduct.description);
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
    trimWhitespace(productID_to_update);
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
        trimWhitespace(newValue_str);
        if (strlen(newValue_str) == 0)
        {
            printf("Category ID cannot be empty for update. Update cancelled.\n");
            return;
        }
        break;
    case 2:
        strcpy(attribute_name, "name");
        getStringInput("Enter new Product Name: ", newValue_str, MAX_NAME_LENGTH);
        trimWhitespace(newValue_str);
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
        trimWhitespace(newValue_str);
        break;
    case 0:
        printf("Update cancelled.\n");
        return;
    default:
        printf("Invalid choice for attribute. Update cancelled.\n");
        return;
    }
    // Corrected the call to updateDataInventory
    updateDataInventory(PRODUCT_FILE, attribute_name, productID_to_update, newValue_str);
}

void deleteProduct()
{
    printf("\n--- Delete Product ---\n");
    char productID_to_delete[MAX_ID_LENGTH];
    char confirmation_buffer[10];

    getStringInput("Enter Product ID of the product to delete: ", productID_to_delete, MAX_ID_LENGTH);
    trimWhitespace(productID_to_delete);
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
    trimWhitespace(productID_to_view);
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

        char file_productID[MAX_ID_LENGTH] = "";
        int itemsScanned = sscanf(line_buffer, "%[^,],%[^,],%[^,],%f,%d,%[^\n]",
                                  file_productID, product.categoryID, product.name,
                                  &product.price, &product.quantity, product.description);

        trimWhitespace(file_productID);

        if (file_productID[0] != '\0' && strcmp(file_productID, productID_to_view) == 0)
        {
            found = 1;
            strcpy(product.productID, file_productID);

            printf("\n--- Product Details for ID: %s (PK) ---\n", product.productID);
            printf("Category ID : %s (FK)\n", product.categoryID[0] == '\0' ? "N/A" : product.categoryID);
            printf("Name        : %s\n", product.name[0] == '\0' ? "N/A" : product.name);
            printf("Price       : %.2f\n", product.price);
            printf("Quantity    : %d\n", product.quantity);
            printf("Description : %s\n", (strlen(product.description) == 0) ? "(empty)" : product.description);
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

        char file_productID[MAX_ID_LENGTH] = "";
        int itemsScanned = sscanf(line_buffer, "%[^,],%[^,],%[^,],%f,%d,%[^\n]",
                                  file_productID, product.categoryID, product.name,
                                  &product.price, &product.quantity, product.description);

        trimWhitespace(file_productID);
        strcpy(product.productID, file_productID);

        if (itemsScanned >= 5 && product.productID[0] != '\0')
        {
            printf("| %-10.10s | %-13.13s | %-30.30s | %-10.2f | %-8d | %-34.34s |\n",
                   product.productID,
                   product.categoryID[0] == '\0' ? "N/A" : product.categoryID,
                   product.name[0] == '\0' ? "N/A" : product.name,
                   product.price,
                   product.quantity,
                   (strlen(product.description) == 0) ? "(empty)" : product.description);
            count++;
        }
        else if (strlen(line_buffer) > 0 && line_buffer[0] != '\n' && line_buffer[0] != EOF && itemsScanned < 5)
        {
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
