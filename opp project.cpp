#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
using namespace std;

// ================= USER =================
class User {
protected:
    int userId;
    string name, email, password;

public:
    User(int id, string n, string e, string p)
        : userId(id), name(n), email(e), password(p) {}

    virtual void displayProfile() const {
        cout << "User: " << name << " | Email: " << email << '\n';
    }

    virtual ~User() = default;
};

// ================= ADMIN =================
class Admin : public User {
public:
    Admin(int id, string n, string e, string p)
        : User(id, n, e, p) {}

    void displayProfile() const override {
        cout << "[Admin] ";
        User::displayProfile();
    }
};

// ================= CUSTOMER =================
class Customer : public User {
    string address;

public:
    Customer(int id, string n, string e, string p, string addr)
        : User(id, n, e, p), address(addr) {}

    void displayProfile() const override {
        cout << "[Customer] ";
        User::displayProfile();
        cout << "Address: " << address << '\n';
    }
};

// ================= CATEGORY =================
class Category {
    int categoryId;
    string categoryName;

public:
    Category(int id, const string& name)
        : categoryId(id), categoryName(name) {}

    string getName() const {
        return categoryName;
    }

    void display() const {
        cout << "Category: " << categoryName << '\n';
    }
};

// ================= SUPPLIER =================
class Supplier {
    int supplierId;
    string name, contact;

public:
    Supplier(int id, const string& n, const string& c)
        : supplierId(id), name(n), contact(c) {}

    void display() const {
        cout << "Supplier: " << name
             << " | Contact: " << contact << '\n';
    }
};

// ================= PRODUCT =================
class Product {
    int productId;
    string name;
    double price;
    int quantity;
    Category* category;
    Supplier* supplier;

public:
    Product(int id, const string& n, double p, int q,
            Category* c, Supplier* s)
        : productId(id), name(n), price(p), quantity(q),
          category(c), supplier(s) {}

    int getId() const { return productId; }
    double getPrice() const { return price; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }

    void display() const {
        cout << "ID: " << productId
             << " | Name: " << name
             << " | Price: " << fixed << setprecision(2) << price
             << " | Quantity: " << quantity
             << " | Category: " << category->getName() << '\n';
    }
};

// ================= INVENTORY =================
class Inventory {
    vector<Product> products;

public:
    void addProduct(const Product& p) {
        products.push_back(p);
    }

    Product* searchProduct(int id) {
        for (auto& p : products) {
            if (p.getId() == id)
                return &p;
        }
        return nullptr;
    }

    void displayInventory() const {
        cout << "\n========== INVENTORY ==========\n";
        if (products.empty()) {
            cout << "No products in inventory.\n";
            return;
        }

        for (const auto& p : products)
            p.display();
    }

    void saveToFile() const {
        ofstream file("inventory.txt");

        if (!file) {
            cout << "Error: Could not open inventory.txt for writing.\n";
            return;
        }

        for (const auto& p : products) {
            // Quoted strings allow product names containing spaces.
            file << p.getId() << ' '
                 << quoted(p.getName()) << ' '
                 << p.getPrice() << ' '
                 << p.getQuantity() << '\n';
        }

        cout << "\nInventory saved to inventory.txt.\n";
    }

    void readFromFile() const {
        ifstream file("inventory.txt");

        if (!file) {
            cout << "\nNo saved inventory file was found.\n";
            return;
        }

        int id, qty;
        string name;
        double price;

        cout << "\n======= SAVED INVENTORY FILE =======\n";
        while (file >> id >> quoted(name) >> price >> qty) {
            cout << "ID: " << id
                 << " | Name: " << name
                 << " | Price: " << fixed << setprecision(2) << price
                 << " | Quantity: " << qty << '\n';
        }
    }
};

// ================= ORDER ITEM =================
class OrderItem {
    Product* product;
    int quantity;

public:
    OrderItem(Product* p, int q)
        : product(p), quantity(q) {}

    double calculateSubtotal() const {
        return product->getPrice() * quantity;
    }

    void display() const {
        cout << "Product: " << product->getName()
             << " | ID: " << product->getId()
             << " | Quantity: " << quantity
             << " | Subtotal: " << fixed << setprecision(2)
             << calculateSubtotal() << '\n';
    }
};

// ================= ORDER =================
class Order {
    int orderId;
    Customer* customer;
    vector<OrderItem> items;

public:
    Order(int id, Customer* c)
        : orderId(id), customer(c) {}

    void addItem(Product* p, int qty) {
        items.push_back(OrderItem(p, qty));
    }

    bool hasItems() const {
        return !items.empty();
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : items)
            total += item.calculateSubtotal();
        return total;
    }

    void displayOrder() const {
        cout << "\n========== ORDER " << orderId << " ==========\n";
        customer->displayProfile();

        if (items.empty()) {
            cout << "No items in this order.\n";
            return;
        }

        for (const auto& item : items)
            item.display();

        cout << "TOTAL: " << fixed << setprecision(2)
             << calculateTotal() << '\n';
    }
};

