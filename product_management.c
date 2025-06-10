// product_management.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For atof, atoi
#include <ctype.h>  // For isdigit etc.

// Include your new, updated header files provided by the lecturer
#include "FileHandling.h"
#include "FormatHandling.h"
#include "product_management.h" // <<<--- ADD THIS LINE

// Define filenames as constants for easy modification and consistency
#define INVENTORY_FILE "inventory.txt"
#define CATEGORIES_FILE "categories.txt"

// --- Forward Declarations of Functions within this file ---
void addNewProduct();
void updateProductInfo();
void deleteProduct();
void viewSpecificProductDetails();
void viewAllProducts_local();
int getProductDetails_local(const char *productID, Inventory *productOut);
int displayAndSelectCategory(char *selectedCategoryID);
int displayAndSelectProductID(char *selectedProductID);
void getValidString(char *outputBuffer, int bufferSize, const char *prompt);
int getValidIntegerInput(const char *prompt, int allowZero, int allowNegative);
float getValidFloatInput(const char *prompt, int allowZero, int allowNegative);
void printInventoryFields(const Inventory *product);
void addNewProduct_local(const Inventory *newProduct); // Local function to fix broken add functionality

// --- Helper Functions for Input Validation ---

/**
 * @brief Reads a string from stdin, validates it (not empty, within length limits).
 * @param outputBuffer Buffer to store the validated string.
 * @param bufferSize Size of outputBuffer.
 * @param prompt Message to display to the user.
 */
void getValidString(char *outputBuffer, int bufferSize, const char *prompt)
{
    char tempBuffer[256]; // Temporary buffer for fgets to prevent overflow
    do
    {
        printf("%s: ", prompt);
        if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL)
        { // Read user input
            printf("Error reading input. Please try again.\n");
            tempBuffer[0] = '\0'; // Reset buffer to loop again
            continue;
        }
        tempBuffer[strcspn(tempBuffer, "\n")] = 0; // Remove trailing newline

        if (tempBuffer[0] == '\0')
        { // Check for empty input
            printf("Input cannot be empty. Please try again.\n");
        }
        else if (strlen(tempBuffer) >= (unsigned int)bufferSize)
        { // Check for overly long input
            printf("Input too long. Maximum %d characters allowed. Please try again.\n", bufferSize - 1);
        }
        else
        {
            strcpy(outputBuffer, tempBuffer); // Copy valid input to output
            return;
        }
    } while (1);
}

/**
 * @brief Gets a valid integer input from the user.
 * @param prompt Message to display.
 * @param allowZero Flag to allow zero as valid input.
 * @param allowNegative Flag to allow negative numbers as valid input.
 * @return The validated integer.
 */
int getValidIntegerInput(const char *prompt, int allowZero, int allowNegative)
{
    char buffer[100];
    int value;
    int isValid;

    do
    {
        isValid = 1;                                    // Assume input is valid initially
        getValidString(buffer, sizeof(buffer), prompt); // Get string input first
        value = intChecker(buffer);                     // Validate and convert string to int (from FormatHandling.h)

        if (value == 0 && strcmp(buffer, "0") != 0)
        { // Error if intChecker returns 0 for non-"0" string
            printf("Invalid integer format. Please enter a whole number.\n");
            isValid = 0;
        }
        else if (!allowZero && value == 0 && strcmp(buffer, "0") == 0)
        { // Check if zero is disallowed
            printf("Zero is not allowed for this field. Please try again.\n");
            isValid = 0;
        }
        else if (!allowNegative && value < 0)
        { // Check if negative is disallowed
            printf("Negative numbers are not allowed for this field. Please try again.\n");
            isValid = 0;
        }
    } while (!isValid); // Loop until valid integer is entered
    return value;
}

/**
 * @brief Gets a valid float input from the user.
 * @param prompt Message to display.
 * @param allowZero Flag to allow zero as valid input.
 * @param allowNegative Flag to allow negative numbers as valid input.
 * @return The validated float.
 */
