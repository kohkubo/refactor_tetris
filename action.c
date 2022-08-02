#include "tetris.h"

static t_point down(t_point pos)
{
	return (t_point){pos.row + 1, pos.col};
}

static t_point left(t_point pos)
{
	return (t_point){pos.row, pos.col - 1};
}

static t_point right(t_point pos)
{
	return (t_point){pos.row, pos.col + 1};
}

bool try_move_down(t_game *game, t_mino *mino)
{
	if (can_place_in_field(game->field, &mino->mino_shape, down(mino->pos)))
	{
		mino->pos = down(mino->pos);
		return true;
	}
	return false;
}

bool try_move_left(t_game *game, t_mino *mino)
{
	if (can_place_in_field(game->field, &mino->mino_shape, left(mino->pos)))
	{
		mino->pos = left(mino->pos);
		return true;
	}
	return false;
}

bool try_move_right(t_game *game, t_mino *mino)
{
	if (can_place_in_field(game->field, &mino->mino_shape, right(mino->pos)))
	{
		mino->pos = right(mino->pos);
		return true;
	}
	return false;
}

bool try_move_rotate(t_game *game, t_mino *mino)
{
	t_mino temp = copy_mino(mino);
	rotate_right(&temp.mino_shape);
	if (can_place_in_field(game->field, &temp.mino_shape, temp.pos))
	{
		rotate_right(&mino->mino_shape);
		free_mino(temp);
		return true;
	}
	free_mino(temp);
	return false;
}
