#include "users.h"

User::User::User(const std::string& name, const std::string& email, IdType user_id):
                 name_(name), email_(email), user_id_(user_id) {

};

User::IdType User::User::get_user_id() const {
    return user_id_;
}

const std::string& User::User::get_name() const {
    return name_;
}

const std::string& User::User::get_email() const {
    return email_;
}

void User::User::set_user_id(const IdType user_id) {
    user_id_ = user_id;
}

void User::User::set_user_name(const std::string& name) {
    name_ = name;
}

void User::User::set_user_email(const std::string& email) {
    email_ = email;
}

size_t User::User::get_borrowed_books_count() const {
    return borrowedBooks.size();
}

bool User::User::can_borrow() const {
    return this->get_max_books() > this->get_borrowed_books_count();
}

const std::unordered_set<Book::IdType>& User::User::get_borrowed_books() const {
    return borrowedBooks;
}

void User::User::borrow(Book::IdType) {
    if (!this->can_borrow()) {
        throw UserCannotBorrowError();
    } 

    if (borrowedBooks.contains(Book::IdType())) {
        throw BookAlreadyBorrowedError();
    }

    borrowedBooks.insert(Book::IdType());
}

void User::User::return_book(Book::IdType) {
    if (!borrowedBooks.contains(Book::IdType())) {
        throw BookNotBorrowedError();
    }
    borrowedBooks.erase(Book::IdType());
}

User::User::~User() = default;

User::Student::Student(const std::string& name, const std::string& email, IdType user_id):
        User(name, email, user_id) {};

size_t User::Student::get_max_books() const {
    return 3;
}

size_t User::Student::get_borrow_days() const {
    return 14;
}

User::Student::~Student() = default;

User::Faculty::Faculty(const std::string& name, const std::string& email, IdType user_id):
        User(name, email, user_id) {};

size_t User::Faculty::get_max_books() const {
    return 10;
}

size_t User::Faculty::get_borrow_days() const {
    return 30;
}

User::Faculty::~Faculty() = default;

User::Guest::Guest(const std::string& name, const std::string& email, IdType user_id):
        User(name, email, user_id) {};

size_t User::Guest::get_max_books() const {
    return 1;
}

size_t User::Guest::get_borrow_days() const {
    return 7;
}

User::Guest::~Guest() = default;