/***************************************************************************
* File: main.cpp
* Author: Rameen Housseini
* Modification History: April 18, 20, 22, 23, 24 2020, every time by Rameen Housseini
* Procedures:
* main - generates random pages numbers and calculates the average number of
*        page faults for FIFO LRU and clock replacement algorithm in 1000 experiments
* queueContains - tells whether or not a queue contains a number
* queueContainsIndex - returns the index of a number within a queue
* LRU - the least recently used page replacement algorithm for memory
* FIFO - The first in first out page replacement algorithm for memory
* arrayContains - steps through a 2D array and determines if a given element
*                 exists within it
* arrayContainsWithIndex - looks through a 2D array and gives us the index of an
*                          element
* Clock - The clock or second chance page replacement algorithm for memory
***************************************************************************/

#include <iostream>
#include <random>
#include <chrono>
#include <queue>

using namespace std;


/***************************************************************************
* bool queueContains(queue<int> a, int b)
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: Returns whether or not an element exists in a queue
* Parameters:
* a             I/P queue<int> The queue that the function is searching through
* b             I/P int        The element the function is looking for in the queue
* queueContains O/P bool       Whether or not the queue contains integer b
**************************************************************************/
bool queueContains(queue<int> a, int b){
    for(int i=a.size(); i>0; i--){       //spans the entire queue
        if(a.front() == b){              //checks to see if the current front of the queue matches the passed element
            return true;                 //if so, return true
        }
        a.pop();                         //otherwise, pop and try again
    }
    return false;                        //at this point we've looked through the whole queue, return false
}

/***************************************************************************
* int queueContainsIndex(queue<int> a, int b)
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: Looks through a queue and return the index of an element if
* the function finds it. Will always return a valid value due to being called
* only if queueContains is true in the driver function
* Parameters:
* a                  I/P queue<int> The queue the function is searching through
* b                  I/P int        The element the function is looking for in the queue
* queueContainsIndex O/P int        The index at which element b lies in the queue
**************************************************************************/
int queueContainsIndex(queue<int> a, int b) {
    for (int i = a.size(); i > 0; i--) {      //spans the whole queue
        if (a.front() == b) {                 //checks to see if the front of the queue matches the passed element
            return i;                         //if found, return the index
        }
        a.pop();                              //otherwise, pop and try again
    }
    return -1;                                //at this point we've looked through the whole queue, return -1
}

/***************************************************************************
* int LRU(int wss, int pageNumbers[])
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: the least recently used page replacement algorithm, similar to
* first in first out except if a page is referenced, it gets moved to the back
* of the queue/stack so it doesn't get replaced next/sooner
* Parameters:
* wss          I/P  int   The size of the working set
* pageNumbers  I/P  int[] An array containing all of the page numbers
* LRU          O/P  int   The number of page faults that occurred from the algorithm
**************************************************************************/
int LRU(int wss, int pageNumbers[]){
    queue<int> LRUqueue;                 //the queue where we will store the pages held in memory for our working set
    int counter = 0;                     //a variable to tell when the working set queue is full
    int pageFaultCounter = 0;            //a variable to keep track of the number of page faults

    for(int i=0; i<1000; i++){                        //spans the 1000 page numbers in the page number array
        if(!queueContains(LRUqueue, pageNumbers[i])){ //if the queue does not contain the incoming page number
            if(counter < wss) {                       //and if the queue is not at max capacity
                LRUqueue.push(pageNumbers[i]);        //push the page number in the queue
                counter++;                            //this will keep track to see if the queue is at max capacity
            }
            else{                                     //otherwise, if the queue is at max capacity
                LRUqueue.pop();                //pop that last added page (the queue  is automatically FIFO with pop())
                LRUqueue.push(pageNumbers[i]); //push the new pageNumber in now that we have the oldest page removed
                pageFaultCounter++;            //increment the page fault counter since page fault occurred
            }
        }
        else{ //in this case, the queue already contains the page number, we will
              // have to perform a move advanced maneuver to move it to the back
            int pageNumIndex = LRUqueue.size() - queueContainsIndex(LRUqueue, pageNumbers[i]); //find out that index the
                                                                                               // pre-existing page
                                                                                               // number is at and store
                                                                                               // it in a variable
            vector<int> queueTempFH;  //create an int vector for all of the page numbers before pre-existing page number
            vector<int> queueTempSH;  //and for all the ones after the pre-existing page number

            for (int i = 0; i < pageNumIndex; i++) {      //from the beginning to just before the pre-existing page
                queueTempFH.push_back(LRUqueue.front());  //add the page number from the queue to our first half vector
                LRUqueue.pop();                           //and pop each page number after adding
            }
            LRUqueue.pop();                               //this is the page number we're moving to the front, pop it

            int sizeForSH = LRUqueue.size();              // there's a weird glitch where I have to store this in a
                                                          // variable and pass it to the for loop below to it to work
                                                          // for some reason

            for (int i = 0; i < sizeForSH; i++) {         // at this point, the queue contains all of the numbers to the
                                                          // right of the pre-existing page number
                queueTempSH.push_back(LRUqueue.front());  //repeat the process we did for the first half
                LRUqueue.pop();
            }

            for (int i = 0; i < queueTempFH.size(); i++) { //reassemble the queue by re-adding the first half
                LRUqueue.push(queueTempFH.at(i));
            }

            for (int i = 0; i < queueTempSH.size(); i++) { //then the second half
                LRUqueue.push(queueTempSH.at(i));
            }

            LRUqueue.push(pageNumbers[i]);                 //then the pre-existing page number, and we're done, page
                                                           // number is now in the back of the queue where it should be
        }
    }
    return pageFaultCounter;                               //return the number of page faults that occurred
}

