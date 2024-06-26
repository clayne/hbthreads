#pragma once
#include <cstdint>
#include <iostream>

//--------------------------------------------------
// DateTime to give semantics to time values
//--------------------------------------------------

namespace hbthreads {

struct DecomposedTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int nanos;
};

//! A wrapper around a 64-bit time in nanoseconds
//! If a date, it can be epoch but it can also be an interval.
struct DateTime {
    static constexpr uint32_t MAXDATES = 105000;
    static constexpr uint32_t MAXYEARS = MAXDATES / 365 + 1;
    static constexpr std::int64_t NANOS_IN_SECOND = 1'000'000'000LL;
    static constexpr std::int64_t MICROS_IN_SECOND = 1'000'000LL;
    static constexpr std::int64_t MILLIS_IN_SECOND = 1'000LL;
    static constexpr std::int64_t SECONDS_IN_DAY = 24LL * 60 * 60;
    static constexpr std::int64_t NANOS_IN_DAY = SECONDS_IN_DAY * NANOS_IN_SECOND;
    static constexpr std::int64_t NANOS_IN_MINUTE = 60 * NANOS_IN_SECOND;
    static constexpr std::int64_t NANOS_IN_HOUR = 3600 * NANOS_IN_SECOND;

    //! Zero it
    constexpr DateTime() : epochns(0) {
    }

    //! Returns the date part of the object
    constexpr DateTime date() const {
        return DateTime(epochns - (epochns % NANOS_IN_DAY));
    }

    //! Returns the time part of the object
    constexpr DateTime time() const {
        return DateTime(epochns % NANOS_IN_DAY);
    }

    //! Constructs a DateTime from hour, minute, second
    static DateTime fromTime(int hour, int minute = 0, int second = 0);

    //! Constructs a DateTime from year/month/day
    static DateTime fromDate(int year, int month = 1, int day = 1);

    //! Construct a DateTime interval from days
    static constexpr DateTime days(int64_t days) {
        return DateTime(days * NANOS_IN_DAY);
    }
    //! Construct a DateTime interval from hours
    static constexpr DateTime hours(int64_t hrs) {
        return DateTime::secs(hrs * 3600);
    }
    //! Construct a DateTime interval from minutes
    static constexpr DateTime minutes(int64_t mins) {
        return DateTime::secs(60 * mins);
    }
    //! Construct a Datetime from seconds
    static constexpr DateTime secs(int64_t s) {
        return DateTime(s * NANOS_IN_SECOND);
    }
    //! Construct a Datetime from milliseconds
    static constexpr DateTime msecs(int64_t ms) {
        return DateTime(ms * MICROS_IN_SECOND);
    }
    //! Construct a Datetime from microseconds
    static constexpr DateTime usecs(int64_t us) {
        return DateTime(us * MILLIS_IN_SECOND);
    }
    //! Construct a Datetime from nanoseconds
    static constexpr DateTime nsecs(int64_t ns) {
        return DateTime(ns);
    }
    //! Returns the number of entire milliseconds
    constexpr std::int64_t msecs() const {
        return epochns / 1000000LL;
    }
    //! Returns the number of entire microseconds
    constexpr std::int64_t usecs() const {
        return epochns / 1000LL;
    }
    //! Returns the number of nanoseconds
    constexpr std::int64_t nsecs() const {
        return epochns;
    }
    //! Returns the total number of seconds as double
    constexpr double total_seconds() const {
        return double(epochns) / NANOS_IN_SECOND;
    }
    //! Returns the number of seconds
    constexpr std::int64_t secs() const {
        return epochns / NANOS_IN_SECOND;
    }
    //! Return the total number of days in this object
    constexpr std::int64_t days() const {
        return epochns / NANOS_IN_DAY;
    }
    //! Return the total number of minutes in this object
    constexpr std::int64_t minutes() const {
        return epochns / NANOS_IN_MINUTE;
    }
    //! Return the total number of hours in this object
    constexpr std::int64_t hours() const {
        return epochns / NANOS_IN_HOUR;
    }
    //! Returns the nanoseconds part of the time
    constexpr std::int64_t nanos() const {
        return epochns % NANOS_IN_SECOND;
    }

    //! \defgroup Operators DateTime operator overloading
    //! @{

