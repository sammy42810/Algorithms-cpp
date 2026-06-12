/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Samantha Bryan
 * Version     : 1.0
 * Date        : 11/29/24
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in Theta(n^2) time using two nested loops.
 */
long count_inversions_slow(int array[], int length) { //Will count the number of inversion in an array using nested loops which is the slow version
    // TODO
    long c = 0;
    for(int i = 0; i < length; i++){ 
        for(int j = i+1; j < length; j++){ 
            if(array[i] > array[j]){
                c++;    //If there is an inversion it upps the counter
            }
        }
    }
    return c;
}

/**
 * Counts the number of inversions in an array in Theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!
    int* B = new int[length];   //Creates a new array pointer with the same length as the input array
    long c = mergesort(array, B, 0, length-1);  //Uses mergesort to count the inversions
    delete [] B;    
    return c;
}

static long mergesort(int array[], int scratch[], int low, int high){
    long count = 0; 
    if(low < high){ //Recursively runs mergesort until the array is split as much as it can be
        int mid = low + (high-low)/2;   
        count += mergesort(array, scratch, low, mid);   //Does merge sort on the bottom half of the array
        count += mergesort(array, scratch, mid+1, high); //Does merge sort on the top half

        int i1 = low;
        int i2 = mid+1;
        int i = low;
        
        while(i1 <= mid && i2 <= high){ 
             if(array[i1]<=array[i2]){  
                 scratch[i++]=array[i1++];
             }
             else{
                 scratch[i++] = array[i2++];
                 count += mid - i1 +1;
             }
        }

        while (i1 <= mid) {
            scratch[i++] = array[i1++];
        }
        while (i2 <= high) {
            scratch[i++] = array[i2++];
        }
        for(int h = low; h <= high; h++){
             array[h] = scratch[h];
        }

        return count;
    }

    else{
        return 0;
    }    
}

int main(int argc, char *argv[]) {
    // TODO: parse command-line argument
    //Too many arguments checker
    if (argc > 2) {
        cerr << "Usage: ./inversioncounter [slow]" << endl;
        return 1;
    }
    
    //Checks if it says anything other than slow if there is more than one argument
    else if (argc == 2 && (string(argv[1]) != "slow")) {
        cerr << "Error: Unrecognized option '" << argv[1] << "'." << endl;
        return 1;
    }


    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    // TODO: produce output
    if (values.size() == 0) {
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }
  
    if (argc == 2 && ((string(argv[1])) == "slow")) {
        cout << "Number of inversions (slow): " << count_inversions_slow(&values[0], values.size()) << endl;
    }
    else {
        cout << "Number of inversions (fast): " << count_inversions_fast(&values[0], values.size()) << endl;
    }

    return 0;
}
