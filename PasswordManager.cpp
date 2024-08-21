#include <iostream>
#include <cstring> // For string manipulation
#include <fstream> // For file handling
#include <cstdlib>
#include <ctime>
#include <string>


using namespace std;

const int tableSize = 100; // Choose an appropriate table size

//MASTER PASSWORD FUNCTIONS

struct UserData
{
    string username;
    string password;
    string securityQuestion;
    string securityAnswer;
    bool isLocked;
};

class UserAuthentication
{
private:
    UserData* users[tableSize];

    int hashFunction(const string& key)
    {
        int hash = 0;
        for (char ch : key)
        {
            hash = (hash + ch) % tableSize;
        }
        return hash;
    }

void saveUserDataToFile(const UserData& user)
{
        ofstream file("userdata2.txt", ios::app);
        if (file.is_open())
        {
            file << user.username << " " << user.password << " " << user.securityQuestion << " " << user.securityAnswer << " " << user.isLocked << endl;
            file.close();
        } else
        {
            cout << "Unable to open file." << endl;
        }
    }
public:
    UserAuthentication()
    {
        // Initialize user data array to nullptr
        for (int i = 0; i < tableSize; ++i)
        {
            users[i] = nullptr;
        }
    }

    void signup(const string& username, const string& password, const string& securityQuestion, const string& securityAnswer)
    {
        int index = hashFunction(username);

        UserData* newUser = new UserData();
        newUser->username = username;
        newUser->password = password;
        newUser->securityQuestion = securityQuestion;
        newUser->securityAnswer = securityAnswer;
        newUser->isLocked = false;

        users[index] = newUser;

        saveUserDataToFile(*newUser);

    }

    void loadUserDataFromFile()
    {
        ifstream file("userdata2.txt");
        if (file.is_open())
            {
            string username, password, securityQuestion, securityAnswer;
            bool isLocked;

            while (file >> username >> password >> securityQuestion >> securityAnswer >> isLocked)
                {
                int index = hashFunction(username);
                UserData* user = new UserData();
                user->username = username;
                user->password = password;
                user->securityQuestion = securityQuestion;
                user->securityAnswer = securityAnswer;
                user->isLocked = isLocked;

                users[index] = user;
            }

            file.close();
        } else
        {
            cout << "Unable to open file." << endl;
        }
    }

    bool login(const string& username, const string& password)
    {
        int index = hashFunction(username);

        UserData* user = users[index];
        if (user != nullptr && user->username == username && !user->isLocked)
            {
            if (user->password == password)
            {
                return true; // Successful login
            } else {
                return false; // Incorrect password
            }
        }
        return false; // User not found or account locked
    }

    bool securityQuestionLogin(const string& username, const string& securityAnswer)
    {
        int index = hashFunction(username);

        UserData* user = users[index];
        if (user != nullptr && user->username == username && !user->isLocked)
            {
            if (user->securityAnswer == securityAnswer)
            {
                return true; // Correct security answer
            } else
            {
                return false; // Incorrect security answer
            }
        }
        return false; // User not found or account locked
    }

    void lockAccount(const string& username)
    {
        int index = hashFunction(username);

        UserData* user = users[index];
        if (user != nullptr && user->username == username)
            {
            user->isLocked = true;
            cout << "Account locked due to multiple login attempts." << endl;
        }
    }

    ~UserAuthentication()
    {
        for (int i = 0; i < tableSize; ++i)
            {
            if (users[i] != nullptr) {
                delete users[i]; // Free allocated memory for UserData
            }
        }
    }
};

//=========================================================================================================================
//PASSWORD HASHTABLE HOME PAGE FUNCTIONS

class PasswordEntry
{
public:
    string password;
    string username;
    string website;
    string category;
    PasswordEntry* next;
};

class HashTable
{
private:
    PasswordEntry* hashTable[tableSize];

    int hashFunction(const string& key)
    {
        int hash = 0;
        for (char ch : key)
        {
            hash = (hash + ch) % tableSize;
        }
        return hash;
    }

public:
    HashTable() {
        for (int i = 0; i < tableSize; ++i)
            {
            hashTable[i] = nullptr;
        }
    }

