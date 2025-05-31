#ifndef FORMAT_HANDLING_H
#define FORMAT_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int intChecker(const char *integer_ptr) {
    if (integer_ptr == NULL || *integer_ptr == '\0') {  // Check for NULL or empty string
        return 0;
    }
    const char *original_ptr = integer_ptr; // Save the original pointer for conversion
    while (*integer_ptr) {
        if (!isdigit(*integer_ptr)) {   // Check if the character is a digit
            return 0;
        }
        integer_ptr++;
    }
    return atoi(original_ptr); // Convert to integer
}

float floatChecker(const char *float_ptr) {
    if (float_ptr == NULL || *float_ptr == '\0') {  // Check for NULL or empty string
        return 0.0f;
    }
    const char *original_ptr = float_ptr; // Save the original pointer for conversion
    int has_digits = 0;
    int has_dot = 0;
    while (*float_ptr) {    // Iterate through each character
        if (*float_ptr == '.') {    // Check for decimal point
            if (has_dot) {
                return 0.0f;
            }
            has_dot = 1;
        } else if (isdigit(*float_ptr)) {   // Check if the character is a digit
            has_digits = 1;
        } else {
            return 0.0f;
        }
        float_ptr++;
    }
    if (!has_digits) {      // If no digits were found, return 0.0f
        return 0.0f;
    }
    return atof(original_ptr);  // Convert to float
}

const char* dateChecker(const char *date_str) {
    if (date_str == NULL || strlen(date_str) != 10) {   // Check for NULL or incorrect length
        return "Error: Invalid date format";
    }
    for (int i = 0; i < 10; i++) {  // Check each character in the date string
        if (i == 2 || i == 5) {
            if (date_str[i] != '-') return "Error: Invalid date format";
        } 
        else {
            if (!isdigit(date_str[i])) {
                return "Error: Invalid date format";
            }
        }
    }
    int day = atoi(date_str);          // DD from index 0
    int month = atoi(date_str + 3);    // MM from index 3
    int year = atoi(date_str + 6);     // YYYY from index 6
    if (month < 1 || month > 12) {
        return "Error: Invalid month";
    }
    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) { // Check for leap year
        days_in_month[1] = 29;
    }
    if (day < 1 || day > days_in_month[month - 1]) {  // Check for valid day
        return "Error: Invalid day for given month";
    }
    return date_str;    // Return the original date
}

#endif