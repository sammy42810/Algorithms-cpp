/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Samantha Bryan
 * Date        : 9/15/24
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System. 
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const { //Displaces the primes in the specified format so that the right side is
    //lined up for each number.

    // TODO: write code to display the primes in the format specified in the
    // requirements document.

    //Declares the max prime width and the primes per row variables
    const int max_prime_width = num_digits(max_prime_), primes_per_row = 80 / (max_prime_width + 1);
    //Prints num of primes and what they are up to
    cout << "Number of primes found: " << num_primes_ << endl << "Primes up to " << limit_ << ":" << endl;

    //Makes a counter and sets to 0
    int c = 0;
    for(int i = 0; i <= limit_; i++){ //Iterates through all numbers until the limit
        if(is_prime_[i] == true){ //Checks if it is prime or not
            if(num_primes_ > primes_per_row){
                cout << setw(max_prime_width); 
            }
            cout << i; //Outputs the prime
            c++;

            if(c%primes_per_row == 0 && c != 0){ //Checks if the primes has reached the max width 
                cout << endl; //If so indents to a new line
            }

            else{
                if(i != max_prime_){ //If it's not the last prime then it adds a space
                    cout << " ";
                }
                else{
                    cout << endl; //If it is the last prime then it ends the line
                }
            }
        }
    }


}

void PrimesSieve::sieve() {
    // TODO: write sieve algorithm
    // All instance variables must be initialized by the end of this method.

    //Sets 1 and 0 to false because they are not prime
    is_prime_[0] = false; 
    is_prime_[1] = false;
    
    //sets all numbers up to the limit as true initially
    for (int i=2; i <= limit_; i++){
        is_prime_[i] = true;
    }

    //Iterates through numbers up to the sqrt of the limit
    for(int i = 2; i < sqrt(limit_); i++){
        if(is_prime_[i] == true){ //Checks if they are prime
            for(int j = pow(i, 2); j <= limit_; j += i){
                is_prime_[j] = false;
            }
        }
    }

    //Finds the maximum prime within the limit
    for(int i = limit_; i >= 2; i--){
        if(is_prime_[i] == true){
            max_prime_ = i;
            break;
        }
    }

    //Counts the number of primes up to the limit
    num_primes_ = 0;
    for(int i = 2; i <= limit_; i++){
        if(is_prime_[i] == true){
            num_primes_ += 1;
        }
    }
}


int PrimesSieve::num_digits(int num) {
    // TODO: write code to determine how many digits are in an integer
    // Hint: No strings are needed. Keep dividing by 10.
    int c = 1;
    while(num >= 10){
        c++;
        num = num/10;
    }
    return c;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    // TODO: write code that uses your class to produce the desired output.
    
    //Creates a primesieve object with the desired limit
    PrimesSieve one = PrimesSieve(limit);
    cout << endl;
    one.display_primes(); //Calls the display primes method to display them up to the limit in the right formating.
    return 0;
}
