/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Samantha Bryan
 * Date        : 9/26/24
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    // TODO: returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.

    //Checks to make sure every letter is a lowercase letter (not upper, number, or special character)
    for(size_t i = 0; i < s.size(); i++){
        if(!(islower(s[i]))){
            return false;
        }
    }
    return true;
}

bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.

    // You MUST use only single unsigned integers and work with bitwise
    // and bitshifting operators only.  Using any other kind of solution
    // will automatically result in a grade of ZERO for the whole assignment.

    unsigned int vector = 0;
    unsigned int setter;
    unsigned int temp = 0;

    //Iterates through the string to make sure all the letters are unique
    for(size_t i = 0; i < s.size(); i++){
        setter = 1 << (s[i] - 'a');
        temp = vector & setter;
        if(temp != 0){
            return false;
        }
        vector = setter | vector;
        temp = 0;
    }
    return true;
}

int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.

    //Checks for correct # of arguments
    if(argc != 2){
        cerr << "Usage: ./unique <string>" << endl;
        return 1;
    }

    
    if(is_all_lowercase(argv[1]) == false){ //Checks is_all_lowercase to make sure the letters are lowercase
        cerr << "Error: String must contain only lowercase letters." << endl;
        return 1;
    }
    else if(all_unique_letters(argv[1])){ //Checks if the letters are unique
        cout << "All letters are unique." << endl;
    }
    else{
        cout << "Duplicate letters found." << endl;
    }
    return 0;
}
