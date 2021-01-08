#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include "maze.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

bool find_marker(char ch, char** maze, int height, int width,
		 int& row, int& column) {

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (maze[i][j] == ch) {
	      row = i;
	      column = j;
	      return true;
      }
    }
  }
  row = column = -1;
  return false;
}

// Checks that given character is a valid direction
bool valid_direction(char ch) {
  if (ch == 'N' || ch == 'E' || ch == 'S' || ch == 'W')
    return true;
  else
    return false;
}

// Checks that given move is valid
bool valid_move(char** maze, char direction, char content ,int height, int width,
		int& row, int& column){
  // Case east
  if (direction == 'E' && column + 1 < width && maze[row][column + 1] == content){
    column++;
    return true;
  }
  // Case south
  else if (direction == 'S' && row + 1 < height && maze[row+1][column] == content){
    row++;
    return true;
  }
  // Case west
  else if (direction == 'W' && column - 1 >= 0 && maze[row][column-1] == content){
    column--;
    return true;
  }
  // Case north
  else if (direction == 'N' && row - 1 >= 0 && maze[row-1][column] == content){
    row--;
    return true;
  }
  else {
    row = column = -1;
    return false;
  }
}

bool valid_solution(const char* path, char** maze,int height, int width){
  // Set the starting position
  int row, column;
  if (!find_marker('>', maze, height, width, row, column)) {
    cout << "Start not found " << endl;
    return false;
  }

  int length = strlen(path);
 
  // Check all the moves excep the last
  for (int i = 0; i < length - 1; i++)
  {
    // Check direction
    if (!valid_direction(path[i])) {
      cout << path[i] << " is not a valid direction" << endl;
    }
    // Check that the square is empty
    if (!valid_move(maze, path[i], ' ', height, width, row, column)){
      return false;
    }  
  }

  // last move shud be the end
  if (!valid_direction(path[length-1]))
    return false;

  if (!valid_move(maze, path[length-1], 'X', height, width, row, column)) {
     return false;
  }
  return true;
}

const char* find_path(char** maze, int height, int width, char start, char end){
  const char *error = "No solution";
  cout << "start: " << start << " end: " << end << endl;
  // Find starting position
  int row, column;
  if (!find_marker(start, maze, height, width, row, column)) {
    return error;
  }
  maze[row][column] = '#';

  char success[2000];
  strcpy(success, "");
  if (valid_way_exists(maze, height, width, row, column, end, success)){ 
    // empty dead cells
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (maze[i][j] == 'D')
          maze[i][j] = ' ';
      }
    }
    return success;
  }
  else {
    return error;
  }
}

bool valid_way_exists(char** maze, int height, int width, int row, int column,
	       char destination, char *success) {
  // If target is reached, set the position to '#' and return true;
  if (maze[row][column] == destination) {
    maze[row][column] = '#';
    *success = '\0';
    return true;
  }

  // Test all directions recursively, if valid move, call the function again
  // When dead end is faced, marke the cell as 'dead' = 'D'
  if (can_move_east(maze, height, width, row, column, destination)){
    *success++ = 'E';
    if (maze[row][column+1] != destination){
      maze[row][column+1] = '#';
    }
    if (valid_way_exists(maze, height, width, row, column + 1, destination, success)){
      return true;
    }
    success--;
    maze[row][column + 1] = 'D';
  }

  if (can_move_south(maze, height, width, row, column, destination)) {
    *success++ = 'S';
    if (maze[row + 1][column] != destination) {
      maze[row + 1][column] = '#';
    }
    if (valid_way_exists(maze, height, width, row + 1, column, destination, success))
      return true;
    success--;
    maze[row + 1][column] = 'D';
  }
  
  if (can_move_west(maze, height, width, row, column, destination)) {
    *success++ = 'W';
    if (maze[row][column - 1] != destination) {
      maze[row][column - 1] = '#';
    }
    if(valid_way_exists(maze, height, width, row, column - 1, destination, success))
      return true;
    success--;
    maze[row][column - 1] = 'D';
  }

  if (can_move_north(maze, height, width, row, column, destination)){
    *success++ = 'N';
    if (maze[row - 1][column] != destination) {
      maze[row - 1][column] = '#';
    }
    if (valid_way_exists(maze, height, width, row - 1, column, destination, success))
      return true;
    success--;
    maze[row -1 ][column] = 'D';
  }
  return false;
}

bool can_move_east(char** maze, int height, int width, int row, int column, char target) {
  if (column + 1 >= width)
    return false;
  if (maze[row][column + 1] == ' ' || maze[row][column + 1] == target){
    return true;
  }
  else
    return false;
}

bool can_move_west(char** maze, int height, int width, int row, int column, char target) {
  if (column - 1 < 0)
    return false;
  if (maze[row][column - 1] == ' ' || maze[row][column - 1] == target) {
    return true;
  }
  return false;
}

bool can_move_south(char** maze, int height, int width, int row, int column, char target) {
  if (row + 1 >= height)
    return false;
  if (maze[row + 1][column] == ' ' || maze[row + 1][column] == target){
    return true;
  }
  return false;
}

bool can_move_north(char** maze, int height, int width, int row, int column, char target) {
  if (row - 1 < 0)
    return false;
  if (maze[row - 1][column] == ' ' || maze[row - 1][column] == target){
    return true;
  }
  return false;
}
