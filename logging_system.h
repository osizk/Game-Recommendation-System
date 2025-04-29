#ifndef LOGGING_SYSTEM_H
#define LOGGING_SYSTEM_H

// Function to log an event with associated user information
// event: A string describing the event (e.g., "User login")
// user: The username associated with the event
void logging_event(const char *event, const char *user);

void logging_user(const char *event, const char *user);

// Function to display the contents of the logging file
void display_logging(const char *date);

void display_user_logging(const char *user);

#endif // LOGGING_SYSTEM_H
