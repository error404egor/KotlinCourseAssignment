#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <optional>
#include <ctime>
#include "app_time.h"
#include "types.h"


namespace Book {

enum Genres {
    NS,
    ActionAndAdventure,
    Anthology,
    Classic,
    Comics,
    CrimeAndDetective,
    Drama,
    Fable,
    FairyTale,
    FanFiction,
    Fantasy,
    HistoricalFiction,
    Horror,
    Humor,
    Legend,
    MagicalRealism,
    Mystery,
    Mythology,
    RealisticFiction,
    Romance,
    Satire,
    ScienceFiction,
    ShortStory,
    SuspenseThriller,
    Biography,
    Autobiography,
    Essay,
    Memoir,
    NarrativeNonfiction,
    Periodicals,
    ReferenceBooks,
    SelfHelpBook,
    Speech,
    Textbook,
    Poetry
};

std::string genre_to_string(Genres genre); 

Genres string_to_genre(std::string& genre_string);

class BookUnavailableError : public std::runtime_error {
public:
    BookUnavailableError() : std::runtime_error("Book is already unavailable") {}
};

class Book {
public:
    Book() = default;

    Book(const std::string& name, const std::string& author, const  std::string& isbn, IdType book_id, Genres genre=Genres::NS);

    Book(const Book& other) = default;

    // getters

    bool is_exeed(AppTime::Date date) const;

    AppTime::Date get_exeed_date() const;
    
    bool is_available() const; // returns is_available_

    User::IdType get_owner_id() const;

    const std::string& get_name() const;

    const std::string& get_author() const;

    const std::string& get_isbn() const;

    IdType get_book_id() const;

    Genres get_genre() const;

    // setters 

    void make_available();

    void make_unavailable(AppTime::Date exeed_date, User::IdType owner_id);

    void set_name(const std::string& name);   

    void set_author(const std::string& author);

    void set_isbn(const std::string& isbn);

    void set_genre(Genres genre);

    void set_book_id(IdType book_id);



private:
    std::string name_, author_, isbn_;
    Genres genre_ = Genres::NS;
    IdType book_id_ = 0;

    bool is_available_ = true;
    AppTime::Date exeed_date_;
    User::IdType owner_id_ = 0;

};

}