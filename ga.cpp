#include "ga.hpp"

/**************************************************************
 * Constant data for keyword features. Can improve later with dictionary function.
 * All test data will be converted to lower case in pre-processing step for ease of search.
 * 
 * GA Algorithm: 
 * Initialize population with N = 16
 * Each 'chromosone' will 32 bits wide
 * Each member of the population has chromosone initialzed randomly, 0 or 1
 * 
 * Fitness Function will generate a score that is based on the following criteria:
 *  -The number of matches in a text message with a chromosome
 *  -The weighting of the matching chromosomes
 *  -Small score penalty for words that occur too commonly in all messages
 *  -A score penalty for chromosomes that have very little diversity (ie < 8)
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
    "extra", "gift", "money", "win",
    "income", "please", "credit", "sex", "hello", "collect", "now", "luxury", "instant",
    "important", "notification", "you", "your", "congratulations", "congrats", "nude"
};

int64_t global_random_helper = 0;

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

/* Param: x: lower bound
 *        y: upper bound, must be > x  
 * Returns: random number between bounds x and y */
int random(int x, int y)
{

    // construct a trivial random generator engine from a time-based seed:

    int64_t seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    global_random_helper %= 10;
    seed += global_random_helper;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(x,y);
    
    global_random_helper++;

    int RetVal = reinterpret_cast<int>(dis(gen));

    //std::cout<<"random"<<RetVal<<std::endl;
    return RetVal;
}

/* CLASS DEFINITIONS*/

/* 
Constructor.
Param In: sms_data data, contains the two vectors of sms test data in lower case
*/
GA::GA()
{
    /* Initialization of population */
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < W; j++)
        {
            // Setting initial population
            mPopulation[i].features[j].order_id_index = j;
            mPopulation[i].features[j].on_off = random(0, 1);
        }

        /* For each population member, eval fitness */
        mPopulation[i].fitness = Fitness_func(mPopulation[i]);

        std::cout << "inital generation" <<std::endl;
        std::cout << "fitness: " << mPopulation[i].fitness << std::endl;
    }
    
    
    for (int k = 0; k < MAX_GENERATIONS; k++)
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

        if ( (k != 0) && (!(k % 10)) ) // Check number of unique remaining chromosomes every 10 iterations
        {
            if (!Remaining())
            {
                std::cout << "Number of iterations: " << k+1 << std::endl;
                break;
            }
        }
    }
}

void GA::Mutation()
{
    int random_index = random(0, (W - 1));
    
    for (int i = 0; i < N; i++)
    {
        if (random(0, 1)) //50% of the time, flip a random bit
        {
            mPopulation[N].features[random_index].on_off = !(mPopulation[N].features[random_index].on_off);
        }
    }
}

void GA::Inversion()
{
    for (int i = 0; i < N; i++)
    {
        int inv_start = random(0, (W - 1));
        int inv_end = random(0, (W - 1));

        if (inv_start < inv_end)
        {
            population temp = mPopulation[i];
            
            for (int j = 0; j <= (inv_end - inv_start); j++)
            {
                temp.features[inv_start + j] = mPopulation[i].features[inv_end - j];
            }

            for (int j = 0; j <= (inv_end - inv_start); j++)
            {
                mPopulation[i].features[j] = temp.features[j];
            }
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

    if (index >= N) {index = N - 1;} // Protect against out of bounds (should never happen)

    return index;
}

bool GA::Remaining()
{
    bool RetVal = true;

    /* Find best population member */
    int pop_fitness = 0;
    int best_pop = 0;
    for (int i = 0; i < N; i++)
    {
        if (mPopulation[i].fitness > pop_fitness)
        {
            pop_fitness = mPopulation[i].fitness;
            best_pop = i;
        }
    }

    int count = 0;
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < W; j++)
        {
           if ((mPopulation[best_pop].features[j].order_id_index == i) 
           && mPopulation[best_pop].features[j].on_off == 1)
           {
               count++;
               break;
           }
        }
    }

    if (count < 12) // Genetic Algorithm Stop Criteria
    {
        std::cout <<"chromosomes left "<< count << std::endl;
        RetVal = false;
    }

    return RetVal;
}

/*Param In: Population pop, data structure representing population of current generation
            sms_data data, contains the two vectors of sms test data
  Returns: int score, the total number of hits all keywords have in spam messages. (Consider normalizing score)          
 */
int GA::Fitness_func(population pop)
{
    int score = 0;
    int ReturnVal = 0;
    
    std::string keyword[W];
    int index_id = 0;

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
            //Perform scoring with test data
    
    int k = 0;
    for (auto i = sms.begin(); i != sms.end(); ++i)
    {
        std::istringstream iss(*i);
        std::string word;
        while (iss >> word)
        {
            for (int j = 0; j < W; j++)
            {
                if (word == keyword[j])
                {
                    score += W/8 - j/8;
                }
            }
        }
        if (score >= (W/5)) // Threshold, can be tuned to other value. 
        {
            // std::cout<<"spam"<<std::endl;
            if (labels.at(k) == "spam")
            {
                ReturnVal += 6;
            }
            else
            {
                --ReturnVal;
            }
        }
        else
        {
            // std::cout<<"notspam"<<std::endl;
            if (labels.at(k) == "spam")
            {
                --ReturnVal; // Missed a spam message, reduce score
            }
        }

        int unique_chromo = 0;
        for (int i = 0; i < W; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if ((pop.features[j].order_id_index == i) 
                && pop.features[j].on_off == 1)
                {
                    unique_chromo++;
                    break;
                }
            }
        }

        if (unique_chromo < 8)
        {
            // Penalize chromosomes with little diversity
            ReturnVal /= 2;
        }


        score = 0; //reset score for next SMS
        k++;
    }

    if (ReturnVal <= 0)
    {
        ReturnVal = 1;
    }

    return ReturnVal;
}

void GA::fitness_func_wrapper()
{
    
    for (int i = 0; i < N; i++)
    {
        /* For each population member, eval fitness */
        mPopulation[i].fitness = Fitness_func(mPopulation[i]);

        std::cout << "fitness: " << mPopulation[i].fitness << std::endl;
    }
}
