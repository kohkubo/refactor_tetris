#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define ROW 20
#define COL 15

char g_field[ROW][COL] = {0};
int g_final = 0;
bool g_game_on = true;
suseconds_t g_timer = 400000;
int g_decrease = 1000;

typedef struct
{
	char **shape;
	size_t width;
} t_mino_shape;

typedef struct
{
	t_mino_shape mino_shape;
	int row;
	int col;
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
		for (size_t j = 0; j < mino_shape.width; j++) {
			new_mino_shape.shape[i][j] = mino_shape.shape[i][j];
		}
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

bool FunctionCP(t_mino mino)
{
	char **shape = mino.mino_shape.shape;
	for (size_t i = 0; i < mino.mino_shape.width; i++)
	{
		for (size_t j = 0; j < mino.mino_shape.width; j++)
		{
			if ((mino.col + j < 0 || mino.col + j >= COL || mino.row + i >= ROW))
			{
				if (shape[i][j])
				{
					return false;
				}
			}
			else if (g_field[mino.row + i][mino.col + j] && shape[i][j])
				return false;
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

void print_field()
{
	char Buffer[ROW][COL] = {0};
	for (size_t i = 0; i < g_current.mino_shape.width; i++)
	{
		for (size_t j = 0; j < g_current.mino_shape.width; j++)
		{
			if (g_current.mino_shape.shape[i][j])
				Buffer[g_current.row + i][g_current.col + j] = g_current.mino_shape.shape[i][j];
		}
	}
	clear();
	for (size_t i = 0; i < COL - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t j = 0; j < COL; j++)
		{
			printw("%c ", (g_field[i][j] + Buffer[i][j]) ? '#' : '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", g_final);
}

struct timeval g_before_now;
struct timeval g_now;
bool hasToUpdate()
{
	return ((suseconds_t)(g_now.tv_sec * 1000000 + g_now.tv_usec) - ((suseconds_t)g_before_now.tv_sec * 1000000 + g_before_now.tv_usec)) > g_timer;
}

t_mino generate_random_mino()
{
	t_mino new_mino;
	new_mino.mino_shape = copy_mino_shape(g_mino_shapes[rand() % sizeof(g_mino_shapes) / sizeof(t_mino_shape)]);
	new_mino.col = rand() % (COL - new_mino.mino_shape.width + 1);
	new_mino.row = 0;
	return new_mino;
}

void init_game()
{
	srand(time(0));
	g_final = 0;
	initscr();
	gettimeofday(&g_before_now, NULL);
	timeout(1);
	g_current = generate_random_mino();
	if (!FunctionCP(g_current))
	{
		g_game_on = false;
	}
	print_field();
}

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
	printf("\nScore: %d\n", g_final);
}

int main()
{
	init_game();
	while (g_game_on)
	{
		int c = getch();
		if (c != ERR)
		{
			t_mino temp = copy_mino(g_current);
			switch (c)
			{
			case 's':
				temp.row++; // move down
				if (FunctionCP(temp))
				{
					g_current.row++;
				}
				else
				{
					for (size_t i = 0; i < g_current.mino_shape.width; i++)
					{
						for (size_t j = 0; j < g_current.mino_shape.width; j++)
						{
							if (g_current.mino_shape.shape[i][j])
							{
								g_field[g_current.row + i][g_current.col + j] = g_current.mino_shape.shape[i][j];
							}
						}
					}
					int count = 0;
					for (size_t n = 0; n < ROW; n++)
					{
						int sum = 0;
						for (size_t m = 0; m < COL; m++)
						{
							sum += g_field[n][m];
						}
						if (sum == COL)
						{
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
							{
								for (l = 0; l < COL; l++)
								{
									g_field[k][l] = g_field[k - 1][l];
								}
							}
							for (l = 0; l < COL; l++)
							{
								g_field[k][l] = 0;
							}
							g_timer -= g_decrease--;
						}
					}
					g_final += 100 * count;
					t_mino new_shape = generate_random_mino();
					free_mino(g_current);
					g_current = new_shape;
					if (!FunctionCP(g_current))
					{
						g_game_on = false;
					}
				}
				break;
			case 'd':
				temp.col++;
				if (FunctionCP(temp))
					g_current.col++;
				break;
			case 'a':
				temp.col--;
				if (FunctionCP(temp))
					g_current.col--;
				break;
			case 'w':
				rotate_right(temp);
				if (FunctionCP(temp))
					rotate_right(g_current);
				break;
			}
			free_mino(temp);
			print_field();
		}
		gettimeofday(&g_now, NULL);
		if (hasToUpdate())
		{
			t_mino temp = copy_mino(g_current);
			switch ('s')
			{
			case 's':
				temp.row++;
				if (FunctionCP(temp))
				{
					g_current.row++;
				}
				else
				{
					for (size_t i = 0; i < g_current.mino_shape.width; i++)
					{
						for (size_t j = 0; j < g_current.mino_shape.width; j++)
						{
							if (g_current.mino_shape.shape[i][j])
								g_field[g_current.row + i][g_current.col + j] = g_current.mino_shape.shape[i][j];
						}
					}
					int sum, count = 0;
					for (size_t n = 0; n < ROW; n++)
					{
						sum = 0;
						for (size_t m = 0; m < COL; m++)
						{
							sum += g_field[n][m];
						}
						if (sum == COL)
						{
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
								for (l = 0; l < COL; l++)
									g_field[k][l] = g_field[k - 1][l];
							for (l = 0; l < COL; l++)
								g_field[k][l] = 0;
							g_timer -= g_decrease--;
						}
					}
					t_mino new_shape = generate_random_mino();
					free_mino(g_current);
					g_current = new_shape;
					if (!FunctionCP(g_current))
					{
						g_game_on = false;
					}
				}
				break;
			}
			free_mino(temp);
			print_field();
			gettimeofday(&g_before_now, NULL);
		}
	}
	free_mino(g_current);
	end_game();
	return 0;
}
