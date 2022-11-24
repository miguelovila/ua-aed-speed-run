//
// AED, August 2022 (Tomás Oliveira e Silva)
//
// First practical assignement (speed run)
//
// Compile using either
//   cc -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -lm
// or
//   cc -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -lm -lz
//
// Place your student numbers and names here
//   N.Mec. 107276  Name: Miguel Vila
//   N.Mec. 107993  Name: Francisco Ribeiro
//   N.Mec. 108671  Name: Guilherme Vieira
//
//
// static configuration
//

#define executeSolution(i) 																												\
		solve_##i(final_position); 																										\
		if (print_this_one != 0) 																										\
		{ 																																\
			sprintf(file_name, "%03d_%02d.pdf", final_position, i); 																	\
			make_custom_pdf_file(file_name, final_position, 																			\
				&max_road_speed[0], solution_ ## i ## _best.n_moves, 																	\
				&solution_ ## i ## _best.positions[0], solution_ ## i ## _elapsed_time, 												\
				solution_ ## i ## _count, "A valid solution"); 																			\
		} 																																\
		printf(" %3d %16lu %9.3e |", solution_ ## i ## _best.n_moves, solution_ ## i ## _count, solution_ ## i ## _elapsed_time); 		

#define _max_road_size_ 800 // the maximum problem size
#define _min_road_speed_ 2	// must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_ 9	// must not be larger than 9 (only because of the PDF figure)

//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include "../P02/elapsed_time.h"
#include "make_custom_pdf.c"

//
// road stuff
//

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
	double speed;
	int i;

	for (i = 0; i <= _max_road_size_; i++)
	{
		speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
		max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;
		if (max_road_speed[i] < _min_road_speed_)
			max_road_speed[i] = _min_road_speed_;
		if (max_road_speed[i] > _max_road_speed_)
			max_road_speed[i] = _max_road_speed_;
	}
}

//
// description of a solution
//

typedef struct
{
	int n_moves;						// the number of moves (the number of positions is one more than the number of moves)
	int positions[1 + _max_road_size_]; // the positions (the first one must be zero)
} solution_t;

// 
// solution 1: a possible solution
// 

static solution_t solution_1, solution_1_best;
static double solution_1_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count; // effort dispended solving the problem

// A road is subdivided into segments of approximately the same length.
//  The speed limit on each segment is given by the array max_road_speed.
//  The car starts at the beginning of the road with speed 0 and must reach the end of the road with speed 1.
//  The car can maintain its speed, accelerate or decelerate by 1 unit of speed per move.
//  The car cannot accelerate beyond the speed limit of the current segment.
//  The car cannot decelerate below speed 0.
//  The car cannot accelerate beyond speed 9.
//  The car cannot accelerate beyond the speed limit of the next segments.
//    For example, if the current segment has speed limit 5 and in the next 5 segments
//      there is a segment with speed limit 3, then the car cannot accelerate beyond speed 3, so it must decelerate.
// The next position is the current position plus the current speed.

static void solution_1_execute(void)
{
	int i, new_speed, current_speed, current_position;
	solution_1_count++;
	current_position = 0;
	current_speed = 0;
	for (i = 0; i < solution_1_best.n_moves; i++)
	{
		solution_1.positions[i] = current_position;
		new_speed = current_speed;
		if (current_speed < max_road_speed[current_position])
			new_speed++;
		if (current_speed > 0)
		{
			if (current_position + current_speed + 1 > _max_road_size_)
				new_speed--;
			else if (current_speed > max_road_speed[current_position + current_speed + 1])
				new_speed--;
		}
		current_position += current_speed;
		current_speed = new_speed;
	}
	solution_1.positions[i] = current_position;
	solution_1.n_moves = i;

	// found a solution
	if (current_position == _max_road_size_ && current_speed == 1)
	{
		if (solution_1.n_moves < solution_1_best.n_moves)
		{
			solution_1_best = solution_1;
			printf(" %3d", solution_1_best.n_moves);
		}
	}
}

static void solve_1(int final_position)
{
	if (final_position < 1 || final_position > _max_road_size_)
	{
		fprintf(stderr, "solve_1: bad final_position\n");
		exit(1);
	}
	solution_1_elapsed_time = cpu_time();
	solution_1_count = 0ul;
	solution_1_best.n_moves = final_position + 100;
	solution_1_execute();
	solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}

//
// main program
//

int main(int argc, char *argv[argc + 1])
{
	int n_mec, final_position, print_this_one;
	char file_name[64];
	
	// initialization
	n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
	srandom((unsigned int)n_mec);
	init_road_speeds();
	// run all solution methods for all interesting sizes of the problem
	final_position = 1;
	printf("    + --- ---------------- --------- +\n");
	printf("    |                plain recursion |\n");
	printf("--- + --- ---------------- --------- +\n");
	printf("  n | sol            count  cpu time |\n");
	printf("--- + --- ---------------- --------- +\n");
	while (final_position <= _max_road_size_ /* && final_position <= 20*/)
	{
		print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
		printf("%3d |", final_position);
		
		executeSolution(4)

		// done
		printf("\n");
		fflush(stdout);
		// new final_position
		if (final_position < 50)
			final_position += 1;
		else if (final_position < 100)
			final_position += 5;
		else if (final_position < 200)
			final_position += 10;
		else
			final_position += 20;
	}
	printf("--- + --- ---------------- --------- +\n");
	return 0;
}