    //! Binary subtraction
    constexpr inline DateTime operator-(DateTime rhs) const {
        return DateTime(epochns - rhs.epochns);
    }
    //! Binary addition
    constexpr inline DateTime operator+(DateTime rhs) const {
        return DateTime(epochns + rhs.epochns);
    }
    //! Addition
    constexpr inline DateTime& operator+=(DateTime rhs) {
        epochns += rhs.epochns;
        return *this;
    }
    //! Addition
    constexpr inline DateTime& operator-=(DateTime rhs) {
        epochns -= rhs.epochns;
        return *this;
    }
    //! Unary negation
    constexpr inline DateTime operator-() const {
        return DateTime(-epochns);
    }
    //! Multiplication operator
    constexpr inline DateTime operator*(int64_t value) {
        return DateTime(epochns * value);
    }
    //! Multiplication operator
    friend constexpr inline DateTime operator*(int64_t value, DateTime time) {
        return DateTime(time.epochns * value);
    }
    //! Binary less
    constexpr inline bool operator<(DateTime rhs) const {
        return epochns < rhs.epochns;
    }
    //! Binary greater
    constexpr inline bool operator>(DateTime rhs) const {
        return epochns > rhs.epochns;
    }
    //! Binary less or equal
    constexpr inline bool operator<=(DateTime rhs) const {
        return epochns <= rhs.epochns;
    }
    //! Binary greater or equal
    constexpr inline bool operator>=(DateTime rhs) const {
        return epochns >= rhs.epochns;
    }
    //! Binary equal
    constexpr inline bool operator==(DateTime rhs) const {
        return epochns == rhs.epochns;
    }
    //! @}

    //! Returns a zero interval
    constexpr static DateTime zero() {
        return DateTime(0);
    }

    //! Rounds this time object
    DateTime round(DateTime interval) const;

    //! Rounds this time object
    template <int64_t INTERVAL_NSECS>
    constexpr DateTime round() const;

    //! Advance to a given time rounded by an interval
    //! Returns true if the time changed
    bool advance(DateTime time, DateTime interval);

    //! Advance to a given time rounded by an interval
    //! Returns true if the time changed
    template <int64_t INTERVAL_NSECS>
    constexpr bool advance(DateTime time);

    //! Returns the current time in epoch/UTC
    enum class ClockType : uint8_t { RealTime = 1, Monotonic = 2 };
    static DateTime now(ClockType clock = ClockType::RealTime);

    //! Decompose this DateTime object in its components
    void decompose(struct DecomposedTime& dectime) const;

    //! Returns the YYYYMMDD representation of this object
    int64_t YYYYMMDD() const;

    //! Prints the date in a char buffer, no zero is added at the end
    //! YYYYMMDD-HH:MM:SS.NNNNNNNNN
    //! Returns a pointer to the next character
    //! `buf` should hold at least 27 bytes
    char* print(char* buf) const;

    //! Prints the time part in a char buffer, , no zero is added at the end
    //! HH:MM:SS.NNNNNNNNN
    //! Returns a pointer to the next character
    //! `buf` should hold at least 18 bytes
    char* printTime(char* buf) const;

private:
    //! Force user to use the static methods otherwise they will
    //! use it wrong, believe me
    explicit constexpr DateTime(int64_t ns) : epochns(ns) {
    }
    //! All this boilerplate for me? Awwww
    int64_t epochns;
};  // namespace hbthreads

std::ostream& operator<<(std::ostream& out, const DateTime date);

template <int64_t INTERVAL_NSECS>
constexpr DateTime DateTime::round() const {
    int64_t rem = epochns % INTERVAL_NSECS;
    if (rem >= INTERVAL_NSECS / 2) {
        rem -= INTERVAL_NSECS;
    } else if (rem <= -INTERVAL_NSECS / 2) {
        rem += INTERVAL_NSECS;
    }
    return DateTime(rem);
}

template <int64_t INTERVAL_NSECS>
constexpr bool DateTime::advance(DateTime time) {
    if (time.epochns < epochns) {
        return false;
    }
    epochns += INTERVAL_NSECS;
    if (time.epochns >= epochns) {
        auto num_intervals = (time.epochns) / INTERVAL_NSECS + 1;
        epochns = num_intervals * INTERVAL_NSECS;
    }
    return true;
}
}  // namespace hbthreads