float getValidFloatInput(const char *prompt, int allowZero, int allowNegative)
{
    char buffer[100];
    float value;
    int isValid;

    do
    {
        isValid = 1;                                    // Assume input is valid initially
        getValidString(buffer, sizeof(buffer), prompt); // Get string input
        value = floatChecker(buffer);                   // Validate and convert string to float (from FormatHandling.h)

        // Check for genuine conversion errors vs. valid zero input
        if (value == 0.0f && !(strcmp(buffer, "0") == 0 || strcmp(buffer, "0.0") == 0 || strcmp(buffer, ".0") == 0 || strcmp(buffer, "0.") == 0))
        {
            printf("Invalid float format. Please enter a number (e.g., 123.45).\n");
            isValid = 0;
        }
        else if (!allowZero && value == 0.0f && (strcmp(buffer, "0") == 0 || strcmp(buffer, "0.0") == 0 || strcmp(buffer, ".0") == 0 || strcmp(buffer, "0.") == 0))
        { // Check if zero is disallowed
            printf("Zero is not allowed for this field. Please try again.\n");
            isValid = 0;
        }
        else if (!allowNegative && value < 0.0f)
        { // Check if negative is disallowed
            printf("Negative numbers are not allowed for this field. Please try again.\n");
            isValid = 0;
        }
    } while (!isValid); // Loop until valid float is entered
    return value;
}

// --- Local Helper Functions for Data Retrieval, and Validation ---

/**
 * @brief Prints the fields of an Inventory struct.
 * @param product Pointer to the Inventory struct to print.
 */
void printInventoryFields(const Inventory *product)
{
    if (product == NULL)
    { // Basic null check
        printf("Error: Cannot print details of a NULL product.\n");
        return;
    }
    printf("Product ID   : %s\n", product->productID);
    printf("Category ID  : %s\n", product->categoryID);
    printf("Name         : %s\n", product->name);
    printf("Price        : %.2f\n", product->price);
    printf("Quantity     : %d\n", product->quantity);
    printf("Description  : %s\n", product->description);
}

/**
 * @brief Retrieves all details of a specific product by ID from the inventory file.
 * This local function reads the file directly for more control than the FileHandling.h functions.
 * @param productID The ID of the product to find.
 * @param productOut Pointer to Inventory struct to store found product's details.
 * @return 1 if product found and details loaded, 0 otherwise.
 */
int getProductDetails_local(const char *productID, Inventory *productOut)
{
    FILE *file = fopen(INVENTORY_FILE, "r"); // Open inventory file for reading
    if (file == NULL)
    {
        return 0; // File open error
    }

    Inventory item_buffer;    // Temporary struct to hold data read from file line
    char tempPriceStr[50];    // Buffer for string representation of price
    char tempQuantityStr[50]; // Buffer for string representation of quantity

    // Read file line by line, parsing CSV format
    // NOTE: This reads price and quantity as strings and converts them, which is more robust
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                  item_buffer.productID, item_buffer.categoryID, item_buffer.name,
                  tempPriceStr, tempQuantityStr, item_buffer.description) == 6)
    { // Ensure all 6 fields are read
        if (strcmp(item_buffer.productID, productID) == 0)
        { // Check if current productID matches target
            // Copy found product details to output struct
            strcpy(productOut->productID, item_buffer.productID);
            strcpy(productOut->categoryID, item_buffer.categoryID);
            strcpy(productOut->name, item_buffer.name);
            productOut->price = atof(tempPriceStr);       // Convert string to float
            productOut->quantity = atoi(tempQuantityStr); // Convert string to int
            strcpy(productOut->description, item_buffer.description);
            fclose(file); // Close file
            return 1;     // Product found
        }
    }
    fclose(file); // Close file if product not found after checking all lines
    return 0;     // Product not found
}

/**
 * @brief Displays available categories, lets user select one by number.
 * Allows cancellation by entering 0.
 * @param selectedCategoryID Buffer to store the ID of the chosen category.
 * @return 1 if selected, 0 if cancelled/error.
 */
