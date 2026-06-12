/*******************************************************************************
* Name        : waterjugpuzzle.cpp
* Author      : Tyler Christensen and Samantha Bryan
* Date        : 10/19/24
* Description : Water Jug Puzzle
* Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
******************************************************************************/
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <tuple>


using namespace std;




// Struct to represent state of water in the jugs. (this class was provided by Philippe)
struct State {
       int a, b, c;
string directions;
State *parent;


State(int _a, int _b, int _c, string _directions) :
a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }


// String representation of state in tuple form.
string to_string() {
   ostringstream oss;
   oss << "(" << a << ", " << b << ", " << c << ")";
   return oss.str();
}
};


queue<State*> bfsQueue; //A global variable for the BFS Queue so that we can access it throughout our code


vector<State> solution; //A vector that we will use to store our solution once we have reaches the goal state -- also a global variable


vector<State*> heapStates; //A vector that will store all States that we create to make it easier to erase them from memory at the end




//Will return the solution vector after doing BFS of the states that we create by pouring water from jug to jug
vector<State> bfs (int a, int b, int c, string directions, State goalState, State capState, tuple<int,int> **array){
   //Every time we add a state to the queue, we also add it to the 2d array with a pointer to its parent and to the heapStates vector so that it can be deleted later
   
   State *newState = new State(a,b,c,directions); //Creates the new state, with assigned directions


   heapStates.insert(heapStates.begin(), newState); //Inserts the newState into the heapStates vector


   bfsQueue.push(newState); //Pushes the newState onto the queue




   while (!bfsQueue.empty()){ //While there are still States in the queue to check, it will check if the state is the goal state
       State *current = bfsQueue.front(); //Sets current state equal to the front of the queue
       bfsQueue.pop(); //Removes current from the queue so that we do not check it again
       if (current -> a == goalState.a && current -> b == goalState.b && current -> c == goalState.c){
           //If the conditions of the current state are the sanme as the goal state,
           //It will iteratively return this state and the pointer to its parent state
           //and insert it into the solution vector
           while (current -> parent != nullptr){
               solution.insert(solution.begin(), *current);
               current = current -> parent;
           }
           solution.insert(solution.begin(), *current);
           return solution;
       }




       //If it has already been seen, we skip to the next state in the queue
       if (array[current -> a][current -> b] == make_tuple(current -> a, current -> b)){
           continue;
       }




       //If the state is not the goal state, it puts it into the array so that we do not visit it again
       array[current -> a][current -> b] = make_tuple(current -> a, current -> b);




       int poured_amount; //Keeps track of how much is poured from jug to jug for each transfer type
       int newA; //newA, newB, and newC, keep track of the new values of the 3 jugs after the transfers
       int newB;
       int newC;




       //pouring C to A
       if (current -> a + current -> c > capState.a){ //If pouring all of C to A would cause A to overflow, only fill jug A up to its capacity
           newA = capState.a;
           newC = current -> c - capState.a + current -> a;
           poured_amount = capState.a - current -> a;
       } else { //If A will not overflow by pouring C, empty jug C into jug A
           newA = current -> a + current -> c;
           newC = 0;
           poured_amount = current -> c;
       }
       //Checking that jug values are valid, and that the state is not a repeat (could happen if jug A was already full)
       if (newA <= capState.a && newA >= 0 && newC <= capState.c && newC >= 0 && array[newA][current -> b] == make_tuple(-1,-1)){
           string C_to_A_directions;
           if (poured_amount == 1){ //Checks to make sure grammar is printed correctly
               C_to_A_directions = "Pour " + to_string(poured_amount) + " gallon from C to A.";
           } else {
               C_to_A_directions = "Pour " + to_string(poured_amount) + " gallons from C to A.";
           }


           State *C_to_A = new State (newA, current -> b, newC, C_to_A_directions); //Creates a new state pointer
           C_to_A -> parent = current; //And assigns current to the parent attribute
           bfsQueue.push(C_to_A); //Pushes our new state into the queue so that we can later check to see if it is the goal state
           heapStates.insert(heapStates.begin(), C_to_A); //Also inserts the state pointer into the heapState vector so that we can delete it from memory later
       }




       //pouring B to A
       if (current -> a + current -> b > capState.a){ //If pouring all of B to A would cause A to overflow, only fill jug A up to its capacity
           newA = capState.a;
           newB = current -> b - capState.a + current -> a;
           poured_amount = capState.a - current -> a;
       } else { //If A will not overflow by pouring B, empty jug B into jug A
           newA = current -> a + current -> b;
           newB = 0;
           poured_amount = current -> b;
       }
       //Checking that jug values are valid, and that the state is not a repeat (could happen if jug A was already full)
       if (newA <= capState.a && newA >= 0 && newB <= capState.b && newB >= 0 && array[newA][newB] == make_tuple(-1,-1)){
           string B_to_A_directions;
           if (poured_amount == 1){ //Checks to make sure grammar is printed correctly
               B_to_A_directions = "Pour " + to_string(poured_amount) + " gallon from B to A.";
           } else {
               B_to_A_directions = "Pour " + to_string(poured_amount) + " gallons from B to A.";
           }
           State *B_to_A =  new State(newA, newB, current -> c, B_to_A_directions); //Creates a new state pointer
           B_to_A -> parent = current; //And assigns current to the parent attribute
           bfsQueue.push(B_to_A); //Pushes our new state into the queue so that we can later check to see if it is the goal state
           heapStates.insert(heapStates.begin(), B_to_A); //Also inserts the state pointer into the heapState vector so that we can delete it from memory later
       }




       //pouring C to B
       if (current -> b + current -> c > capState.b){ //If pouring all of C to B would cause B to overflow, only fill jug B up to its capacity
           newB = capState.b;
           newC = current -> c - capState.b + current -> b;
           poured_amount = capState.b - current -> b;
       } else { //If B will not overflow by pouring C, empty jug C into jug B
           newB = current->b + current->c;
           newC = 0;
           poured_amount = current->c;
       }
       //Checking that jug values are valid, and that the state is not a repeat (could happen if jug B was already full)
       if (newB <= capState.b && newB >= 0 && newC <= capState.c && newC >= 0 && array[current->a][newB] == make_tuple(-1,-1)){
           string C_to_B_directions;
           if (poured_amount == 1){ //Checks to make sure grammar is printed correctly
               C_to_B_directions = "Pour " + to_string(poured_amount) + " gallon from C to B.";
           } else {
               C_to_B_directions = "Pour " + to_string(poured_amount) + " gallons from C to B.";
           }
           State *C_to_B = new State(current->a, newB, newC, C_to_B_directions); //Creates a new state pointer
           C_to_B->parent = current; //And assigns current to the parent attribute
           bfsQueue.push(C_to_B); //Pushes our new state into the queue so that we can later check to see if it is the goal state
           heapStates.insert(heapStates.begin(), C_to_B); //Also inserts the state pointer into the heapState vector so that we can delete it from memory later
       }




       //pouring A to B
       if (current->b + current->a > capState.b){ //If pouring all of A to B would cause B to overflow, only fill jug B up to its capacity
           newB = capState.b;
           newA = current->a - capState.b + current->b;
           poured_amount = capState.b - current->b;
       } else { //If B will not overflow by pouring A, empty jug A into jug B
           newB = current->b + current->a;
           newA = 0;
           poured_amount = current->a;
       }
       //Checking that jug values are valid, and that the state is not a repeat (could happen if jug B was already full)
       if (newB <= capState.b && newB >= 0 && newA <= capState.a && newA >= 0 && array[newA][newB] == make_tuple(-1,-1)){
           string A_to_B_directions;
           if (poured_amount == 1){ //Checks to make sure grammar is printed correctly
               A_to_B_directions = "Pour " + to_string(poured_amount) + " gallon from A to B.";
           } else {
               A_to_B_directions = "Pour " + to_string(poured_amount) + " gallons from A to B.";
           }
           State *A_to_B = new State(newA, newB, current->c, A_to_B_directions); //Creates a new state pointer
           A_to_B->parent = current; //And assigns current to the parent attribute
           bfsQueue.push(A_to_B); //Pushes our new state into the queue so that we can later check to see if it is the goal state
           heapStates.insert(heapStates.begin(), A_to_B); //Also inserts the state pointer into the heapState vector so that we can delete it from memory later
       }




       //pouring A to C
       if (current->c + current->a > capState.c){ //If pouring all of A to C would cause C to overflow, only fill jug C up to its capacity
           newC = capState.c;
           newA = current->a - capState.c + current->c;
           poured_amount = capState.c - current->c;
       } else { //If C will not overflow by pouring A, empty jug A into jug C
           newC = current->c + current->a;
           newA = 0;
           poured_amount = current->a;
       }
       //Checking that jug values are valid, and that the state is not a repeat (could happen if jug C was already full)
       if (newC <= capState.c && newC >= 0 && newA <= capState.a && newA >= 0 && array[newA][current->b] == make_tuple(-1,-1)){
           string A_to_C_directions;
           if (poured_amount == 1){ //Checks to make sure grammar is printed correctly
               A_to_C_directions = "Pour " + to_string(poured_amount) + " gallon from A to C.";
           } else {
               A_to_C_directions = "Pour " + to_string(poured_amount) + " gallons from A to C.";
           }
           State *A_to_C = new State(newA, current->b, newC, A_to_C_directions); //Creates a new state pointer
           A_to_C->parent = current; //And assigns current to the parent attribute
           bfsQueue.push(A_to_C); //Pushes our new state into the queue so that we can later check to see if it is the goal state
           heapStates.insert(heapStates.begin(), A_to_C); //Also inserts the state pointer into the heapState vector so that we can delete it from memory later
       }




       //pouring B to C
       if (current->c + current->b > capState.c){ //If pouring all of B to C would cause C to overflow, only fill jug C up to its capacity
           newC = capState.c;
           newB = current->b - capState.c + current->c;
           poured_amount = capState.c - current->c;
       } else { //If C will not overflow by pouring B, empty jug B into jug C
           newC = current->c + current->b;
           newB = 0;
           poured_amount = current->b;
       }
       //Checking that jug values are valid, and that the state is not a repeat (could happen if jug C was already full)
       if (newC <= capState.c && newC >= 0 && newB <= capState.b && newB >= 0 && array[current->a][newB] == make_tuple(-1,-1)){
           string B_to_C_directions;
           if (poured_amount == 1){  //Checks to make sure grammar is printed correctly
               B_to_C_directions = "Pour " + to_string(poured_amount) + " gallon from B to C.";
           } else {
               B_to_C_directions = "Pour " + to_string(poured_amount) + " gallons from B to C.";
           }
           State *B_to_C = new State(current->a, newB, newC, B_to_C_directions); //Creates a new state pointer
           B_to_C->parent = current; //And assigns current to the parent attribute
           bfsQueue.push(B_to_C); //Pushes our new state into the queue so that we can later check to see if it is the goal state
           heapStates.insert(heapStates.begin(), B_to_C); //Also inserts the state pointer into the heapState vector so that we can delete it from memory later
       }
   }
   return solution; //Returns the solution vector
}




