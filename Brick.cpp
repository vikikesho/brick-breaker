#include"Brick.h"
bool intersects(const RectangleShape& rect1, const RectangleShape& rect2)
{
	FloatRect r1 = rect1.getGlobalBounds();
	FloatRect r2 = rect2.getGlobalBounds();
	return r1.intersects(r2);
}

float clamp(const float x, const float a, const float b)
{
	return std::min(std::max(a, x), b);
}
bool Brick::collide(RectangleShape& ball, Vector2f& ballSpeed)
{
	if (intersects(ball, *this))
	{
		FloatRect r = getGlobalBounds();
		FloatRect b = ball.getGlobalBounds();
		Vector2f o = Vector2f(r.left + r.width / 2, r.top + r.height / 2);
		Vector2f om = Vector2f(b.left + b.width / 2 - o.x, b.top + b.height / 2 - o.y);
		om.x /= r.width;
		om.y /= r.height;
		float angle = atan2(om.y, om.x);
		if (angle >= PI / 4 && angle <= 3 * PI / 4)
		{
			ballSpeed.y = abs(ballSpeed.y);
			float u = r.top + r.height - b.top;
			ball.move(0, 2 * u);
			return true;
		}
		if (angle <= -PI / 4 && angle >= -3 * PI / 4) 
		{
			ballSpeed.y = -abs(ballSpeed.y);
			float u = b.top + b.height - r.top;
			ball.move(0, -2 * u);
			return true;
		}
		if (angle <= -3 * PI / 4) 
		{
			ballSpeed.y = -abs(ballSpeed.x);
			float u = b.left + b.width - r.left;
			ball.move(-2 * u, 0);
			return true;
		}
		if (abs(angle) <= PI / 4) 
		{
			ballSpeed.y = abs(ballSpeed.x);
			float u = r.left + r.width - b.left;
			ball.move(2 * u, 0);
			return true;
		}
	}
	return false;
}