#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "ga.hpp"
// #include "spam/main.hpp"

// Global SMS data
std::vector<std::string> labels;
std::vector<std::string> sms;

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
void parse_contents()
{
    // Get the contents of file in a vector
	// bool result = getFileContent("sms/spam.txt", vecOfStr);
    std::ifstream file("sms/spam.txt");
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
            // std::cout << sms_text << std::endl;
            labels.push_back(first_word);
            sms.push_back(trim(sms_text));
        }
        file.close();
    }
}

int main()
{ 

    parse_contents();
    std::string word;

    for (auto i = sms.begin(); i != sms.end(); ++i)
    {
        //convert to lower case
        *i = to_lower_case(*i);

        std::istringstream iss(*i);
        
        while (iss >> word)
        {
            // remove punctuation
            word.erase (std::remove_if (word.begin (), word.end (), ispunct), word.end ());
        } 
    }

    GA new_population = GA();

    // /* Search for it inside SMS messeges.
    // Searches through every word in sms_data.*/
    int score = 0;
    std::string keyword[W];
    int index_id = 0;
    
    std::vector<std::string> results_labels;

    /* Find best population member */
    int pop_fitness = 0;
    int best_pop = 0;
    for (int i = 0; i < N; i++)
    {
        if (new_population.mPopulation[i].fitness > pop_fitness)
        {
            pop_fitness = new_population.mPopulation[i].fitness;
            best_pop = i;
        }
    }
    std::cout << "best_pop fitness" << new_population.mPopulation[best_pop].fitness << std::endl;

    // Set keyword array using the best population member.
    for (int j = 0; j < W; j++)
    {
        if (new_population.mPopulation[best_pop].features[j].on_off == ON)
        {
            /* If element is turned ON, get keyword feature */
            index_id = new_population.mPopulation[best_pop].features[j].order_id_index;
            keyword[j].assign(global_keywords[index_id]);

            std::cout<<keyword[j] << std::endl;
        }
        else
        {
            keyword[j] = ""; //Set to null 
        }
    }
    
    //Perform scoring with test data
    for (auto i = sms.begin(); i != sms.end(); ++i)
    {
        std::istringstream iss(*i);
        while (iss >> word)
        {
            for (int j = 0; j < W; j++)
            {
                if (word == keyword[j])
                {
                    score += W/4 - j/4;
                }
            }
        }
        if (score > W )
        {
            results_labels.push_back("spam");
        }
        else
        {
            results_labels.push_back("ham");
        }
        score = 0;
    }

    int correct = 0;
    
    for (int i = 0; i < 5574; ++i)
    {
        if (labels.at(i) != results_labels.at(i))
        {
            //nothing
        }
        else
        {
            correct++;
        }
    }

    float percent_result = float(correct)/5574;

    std::cout<<"Result: "<<percent_result << std::endl;
 
    return 0;
}


