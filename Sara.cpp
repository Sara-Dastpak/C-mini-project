#include <algorithm> // for std::sort
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
                    if (!wordExists(word)) { // Check if word exists in the linked list
                        Node* newNode = new Node(word); // Create a new node for each word
                        newNode->meanings.push_back(meaning); // Add meanings to the node
                        newNode->next = head; // Set the next pointer of the new node to the current head
                        head = newNode; // Update the head pointer to the new node
                    } else { // If word already exists, just add the meaning to its existing node
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
        file.close();
    }

    bool wordExists(const std::string& word) {
        Node* current = head;
        while (current != nullptr) {
            if (current->word == word)
                return true;
            current = current->next;
        }
        return false;
    }


    void addWord(const std::string& word, const std::string& meaning) {
        //readFromFile(filename); // Read the dictionary file to ensure proper initialization of head
        if(wordExists(word)){
            Node* current = head;
            while (current != nullptr) {
                if (current->word == word) {
                // If the word exists, find the corresponding node and add the meaning to it
                    current->meanings.push_back(meaning);
                //saveToFile(filename); // Save the updated dictionary to file
                    break; // Exit the function once the meaning is added
                }
            current = current->next;
            }
        }else{
        // If the word doesn't exist, create a new node and add it to the list
            Node* newNode = new Node(word); // Create a new node with the given word
            newNode->meanings.push_back(meaning); // Add the meaning to the new node
            newNode->next = head; // Set the next pointer of the new node to the current head
            head = newNode; // Update the head pointer to the new node
            //saveToFile(filename); // Save the updated dictionary to file
        }
    }
    void sortWords() {
        std::vector<Node*> nodes;
        Node* current = head;
        while (current != nullptr) {
            nodes.push_back(current);
            current = current->next;
        }

        std::sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
            return a->word > b->word; // Reverse the comparison logic
        });

        head = nullptr;
        for (Node* node : nodes) {
            node->next = head;
            head = node;
        }
    }
    void sortMeanings() {
        Node* current = head;
        while (current != nullptr) {
            std::sort(current->meanings.begin(), current->meanings.end()); // Sort meanings alphabetically
            auto last = std::unique(current->meanings.begin(), current->meanings.end()); // Remove duplicates
            current->meanings.erase(last, current->meanings.end()); // Erase removed duplicates
            current = current->next;
        }
    }
    void saveToFile(const std::string& filename) {
        std::ofstream outputFile(filename, std::ios::trunc);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Couldn't open the file for writing." << std::endl;
            return;
        }
        sortWords();
        Node* current = head;
        while (current != nullptr) {
            // Write the word to the file
            outputFile << current->word << ":";

            // Write the meanings to the file
            for (size_t i = 0; i < current->meanings.size(); ++i) {
                outputFile << current->meanings[i];
                if (i != current->meanings.size() - 1)
                    outputFile << ",";
            }

            outputFile << std::endl; // End the line after writing all meanings for the word

            current = current->next; // Move to the next node
        }
        outputFile.close(); // Close the file after writing
    }
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
private:
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }

};

int main() {
    Dictionary dict;
    //std::ofstream file("dictionary.txt");

    dict.readFromFile("dictionary.txt");
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
    dict.sortWords();
    dict.sortMeanings();
    dict.printDictionary();
    dict.saveToFile("dictionary.txt"); // Save to file
    std::cout << "Dictionary updated and saved." << std::endl;

    return 0;
}
