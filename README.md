```markdown
# Game Management System

A simple command-line application written in C for managing a list of games. Supports both administrative users (adding, editing, deleting games) and regular customers (viewing the game list). Uses an in-memory hash table for game data, CSV files for persistence, and logs login events to a text file.

---

## Features

### Admin Interface
- **Secure login**  
- **Add new games** (Name, Genre, Price)  
- **Edit existing games** (modify genre & price by name)  
- **Delete games** by name  
- **View** a formatted list of all games  

### Customer Interface
- **User login**  
- **View** a formatted list of all games  

### Data Storage
- **In-memory hash table** for fast lookups  
- **Initial data loaded** from `games.csv`  
- **Credentials** loaded from `admins.csv` & `users.csv`  

### Logging
- Records admin and user login events in `logging.txt`

---

## Prerequisites

- A C compiler (e.g., GCC)  
- Standard C libraries:  
  - `<stdio.h>`  
  - `<stdlib.h>`  
  - `<string.h>`  
  - `<ctype.h>`  
  - `<time.h>`  
  - `<strings.h>`

---

## File Structure

```
.
├── main.c                  # Entry point and main menu
├── game.h                  # Game data structures & function declarations
├── game.c                  # Hashing & game management functions
├── login.h                 # Login function declarations
├── login.c                 # Admin/user login implementations
├── admin_interface.h       # Admin menu declaration
├── admin_interface.c       # Admin menu & operations
├── customer_interface.h    # Customer menu declaration
├── customer_interface.c    # Customer menu implementation
├── logging_system.h        # Logging function declarations
├── logging_system.c        # Logging implementation
├── admins.csv              # Admin credentials (username,password)
├── users.csv               # User credentials (username,password)
├── games.csv               # Initial game data (Name,Genre,Price)
└── logging.txt             # Auto-generated login event logs
```

---

## Setup & Compilation

1. **Save** all the `.c` and `.h` files in the same directory.  
2. **Create** the CSV files in that directory:

   - **admins.csv**  
     ```csv
     admin,password123
     super_user,secure_pass
     ```
   - **users.csv**  
     ```csv
     user1,pass1
     customer_A,my_pass
     ```
   - **games.csv**  
     ```csv
     Game A,Action,19.99
     Game B,RPG,39.50
     Game C,Puzzle,9.99
     ```

3. **Compile** using GCC:

   ```bash
   gcc main.c game.c admin_interface.c customer_interface.c login.c logging_system.c -o game_manager
   ```
   - `-o game_manager` specifies the output executable name (you can change it).

---

## Running the Program

```bash
./game_manager
```

_On Windows, run `game_manager.exe`._

You will see:

```
--- Main Menu ---
[1] Admin
[2] Customer
[3] Exit
Enter your choice:
```

---

## Usage

### [1] Admin
1. **Login** with admin credentials.  
2. On success, you enter the **Admin Menu**, where the current game list is displayed automatically.
3. Choose from:
   1. **Add Game** — Enter name, genre, price.  
   2. **Edit Game** — Modify genre & price by game name.  
   3. **Delete Game** — Remove by name.  
   4. **View All Games** — Re-display the formatted list.  
   5. **Back to Main Menu**  

### [2] Customer
1. **Login** with user credentials.  
2. On success, view the formatted list of all games.  

### [3] Exit
- Quit the application.

---

## Managing Users & Admins

- To **add** or **remove** admins/users, manually edit `admins.csv` or `users.csv`.  
- Each line: `username,password`

---

## Viewing Logs

- All login events are appended to `logging.txt`.  
- Open it with any text editor to review timestamps and usernames.

---

## Extending the System

- **Customer features**: search, purchase flow, ratings, etc.  
- **Additional game fields**: release date, developer, rating.  
- **Persistence**: save back to `games.csv` on exit.  
- **Error handling**: detect missing/malformed CSV files.  
- **Input validation**: ensure valid names, genres, prices.  
- **Relationships**: use adjacency lists/matrices for game recommendations.

---

## License

(Optional: add your project’s license here.)

```

Feel free to customize any section or add further details as your project evolves!
