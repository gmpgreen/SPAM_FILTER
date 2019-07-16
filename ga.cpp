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
std::string keywords[] = {"complimentary", "free", "cash", "prize", "urgent", 
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

