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
#include <queue>

using namespace std;

bool queueContains(queue<int> a, int b){
    for(int i=a.size(); i>0; i--){
        if(a.front() == b){
            return true;
        }
        a.pop();
    }
    return false;
}

int queueContainsIndex(queue<int> a, int b) {
    for (int i = a.size(); i > 0; i--) {
        if (a.front() == b) {
            return i;
        }
        a.pop();
    }
    return -1;
}

int LRU(int wss, int pageNumbers[]){
    queue<int> LRUqueue;
    int counter = 0;
    int pageFaultCounter = 0;

    for(int i=0; i<1000; i++){
        if(!queueContains(LRUqueue, pageNumbers[i])){
            if(counter < wss) { //if the queue is not at max capacity
                LRUqueue.push(pageNumbers[i]);
                counter++; //this will keep track to see if the queue is at max capacity
            }
            else{ //if the queue is at max capacity
                LRUqueue.pop(); //pop that last added page (the queue class is automatically FIFO with pop()
                LRUqueue.push(pageNumbers[i]); //push the new pageNumber in now that we have the oldest page removed
                pageFaultCounter++;
            }
        }
        else{
            int pageNumIndex = LRUqueue.size() - queueContainsIndex(LRUqueue, pageNumbers[i]);
            vector<int> queueTempFH;
            vector<int> queueTempSH;

            for (int i = 0; i < pageNumIndex; i++) {
                queueTempFH.push_back(LRUqueue.front());
                LRUqueue.pop();
            }
            LRUqueue.pop(); //this is the page number we're moving to the front

            int sizeForSH = LRUqueue.size();

            for (int i = 0; i < sizeForSH; i++) {
                queueTempSH.push_back(LRUqueue.front());
                LRUqueue.pop();
            }

            for (int i = 0; i < queueTempFH.size(); i++) {
                LRUqueue.push(queueTempFH.at(i));
            }

            for (int i = 0; i < queueTempSH.size(); i++) {
                LRUqueue.push(queueTempSH.at(i));
            }

            LRUqueue.push(pageNumbers[i]);
        }
        }
    return pageFaultCounter;
}


int FIFO(int wss, int pageNumbers[]){
    queue<int> FIFOqueue;
    int counter = 0;
    int pageFaultCounter = 0;

        for(int i=0; i<1000; i++){
            if(!queueContains(FIFOqueue, pageNumbers[i])){
                if(counter < wss) { //if the queue is not at max capacity
                    FIFOqueue.push(pageNumbers[i]);
                    counter++; //this will keep track to see if the queue is at max capacity
                }
                else{ //if the queue is at max capacity
                    FIFOqueue.pop(); //pop that last added page (the queue class is automatically FIFO with pop()
                    FIFOqueue.push(pageNumbers[i]); //push the new pageNumber in now that we have the oldest page removed
                    pageFaultCounter++;
                }
        }
    }
    return pageFaultCounter;
}

bool arrayContains(int test[][2], int b, int length) {
    for (int i = 0; i < length; i++) {
        if (test[i][0] == b) return true;
    }
    return false;
}

int arrayContainsWithIndex(int test[][2], int b, int length) {
    int whereIs;
    for (int i = 0; i < length; i++) {
        if (test[i][0] == b) {
            whereIs = i;
        }
    }
    return whereIs;
}

