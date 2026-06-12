/*******************************************************************************
* Name        : shortestpaths.cpp
* Author      : Tyler Christensen and Samantha Bryan
* Version     : 1.0
* Date        : 11/30/2024
* Description : Solve the all pairs shortest paths problem with Floyd's Algorithm.
* Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
******************************************************************************/
#include <cmath>
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>


using namespace std;


//Applies Floyd's Algorithm to find the shortest paths from vertex to vertex, stored in a matrix
long** floyd(long** const path, long** const I, int num_verticies){
   for(int k = 0; k < num_verticies; k++){
       for(int i = 0; i < num_verticies; i++){
           for(int j = 0; j < num_verticies; j++){
               if(path[i][k] != numeric_limits<long>::max() && path[k][j] != numeric_limits<long>::max() && (path[i][k] + path[k][j]) < path[i][j]){
                   path[i][j] = path[i][k] + path[k][j];
                   I[i][j] = k;    //Changes the itermediate array
               }
           }
       }
   }
   return path;
}


//Helper function for displaying the paths. Recursively prints the intermediate vertices
string make_path(int i, int j, long** intermediates) {
   //Base case: If no intermediate vertex, return the direct path.
   if (intermediates[i][j] == numeric_limits<long>::max()) {
       if (i == j) {
           return "";
       }
       return " -> " + string(1, static_cast<char>(j + 'A'));
   }
   int k = intermediates[i][j];
   // Construct path recursively through the intermediate vertex `k`.
   return make_path(i, k, intermediates) + make_path(k, j, intermediates);
}


//Display the paths using the short_paths and intermediates matrices
void display_paths(long** short_paths, long** intermediates, int num_vertices){
   for(int i = 0; i < num_vertices; i++){
       for(int j = 0; j < num_vertices; j++){
           cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: ";
           if(short_paths[i][j] == numeric_limits<long>::max()){
               cout << "infinity, path: none" << endl;
           }
           else{
               cout << short_paths[i][j] << ", path: " << static_cast<char>(i + 'A') << make_path(i, j, intermediates) << endl;
           }
       }
   }
}


/**
Displays the matrix on the screen formatted as a table. Used for the distances, paths, and intermediate vertices matrices.
*/
void display_table(long** const matrix, const string &label, int num_vertices, const bool use_letters = false){
  cout << label << endl;
  long max_val = 0;
  for (int i = 0; i < num_vertices; i++){
      for (int j = 0; j < num_vertices; j++){
          long cell = matrix[i][j];
          if (cell < numeric_limits<long>::max() && cell > max_val){
              max_val = matrix[i][j];
          }
      }
  }


  int max_cell_width = use_letters ? to_string(max_val).length() : to_string(max(static_cast<long>(num_vertices), max_val)).length();
  cout << ' ';


  for (int j = 0; j < num_vertices; j++){
      cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
  }
  cout << endl;


  for (int i = 0; i < num_vertices; i++){
      cout << static_cast<char>(i + 'A');
      for (int j = 0; j < num_vertices; j++){
          cout << " " << setw(max_cell_width);
          if (matrix[i][j] == numeric_limits<long>::max()) {
              cout << "-";
          } else if (use_letters) {
              cout << static_cast<char>(matrix[i][j] + 'A');
          } else {
              cout << matrix[i][j];
          }
      }
      cout << endl;
  }
  cout << endl;
}


//Delete dynamically allocated 2D arrays
void delete_matrix(long** matrix, int cols){
  for (int i = 0; i < cols; i++){
      delete[] matrix[i];
  }
  delete[] matrix;
}


