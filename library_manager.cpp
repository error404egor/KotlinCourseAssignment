#include "library_manager.h"
#include <tuple>
#include <iostream>



namespace library_manager {

template <class T>
IdGenerator<T>::IdGenerator() : last_unused_id_(0) {}

template <class T>
T IdGenerator<T>::generate() {
    if (!free_ids_.empty()) {
        T id = free_ids_.back();
        free_ids_.pop_back();
        return id;
    }
    return last_unused_id_++;
}

template <class T>
void IdGenerator<T>::free(T id) {
    free_ids_.push_back(id);
}

LibraryManager::LibraryManager(const AppTime::Clock& clock): clock_(clock) {}

Book::IdType LibraryManager::add_book(const std::string& name, const std::string& author,
                              const std::string& isbn, Book::Genres genre) {
    auto id = book_id_generator_.generate();

    books_.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(name, author, isbn, id, genre));

    books_by_isbn_[isbn].insert(id);
    books_by_title_[name].insert(id);
    books_by_author_[author].insert(id);
    books_by_genre_[genre].insert(id);

    return id;
}

void LibraryManager::remove_book(Book::IdType id) {
    if (!books_.contains(id)) {
        throw BookNotFound();
    }

    Book::Book& book = books_[id];

    if (!book.is_available()) {
        throw BookInUse();
    }

    try {
        books_by_isbn_.at(book.get_isbn()).erase(id);
        books_by_title_.at(book.get_name()).erase(id);
        books_by_author_.at(book.get_author()).erase(id);
        books_by_genre_.at(book.get_genre()).erase(id);
        books_.erase(id);
        book_id_generator_.free(id);
    } catch (std::exception) {
        throw LibraryManagerError();
    }
}

const Book::Book& LibraryManager::find_book(Book::IdType id) const {
    if (!books_.contains(id)) {
        throw BookNotFound();
    }    
    return books_.at(id);
}

const std::unordered_set<Book::IdType>& LibraryManager::find_books_by_isbn(const std::string& isbn) const {
    static const std::unordered_set<Book::IdType> empty_set;
    return (books_by_isbn_.contains(isbn) ? books_by_isbn_.at(isbn) : empty_set);
}

const std::unordered_set<Book::IdType>& LibraryManager::find_books_by_title(const std::string& title) const {
    static const std::unordered_set<Book::IdType> empty_set;
    return (books_by_title_.contains(title) ? books_by_title_.at(title) : empty_set);
}

const std::unordered_set<Book::IdType>& LibraryManager::find_books_by_author(const std::string& author) const {
    static const std::unordered_set<Book::IdType> empty_set;
    return (books_by_author_.contains(author) ? books_by_author_.at(author) : empty_set);
}

const std::unordered_set<Book::IdType>& LibraryManager::find_books_by_genre(Book::Genres genre) const {
    static const std::unordered_set<Book::IdType> empty_set;
    return (books_by_genre_.contains(genre) ? books_by_genre_.at(genre) : empty_set);
}

std::unordered_set<Book::IdType> LibraryManager::find_books(const std::string& request) const {
    std::unordered_set<Book::IdType> ans;
    auto by_isbn = this->find_books_by_isbn(request);
    auto by_title = this->find_books_by_title(request);
    auto by_author = this->find_books_by_author(request);
    
    std::string genre_request = request;
    Book::Genres genre = Book::string_to_genre(genre_request);
    if (genre != Book::Genres::NS || request == "not stated") {
        auto by_genre = this->find_books_by_genre(genre);
        ans.insert(by_genre.begin(), by_genre.end());
    }

    ans.insert(by_isbn.begin(), by_isbn.end());
    ans.insert(by_title.begin(), by_title.end());
    ans.insert(by_author.begin(), by_author.end());

    return std::move(ans);
}

template <class T> 
User::IdType LibraryManager::register_user(const std::string& name, const std::string& email) {
    auto id = user_id_generator_.generate();

    auto new_user = std::make_shared<T>(name, email, id);

    users_.emplace(id, new_user);

    return id;
}

void LibraryManager::remove_user(User::IdType id) {
    if (!users_.contains(id)) {
        throw UserNotFound();
    }

    std::shared_ptr<const User::User> user = users_.at(id);

    if (user->get_borrowed_books_count() > 0) {
        throw UserHasBooks();
    }

    users_.erase(id);
    user_id_generator_.free(id);
}

std::shared_ptr<const User::User> LibraryManager::find_user(User::IdType id) const {
    if (!users_.contains(id)) {
        throw UserNotFound();
    }

    return users_.at(id);
}

void LibraryManager::borrow_book(User::IdType user_id, Book::IdType book_id) {
    if (!users_.contains(user_id)) {
        throw UserNotFound();
    }
    if (!books_.contains(book_id)) {
        throw BookNotFound();
    }

    std::cout << book_id << ' ' << books_[book_id].is_available() << std::endl;
    if (!books_[book_id].is_available()) {
        throw BookInUse();
    }

    if (!users_[user_id]->can_borrow()) {
        throw TooManyBooksBorrowed();
    }

    books_[book_id].make_unavailable(clock_.get_date() + users_[user_id]->get_borrow_days(), user_id);

    users_[user_id]->borrow(book_id);

    records_.emplace_back(user_id, book_id, books_[book_id].get_exeed_date(), RecordType::Borrow);
    closing_times[books_[book_id].get_exeed_date()].emplace(book_id, &records_[records_.size() - 1]);
}

void LibraryManager::return_book(User::IdType user_id, Book::IdType book_id) {
    if (!users_.contains(user_id)) {
        throw UserNotFound();
    }
    if (!books_.contains(book_id)) {
        throw BookNotFound();
    }

    if (!users_[user_id]->get_borrowed_books().contains(book_id)) {
        throw BookNotBorrowedByUser();
    }


    records_.emplace_back(user_id, book_id, clock_.get_date(), RecordType::Return);

    closing_times[records_.back().date].erase(BookIdWithBorrowingRecord(book_id, nullptr));

    if (closing_times[records_.back().date].empty()) {
        closing_times.erase(records_.back().date);
    }

    books_[book_id].make_available();

    users_[user_id]->return_book(book_id);

}

std::vector<BorrowingRecord> LibraryManager::get_overdue_books() const {

    std::vector<BorrowingRecord> ans;

    auto last_it = closing_times.lower_bound(clock_.get_date()); 


    for (auto it = closing_times.begin(); it != last_it; ++it) {
        for (const auto& record: it->second) {
            ans.emplace_back(*record.record);
        }
    }
    


    return std::move(ans);
}

// Explicit template instantiations
template class IdGenerator<uint32_t>;
template User::IdType LibraryManager::register_user<User::Student>(const std::string&, const std::string&);
template User::IdType LibraryManager::register_user<User::Faculty>(const std::string&, const std::string&);
template User::IdType LibraryManager::register_user<User::Guest>(const std::string&, const std::string&);

}