/***************************************************************************
* int FIFO(int wss, int pageNumbers[])
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: The first in first out page replacement algorithm, the simplest
* one where the first page to go in will be the first to leave when it comes
* time for replacement and the pattern continues.
* Parameters:
* wss          I/P  int    The size of the working set
* pageNumbers  I/P  int[]  An array containing all of the page numbers
* FIFO         O/P  int    The number of page faults that occurred from the algorithm
**************************************************************************/
int FIFO(int wss, int pageNumbers[]){
    queue<int> FIFOqueue;
    int counter = 0;
    int pageFaultCounter = 0;

        for(int i=0; i<1000; i++){
            if(!queueContains(FIFOqueue, pageNumbers[i])){
                if(counter < wss) {                        // if the queue is not at max capacity
                    FIFOqueue.push(pageNumbers[i]);
                    counter++;                             //this will keep track to see if the queue is at max capacity
                }
                else{                                      // if the queue is at max capacity
                    FIFOqueue.pop();                       // pop that last added page (the queue
                                                           // class is automatically FIFO with pop()
                    FIFOqueue.push(pageNumbers[i]);        // push the new pageNumber in now that we
                                                           // have the oldest page removed
                    pageFaultCounter++;                    // increment the page fault counter since one occurred
                }
        }
    }
    return pageFaultCounter;                               // return the number of page faults
}

/***************************************************************************
* bool arrayContains(int test[][2], int b, int length)
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: looks through a 2D array and determines if an int exists in it
* Parameters:
* test          I/P int[][] the array the the function is searching through
* b             I/P int     the element that is being searched
* length        I/P int     the length of the test array column-wise
* arrayContains O/P bool    whether or not the array contains the passed integer b
**************************************************************************/
bool arrayContains(int test[][2], int b, int length) {
    for (int i = 0; i < length; i++) {    //spanning the whole array column-wise
        if (test[i][0] == b) return true; //is the page number field of the array matches the element, return true
    }
    return false;                         //we've gone through the whole array and no match found, return false
}

/***************************************************************************
* int arrayContainsWithIndex(int test[][2], int b , int length)
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: Steps through a 2D array and if it finds the desired element,
* return the index. Will always return a value because it's only called if the
* arrayContains method is true in the driver function
* Parameters:
* test                   I/P int[][]  the array the the function is searching through
* b                      I/P int      the element that is being searched
* length                 I/P int      the length of the test array column-wise
* arrayContainsWithIndex O/P int      the index that the element was found in
**************************************************************************/
int arrayContainsWithIndex(int test[][2], int b, int length) {
    int whereIs;                       //variable to store the index of where b is
    for (int i = 0; i < length; i++) { //spans the whole array
        if (test[i][0] == b) {         //if the page number slot of the array matches b
            whereIs = i;               //set the variable whereIs to the index
        }
    }
    return whereIs;                    //return whereIs
}

/***************************************************************************
* int Clock(int wss, int pageNumbers[])
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: The clock page replacement algorithm, also known as the second
* chance page replacement algorithm
* Parameters:
* wss          I/P   int   The number of arguments on the command line
* pageNumbers  I/P   int[] An array containing all of the page numbers
* Clock        O/P   int   The number of page faults after the algorithms runs
**************************************************************************/
int Clock(int wss, int pageNumbers[]) { // we will have to create our own "queue" for this one
    int workingSetArray[wss][2];        // second part of two-dimensional array will be the use bit
    int counter = 0;                    // to keep track of when we hit out wss limit
    int pointer = 0;                    // initialized to the first position in our queue
    int pageReferenced;                 // initialize a variable to store the index of a referenced page number
    int pageFaultCount = 0;             // initialize the counter for page faults to zero
    for (int i = 0; i < 12; i++) {
        if (!arrayContains(workingSetArray, pageNumbers[i],
                           wss)) {      // if our working set does not contain this incoming page number
            if (counter < wss) {        // as long as there is an empty slot in the working set,
                                        // no need to do any page replacement, just tack it on the end
                workingSetArray[pointer][0] = pageNumbers[i];
                workingSetArray[pointer][1] = 1; // the zero indicates that the page is given its first chance
                pointer++;                       // we have to manually keep track of where we are in the working set
                                                 // page queue move it up one; we don't need to check to see if the
                                                 // pointer will exceed the working set size because our condition to
                                                 // get into this coding section already prevents that
                counter++;                       // we just added an element to our working set; moves in lockstep with
                                                 // the pointer variable in this section of code
            } else {                             // this means that our working set has reached max
                                                 // capacity and we must start swapping out pages
                while (workingSetArray[pointer][1] != 0) { // keep doing this loop until we encounter a replaceable page
                    workingSetArray[pointer][1] = 0;       // set the current position to replaceable
                    if (pointer == wss - 1) {              // if the pointer is at the last slot in the working set
                        pointer = 0;                       // set the pointer to point to slot zero
                    } else {                               // otherwise
                        pointer++;                         // we have to manually keep track of where we are
                                                           // in the working set page queue, move it up one
                    }
                }
                //at this point in the code, the pointer MUST be pointing to a page index where is it replaceable, it
                // may have not ran at all, in which case the program simply started in a replaceable page
                workingSetArray[pointer][0] = pageNumbers[i]; //the page has now been replaced
                workingSetArray[pointer][1] = 1;              //set it to newly placed so it won't be replaced sooner
                pageFaultCount++;                             //increment the page fault counter
                pointer++;                                    //increment the pointer
            }
        } else                                                // if the working set does include this incoming page
                                                              // number already, pointer nor counter increment
        {
            pageReferenced = arrayContainsWithIndex(workingSetArray, pageNumbers[i], wss); //locate the  position of
                                                                                           // where that page is located
            workingSetArray[pageReferenced][1] = 1;//a rule of the clock algorithm is that if a page is referenced
                                                   // in the working set, even if the use bit is 1, set it back to 1
        }

        if (pointer == wss) {                      //if the pointer is beyond the end of the array
            pointer = 0;                           //set it to point at the beginning
        }
    }
    return pageFaultCount;                         //return the number of page faults
}

