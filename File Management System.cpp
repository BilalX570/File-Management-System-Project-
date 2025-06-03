#include<iostream>
#include <fstream>
#include<string>
#include <algorithm>
#include <ctime>
#include <sys/stat.h>
#include <map>
#include <limits>
#include <vector>
#include <filesystem>
#include <deque>
#include <iomanip>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

// File type classification
enum FileType {
    DOCUMENT, IMAGE, AUDIO, VIDEO, ARCHIVE, DIRECTORY, OTHER
};

// Mapping of file extensions to their types
map<string, FileType> fileTypeMap = {
    {".txt", DOCUMENT}, {".pdf", DOCUMENT}, 
    {".doc", DOCUMENT}, {".docx", DOCUMENT},
    {".jpg", IMAGE}, {".png", IMAGE},
    {".gif", IMAGE}, {".bmp", IMAGE},
    {".mp3", AUDIO}, {".wav", AUDIO},
    {".mp4", VIDEO}, {".mov", VIDEO}, 
    {".zip", ARCHIVE}, {".rar", ARCHIVE}
};

// Helper function to format time
string formatTime(time_t time) {
    if (time == 0) return "Unknown";
    tm* timeinfo = localtime(&time);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

// Determine file type based on extension
FileType getFileType(const string& filename) {
    if (fs::is_directory(filename)) {
        return DIRECTORY;
    }

    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos) {
        string ext = filename.substr(dotPos);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        auto it = fileTypeMap.find(ext);
        if (it != fileTypeMap.end()) return it->second;
    }
    return OTHER;
}

// Convert FileType enum to string for display
string fileTypeToString(FileType type) {
    switch(type) {
        case DOCUMENT: return "Document";
        case IMAGE:    return "Image";
        case AUDIO:    return "Audio";
        case VIDEO:    return "Video";
        case ARCHIVE:  return "Archive";
        case DIRECTORY: return "Directory";
        default:       return "Other";
    }
}
struct FileNode {
    string filename;
    string content;
    size_t size;
    time_t createdDate;
    time_t lastModified;
    time_t lastSeenDate;
    FileType type;
    FileNode* prev;
    FileNode* next;
  
    FileNode(const string& name, const string& cont = "") : 
        filename(name), content(cont), prev(nullptr), next(nullptr) {
        type = getFileType(filename);
        updateFileStats();
        createdDate = time(nullptr);
        lastSeenDate = time(nullptr);
    }
    
    void updateFileStats() {
        size = (type == DIRECTORY) ? 0 : content.size();
        lastModified = time(nullptr);
        lastSeenDate = time(nullptr);
    }
    
    void displayInfo() const {
        cout << "File: " << filename << "\n";
        cout << "Type: " << fileTypeToString(type) << "\n";
        cout << "Size: " << size << " bytes\n";
        cout << "Created: " << formatTime(createdDate) << "\n";
        cout << "Modified: " << formatTime(lastModified) << "\n";
        cout << "Last Seen: " << formatTime(lastSeenDate) << "\n";
        
        if (type != DIRECTORY) {
            int lineCount = count(content.begin(), content.end(), '\n');
            if (!content.empty() && content.back() != '\n') lineCount++;
            cout << "Lines: " << lineCount << "\n";
        }
    }
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

void removeFileFromBeginning() {
        if (isEmpty()) {
            cout << "List is empty.\n";
            return;
        }

        FileNode* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        
        cout << "File '" << temp->filename << "' removed from beginning.\n";
        delete temp;
        count--;
    }

    void removeFileFromEnd() {
        if (isEmpty()) {
            cout << "List is empty.\n";
            return;
        }

        FileNode* temp = tail;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        
        cout << "File '" << temp->filename << "' removed from end.\n";
        delete temp;
        count--;
    }

    void removeFileFromPosition(int position) {
        if (position < 0 || position >= count) {
            cout << "Invalid position.\n";
            return;
        }

        if (position == 0) {
            removeFileFromBeginning();
            return;
        }

        if (position == count - 1) {
            removeFileFromEnd();
            return;
        }

        FileNode* current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        
        current->prev->next = current->next;
        current->next->prev = current->prev;
        
        cout << "File '" << current->filename << "' removed from position " << position << ".\n";
        delete current;
        count--;
    }

    void removeFile(const string& filename) {
        if (isEmpty()) {
            cout << "List is empty.\n";
            return;
        }

        if (head->filename == filename) {
            removeFileFromBeginning();
            return;
        }

        if (tail->filename == filename) {
            removeFileFromEnd();
            return;
        }

        FileNode* current = head->next;
        while (current && current->filename != filename) {
            current = current->next;
        }

        if (current) {
            current->prev->next = current->next;
            if (current->next) {
                current->next->prev = current->prev;
            }
            cout << "File '" << current->filename << "' removed.\n";
            delete current;
            count--;
        } else {
            cout << "File '" << filename << "' not found.\n";
        }
    }

