#include <stdio.h>
#include <stdlib.h> // For exit()

#define ROWS 6
#define COLS 7

// Function to calculate the day of the week for a given date
int get_dow(int day, int month, int year) {
    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
        // Gregorian calendar
        if (month < 3) {
            month += 12;
            year -= 1;
        }
        int k = year % 100;
        int j = year / 100;
        int dow = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
        return (dow + 7) % 7; // Ensure positive result
    } else {
        // Julian calendar
        if (month < 3) {
            month += 12;
            year -= 1;
        }
        int dow = (day + 13 * (month + 1) / 5 + year + year / 4 + 5) % 7;
        return (dow + 7) % 7; // Ensure positive result
    }
}

// Function to generate the month's layout in a 2D array
void generate_month(int month, int year, int isLeapYear, const int daysInMonth[], int daysArray[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            daysArray[i][j] = 0; // Clear the array
        }
    }

    int days = (month == 2 && isLeapYear) ? 29 : daysInMonth[month - 1];
    int startDay = get_dow(1, month, year);

    if (year == 1582 && month == 10) {
        // Special case: October 1582
        int day = 1;
        for (int i = 0, j = startDay; day <= 4; j++) {
            if (j == COLS) {
                i++;
                j = 0;
            }
            daysArray[i][j] = day++;
        }
        for (int i = 0, j = (startDay + 10) % 7; day <= 31; j++) {
            if (j == COLS) {
                i++;
                j = 0;
            }
            daysArray[i][j] = day++;
        }
    } else {
        int day = 1;
        for (int i = 0, j = startDay; day <= days; j++) {
            if (j == COLS) {
                i++;
                j = 0;
            }
            daysArray[i][j] = day++;
        }
    }
}

// Function to print a single month's calendar
void print_month(const char *name, int daysArray[ROWS][COLS]) {
    printf("%-20s\n", name);
    printf("Su Mo Tu We Th Fr Sa\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (daysArray[i][j] == 0)
                printf("   ");
            else
                printf("%2d ", daysArray[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int year, month;
    printf("Welcome to the calendar.\n");
    printf("Enter year: ");
    if (scanf("%d", &year) != 1) {
        printf("Invalid input.\n");
        exit(1);
    }
    printf("Enter month (0 to print entire year): ");
    if (scanf("%d", &month) != 1) {
        printf("Invalid input.\n");
        exit(1);
    }

    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const char *monthNames[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    // Determine if the year is a leap year
    int isLeapYear;
    if (year > 1582 || (year == 1582 && month >= 10)) {
        isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    } else {
        isLeapYear = (year % 4 == 0);
    }

    // If a single month is requested
    if (month >= 1 && month <= 12) {
        int daysArray[ROWS][COLS];
        generate_month(month, year, isLeapYear, daysInMonth, daysArray);
        printf("\n  %s %d\n\n", monthNames[month - 1], year);
        print_month(monthNames[month - 1], daysArray);
    } else if (month == 0) {
        // Print the entire year in a 3x4 grid
        int daysArray[12][ROWS][COLS]; // Array for all months
        for (int m = 1; m <= 12; m++) {
            // Determine leap year per month context if around 1582
            int leap = (year > 1582 || (year == 1582 && m >= 10)) 
                       ? (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) 
                       : (year % 4 == 0);
            generate_month(m, year, leap, daysInMonth, daysArray[m - 1]);
        }

        printf("\n  Year: %d\n\n", year);
        for (int row = 0; row < 4; row++) {
            // Print month names for the row
            for (int col = 0; col < 3; col++) {
                int m = row * 3 + col;
                printf("%-20s  ", monthNames[m]);
            }
            printf("\n");

            // Print day headers for the row
            for (int col = 0; col < 3; col++) {
                printf("Su Mo Tu We Th Fr Sa  ");
            }
            printf("\n");

            // Print the rows of days for the three months
            for (int week = 0; week < ROWS; week++) {
                for (int col = 0; col < 3; col++) {
                    int m = row * 3 + col;
                    for (int d = 0; d < COLS; d++) {
                        if (daysArray[m][week][d] == 0)
                            printf("   ");
                        else
                            printf("%2d ", daysArray[m][week][d]);
                    }
                    printf(" ");
                }
                printf("\n");
            }
            printf("\n");
        }
    } else {
        printf("Invalid month entered.\n");
    }

    return 0;
}