/***************************************************************************
* int main()
* Author: Rameen Housseini
* Date: 24 April 2021
* Description: Creates the page numbers, arrays for the page faults, runs the
*              algorithms to fill in the page fault arrays and calculates the
*               average page faults over 1,000 experiments
* Parameters:
* main     O/P      int Status code (not currently used)
**************************************************************************/
int main() {
    int LRUFaults[20] = {};       //we will keep the faults for FIFO, clock and LRU algorithms in these arrays for every
    int FIFOFaults[20] = {};      //working set and calculate their average in the end
    int ClockFaults[20] = {};

    for (int experiments = 0; experiments < 1000; experiments++) { //we are trying to run 1,000 experiments

        int pageNumbers [1000] = {};                                //initializing array of page number/address stream
                                                                    //as empty; structuring the command like this should
                                                                    //reset the data with every iteration of the loop

        for (int pageNumGen = 0; pageNumGen < 1000; pageNumGen++) { //we are trying to generate 1,000 numbers

            unsigned seed = chrono::system_clock::now().time_since_epoch().count(); //creating the random seed
            default_random_engine generator (seed);                                 //assigning seed to random engine
            poisson_distribution<int> distribution (10); //declaring pre-defined poisson distribution random
                                                               // number generator with lambda = 10 as parameter

            for (int i=0; i<1000; ++i){                       //loop for all 1,000 page numbers we need
                pageNumbers[i] = distribution(generator);  //generate a random number that fits the poisson
                                                              // distribution with lambda = 10 and assign it to index
                                                              // in page number array
            }
        }

        for (int wss = 2; wss <= 20; wss++) {            //wss = working set size; we are working with 2 to 20 working
                                                         //set size; set loop for such
            LRUFaults[wss] += LRU(wss, pageNumbers);     //will import the total number of page faults for the LRU
                                                         //algorithm for each working set size and cumulatively add them
            FIFOFaults[wss] += FIFO(wss, pageNumbers);   //same as above but for the FIFO algorithm
            ClockFaults[wss] += Clock(wss, pageNumbers); //same as above but for the Clock algorithm
        }

        for (int wss = 2; wss <= 20; wss++) { //wss = working set size; we are working with 2 to 20 working set size
            LRUFaults[wss] /= 1000;           //will calculate the average, the last loop added all the values up so all
                                              //we have to do is divide each slot by 1000 since we added up 1000's
                                              // worth of numbers
            FIFOFaults[wss] /= 1000;          //same as above but for the FIFO algorithm
            ClockFaults[wss] /= 1000;         //same as above but for the Clock algorithm
        }

        cout << "Average number of LRU page faults for working set size 2-20: ";
        for (int wss = 2; wss <= 20; wss++)       //wss = working set size; we are working with 2 to 20 working set size
            cout << LRUFaults[wss] << " ";        //lists every element in LRUfaults with a space in between
        cout << endl;

        cout << "Average number of FIFO page faults for working set size 2-20: ";
        for (int wss = 2; wss <= 20; wss++)       //wss = working set size; we are working with 2 to 20 working set size
            cout << FIFOFaults[wss] << " ";       //lists every element in FIFOfaults with a space in between
        cout << endl;

        cout << "Average number of Clock page faults for working set size 2-20: ";
        for (int wss = 2; wss <= 20; wss++)       //wss = working set size; we are working with 2 to 20 working set size
            cout << ClockFaults[wss] << " ";      //lists every element in Clockfaults with a space in between
        cout << endl;
    }
    return 0;
}