    void sortByName() {
        if (count <= 1) return;

        bool swapped;
        FileNode* last = nullptr;
        
        do {
            swapped = false;
            FileNode* current = head;
            
            while (current->next != last) {
                if (current->filename > current->next->filename) {
                    swapNodesData(current, current->next);
                    swapped = true;
                }
                current = current->next;
            }
            last = current;
        } while (swapped);
    }

    void sortBySize() {
        if (count <= 1) return;

        bool swapped;
        FileNode* last = nullptr;
        
        do {
            swapped = false;
            FileNode* current = head;
            
            while (current->next != last) {
                if (current->size > current->next->size) {
                    swapNodesData(current, current->next);
                    swapped = true;
                }
                current = current->next;
            }
            last = current;
        } while (swapped);
    }

    void sortByModifiedDate() {
        if (count <= 1) return;

        bool swapped;
        FileNode* last = nullptr;
        
        do {
            swapped = false;
            FileNode* current = head;
            
            while (current->next != last) {
                if (current->lastModified > current->next->lastModified) {
                    swapNodesData(current, current->next);
                    swapped = true;
                }
                current = current->next;
            }
            last = current;
        } while (swapped);
    }

    map<FileType, size_t> getTotalSizesByType() const {
        map<FileType, size_t> sizeMap;
        FileNode* current = head;
        while (current) {
            sizeMap[current->type] += current->size;
            current = current->next;
        }
        return sizeMap;
    }

    void printFiles() const {
        if (isEmpty()) {
            cout << "No files in the list.\n";
            return;
        }

        FileNode* current = head;
        int index = 1;
        
        while (current) {
            cout << index++ << ". " << current->filename << endl;
            cout << "------------------------------------------------------\n";
            current->displayInfo();
            current = current->next;
            if (current) cout << endl;
        }
    }