int displayAndSelectCategory(char *selectedCategoryID)
{
    FILE *file = fopen(CATEGORIES_FILE, "r"); // Open categories file
    if (file == NULL)
    {
        printf("Error: Could not open categories file '%s'. Cannot select category.\n", CATEGORIES_FILE);
        return 0;
    }

    Category categoriesList[MAX_RECORDS]; // Array to hold categories from file
    int categoryCount = 0;                // Counter for categories found

    printf("\n--- Available Categories ---\n");
    // Read and display categories
    while (categoryCount < MAX_RECORDS &&
           fscanf(file, "%[^,],%[^,],%[^\n]\n",
                  categoriesList[categoryCount].categoryID,
                  categoriesList[categoryCount].name,
                  categoriesList[categoryCount].description) == 3)
    {
        printf("%d. %s - %s\n", categoryCount + 1, categoriesList[categoryCount].categoryID, categoriesList[categoryCount].name);
        categoryCount++;
    }
    fclose(file); // Close file

    if (categoryCount == 0)
    { // Check if any categories were found
        printf("No categories available. Please add categories first.\n");
        return 0;
    }

    int choice; // User's menu choice
    do
    {
        choice = getValidIntegerInput("Select Category (by number, 0 to Cancel)", 1, 0); // Get user choice, allow 0
        if (choice == 0)
        { // Handle cancellation
            printf("Category selection cancelled.\n");
            return 0;
        }
        if (choice >= 1 && choice <= categoryCount)
        {                                                                      // Validate choice against list size
            strcpy(selectedCategoryID, categoriesList[choice - 1].categoryID); // Copy selected ID
            return 1;                                                          // Successful selection
        }
        else
        {
            printf("Invalid selection. Please enter a number between 1 and %d, or 0 to cancel.\n", categoryCount);
        }
    } while (1); // Loop until valid choice or cancellation
}

/**
 * @brief Displays available products, lets user select one by number.
 * Allows cancellation by entering 0.
 * @param selectedProductID Buffer to store the ID of the chosen product.
 * @return 1 if selected, 0 if cancelled/error.
 */
int displayAndSelectProductID(char *selectedProductID)
{
    FILE *file = fopen(INVENTORY_FILE, "r"); // Open inventory file
    if (file == NULL)
    {
        printf("Error: Could not open inventory file '%s'. Cannot select product.\n", INVENTORY_FILE);
        return 0;
    }

    Inventory productsList[MAX_RECORDS];        // Array to hold products from file
    int productCount = 0;                       // Counter for products found
    char tempPriceStr[50], tempQuantityStr[50]; // Buffers for fscanf

    printf("\n--- Available Products ---\n");
    // Read and display products (ID and Name)
    while (productCount < MAX_RECORDS &&
           fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                  productsList[productCount].productID,
                  productsList[productCount].categoryID,
                  productsList[productCount].name,
                  tempPriceStr,
                  tempQuantityStr,
                  productsList[productCount].description) == 6)
    {
        printf("%d. %s - %s\n", productCount + 1, productsList[productCount].productID, productsList[productCount].name);
        productCount++;
    }
    fclose(file); // Close file

    if (productCount == 0)
    { // Check if any products were found
        printf("No products available in inventory.\n");
        return 0;
    }

    int choice; // User's menu choice
    do
    {
        choice = getValidIntegerInput("Select Product (by number, 0 to Cancel)", 1, 0); // Get user choice, allow 0
        if (choice == 0)
        { // Handle cancellation
            printf("Product selection cancelled.\n");
            return 0;
        }
        if (choice >= 1 && choice <= productCount)
        {                                                                  // Validate choice
            strcpy(selectedProductID, productsList[choice - 1].productID); // Copy selected ID
            return 1;                                                      // Successful selection
        }
        else
        {
            printf("Invalid selection. Please enter a number between 1 and %d, or 0 to cancel.\n", productCount);
        }
    } while (1); // Loop until valid choice or cancellation
}

// --- Product Management Core Functions ---

/**
 * @brief WORKAROUND: Local function to correctly add a full product record to a file.
 * This is needed because the provided addDataInventory in FileHandling.h is broken/incomplete.
 * @param newProduct Pointer to the fully populated Inventory struct to add.
 */
