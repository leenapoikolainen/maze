/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

/* Function to find a given marker */
bool find_marker(char ch, char** maze, int height, int width,
		 		int& row, int& column);

/* Function that validates if a given path is accpetable */
bool valid_solution(const char* path, char** maze,
		    		int height, int width);

bool valid_move(char** maze, char direction, char content, int height, int width,
				int& row, int& column);

bool valid_direction(char ch);

const char* find_path(char** maze, int height, int width, char start, char end);

bool valid_way_exists(char** maze, int height, int width, int row, int column, 
						char destination, char *success);

bool can_move_east(char** maze, int height, int width, int row, int column, char target);
bool can_move_west(char** maze, int height, int width, int row, int column, char target);
bool can_move_south(char** maze, int height, int width, int row, int column, char target);
bool can_move_north(char** maze, int height, int width, int row, int column, char target);
