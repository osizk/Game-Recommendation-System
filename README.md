Game Management System
This is a simple command-line application written in C for managing a list of games. It includes features for both administrative users (adding, editing, deleting games) and regular customers (viewing the game list). The system uses a hash table to store game data and logs user and admin login events.

Features
Admin Interface:

Secure admin login.

Add new game records (Name, Genre, Price).

Edit existing game records.

Delete game records.

View a formatted list of all games.

Customer Interface:

User login.

View a formatted list of all games.

Data Storage:

Game data is stored in memory using a hash table.

Initial game data is loaded from a CSV file (games.csv).

Admin and user credentials are read from CSV files (admins.csv and users.csv).

Logging:

Logs admin and user login events to a file (logging.txt).

Prerequisites
A C compiler (like GCC).

Standard C libraries (stdio.h, stdlib.h, string.h, ctype.h, time.h, strings.h).

File Structure
main.c: Contains the main application loop and handles the initial menu (Admin/Customer/Exit).

game.h: Header file for game-related data structures and function declarations (hash table operations, game management).

game.c: Implementation of game-related functions (hashing, adding, loading, finding, editing, deleting, printing games).

login.h: Header file for login function declarations.

login.c: Implementation of user and admin login functions, reading credentials from CSV files.

admin_interface.h: Header file for the admin menu function declaration.

admin_interface.c: Implementation of the admin menu, handling admin operations by calling game management functions.

customer_interface.h: Header file for the customer menu function declaration.

customer_interface.c: Implementation of the customer menu

logging_system.h: Header file for logging function declarations.

logging_system.c: Implementation of the logging functions (writing to logging.txt).

customer_interface.h: (Currently empty, can be used for future customer-specific features).

admins.csv: Stores admin credentials (username,password per line).

users.csv: Stores user credentials (username,password per line).

games.csv: Stores initial game data (Name,Genre,Price per line).

logging.txt: Stores login event logs (created automatically if it doesn't exist).

Setup and Compilation
Save all the .c and .h files in the same directory.

Create the necessary CSV files (admins.csv, users.csv, games.csv) in the same directory.

admins.csv example:

admin,password123
super_user,secure_pass

users.csv example:

user1,pass1
customer_A,my_pass

games.csv example:

Game A,Action,19.99
Game B,RPG,39.50
Game C,Puzzle,9.99

Open a terminal or command prompt in the directory where you saved the files.

Compile the program using a C compiler, including all .c files:

gcc main.c game.c admin_interface.c customer_interface.c login.c logging_system.c -o run

gcc: The GCC compiler.

main.c game.c admin_interface.c customer_interface.c login.c logging_system.c: The source files to compile.

-o run: Specifies the output executable name as run (you can change this).

Running the Program
After successful compilation, run the executable from the terminal:

./run

(On Windows, you might just need to type game_manager or game_manager.exe).

The program will start and display the main menu.

Usage
When you run the program, you will see the main menu:

--- Main Menu ---
[1] Admin
[2] Customer
[3] Exit
Enter your choice:

[1] Admin:

Prompts for admin username and password.

If login is successful, enters the Admin Menu.

The game list is displayed automatically upon entering the Admin Menu.

Admin Menu Options:

1. Add Game: Add a new game record.

2. Edit Game: Modify an existing game's genre and price by name.

3. Delete Game: Remove a game record by name.

5. Back to Main Menu: Return to the main menu.

[2] Customer:

Prompts for username and password.

If login is successful, displays the formatted game list.

Currently, there are no other customer-specific features implemented beyond viewing the list.

[3] Exit:

Exits the program.

Adding Users and Admins
To add new users or admins, manually edit the users.csv and admins.csv files respectively. Add a new line for each user/admin in the format username,password.

Viewing Logs
Login events are logged to the logging.txt file in the same directory as the executable. You can view this file using any text editor

Extending the System
Implement more features for the customer interface (e.g., searching for games, purchasing).

Add more fields to the game struct (e.g., release date, developer, rating).

Implement the relationship management features (e.g., using an adjacency list or matrix to represent relationships between games).

Add error handling for file operations (e.g., if CSV files are missing or malformed).

Improve input validation.

Implement saving game data back to games.csv upon exiting the program.
