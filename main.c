#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>        // For isdigit, etc.
#include "FileHandling.h" // Your provided header file

// Define filenames
#define PRODUCT_FILE "inventory.txt"
#define ADMIN_FILE "admins.txt" // File for storing admin credentials

// --- Function Prototypes for Main System Menu ---
void mainSystemMenu();

// --- Function Prototypes for Product Management ---
void productManagementMenu(); // Forward declaration
void addProduct();
void updateProduct();
void deleteProduct();
void viewSpecificProductDetails();
void viewAllProducts();

// --- Function Prototypes for Other Modules (Placeholders) ---
void inventoryManagementMenu();        // Placeholder
void stockManagementMenu();            // Placeholder
void categorySupplierManagementMenu(); // Placeholder
void userTransactionManagementMenu();  // Placeholder

// --- Function Prototypes for Login ---
int handleLogin();
int verifyAdminCredentials(const char *adminID, const char *password);

// --- Helper Function Prototypes ---
void getStringInput(const char *prompt, char *buffer, int buffer_size);
float getFloatInput(const char *prompt);
int getIntInput(const char *prompt);
int productExists(const char *productID); // Checks if a product ID already exists in the file
void clearInputBuffer();                  // Utility to clear stdin

// --- Main System Menu Function ---
void mainSystemMenu()
{
    int choice;
    do
    {
        printf("\n--- Main System Menu ---\n");
        printf("1. Product Management\n");
        printf("2. Inventory Management (Placeholder)\n");
        printf("3. Stock Management (Placeholder)\n");
        printf("4. Category and Supplier Management (Placeholder)\n");
        printf("5. User and Transaction Management (Placeholder)\n");
        printf("0. Logout and Exit System\n");
        printf("--------------------------\n");
        printf("Enter your choice: ");

        // Using getIntInput for safer menu choice input
        choice = getIntInput(""); // Prompt is already printed

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
            categorySupplierManagementMenu();
            break;
        case 5:
            userTransactionManagementMenu();
            break;
        case 0:
            printf("Logging out and exiting system...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
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
        printf("0. Return to Main System Menu\n"); // Changed to reflect hierarchy
        printf("-----------------------------------\n");
        printf("Enter your choice: ");

        // Using getIntInput for safer menu choice input
        choice = getIntInput(""); // Prompt is already printed

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
    printf("\n--- Inventory Management Menu (Placeholder) ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer(); // Wait for user to press Enter
}

void stockManagementMenu()
{
    printf("\n--- Stock Management Menu (Placeholder) ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
}

void categorySupplierManagementMenu()
{
    printf("\n--- Category and Supplier Management Menu (Placeholder) ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
}

void userTransactionManagementMenu()
{
    printf("\n--- User and Transaction Management Menu (Placeholder) ---\n");
    printf("This module is not yet implemented.\n");
    printf("Press Enter to return to Main System Menu...");
    clearInputBuffer();
}

// --- Login Function Implementations ---

// Verifies admin credentials against the ADMIN_FILE
// Returns 1 if credentials are valid, 0 otherwise.
int verifyAdminCredentials(const char *adminID, const char *password)
{
    FILE *file = fopen(ADMIN_FILE, "r");
    if (file == NULL)
    {
        printf("Error: Could not open admin credentials file '%s'.\n", ADMIN_FILE);
        printf("Please ensure '%s' exists and has admin data (e.g., using addDataAdmin2 from FileHandling.h).\n", ADMIN_FILE);
        // Example of how one might add an admin:
        // addDataAdmin2(ADMIN_FILE, "A001", "Default Admin", "123 Main St", "admin@example.com", "0123456789", "password123");
        return 0; // Cannot verify if file doesn't exist or can't be opened
    }

    Admin tempAdmin;       // From FileHandling.h
    char line_buffer[512]; // Buffer to read each line

    // The FileHandling.h addDataAdmin2 function writes 6 fields:
    // adminID,name,address,email,contactNumber,password
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL)
    {
        line_buffer[strcspn(line_buffer, "\n")] = '\0'; // Remove newline

        // Initialize struct fields to avoid using old data from previous loop iterations
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
        { // Ensure all fields were parsed
            if (strcmp(tempAdmin.adminID, adminID) == 0 && strcmp(tempAdmin.password, password) == 0)
            {
                fclose(file);
                return 1; // Credentials match
            }
        }
    }
    fclose(file);
    return 0; // No matching credentials found
}

// Handles the login process
// Returns 1 for successful login, 0 for failure after attempts.
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
            loginAttempts++; // Count as an attempt
            printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - loginAttempts);
            if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
            {
                printf("Maximum login attempts reached. Exiting system.\n");
                return 0;
            }
            continue;
        }
        getStringInput("Enter Password: ", password, MAX_PASSWORD_LENGTH);
        if (strlen(password) == 0)
        {
            printf("Password cannot be empty.\n");
            loginAttempts++; // Count as an attempt
            printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - loginAttempts);
            if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
            {
                printf("Maximum login attempts reached. Exiting system.\n");
                return 0;
            }
            continue;
        }

        if (verifyAdminCredentials(adminID, password))
        {
            // Fetch the admin's actual name to display in the welcome message
            FILE *file = fopen(ADMIN_FILE, "r");
            char adminName[MAX_NAME_LENGTH] = "Admin"; // Default name
            if (file != NULL)
            {
                Admin tempAdminRead;
                char line_buffer_login[512];
                while (fgets(line_buffer_login, sizeof(line_buffer_login), file) != NULL)
                {
                    line_buffer_login[strcspn(line_buffer_login, "\n")] = '\0';
                    sscanf(line_buffer_login, "%[^,],%[^,],%*[^,],%*[^,],%*[^,],%*s",
                           tempAdminRead.adminID, tempAdminRead.name);
                    if (strcmp(tempAdminRead.adminID, adminID) == 0)
                    {
                        strcpy(adminName, tempAdminRead.name);
                        break;
                    }
                }
                fclose(file);
            }
            printf("Login successful! Welcome, %s.\n", adminName);
            return 1; // Success
        }
        else
        {
            printf("Invalid Admin ID or Password.\n");
            loginAttempts++;
            printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - loginAttempts);
            if (loginAttempts >= MAX_LOGIN_ATTEMPTS)
            {
                printf("Maximum login attempts reached. Exiting system.\n");
                return 0; // Failure
            }
        }
    }
    return 0; // Should not be reached if loop logic is correct
}

