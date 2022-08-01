#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define R 20
#define C 15
#define T 1
#define F 0

char g_table[R][C] = {0};
int g_final = 0;
char g_game_on = T;
suseconds_t g_timer = 400000;
int g_decrease = 1000;

typedef struct
{
	char **array;
	int width;
	int row;
	int col;
} t_mino;
t_mino g_current;

const t_mino g_minos[] = {
	{(char *[]){(char[]){0, 1, 1},
				(char[]){1, 1, 0},
				(char[]){0, 0, 0}},
	 3, 0, 0},
	{(char *[]){(char[]){1, 1, 0},
				(char[]){0, 1, 1},
				(char[]){0, 0, 0}},
	 3, 0, 0},
	{(char *[]){(char[]){0, 1, 0},
				(char[]){1, 1, 1},
				(char[]){0, 0, 0}},
	 3, 0, 0},
	{(char *[]){(char[]){0, 0, 1},
				(char[]){1, 1, 1},
				(char[]){0, 0, 0}},
	 3, 0, 0},
	{(char *[]){(char[]){1, 0, 0},
				(char[]){1, 1, 1},
				(char[]){0, 0, 0}},
	 3, 0, 0},
	{(char *[]){(char[]){1, 1},
				(char[]){1, 1}},
	 2, 0, 0},
	{(char *[]){(char[]){0, 0, 0, 0},
				(char[]){1, 1, 1, 1},
				(char[]){0, 0, 0, 0},
				(char[]){0, 0, 0, 0}},
	 4, 0, 0}};

t_mino FunctionCS(t_mino shape)
{
	t_mino new_shape = shape;
	new_shape.array = (char **)malloc(new_shape.width * sizeof(char *));
	for (int i = 0; i < new_shape.width; i++)
	{
		new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
		for (int j = 0; j < new_shape.width; j++)
		{
			new_shape.array[i][j] = shape.array[i][j];
		}
	}
	return new_shape;
}

void FunctionDS(t_mino shape)
{
	int i;
	for (i = 0; i < shape.width; i++)
	{
		free(shape.array[i]);
	}
	free(shape.array);
}

int FunctionCP(t_mino shape)
{
	char **array = shape.array;
	int i, j;
	for (i = 0; i < shape.width; i++)
	{
		for (j = 0; j < shape.width; j++)
		{
			if ((shape.col + j < 0 || shape.col + j >= C || shape.row + i >= R))
			{
				if (array[i][j])
					return F;
			}
			else if (g_table[shape.row + i][shape.col + j] && array[i][j])
				return F;
		}
	}
	return T;
}

void FunctionRS(t_mino shape)
{
	t_mino temp = FunctionCS(shape);
	int i, j, k, width;
	width = shape.width;
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			shape.array[i][j] = temp.array[k][i];
		}
	}
	FunctionDS(temp);
}

void FunctionPT()
{
	char Buffer[R][C] = {0};
	int i, j;
	for (i = 0; i < g_current.width; i++)
	{
		for (j = 0; j < g_current.width; j++)
		{
			if (g_current.array[i][j])
				Buffer[g_current.row + i][g_current.col + j] = g_current.array[i][j];
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
int hasToUpdate()
{
	return ((suseconds_t)(g_now.tv_sec * 1000000 + g_now.tv_usec) - ((suseconds_t)g_before_now.tv_sec * 1000000 + g_before_now.tv_usec)) > g_timer;
}

void set_timeout(int time)
{
	time = 1;
	timeout(1);
}

int main()
{
	srand(time(0));
	g_final = 0;
	int c;
	initscr();
	gettimeofday(&g_before_now, NULL);
	set_timeout(1);
	t_mino new_shape = FunctionCS(g_minos[rand() % 7]);
	new_shape.col = rand() % (C - new_shape.width + 1);
	new_shape.row = 0;
	FunctionDS(g_current);
	g_current = new_shape;
	if (!FunctionCP(g_current))
	{
		g_game_on = F;
	}
	FunctionPT();
	while (g_game_on)
	{
		if ((c = getch()) != ERR)
		{
			t_mino temp = FunctionCS(g_current);
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
							if (g_current.array[i][j])
								g_table[g_current.row + i][g_current.col + j] = g_current.array[i][j];
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
					t_mino new_shape = FunctionCS(g_minos[rand() % 7]);
					new_shape.col = rand() % (C - new_shape.width + 1);
					new_shape.row = 0;
					FunctionDS(g_current);
					g_current = new_shape;
					if (!FunctionCP(g_current))
					{
						g_game_on = F;
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
			FunctionDS(temp);
			FunctionPT();
		}
		gettimeofday(&g_now, NULL);
		if (hasToUpdate())
		{
			t_mino temp = FunctionCS(g_current);
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
							if (g_current.array[i][j])
								g_table[g_current.row + i][g_current.col + j] = g_current.array[i][j];
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
					t_mino new_shape = FunctionCS(g_minos[rand() % 7]);
					new_shape.col = rand() % (C - new_shape.width + 1);
					new_shape.row = 0;
					FunctionDS(g_current);
					g_current = new_shape;
					if (!FunctionCP(g_current))
					{
						g_game_on = F;
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
			FunctionDS(temp);
			FunctionPT();
			gettimeofday(&g_before_now, NULL);
		}
	}
	FunctionDS(g_current);
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