void addNewProduct_local(const Inventory *newProduct)
{
    checkFileExist(INVENTORY_FILE);          // Ensure file exists
    FILE *file = fopen(INVENTORY_FILE, "a"); // Open in append mode
    if (file == NULL)
    {
        printf("CRITICAL ERROR: Could not open inventory file for writing.\n");
        return;
    }

    // Write the new product as a new line in CSV format
    fprintf(file, "%s,%s,%s,%.2f,%d,%s\n",
            newProduct->productID,
            newProduct->categoryID,
            newProduct->name,
            newProduct->price,
            newProduct->quantity,
            newProduct->description);

    fclose(file);
    printf("Inventory data added successfully.\n"); // Provide success message
}

/**
 * @brief Adds a new product to the inventory.
 * Auto-generates Product ID using the new generateID function.
 * Allows Category selection (cancellable). Displays preview of added product.
 */
void addNewProduct()
{
    printf("\n--- Add New Product ---\n");
    Inventory newProduct; // Struct for the new product

    // Use the generateID function from the new FormatHandling.h
    // It returns a pointer to a static buffer, so we must copy the result immediately.
    const char *generatedID_ptr = generateID(INVENTORY_FILE, "PROD0000"); // Pass prefix as specified
    if (strstr(generatedID_ptr, "Error") != NULL)
    { // Check if generateID returned an error
        printf("Error generating new Product ID: %s\n", generatedID_ptr);
        return;
    }
    strcpy(newProduct.productID, generatedID_ptr); // Copy the generated ID
    printf("Generated Product ID: %s\n", newProduct.productID);

    if (!displayAndSelectCategory(newProduct.categoryID))
    { // Let user select category
        printf("Product addition aborted.\n");
        return;
    }
    printf("Selected Category ID: %s\n", newProduct.categoryID);

    // Get remaining product details from user
    getValidString(newProduct.name, MAX_NAME_LENGTH, "Enter Product Name");
    newProduct.price = getValidFloatInput("Enter Product Price", 0, 0);
    newProduct.quantity = getValidIntegerInput("Enter Product Quantity", 1, 0);
    getValidString(newProduct.description, MAX_DESCRIPTION_LENGTH, "Enter Product Description");

    // Use our local workaround function to add the product, as the one in FileHandling.h is broken.
    addNewProduct_local(&newProduct);

    // Preview the added product
    printf("\n--- Product Added Successfully ---\n");
    Inventory addedProductDetails;
    if (getProductDetails_local(newProduct.productID, &addedProductDetails))
    {                                               // Fetch details to confirm
        printInventoryFields(&addedProductDetails); // Use helper to print fields
        printf("----------------------------------------\n");
    }
    else
    {
        printf("Could not retrieve details of the added product for preview.\n");
    }
}

/**
 * @brief Updates existing product information.
 * User selects product (cancellable), then can update multiple fields for that product.
 */
