#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "ga.hpp"
// #include "spam/main.hpp"

int fitness_func(population pop, sms_data data);

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
sms_data parse_contents()
{
    sms_data data;

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
            data.labels.push_back(first_word);
            data.sms.push_back(trim(sms_text));
        }
        file.close();
    }
    return data;
}

int main()
{ 

    sms_data data = parse_contents();

    // output labels
    // for (auto i = data.labels.begin(); i != data.labels.end(); ++i)
    // {
    //     std::cout << *i << '\n';    
    // }

    // output sms
    // for (auto i = data.sms.begin(); i != data.sms.end(); ++i)
    // {
    //     std::cout << *i << '\n';
    // }

    //convert to lower case
    for (auto i = data.sms.begin(); i != data.sms.end(); ++i)
    {
        *i = to_lower_case(*i); 
    }

    population pop[N];
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < W; j++)
        {
            // Setting initial population
            pop[i].features[j].order_id_index = j;
            pop[i].features[j].on_off = rand() % 2;
        }

        /* For each population member, eval fitness */
        pop[i].fitness = fitness_func(pop[i], data);

        std::cout << "fitness: " << pop[i].fitness << std::endl;
    }
 
    return 0;
}

/*Param In: Population pop, data structure representing population of current generation
            sms_data data, contains the two vectors of sms test data
  Returns: int score, the total number of hits all keywords have in spam messages. (Consider normalizing score)          
 */
int fitness_func(population pop, sms_data data)
{
        std::string keyword[W];
        int index_id = 0; 
        int score = 0;

        for (int j = 0; j < W; j++)
        {
            if (pop.features[j].on_off == ON)
            {
                /* If element is turned ON, get keyword feature */
                index_id = pop.features[j].order_id_index;
                keyword[j].assign(keywords[index_id]);
            }
            else
            {
                keyword[j] = ""; //Set to null 
            }
        }
        
        /* Search for it inside SMS messeges.
        Searches through every word in sms_data.*/
        int k = 0;
        for (auto i = data.sms.begin(); i != data.sms.end(); ++i)
        {
            for (int j = 0; j < W; j++)
            {
                std::istringstream iss(*i);
                std::string word;
                while (iss >> word)
                {
                    if (word == keyword[j])
                    {
                        if (data.labels.at(k) == "spam")
                        {
                            score++;
                        }
                    }
                }
            }
            k++;
        }

    return score;
}