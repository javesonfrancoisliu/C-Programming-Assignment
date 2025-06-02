#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "FileHandling.h"
#include "FormatHandling.h" // Included new header

#define PRODUCT_FILE "Inventory.txt"
#define ADMIN_FILE "Admins.txt"

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
float getFloatValidatedFH(const char *prompt);
int getIntegerValidatedFH(const char *prompt);
int getMenuChoiceValidatedFH(const char *prompt);

int productExists(const char *productID);
void clearInputBuffer();
void generateNewProductID(char *newIDBuffer, int bufferSize);
void trimWhitespace(char *str);

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

// --- Input Helper Functions using FormatHandling.h ---
int getIntegerValidatedFH(const char *prompt)
{
    char buffer[100];
    int value;
    while (1)
    {
        getStringInput(prompt, buffer, sizeof(buffer));
        trimWhitespace(buffer);
        if (buffer[0] == '\0')
        {
            if (strlen(prompt) > 0)
            {
                printf("Input cannot be empty. Please try again.\n");
            }
            else
            {
                // For menu choices with empty prompt, an empty input is just invalid.
                // Let the loop in menu function with default case handle "Invalid choice".
                // However, returning a specific error value might be cleaner for menus.
                // For now, let menu choice validation be slightly different if needed or rely on this.
                printf("Invalid input. Please enter a number.\n");
            }
            continue;
        }

        int is_numeric_string = 1;
        if (buffer[0] == '-' && buffer[1] != '\0')
        { // Allow negative if it's not just "-"
            for (int i = 1; buffer[i] != '\0'; ++i)
            { // Check digits after '-'
                if (!isdigit((unsigned char)buffer[i]))
                {
                    is_numeric_string = 0;
                    break;
                }
            }
        }
        else if (buffer[0] == '-' && buffer[1] == '\0')
        { // Just "-" is invalid
            is_numeric_string = 0;
        }
        else
        { // Check all for digits if not starting with '-' or if it is '+'
            char *p = buffer;
            if (*p == '+')
                p++; // Skip leading '+'
            if (*p == '\0' && *(p - 1) == '+')
                is_numeric_string = 0; // Just "+" is invalid

            while (*p != '\0')
            {
                if (!isdigit((unsigned char)*p))
                {
                    is_numeric_string = 0;
                    break;
                }
                p++;
            }
        }

        if (is_numeric_string)
        {
            // Note: FormatHandling.h's intChecker only supports positive integers based on its code (checks isdigit for all chars)
            // If you need to support negative integers, intChecker itself would need modification,
            // or you'd have to handle the sign separately before passing to a modified intChecker.
            // For now, assuming intChecker is used for positive integers or '0' as per its definition.
            // The pre-validation ensures only strings of digits are passed to intChecker.
            // If intChecker strictly only takes positive, then "0" might be an issue if it returns 0 for that too.
            // Let's use strtol for robust conversion after our pre-validation.
            char *endptr;
            long long_val = strtol(buffer, &endptr, 10);
            if (*endptr == '\0' && strlen(buffer) > 0)
            { // Conversion successful, entire string consumed
                if (long_val < -2147483648L || long_val > 2147483647L)
                { // Check int range
                    printf("Input out of integer range. Please enter a valid integer.\n");
                }
                else
                {
                    value = (int)long_val;
                    // If FormatHandling.h intChecker is preferred despite its limitations:
                    // value = intChecker(buffer);
                    // if (value == 0 && strcmp(buffer, "0") != 0) {
                    //     // This condition means intChecker failed for a non-"0" string
                    //     printf("Invalid integer format according to intChecker. Please use digits only.\n");
                    //     continue;
                    // }
                    return value;
                }
            }
            else
            {
                printf("Invalid integer format. Please use digits only.\n");
            }
        }
        else
        {
            if (strlen(prompt) > 0)
            {
                printf("Invalid integer. Please use digits only.\n");
            }
            else
            { // For menu, usually handled by default case
                printf("Invalid choice. Please enter a number.\n");
            }
        }
    }
}

float getFloatValidatedFH(const char *prompt)
{
    char buffer[100];
    float value;
    while (1)
    {
        getStringInput(prompt, buffer, sizeof(buffer));
        trimWhitespace(buffer);
        if (buffer[0] == '\0')
        {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }

        // Use strtod for robust float conversion after basic check for FormatHandling.h compatibility
        char *endptr;
        value = strtod(buffer, &endptr);

        if (*endptr == '\0' && strlen(buffer) > 0)
        { // Conversion successful, entire string was number-like
            // Now, check if it adheres to FormatHandling.h's floatChecker rules (digits, at most one dot)
            // This is mostly for conceptual alignment, as strtod is more permissive.
            // FormatHandling.h's floatChecker implies non-negative due to isdigit check.
            int isValidForFHChecker = 1;
            int dotCount = 0;
            int digitCount = 0;
            char *p = buffer;
            if (*p == '+' || *p == '-')
                p++; // strtod handles sign, floatChecker doesn't
            if (*p == '\0' && (*(p - 1) == '+' || *(p - 1) == '-'))
                isValidForFHChecker = 0; // only sign

            while (*p != '\0')
            {
                if (isdigit((unsigned char)*p))
                    digitCount++;
                else if (*p == '.')
                    dotCount++;
                else
                {
                    isValidForFHChecker = 0;
                    break;
                }
                p++;
            }
            if (dotCount > 1 || digitCount == 0)
                isValidForFHChecker = 0;

            if (isValidForFHChecker)
            {
                // Check non-negativity if implied by floatChecker (which it is)
                if (value < 0)
                {
                    printf("Value cannot be negative (as per FormatHandling.h floatChecker rules). Please enter a non-negative number.\n");
                    // continue loop
                }
                else
                {
                    return value;
                }
            }
            else
            {
                printf("Invalid float format. Please use digits and at most one decimal point.\n");
            }
        }
        else
        {
            printf("Invalid float format. Please try again.\n");
        }
    }
}

