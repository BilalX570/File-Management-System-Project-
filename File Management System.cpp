#include<iostream>
#include<string>
using namespace std;

struct FileNode {
    string filename;
    string content;
    size_t size;
    time_t createdDate;
    time_t lastModified;
    time_t lastSeenDate;
    
    FileNode* prev;
    FileNode* next;
  
};
// Doubly linked list for file management
struct FileList {

    FileNode* head;                                              
    FileNode* tail;
    int count;
    
    void swapNodesData(FileNode* a, FileNode* b) {
        swap(a->filename, b->filename);
        swap(a->content, b->content);
        swap(a->size, b->size);
        swap(a->type, b->type);
        swap(a->createdDate, b->createdDate);
        swap(a->lastModified, b->lastModified);
        swap(a->lastSeenDate, b->lastSeenDate);
    }


    FileList() : head(nullptr), tail(nullptr), count(0) {}
    
    ~FileList() {
        clear();
    }

    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }

    bool contains(const string& filename) const {
        FileNode* current = head;
        while (current) {
            if (current->filename == filename) return true;
            current = current->next;
        }
        return false;
    }

    void addFileAtBeginning(const string& filename, const string& content) {
        if (contains(filename)) {
            cout << "File '" << filename << "' already exists.\n";
            return;
        }

        FileNode* newNode = new FileNode(filename, content);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void addFileAtEnd(const string& filename, const string& content) {
        if (contains(filename)) {
            cout << "File '" << filename << "' already exists.\n";
            return;
        }

        FileNode* newNode = new FileNode(filename, content);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void addFileAtPosition(const string& filename, int position, const string& content) {
        if (position < 0 || position > count) {
            cout << "Invalid position.\n";
            return;
        }

        if (position == 0) {
            addFileAtBeginning(filename, content);
            return;
        }

        if (position == count) {
            addFileAtEnd(filename, content);
            return;
        }

        FileNode* newNode = new FileNode(filename, content);
        FileNode* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
    
        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;
        count++;
    }







void displayMainMenu() {
    cout << "----------------------------------------\n";
    cout << "\nFile Manager Menu\n";
    cout << "1. Create File/Directory\n";
    cout << "2. Delete File/Directory\n";
    cout << "3. List Files\n";
    cout << "4. Search File\n";
    cout << "5. Sort Files\n";
    cout << "6. File Operations\n";
    cout << "7. Advanced Search\n";
    cout << "8. Delete All Files\n";
    cout << "9. View Directory Contents\n";
    cout << "10. Recycle Bin\n";
    cout << "11. Memory Status\n";
    cout << "12. Exit\n";
    cout << "----------------------------------------\n";
    cout << "Enter your choice: ";
    
}

void displayFileOperationsMenu() {
    cout << "----------------------------------------\n";
    cout << "\nFile Operations Menu\n";
    cout << "1. Read File\n";
    cout << "2. Update File (Append)\n";
    cout << "3. Overwrite File\n";
    cout << "4. Display File Content\n";
    cout << "5. Rename File\n";
    cout << "6. View File Statistics\n";
    cout << "7. Update File Metadata\n";
    cout << "0. Back to Main Menu\n";
    cout << "----------------------------------------\n";
    cout << "Enter your choice: ";
    
}

void displayAdvancedSearchMenu() {
    cout << "----------------------------------------\n";
    cout << "\nAdvanced Search Menu\n";
    cout << "1. Search by Content\n";
    cout << "2. Search by Type\n";
    cout << "3. Search by Size Range\n";
    cout << "4. Search by Prefix\n";
    cout << "0. Back to Main Menu\n";
    cout << "----------------------------------------\n";
    cout << "Enter your choice: ";
}

void displaySortMenu() {
    cout << "----------------------------------------\n";
    cout << "\nSort Files By\n";
    cout << "1. Name\n";
    cout << "2. Size\n";
    cout << "3. Modification Date\n";
    cout << "0. Back to Main Menu\n";
    cout << "----------------------------------------\n";
    cout << "Enter your choice: ";
}
int main() {

   
    
    while (true) {
        displayMainMenu();
        int choice;
        cin >> choice;
        
        
        if (choice == 12) {
            cout << "Exiting program...\n";
            break;
        }
        
        string filename, newName, content;
        int position;
        
        switch (choice) {
            case 1: { // Create File/Directory
                cout << "1. Create File\n";
                cout << "2. Create Directory\n";
                cout << "Enter choice: ";
                int createChoice;
                cin >> createChoice;
                cin.ignore();
                
                cout << "Enter name: ";
                getline(cin, filename);
                cout << "  To create at the last, enter (-1)\n  To create at the  first, enter (0)\n  Create at specific index\nEnter position: ";
                cin >> position;
                cin.ignore();
                
                if (createChoice == 1) {
                       
                } else if (createChoice == 2) {
                    
                } else {
                cout << "|-----------------------------------|\n";
                cout << "| Invalid choice.                   |\n";
                cout << "|-----------------------------------|\n";
                }
                break;
            }
            case 2: { // Delete File/Directory
                cout << "1. Delete by position\n";
                cout << "2. Delete by name\n";
                cout << "Enter choice: ";
                int deleteChoice;
                cin >> deleteChoice;
                cin.ignore();
                
                if (deleteChoice == 1) {
                    cout << "  To delete from last, enter (-1)\n  To delete from first, enter (0)\n  Delete from specific index\nEnter position: ";
                    cin >> position;
                    cin.ignore();
                   
                } else if (deleteChoice == 2) {
                    cout << "Enter filename: ";
                    getline(cin, filename);
                    
                } else {
                cout << "|-----------------------------------|\n";
                cout << "| Invalid choice.                   |\n";
                cout << "|-----------------------------------|\n";
                }
                break;
            }
            case 3: // List Files
                
                break;
            case 4: { // Search File
                cout << "Enter filename to search: ";
                getline(cin, filename);
                
                break;
            }
            case 5: { // Sort Files
                displaySortMenu();
                int sortChoice;
                cin >> sortChoice;
                cin.ignore();
                if (sortChoice >= 1 && sortChoice <= 3) {
                    
                } else if (sortChoice != 0) {
                cout << "|-----------------------------------|\n";
                cout << "| Invalid choice.                   |\n";
                cout << "|-----------------------------------|\n";
                }
                break;
            }
            case 6: { // File Operations
                while (true) {
                    displayFileOperationsMenu();
                    int fileOpChoice;
                    cin >> fileOpChoice;
                    cin.ignore();
                    
                    if (fileOpChoice == 0) break;
                    
                    cout << "Enter filename: ";
                    getline(cin, filename);
                    
                    switch (fileOpChoice) {
                        case 1:
                            
                            break;
                        case 2:
                            cout << "Enter content to append: ";
                            getline(cin, content);
                            
                            break;
                        case 3:
                            cout << "Enter new content: ";
                            getline(cin, content);
                            
                            break;
                        case 4:
                            
                            break;
                        case 5:
                            cout << "Enter new name: ";
                            getline(cin, newName);
                            
                            break;
                        case 6:
                            
                            break;
                        case 7:
                           
                            break;
                        default:
                        cout << "|-----------------------------------|\n";
                        cout << "| Invalid choice.                   |\n";
                        cout << "|-----------------------------------|\n";
                    }
                }
                break;
            }
            case 7: { // Advanced Search
                while (true) {
                    displayAdvancedSearchMenu();
                    int advSearchChoice;
                    cin >> advSearchChoice;
                    cin.ignore();
                    
                    if (advSearchChoice == 0) break;
                    
                    switch (advSearchChoice) {
                        case 1:
                            
                            break;
                        case 2:
                          
                            break;
                        case 3:
                            
                            break;
                        case 4:
                          
                            break;
                        default:
                            cout << "|-----------------------------------|\n";
                            cout << "| Invalid choice.                   |\n";
                            cout << "|-----------------------------------|\n";
                    }
                }
                break;
            }
            case 8: // Delete All Files
                cout << "Are you sure you want to delete all files? (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore();
                if (confirm == 'y' || confirm == 'Y') {
                    
                }
                break;
            case 9: { // View Directory Contents
                cout << "Enter directory path (leave empty for current): ";
                string path;
                getline(cin, path);
                if (path.empty()) {
                   
                } else {
                    
                }
                break;
            }
            case 10: // Manage Recycle Bin
                
                break;
            case 11: // View Memory Status
                
                break;
            default:
                cout << "|-----------------------------------|\n";
                cout << "| Invalid choice. Please try again. |\n";
                cout << "|-----------------------------------|\n";
        }
    }

    return 0;
}