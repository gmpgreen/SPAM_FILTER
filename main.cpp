#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

// #include "spam/main.hpp"
std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
// https://stackoverflow.com/questions/15347123/how-to-construct-a-stdstring-from-a-stdvectorstring/18703743
int main()
{
	std::vector<std::string> labels;
    std::vector<std::string> sms;
 
	// Get the contents of file in a vector
	// bool result = getFileContent("sms/spam.txt", vecOfStr);
    std::ifstream file("sms/spam_small.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            const char* char_line = line.c_str();
            std::string text_line = char_line;

            // https://stackoverflow.com/questions/17135452/how-to-remove-first-word-from-a-string?lq=1
            std::string first_word;
            std::istringstream iss{text_line};
            iss >> first_word;
            std::ostringstream oss;
            oss << iss.rdbuf();
            std::string sms_text = oss.str();
            std::cout << sms_text << std::endl;
            labels.push_back(first_word);
            sms.push_back(trim(sms_text));
        }
        file.close();
    }
    // output labels
    for (auto i = labels.begin(); i != labels.end(); ++i)
        std::cout << *i << '\n';

    // output sms
    for (auto i = sms.begin(); i != sms.end(); ++i)
        std::cout << *i << '\n';
    
    // Updating content
    // each vector contains a vector (numbers for the feature)
    std::vector<std::vector<int>> features;
    
    // iterate across sms vector and extract features
    // for a proof of concept we can use bad features like number of exclaimation marks, question marks, length of string and how many spammy words they have.
   std::regex word_regex("(\\S+)");
   // see https://en.cppreference.com/w/cpp/regex
   std::cout << "Trying out other regex" << std::endl;
   std::regex emarks_regex("[!]");
   std::cout << "Q mark down";
   std::regex qmarks_regex("[\?]");

   // http://www.cplusplus.com/reference/string/string/find/
   for (auto i = sms.begin(); i != sms.end(); ++i)
   {
     auto sms_string = *i;
     auto words_begin = std::sregex_iterator(sms_string.begin(), sms_string.end(), word_regex);
     auto words_end = std::sregex_iterator();
  std::cout << "Found" << std::distance(words_begin, words_end) << "words\n";
     
     auto emarks_begin = std::sregex_iterator(sms_string.begin(), sms_string.end(), emarks_regex);
   std::cout << "Found" << std::distance(emarks_begin, words_end) << "!\n";
   } 
    return 0;
}
