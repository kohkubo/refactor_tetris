#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#define ROW 20
#define COL 15

typedef uint8_t t_field[ROW][COL];

t_field g_field = {};
int g_score = 0;
bool g_game_on = true;
suseconds_t g_timer = 400000;
int g_decrease = 1000;

typedef struct
{
	int row;
	int col;
} t_point;

typedef struct
{
	char **shape;
	size_t width;
} t_mino_shape;

typedef struct
{
	t_mino_shape mino_shape;
	t_point pos;
} t_mino;
t_mino g_current;

// clang-format off
const t_mino_shape g_mino_shapes[] = {
	{
		.shape = (char *[]){
			(char[]){0, 1, 1},
			(char[]){1, 1, 0},
			(char[]){0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){1, 1, 0},
			(char[]){0, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3},
	{
		.shape = (char *[]){
			(char[]){0, 1, 0},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){0, 0, 1},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){1, 0, 0},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
	 	.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){1, 1},
			(char[]){1, 1}
		},
		.width = 2
	},
	{
		.shape = (char *[]){
			(char[]){0, 0, 0, 0},
			(char[]){1, 1, 1, 1},
			(char[]){0, 0, 0, 0},
			(char[]){0, 0, 0, 0}
		},
		.width = 4
	}
	};
// clang-format on

t_mino_shape copy_mino_shape(t_mino_shape mino_shape)
{
	t_mino_shape new_mino_shape;
	new_mino_shape.width = mino_shape.width;
	new_mino_shape.shape = (char **)malloc(sizeof(char *) * mino_shape.width);
	for (size_t i = 0; i < mino_shape.width; i++)
	{
		new_mino_shape.shape[i] = (char *)malloc(sizeof(char) * mino_shape.width);
		memcpy(new_mino_shape.shape[i], mino_shape.shape[i], mino_shape.width);
	}
	return new_mino_shape;
}

t_mino copy_mino(t_mino mino)
{
	t_mino new_shape = mino;
	new_shape.mino_shape = copy_mino_shape(mino.mino_shape);
	return new_shape;
}

void free_mino(t_mino mino)
{
	for (size_t i = 0; i < mino.mino_shape.width; i++)
	{
		free(mino.mino_shape.shape[i]);
	}
	free(mino.mino_shape.shape);
}

bool can_place_in_field(t_mino mino, t_point dest)
{
	char **shape = mino.mino_shape.shape;
	for (int i = 0; i < (int)mino.mino_shape.width; i++)
	{
		for (int j = 0; j < (int)mino.mino_shape.width; j++)
		{
			if (!shape[i][j]) {
				continue ;
			}
			if (dest.col + j < 0 ||
				dest.col + j >= COL ||
				dest.row + i >= ROW ||
				g_field[dest.row + i][dest.col + j]) {
				return false;
			}
		}
	}
	return true;
}

void rotate_right(t_mino mino)
{
	t_mino temp = copy_mino(mino);
	for (size_t i = 0; i < mino.mino_shape.width; i++)
	{
		for (size_t j = 0, k = mino.mino_shape.width - 1; j < mino.mino_shape.width; j++, k--)
		{
			mino.mino_shape.shape[i][j] = temp.mino_shape.shape[k][i];
		}
	}
	free_mino(temp);
}

void update_field(t_field field)
{
	for (size_t i = 0; i < g_current.mino_shape.width; i++)
	{
		for (size_t j = 0; j < g_current.mino_shape.width; j++)
		{
			if (g_current.mino_shape.shape[i][j])
			{
				field[g_current.pos.row + i][g_current.pos.col + j] = g_current.mino_shape.shape[i][j];
			}
		}
	}
}

static void print_header()
{
	for (size_t i = 0; i < COL / 2; i++)
		printw(" ");
	printw("42 Tetris\n");
}

