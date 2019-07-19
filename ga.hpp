#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#ifndef GA_HPP
#define GA_HPP

#define ON  1
#define OFF 0
#define N   8       // Population size
#define W   16      // Width of chromosome (number of features)

/* CONSTANT DATA: Keywords to use as features. */
extern std::string keywords[];

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
    int fitness_score;
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
    population mPopulation[N];

    GA(sms_data data);
    int Fitness_func(population pop, sms_data data);

    private:
    sms_data mData;

};

#endif