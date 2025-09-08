#include "app_time.h"


namespace AppTime {

Clock::Clock(u_int32_t seconds_per_day): seconds_per_day_(seconds_per_day) {
    start_time = std::time(nullptr);
}

u_int32_t Clock::get_day() const {
    return (std::time(nullptr) - start_time) / seconds_per_day_;
}

Date Clock::get_date() const {
    return Date(*this);
}

Date::Date(const Clock& clock) {
    day_ = clock.get_day();
}

Date::Date(uint32_t day): day_(day) {}

Date::Date(const Date& other) {
    day_ = other.day_;
}

Date Date::operator-(uint32_t days) const {
    return Date(day_ - days);
}

Date Date::operator+(uint32_t days) const {
    return Date(day_ + days);
}

void Date::operator+=(uint32_t days) {
    day_ += days;
}

bool Date::operator==(const Date& other) const {
    return day_ == other.day_;
}

bool Date::operator<(const Date& other) const {
    return day_ < other.day_;
}

bool Date::operator>(const Date& other) const {
    return day_ > other.day_;
}

bool Date::operator<=(const Date& other) const {
    return day_ <= other.day_;
}

bool Date::operator>=(const Date& other) const {
    return day_ >= other.day_;
}

bool Date::operator!=(const Date& other) const {
    return day_ != other.day_;
}

bool Date::operator==(const Clock& clock) const {
    return day_ == clock.get_day();
}

bool Date::operator<(const Clock& clock) const {
    return day_ < clock.get_day();
}

bool Date::operator>(const Clock& clock) const {
    return day_ > clock.get_day();
}

bool Date::operator<=(const Clock& clock) const {
    return day_ <= clock.get_day();
}

bool Date::operator>=(const Clock& clock) const {
    return day_ >= clock.get_day();
}

bool Date::operator!=(const Clock& clock) const {
    return day_ != clock.get_day();
}

uint32_t Date::get_day() const {
    return day_;
}

}