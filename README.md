# PASSWORD-MANAGER-PROJECT
Design and implement a Password Manager application in C++ that securely stores and manages user passwords for various accounts. The Password Manager should provide a secure environment for users to store their passwords, usernames, associated websites, and categories.
The Password Manager application aims to address the growing need for users to manage their numerous account credentials securely. The primary objective is to create a user-friendly, secure, and efficient system for storing, organizing, retrieving, and managing passwords. The application will offer functionalities including but not limited to:
1.  User Authentication: 
   - Users can log in using a master password or sign up for a new account.
   - After unsuccessful login attempts, users can use a security question to access their account.
2.  Password Management: 
   - Store passwords associated with websites, usernames, and categories.
   - Implement a secure data structure (like a hash table) for efficient password storage and retrieval.
3.  User Interface: 
   - Implement a user-friendly command-line or graphical user interface (GUI).
   - Provide options for users to add new passwords, view existing passwords, edit passwords, filter passwords based on categories, and generate random passwords.
4.  Data Storage: 
   - Use file handling to store user account information and passwords securely in a file.
   - Ensure data persistence between sessions to maintain user passwords and information.
5.  Password Strength Verification: 
   - Check the strength of passwords entered by users.
   - Implement rules for strong passwords (length, complexity, avoidance of common patterns).
