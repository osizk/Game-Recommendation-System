# Game Store Management System

This is a **C-based console application** that simulates a game store management system. It supports both **Admin** and **Customer** functionalities, including game management, cart and checkout system, and personalized game recommendations based on purchase history and related games.



## Project Description

This project consists of the following features:

### Admin Features:
- Login with credentials (`admin.csv`)
- Add/Edit/Delete games
- Create relations between games (stored in `relations.csv`)
- View system activity logs by date (`Logging/logging_YYYY-MM-DD.csv`)

### Customer Features:
- Login with credentials (`users.csv`)
- Search for games
- Add to/Delete from cart
- View cart and checkout
- View purchase history
- Get personalized game recommendations using **BFS** from previously bought games and genre matching



## How to Run the Code

### 1. Compile all files (assuming GCC):
```bash
gcc main.c game.c login.c admin_interface.c customer_interface.c logging_system.c -o run
```

### 2. Make sure required folders/files exist:
- `games.csv` – contains game list
- `relations.csv` – game relations
- `users.csv` – user credentials
- `admin.csv` – admin credentials
- Create a folder named `Logging` and `UserHistory` in the working directory.

### 3. Run:
```bash
./run
```



## Sample Input and Output

### Admin Login
```
Enter admin username: admin
Enter admin password: admin123
```
**Output:**
```
+-----------------------------+
|         Admin Menu          |
+-----------------------------+
| [1] Add Game                |
| [2] Edit Game               |
| [3] Delete Game             |
| [4] Watch Logging           |
| [5] Add relation            |
| [6] Back to Main Menu       |
+-----------------------------+
```



### Customer Menu Options

**Sample Input:**
```
[2] Customer Login
Username: user
Password: user123
```

**Output:**
```
+-----------------------------+
|       Customer Menu         |
+-----------------------------+
| [1] Search Games            |
| [2] Add to Cart             |
| [3] Delete from Cart        |
| [4] View Cart               |
| [5] Checkout                |
| [6] View Purchase History   |
| [7] Recommend Game          |
+-----------------------------+
```



### Sample Game Data (`games.csv`)
```csv
Name,Genre,Price
Elden Ring,Action RPG,59.99
Demon's Souls,Action RPG,49.99
FIFA 23,Sports,39.99
```



### Sample Relation (`relations.csv`)
```csv
Elden Ring,Demon's Souls
FIFA 23,NBA 2K23
```



### Sample Log Output (`Logging/logging_2025-04-28.csv`)
```csv
Date,Time,Activity,User
2025-04-28,21:13:22,User user1 login,user1
2025-04-28,21:14:02,Purchased Game: Elden Ring ($59.99),user1
```
