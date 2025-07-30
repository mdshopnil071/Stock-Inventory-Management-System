#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_LEN 50
#define MAX_USERS 100
#define MAX_PRODUCTS 100
#define MAX_DISPATCHES 500

// File names for data storage
#define USERS_FILE "users.txt"
#define PRODUCTS_FILE "products.txt"
#define DISPATCHES_FILE "dispatches.txt"

// Structure for user accounts (admin/staff)
struct User {
    char username[MAX_LEN];
    char password[MAX_LEN];
    char role[10]; // "admin" or "staff"
};

// Structure for products
struct Product {
    int id;
    char name[MAX_LEN];
    int quantity;
    float price;
};

// Structure for dispatch records
struct Dispatch {
    int productId;
    char productName[MAX_LEN];
    int quantityDispatched;
    char dispatchedBy[MAX_LEN];
    char date[MAX_LEN];
    char time[MAX_LEN];
};

// Global arrays to store data
struct User users[MAX_USERS];
struct Product products[MAX_PRODUCTS];
struct Dispatch dispatches[MAX_DISPATCHES];

// Global counters
int userCount = 0;
int productCount = 0;
int dispatchCount = 0;

// Function declarations
void loadAllData();
void saveAllData();
void getHiddenPassword(char *password);
void getCurrentDateTime(char *date, char *time);

// Main menu functions
void mainMenu();
void registerUser();
void loginUser();

// Admin functions
void adminMenu(char *username);
void addProduct();
void viewAllProducts();
void restockProduct();
void generateReports();

// Staff functions
void staffMenu(char *username);
void dispatchProduct(char *staffName);
void dispatchAnalytics();
void viewLowStock();

// File handling functions
void loadUsers();
void saveUsers();
void loadProducts();
void saveProducts();
void loadDispatches();
void saveDispatches();

// Utility functions
int findUserByUsername(char *username);
int findProductById(int id);
void clearScreen();

int main() {
    printf("=== <COURT ROOM Departmental Store/> ===\n\n");
    printf("Welcome to the Court Room Departmental Store Management System!\n");
    loadAllData();
    mainMenu();
    return 0;
}

