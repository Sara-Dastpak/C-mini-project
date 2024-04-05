#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
struct Node {
    std::string word;                   // Word
    std::string meanings;               // Meanings
    Node* next;                         // Pointer to the next node
};

// Dictionary class definition
class Dictionary {
public:
    // Constructor
    Dictionary() {
        // Check if the file exists, if not create it
        std::ifstream file("dictionary.txt");
        if (!file.is_open()) {
            std::ofstream createFile("dictionary.txt");
            createFile.close();
        }
    }

    // Function to print words and their meanings
    void printDictionary() const {
        std::ifstream inputFile("dictionary.txt");
        if (!inputFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }
        std::string line;
        while (std::getline(inputFile, line)) {
            std::cout << line << std::endl;
        }
        inputFile.close();
    }

    // Function to check if a word exists in the dictionary
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
            // Check each word in the line for a match
            while (std::getline(iss, token, ':')) {
                if (token == word) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }

    // Function to add words and their meanings
    // Function to add words and their meanings
    void addWord(const std::string& word, const std::string& meanings) {
        // Check if the word exists
        bool wordFound = false;
        std::ifstream inputFile("dictionary.txt");
        std::ofstream tempFile("temp.txt");
        if (!inputFile.is_open() || !tempFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            inputFile.close();
            tempFile.close();
            return;
        }

        std::string line;
        while (std::getline(inputFile, line)) {
            std::string currentWord;
            std::stringstream ss(line);
            std::getline(ss, currentWord, ':');
            if (currentWord == word) {
                wordFound = true;
                std::vector<std::string> existingMeanings;
                std::string meaning;
                while (std::getline(ss, meaning, ',')) {
                    existingMeanings.push_back(meaning);
                }
                std::istringstream iss(meanings);
                while (std::getline(iss, meaning, ',')) {
                    if (std::find(existingMeanings.begin(), existingMeanings.end(), meaning) == existingMeanings.end()) {
                        existingMeanings.push_back(meaning);
                    }
                }
                std::sort(existingMeanings.begin(), existingMeanings.end());
                tempFile << currentWord << ":";
                for (size_t i = 0; i < existingMeanings.size(); ++i) {
                    if (i != 0)
                        tempFile << ",";
                    tempFile << existingMeanings[i];
                }
                tempFile << std::endl;
            } else {
                tempFile << line << std::endl; // Write the line as it is
            }
        }

        // If word is not found, append it to the end
        if (!wordFound) {
            tempFile << word << ":" << meanings << std::endl;
        }

        inputFile.close();
        tempFile.close();

        // Replace the original file with the temporary file
        std::remove("dictionary.txt");
        std::rename("temp.txt", "dictionary.txt");

        // Sort all meanings in the dictionary file
        sortMeanings();
        sortWords();
    }

    void sortWords() {
        std::ifstream inputFile("dictionary.txt");
        if (!inputFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();

        // Sort the lines
        std::sort(lines.begin(), lines.end());

        // Rewrite the sorted lines to the file
        std::ofstream outputFile("dictionary.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        for (const auto& sortedLine : lines) {
            outputFile << sortedLine << std::endl;
        }
        outputFile.close();
    }

    // Function to sort the meanings of each word
    void sortMeanings() {
        std::ifstream inputFile("dictionary.txt");
        if (!inputFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return;
        }

        std::ofstream tempFile("temp.txt");
        if (!tempFile.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            inputFile.close();
            return;
        }

        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string word;
            std::getline(iss, word, ':');

            // Extract and sort meanings
            std::vector<std::string> meanings;
            std::string meaning;
            while (std::getline(iss, meaning, ',')) {
                meanings.push_back(meaning);
            }
            std::sort(meanings.begin(), meanings.end());

            // Write sorted line to temporary file
            tempFile << word << ":";
            for (const auto& m : meanings) {
                tempFile << m;
                if (&m != &meanings.back()) // Add comma if not the last meaning
                    tempFile << ",";
            }
            tempFile << std::endl;
        }

        inputFile.close();
        tempFile.close();

        // Replace the original file with the temporary file
        std::remove("dictionary.txt");
        std::rename("temp.txt", "dictionary.txt");
    }
// Function to check if a meaning exists for a word
    bool meaningExists(const std::string& word, const std::string& meaning) const {
        std::ifstream file("dictionary.txt");
        if (!file.is_open()) {
            std::cerr << "Error: Couldn't open the file." << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string currentWord;
            std::getline(iss, currentWord, ':');
            if (currentWord == word) {
                std::string token;
                while (std::getline(iss, token, ',')) {
                    // Trim leading and trailing spaces
                    token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
                    token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
                    if (token == meaning) {
                        file.close();
                        return true;
                    }
                }
                break;
            }
        }

        file.close();
        return false;
    }

};
int main() {
    Dictionary dict;
    std::string newWord;
    std::cout << "Enter a new word: ";
    std::getline(std::cin, newWord); // Read the word including spaces

    std::string meanings;
    std::string input;
    std::cout << "Enter meanings for '" << newWord << "' (type 'end' to stop):" << std::endl;
    while (true) {
        std::getline(std::cin, input);
        if (input == "end")
            break;
        
        // Check if the meaning already exists
        if (dict.wordExists(newWord) && dict.meaningExists(newWord, input)) {
            std::cout << "Meaning already exists. Enter another or type 'end' to stop:" << std::endl;
            continue;
        }

        if (!meanings.empty())
            meanings += ",";
        meanings += input; // Concatenate the meanings
    }

    dict.addWord(newWord, meanings);
    
    dict.printDictionary();
    std::cout << "Dictionary saved to 'dictionary.txt'." << std::endl;

    return 0;
}
