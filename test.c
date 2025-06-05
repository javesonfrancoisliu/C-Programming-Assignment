#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileHandling.h"
#include "FormatHandling.h"

int main() {
    // //test 1
    // //Admin
    // addDataAdmin("admin.txt", "A001", "John Doe", "123 Main St", "john.doe@example.com", "555-1234", "password123");
    // addDataAdmin("admin.txt", "A002", "Jane Smith", "456 Elm St", "jane.smith@example.com", "555-5678", "password456");
    // addDataAdmin("admin.txt", "A003", "Alice Johnson", "789 Oak St", "alice.johnson@example.com", "555-9012", "password789");
    // // Customer
    // addDataCustomer("customer.txt", "C001", "Bob Brown", "101 Pine St", "bob.brown@example.com", "555-3456");
    // addDataCustomer("customer.txt", "C002", "Charlie Green", "202 Maple St", "charlie.green@example.com", "555-7890");
    // addDataCustomer("customer.txt", "C003", "Diana White", "303 Birch St", "diana.white@example.com", "555-9012");
    // // Logistic
    // addDataLogistic("logistic.txt", "L001", "FedEx", "fedex@example.com", "555-1111");
    // addDataLogistic("logistic.txt", "L002", "UPS", "ups@example.com", "555-2222");
    // addDataLogistic("logistic.txt", "L003", "DHL", "dhl@example.com", "555-3333");
    // // Category
    // addDataCategory("category.txt", "CAT001", "Electronics", "Devices and gadgets");
    // addDataCategory("category.txt", "CAT002", "Clothing", "Apparel and accessories");
    // addDataCategory("category.txt", "CAT003", "Books", "Literature and educational materials");
    // // Inventory
    // addDataInventory("inventory.txt", "P001", "CAT001", "Smartphone", 699.99, 50, "Latest model smartphone");
    // addDataInventory("inventory.txt", "P002", "CAT002", "T-Shirt", 19.99, 200, "Cotton t-shirt");
    // addDataInventory("inventory.txt", "P003", "CAT003", "Novel", 14.99, 100, "Bestselling novel");
    // // Supplier
    // addDataSupplier("supplier.txt", "S001", "Tech Supplies", "123 Tech Ave", "techsupplies@example.com", "555-4444");
    // addDataSupplier("supplier.txt", "S002", "Office Depot", "456 Office Park", "officedepot@example.com", "555-5555");
    // addDataSupplier("supplier.txt", "S003", "Bookstore Inc.", "789 Book St", "bookstore@example.com", "555-6666");
    // // Transaction
    // addDataTransaction("transaction.txt", "T001", "A001", "P001", "S001", "2023-10-01", 10, 6999.90);
    // addDataTransaction("transaction.txt", "T002", "A002", "P002", "S002", "2023-10-02", 5, 99.95);
    // addDataTransaction("transaction.txt", "T003", "A003", "P003", "S003", "2023-10-03", 2, 29.98);
    // // Order
    // addDataOrder("order.txt", "O001", "A001", "C001", "P001", "L001", "2023-10-04", 1, 699.99);
    // addDataOrder("order.txt", "O002", "A002", "C002", "P002", "L002", "2023-10-05", 2, 39.98);
    // addDataOrder("order.txt", "O003", "A003", "C003", "P003", "L003", "2023-10-06", 3, 44.97);

    // //test 2
    // // Admin
    // readDataAdmin("admin.txt", "name", "A001");
    // // Customer
    // readDataCustomer("customer.txt", "name", "C001");
    // // Logistic
    // readDataLogistic("logistic.txt", "name", "L001");
    // // Category
    // readDataCategory("category.txt", "name", "CAT001");
    // // Inventory
    // // readDataInventory("inventory.txt", "name", "P001");
    // readDataInventory("inventory.txt", "price", "P002");
    // // Supplier
    // readDataSupplier("supplier.txt", "name", "S001");
    // // Transaction
    // readDataTransaction("transaction.txt", "quantity", "T001");
    // // Order
    // readDataOrder("order.txt", "quantity", "O001");

    // // test 3
    // // Admin
    // updateDataAdmin("admin.txt", "name", "A001", "John Doe Updated");
    // // Customer
    // updateDataCustomer("customer.txt", "name", "C001", "Bob Brown Updated");
    // // Logistic
    // updateDataLogistic("logistic.txt", "name", "L001", "FedEx Updated");
    // // Category
    // updateDataCategory("category.txt", "name", "CAT001", "Electronics Updated");
    // // Inventory
    // updateDataInventory("inventory.txt", "quantity", "P001", "500"); // Updated soon
    // updateDataInventory("inventory.txt", "quantity", "P002", "500"); // Updated soon
    // updateDataInventory("inventory.txt", "quantity", "P003", "500"); // Updated soon
    // // Supplier
    // updateDataSupplier("supplier.txt", "name", "S001", "Tech Supplies Updated");
    // // Transaction
    // updateDataTransaction("transaction.txt", "quantity", "T001", "15");  // Updated soon
    // // Order
    // updateDataOrder("order.txt", "quantity", "O001", "2");   // Updated soon

    // // test 4
    // // Admin
    // deleteDataAdmin("admin.txt", "A001");
    // // Customer
    // deleteDataCustomer("customer.txt", "C001");
    // // Logistic
    // deleteDataLogistic("logistic.txt", "L001");
    // // Category
    // deleteDataCategory("category.txt", "CAT001");
    // // // Inventory
    // deleteDataInventory("inventory.txt", "P001");
    // // // Supplier
    // deleteDataSupplier("supplier.txt", "S001");
    // // Transaction
    // deleteDataTransaction("transaction.txt", "T001");
    // // Order
    // deleteDataOrder("order.txt", "O001");   

    // // test intChecker

    // const char *testInt = "12345";
    // int result = intChecker(testInt);
    // printf("Result of intChecker for '%s': %d\n", testInt, result);
    // printf("%d\n", result + 10); // Example usage of the result
    
    // // test floatChecker

    // const char *testFloat = "123.45";
    // float resultFloat = floatChecker(testFloat);
    // printf("Result of floatChecker for '%s': %.2f\n", testFloat, resultFloat);
    // printf("%.2f\n", resultFloat + 10.0f); // Example usage of the result

    // // test dateChecker

    // const char *testDate = "01-01-2023";
    // const char *resultDate = dateChecker(testDate);
    // printf("Result of dateChecker for '%s': %s\n", testDate, resultDate);

    // checkID
    int number = checkID("inventory.txt");
    printf("Last identifier number from file: %d\n", number);

    // generateID

    char ID[MAX_ID_LENGTH];
    strcpy(ID, generateID("inventory.txt", "PROD0000"));
    printf("Generated ID: %s\n", ID);

    // checkIDExists
    int exists = checkIDExists("inventory.txt", "P001");
    if (exists) {
        printf("ID exists in the file.\n");
    } else {
        printf("ID does not exist in the file.\n");
    }

    return 0;
}