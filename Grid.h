#pragma once
#include"Brick.h"
class Grid: public FloatRect
{
private:
	unsigned int rows;
	unsigned int columns;

	std::vector<Brick>* bricks;

public:
	Grid()
	{
		rows = 0;
		columns = 0;
		bricks = NULL;
	}

	~Grid()
	{
		if (bricks != NULL)
			delete bricks;
	}
	Vector2f cellSize();
	void setDimensions(unsigned int col, unsigned int row);
	void init();
	void display(RenderWindow& window);
	bool collide(RectangleShape& ball, Vector2f& ballSpeed, unsigned int& score);
	bool isGameWon();
};