// Main menu - entry point of the system
void mainMenu() {
    int choice;
    
    while (1) {
        printf("\n=== MAIN MENU ===\n");
        printf("1. Register New User\n");
        printf("2. Login\n");
        printf("3. Exit System\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                printf("Thank you for using the system!\n");
                saveAllData();
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Function to register a new user (admin or staff)
void registerUser() {
    struct User newUser;
    int roleChoice;
    
    printf("\n=== USER REGISTRATION ===\n");
    
    // Check if maximum users reached
    if (userCount >= MAX_USERS) {
        printf("Maximum user limit reached!\n");
        return;
    }
    
    printf("Enter username: ");
    scanf("%s", newUser.username);
    
    // Check if username already exists
    if (findUserByUsername(newUser.username) != -1) {
        printf("Username already exists! Please choose a different username.\n");
        return;
    }
    
    printf("Enter password: ");
    getHiddenPassword(newUser.password);
    
    printf("Select role:\n");
    printf("1. Admin\n");
    printf("2. Staff\n");
    printf("Enter choice: ");
    scanf("%d", &roleChoice);
    
    if (roleChoice == 1) {
        strcpy(newUser.role, "admin");
    } else if (roleChoice == 2) {
        strcpy(newUser.role, "staff");
    } else {
        printf("Invalid role selection!\n");
        return;
    }
    
    // Add user to array and save
    users[userCount] = newUser;
    userCount++;
    saveUsers();
    
    printf("Registration successful! You can now login.\n");
}

// Function to handle user login
void loginUser() {
    char username[MAX_LEN], password[MAX_LEN];
    int userIndex;
    
    printf("\n=== USER LOGIN ===\n");
    printf("Enter username: ");
    scanf("%s", username);
    
    printf("Enter password: ");
    getHiddenPassword(password);
    
    userIndex = findUserByUsername(username);
    
    if (userIndex == -1) {
        printf("Username not found!\n");
        return;
    }
    
    if (strcmp(users[userIndex].password, password) != 0) {
        printf("Invalid password!\n");
        return;
    }
    
    printf("Login successful! Welcome %s (%s)\n", username, users[userIndex].role);
    
    // Redirect to appropriate menu based on role
    if (strcmp(users[userIndex].role, "admin") == 0) {
        adminMenu(username);
    } else {
        staffMenu(username);
    }
}

// Admin menu with all admin features
void adminMenu(char *username) {
    int choice;
    
    while (1) {
        printf("\n=== ADMIN MENU ===\n");
        printf("Welcome, %s!\n", username);
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Restock Products\n");
        printf("4. Generate Reports\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                viewAllProducts();
                break;
            case 3:
                restockProduct();
                break;
            case 4:
                generateReports();
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Staff menu with staff features
void staffMenu(char *username) {
    int choice;
    
    while (1) {
        printf("\n=== STAFF MENU ===\n");
        printf("Welcome, %s!\n", username);
        printf("1. View All Products\n");
        printf("2. Dispatch Products\n");
        printf("3. Dispatch Analytics (Most, Least, Avg)\n");
        printf("4. View Low Stock Items\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                viewAllProducts();
                break;
            case 2:
                dispatchProduct(username);
                break;
            case 3:
                dispatchAnalytics();
                break;
            case 4:
                viewLowStock();
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Function to add a new product (Admin only)
void addProduct() {
    struct Product newProduct;
    
    printf("\n=== ADD NEW PRODUCT ===\n");
    
    if (productCount >= MAX_PRODUCTS) {
        printf("Maximum product limit reached!\n");
        return;
    }
    
    printf("Enter Product ID: ");
    scanf("%d", &newProduct.id);
    
    // Check if product ID already exists
    if (findProductById(newProduct.id) != -1) {
        printf("Product ID already exists! Please use a different ID.\n");
        return;
    }
    
    printf("Enter Product Name: ");
    scanf(" %[^\n]s", newProduct.name);
    
    printf("Enter Initial Quantity: ");
    scanf("%d", &newProduct.quantity);
    
    printf("Enter Price per unit: $");
    scanf("%f", &newProduct.price);
    
    products[productCount] = newProduct;
    productCount++;
    saveProducts();
    
    printf("Product added successfully!\n");
}

// Function to view all products
void viewAllProducts() {
    printf("\n=== ALL PRODUCTS ===\n");
    
    if (productCount == 0) {
        printf("No products available in inventory!\n");
        return;
    }
    
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Quantity", "Price");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < productCount; i++) {
        printf("%-5d %-20s %-10d $%-9.2f\n", 
               products[i].id, 
               products[i].name, 
               products[i].quantity, 
               products[i].price);
    }
}

// Function to restock products (Admin only)
void restockProduct() {
    int productId, additionalQuantity, productIndex;
    
    printf("\n=== RESTOCK PRODUCT ===\n");
    
    printf("Enter Product ID to restock: ");
    scanf("%d", &productId);
    
    productIndex = findProductById(productId);
    
    if (productIndex == -1) {
        printf("Product not found!\n");
        return;
    }
    
    printf("Current quantity of %s: %d\n", 
           products[productIndex].name, 
           products[productIndex].quantity);
    
    printf("Enter additional quantity to add: ");
    scanf("%d", &additionalQuantity);
    
    if (additionalQuantity <= 0) {
        printf("Invalid quantity!\n");
        return;
    }
    
    products[productIndex].quantity += additionalQuantity;
    saveProducts();
    
    printf("Product restocked successfully!\n");
    printf("New quantity of %s: %d\n", 
           products[productIndex].name, 
           products[productIndex].quantity);
}

// Function to dispatch products (Staff only)
void dispatchProduct(char *staffName) {
    int productId, dispatchQuantity, productIndex;
    struct Dispatch newDispatch;
    char date[MAX_LEN], timeStr[MAX_LEN];
    
    printf("\n=== DISPATCH PRODUCT ===\n");
    
    printf("Enter Product ID to dispatch: ");
    scanf("%d", &productId);
    
    productIndex = findProductById(productId);
    
    if (productIndex == -1) {
        printf("Product not found!\n");
        return;
    }
    
    printf("Available quantity of %s: %d\n", 
           products[productIndex].name, 
           products[productIndex].quantity);
    
    printf("Enter quantity to dispatch: ");
    scanf("%d", &dispatchQuantity);
    
    if (dispatchQuantity <= 0 || dispatchQuantity > products[productIndex].quantity) {
        printf("Invalid quantity! Cannot dispatch more than available stock.\n");
        return;
    }
    
    // Update product quantity
    products[productIndex].quantity -= dispatchQuantity;
    
    // Create dispatch record
    newDispatch.productId = productId;
    strcpy(newDispatch.productName, products[productIndex].name);
    newDispatch.quantityDispatched = dispatchQuantity;
    strcpy(newDispatch.dispatchedBy, staffName);
    
    getCurrentDateTime(date, timeStr);
    strcpy(newDispatch.date, date);
    strcpy(newDispatch.time, timeStr);
    
    dispatches[dispatchCount] = newDispatch;
    dispatchCount++;
    
    // Save data
    saveProducts();
    saveDispatches();
    
    printf("Product dispatched successfully!\n");
    printf("Remaining quantity of %s: %d\n", 
           products[productIndex].name, 
           products[productIndex].quantity);
    printf("Dispatched by: %s on %s at %s\n", staffName, date, timeStr);
}

// Function to view low stock items
void viewLowStock() {
    int lowStockLimit;
    int foundLowStock = 0;
    
    printf("\n=== LOW STOCK ITEMS ===\n");
    printf("Enter low stock threshold: ");
    scanf("%d", &lowStockLimit);
    
    printf("\nProducts with quantity <= %d:\n", lowStockLimit);
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Quantity", "Price");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < productCount; i++) {
        if (products[i].quantity <= lowStockLimit) {
            printf("%-5d %-20s %-10d $%-9.2f\n", 
                   products[i].id, 
                   products[i].name, 
                   products[i].quantity, 
                   products[i].price);
            foundLowStock = 1;
        }
    }
    
    if (!foundLowStock) {
        printf("No products found with low stock!\n");
    }
}

// Function to generate various reports (Admin only)
void generateReports() {
    int choice;
    
    printf("\n=== GENERATE REPORTS ===\n");
    printf("1. Inventory Summary\n");
    printf("2. Dispatch History\n");
    printf("3. Low Stock Report\n");
    printf("4. Back to Admin Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("\n=== INVENTORY SUMMARY ===\n");
            printf("Total Products: %d\n", productCount);
            viewAllProducts();
            break;
            
        case 2:
            printf("\n=== DISPATCH HISTORY ===\n");
            if (dispatchCount == 0) {
                printf("No dispatch records found!\n");
            } else {
                printf("%-5s %-20s %-8s %-15s %-12s %-10s\n", 
                       "ID", "Product", "Qty", "Dispatched By", "Date", "Time");
                printf("------------------------------------------------------------------------\n");
                for (int i = 0; i < dispatchCount; i++) {
                    printf("%-5d %-20s %-8d %-15s %-12s %-10s\n",
                           dispatches[i].productId,
                           dispatches[i].productName,
                           dispatches[i].quantityDispatched,
                           dispatches[i].dispatchedBy,
                           dispatches[i].date,
                           dispatches[i].time);
                }
            }
            break;
            
        case 3:
            viewLowStock();
            break;
            
        case 4:
            return;
            
        default:
            printf("Invalid choice!\n");
    }
}

// Function to get hidden password input
void getHiddenPassword(char *password) {
    int i = 0;
    char ch;
    
    while ((ch = getch()) != '\r' && i < MAX_LEN - 1) {
        if (ch == '\b') { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

// Function to get current date and time
void getCurrentDateTime(char *date, char *timeStr) {
    time_t now;
    struct tm *timeinfo;
    
    time(&now);
    timeinfo = localtime(&now);
    
    strftime(date, MAX_LEN, "%Y-%m-%d", timeinfo);
    strftime(timeStr, MAX_LEN, "%H:%M:%S", timeinfo);
}

// Utility function to find user by username
int findUserByUsername(char *username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Utility function to find product by ID
int findProductById(int id) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Function to load all data from files
void loadAllData() {
    loadUsers();
    loadProducts();
    loadDispatches();
}

// Function to save all data to files
void saveAllData() {
    saveUsers();
    saveProducts();
    saveDispatches();
}

// File handling functions for users
void loadUsers() {
    FILE *fp = fopen(USERS_FILE, "r");
    if (fp == NULL) {
        return; // File doesn't exist yet
    }
    
    userCount = 0;
    while (fscanf(fp, "%[^;];%[^;];%[^\n]\n", 
                  users[userCount].username, 
                  users[userCount].password, 
                  users[userCount].role) == 3) {
        userCount++;
        if (userCount >= MAX_USERS) break;
    }
    
    fclose(fp);
}

// Function for dispatch analytics (Staff feature)
void dispatchAnalytics() {
    printf("\n=== DISPATCH ANALYTICS ===\n");
    
    if (dispatchCount == 0) {
        printf("No dispatch records available for analysis!\n");
        return;
    }
    
    // Arrays to store analytics data for each product
    int productIds[MAX_PRODUCTS];
    char productNames[MAX_PRODUCTS][MAX_LEN];
    int totalDispatched[MAX_PRODUCTS];
    int dispatchFrequency[MAX_PRODUCTS];
    int uniqueProducts = 0;
    
    // Initialize arrays
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        productIds[i] = -1;
        strcpy(productNames[i], "");
        totalDispatched[i] = 0;
        dispatchFrequency[i] = 0;
    }
    
    // Process dispatch records to calculate analytics
    for (int i = 0; i < dispatchCount; i++) {
        int productIndex = -1;
        
        // Find if product already exists in our analytics arrays
        for (int j = 0; j < uniqueProducts; j++) {
            if (productIds[j] == dispatches[i].productId) {
                productIndex = j;
                break;
            }
        }
        
        // If product not found, add it as new
        if (productIndex == -1) {
            productIndex = uniqueProducts;
            productIds[productIndex] = dispatches[i].productId;
            strcpy(productNames[productIndex], dispatches[i].productName);
            uniqueProducts++;
        }
        
        // Update analytics data
        totalDispatched[productIndex] += dispatches[i].quantityDispatched;
        dispatchFrequency[productIndex]++;
    }
    
    if (uniqueProducts == 0) {
        printf("No products found in dispatch records!\n");
        return;
    }
    
    // Find Most Dispatched Product (by quantity)
    int mostDispatchedIndex = 0;
    for (int i = 1; i < uniqueProducts; i++) {
        if (totalDispatched[i] > totalDispatched[mostDispatchedIndex]) {
            mostDispatchedIndex = i;
        }
    }
    
    // Find Least Dispatched Product (by quantity)
    int leastDispatchedIndex = 0;
    for (int i = 1; i < uniqueProducts; i++) {
        if (totalDispatched[i] < totalDispatched[leastDispatchedIndex]) {
            leastDispatchedIndex = i;
        }
    }
    
    // Calculate Average Dispatch per Product
    int totalQuantityDispatched = 0;
    for (int i = 0; i < uniqueProducts; i++) {
        totalQuantityDispatched += totalDispatched[i];
    }
    float averageDispatch = (float)totalQuantityDispatched / uniqueProducts;
    
    // Display Analytics Results
    printf("\n=== DISPATCH ANALYTICS RESULTS ===\n");
    printf("Analysis based on %d dispatch records for %d unique products\n\n", 
           dispatchCount, uniqueProducts);
    
    printf(" MOST DISPATCHED PRODUCT:\n");
    printf("   Product: %s (ID: %d)\n", 
           productNames[mostDispatchedIndex], productIds[mostDispatchedIndex]);
    printf("   Total Quantity Dispatched: %d units\n", 
           totalDispatched[mostDispatchedIndex]);
    printf("   Number of Dispatches: %d times\n\n", 
           dispatchFrequency[mostDispatchedIndex]);
    
    printf(" LEAST DISPATCHED PRODUCT:\n");
    printf("   Product: %s (ID: %d)\n", 
           productNames[leastDispatchedIndex], productIds[leastDispatchedIndex]);
    printf("   Total Quantity Dispatched: %d units\n", 
           totalDispatched[leastDispatchedIndex]);
    printf("   Number of Dispatches: %d times\n\n", 
           dispatchFrequency[leastDispatchedIndex]);
    
    printf(" AVERAGE DISPATCH PER PRODUCT:\n");
    printf("   Average Quantity per Product: %.2f units\n", averageDispatch);
    printf("   Total Products with Dispatches: %d\n", uniqueProducts);
    printf("   Total Quantity Dispatched (All Products): %d units\n\n", 
           totalQuantityDispatched);
    
    // Show detailed breakdown
    printf("=== DETAILED BREAKDOWN ===\n");
    printf("%-5s %-20s %-12s %-10s\n", "ID", "Product Name", "Total Qty", "Dispatches");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < uniqueProducts; i++) {
        printf("%-5d %-20s %-12d %-10d\n", 
               productIds[i], 
               productNames[i], 
               totalDispatched[i], 
               dispatchFrequency[i]);
    }
}

void saveUsers() {
    FILE *fp = fopen(USERS_FILE, "w");
    if (fp == NULL) {
        printf("Error saving users!\n");
        return;
    }
    
    for (int i = 0; i < userCount; i++) {
        fprintf(fp, "%s;%s;%s\n", 
                users[i].username, 
                users[i].password, 
                users[i].role);
    }
    
    fclose(fp);
}

// File handling functions for products
void loadProducts() {
    FILE *fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) {
        return; // File doesn't exist yet
    }
    
    productCount = 0;
    while (fscanf(fp, "%d;%[^;];%d;%f\n", 
                  &products[productCount].id,
                  products[productCount].name,
                  &products[productCount].quantity,
                  &products[productCount].price) == 4) {
        productCount++;
        if (productCount >= MAX_PRODUCTS) break;
    }
    
    fclose(fp);
}

void saveProducts() {
    FILE *fp = fopen(PRODUCTS_FILE, "w");
    if (fp == NULL) {
        printf("Error saving products!\n");
        return;
    }
    
    for (int i = 0; i < productCount; i++) {
        fprintf(fp, "%d;%s;%d;%.2f\n", 
                products[i].id, 
                products[i].name, 
                products[i].quantity, 
                products[i].price);
    }
    
    fclose(fp);
}

// File handling functions for dispatch records
void loadDispatches() {
    FILE *fp = fopen(DISPATCHES_FILE, "r");
    if (fp == NULL) {
        return; // File doesn't exist yet
    }
    
    dispatchCount = 0;
    while (fscanf(fp, "%d;%[^;];%d;%[^;];%[^;];%[^\n]\n", 
                  &dispatches[dispatchCount].productId,
                  dispatches[dispatchCount].productName,
                  &dispatches[dispatchCount].quantityDispatched,
                  dispatches[dispatchCount].dispatchedBy,
                  dispatches[dispatchCount].date,
                  dispatches[dispatchCount].time) == 6) {
        dispatchCount++;
        if (dispatchCount >= MAX_DISPATCHES) break;
    }
    
    fclose(fp);
}

void saveDispatches() {
    FILE *fp = fopen(DISPATCHES_FILE, "w");
    if (fp == NULL) {
        printf("Error saving dispatch records!\n");
        return;
    }
    
    for (int i = 0; i < dispatchCount; i++) {
        fprintf(fp, "%d;%s;%d;%s;%s;%s\n", 
                dispatches[i].productId,
                dispatches[i].productName,
                dispatches[i].quantityDispatched,
                dispatches[i].dispatchedBy,
                dispatches[i].date,
                dispatches[i].time);
    }
    
    fclose(fp);
}
