/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Sam Bryan
 * Date        : 10/03/24
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

int num_stairs;

vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.

    vector < vector<int> > ways;    //Declares the vector that will store all posible paths to climb the stairs
    vector < vector<int> > result;  //Declares a vector result to store paths after each recursion
    
    if(num_stairs<=0){  //Checks that there are more than 0 stairs
        return {{}};
    }

    for(int i=1; i<4; i++){ //Iterates through each option of choosing 1, 2, or 3 stairs at a time
        if(num_stairs>=i){
            result = get_ways(num_stairs-i);    //Calls the recursive function
            for(size_t j=0; j<result.size(); j++){  //Stores the ways
                result[j].insert(result[j].begin(), i);
                ways.push_back(result[j]);
            }
        }
    }
    return ways;
}

//From my sieve assignment
int num_digits(int num) {
    int c = 1;
    while(num >= 10){   //Counts the number of digits by dividing by 10
        c++;
        num = num/10;
    }
    return c;
}

void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.
    
    int max_width = num_digits(ways.size());   
    int max_space = num_digits(ways.size()) -1;

    cout << "max width: " << max_width << endl;
    cout << "max space: " << max_space << endl;


    //Adjusts grammar for plural vs non-plural senarios
    if(num_stairs == 1){
        cout << ways.size() << " way to climb " << num_stairs <<" stair." << endl;
    }
    else{
        cout << ways.size() << " ways to climb "<< num_stairs <<" stairs." << endl;
    }

    for (size_t i = 0; i < ways.size(); i++){  //Iterates through the list of all possible paths to climb
        if ((num_digits(i + 1) + max_space) > max_width){ //Proper spacing between labels
            max_space--; 
        }
        for (int l = 0; l < max_space; l++){ //Just prints spaces
            cout << " ";
        }
        cout << i + 1 << ". [" << ways[i][0]; //Beginning of print line but does not end line yet
        if (ways[i].size() == 1){   //If there is only one path to climb the stairs
            cout << "]" << endl;
        }
        else{
            for (size_t j = 1; j < ways[i].size() - 1; j++){ //Iterates through the ways vector
                cout << ", " << ways[i][j]; //Prints each and seperates with a comma
            }
            cout << ", " << ways[i][ways[i].size() - 1] << "]" << endl; //For the last way, ends with a bracket
        }
    }
}

int main(int argc, char * const argv[]) {
    if (argc != 2){ //Prints an error if the wrong number of arguments
        cerr << "Usage: ./stairclimber <number of stairs>" << endl;
        return 0;
    }

    //Reads the integer in argv[1]
    istringstream iss;
    iss.str(argv[1]);
    int n;

    if (!(iss >> n)){ //Prints an error if given a string
        cerr << "Error: Number of stairs must be a positive integer." << endl;
        return 0;
    }
    if (n < 1){ //Prints an error if number is negative
        cerr << "Error: Number of stairs must be a positive integer." << endl;
        return 0;
    }

    //Stores the number in argv[1] into the global variable for the number of stairs
    num_stairs = n;

    //Runs the method to get the ways to climb the stairs
    display_ways(get_ways(n));

    return 1;
}
