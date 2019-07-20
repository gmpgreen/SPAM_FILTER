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
    /* Initialization of population */
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

        std::cout << "inital generation" <<std::endl;
        std::cout << "fitness: " << mPopulation[i].fitness << std::endl;
    }
    
    
    for (int k = 0; k < NUM_GENERATIONS; k++)
    {
        std::cout << "generation " << k+1 << ":" << std::endl;

        /* Crossover */
        for (int i = 0; i < N; i+=2)
        {
            Crossover(Selection(), Selection(), i, i + 1);
        }
        for (int i = 0; i < N; i++)
        {
            mPopulation[i] = mPopulationTemp[i];
        }

        Mutation();
        Inversion();
        fitness_func_wrapper();

        for (int i = 0; i < N; i++)
        {
            std::cout << "fitness: " << mPopulation[i].fitness << std::endl;
        }
    }
}

void GA::Mutation()
{
    int random_index = rand() % (W - 1);
    
    for (int i = 0; i < N; i++)
    {
        if (rand()%2) //50% of the time, flip a random bit
        {
            mPopulation[N].features[random_index].on_off = !(mPopulation[N].features[random_index].on_off);
        }
    }
}

void GA::Inversion()
{
    for (int i = 0; i < N; i++)
    {
        int inv_start = rand()%16;
        int inv_end = rand()%16;

        if (inv_start < inv_end)
        {
            population temp = mPopulation[i];
            
            for (int j = inv_start; j < inv_end; j++)
            {
                temp.features[j] = mPopulation[i].features[inv_start - j + inv_end];
            }
            mPopulation[i] = temp;
        }
    }
}

/* 
 * Param In: index_p1 parent index 1
 *           index_p2 parent index 2
 *           index_c1 child index 1
 *           index_c2 child index 2  
*/
void GA::Crossover(int index_p1, int index_p2, int index_c1, int index_c2)
{
    population pop1;
    population pop2;

    pop1.fitness = 0; //Init to zero
    pop2.fitness = 0;

    int half_chromosome  = W/2;
    
    for (int j = 0; j < W; j++)
    {
        if (j < half_chromosome)
        {
            //1st half of chromosome 1 stays in 1st half of new chromosome 1
            pop1.features[j].order_id_index = mPopulation[index_p1].features[j].order_id_index;
            pop1.features[j].on_off = mPopulation[index_p1].features[j].on_off;

            //2nd half of chromosome 1 goes in 1st half of new chromosome 2
            pop2.features[j].order_id_index = mPopulation[index_p1].features[j + half_chromosome].order_id_index;
            pop2.features[j].on_off = mPopulation[index_p1].features[j + half_chromosome].on_off;
        }
        else
        {
            //1st half of chromosome 2 goes in 2nd half of new chromosome 1
            pop1.features[j].order_id_index = mPopulation[index_p2].features[j - half_chromosome].order_id_index;
            pop1.features[j].on_off = mPopulation[index_p2].features[j - half_chromosome].on_off;

            //2nd half of chromosome 2 stays in 2nd half of new chromosome 2
            pop2.features[j].order_id_index = mPopulation[index_p2].features[j].order_id_index;
            pop2.features[j].on_off = mPopulation[index_p2].features[j].on_off;
        }
    }
    mPopulationTemp[index_c1] = pop1;
    mPopulationTemp[index_c2] = pop2;
}


/* Selection Method:
 * 1) Calculate the sum of all score
 * 2) pick a random number that is between 0 and sum
 * 3) iterate through, subtracting current score from sum until
 *    the random number is less than current score
 * Returns: index reprsenting parent selected
 */
int GA::Selection()
{
    int sum_fitness = 0;
    int random;
    int index;

    for (int i = 0; i < N; i++)
    {
        sum_fitness += mPopulation[i].fitness;
    }
    random = rand() % sum_fitness;

    index = 0;
    while (random >= mPopulation[index].fitness)
    {
        random -= mPopulation[index].fitness;
        index++;
    }

    std::cout <<"index"<<index<<std::endl;
    return index;
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
                        else
                        {
                            score--;  // Evens out words commonly used in all texts
                        }
                    }
                }
            }
            k++;
        }

    return (score);
}

void GA::fitness_func_wrapper()
{
    for (int i = 0; i < N; i++)
    {
        /* For each population member, eval fitness */
        mPopulation[i].fitness = Fitness_func(mPopulation[i], mData);
        

        std::cout << "fitness: " << mPopulation[i].fitness << std::endl;
    }
}
