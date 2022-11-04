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

#define executeSolution(i)                                                                                        \
	if (solution_##i##_elapsed_time < _time_limit_)                                                                 \
	{                                                                                                               \
		solve_##i(final_position);                                                                                    \
		if (print_this_one != 0)                                                                                      \
		{                                                                                                             \
			sprintf(file_name, "%03d_%02d.pdf", final_position, i);                                                     \
			make_custom_pdf_file(file_name, final_position,                                                             \
													 &max_road_speed[0], solution_##i##_best.n_moves,                                       \
													 &solution_##i##_best.positions[0], solution_##i##_elapsed_time,                        \
													 solution_##i##_count, "Modified recursion");                                           \
		}                                                                                                             \
		printf(" %3d %16lu %9.3e |", solution_##i##_best.n_moves, solution_##i##_count, solution_##i##_elapsed_time); \
	}                                                                                                               \
	else                                                                                                            \
	{                                                                                                               \
		solution_##i##_best.n_moves = -1;                                                                             \
		printf("                                |");                                                                  \
	}

#define _max_road_size_ 800 // the maximum problem size
#define _min_road_speed_ 2	// must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_ 9	// must not be larger than 9 (only because of the PDF figure)

//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include "./elapsed_time.h"
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
	int n_moves;												// the number of moves (the number of positions is one more than the number of moves)
	int positions[1 + _max_road_size_]; // the positions (the first one must be zero)
} solution_t;

//
// the (very inefficient) recursive solution given to the students
//

static solution_t solution_1, solution_1_best;
static double solution_1_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count; // effort dispended solving the problem

static void solution_1_recursion(int move_number, int position, int speed, int final_position)
{
	int i, new_speed;

	// record move
	solution_1_count++;
	solution_1.positions[move_number] = position;
	// is it a solution?
	if (position == final_position && speed == 1)
	{
		// is it a better solution?
		if (move_number < solution_1_best.n_moves)
		{
			solution_1_best = solution_1;
			solution_1_best.n_moves = move_number;
		}
		return;
	}
	// no, try all legal speeds
	for (new_speed = speed - 1; new_speed <= speed + 1; new_speed++)
		if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
		{
			for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
				;
			if (i > new_speed)
				solution_1_recursion(move_number + 1, position + new_speed, new_speed, final_position);
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
	solution_1_recursion(0, 0, 0, final_position);
	solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}

//
// Our solution n. 1 using optimized brute force
//

static solution_t solution_2, solution_2_best;
static double solution_2_elapsed_time; // time it took to solve the problem
static unsigned long solution_2_count; // effort dispended solving the problem

static void solution_2_recursion(int move_number, int position, int speed, int final_position)
{
	int i, new_speed;

	// record move
	solution_2_count++;
	solution_2.positions[move_number] = position;
	// is it a solution?
	if (position == final_position && speed == 1)
	{
		// is it a better solution?
		if (move_number < solution_2_best.n_moves)
		{
			solution_2_best = solution_2;
			solution_2_best.n_moves = move_number;
		}
		return;
	}
	// no, try all legal speeds
	for (new_speed = speed - 1; new_speed <= speed + 1; new_speed++)
		if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
		{
			for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
				;
			if (i > new_speed)
			{
				if (move_number + 1 < solution_2_best.n_moves)
					solution_2_recursion(move_number + 1, position + new_speed, new_speed, final_position);
			}
		}
}

static void solve_2(int final_position)
{
	if (final_position < 1 || final_position > _max_road_size_)
	{
		fprintf(stderr, "solve_2: bad final_position\n");
		exit(1);
	}
	solution_2_elapsed_time = cpu_time();
	solution_2_count = 0ul;
	solution_2_best.n_moves = final_position + 100;
	solution_2_recursion(0, 0, 0, final_position);
	solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;
}

//
// Our solution n. 2 using optimized brute force
//

static solution_t solution_3, solution_3_best;
static double solution_3_elapsed_time; // time it took to solve the problem
static unsigned long solution_3_count; // effort dispended solving the problem

static int solution_3_recursion(int move_number, int position, int speed, int final_position)
{
	int i, new_speed;

	// record move
	solution_3_count++;
	solution_3.positions[move_number] = position;
	// is it a solution?
	if (position == final_position && speed == 1)
	{
		// is it a better solution?
		if (move_number < solution_3_best.n_moves)
		{
			solution_3_best = solution_3;
			solution_3_best.n_moves = move_number;
		}
		return 1;
	}
	// no, try all legal speeds
	for (new_speed = speed + 1; new_speed >= speed - 1; new_speed--)
	{
		if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
		{
			for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++) // Confirma se todas as roads têm velocidade aceitável para a new_speed
				;
			if (i > new_speed)
			{ // Este testa, se for verdade (se todas as roads tiverem velocidade máxima aceitavel), chama outra vez esta função
				int d = solution_3_recursion(move_number + 1, position + new_speed, new_speed, final_position);
				if (d == 1)
					return 1;
			}
		}
	}
	return 0;
}

static void solve_3(int final_position)
{
	if (final_position < 1 || final_position > _max_road_size_)
	{
		fprintf(stderr, "solve_3: bad final_position\n");
		exit(1);
	}
	solution_3_elapsed_time = cpu_time();
	solution_3_count = 0ul;
	solution_3_best.n_moves = final_position + 100;
	solution_3_recursion(0, 0, 0, final_position);
	solution_3_elapsed_time = cpu_time() - solution_3_elapsed_time;
}

//
// Our solution n. 3 using optimized brute force
//
typedef struct
{
	int speed;								 // the current speed
	int blacklisted_speeds[3]; // the speeds that are not allowed at this position
	int i											 // the number of blacklisted speeds
} solution_t_info;

static solution_t solution_4, solution_4_best;
static solution_t_info solution_4_info[_max_road_size_ + 1];
static double solution_4_elapsed_time; // time it took to solve the problem
static unsigned long solution_4_count; // effort dispended solving the problem

static int check_speed(int position, int speed)
{
	for (int i = 0; i < solution_4_info[position].i; i++)
		if (solution_4_info[position].blacklisted_speeds[i] == speed)
			return 1;
	return 0;
}

static void solution_4_recursion(int speed, int position, int debt_pos, int debt, int move_number, int final_position)
{
	int i, new_speed, new_position, speed_limit;

	// record move
	solution_4_count++;
	solution_4.positions[move_number] = position;

	if (position == final_position && speed == 1)
	{
		// is it a better solution?
		if (move_number < solution_4_best.n_moves)
		{
			solution_4_best = solution_4;
			solution_4_best.n_moves = move_number;
		}
		return;
	}

	if (debt < -2)
	{
		int previous_position = solution_4.positions[move_number - 1];
		int previous_speed = solution_4_info[previous_position].speed;

		// reset current info
		solution_4_info[position].speed = 0;
		solution_4_info[position].i = 0;

		solution_4_recursion(previous_speed, previous_position, 0, 0, move_number - 1, final_position);
		return;
	}

	new_speed = speed + 1 + (position == debt_pos ? debt : 0);
	new_position = position + new_speed;

	for (i = 1; i <= new_speed; i++)
	{
		speed_limit = max_road_speed[position + i];

		if (new_position >= final_position)
			speed_limit = 1;

		// fail to run in this road
		if (new_speed > speed_limit || check_speed(position, new_speed - speed))
		{
			solution_4_recursion(speed, position, position, debt - 1, move_number, final_position);
			return;
		}
	}

	// ok to run in this road
	solution_4_info[position].speed = speed;
	solution_4_info[position].blacklisted_speeds[solution_4_info[position].i] = new_speed - speed;
	solution_4_info[position].i++;
	solution_4_recursion(new_speed, new_position, 0, 0, move_number + 1, final_position);
}

static void solve_4(int final_position)
{
	if (final_position < 1 || final_position > _max_road_size_)
	{
		fprintf(stderr, "solve_4: bad final_position\n");
		exit(1);
	}

	for (int i = 0; i < _max_road_size_ + 1; i++)
	{
		solution_4_info[i].speed = 0;
		solution_4_info[i].i = 0;

		for (int j = 0; j < 3; j++)
			solution_4_info[i].blacklisted_speeds[j] = 100;
	}

	solution_4_elapsed_time = cpu_time();
	solution_4_count = 0ul;
	solution_4_best.n_moves = final_position + 100;
	solution_4_recursion(0, 0, 0, 0, 0, final_position);
	solution_4_elapsed_time = cpu_time() - solution_4_elapsed_time;
}

//
// Our solution n. 4 using dynamic programming
//

typedef struct
{
	int n_moves;												// the number of moves (the number of positions is one more than the number of moves)
	int positions[1 + _max_road_size_]; // the positions (the first one must be zero)
	int speed;													// the speed at the end of this solution
} solution_t_previous;

typedef struct
{
	int save;	 // if this is a valid solution
	int saved; // if this solution has been saved
	int valid; // if this solution is valid
} run_status_t;

static solution_t solution_5, solution_5_best;
static solution_t_previous solution_5_previous;
static double solution_5_elapsed_time; // time it took to solve the problem
static unsigned long solution_5_count; // effort dispended solving the problem

// Este array guarda a quantidade de posições necessárias para chegar à velocidade 1.
// Exemplo: sumCache[5] = 15, ou seja, para chegar à velocidade 1, é necessário percorrer 15 posições.
// velocidades |05|--|--|--|--|04|--|--|--|03|--|--|02|--|01|
// nº de casas |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1|
static int sumCache[10] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 45};

static void solution_5_dynamic(int move_number, int position, int speed, int final_position)
{
	run_status_t run_status;
	run_status.save = 0;
	run_status.saved = 0;

	while (position != final_position)
	{
		// Vai aumentar o effort
		solution_5_count++;
		// Vai guardar a posição atual onde está a fazer o movimento
		solution_5.positions[move_number] = position;

		// Vai experimentar as velocidades possiveis: aumentar, manter, diminuir
		for (int i = 1; i >= -1; i--)
		{
			// Definição da nova velocidade dependendo da velocidade atual e da velocidade que se quer aumentar, diminuir ou manter
			int new_speed = speed + i;
			// Definição da nova posição dependendo da posição atual e da nova velocidade definida
			int new_position = position + new_speed;
			// À partida a solução é válida, mas se for encontrada alguma situação que a torne inválida, esta variável vai ser alterada para 0
			run_status.valid = 1;

			// Se a velocidade for menor que 1 ou inválida para a posição atual, a solução é inválida e passa-se para a próxima velocidade
			if (new_speed < 1 || new_speed > _max_road_speed_ || max_road_speed[position] < new_speed)
				continue;

			int positions_array[sumCache[new_speed]];

			// Vai preencher o array com as posições que se vão percorrer para chegar à velocidade 1
			int temp_index = 0;
			for (int j = new_speed; j > 0; j--)
			{
				for (int k = 0; k < j; k++)
				{
					positions_array[temp_index] = j;
					temp_index++;
				}
			}

			// Vai verificar se a nova velocidade é válida para as próximas posições
			for (int j = 1; j <= sumCache[new_speed]; j++)
			{
				// Se a velocidade for inválida para a posição atual + j, a solução é inválida
				if (max_road_speed[position + j] < positions_array[j - 1])
				{
					run_status.valid = 0;
					break;
				}

				// Se a posição atual + j for maior que a posição final, a solução é inválida
				// e a solução é guardada para ser usada na proxima chamada quando a posição atual for menor que a posição final
				if (position + j > final_position)
				{
					run_status.save = 1;
					run_status.valid = 0;
					break;
				}
			}

			// Se a solução for válida, a solução é guardada para ser usada na proxima chamada quando a posição atual for menor que a posição final
			if (run_status.save && !run_status.saved)
			{
				// Guarda as posições que já foram percorridas
				for (int i = 0; i < _max_road_size_ + 1; i++)
					solution_5_previous.positions[i] = solution_5.positions[i];
				// Guarda o número de movimentos que já foram feitos
				solution_5_previous.n_moves = move_number;
				// Guarda a velocidade atual
				solution_5_previous.speed = speed;
				// Indica que a solução foi guardada
				run_status.saved = 1;
			}

			// Se a solução for válida a posição atual é atualizada para a nova posição e a velocidade atual é atualizada para a nova velocidade
			// e o número de movimentos é incrementado
			if (run_status.valid)
			{
				position = new_position;
				speed = new_speed;
				move_number++;
				// Visto que a solução é válida, não se precisa de ir verificar as outras velocidades
				break;
			}
		}
	}

	solution_5.positions[move_number] = position;
	solution_5_best = solution_5;
	solution_5_best.n_moves = move_number;
}

static void solve_5(int final_position)
{
	if (final_position < 1 || final_position > _max_road_size_)
	{
		fprintf(stderr, "solve_5: bad final_position\n");
		exit(1);
	}
	solution_5_elapsed_time = cpu_time();
	solution_5_count = 0ul;
	solution_5_best.n_moves = final_position + 100;

	// Get the values from the previous run
	solution_5.n_moves = solution_5_previous.n_moves;
	for (int i = 0; i < _max_road_size_ + 1; i++)
		solution_5.positions[i] = solution_5_previous.positions[i];

	solution_5_dynamic(solution_5_previous.n_moves, solution_5_previous.positions[solution_5_previous.n_moves], solution_5_previous.speed, final_position);
	solution_5_elapsed_time = cpu_time() - solution_5_elapsed_time;
}

//
// example of the slides
//

static void example(void)
{
	int i, final_position;

	srandom(0xAED2022);
	init_road_speeds();
	final_position = 30;
	solve_1(final_position);
	make_custom_pdf_file("example.pdf", final_position, &max_road_speed[0], solution_1_best.n_moves, &solution_1_best.positions[0], solution_1_elapsed_time, solution_1_count, "Plain recursion");
	printf("mad road speeds:");
	for (i = 0; i <= final_position; i++)
		printf(" %d", max_road_speed[i]);
	printf("\n");
	printf("positions:");
	for (i = 0; i <= solution_1_best.n_moves; i++)
		printf(" %d", solution_1_best.positions[i]);
	printf("\n");
}

//
// main program
//

int main(int argc, char *argv[argc + 1])
{
#define _time_limit_ 3600.0
	int n_mec, final_position, print_this_one;
	char file_name[64];

	// generate the example data
	if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
	{
		example();
		return 0;
	}
	// initialization
	n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
	srandom((unsigned int)n_mec);
	init_road_speeds();
	// run all solution methods for all interesting sizes of the problem
	final_position = 1;
	solution_1_elapsed_time = 0.0;
	printf("    + --- ---------------- --------- +\n");
	printf("    |                plain recursion |\n");
	printf("--- + --- ---------------- --------- +\n");
	printf("  n | sol            count  cpu time |\n");
	printf("--- + --- ---------------- --------- +\n");
	while (final_position <= _max_road_size_ /* && final_position <= 20*/)
	{
		print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
		printf("%3d |", final_position);

		executeSolution(4);

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
#undef _time_limit_
}