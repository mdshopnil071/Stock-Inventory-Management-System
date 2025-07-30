# Court Room Departmental Store Management System

A simple **console-based inventory and dispatch management system** for a departmental store, implemented in C language.  
This project allows user registration (Admin/Staff), product management, stock control, dispatch records, and analytics with persistent file storage.

---

## Features

- **User Management**
  - Register new users with roles: Admin or Staff.
  - Secure password input with hidden characters.
  - Login authentication.

- **Admin Features**
  - Add new products.
  - View all products in inventory.
  - Restock existing products.
  - Generate various reports including inventory summary, dispatch history, and low stock alerts.

- **Staff Features**
  - View products.
  - Dispatch products with quantity validation.
  - View dispatch analytics: most dispatched, least dispatched, average dispatch per product.
  - View low stock products.

- **Data Persistence**
  - User, product, and dispatch data saved and loaded from text files.
  - Simple file format using semicolon (`;`) as delimiter.

---

## Project Structure

- **Source Files:**  
  - `main.c` — contains main logic, menus, and function implementations.
- **Data Files:**  
  - `users.txt` — stores user credentials and roles.  
  - `products.txt` — stores product details.  
  - `dispatches.txt` — stores dispatch records.

---

## How to Run
1. Compile `Source_Code.c` using a C compiler:
   ```
   gcc Source_Code.c -o store
   ```
2. Run the executable:
   ```
   ./store
   ```
   (On Windows, use `store.exe`)

---

## Author
Md. Khairul Anam Shopnil,
 Ibnul Jobayer,
 Kazi Adiba Rahman Adhora,
 Mohammad Siyam Hossen

---

## License
This project is for educational purposes.
   
