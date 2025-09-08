#pragma once

#include <ctime>
#include <cstdint>


namespace AppTime {

class Date;

class Clock {
public:
    Clock(uint32_t seconds_per_day = 10);

    uint32_t get_day() const;

    Date get_date() const;

private:
    uint32_t seconds_per_day_;
    time_t start_time;

};


class Date {
public:
    Date(uint32_t day=0);

    Date(const Clock& clock);

    Date(const Date& other);

    // arythmetic operations

    Date operator+(uint32_t days) const;

    Date operator-(uint32_t days) const;

    void operator+=(uint32_t days);

    // comp with Date

    bool operator==(const Date& other) const;

    bool operator<(const Date& other) const;

    bool operator>(const Date& other) const;

    bool operator<=(const Date& other) const;

    bool operator>=(const Date& other) const;

    bool operator!=(const Date& other) const;

    // comp with Clock

    bool operator==(const Clock& clock) const;

    bool operator<(const Clock& clock) const;

    bool operator>(const Clock& clock) const;

    bool operator<=(const Clock& clock) const;

    bool operator>=(const Clock& clock) const;

    bool operator!=(const Clock& clock) const;

    uint32_t get_day() const;

private:
    uint32_t day_;
};

}