int getMenuChoiceValidatedFH(const char *prompt)
{ // Specifically for menu choices
    char buffer[100];
    int value;
    while (1)
    {
        getStringInput(prompt, buffer, sizeof(buffer)); // Prompt is "" for menu
        trimWhitespace(buffer);
        if (buffer[0] == '\0')
        {
            printf("Invalid choice. Please enter a number.\n");
            continue;
        }

        int is_numeric_string = 1;
        char *p = buffer;
        if (*p == '-' && *(p + 1) != '\0')
            p++; // Allow negative menu choices if ever needed, but usually not
        else if (*p == '-' && *(p + 1) == '\0')
            is_numeric_string = 0; // just "-"

        while (*p != '\0')
        {
            if (!isdigit((unsigned char)*p))
            {
                is_numeric_string = 0;
                break;
            }
            p++;
        }

        if (is_numeric_string)
        {
            // Using strtol for robust conversion for menu choices as well
            char *endptr_menu;
            long long_val_menu = strtol(buffer, &endptr_menu, 10);
            if (*endptr_menu == '\0')
            { // Full conversion
                if (long_val_menu < -2147483648L || long_val_menu > 2147483647L)
                {
                    printf("Choice out of integer range.\n");
                }
                else
                {
                    return (int)long_val_menu;
                }
            }
            else
            {
                printf("Invalid choice format. Please enter a number.\n");
            }
        }
        else
        {
            printf("Invalid choice. Please enter a number.\n");
        }
    }
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

        choice = getMenuChoiceValidatedFH("");

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
        case 0:
            printf("Logging out...\n");
            *loggedInStatus = 0;

            char exitChoiceBuffer[10];
            getStringInput("Do you want to exit the system completely? (y/n): ", exitChoiceBuffer, sizeof(exitChoiceBuffer));
            trimWhitespace(exitChoiceBuffer);
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

        choice = getMenuChoiceValidatedFH("");

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
        trimWhitespace(adminID);
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
                    trimWhitespace(tempAdminRead.adminID);
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

// Function to trim leading and trailing whitespace from a string
void trimWhitespace(char *str)
{
    if (str == NULL || *str == '\0')
        return;

    char *start = str;
    while (isspace((unsigned char)*start))
    {
        start++;
    }

    if (*start == '\0')
    {
        *str = '\0';
        return;
    }

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }
    *(end + 1) = '\0';

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

    char clean_productID_to_check[MAX_ID_LENGTH];
    strncpy(clean_productID_to_check, productID_to_check, MAX_ID_LENGTH - 1);
    clean_productID_to_check[MAX_ID_LENGTH - 1] = '\0';
    trimWhitespace(clean_productID_to_check);

    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        id_from_file[0] = '\0';

        if (sscanf(line_buffer, "%[^,]", id_from_file) == 1)
        {
            trimWhitespace(id_from_file);
            if (strcmp(id_from_file, clean_productID_to_check) == 0)
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
    trimWhitespace(idChoice);

    if (strcmp(idChoice, "auto") == 0)
    {
        generateNewProductID(newProduct.productID, MAX_ID_LENGTH);
    }
    else if (strcmp(idChoice, "manual") == 0)
    {
        do
        {
            getStringInput("Enter Product ID (e.g., P001): ", newProduct.productID, MAX_ID_LENGTH);
            trimWhitespace(newProduct.productID);
            if (strlen(newProduct.productID) == 0)
            {
                printf("Product ID cannot be empty. Please try again.\n");
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
        printf("Invalid choice ('%s'). Defaulting to manual ID entry.\n", idChoice);
        do
        {
            getStringInput("Enter Product ID (e.g., P001): ", newProduct.productID, MAX_ID_LENGTH);
            trimWhitespace(newProduct.productID);
            if (strlen(newProduct.productID) == 0)
            {
                printf("Product ID cannot be empty. Please try again.\n");
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

    newProduct.price = getFloatValidatedFH("Enter Product Price: ");
    newProduct.quantity = getIntegerValidatedFH("Enter Product Quantity (non-negative): ");

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
    choice = getMenuChoiceValidatedFH("");

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
        float new_price = getFloatValidatedFH("Enter new Price: ");
        sprintf(newValue_str, "%.2f", new_price);
        break;
    }
    case 4:
    {
        strcpy(attribute_name, "quantity");
        int new_quantity = getIntegerValidatedFH("Enter new Quantity (non-negative): ");
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
    trimWhitespace(confirmation_buffer);
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
        product.categoryID[0] = '\0';
        product.name[0] = '\0';
        product.description[0] = '\0';

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
            printf("Price       : RM %.2f\n", product.price); // Added RM
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
            // Adjusted format for RM, assuming price column can accommodate it
            printf("| %-10.10s | %-13.13s | %-30.30s | RM %-7.2f | %-8d | %-34.34s |\n",
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