void updateProductInfo()
{
    printf("\n--- Update Product Information ---\n");
    char productIDToUpdate[MAX_ID_LENGTH]; // ID of product to update
    Inventory currentProduct;              // Struct to hold current product details

    if (!displayAndSelectProductID(productIDToUpdate))
    { // User selects product
        printf("Product update aborted.\n");
        return;
    }
    printf("Selected Product ID to update: %s\n", productIDToUpdate);

    char continueEditing; // To control the loop for editing multiple fields

    do
    { // Loop to allow multiple field updates for the same product
        if (!getProductDetails_local(productIDToUpdate, &currentProduct))
        { // Fetch current (possibly updated) details
            printf("Error: Product with ID '%s' not found. This is unexpected.\n", productIDToUpdate);
            return; // Exit if product suddenly disappears
        }

        printf("\nCurrent details for Product ID %s:\n", currentProduct.productID);
        printInventoryFields(&currentProduct); // Display current fields
        printf("----------------------------------------\n");

        printf("\nWhich information to update? (Enter 0 to Finish Editing this Product)\n");
        printf("1. Category ID\n");
        printf("2. Name\n");
        printf("3. Price\n");
        printf("4. Quantity\n");
        printf("5. Description\n");

        int fieldChoice = getValidIntegerInput("Enter your choice (1-5, 0 to Finish)", 1, 0); // Choice of field

        if (fieldChoice == 0)
        { // User chose to finish editing this product
            printf("Finished editing Product ID %s.\n", productIDToUpdate);
            break; // Exit the loop for editing fields of this product
        }

        char newValueBuffer[MAX_DESCRIPTION_LENGTH]; // Buffer for the new value (string)
        const char *attributeToUpdate = NULL;        // String representation of attribute for FileHandling.h

        switch (fieldChoice)
        { // Process choice of field to update
        case 1:
            attributeToUpdate = "categoryID";
            if (!displayAndSelectCategory(newValueBuffer))
            { // User selects new category
                printf("Category update for this field cancelled.\n");
                continue; // Skip this update, re-loop to ask for another field or finish
            }
            printf("New Category ID selected: %s\n", newValueBuffer);
            break;
        case 2:
            attributeToUpdate = "name";
            getValidString(newValueBuffer, MAX_NAME_LENGTH, "Enter new Product Name");
            break;
        case 3:
            attributeToUpdate = "price";
            float newPrice = getValidFloatInput("Enter new Price", 0, 0);
            sprintf(newValueBuffer, "%.2f", newPrice); // Convert float to string
            break;
        case 4:
            attributeToUpdate = "quantity";
            int newQuantity = getValidIntegerInput("Enter new Quantity", 1, 0);
            sprintf(newValueBuffer, "%d", newQuantity); // Convert int to string
            break;
        case 5:
            attributeToUpdate = "description";
            getValidString(newValueBuffer, MAX_DESCRIPTION_LENGTH, "Enter new Description");
            break;
        default:
            printf("Invalid field choice. Please try again.\n");
            continue; // Skip this iteration, re-loop
        }

        if (attributeToUpdate)
        {                                                                                              // If a valid attribute was chosen and new value obtained
            updateDataInventory(INVENTORY_FILE, attributeToUpdate, productIDToUpdate, newValueBuffer); // Call FileHandling.h
            // Preview updated product immediately
            printf("\n--- Field Updated Successfully (Preview) ---\n");
            Inventory updatedProductDetails;
            if (getProductDetails_local(productIDToUpdate, &updatedProductDetails))
            {
                printInventoryFields(&updatedProductDetails);
                printf("----------------------------------------\n");
            }
            else
            {
                printf("Could not retrieve updated details for preview.\n");
            }
        }

        // Ask if user wants to update another field for the SAME product
        char editAnotherField[10];
        getValidString(editAnotherField, sizeof(editAnotherField), "Update another field for this product? (yes/no, or 0 to finish)");
        if (strcmp(editAnotherField, "yes") == 0)
        {
            continueEditing = 'y';
        }
        else if (strcmp(editAnotherField, "0") == 0 || strcmp(editAnotherField, "no") == 0)
        {
            continueEditing = 'n';
            printf("Finished editing Product ID %s.\n", productIDToUpdate);
        }
        else
        {
            printf("Invalid input. Assuming no further edits for this product.\n");
            continueEditing = 'n';
        }

    } while (continueEditing == 'y'); // Loop while user wants to continue editing this product
}

/**
 * @brief Deletes a product from inventory.
 * User selects product (cancellable), confirms (cancellable).
 */
void deleteProduct()
{
    printf("\n--- Delete Product from Inventory ---\n");
    char productIDToDelete[MAX_ID_LENGTH]; // ID of product to delete
    char confirmation[10];                 // For "yes"/"no" or "0"
    Inventory productToDeleteDetails;      // Details for confirmation display

    if (!displayAndSelectProductID(productIDToDelete))
    { // User selects product
        printf("Product deletion aborted.\n");
        return;
    }
    printf("Selected Product ID to delete: %s\n", productIDToDelete);

    if (!getProductDetails_local(productIDToDelete, &productToDeleteDetails))
    { // Fetch details
        printf("Error: Product with ID '%s' not found after selection. This is unexpected.\n", productIDToDelete);
        return;
    }

    // Display full details before asking for final confirmation
    printf("\nYou are about to delete the following product:\n");
    printInventoryFields(&productToDeleteDetails);
    printf("----------------------------------------\n");

    printf("Are you sure you want to delete Product ID '%s'? (yes/no, or 0 to cancel): ", productIDToDelete);
    getValidString(confirmation, sizeof(confirmation), "");

    if (strcmp(confirmation, "0") == 0)
    { // User cancels deletion
        printf("Deletion cancelled by user.\n");
        return;
    }

    if (strcmp(confirmation, "yes") == 0)
    {                                                           // Confirm deletion
        deleteDataInventory(INVENTORY_FILE, productIDToDelete); // Call FileHandling.h
    }
    else
    { // Any other input (like "no" or gibberish) cancels
        printf("Deletion cancelled.\n");
    }
}

