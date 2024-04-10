#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//تعریف کردن لینک لیست
struct Node {
    std::string word;
    std::vector<std::string> meanings;
    Node* next;
    Node(const std::string& w) : word(w), next(nullptr) {}
};

class Dictionary {
private:
    Node* head;
public:
    Dictionary() : head(nullptr) {}
    // تکست فایل را تبدیل به لینک لیست می کند
    void readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "File doesn't exist. Creating a new file..." << std::endl;
            std::ofstream createFile(filename);
            createFile.close();
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string word;
            if (std::getline(iss, word, ':')) {
                std::string meaning;
                while (std::getline(iss, meaning, ',')) {
                    if (!wordExists(word)) {
                        Node* newNode = new Node(word);
                        newNode->meanings.push_back(meaning);
                        newNode->next = head;
                        head = newNode;
                    } else {
                        Node* current = head;
                        while (current != nullptr) {
                            if (current->word == word) {
                                current->meanings.push_back(meaning);
                                break;
                            }
                            current = current->next;
                        }
                    }
                }
            }
        }
        sortWords();
        sortMeanings();
        file.close();
    }
    // تابعی که چک می کند آیا لغت در لینک لیست موجود می باشد یا نه
    bool wordExists(const std::string& word) {
        Node* current = head;
        while (current != nullptr) {
            if (current->word == word)
                return true;
            current = current->next;
        }
        return false;
    }
    // تابعی که لغت جدید یا در صورت وجود لغت معنی جدید را به لینک لیست اضافه میکند
    void addWord(const std::string& word, const std::string& meaning) {
        // اضافه کردن معنی
        if(wordExists(word)){
            Node* current = head;
            while (current != nullptr) {
                if (current->word == word) {
                    current->meanings.push_back(meaning);
                    break;
                }
            current = current->next;
            }
        }
        // اضافه کردن لغت جدید و معنی
        else{
            Node* newNode = new Node(word);
            newNode->meanings.push_back(meaning);
            newNode->next = head;
            head = newNode;
        }
    }
    // تابعی که لغت ها را نسبت به حروف الفبا مرتب می کند
    void sortWords() {
        // ابتدا لغت ها را یک لیست کرده سپس آن ها را مرتب می کند
        std::vector<Node*> nodes;
        Node* current = head;
        while (current != nullptr) {
            nodes.push_back(current);
            current = current->next;
        }
        std::sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
            return a->word > b->word; // Reverse the comparison logic
        });
        // آنهارا تبدیل به لینک لیست میکند
        head = nullptr;
        for (Node* node : nodes) {
            node->next = head;
            head = node;
        }
    }
    // تابعی که معنی هر لغت را نسبت یه حروف الفبا مرتب می کند
    void sortMeanings() {
        Node* current = head;
        while (current != nullptr) {
            std::sort(current->meanings.begin(), current->meanings.end()); // Sort meanings alphabetically
            auto last = std::unique(current->meanings.begin(), current->meanings.end()); // Remove duplicates
            current->meanings.erase(last, current->meanings.end()); // Erase removed duplicates
            current = current->next;
        }
    }
    // تابعی که لغت را گرفته و لغت و معنی هایش را از لینک لیست حذف می کند
    void removeWord(const std::string& word) {
        Node* current = head;
        Node* prev = nullptr;
        while (current != nullptr && current->word != word) {
            prev = current;
            current = current->next;
        }
        // اگر کلمه وجود نداشته باشد
        if (current == nullptr) {
            std::cerr << "Word not found in the dictionary." << std::endl;
            return;
        }
        // حذف گره مربوط به کلمه از لیست
        if (prev == nullptr) {
            head = current->next;
        } else {
            prev->next = current->next;
        }
        delete current;
        std::cout << "Word and its synonyms removed from the dictionary list." << std::endl;
    }
    // تابع حذف معنی لغت
    void removeMeaning(const std::string& word, const std::string& meaning) {
        Node* current = head;
        Node* prev = nullptr;
        while (current != nullptr && current->word != word) {
            prev = current;
            current = current->next;
        }
       // اگر کلمه وجود نداشته باشد
        if (current == nullptr) {
            std::cerr << "Word not found in the dictionary." << std::endl;
            return;
        }
        auto& meanings = current->meanings;
        auto it = std::find(meanings.begin(), meanings.end(), meaning);
        // حذف معنی
        if (it != meanings.end()) {
            meanings.erase(it);
            std::cout << "Meaning removed from the dictionary." << std::endl;
        } else {
            std::cerr << "Meaning not found for the word." << std::endl;
            return;
        }
        // حذف کلمه اگر تنها معنی کلمه حذف شده
        if (meanings.empty()) {
            if (prev == nullptr) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            std::cout << "Word and its meanings removed from the dictionary." << std::endl;
        }
    }
    // تابع پیدا کردن کلمه در لیست و پرینت آن
    void searchWord(const std::string& word) const {
        Node* current = head;
        while (current != nullptr && current->word != word) {
            current = current->next;
        }
        // اگر لغت وارد شده در لیست موجود نباشد
        if (current == nullptr) {
            std::cerr << "Word not found in the dictionary." << std::endl;
            return;
        }
        // پرینت لغت وارد شده و معانی آن
        std::cout << "The word and its meanings you searched for:" << std::endl;
        std::cout << current->word << ": ";
        for (size_t i = 0; i < current->meanings.size(); ++i) {
            std::cout << current->meanings[i];
            if (i != current->meanings.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }
    // تابع تغییر املا کلمه
    void replaceWord(const std::string& oldWord, const std::string& newWord) {
        Node* current = head;
        while (current != nullptr) {
            // جایگزین کردن کلمه قبلی با کلمه جدید
            if (current->word == oldWord) {
                current->word = newWord;
                std::cout << "Word '" << oldWord << "' replaced with '" << newWord << "' in the linked list." << std::endl;
                sortWords();
                sortMeanings();
                return;
            }
            current = current->next;
        }
        // اگر کلمه در لیست وجود نداشته باشد
        std::cout << "Word '" << oldWord << "' not found in the linked list." << std::endl;
    }
    // تابعی که تغییرات ایجاد شده در لینک لیست را در فایل تکست نیز اعمال کند
    void saveToFile(const std::string& filename) {
        std::ofstream outputFile(filename, std::ios::trunc);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Couldn't open the file for writing." << std::endl;
            return;
        }
        Node* current = head;
        while (current != nullptr) {
            outputFile << current->word << ":";
            for (size_t i = 0; i < current->meanings.size(); ++i) {
                outputFile << current->meanings[i];
                if (i != current->meanings.size() - 1)
                    outputFile << ",";
            }
            outputFile << std::endl;
            current = current->next;
        }
        outputFile.close();
    }
    // تابعی که تمام لغات با معنی هایشان را چاپ کند
    void printDictionary() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->word << ": ";
            for (size_t i = 0; i < current->meanings.size(); ++i) {
                std::cout << current->meanings[i];
                if (i != current->meanings.size() - 1)
                    std::cout << ", ";
            }
            std::cout << std::endl;
            current = current->next;
        }
    }
};