//Takes the output vector from BFS and prints its elements in the specified format
void display_solution(vector<State> solution){
   for (size_t i = 0; i < solution.size(); i++){ //Loop to iterate through outer vector
       cout << solution[i].directions << " " << "(" << solution[i].a << ", " << solution[i].b << ", " << solution[i].c << ")" << endl;
   }
}




int main(int argc, char * const argv[]) {


   if (argc != 7){ //Makes sure that there is the right number of arguments in the function call
       cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
       return 0;
   }
 


   //Uses istringstream to read the arguments from argv[] which is the cap of A, B, and C, as well as the goal amount for each
   istringstream iss;
   istringstream iss2;
   istringstream iss3;
   istringstream iss4;
   istringstream iss5;
   istringstream iss6;


   int capA;
   int capB;
   int capC;
   int goalA;
   int goalB;
   int goalC;
 
   iss.str(argv[1]);
   iss2.str(argv[2]);
   iss3.str(argv[3]);
   iss4.str(argv[4]);
   iss5.str(argv[5]);
   iss6.str(argv[6]);
 


   //If the input is not a number that is greater than 0, then it outputs an error  
   if (!(iss >> capA) || capA <= 0){
       cerr << "Error: Invalid capacity '" << argv[1] << "' for jug A." << endl;
       return 0;
   }




   //If the input is not a number that is greater than 0, then it outputs an error
   else if (!(iss2 >> capB) || capB <= 0){
       cerr << "Error: Invalid capacity '" << argv[2] << "' for jug B." << endl;
       return 0;
   }
 


   //If the input is not a number that is greater than 0, then it outputs an error
   else if (!(iss3 >> capC) || capC <= 0){
       cerr << "Error: Invalid capacity '" << argv[3] << "' for jug C." << endl;
       return 0;
   }




   //If the input is not a number that is greater than or equal to 0, then it outputs an error
   else if (!(iss4 >> goalA) || goalA < 0){
       cerr << "Error: Invalid goal '" << argv[4] << "' for jug A." << endl;
       return 0;
   }




   //If the input is not a number that is greater than or equal to 0, then it outputs an error
   else if (!(iss5 >> goalB) || goalB < 0){
       cerr << "Error: Invalid goal '" << argv[5] << "' for jug B." << endl;
       return 0;
   }




   //If the input is not a number that is greater than or equal to 0, then it outputs an error
   else if (!(iss6 >> goalC) || goalC < 0){
       cerr << "Error: Invalid goal '" << argv[6] << "' for jug C." << endl;
       return 0;
   }




   //If the goal is greater than the capacity of the jug then it returns an error
   else if (goalA > capA){
       cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
       return 0;
   }




   //If the goal is greater than the capacity of the jug then it returns an error
   else if (goalB > capB){
       cerr << "Error: Goal cannot exceed capacity of jug B." << endl;
       return 0;
   }




   //If the goal is greater than the capacity of the jug then it returns an error
   else if (goalC > capC){
       cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
       return 0;
   }




   //If the goal for each jug added together is greater than the initial water in the problem (capacity of jug C) then it returns an error
   else if (goalA + goalB + goalC != capC){
       cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
       return 0;
   }




   else {
       int num_rows = capA + 1;
       int num_cols = capB + 1;


       // Declare a 2D array with 'num_rows' rows.
       tuple<int,int> **array = new tuple<int,int>*[num_rows];
       // For each row, make an array of 'num_cols' booleans.
       for(int i = 0; i < num_rows; i++) {
           array[i] = new tuple<int,int>[num_cols];
           // Initialize the new row.
           fill(array[i], array[i] + num_cols, make_tuple(-1,-1));
       }




       //Creating the States for initial, goal, and cap States
       State initialState(0, 0, capC, "Initial state.");
       State goalState(goalA, goalB, goalC, "Goal State");
       State capState(capA, capB, capC, "Capacity State");




       //Calls the BFS function to be stored in the solution vector of States
       vector<State> solution = bfs(0, 0, capC, "Initial state.", goalState, capState, array);




       //If the solution vector has a size of 0, then there would be no solution
       if (solution.size() == 0){
           cout << "No solution." << endl;
       }
       else {
           display_solution(solution); //Otherwise, it will call the display solution function for the solution vector
       }


       // Delete each row first.
       for(int i = 0; i < num_rows; i++) {
           delete [] array[i];
       }
       // Delete the array itself.
       delete [] array;
   }


   //Using the heapStates vector we made, we will iterate through to each State and delete it from memory
   for (size_t j = 0; j < heapStates.size(); j++){
       delete heapStates[j];
   }


   return 1;


}