int Clock(int wss, int pageNumbers[]){ //we will have to create our own "queue" for this one
    int workingSetArray[wss][2]; //second part of two-dimensional array will be the use bit
    int counter = 0; //to keep track of when we hit out wss limit
    int pointer=0; //initialized to the first position in our queue
    int pageReferenced; //initialize a variable to store the index of a referenced page number
    int pageFaultCount = 0; //initialize the counter for page faults to zero
    for(int i=0; i<1000; i++){
        if(!arrayContains(workingSetArray, pageNumbers[i], wss)){ //if our working set does not contain this incoming page number
            if(counter < wss){ //as long as there is an empty slot in the working set, no need to do any page replacement, just tack it on the end
                workingSetArray[pointer][0]=pageNumbers[i];
                workingSetArray[pointer][1]=1; //the zero indicates that the page is given its first chance
                pointer++; //we have to manually keep track of where we are in the working set page queue, move it up one; we don't need to check to see
                // if the pointer will exceed the working set size because our condition to get into this coding section already prevents that
                counter++; //we just added an element to our working set; moves in lockstep with the pointer variable in this section of code
            }
            else{ //this means that our working set has reached max capacity and we must start swapping out pages
                while(workingSetArray[pointer][1] != 0) { //keep doing this loop until we encounter a page that is replaceable
                    workingSetArray[pointer][1] = 0;//set the current position to replaceable
                    if(pointer==wss) //if the pointer is pointing to the last slot in the working set
                        pointer = 0; //set the pointer to point to slot zero
                    else //otherwise
                        pointer++; //we have to manually keep track of where we are in the working set page queue, move it up one
                }
                //at this point in the code, the pointer MUST be pointing to a page index where is it replaceable, it
                // may have not ran at all, in which case the program simply started in a replaceable page
                workingSetArray[pointer][0]=pageNumbers[i]; //the page has now been replaced
                workingSetArray[pointer][1]=1; //set it to newly place so it's not chosen for replacement
                pageFaultCount++; //increment the page fault counter
            }
        }
        else //if the working set does include this incoming page number already, pointer nor counter increment
        {
            pageReferenced = arrayContainsWithIndex(workingSetArray, pageNumbers[i], wss); //locate the specific position of where that page is located in the working set
            workingSetArray[pageReferenced][1]=1; //a rule of the clock algorithm is that if a page is referenced in the working set, even if the use bit is 0, set it back to 1
        }
    }
    return pageFaultCount;
}

int main() {
    int LRUFaults[20] = {}; //we will keep the faults for FIFO, clock and LRU algorithms in these arrays for every
    int FIFOFaults[20] = {}; //working set and calculate their average in the end
    int ClockFaults[20] = {};

    for (int experiments = 0; experiments < 1000; experiments++) { //we are trying to run 1,000 experiments

        int pageNumbers [1000] = {}; //initializing array of page number/address stream as empty; structuring the command like this should reset the data with every iteration of the loop

        for (int pageNumGen = 0; pageNumGen < 1000; pageNumGen++) { //we are trying to generate 1,000 numbers

            unsigned seed = chrono::system_clock::now().time_since_epoch().count(); //creating the seed for our random generator
            default_random_engine generator (seed); //setting the seed for our random generator
            poisson_distribution<int> distribution (10); //declaring pre-defined poisson distribution random number generator with lambda = 10 as parameter

            for (int i=0; i<1000; ++i){ //loop for all 1,000 page numbers we need
                pageNumbers[i] = distribution(generator); //generate a random number that fits the poisson distribution with lambda = 10 and assign it to index in page number array
            }
        }

        for (int wss = 2; wss <= 20; wss++) { //wss = working set size; we are working with 2 to 20 working set size; set loop for such
            LRUFaults[wss] += LRU(wss, pageNumbers); //will import the total number of page faults for the LRU algorithm for each working set size and cumulatively add them up
            FIFOFaults[wss] += FIFO(wss, pageNumbers); //same as above but for the FIFO algorithm
            ClockFaults[wss] += Clock(wss, pageNumbers); //same as above but for the Clock algorithm
        }

        for (int wss = 2; wss <= 20; wss++) { //wss = working set size; we are working with 2 to 20 working set size; set loop for such
            LRUFaults[wss] /= 1000; //will calculate the average, the last loop added all the values up so all we have to do is divide each slot by 1000 since we added up 1000's worth of numbers
            FIFOFaults[wss] /= 1000; //same as above but for the FIFO algorithm
            ClockFaults[wss] /= 1000; //same as above but for the Clock algorithm
        }

        cout << "Average number of LRU page faults for working set size 2-20: ";
        for (int wss = 2; wss <= 20; wss++)   //wss = working set size; we are working with 2 to 20 working set size; set loop for such
            cout << LRUFaults[wss] << " ";
        cout << endl;

        cout << "Average number of FIFO page faults for working set size 2-20: ";
        for (int wss = 2; wss <= 20; wss++)  //wss = working set size; we are working with 2 to 20 working set size; set loop for such
            cout << FIFOFaults[wss] << " ";
        cout << endl;

        cout << "Average number of Clock page faults for working set size 2-20: ";
        for (int wss = 2; wss <= 20; wss++) //wss = working set size; we are working with 2 to 20 working set size; set loop for such
            cout << ClockFaults[wss] << " ";
        cout << endl;
    }
    return 0;
}