int main(int argc, const char *argv[]) {
  // Make sure the right number of command line arguments exist.
  if (argc != 2) {
      cerr << "Usage: " << argv[0] << " <filename>" << endl;
      return 1;
  }

  // Create an ifstream object.
  ifstream input_file(argv[1]);

  // If it does not exist, print an error message.
  if (!input_file) {
      cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
      return 1;
  }

  // Add read errors to the list of exceptions the ifstream will handle.
  input_file.exceptions(ifstream::badbit);
  string line;


  try {
      unsigned int line_number = 1;
      // Use getline to read in a line.
      // See http://www.cplusplus.com/reference/string/string/getline/

      getline(input_file,line); //get the first line of the file
      int num_vertices = 0;
      try {
          num_vertices = stol(line);  // try to convert to long
          if (num_vertices < 1 || num_vertices > 26){
              cerr << "Error: Invalid number of vertices '" << num_vertices << "' on line 1." << endl;
              return 1;
          }
      } catch (const invalid_argument& e) {
          cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
          return 1;
      } catch (const out_of_range& e) {
          cerr << "Error: The number is out of range for the long type." << endl;
          return 1;
      }

      long rows = num_vertices;
      long cols = num_vertices;
      //Initialize 2D array of distances
      long** distances = new long*[rows]; //will need to delete this for each error case as well as successful cases


      //Allocate memory for each column in each row
      for (int i = 0; i < rows; i++) {
          distances[i] = new long[cols];
          for (int j = 0; j < cols; j++) {
              if (i == j){
                  distances[i][j] = 0;
              } else {
                  distances[i][j] = numeric_limits<long>::max();
              }
          }
      }


      line_number++;
      string vertex_one = "";
      string vertex_two = "";
      string weight_string = "";
      long unsigned int i;
      while (getline(input_file, line)) {
          vertex_one = "";
          vertex_two = "";

          i = 0;
          while (i < line.size() && line[i] != ' '){ //assign the starting vertex to a string variable
              vertex_one += line[i];
              i++;
          }

          i++;
          while(i < line.size() && line[i] != ' ' && line[i] != '\0'){ //assign the ending vertex to a string variable
              vertex_two += line[i];
              i++;
          }

          i++;
          weight_string = "";
          while(i < line.length() && line[i] != ' ' && line[i] != '\0'){ //assign the weight to a string variable
              weight_string += line[i];
              i++;
          }

          if (weight_string.empty()){ //no weight given
              cerr << "Error: Invalid edge data '" << vertex_one << " " << vertex_two << "' on line " << line_number << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          }

          if (vertex_one.length() > 1){ //starting vertex is more than one character
              cerr << "Error: Starting vertex '" << vertex_one << "' on line " << line_number << " is not among valid values A-" << static_cast<char>(num_vertices - 1 + 'A') << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          }

          if (vertex_two.length() > 1){ //ending vertex is more than one character
              cerr << "Error: Ending vertex '" << vertex_two << "' on line " << line_number << " is not among valid values A-" << static_cast<char>(num_vertices - 1 + 'A') << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          }

          if (num_vertices <= line[0] - 'A' || line[0] - 'A' < 0){ //starting vertex is not between 'A' and ('A' + num_vertices)
              cerr << "Error: Starting vertex '" << line[0] << "' on line " << line_number << " is not among valid values A-" << static_cast<char>(num_vertices - 1 + 'A') << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          }

          if (num_vertices <= line[2] - 'A' || line[2] - 'A' < 0){ //ending vertex is not between 'A' and ('A' + num_vertices)
              cerr << "Error: Ending vertex '" << line[2] << "' on line " << line_number << " is not among valid values A-" << static_cast<char>(num_vertices - 1 + 'A') << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          }

          if ((!(line[0] >= 'a' && line[0] <= 'z') && (!(line[0] >= 'A' && line[0] <= 'Z'))) || line [1] != ' ' || (!(line[2] >= 'a' && line[2] <= 'z') && (!(line[2] >= 'A' && line[2] <= 'Z')))){
              cerr << "Error: Invalid edge data '" << vertex_one << line[1] << vertex_two << "' on line " << line_number << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          }


          long weight = 0;
          try {
              weight = stol(weight_string);  // try to convert to long
              if (weight < 1){
                  cerr << "Error: Invalid edge weight '" << weight << "' on line " << line_number << "." << endl;
                  delete_matrix(distances, num_vertices);
                  return 1;
              }
          } catch (const invalid_argument& e) {
              cerr << "Error: Invalid edge weight '" << weight_string << "' on line " << line_number << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          } catch (const out_of_range& e) {
              cerr << "Error: Invalid edge weight '" << weight_string << "' on line " << line_number << "." << endl;
              delete_matrix(distances, num_vertices);
              return 1;
          }

          //Populate the distances matrix
          //vertex_one corresponds to the starting vertex, which corresponds to a row index
          //vertex_two corresponds to the destination vertex, which corresponds to a column index
          distances[line[0] - 'A'][line[2] - 'A'] = weight;

          ++line_number;
      }
      // Don't forget to close the file.
      input_file.close();

      const string& distance_label = "Distance matrix:";
      display_table(distances, distance_label, num_vertices); //display distance matrix

       //Creates an identical matric to distances that we will use and modify to create a matrix of shortest paths
       long** paths = new long*[rows];
       for (int i = 0; i < rows; i++) {
           paths[i] = new long[cols];
           for (int j = 0; j < cols; j++) {
               paths[i][j] = distances[i][j]; // Copy directly from distances
           }
       }

       //Creates another matrix called intermediates that will hold the steps needed to take to complete the paths
       long** intermediates = new long*[rows];
       for (int i = 0; i < rows; i++){
           intermediates[i] = new long[cols];
           for (int j = 0; j < cols; j++){
               intermediates[i][j] = numeric_limits<long>::max();
           }
       }

       const string& path_label = "Path lengths:";
       long** short_paths = floyd(paths, intermediates, num_vertices);
       display_table(short_paths, path_label, num_vertices); //display path matrix
     
       const string& intermediate_label = "Intermediate vertices:";
       display_table(intermediates, intermediate_label, num_vertices, true); //display intermediate matrix

       display_paths(short_paths, intermediates, num_vertices);




       //delete memory on the heap
       delete_matrix(distances, num_vertices);
       delete_matrix(intermediates, num_vertices);
       delete_matrix(short_paths, num_vertices);




  } catch (const ifstream::failure &f) {
      cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
      return 1;
  }




  return 0;
}