int main()
{
    // Ensure the necessary data files exist.
    checkFileExist(ADMIN_FILE);
    checkFileExist(PRODUCT_FILE);
    // Add checkFileExist for other data files as they are implemented (e.g., categories.txt, suppliers.txt etc.)

    if (handleLogin())
    {
        // If login is successful, show the Main System Menu
        mainSystemMenu();
    }
    // If login fails, the program will terminate as per handleLogin's message.

    printf("\nSystem shutting down. Thank you!\n");
    return 0;
}

// --- Helper Function Implementations ---

// Utility to clear the input buffer
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Gets string input safely and removes trailing newline
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
        if (!feof(stdin) && !ferror(stdin))
        {                       // If not EOF and no error, buffer might be too small or other issue
            clearInputBuffer(); // Clear rest of the line if fgets didn't consume it
        }
        else if (ferror(stdin))
        {
            perror("fgets error"); // Print system error message for fgets
            // Potentially exit or handle more gracefully
        }
        // If feof, nothing more to read.
    }
}

// Gets float input with validation (non-negative)
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

// Gets integer input with validation (non-negative for most cases, menu choices can be 0)
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
            // For menu choices, an empty prompt string "" is passed, so don't print "Input cannot be empty" here.
            // The calling menu loop will handle invalid (non-numeric) input.
            if (strlen(prompt) > 0)
            { // Only print if it's a data input prompt
                printf("Input cannot be empty. Please enter an integer.\n");
            }
            validInput = 0;
            continue;
        }
        value_long = strtol(buffer, &endptr, 10);

        // Allow negative for general purpose, but specific functions might re-validate for non-negative.
        // For menu choices, 0 is valid.
        if (*endptr != '\0' || endptr == buffer)
        { // Check if conversion failed or nothing was converted
            if (strlen(prompt) > 0)
            { // Only print for data input prompts
                printf("Invalid input. Please enter a valid integer.\n");
            }
            else
            { // For menu choices, let the menu loop print "Invalid choice"
                // To ensure the menu loop in productManagementMenu or mainSystemMenu works correctly
                // when non-numeric input is given to getIntInput, we should return a value
                // that will trigger the "default" case in the switch.
                return -999; // A distinct invalid menu choice
            }
            validInput = 0;
        }
        else if (value_long < -2147483648L || value_long > 2147483647L)
        { // Check int range
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

// Checks if a product ID already exists in the product file.
// Returns 1 if exists, 0 otherwise.
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
    { // Specific validation for non-negative quantity
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
        else if (strlen(line_buffer) > 0 && line_buffer[0] != '\n' && line_buffer[0] != EOF)
        {
            // Optional: printf("Skipping potentially malformed or empty line in product file.\n");
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