// ================= TRANSACTION =================
class Transaction {
    int transactionId;
    string type;
    double amount;

public:
    Transaction(int id, const string& t, double amt)
        : transactionId(id), type(t), amount(amt) {}

    void display() const {
        cout << "\n========== TRANSACTION ==========\n"
             << "Transaction ID: " << transactionId
             << "\nType: " << type
             << "\nAmount: " << fixed << setprecision(2) << amount << '\n';
    }
};

// ================= REPORT =================
class Report {
public:
    void generateInventoryReport(const Inventory& inv) const {
        cout << "\n========== INVENTORY REPORT ==========\n";
        inv.displayInventory();
    }

    void generateSalesReport(const Order& order) const {
        cout << "\n========== SALES REPORT ==========\n";
        order.displayOrder();
    }
};

// Read a complete line, including spaces.
string readText(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin >> ws, value);
    return value;
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value)
            return value;

        cout << "Please enter a valid whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0)
            return value;

        cout << "Please enter a valid number (zero or greater).\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ================= MAIN =================
int main() {
    cout << "========================================\n";
    cout << "       INVENTORY MANAGEMENT SYSTEM\n";
    cout << "========================================\n";

    // Admin information
    cout << "\n--- Enter Admin Information ---\n";
    int adminId = readInt("Admin ID: ");
    string adminName = readText("Admin name: ");
    string adminEmail = readText("Admin email: ");
    string adminPassword = readText("Admin password: ");
    Admin admin(adminId, adminName, adminEmail, adminPassword);

    // Category and supplier information
    cout << "\n--- Enter Category Information ---\n";
    int categoryId = readInt("Category ID: ");
    string categoryName = readText("Category name: ");
    Category category(categoryId, categoryName);

    cout << "\n--- Enter Supplier Information ---\n";
    int supplierId = readInt("Supplier ID: ");
    string supplierName = readText("Supplier name: ");
    string supplierContact = readText("Supplier contact: ");
    Supplier supplier(supplierId, supplierName, supplierContact);

    // Products
    Inventory inventory;
    cout << "\n--- Add Products ---\n";
    int productCount = readInt("How many products do you want to add? ");

    while (productCount < 1) {
        cout << "Please add at least one product.\n";
        productCount = readInt("Number of products: ");
    }

    for (int i = 0; i < productCount; i++) {
        cout << "\nProduct " << i + 1 << ":\n";
        int id = readInt("Product ID: ");
        string name = readText("Product name: ");
        double price = readDouble("Product price: ");
        int quantity = readInt("Available quantity: ");

        while (quantity < 0) {
            cout << "Quantity cannot be negative.\n";
            quantity = readInt("Available quantity: ");
        }

        Product product(id, name, price, quantity, &category, &supplier);
        inventory.addProduct(product);
    }

    // Display admin, category, supplier and inventory
    cout << "\n--- Admin Profile ---\n";
    admin.displayProfile();

    cout << "\n--- Category ---\n";
    category.display();

    cout << "\n--- Supplier ---\n";
    supplier.display();

    inventory.displayInventory();

    // Save and read inventory
    inventory.saveToFile();
    inventory.readFromFile();

    // Customer information
    cout << "\n--- Enter Customer Information ---\n";
    int customerId = readInt("Customer ID: ");
    string customerName = readText("Customer name: ");
    string customerEmail = readText("Customer email: ");
    string customerPassword = readText("Customer password: ");
    string customerAddress = readText("Customer address: ");

    Customer customer(customerId, customerName, customerEmail,
                      customerPassword, customerAddress);

    // Create an order
    int orderId = readInt("\nEnter Order ID: ");
    Order order(orderId, &customer);

    cout << "\n--- Add Items to Order ---\n";
    cout << "Enter 0 as the product ID when you finish adding items.\n";

    while (true) {
        int productId = readInt("\nEnter product ID (0 to finish): ");

        if (productId == 0)
            break;

        Product* selectedProduct = inventory.searchProduct(productId);

        if (selectedProduct == nullptr) {
            cout << "Product not found. Please try again.\n";
            continue;
        }

        int orderQuantity = readInt("Enter order quantity: ");

        if (orderQuantity <= 0) {
            cout << "Quantity must be greater than zero.\n";
            continue;
        }

        if (orderQuantity > selectedProduct->getQuantity()) {
            cout << "Not enough stock. Available quantity: "
                 << selectedProduct->getQuantity() << '\n';
            continue;
        }

        order.addItem(selectedProduct, orderQuantity);
        cout << "Product added to order.\n";
    }

    // Display order and reports
    order.displayOrder();

    if (order.hasItems()) {
        Transaction transaction(1, "Sale", order.calculateTotal());
        transaction.display();
    }

    Report report;
    report.generateInventoryReport(inventory);
    report.generateSalesReport(order);

    cout << "\nThank you for using the Inventory Management System!\n";
    return 0;
}
