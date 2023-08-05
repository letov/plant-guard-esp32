#include <time.h>

static uint8_t locked_hours = 11;
static uint8_t locked_minutes = 12;
static uint8_t locked_seconds = 13;

static uint8_t locked_day = 1;
static uint8_t locked_month = 2;
static uint8_t locked_year = 23;

void get_date(uint8_t *day, uint8_t *month, uint8_t *year) {
    *day = locked_day;
    *month = locked_month;
    *year = locked_year;
}

void get_time(uint8_t *hours, uint8_t *minutes, uint8_t *seconds) {
    *hours = locked_hours;
    *minutes = locked_minutes;
    *seconds = locked_seconds;
}

uint64_t get_unix_timestamp() {
    uint8_t year, month, day, hours, minutes, seconds;
    get_date(&day, &month, &year);
    get_time(&hours, &minutes, &seconds);

    struct tm time_info;
    time_info.tm_year = year + 100;
    time_info.tm_mon = month - 1;
    time_info.tm_mday = day;
    time_info.tm_hour = hours;
    time_info.tm_min = minutes;
    time_info.tm_sec = seconds;
    time_info.tm_isdst = -1;

    time_t unix_time = mktime(&time_info);
    uint64_t unix_timestamp = (uint64_t) unix_time * 1000;
    return unix_timestamp;
}