static void print_body()
{
	t_field current_field = {};

	update_field(current_field);
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t j = 0; j < COL; j++)
		{
			printw("%c ", g_field[i][j] || current_field[i][j] ? '#' : '.');
		}
		printw("\n");
	}
}

void update_screen()
{
	clear();
	print_header();
	print_body();
	printw("\nScore: %d\n", g_score);
}

t_mino generate_random_mino()
{
	t_mino new_mino;
	new_mino.mino_shape = copy_mino_shape(g_mino_shapes[rand() % sizeof(g_mino_shapes) / sizeof(t_mino_shape)]);
	new_mino.pos.col = rand() % (COL - new_mino.mino_shape.width + 1);
	new_mino.pos.row = 0;
	return new_mino;
}

t_point down(t_point pos)
{
	return (t_point){pos.row + 1, pos.col};
}

t_point left(t_point pos)
{
	return (t_point){pos.row, pos.col - 1};
}

t_point right(t_point pos)
{
	return (t_point){pos.row, pos.col + 1};
}

void key_action_down()
{
	if (can_place_in_field(g_current, down(g_current.pos)))
		g_current.pos = down(g_current.pos);
}

void key_action_left()
{
	if (can_place_in_field(g_current, left(g_current.pos)))
		g_current.pos = left(g_current.pos);
}

void key_action_right()
{
	if (can_place_in_field(g_current, right(g_current.pos)))
		g_current.pos = right(g_current.pos);
}

void key_action_rotate()
{
	t_mino temp = copy_mino(g_current);
	rotate_right(temp);
	if (can_place_in_field(temp, temp.pos))
		rotate_right(g_current);
	free_mino(temp);
}

typedef void (*t_keyhook_func)(t_mino *);

t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

void end_game()
{
	endwin();
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t j = 0; j < COL; j++)
		{
			printf("%c ", g_field[i][j] ? '#' : '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", g_score);
}

struct timeval g_before_now;
struct timeval g_now;
bool hasToUpdate()
{
	return ((suseconds_t)(g_now.tv_sec * 1000000 + g_now.tv_usec) - ((suseconds_t)g_before_now.tv_sec * 1000000 + g_before_now.tv_usec)) > g_timer;
}

void init_game()
{
	srand(time(0));
	g_score = 0;
	initscr();
	gettimeofday(&g_before_now, NULL);
	timeout(1);
	g_current = generate_random_mino();
	// assert(can_place_in_field(g_current, g_current.pos));
	g_keyhooks['s'] = key_action_down;
	g_keyhooks['a'] = key_action_left;
	g_keyhooks['d'] = key_action_right;
	g_keyhooks['w'] = key_action_rotate;
}

void handle_filled_lines()
{
	size_t count = 0;
	for (size_t n = 0; n < ROW; n++)
	{
		size_t sum = 0;
		for (size_t m = 0; m < COL; m++)
		{
			sum += g_field[n][m];
		}
		if (sum == COL)
		{
			count++;
			for (size_t k = n; k > 0; k--)	{
				memcpy(g_field[k], g_field[k - 1], COL);
			}
			memset(g_field[0], 0, COL);
			g_timer -= g_decrease--;
		}
	}
	g_score += 100 * count;
}

int main()
{
	init_game();
	update_screen();
	while (g_game_on)
	{
		int c = getch();
		if (c != ERR && g_keyhooks[c])
		{
			g_keyhooks[c](&g_current);
			update_screen();
		}
		gettimeofday(&g_now, NULL);
		if (hasToUpdate())
		{
			if (can_place_in_field(g_current, down(g_current.pos)))
			{
				g_current.pos = down(g_current.pos);
			}
			else
			{
				update_field(g_field);
				handle_filled_lines();
				free_mino(g_current);
				g_current = new_shape;
				if (!can_place_in_field(g_current, g_current.pos))
				{
					g_game_on = false;
				}
			}
			update_screen();
			gettimeofday(&g_before_now, NULL);
		}
	}
	free_mino(g_current);
	end_game();
	return 0;
}