    void insertEntry(const string& password, const string& username, const string& website,
                     const string& category)
                     {
        int index = hashFunction(password);

        PasswordEntry* newEntry = new PasswordEntry();
        newEntry->password = password;
        newEntry->username = username;
        newEntry->website = website;
        newEntry->category = category;
        newEntry->next = nullptr;

        if (hashTable[index] == nullptr)
            {
            hashTable[index] = newEntry;
        } else {
            PasswordEntry* current = hashTable[index];
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newEntry;
        }

        // Store data into a file
        ofstream file("passwords.txt", ios::app);
        if (file.is_open())
            {
            file << password << " " << username << " " << website << " " << category << endl;
            file.close();
        } else
        {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    void viewAllPasswords()
    {
        ifstream file("passwords.txt");
        string password, username, website, category;

        if (file.is_open())
            {
            while (file >> password >> username >> website >> category)
            {
                cout << "Password: " << password << ", Username: " << username
                     << ", Website: " << website << ", Category: " << category << endl;
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }
    }

    void filterByCategory(const string& searchCategory)
    {
        ifstream file("passwords.txt");
        string password, username, website, category;

        bool found = false;
        if (file.is_open())
            {
            while (file >> password >> username >> website >> category)
            {
                if (category == searchCategory)
                {
                    found = true;
                    cout << "Password: " << password << ", Username: " << username
                         << ", Website: " << website << ", Category: " << category << endl;
                }
            }
            file.close();
        }
        else
            {
            cerr << "Unable to open file for reading." << endl;
        }

        if (!found)
            {
            cout << "No passwords found in the category: " << searchCategory << endl;
        }
    }
    void deleteEntry(const string& password)
    {
        ifstream file("passwords.txt");
        string tempPass, tempUsername, tempWebsite, tempCategory;
        ofstream tempFile("temp_passwords.txt");

        bool found = false;
        if (file.is_open() && tempFile.is_open())
        {
            while (file >> tempPass >> tempUsername >> tempWebsite >> tempCategory)
            {
                if (tempPass == password)
                {
                    found = true;
                }
                else
                {
                    tempFile << tempPass << " " << tempUsername << " " << tempWebsite << " " << tempCategory << endl;
                }
            }
            file.close();
            tempFile.close();

            remove("passwords.txt");
            rename("temp_passwords.txt", "passwords.txt");

            if (!found)
            {
                cout << "Password not found for Deleting." << endl;
            }
        }
        else
        {
            cerr << "Unable to open files for Deleting." << endl;
        }

    }
    void editEntry(const string& password, const string& newPass)
    {
        ifstream file("passwords.txt");
        string tempPass, tempUsername, tempWebsite, tempCategory;
        ofstream tempFile("temp_passwords.txt");

        bool found = false;
        if (file.is_open() && tempFile.is_open())
        {
            while (file >> tempPass >> tempUsername >> tempWebsite >> tempCategory)
            {
                if (tempPass == password)
                {
                    found = true;
                    tempFile << newPass << " " << tempUsername << " " << tempWebsite << " " << tempCategory << endl;
                }
                else
                {
                    tempFile << tempPass << " " << tempUsername << " " << tempWebsite << " " << tempCategory << endl;
                }
            }
            file.close();
            tempFile.close();

            remove("passwords.txt");
            rename("temp_passwords.txt", "passwords.txt");

            if (!found)
            {
                cout << "Password not found for editing." << endl;
            }
        }
        else
        {
            cerr << "Unable to open files for editing." << endl;
        }
    }
    string generateStrongPassword()
    {
        int length=16;
    string password;
    string uppercaseChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string lowercaseChars = "abcdefghijklmnopqrstuvwxyz";
    string digitChars = "0123456789";
    string specialChars = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    srand(static_cast<unsigned int>(time(nullptr)));

    password += uppercaseChars[rand() % uppercaseChars.length()];
    password += lowercaseChars[rand() % lowercaseChars.length()];
    password += digitChars[rand() % digitChars.length()];
    password += specialChars[rand() % specialChars.length()];

    int remainingLength = length - 4;
    for (int i = 0; i < remainingLength; ++i)
        {
        int category = rand() % 4;
        switch (category)
        {
            case 0:
                password += uppercaseChars[rand() % uppercaseChars.length()];
                break;
            case 1:
                password += lowercaseChars[rand() % lowercaseChars.length()];
                break;
            case 2:
                password += digitChars[rand() % digitChars.length()];
                break;
            case 3:
                password += specialChars[rand() % specialChars.length()];
                break;
        }
    }

    for (int i = 0; i < length; ++i) {
        int randomIndex = rand() % length;
        swap(password[i], password[randomIndex]);
    }

    return password;
}
bool checkDuplicacy(const string& password)
    {
        ifstream file("passwords.txt");
        string tempPass, tempUsername, tempWebsite, tempCategory;

        bool found = false;
        if (file.is_open())
        {
            while (file >> tempPass >> tempUsername >> tempWebsite >> tempCategory)
            {
                if (tempPass == password)
                {
                    found = true;
                }
            }
            file.close();

            if (!found)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            cerr << "Unable to open files for editing." << endl;
        }

    }

    bool isStrongPassword(const string& password)
    {
    // Minimum length check
    if (password.length() <= 12)
        {
        cout << "Password length should be more than 12 characters." << endl;
        return false;
    }

    // Complexity checks
    bool hasUpperCase = false, hasLowerCase = false, hasDigit = false, hasSpecialChar = false, isSequential = false;
    bool duplicate=checkDuplicacy(password);
    char prevChar = '\0';
    for (char ch : password)
        {
        if (isupper(ch))
        {
            hasUpperCase = true;
        } else if (islower(ch))
        {
            hasLowerCase = true;
        } else if (isdigit(ch))
        {
            hasDigit = true;
        } else if (ispunct(ch))
        {
            hasSpecialChar = true;
        }

        // Sequential character check
        if (prevChar != '\0' && (ch == prevChar + 1 || ch == prevChar - 1))
            {
            isSequential = true;
        }

        // Repeated character check
        if (ch == prevChar)
            {
            isSequential = true;
        }

        prevChar = ch;
    }

    if (!hasUpperCase)
        {
        cout << "Password should contain at least one uppercase letter." << endl;
        return false;
    }

    if (!hasLowerCase)
        {
        cout << "Password should contain at least one lowercase letter." << endl;
        return false;
    }

    if (!hasDigit)
        {
        cout << "Password should contain at least one digit." << endl;
        return false;
    }

    if (!hasSpecialChar)
        {
        cout << "Password should contain at least one special character." << endl;
        return false;
    }

    if (isSequential)
        {
        //cout << "Password should not contain sequences or repeated characters." << endl;
        return true;
    }

    if(duplicate)
        {
        cout<<"The same password already exists in the database. DO NOT repeat passwords."<<endl;
        return false;
    }

    // Password meets all criteria
    return true;
}
    void displayPasswordRules()
    {
        cout << "Rules for a Strong Password:" << endl;
        cout << "1. Length: Should be more than 12 characters." << endl;
        cout << "2. Complexity: Include a mix of:" << endl;
        cout << "   - Uppercase letters" << endl;
        cout << "   - Lowercase letters" << endl;
        cout << "   - Numbers" << endl;
        cout << "   - Special characters (such as !, @, #, $, %, etc.)" << endl;
        cout << "3. Avoid: " << endl;
        cout << "   - Duplicacy of passwords. " << endl;
    }


    ~HashTable()
    {
        for (int i = 0; i < tableSize; ++i)
            {
            PasswordEntry* current = hashTable[i];
            while (current != nullptr)
            {
                PasswordEntry* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};

//================================================================================================================================
//PASSWORD STACK

class Node
 {
public:
    string data;
    Node* next;

    Node(const string& value)
    {
        data = value;
        next = nullptr;
    }
};

class Stack
{
private:
    Node* top;

public:
    Stack()
    {
        top = nullptr;
    }

    void push(const string& value)
    {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        cout << "\"" << value << "\" pushed into the stack." << endl;
    }

    bool isEmpty()
    {
        return top == nullptr;
    }

    string pop()
    {
        if (isEmpty())
            {
            cout << "No passwords Present. Please Generate a Random password first." << endl;
            return ""; // Return an empty string indicating failure
        }
        string poppedValue = top->data;
        Node* temp = top;
        top = top->next;
        delete temp;
        return poppedValue;
    }

    void display()
    {
        if (isEmpty())
            {
            cout << "Stack is empty. No elements to display." << endl;
            return;
        }
        cout << "Elements in the stack: ";
        Node* current = top;
        while (current != nullptr)
        {
            cout << "\"" << current->data << "\" ";
            current = current->next;
        }
        cout << endl;
    }
};

//================================================================================================================================
//LOGIN SCREEN

int authentication_screen(int choice)
{
    UserAuthentication authSystem;
    authSystem.loadUserDataFromFile(); // Load user data from file


    // Predefined security questions
    string securityQuestions =
    {
        "What is your mother's maiden name?",
    };
    if(choice==1)
    {
    string username, password;
    int attempts=0;
    do
    {
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    bool loggedIn = authSystem.login(username, password);
    if (loggedIn)
    {
        cout << "Login successful! Proceed to homepage." << endl;
        return 1;
    } else
    {
        attempts++;
        cout<<"Incorrect credentials. try again."<<endl;
    }
    }while(attempts<3);
    if(attempts==3)
    {
        string securityAnswer;
            cout << "Incorrect password. Answer security question to proceed (" << securityQuestions<< "): ";
            cin >> securityAnswer;

            bool securityLogin = authSystem.securityQuestionLogin(username, securityAnswer);
            if (securityLogin)
                {
                cout << "Security question answered correctly! Proceed to homepage." << endl;
                return 1;
            } else
            {
                cout << "Incorrect security answer. ACCOUNT LOCKED" << endl;
                authSystem.lockAccount(username);

            }

    }
    }
    if(choice==2)
    {

    // Signup process
    string user1,password1,answer1;
    cout<<"Enter the USERNAME :";
    cin>>user1;
    cout<<"Enter the Password :";
    cin>>password1;
    cout<<"The security question is :"<<securityQuestions<<endl;
    cout<<"Enter the answer :";
    cin>>answer1;
    authSystem.signup(user1, password1, securityQuestions, answer1);
    cout<<"SIGNED UP SUCCESSFULLY.Proceeding to login page"<<endl;
    cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press

            system("cls");
            cout<<"LOGIN with MASTER PASSWORD"<<endl;

    string username, password;
    int attempts=0;
    do{
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    bool loggedIn = authSystem.login(username, password);
    if (loggedIn)
        {
        cout << "Login successful! Proceed to homepage." << endl;
        cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
        return 1;
    }
    else
        {
        attempts++;
        cout<<"Incorrect credentials. try again."<<endl;
    }
    }
    while(attempts<3);
    if(attempts==3)
    {
        string securityAnswer;
            cout << "Incorrect password. Answer security question to proceed (" << securityQuestions<< "): ";
            cin >> securityAnswer;

            bool securityLogin = authSystem.securityQuestionLogin(username, securityAnswer);
            if (securityLogin) {
                cout << "Security question answered correctly! Proceed to homepage." << endl;
                return 1;
            } else {
                cout << "Incorrect security answer. ACCOUNT LOCKED" << endl;
                authSystem.lockAccount(username);
                return 2;

            }

    }
    }
    return 0;
}


int homepage()
{
    int choice;
    while (true) {
        // Clear the screen for better presentation (assuming a clear screen command exists)
        // You may replace the "clear" command with the appropriate command for your system (e.g., "cls" for Windows)
        system("cls");
        cout << "==================================================" << endl;
    cout << "          Welcome to Your Password Manager         " << endl;
    cout << "==================================================" << endl;
    cout << "                   Main Menu                      " << endl;
    cout << "==================================================" << endl;
    cout << "1. Add New Password" << endl;
    cout << "2. View All Passwords" << endl;
    cout << "3. Edit Password" << endl;
    cout << "4. Filter/Search Passwords by Category" << endl;
    cout << "5. Delete a Password "<<endl;
    cout << "6. Generate Random Password" << endl;
    cout << "7. Exit" << endl;
    cout << "==================================================" << endl;
    cout << "Enter your choice: ";
        cin >> choice;
        // Handle the user's choice here (implementation similar to the previous code snippet)
        // ...

        // Pause after displaying the menu before clearing the screen to read user input
        /*cout << "Press Enter to continue...";
        cin.ignore(); // Clear the input buffer
        cin.get();    // Wait for Enter key press
*/
    return choice;
    }
}

int main()
{
    int choice,r;
    cout << "Welcome to the Password Manager!" << endl;
    cout << "1. Login\n2. Signup\nChoose an option: ";
    cin >> choice;
    if(choice==1 || choice==2)
    {
                system("cls");

        r=authentication_screen(choice);
    }
    else{
        cout<<"INVALID CHOICE: EXITING"<<endl;
        return 0;
    }

    HashTable passwordTable;
    Stack S;

    if(r==2)
    {
        cout<<"EXITING...."<<endl;
        return 0;
    }

    int choice2;
    while(true)
    {
        if(r==1)
        {
            choice2=homepage();
        }
        if(choice2==1)
        {
            string usnm,pwd,webnm,category;
            system("cls");
            cout<<"Enter the Username: ";
            cin>>usnm;
            cout<<"Press 1 TO USE THE RANDOMLY GENERATED PASSWORD\nPress any other key for MANUAL password entry. "<<endl;
            int choice3;
            cin>>choice3;
            if(choice3==1)
            {
                string temppwd=S.pop();
                if(temppwd=="")
                {
            cout << "Press Enter to continue...";
                    cin.ignore(); // Clear the input buffer
            cin.get();
                    continue;
                }
                else{
                    pwd=temppwd;
                }

            }
            else
            {
                int flag=0;
                while(true)
                {
                    passwordTable.displayPasswordRules();
                    cout<<"Enter the Password: ";
                    cin>>pwd;
                    if(passwordTable.isStrongPassword(pwd))
                    {
                        cout<<"STRONG PASSWORD."<<endl;
                        break;
                    }
                    else
                    {
                        cout<<"WEAK PASSWORD. PLEASE ENTER A STRONG PASSWORD(press 0). OR Press 1 to use randomly generated password"<<endl;
                        int choice4;
                        cin>>choice4;
                        if(choice4==1)
                        {
                            string temppwd=S.pop();
                            if(temppwd.empty())
                            {
                                cout<<"GOING TO Homepage"<<endl;
                                cout << "Press Enter to continue...";
                                cin.ignore(); // Clear the input buffer
                                cin.get();
                                flag=1;
                                break;
                            }
                            else
                            {
                                pwd=temppwd;
                                break;
                            }
                        }
                        system("cls");

                    }

                }
                if(flag==1)
                {
                    continue;
                }

            }
            cout<<"Enter the Name of Website: ";
            cin>>webnm;
            cout<<"Enter the Category: ";
            cin>>category;
            passwordTable.insertEntry(pwd,usnm,webnm,category);
            cout<<"SUCCESSFULLY INSERTED"<<endl;
                        passwordTable.viewAllPasswords();

            cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
            continue;
        }
        else if (choice2==2)
        {
            system("cls");
            passwordTable.viewAllPasswords();
            cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
            continue;
        }
        else if(choice2==3)
        {
            system("cls");
            passwordTable.viewAllPasswords();
            string oldpass,newpass;
            cout<<"Enter the password you want to edit: ";
            cin>>oldpass;
            cout<<"Press 1 TO USE THE RANDOMLY GENERATED PASSWORD\nPress any other key for MANUAL password entry. "<<endl;
            int choice3;
            cin>>choice3;
            if(choice3==1)
            {
                string temppwd=S.pop();
                if(temppwd.empty())
                {
            cout << "Press Enter to continue...";
                    cin.ignore(); // Clear the input buffer
            cin.get();
                    continue;
                }
                else{
                    newpass=temppwd;
                }

            }
            else
            {
                int flag=0;
                while(true)
                {
                    passwordTable.displayPasswordRules();
                    cout<<"Enter the NEW Password: ";
                    cin>>newpass;
                    if(passwordTable.isStrongPassword(newpass))
                    {
                        cout<<"STRONG PASSWORD."<<endl;
                        break;
                    }
                    else
                    {
                        cout<<"WEAK PASSWORD. PLEASE ENTER A STRONG PASSWORD(press 0). OR Press 1 to use randomly generated password"<<endl;
                        int choice4;
                        cin>>choice4;
                        if(choice4==1)
                        {
                            string temppwd=S.pop();
                            if(temppwd.empty())
                            {
                                cout<<"GOING TO Homepage"<<endl;
                                cout << "Press Enter to continue...";
                                cin.ignore(); // Clear the input buffer
                                cin.get();
                                flag=1;
                                break;
                            }
                            else
                            {
                                newpass=temppwd;
                                break;
                            }
                        }
                        system("cls");

                    }

                }
                if(flag==1)
                {
                    continue;
                }

            }
            passwordTable.editEntry(oldpass,newpass);
            cout<<"PASSWORD EDITED"<<endl;
            cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
            continue;
        }
        else if(choice2==4)
        {
            system("cls");
            string catg;
            cout<<"Enter the CATEGORY: ";
            cin>>catg;
            passwordTable.filterByCategory(catg);
            cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
            continue;
        }
        else if(choice2==5)
        {
            system("cls");
            passwordTable.viewAllPasswords();
            string delpass;
            cout<<"Enter the password you want to DELETE: ";
            cin>>delpass;
            passwordTable.deleteEntry(delpass);
            cout<<"PASSWORD DELETED"<<endl;
            cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
            continue;

        }
        else if(choice2==6)
        {
            string pw;
            pw=passwordTable.generateStrongPassword();
            cout<<"The password is :"<<pw<<endl;
            S.push(pw);
            cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
            continue;
        }
        else if(choice2==7)
        {
            cout<<"EXITING"<<endl;
            return 0;
        }
        else
        {
            cout<<"INVALID CHOICE ENTERED"<<endl;
            cout << "Press Enter to continue...";
            cin.ignore(); // Clear the input buffer
            cin.get();    // Wait for Enter key press
            continue;
        }
    }
}
