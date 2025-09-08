#include <iostream>

#include <thread>
#include <chrono>

#include "app_time.h"
#include "users.h"
#include "book.h"


int main() {

    std::cout << "Users.h test\n";

    User::Faculty userFaculty("nameFaculty", "emailFaculty", 1);
    std::cout << "user faculty created" << std::endl;
    std::cout << userFaculty.get_name() << std::endl;
    std::cout << userFaculty.get_email() << std::endl;
    std::cout << userFaculty.get_max_books() << std::endl;
    std::cout << userFaculty.get_user_id() << std::endl;

    User::Student userStudent("nameStudent", "emailStudent", 2);
    std::cout << "user student created" << std::endl;
    std::cout << userStudent.get_name() << std::endl;
    std::cout << userStudent.get_email() << std::endl;
    std::cout << userStudent.get_max_books() << std::endl;
    std::cout << userStudent.get_user_id() << std::endl;


    User::Guest userGuest("nameGuest", "emailGuest", 3);
    std::cout << "user guest created" << std::endl;
    std::cout << userGuest.get_name() << std::endl;
    std::cout << userGuest.get_email() << std::endl;
    std::cout << userGuest.get_max_books() << std::endl;
    std::cout << userGuest.get_user_id() << std::endl;

    std::cout << "App_time.h test\n"; 

    AppTime::Clock appClock(1);

    AppTime::Date date1(appClock);

    std::cout << date1.get_day() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    AppTime::Date date2(appClock);

    std::cout << date2.get_day() << std::endl;

    date1 += 2;

    std::cout << date1.get_day() << ' ' << date2.get_day() << std::endl;
    std::cout << (date1 == date2) << std::endl;

    std::cout << "Book.h test\n";

    std::string bookName = "Test Book";
    std::string bookAuthor = "Test Author";  
    std::string bookIsbn = "123-456-789";
    Book::Book book(bookName, bookAuthor, bookIsbn, 1);

    std::cout << "book created" << std::endl;
    std::cout << "Name: " << book.get_name() << std::endl;
    std::cout << "Author: " << book.get_author() << std::endl;
    std::cout << "ISBN: " << book.get_isbn() << std::endl;
    std::cout << "ID: " << book.get_book_id() << std::endl;
    std::cout << "Available: " << book.is_availabale() << std::endl;

    book.set_genre(Book::Fantasy);
    std::cout << "Genre: " << Book::genre_to_string(book.get_genre()) << std::endl;

    book.make_unavailable(AppTime::Date(appClock), userStudent.get_user_id());
    std::cout << "Available after make_unavailable: " << book.is_availabale() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "exeed after 0.5 second: " << book.is_exeed(appClock.get_date()) << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    std::cout << "exeed after 2 seconds: " << book.is_exeed(appClock.get_date()) << std::endl;

    book.make_available();
    std::cout << "Available after make_available: " << book.is_availabale() << std::endl;



    


    
    return 0;
}

