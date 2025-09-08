#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <memory>
#include <exception>
#include "book.h"
#include "users.h"
#include "app_time.h"




namespace library_manager {



enum RecordType {
    Borrow,
    Return
};

struct BorrowingRecord {
    User::IdType user_id;
    Book::IdType book_id;
    AppTime::Date date;
    RecordType type;
};

struct BookIdWithBorrowingRecord {
    Book::IdType book_id;
    BorrowingRecord* record;

    bool operator==(const BookIdWithBorrowingRecord& other) const {
        return book_id == other.book_id;
    }
};

}



namespace std {
    template<>
    struct hash<library_manager::BookIdWithBorrowingRecord> {
        size_t operator()(const library_manager::BookIdWithBorrowingRecord& record) const {
            return std::hash<Book::IdType>{}(record.book_id);
        }
    };
}


namespace library_manager {


// Id generator, T is uint like type
template <class T>
class IdGenerator {
public:
    IdGenerator();

    T generate();

    void free(T id);

private:
    T last_unused_id_;
    std::vector<T> free_ids_;
};

class LibraryManagerError: public std::exception {
public:
    LibraryManagerError(): std::exception() {};

    virtual const char* what() const noexcept override {
        return "library manager error";
    }
};

class UserNotFound: public LibraryManagerError {
public:
    UserNotFound(): LibraryManagerError() {}
    
    virtual const char* what() const noexcept override {
        return "user not found";
    }
};

class BookNotFound: public LibraryManagerError {
public:
    BookNotFound(): LibraryManagerError() {}
    
    virtual const char* what() const noexcept override {
        return "book not found";
    }
};

class UserHasBooks: public LibraryManagerError {
public:
    UserHasBooks(): LibraryManagerError() {}
    
    virtual const char* what() const noexcept override {
        return "user still has borrowed books";
    }
};

class BookInUse: public LibraryManagerError {
public:
    BookInUse(): LibraryManagerError() {}
    
    virtual const char* what() const noexcept override {
        return "book is currently in use";
    }
};

class TooManyBooksBorrowed: public LibraryManagerError {
public:
    TooManyBooksBorrowed(): LibraryManagerError() {}
    
    virtual const char* what() const noexcept override {
        return "user has borrowed too many books";
    }
};

class BookNotBorrowedByUser: public LibraryManagerError {
public:
    BookNotBorrowedByUser(): LibraryManagerError() {}
    
    virtual const char* what() const noexcept override {
        return "book is not borrowed by this user";
    }
};



class LibraryManager {
public:

    LibraryManager(const AppTime::Clock& clock);

    // book managment
    
    Book::IdType add_book(const std::string& name, const std::string& author, const std::string& isbn, Book::Genres genre=Book::Genres::NS);

    void remove_book(Book::IdType id);

    const Book::Book& find_book(Book::IdType) const;

    const std::unordered_set<Book::IdType>& find_books_by_isbn(const std::string& isbn) const;

    const std::unordered_set<Book::IdType>& find_books_by_author(const std::string& author) const;

    const std::unordered_set<Book::IdType>& find_books_by_title(const std::string& title) const;

    const std::unordered_set<Book::IdType>& find_books_by_genre(Book::Genres genre) const;

    std::unordered_set<Book::IdType> find_books(const std::string& request) const;

    // user managment
    
    template <class T>
    User::IdType register_user(const std::string& name, const std::string& email);

    void remove_user(User::IdType id);

    std::shared_ptr<const User::User> find_user(User::IdType id) const;

    // borrowing operations

    void borrow_book(User::IdType user_id, Book::IdType book_id);

    void return_book(User::IdType user_id, Book::IdType book_id);
    
    std::vector<BorrowingRecord> get_overdue_books() const;


private:
    // time management
    const AppTime::Clock& clock_;

    // id generators
    IdGenerator<User::IdType> user_id_generator_;
    IdGenerator<Book::IdType> book_id_generator_; 

    // records array
    std::vector<BorrowingRecord> records_;
    std::map<AppTime::Date, std::unordered_set<BookIdWithBorrowingRecord>> closing_times;

    // users hashtable
    std::unordered_map<User::IdType, std::shared_ptr<User::User>> users_;


    // books hashtable
    std::unordered_map<Book::IdType, Book::Book> books_;
    std::unordered_map<std::string, std::unordered_set<Book::IdType>> books_by_isbn_;
    std::unordered_map<std::string, std::unordered_set<Book::IdType>> books_by_title_;
    std::unordered_map<std::string, std::unordered_set<Book::IdType>> books_by_author_;
    std::unordered_map<Book::Genres, std::unordered_set<Book::IdType>> books_by_genre_;

};






}