    void clear() {
        while (head) {
            FileNode* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        count = 0;
    }

    FileNode* getFileNode(const string& filename) {
        FileNode* current = head;
        while (current) {
            if (current->filename == filename) {
                current->lastSeenDate = time(nullptr);
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    const FileNode* getFileNode(const string& filename) const {
        FileNode* current = head;
        while (current) {
            if (current->filename == filename) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    FileNode* getFileNode(int index) {
        if (index < 0 || index >= count) return nullptr;

        FileNode* current = head;
        for (int i = 0; i < index && current; i++) {
            current = current->next;
        }
        if (current) {
            current->lastSeenDate = time(nullptr);
        }
        return current;
    }

    void addFile(const string& filename, const string& content = "", int position = -1) {
        if (position == -1) {
            addFileAtEnd(filename, content);
        } else if (position == 0) {
            addFileAtBeginning(filename, content);
        } else {
            addFileAtPosition(filename, position, content);
        }
    }

    void removeFile(int position = -1) {
        if (position == -1) {
            removeFileFromEnd();
        } else if (position == 0) {
            removeFileFromBeginning();
        } else {
            removeFileFromPosition(position);
        }
    }

    void searchByPrefix(const string& prefix) const {
        FileNode* current = head;
        bool found = false;
        int index = 1;
        
        while (current) {
            if (current->filename.find(prefix) == 0) {
                cout << index++ << ". " << current->filename << endl;
                current->displayInfo();
                found = true;
                current->lastSeenDate = time(nullptr);
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "No files found with prefix '" << prefix << "'.\n";
        }
    }



    void readFile(const string& filename) {
        if (!fileList.contains(filename)) {
            cout << "File not found in the managed list.\n";
            return;
        }

        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode->type == DIRECTORY) {
            cout << "This is a directory, not a file.\n";
            return;
        }

        string content = readFileContent(filename);
        if (!content.empty()) {
            cout << "Contents of '" << filename << "':\n";
            cout << content;
            fileList.updateFileContent(filename, content);
            saveFiles();
        } else {
            cout << "File is empty or couldn't be read.\n";
        }
    }

    void updateFile(const string& filename, const string& content) {
        if (!fileList.contains(filename)) {
            cout << "File doesn't exist. Create it first.\n";
            return;
        }

        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode->type != DOCUMENT) {
            cout << "Append or rewrite is not allowed for non-document files.\n";
            return;
        }

        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << content << '\n';
            cout << "Content appended to '" << filename << "' successfully.\n";
            file.close();
            
            string currentContent = fileList.getFileContent(filename);
            fileList.updateFileContent(filename, currentContent + content + "\n");
            saveFiles();
        } else {
            cout << "Error: Unable to open file '" << filename << "'.\n";
        }
    }

    void overwriteFile(const string& filename, const string& content) {
        if (!fileList.contains(filename)) {
            cout << "File doesn't exist. Create it first.\n";
            return;
        }

        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode->type != DOCUMENT) {
            cout << "Append or rewrite is not allowed for non-document files.\n";
            return;
        }

        ofstream file(filename);
        if (file.is_open()) {
            file << content;
            cout << "File '" << filename << "' overwritten successfully.\n";
            file.close();
            
            fileList.updateFileContent(filename, content);
            saveFiles();
        } else {
            cout << "Error: Unable to open file '" << filename << "'.\n";
        }
    }

    void deleteFile(int position = -1) {
        if (fileList.size() == 0) {
            cout << "No files to delete.\n";
            return;
        }

        FileNode* fileNode;
        if (position == -1) {
            fileNode = fileList.getFileNode(fileList.size() - 1);
        } else if (position == 0) {
            fileNode = fileList.getFileNode(0);
        } else {
            fileNode = fileList.getFileNode(position);
        }

        if (!fileNode) {
            cout << "Invalid file position.\n";
            return;
        }

        string filename = fileNode->filename;
        if (recycleBin.addToBin(filename)) {
            fileList.removeFile(position);
            saveFiles();
        }
    }

    void deleteFileByName(const string& filename) {
        if (fileList.size() == 0) {
            cout << "No files to delete.\n";
            return;
        }

        FileNode* fileNode = fileList.getFileNode(filename);
        if (!fileNode) {
            cout << "File not found in managed list.\n";
            return;
        }

        if (recycleBin.addToBin(filename)) {
            fileList.removeFile(filename);
            saveFiles();
        }
    }

    void deleteAllFiles() {
        if (fileList.size() == 0) {
            cout << "No files to delete.\n";
            return;
        }

        cout << "Are you sure you want to delete all files? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (confirm != 'y' && confirm != 'Y') return;

        FileNode* current = fileList.getFileNode(0);
        while (current) {
            recycleBin.addToBin(current->filename);
            current = current->next;
        }
        
        fileList.clear();
        saveFiles();
        cout << "All files and directories moved to Recycle Bin.\n";
    }

    void listFiles() const {
        cout << "\nManaged Files (" << fileList.size() << "):\n";
        fileList.printFiles();
    }

    void searchFile(const string& filename) const {
        if (fileList.contains(filename)) {
            cout << "File found: " << filename << endl;
            displayFileStats(filename);
        } else {
            cout << "File not found.\n";
        }
    }

    void searchFilesByPrefix() {
        string prefix;
        cout << "Enter filename prefix to search: ";
        getline(cin, prefix);
        fileList.searchByPrefix(prefix);
    }


void updateFileMetadata(const string& filename) {
        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode) {
            fileNode->updateFileStats();
            saveFiles();
            cout << "Metadata updated for " << filename << ".\n";
        } else {
            cout << "File not found.\n";
        }
    }

    void searchFilesByContent() {
        string keyword;
        cout << "Enter content keyword to search: ";
        getline(cin, keyword);
        
        vector<FileNode*> results = fileList.searchByContent(keyword);
        if (results.empty()) {
            cout << "No files found containing '" << keyword << "'.\n";
        } else {
            cout << "Files containing '" << keyword << "':\n";
            for (size_t i = 0; i < results.size(); i++) {
                cout << i+1 << ". " << results[i]->filename << " (" 
                     << fileTypeToString(results[i]->type) << ")\n";
            }
        }
    }

    void searchFilesByType() {
        cout << "----------------------------------------\n";
        cout << "Select file type to search:\n";
        cout << "1. Document\n";
        cout << "2. Image\n";
        cout << "3. Audio\n";
        cout << "4. Video\n";
        cout << "5. Archive\n";
        cout << "6. Directory\n";
        cout << "7. Other\n";
        cout << "----------------------------------------\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        FileType type;
        switch (choice) {
            case 1: type = DOCUMENT; break;
            case 2: type = IMAGE; break;
            case 3: type = AUDIO; break;
            case 4: type = VIDEO; break;
            case 5: type = ARCHIVE; break;
            case 6: type = DIRECTORY; break;
            case 7: type = OTHER; break;
            default: 
                cout << "Invalid choice.\n";
                return;
        }
        
        vector<FileNode*> results = fileList.searchByType(type);
        if (results.empty()) {
            cout << "No files found of type " << fileTypeToString(type) << ".\n";
        } else {
            cout << "Files of type " << fileTypeToString(type) << ":\n";
            for (size_t i = 0; i < results.size(); i++) {
                cout << i+1 << ". " << results[i]->filename << "\n";
            }
        }
    }

    void searchFilesBySizeRange() {
        size_t minSize, maxSize;
        cout << "Enter minimum size (bytes): ";
        cin >> minSize;
        cout << "Enter maximum size (bytes): ";
        cin >> maxSize;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (minSize > maxSize) {
            cout << "Invalid range (min > max).\n";
            return;
        }
        
        vector<FileNode*> results = fileList.searchBySizeRange(minSize, maxSize);
        if (results.empty()) {
            cout << "No files found in size range " << minSize << "-" << maxSize << " bytes.\n";
        } else {
            cout << "Files in size range " << minSize << "-" << maxSize << " bytes:\n";
            for (size_t i = 0; i < results.size(); i++) {
                cout << i+1 << ". " << results[i]->filename << " (" 
                     << results[i]->size << " bytes)\n";
            }
        }
    }

    void displayDirectoryContents(const string& path = ".") const {
        try {
            cout << "\nContents of directory '" << path << "':\n";
            int count = 1;
            
            for (const auto& entry : fs::directory_iterator(path)) {
                string filename = entry.path().filename().string();
                FileType type = getFileType(entry.path().string());
                
                cout << count++ << ". " << filename << " (" << fileTypeToString(type) << ")\n";
                
                if (type != DIRECTORY) {
                    cout << "   Size: " << entry.file_size() << " bytes\n";
                }
                
                auto ftime = entry.last_write_time();
                time_t cftime = chrono::system_clock::to_time_t(
                    chrono::time_point_cast<chrono::system_clock::duration>(
                        ftime - decltype(ftime)::clock::now() + chrono::system_clock::now()
                    )
                );
                cout << "   Modified: " << formatTime(cftime);
            }
        } catch (const exception& e) {
            cerr << "Error reading directory: " << e.what() << endl;
=======
    void fileStatistics(const string& filename) const {
        displayFileStats(filename);
    }

    void displayFileContent(const string& filename) const {
        const FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode) {
            if (fileNode->type == DIRECTORY) {
                cout << filename << " is a directory.\n";
            } else {
                cout << "Content of " << filename << " from memory:\n";
                cout << fileNode->content;
            }
        } else {
            cout << "File not found in memory.\n";
        }
    }

    void sortFiles(int criteria) {
        fileList.sortFiles(criteria);
        saveFiles();
        cout << "Files sorted successfully.\n";
    }

    void retrieveFileContent(const string& filename) const {
        displayFileContent(filename);
    }

    void updateFileName(const string& oldName, const string& newName) {
        FileNode* fileNode = fileList.getFileNode(oldName);
        if (fileNode) {
            if (fileList.contains(newName)) {
                cout << "A file with name '" << newName << "' already exists.\n";
                return;
            }
            
            try {
                fs::rename(oldName, newName);
                fileNode->filename = newName;
                fileNode->type = getFileType(newName);
                saveFiles();
                cout << "File renamed from '" << oldName << "' to '" << newName << "' successfully.\n";
            } catch (const exception& e) {
                cerr << "Error renaming: " << e.what() << endl;
            }
        } else {
            cout << "File not found.\n";
        }
    }

        void manageRecycleBin() {
        while (true) {
            cout << "----------------------------------------\n";
            cout << "\nRecycle Bin Management (" << recycleBin.size() << " items)\n";
            cout << "1. List items\n";
            cout << "2. Restore item\n";
            cout << "3. Delete item permanently\n";
            cout << "4. Empty Recycle Bin\n";
            cout << "0. Back to Main Menu\n";
            cout << "----------------------------------------\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (choice == 0) break;
            
            switch (choice) {
                case 1:
                    recycleBin.listItems();
                    break;
                case 2: {
                    cout << "Enter item number to restore: ";
                    size_t index;
                    cin >> index;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (index > 0) {
                        recycleBin.restoreItem(index - 1);
                    } else {
                        cout << "Invalid index.\n";
                    }
                    break;
                }
                case 3: {
                    cout << "Enter item number to delete permanently: ";
                    size_t index;
                    cin >> index;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (index > 0) {
                        recycleBin.deleteItem(index - 1, true);
                    } else {
                        cout << "Invalid index.\n";
                    }
                    break;
                }
                case 4:
                    cout << "Are you sure you want to empty the Recycle Bin? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (confirm == 'y' || confirm == 'Y') {
                        recycleBin.emptyBin();
                    }
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        }
    }

    void loadFiles() {
        ifstream file("files.txt");
        if (!file) {
            return; // No existing file is okay
        }
        string filename;
        while (getline(file, filename)) {
            if (!filename.empty()) {
                string content = readFileContent(filename);
                fileList.addFile(filename, content);
            }
        }
        file.close();
    }

    void saveFiles() const {
        ofstream file("files.txt");
        if (!file) {
            cout << "Error saving file list.\n";
            return;
        }
        FileNode* current = fileList.head;
        while (current) {
            file << current->filename << '\n';
            current = current->next;
        }
        file.close();
    }
};

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