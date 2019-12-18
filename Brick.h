#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#define PI   3.14159265358979323846
using namespace sf;
bool intersects(const RectangleShape& rect1, const RectangleShape& rect2);
float clamp(const float x, const float a, const float b);
class Brick: public RectangleShape {
public:
	unsigned int armor;

	Brick() :RectangleShape()
	{
		armor = 4;
	}

	Brick(const Vector2f& size) :RectangleShape(size)
	{
		armor = 4;
	}
	bool collide(RectangleShape& ball, Vector2f& ballSpeed);
};