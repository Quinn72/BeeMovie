#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <thread>


// Callback function to write retrieved data to string
size_t writeCallback(char* buf, size_t size, size_t nmemb, std::string* data) {
    data->append(buf, size * nmemb);
    return size * nmemb;
}

std::string getBeeMovieScript() {
    std::string script;
    std::string line;

    // Initialize libcurl
    std::ifstream myfile;
	myfile.open("beemovescript.txt");

    if (!myfile.is_open())
    {
        std::cout << "Failed to open file." << std::endl;
        return "";
    }

    while (std::getline(myfile, line))
    {
        script += line + "\n";
    }

    myfile.close();
    return script;
}

void printScript(const std::string& script, int pace) {
    // Print the script character by character
    while (true) {
        for (char c : script) {
            std::cout << c << std::flush; // Print character without buffering
            std::this_thread::sleep_for(std::chrono::milliseconds(pace)); // Adjust pace here (in milliseconds)
        }
    }
}

int main() {
    std::string beeMovieScript = getBeeMovieScript();
    if (!beeMovieScript.empty()) {
        printScript(beeMovieScript, 50); // Adjust pace here (in milliseconds)
    }
    else {
        std::cerr << "Failed to retrieve Bee Movie script." << std::endl;
        return 1;
    }
    return 0;
}
