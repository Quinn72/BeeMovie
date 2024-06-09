#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>
#include <unordered_map>
#include <regex>
#include <sstream>

size_t writeCallback(char* buf, size_t size, size_t nmemb, std::string* data) {
    data->append(buf, size * nmemb);
    return size * nmemb;
}

std::string getBeeMovieScript() {
    std::string script;
    std::string line;

    std::ifstream myfile;
    myfile.open("beemovescript.txt");

    if (!myfile.is_open()) {
        std::cout << "Failed to open file." << std::endl;
        return "";
    }

    while (std::getline(myfile, line)) {
        script += line + "\n";
    }

    myfile.close();
    return script;
}

void printColoredText(const std::string& text, const std::string& colorCode) {
    std::cout << colorCode << text << "\033[0m"; // Reset color after the text
}

template <typename Iterator>
std::string join(Iterator begin, Iterator end, const std::string& separator) {
    std::ostringstream result;
    if (begin != end) {
        result << *begin++;
    }
    while (begin != end) {
        result << separator << *begin++;
    }
    return result.str();
}

void printScript(const std::string& script, int pace, const std::unordered_map<std::string, std::string>& colorMap) {
    // Create a regex to match any of the character names
    std::vector<std::string> characterNames;
    for (const auto& pair : colorMap) {
        characterNames.push_back(pair.first);
    }

    std::ostringstream regexPattern;
    regexPattern << "\\b(";
    for (size_t i = 0; i < characterNames.size(); ++i) {
        if (i > 0) {
            regexPattern << "|";
        }
        regexPattern << std::regex_replace(characterNames[i], std::regex("([.^$|()\\[\\]{}*+?\\\\])"), "\\$&");
    }
    regexPattern << ")\\b";
    std::regex re(regexPattern.str());

    std::string word;
    for (char c : script) {
        if (isspace(c) || ispunct(c)) {
            if (std::regex_search(word, re)) {
                for (const auto& pair : colorMap) {
                    if (word == pair.first) {
                        printColoredText(word, pair.second);
                        break;
                    }
                }
            }
            else {
                std::cout << word;
            }
            std::cout << c;
            word.clear();
        }
        else {
            word += c;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(pace)); // Adjust pace here (in milliseconds)
    }
    if (std::regex_search(word, re)) {
        for (const auto& pair : colorMap) {
            if (word == pair.first) {
                printColoredText(word, pair.second);
                break;
            }
        }
    }
    else {
        std::cout << word;
    }
}

int main() {
    std::string beeMovieScript = getBeeMovieScript();
    if (!beeMovieScript.empty()) {
        std::unordered_map<std::string, std::string> colorMap = {
            {"NARRATOR", "\033[1;31m"},  // Red
            {"BARRY BENSON", "\033[1;32m"},  // Green
            {"JANET BENSON", "\033[1;33m"},  // Yellow
            {"BARRY", "\033[1;34m"},  // Blue
            {"ADAM FLAYMAN", "\033[1;35m"},  // Magenta
            {"ADAM", "\033[1;36m"},  // Cyan
            {"MARTIN", "\033[1;91m"},  // Bright Red
            {"VANESSA", "\033[1;92m"}  // Bright Green
        };
        printScript(beeMovieScript, 50, colorMap); // Adjust pace here (in milliseconds)
    }
    else {
        std::cerr << "Failed to retrieve Bee Movie script." << std::endl;
        return 1;
    }
    return 0;
}
