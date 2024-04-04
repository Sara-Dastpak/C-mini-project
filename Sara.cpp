#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // برای استفاده از std::sort

// تعریف ساختار گره (Node)
struct Node {
    std::string word;   // کلمه
    std::vector<std::string> meanings;   // معانی
    Node* next;         // اشاره‌گر به گره بعدی
};

// تعریف کلاس دیکشنری
class Dictionary {
private:
    Node* head; // اشاره‌گر به ابتدای دیکشنری

public:
    // سازنده کلاس
    Dictionary() : head(nullptr) {}
    // منوی اضافه کردن کلمات
    void addWord2(const std::string& word, const std::string& meanings) {
    if (wordExists(word)) {
        // If the word exists in the dictionary file, add the new meaning to the existing meanings.
        Node* current = head;
        while (current != nullptr) {
            if (current->word == word) {
                current->meanings.push_back(meanings); // Add the new meaning to the vector of meanings.
                break;
            }
            current = current->next;
        }
    } else {
        // Otherwise, if the word doesn't exist, add the new word along with its meaning to the list.
        Node* newNode = new Node; // Create a new node.
        newNode->word = word;     // Set the word.
        newNode->meanings.push_back(meanings); // Add the meaning to the vector of meanings.
        newNode->next = head;     // Set the next pointer to the current head.
        head = newNode;           // Update the head pointer.
    }
}


     // تابع برای بررسی وجود کلمه در فایل
    bool wordExists(const std::string& word) const {
        std::ifstream file("dictionary.txt");
        if (!file.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;

            // بررسی هر کلمه در خط و برابری آن با کلمه مورد نظر
            while (std::getline(iss, token, ',')) {
                if (token == word) {
                    file.close();
                    return true;
                }
            }
        }

        file.close();
        return false;
    }

    // تابع برای اضافه کردن یک واژه و معنی به دیکشنری
    void addWord(const std::string& word, const std::string& meanings) {
    Node* newNode = new Node; // Create a new node
    newNode->word = word;     // Set the word

    // Split the meanings string into individual meanings
    std::istringstream iss(meanings);
    std::string meaning;
    while (std::getline(iss, meaning, ',')) {
        newNode->meanings.push_back(meaning); // Add each meaning to the vector of meanings
    }

    newNode->next = head;     // Set the next pointer to the current head
    head = newNode;           // Update the head pointer
}


    // تابع برای چاپ لیست کلمات و معانی
    void printDictionary() const {
    std::ofstream outputFile("dictionary.txt", std::ios::app); // Open a file for writing with append mode

    if (!outputFile.is_open()) { // Check if the file was opened successfully
        std::cerr << "Error: Couldn't open the file." << std::endl;
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        outputFile << current->word; // Output the word

        // Output meanings
        for (const std::string& meaning : current->meanings) {
            outputFile << ", " << meaning;
        }

        outputFile << std::endl;
        current = current->next;
    }

    outputFile.close();
}