/**
 * @brief Views details of a specific product.
 * User selects product (cancellable).
 */
void viewSpecificProductDetails()
{
    printf("\n--- View Specific Product Details ---\n");
    char productIDToView[MAX_ID_LENGTH]; // ID of product to view

    if (!displayAndSelectProductID(productIDToView))
    { // User selects product
        printf("View product details aborted.\n");
        return;
    }

    Inventory product; // Struct to hold product details
    if (getProductDetails_local(productIDToView, &product))
    { // Fetch details
        printf("\nDetails for Product ID: %s\n", product.productID);
        printf("----------------------------------------\n");
        printInventoryFields(&product); // Print fields using helper
        printf("----------------------------------------\n");
    }
    else
    {
        // This case should be rare if displayAndSelectProductID ensures valid selection
        printf("Product with productID '%s' not found after selection.\n", productIDToView);
    }
}

/**
 * @brief Displays all products in inventory.
 * Reads file directly and prints details for each.
 */
void viewAllProducts_local()
{
    printf("\n--- All Products in Inventory ---\n");
    FILE *file = fopen(INVENTORY_FILE, "r"); // Open inventory file
    if (file == NULL)
    {
        printf("Inventory is empty or file '%s' cannot be opened.\n", INVENTORY_FILE);
        return;
    }

    Inventory item_buffer;    // Buffer for reading each product
    char tempPriceStr[50];    // String buffer for price
    char tempQuantityStr[50]; // String buffer for quantity
    int count = 0;            // Product counter

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                  item_buffer.productID, item_buffer.categoryID, item_buffer.name,
                  tempPriceStr, tempQuantityStr, item_buffer.description) == 6)
    { // Read all fields
        count++;
        item_buffer.price = atof(tempPriceStr);       // Convert
        item_buffer.quantity = atoi(tempQuantityStr); // Convert

        printf("\n--- Product %d ---\n", count); // Header for each product
        printInventoryFields(&item_buffer);      // Print fields using helper
        printf("----------------------------------------\n");
    }

    fclose(file); // Close file

    if (count == 0)
    { // Check if any products were displayed
        printf("\nNo products found in inventory.\n");
    }
    else
    {
        printf("\nTotal products displayed: %d\n", count);
    }
}

// --- Main Menu for Product Management ---

/**
 * @brief Displays the main product management menu and handles user interaction.
 */
void productManagementMenu()
{
    int choice;                      // User's menu choice
    checkFileExist(INVENTORY_FILE);  // Ensure inventory file exists/is created
    checkFileExist(CATEGORIES_FILE); // Ensure categories file exists/is created

    do
    {
        // Display menu options
        printf("\n\n--- Product Management Menu ---\n");
        printf("1. Add New Product\n");
        printf("2. Update Product Information\n");
        printf("3. Delete Product\n");
        printf("4. View Specific Product Details\n");
        printf("5. View All Products\n");
        printf("0. Back to Main Menu / Exit\n");
        printf("---------------------------------\n");

        choice = getValidIntegerInput("Enter your choice", 1, 0); // Get user's choice, allow 0 to exit

        switch (choice)
        { // Process choice
        case 1:
            addNewProduct();
            break;
        case 2:
            updateProductInfo();
            break;
        case 3:
            deleteProduct();
            break;
        case 4:
            viewSpecificProductDetails();
            break;
        case 5:
            viewAllProducts_local();
            break;
        case 0:
            printf("Returning from Product Management...\n"); // Exit message
            break;
        default:
            printf("Invalid choice. Please try again.\n"); // Handle invalid input
        }

        if (choice != 0)
        { // For any action other than exit
            printf("\nPress Enter to continue...");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ; // Clear input buffer

            if (c == EOF)
            { // Handle unexpected end-of-file
                break;
            }
            getchar(); // Wait for Enter key press
        }
    } while (choice != 0); // Loop until user chooses to exit
}