int main() {
    Dictionary dict;
    // فایل را به لینک لیست تبدیل کند
    dict.readFromFile("dictionary.txt");
    // منو
    std::string choice;
    while (true) {
        std::cout << "What do you want to do? (add / delete word(type deletew) / delete meaning(type deletem) / search / replace / print / close): " << std::endl;
        std::getline(std::cin, choice);
        // لغت جدید اضافه شود
        if (choice == "add") {
            std::string newWord;
            std::cout << "Enter a new word: ";
            std::getline(std::cin, newWord);

            std::string input;
            std::cout << "Enter meanings for '" << newWord << "' (type 'end' to stop):" << std::endl;
            while (true) {
                std::getline(std::cin, input);
                if (input == "end")
                    break;
                dict.addWord(newWord, input);
            }
            // بعد از اضافه کردن لغت و یا معنی داده ها را مرتب کند
            dict.sortWords();
            dict.sortMeanings();
        }
        // لغت حذف شود
        else if (choice == "deletew") {
            std::string deleteWord;
            std::cout << "Enter the word you want to delete: ";
            std::getline(std::cin, deleteWord);
            dict.removeWord(deleteWord);
        }
        // معنی حذف شود
        else if (choice == "deletem") {
            std::string wordToDelete, meaningToDelete;
            std::cout << "Enter the word you want to delete the meaning for: ";
            std::getline(std::cin, wordToDelete);
            std::cout << "Enter the meaning you want to delete: ";
            std::getline(std::cin, meaningToDelete);
            dict.removeMeaning(wordToDelete, meaningToDelete);
        }
        // پیدا کردن یک لغت و چاپ کردن معانی آن
        else if (choice == "search") {
            std::string wordToPrint;
            std::cout << "Enter the word you want to print: ";
            std::getline(std::cin, wordToPrint);
            dict.searchWord(wordToPrint);
        }
        // چاپ کردن کل لینک لیست
        else if (choice == "print") {
            dict.printDictionary();
        }
        // تغییر دادن املای یک کلمه
        else if (choice == "replace") {
            std::string oldWord, newWordd;
            std::cout << "Enter the word you want to replace: ";
            std::getline(std::cin, oldWord);
            std::cout << "Enter the new word: ";
            std::getline(std::cin, newWordd);
            dict.replaceWord(oldWord, newWordd);
        }
        // بستن برنامه
        else if (choice == "close") {
            std::string saveChoice;
            std::cout << "Do you want to save the changes before closing? (yes/no): ";
            std::getline(std::cin, saveChoice);
            // تغییرات ذخیره شود
            if (saveChoice == "yes") {
                dict.saveToFile("dictionary.txt");
                std::cout << "Dictionary updated and saved." << std::endl;
                break;
            }
            // تغییرات ذخیره نشود
            else if (saveChoice == "no") {
                std::cout << "Changes not saved. Closing the application." << std::endl;
                break;
            } else {
                std::cout << "Invalid choice. Please enter 'yes' or 'no'." << std::endl;
            }
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return 0;
}
