#include "book.h"

#include <vector>
#include <map>
#include <algorithm>
#include <cctype>



namespace Book {



std::vector<std::string> genres_vector = {
    "not stated",
    "action and adventure",
    "anthology",
    "classic",
    "comics",
    "crime and detective",
    "drama",
    "fable",
    "fairy tale",
    "fan fiction",
    "fantasy",
    "historical fiction",
    "horror",
    "humor",
    "legend",
    "magical realism",
    "mystery",
    "mythology",
    "realistic fiction",
    "romance",
    "satire",
    "science fiction",
    "short story",
    "suspense thriller",
    "biography",
    "autobiography",
    "essay",
    "memoir",
    "narrative nonfiction",
    "periodicals",
    "reference books",
    "self help book",
    "speech",
    "textbook",
    "poetry"
};

std::map<std::string, Genres> genres_map = {
    {"not stated", Genres::NS},
    {"action and adventure", Genres::ActionAndAdventure},
    {"anthology", Genres::Anthology},
    {"classic", Genres::Classic},
    {"comics", Genres::Comics},
    {"crime and detective", Genres::CrimeAndDetective},
    {"drama", Genres::Drama},
    {"fable", Genres::Fable},
    {"fairy tale", Genres::FairyTale},
    {"fan fiction", Genres::FanFiction},
    {"fantasy", Genres::Fantasy},
    {"historical fiction", Genres::HistoricalFiction},
    {"horror", Genres::Horror},
    {"humor", Genres::Humor},
    {"legend", Genres::Legend},
    {"magical realism", Genres::MagicalRealism},
    {"mystery", Genres::Mystery},
    {"mythology", Genres::Mythology},
    {"realistic fiction", Genres::RealisticFiction},
    {"romance", Genres::Romance},
    {"satire", Genres::Satire},
    {"science fiction", Genres::ScienceFiction},
    {"short story", Genres::ShortStory},
    {"suspense thriller", Genres::SuspenseThriller},
    {"biography", Genres::Biography},
    {"autobiography", Genres::Autobiography},
    {"essay", Genres::Essay},
    {"memoir", Genres::Memoir},
    {"narrative nonfiction", Genres::NarrativeNonfiction},
    {"periodicals", Genres::Periodicals},
    {"reference books", Genres::ReferenceBooks},
    {"self help book", Genres::SelfHelpBook},
    {"speech", Genres::Speech},
    {"textbook", Genres::Textbook},
    {"poetry", Genres::Poetry}
};


std::string genre_to_string(Genres genre) {
    return genres_vector[static_cast<int>(genre)];
}

Genres string_to_genre(std::string& genre_string) {
    std::transform(genre_string.begin(), genre_string.end(), genre_string.begin(),
    [](unsigned char c){ return std::tolower(c); });

    auto it = genres_map.find(genre_string);

    if (it != genres_map.end()) {
        return it->second;
    } else {
        return Genres::NS;
    }
}

Book::Book(const std::string& name, const std::string& author, const std::string& isbn, IdType book_id, Genres genre): 
           name_(name), author_(author), isbn_(isbn), book_id_(book_id), genre_(genre) {}

bool Book::is_exeed(AppTime::Date date) const {
    return !is_available_ && date > exeed_date_;
}

AppTime::Date Book::get_exeed_date() const {
    return exeed_date_;
}

bool Book::is_available() const {
    return is_available_;
}

User::IdType Book::get_owner_id() const {
    return owner_id_;
}

const std::string& Book::get_name() const {
    return name_;
}

const std::string& Book::get_author() const {
    return author_;
}

const std::string& Book::get_isbn() const {
    return isbn_;
}

IdType Book::get_book_id() const {
    return book_id_;
}

Genres Book::get_genre() const {
    return genre_;
}

void Book::make_available() {
    is_available_ = true;
}

void Book::make_unavailable(AppTime::Date exeed_date, User::IdType owner_id) {
    if (!is_available_) {
        throw BookUnavailableError();
    }

    exeed_date_ = exeed_date;
    owner_id_ = owner_id;
    is_available_ = false;
}

void Book::set_genre(Genres genre) {
    genre_ = genre;
}

void Book::set_author(const std::string& author) {
    author_ = author;
}

void Book::set_name(const std::string& name) {
    name_ = name;
}

void Book::set_isbn(const std::string& isbn) {
    isbn_ = isbn;
}

void Book::set_book_id(IdType book_id) {
    book_id_ = book_id;
}






}