    void printDict() {
        std::ifstream inputFile("dictionary.txt"); // باز کردن فایل برای خواندن

        if (!inputFile.is_open()) { // بررسی موفقیت باز کردن فایل
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        std::string line;
        std::cout << "Content of text.txt:" << std::endl;
        while (std::getline(inputFile, line)) { // خواندن و پرینت کردن هر خط از فایل
            std::cout << line << std::endl;
        }

        inputFile.close(); // بستن فایل

    }

    void searchWordInFile(const std::string& word) const {
        std::ifstream inputFile("dictionary.txt"); // باز کردن فایل برای خواندن

        if (!inputFile.is_open()) { // بررسی موفقیت باز کردن فایل
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(inputFile, line)) { // خواندن خط به خط از فایل
            if (line.find(word) != std::string::npos) { // اگر کلمه مورد نظر در خط وجود داشت
                std::cout << line << std::endl; // چاپ خط
            }
        }

        inputFile.close(); // بستن فایل
    }

    // تابع برای حذف کلمه به همراه مترادف‌های آن از لیست
    void removeWordFromList(const std::string& word) {
        Node* current = head;
        Node* prev = nullptr;

        // پیدا کردن گره مربوط به کلمه مورد نظر
        while (current != nullptr && current->word != word) {
            prev = current;
            current = current->next;
        }

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

    // تابع برای حذف کلمه به همراه مترادف‌های آن از فایل متنی
    void removeWordFromFile(const std::string& filename, const std::string& word) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        std::string line;
        std::vector<std::string> lines;

        // خواندن هر خط از فایل و اضافه کردن آن به لیست خطوط
        while (std::getline(file, line)) {
            if (line.find(word) == std::string::npos) { // اگر کلمه مورد نظر در خط نبود
                lines.push_back(line);
            }
        }

        file.close();

        // باز کردن فایل برای نوشتن و نوشتن خطوط غیر حذف شده در آن
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        for (const auto& line : lines) {
            outFile << line << std::endl;
        }

        outFile.close();

        std::cout << "Word and its synonyms removed from the dictionary file." << std::endl;
    }

     void replaceWord(const std::string& oldWord, const std::string& newWord) {
        // جایگزین کردن کلمه در لیست
        Node* current = head;
        while (current != nullptr) {
            if (current->word == oldWord) {
                current->word = newWord;
            }
            current = current->next;
        }

        // جایگزین کردن کلمه در فایل متنی
        std::ifstream file("dictionary.txt");
        if (!file.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        std::string line;
        std::vector<std::string> lines;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            std::string newLine;

            while (std::getline(iss, token, ',')) {
                if (token == oldWord) {
                    token = newWord;
                }
                newLine += token + ",";
            }

            // حذف کاراکتر ',' اضافی از انتهای خط
            if (!newLine.empty() && newLine.back() == ',') {
                newLine.pop_back();
            }

            lines.push_back(newLine);
        }

        file.close();

        std::ofstream outFile("dictionary.txt");
        if (!outFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        for (const auto& line : lines) {
            outFile << line << std::endl;
        }

        outFile.close();

        std::cout << "Word replaced in the dictionary." << std::endl;
    }
};

int main() {
    Dictionary dict; // ایجاد یک شیء از کلاس دیکشنری

    std::string newWord;
    std::cout << "Enter new word: " << std::endl;
    std::cin >> newWord;

    std::vector<std::string> inputs; // استفاده از std::vector به جای std::list

    std::string input;
    std::cout << "Enter synonyms for '" << newWord << "' (type 'end' to stop):" << std::endl;
    while (true) {
        std::cin >> input;
        if (input == "end") // اگر واژه "end" وارد شود، حلقه متوقف می‌شود
            break;
        inputs.push_back(input); // اضافه کردن ورودی به آرایه
    }

    // اضافه کردن کلمات و مترادف‌ها به دیکشنری
    for (const auto& word : inputs) {
        dict.addWord2(newWord, word);
    }

    // چاپ لیست کلمات و معانی در فایل
    dict.printDictionary();

    std::cout << "Dictionary saved to 'dictionary.txt'." << std::endl;

    return 0;

    // Dictionary dict; // ایجاد یک شیء از کلاس دیکشنری

    // std::string newWord;
    // std::cout << "Enter new word: " << std::endl;
    // std::cin >> newWord;

    // std::vector<std::string> inputs; // استفاده از std::vector به جای std::list

    // std::string input;
    // bool atLeastOneWord = false; // متغیری برای نشان دادن اینکه حداقل یک کلمه دریافت شده است یا خیر
    // std::cout << "Enter synonyms for '" << newWord << "' (type 'end' to stop):" << std::endl;
    // while (true) {
    //     std::cin >> input;
    //     if (input == "end") {
    //         if (!atLeastOneWord) {
    //             std::cout << "Please enter at least one synonym." << std::endl;
    //             continue; // اگر حداقل یک کلمه وارد نشده باشد، ادامه دهید
    //         } else {
    //             break;
    //         }
    //     }
    //     inputs.push_back(input); // اضافه کردن ورودی به آرایه
    //     atLeastOneWord = true; // نشان دادن اینکه حداقل یک کلمه دریافت شده است
    // }


    // // اضافه کردن کلمات و مترادف‌ها به دیکشنری
    // for (const auto& word : inputs) {
    //     dict.addWord2(newWord, word);
    // }

    // // چاپ لیست کلمات و معانی در فایل
    // dict.printDictionary();

    // std::cout << "Dictionary saved to 'dictionary.txt'." << std::endl;

    // // print all of dictionary
    // // dict.printDict();
    // dict.searchWordInFile("errr");

    // // حذف کلمه از لیست
    // dict.removeWordFromList("dddd");

    // // حذف کلمه از فایل متنی
    // dict.removeWordFromFile("dictionary.txt", "boy");

    // // change spelling of word in list and file
    // dict.replaceWord("hello", "hi");


    // return 0;
}
