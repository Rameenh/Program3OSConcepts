/*
 * Using a Poisson distribution with λ=10, generate 1,000 page numbers per each of 1,000 experiments. For each experiment calculate
 * the number of faults using working set sizes ranging from 2 to 20 as determined by the LRU, FIFO, and Clock algorithms.
 * using an external program, plot the average number of faults for each working set for each algorithm (the plot is to
 * be included in the report).Make certain that your report identifies any special library requirements, if necessary to
 * support the Poisson distribution. Reference Figures 8.14 & 8.16.

*/

#include <iostream>
#include <random>
#include <chrono>


using namespace std;

int LRU(int wss, /*page trace*/){

}

/*
 *
 */
void generatePoissonPageFaults(){
    int lambda = 10;

}

int main() {
    for (int experiments = 0; experiments < 1000; experiments++) { //we are trying to run 1,000 experiments

        for (int pageNumGen = 0; pageNumGen < 1000; pageNumGen++) { //we are trying to generate 1,000 numbers

            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine generator (seed);
            poisson_distribution<int> distribution (10);

            int pageNumbers [1000] = {};

            for (int i=0; i<1000; ++i){
                pageNumbers[i] = distribution(generator);
            }
        }

        for (int wss = 2; wss <= 20; wss++) {
            LRUFaults[wss] += LRU(wss, pageTrace);
            FIFOFaults[wss] += FIFO(wss, pageTrace);
            ClockFaults[wss] += Clock(wss, pageTrace);
        }

        //output Fault arrays
    }

    return 0;
}


