#include "Grid.h"
void Grid:: setDimensions(unsigned int col, unsigned int row)
{
	rows = row;
	columns = col;
	if (bricks != NULL)
		delete bricks;

	bricks = new std::vector<Brick>(rows * columns);
	init();
}

Vector2f Grid::cellSize()
{
	return Vector2f(width / columns, height / rows);
}

void Grid::init()
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
		{
			bricks->at(i + j * rows).setSize(cellSize());
			bricks->at(i + j * rows).setFillColor(Color::Blue);
			bricks->at(i + j * rows).setOutlineColor(Color::Black);
			bricks->at(i + j * rows).setOutlineThickness(3);
			bricks->at(i + j * rows).setPosition(left + j * cellSize().x, top + i * cellSize().y);
		}
}

void Grid::display(RenderWindow& window)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
		{
			if (bricks->at(i + j * rows).armor <= 0)
				continue;
			window.draw(bricks->at(i + j * rows));
		}
}

bool Grid::collide(RectangleShape& ball, Vector2f& ballSpeed, unsigned int& score)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
		{
			if (bricks->at(i + j * rows).armor <= 0)
				continue;
			if (bricks->at(i + j * rows).collide(ball, ballSpeed))
			{
				bricks->at(i + j * rows).armor--;
				bricks->at(i + j * rows).setFillColor(Color::Green);
				if (bricks->at(i + j * rows).armor == 0)
					score += 700;

				return true;
			}
		}
	return false;
}

bool Grid::isGameWon()
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			if (bricks->at(i + j * rows).armor > 0)
				return false;
	return true;
}

