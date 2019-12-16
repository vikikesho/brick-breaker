#pragma once
#include"Brick.h"
#include"Grid.h"
class Game
{
private:
	static const int FRAMES_PER_SECOND = 60;
	static const int MAX_FRAMESKIP = 10;
	static const int width = 640;
	static const int height = 480;
	static const int borderSize = 12;
	static const int margin = 50;
	static const int moveDistance = 40;


	RenderWindow window;
	Font font;

	RectangleShape top;
	RectangleShape left;
	RectangleShape right;
	RectangleShape bottom;

	RectangleShape ball;

	Vector2f ballSpeed;

	RectangleShape player;

	Text title;
	Text start;
	Text won;
	Text lost;
	Text score;
	Text lives;
	Text fps;

	SoundBuffer soundBuffer1;
	Sound blip;
	SoundBuffer soundBuffer2;
	Sound blam;
	SoundBuffer soundBuffer3;
	Sound blap;
	SoundBuffer soundBuffer4;
	Sound blop;

	Grid grid;

	Time time;
	Int32 updateTime;

	unsigned int playerScore;
	unsigned int playerLives;

	enum states { INTRO, PLAYING, GAME_WON, GAME_LOST };

	int gameState;
public:
	Game()
	{
	}
	bool init();
	int exec();
private:

	void processEvents();
	void update();
	void setup();
	void display();
	void updatePlayer();
	void updateBall();
	void checkCollisions();
	void resetGame();
	void resetBall();
};

