#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#ifndef GA_HPP
#define GA_HPP

#define NUM_GENERATIONS 4
#define ON  1
#define OFF 0
#define N   8       // Population size
#define W   16      // Width of chromosome (number of features)
//#define MAX_SPAM 747 //Total number of spam messages in training data

/* CONSTANT DATA: Keywords to use as features. */
extern std::string global_keywords[];

/* DATA STRUCTURES */
struct sms_data
{
	std::vector<std::string> labels;
    std::vector<std::string> sms;
};

struct chromo
{
    bool     on_off;
    int order_id_index; // Index from 0 to 15 for keyword data. Needed for inversion operations.
    //int fitness_score;
};

struct population
{
    uint16_t fitness;
    chromo features[16]; 
};

/* Param: string that may or may not be lower case
 * Returns: string that is all lower case */
std::string to_lower_case(std::string str_in);

class GA
{
    public:    

    GA(sms_data data);

    population mPopulation[N];
    sms_data mData;

    private:
    
    int Fitness_func(population pop, sms_data data);
    void fitness_func_wrapper();
    void Crossover(int index_p1, int index_p2, int index_c1, int index_c2);
    int Selection();
    void Mutation();
    void Inversion();
    
    population mPopulationTemp[N];

};

#endif