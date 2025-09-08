#include <iostream>
#include <string>
#include <limits>
#include "library_manager.h"
#include "app_time.h"
#include "book.h"

class LibraryConsole {
private:

    AppTime::Clock clock;
    library_manager::LibraryManager library;

    int getIntInput(const std::string& prompt) {
        int choice;
        while (true) {
            std::cout << prompt;
            if (std::cin >> choice) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return choice;
            }
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string getStringInput(const std::string& prompt) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    void showMainMenu() {
        std::cout << "\n=== Library Management System ===            day " << clock.get_day() << '\n';
        std::cout << "1. Book Management\n";
        std::cout << "2. User Management\n";
        std::cout << "3. Borrowing Operations\n";
        std::cout << "4. Search Books\n";
        std::cout << "0. Exit\n";
    }

    void handleBookManagement() {
        std::cout << "\n--- Book Management ---\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Remove Book\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1: addBook(); break;
            case 2: removeBook(); break;
            case 0: return;
            default: std::cout << "Invalid choice\n";
        }
    }

    void addBook() {
        std::string title = getStringInput("Enter book title: ");
        std::string author = getStringInput("Enter author: ");
        std::string isbn = getStringInput("Enter ISBN: ");
        
        // Display available genres
        std::cout << "\nAvailable genres:\n";
        for (int i = 0; i < 35; ++i) {
            std::cout << i << ". " << Book::genre_to_string(static_cast<Book::Genres>(i)) << "\n";
        }
        
        int genreChoice = getIntInput("Enter genre number (0 for 'not stated'): ");
        Book::Genres genre = (genreChoice >= 0 && genreChoice < 35) ? 
                            static_cast<Book::Genres>(genreChoice) : Book::Genres::NS;
        
        try {
            auto bookId = library.add_book(title, author, isbn, genre);
            std::cout << "Book added successfully! ID: " << bookId << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void removeBook() {
        Book::IdType id = getIntInput("Enter book ID: ");
        try {
            library.remove_book(id);
            std::cout << "Book removed successfully!\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleUserManagement() {
        std::cout << "\n--- User Management ---\n";
        std::cout << "1. Register Student\n";
        std::cout << "2. Register Faculty\n";
        std::cout << "3. Register Guest\n";
        std::cout << "4. Remove User\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1: registerUser<User::Student>(); break;
            case 2: registerUser<User::Faculty>(); break;
            case 3: registerUser<User::Guest>(); break;
            case 4: removeUser(); break;
            case 0: return;
            default: std::cout << "Invalid choice\n";
        }
    }

    template<typename UserType>
    void registerUser() {
        std::string name = getStringInput("Enter name: ");
        std::string email = getStringInput("Enter email: ");
        
        try {
            auto userId = library.register_user<UserType>(name, email);
            std::cout << "User registered successfully! ID: " << userId << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void removeUser() {
        User::IdType id = getIntInput("Enter user ID: ");
        try {
            library.remove_user(id);
            std::cout << "User removed successfully!\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleBorrowing() {
        std::cout << "\n--- Borrowing Operations ---\n";
        std::cout << "1. Borrow Book\n";
        std::cout << "2. Return Book\n";
        std::cout << "3. Show Overdue Books\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1: borrowBook(); break;
            case 2: returnBook(); break;
            case 3: showOverdueBooks(); break;
            case 0: return;
            default: std::cout << "Invalid choice\n";
        }
    }

    void borrowBook() {
        User::IdType userId = getIntInput("Enter user ID: ");
        Book::IdType bookId = getIntInput("Enter book ID: ");
        
        try {
            library.borrow_book(userId, bookId);
            std::cout << "Book borrowed successfully!\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void returnBook() {
        User::IdType userId = getIntInput("Enter user ID: ");
        Book::IdType bookId = getIntInput("Enter book ID: ");
        
        try {
            library.return_book(userId, bookId);
            std::cout << "Book returned successfully!\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void searchBooks() {
        std::string query = getStringInput("Enter search query (title/author/ISBN/genre): ");
        
        try {
            auto bookIds = library.find_books(query);
            if (bookIds.empty()) {
                std::cout << "No books found.\n";
                return;
            }
            
            std::cout << "Found books:\n";
            for (const auto& id : bookIds) {
                const auto& book = library.find_book(id);
                std::cout << "ID: " << id << ", Title: " << book.get_name() 
                         << ", Author: " << book.get_author() << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void showOverdueBooks() {
        try {
            auto overdueRecords = library.get_overdue_books();
            if (overdueRecords.empty()) {
                std::cout << "No overdue books found.\n";
                return;
            }
            
            std::cout << "\n=== Overdue Books ===\n";
            for (const auto& record : overdueRecords) {
                const auto& book = library.find_book(record.book_id);
                auto user = library.find_user(record.user_id);
                
                std::cout << "Book ID: " << record.book_id 
                         << ", Title: " << book.get_name()
                         << ", User ID: " << record.user_id
                         << ", User: " << user->get_name()
                         << ", Borrowed on day " << record.date.get_day() << '\n';
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

public:
    LibraryConsole() : clock(), library(clock) {}

    void run() {
        std::cout << "Welcome to Library Management System!\n";
        
        while (true) {
            showMainMenu();
            int choice = getIntInput("Enter choice: ");
            
            switch (choice) {
                case 1: handleBookManagement(); break;
                case 2: handleUserManagement(); break;
                case 3: handleBorrowing(); break;
                case 4: searchBooks(); break;
                case 0: 
                    std::cout << "Goodbye!\n";
                    return;
                default: 
                    std::cout << "Invalid choice\n";
            }
        }
    }
};

int main() {
    LibraryConsole console;
    console.run();
    return 0;
}
