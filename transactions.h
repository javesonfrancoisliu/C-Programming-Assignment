#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileHandling.h"
#include "FormatHandling.h"

// Function Prototypes
void addCustomer();
void updateCustomer();
void deleteCustomer();
void viewCustomer();
void addTransaction();
void viewTransaction();
void updateTransaction();
void deleteTransaction();
void logTransaction(const Transaction *t);
int adminLogin();

// === Main Menu ===
int main() {
    int choice;
    if (!adminLogin()) {
        printf("Access Denied.\n");
        return 1;
    }

    do {
        printf("\n--- User & Transaction Management ---\n");
        printf("1. Add Customer\n");
        printf("2. Update Customer\n");
        printf("3. Delete Customer\n");
        printf("4. View Customer\n");
        printf("5. Add Transaction\n");
        printf("6. View Transaction\n");
        printf("7. Update Transaction\n");
        printf("8. Delete Transaction\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: updateCustomer(); break;
            case 3: deleteCustomer(); break;
            case 4: viewCustomer(); break;
            case 5: addTransaction(); break;
            case 6: viewTransaction(); break;
            case 7: updateTransaction(); break;
            case 8: deleteTransaction(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}

int adminLogin() {
    char adminID[21], password[21];
    printf("Admin ID: "); fgets(adminID, sizeof(adminID), stdin); adminID[strcspn(adminID, "\n")] = 0;
    printf("Password: "); fgets(password, sizeof(password), stdin); password[strcspn(password, "\n")] = 0;

    FILE *file = fopen("admin.txt", "r");
    if (!file) {
        printf("Admin file not found.\n");
        return 0;
    }

    Admin temp;
    while (fscanf(file, "%20[^,],%50[^,],%100[^,],%50[^,],%20[^,],%20[^\n]\n",
                  temp.adminID, temp.name, temp.address, temp.email, temp.contactNumber, temp.password) == 6) {
        if (strcmp(temp.adminID, adminID) == 0 && strcmp(temp.password, password) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void addCustomer() {
    char id[21], name[51], address[101], email[51], phone[21];
    printf("Customer ID: "); fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;
    printf("Name: "); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
    printf("Address: "); fgets(address, sizeof(address), stdin); address[strcspn(address, "\n")] = 0;
    printf("Email: "); fgets(email, sizeof(email), stdin); email[strcspn(email, "\n")] = 0;
    printf("Phone: "); fgets(phone, sizeof(phone), stdin); phone[strcspn(phone, "\n")] = 0;

    addDataCustomer2("customer.txt", id, name, address, email, phone);
}

void updateCustomer() {
    char id[21], field[51], value[51];
    printf("Customer ID: "); fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;
    printf("Field to update (name/address/email/contactNumber): ");
    fgets(field, sizeof(field), stdin); field[strcspn(field, "\n")] = 0;
    printf("New value: "); fgets(value, sizeof(value), stdin); value[strcspn(value, "\n")] = 0;

    updateDataCustomer("customer.txt", field, id, value);
}

void deleteCustomer() {
    char id[21];
    printf("Customer ID: "); fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;
    deleteDataCustomer("customer.txt", id);
}

void viewCustomer() {
    char id[21], attr[51];
    printf("Customer ID: "); fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;
    printf("Attribute (name/address/email/contactNumber): ");
    fgets(attr, sizeof(attr), stdin); attr[strcspn(attr, "\n")] = 0;

    readDataCustomer("customer.txt", attr, id);
}

void addTransaction() {
    Transaction t;
    char quantityStr[21], totalStr[21];

    printf("Transaction ID: "); fgets(t.transactionID, sizeof(t.transactionID), stdin); t.transactionID[strcspn(t.transactionID, "\n")] = 0;
    printf("Admin ID: "); fgets(t.adminID, sizeof(t.adminID), stdin); t.adminID[strcspn(t.adminID, "\n")] = 0;
    printf("Product ID: "); fgets(t.productID, sizeof(t.productID), stdin); t.productID[strcspn(t.productID, "\n")] = 0;
    printf("Supplier ID: "); fgets(t.supplierID, sizeof(t.supplierID), stdin); t.supplierID[strcspn(t.supplierID, "\n")] = 0;
    printf("Date (DD-MM-YYYY): "); fgets(t.date, sizeof(t.date), stdin); t.date[strcspn(t.date, "\n")] = 0;

    printf("Quantity: "); fgets(quantityStr, sizeof(quantityStr), stdin);
    t.quantity = intChecker(quantityStr);

    printf("Total Amount: "); fgets(totalStr, sizeof(totalStr), stdin);
    t.totalAmount = floatChecker(totalStr);

    FILE *file = fopen("transaction.txt", "a");
    if (file) {
        fprintf(file, "%s,%s,%s,%s,%s,%d,%.2f\n",
                t.transactionID, t.adminID, t.productID, t.supplierID, t.date,
                t.quantity, t.totalAmount);
        fclose(file);
        printf("Transaction added.\n");
        logTransaction(&t);
    } else {
        printf("Failed to open transaction file.\n");
    }
}

void viewTransaction() {
    char id[21];
    printf("Transaction ID to view: ");
    fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;

    FILE *file = fopen("transaction.txt", "r");
    if (!file) {
        printf("No transactions found.\n");
        return;
    }

    Transaction t;
    int found = 0;
    while (fscanf(file, "%20[^,],%20[^,],%20[^,],%20[^,],%10[^,],%d,%f\n",
                  t.transactionID, t.adminID, t.productID, t.supplierID,
                  t.date, &t.quantity, &t.totalAmount) == 7) {
        if (strcmp(t.transactionID, id) == 0) {
            printf("Transaction: %s\nAdmin: %s\nProduct: %s\nSupplier: %s\nDate: %s\nQty: %d\nTotal: %.2f\n",
                   t.transactionID, t.adminID, t.productID, t.supplierID, t.date, t.quantity, t.totalAmount);
            found = 1;
        }
    }
    if (!found) printf("Transaction not found.\n");
    fclose(file);
}

void updateTransaction() {
    char id[21], field[21], newVal[51];
    printf("Transaction ID: "); fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;
    printf("Field to update (quantity/totalAmount/date): "); fgets(field, sizeof(field), stdin); field[strcspn(field, "\n")] = 0;
    printf("New Value: "); fgets(newVal, sizeof(newVal), stdin); newVal[strcspn(newVal, "\n")] = 0;

    FILE *file = fopen("transaction.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    Transaction t;
    int found = 0;

    if (!file || !temp) {
        printf("File error.\n");
        return;
    }

    while (fscanf(file, "%20[^,],%20[^,],%20[^,],%20[^,],%10[^,],%d,%f\n",
                  t.transactionID, t.adminID, t.productID, t.supplierID,
                  t.date, &t.quantity, &t.totalAmount) == 7) {
        if (strcmp(t.transactionID, id) == 0) {
            found = 1;
            if (strcmp(field, "quantity") == 0) {
                t.quantity = intChecker(newVal);
            } else if (strcmp(field, "totalAmount") == 0) {
                t.totalAmount = floatChecker(newVal);
            } else if (strcmp(field, "date") == 0) {
                const char *validDate = dateChecker(newVal);
                if (strncmp(validDate, "Error", 5) == 0) {
                    printf("%s\n", validDate);
                    fclose(file);
                    fclose(temp);
                    remove("temp.txt");
                    return;
                }
                strncpy(t.date, newVal, sizeof(t.date));
            } else {
                printf("Invalid field.\n");
                fclose(file);
                fclose(temp);
                remove("temp.txt");
                return;
            }
        }
        fprintf(temp, "%s,%s,%s,%s,%s,%d,%.2f\n",
                t.transactionID, t.adminID, t.productID, t.supplierID,
                t.date, t.quantity, t.totalAmount);
    }
    fclose(file);
    fclose(temp);
    remove("transaction.txt");
    rename("temp.txt", "transaction.txt");

    if (found) {
        printf("Transaction updated.\n");
    } else {
        printf("Transaction ID not found.\n");
    }
}

void deleteTransaction() {
    char id[21];
    printf("Transaction ID: "); fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;

    FILE *in = fopen("transaction.txt", "r");
    FILE *out = fopen("temp.txt", "w");
    Transaction t;
    int found = 0;

    if (!in || !out) {
        printf("File error.\n");
        return;
    }

    while (fscanf(in, "%20[^,],%20[^,],%20[^,],%20[^,],%10[^,],%d,%f\n",
                  t.transactionID, t.adminID, t.productID, t.supplierID,
                  t.date, &t.quantity, &t.totalAmount) == 7) {
        if (strcmp(t.transactionID, id) == 0) {
            found = 1;
            continue;
        }
        fprintf(out, "%s,%s,%s,%s,%s,%d,%.2f\n",
                t.transactionID, t.adminID, t.productID, t.supplierID,
                t.date, t.quantity, t.totalAmount);
    }

    fclose(in);
    fclose(out);
    remove("transaction.txt");
    rename("temp.txt", "transaction.txt");

    if (found) {
        printf("Transaction deleted.\n");
    } else {
        printf("Transaction ID not found.\n");
    }
}

void logTransaction(const Transaction *t) {
    FILE *log = fopen("transaction_log.txt", "a");
    if (log) {
        fprintf(log, "LOG: %s | Admin: %s | Product: %s | Qty: %d | Total: %.2f | Date: %s\n",
                t->transactionID, t->adminID, t->productID, t->quantity, t->totalAmount, t->date);
        fclose(log);
    }
}
