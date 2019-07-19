#include "ga.hpp"

/**************************************************************
 * Constant data for keyword features. Can improve later with dictionary function.
 * All test data will be converted to lower case in pre-processing step for ease of search.
 * 
 * GA Algorithm: 
 * Initialize population with N = 8
 * Each 'chromosone' will 16 bits wide
 * Each member of the population has chromosone initialzed randomly, 0 or 1
 * 
 * For now, Fitness Function will randomly test chromosone against a
 * subset of messages (ie 200) and will return the number of hits it gets from spam  messages.
 * 
 * There will be N/2 crossover operations, using the fitness function to determine probablity of parent.
 * 
 * To discuss: oder the chromosones according to which are weighted heavier.
 * Would then use Inversion (invert the order of chromosone from random start and end points)
 * 
 * ***********************************************************/

/* CONSTANT DATA: Keywords to use as features. Could later be improved with dictionary function 
 * that searches texts for most commonly used words.
 */
std::string global_keywords[] = {"complimentary", "free", "cash", "prize", "urgent", 
    "act", "affordable", "apply", "beneficiary", "call", "click", "debt", 
    "extra", "gift", "money", "win"
};

/* HELPER FUNCTIONS*/

/* Param: string that may or may not be lower case
 * Returns: string that is all lower case */
std::string to_lower_case(std::string str_in)
{
    for (auto i = str_in.begin(); i < str_in.end(); i++)
    {
        *i = std::tolower(*i);
    }
    return str_in;
}

/* CLASS DEFINITIONS*/

/* 
Constructor.
Param In: sms_data data, contains the two vectors of sms test data in lower case
*/
GA::GA(sms_data data)
{
    mData = data;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < W; j++)
        {
            // Setting initial population
            mPopulation[i].features[j].order_id_index = j;
            mPopulation[i].features[j].on_off = rand() % 2;
        }

        /* For each population member, eval fitness */
        mPopulation[i].fitness = Fitness_func(mPopulation[i], data);

        std::cout << "fitness: " << mPopulation[i].fitness << std::endl;
    }
}

/*Param In: Population pop, data structure representing population of current generation
            sms_data data, contains the two vectors of sms test data
  Returns: int score, the total number of hits all keywords have in spam messages. (Consider normalizing score)          
 */
int GA::Fitness_func(population pop, sms_data data)
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
                keyword[j].assign(global_keywords[index_id]);
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
                            score += W - j; // Index 0 of keyword is weighted at 15, Index 15 is weighted to 1
                        }
                    }
                }
            }
            k++;
        }

    return (score/W);
}