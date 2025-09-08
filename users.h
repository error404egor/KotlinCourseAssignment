#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <memory>
#include <exception>
#include "types.h"
#include "book.h"
#include "app_time.h"



namespace User {

class UserCannotBorrowError : public std::exception {
public:
    const char* what() const noexcept override {
        return "User has reached maximum number of allowed borrowed books";
    }
};

class BookAlreadyBorrowedError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Book is already borrowed by this user";
    }
};

class BookNotBorrowedError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Book is not borrowed by this user and cannot be returned";
    }
};

class User {
public:
    // constructor

    User(const std::string& name, const std::string& email, IdType user_id);

    // getters

    IdType get_user_id() const;

    const std::string& get_name() const;

    const std::string& get_email() const;

    // setters

    void set_user_id(IdType user_id);

    void set_user_name(const std::string& name);

    void set_user_email(const std::string& email);

    // info methods defined in hairs

    virtual size_t get_max_books() const = 0;

    virtual size_t get_borrow_days() const = 0;

    // defined there

    size_t get_borrowed_books_count() const;

    bool can_borrow() const;

    const std::unordered_set<Book::IdType>& get_borrowed_books() const;

    // borrow

    void borrow(Book::IdType);

    void return_book(Book::IdType);

    // destructor

    virtual ~User();


private:
    std::string name_, email_;
    IdType user_id_;

    std::unordered_set<Book::IdType> borrowedBooks;
};

class Student final: virtual public User {
public:
    Student(const std::string& name, const std::string& email, IdType user_id);

    size_t get_max_books() const override;

    size_t get_borrow_days() const override;

    ~Student() override;
};

class Faculty final: virtual public User {
public:
    Faculty(const std::string& name, const std::string& email, IdType user_id);

    size_t get_max_books() const override;

    size_t get_borrow_days() const override;

    ~Faculty() override;
};

class Guest final: virtual public User {
public:
    Guest(const std::string& name, const std::string& email, IdType user_id);

    size_t get_max_books() const override;

    size_t get_borrow_days() const override;

    ~Guest() override;
};


}





