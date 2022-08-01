#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define R 20
#define C 15
#define true 1
#define false 0

char g_table[R][C] = {0};
int g_final = 0;
bool g_game_on = true;
suseconds_t g_timer = 400000;
int g_decrease = 1000;

typedef struct
{
	char **shape;
	int width;
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
	for (int i = 0; i < mino.width; i++)
	{
		for (int j = 0; j < mino.width; j++)
		{
			if ((mino.col + j < 0 || mino.col + j >= C || mino.row + i >= R))
			{
				if (shape[i][j])
					return false;
			}
			else if (g_table[mino.row + i][mino.col + j] && shape[i][j])
				return false;
		}
	}
	return true;
}

void FunctionRS(t_mino mino)
{
	t_mino temp = copy_mino(mino);
	int i, j, k, width;
	width = mino.width;
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			mino.shape[i][j] = temp.shape[k][i];
		}
	}
	free_mino(temp);
}

void FunctionPT()
{
	char Buffer[R][C] = {0};
	int i, j;
	for (i = 0; i < g_current.width; i++)
	{
		for (j = 0; j < g_current.width; j++)
		{
			if (g_current.shape[i][j])
				Buffer[g_current.row + i][g_current.col + j] = g_current.shape[i][j];
		}
	}
	clear();
	for (i = 0; i < C - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (i = 0; i < R; i++)
	{
		for (j = 0; j < C; j++)
		{
			printw("%c ", (g_table[i][j] + Buffer[i][j]) ? '#' : '.');
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

int main()
{
	srand(time(0));
	g_final = 0;
	int c;
	initscr();
	gettimeofday(&g_before_now, NULL);
	timeout(1);
	t_mino new_shape = copy_mino(g_minos[rand() % 7]);
	new_shape.col = rand() % (C - new_shape.width + 1);
	new_shape.row = 0;
	free_mino(g_current);
	g_current = new_shape;
	if (!FunctionCP(g_current))
	{
		g_game_on = false;
	}
	FunctionPT();
	while (g_game_on)
	{
		if ((c = getch()) != ERR)
		{
			t_mino temp = copy_mino(g_current);
			switch (c)
			{
			case 's':
				temp.row++; // move down
				if (FunctionCP(temp))
					g_current.row++;
				else
				{
					int i, j;
					for (i = 0; i < g_current.width; i++)
					{
						for (j = 0; j < g_current.width; j++)
						{
							if (g_current.shape[i][j])
								g_table[g_current.row + i][g_current.col + j] = g_current.shape[i][j];
						}
					}
					int n, m, sum, count = 0;
					for (n = 0; n < R; n++)
					{
						sum = 0;
						for (m = 0; m < C; m++)
						{
							sum += g_table[n][m];
						}
						if (sum == C)
						{
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
								for (l = 0; l < C; l++)
									g_table[k][l] = g_table[k - 1][l];
							for (l = 0; l < C; l++)
								g_table[k][l] = 0;
							g_timer -= g_decrease--;
						}
					}
					g_final += 100 * count;
					t_mino new_shape = copy_mino(g_minos[rand() % 7]);
					new_shape.col = rand() % (C - new_shape.width + 1);
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
				FunctionRS(temp);
				if (FunctionCP(temp))
					FunctionRS(g_current);
				break;
			}
			free_mino(temp);
			FunctionPT();
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
					g_current.row++;
				else
				{
					int i, j;
					for (i = 0; i < g_current.width; i++)
					{
						for (j = 0; j < g_current.width; j++)
						{
							if (g_current.shape[i][j])
								g_table[g_current.row + i][g_current.col + j] = g_current.shape[i][j];
						}
					}
					int n, m, sum, count = 0;
					for (n = 0; n < R; n++)
					{
						sum = 0;
						for (m = 0; m < C; m++)
						{
							sum += g_table[n][m];
						}
						if (sum == C)
						{
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
								for (l = 0; l < C; l++)
									g_table[k][l] = g_table[k - 1][l];
							for (l = 0; l < C; l++)
								g_table[k][l] = 0;
							g_timer -= g_decrease--;
						}
					}
					t_mino new_shape = copy_mino(g_minos[rand() % 7]);
					new_shape.col = rand() % (C - new_shape.width + 1);
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
				FunctionRS(temp);
				if (FunctionCP(temp))
					FunctionRS(g_current);
				break;
			}
			free_mino(temp);
			FunctionPT();
			gettimeofday(&g_before_now, NULL);
		}
	}
	free_mino(g_current);
	endwin();
	int i, j;
	for (i = 0; i < R; i++)
	{
		for (j = 0; j < C; j++)
		{
			printf("%c ", g_table[i][j] ? '#' : '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", g_final);
	return 0;
}
