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
	int row;
	int col;
} t_mino;
t_mino g_current;

// clang-format off
const t_mino g_minos[] = {
	{
		.shape = (char *[]){
			(char[]){0, 1, 1},
			(char[]){1, 1, 0},
			(char[]){0, 0, 0}
		},
		.width = 3, .row = 0, .col = 0
	},
	{
		.shape = (char *[]){
			(char[]){1, 1, 0},
			(char[]){0, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3, .row = 0, .col = 0},
	{
		.shape = (char *[]){
			(char[]){0, 1, 0},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3, .row = 0, .col = 0
	},
	{
		.shape = (char *[]){
			(char[]){0, 0, 1},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3, .row = 0, .col = 0
	},
	{
		.shape = (char *[]){
			(char[]){1, 0, 0},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
	 	.width = 3, .row = 0, .col = 0
	},
	{
		.shape = (char *[]){
			(char[]){1, 1},
			(char[]){1, 1}
		},
		.width = 2, .row = 0, .col = 0
	},
	{
		.shape = (char *[]){
			(char[]){0, 0, 0, 0},
			(char[]){1, 1, 1, 1},
			(char[]){0, 0, 0, 0},
			(char[]){0, 0, 0, 0}
		},
		.width = 4, .row = 0, .col = 0}
	};
// clang-format on

t_mino copy_mino(t_mino mino)
{
	t_mino new_shape = mino;
	new_shape.shape = (char **)malloc(new_shape.width * sizeof(char *));
	for (size_t i = 0; i < new_shape.width; i++)
	{
		new_shape.shape[i] = (char *)malloc(new_shape.width * sizeof(char));
		for (size_t j = 0; j < new_shape.width; j++)
		{
			new_shape.shape[i][j] = mino.shape[i][j];
		}
	}
	return new_shape;
}

void free_mino(t_mino mino)
{
	for (size_t i = 0; i < mino.width; i++)
	{
		free(mino.shape[i]);
	}
	free(mino.shape);
}

bool FunctionCP(t_mino mino)
{
	char **shape = mino.shape;
	for (size_t i = 0; i < mino.width; i++) {
		for (size_t j = 0; j < mino.width; j++){
			if ((mino.col + j < 0 || mino.col + j >= COL || mino.row + i >= ROW)) {
				if (shape[i][j]) {
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
	for (size_t i = 0; i < mino.width; i++)
	{
		for (size_t j = 0, k = mino.width - 1; j < mino.width; j++, k--)
		{
			mino.shape[i][j] = temp.shape[k][i];
		}
	}
	free_mino(temp);
}

void print_field()
{
	char Buffer[ROW][COL] = {0};
	for (size_t i = 0; i < g_current.width; i++)
	{
		for (size_t j = 0; j < g_current.width; j++)
		{
			if (g_current.shape[i][j])
				Buffer[g_current.row + i][g_current.col + j] = g_current.shape[i][j];
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
	t_mino new_mino = copy_mino(g_minos[rand() % 7]);
	new_mino.col = rand() % (COL - new_mino.width + 1);
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
	g_current = copy_mino(g_minos[rand() % 7]);
	g_current.col = rand() % (COL - g_current.width + 1);
	g_current.row = 0;
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
					for (size_t i = 0; i < g_current.width; i++)
					{
						for (size_t j = 0; j < g_current.width; j++)
						{
							if (g_current.shape[i][j])
							{
								g_field[g_current.row + i][g_current.col + j] = g_current.shape[i][j];
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
					t_mino new_shape = copy_mino(g_minos[rand() % 7]);
					new_shape.col = rand() % (COL - new_shape.width + 1);
					new_shape.row = 0;
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
					for (size_t i = 0; i < g_current.width; i++)
					{
						for (size_t j = 0; j < g_current.width; j++)
						{
							if (g_current.shape[i][j])
								g_field[g_current.row + i][g_current.col + j] = g_current.shape[i][j];
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
					t_mino new_shape = copy_mino(g_minos[rand() % 7]);
					new_shape.col = rand() % (COL - new_shape.width + 1);
					new_shape.row = 0;
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
