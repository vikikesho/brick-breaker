#include "game_brick.h"
bool Game::init()
{
	VideoMode videoMode(width, height);
	window.create(videoMode, "Break Out");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FRAMES_PER_SECOND);

	if (!font.loadFromFile("ji-sabres.ttf"))
		return false;

	if (!soundBuffer1.loadFromFile("blip.wav"))
		return false;

	if (!soundBuffer2.loadFromFile("blam.wav"))
		return false;

	if (!soundBuffer3.loadFromFile("blap.wav"))
		return false;

	if (!soundBuffer4.loadFromFile("blop.wav"))
		return false;

	setup();
	return true;
}

int Game::exec()
{
	Clock renderClock, updateClock;
	while (window.isOpen())
	{
		time = renderClock.getElapsedTime();
		float fFps = 1000000 / time.asMicroseconds();
		std::stringstream s;
		s << "fps: " << fFps;
		fps.setString(s.str());
		renderClock.restart();

		const Int64 frameTime = 1000000 / FRAMES_PER_SECOND;
		Clock c;
		Time t = c.getElapsedTime();
		Int64 nextFrameTime = t.asMicroseconds() + frameTime;

		int loops = 0;
		while (t.asMicroseconds() < nextFrameTime && loops < MAX_FRAMESKIP)
		{
			processEvents();
			updateTime = updateClock.restart().asMilliseconds();
			update();
			t = c.getElapsedTime();
			loops++;
		}

		display();
	}

	return EXIT_SUCCESS;
}
void Game::processEvents()
{
	Event event;
	while (window.pollEvent(event))
	{
		if ((event.type == Event::Closed) ||
			((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape)))
			window.close();
		else
			if ((event.type == Event::KeyPressed) && (gameState == INTRO))
				gameState = PLAYING;
	}
}

void Game::update()
{
	if (gameState != PLAYING)
		return;

	updatePlayer();
	checkCollisions();
	updateBall();

	if (playerLives < 0)
		gameState = GAME_LOST;
}

void Game::setup()
{
	top.setPosition(0, 0);
	top.setSize(Vector2f(width, borderSize));

	left.setPosition(0, borderSize);
	left.setSize(Vector2f(borderSize, height + borderSize));

	right.setPosition(width - borderSize, borderSize);
	right.setSize(Vector2f(borderSize, height + borderSize));

	bottom.setPosition(0, height + 2 * borderSize);
	bottom.setSize(Vector2f(width, borderSize));

	top.setFillColor(Color(100, 100, 100));
	top.setOutlineColor(Color::Black);
	top.setOutlineThickness(2);

	left.setFillColor(Color(100, 100, 100));
	left.setOutlineColor(Color::Black);
	left.setOutlineThickness(2);

	right.setFillColor(Color(100, 100, 100));
	right.setOutlineColor(Color::Black);
	right.setOutlineThickness(2);

	bottom.setFillColor(Color(100, 100, 100));
	bottom.setOutlineColor(Color::Black);
	bottom.setOutlineThickness(2);

	ball.setPosition(width / 2, height - margin);
	ball.setSize(Vector2f(20, 20));
	ball.setFillColor(Color::Green);
	ball.setOutlineColor(Color::Red);
	ball.setOutlineThickness(2);

	player.setSize(Vector2f(90, borderSize));
	player.setPosition(width / 2 - 45, height - margin - borderSize);
	player.setFillColor(Color(0, 122, 245));
	player.setOutlineColor(Color::Green);
	player.setOutlineThickness(3);

	title.setString("Break Out!");
	title.setFont(font);
	title.setCharacterSize(50);
	title.setPosition(width / 2 - title.getGlobalBounds().width / 2, 100);
	title.setFillColor(Color::Blue);

	start.setString("Press any key to start");
	start.setFont(font);
	start.setCharacterSize(30);
	start.setPosition(width / 2 - start.getGlobalBounds().width / 2, 400);
	start.setFillColor(Color::Red);

	won.setString("You have won this game.\n\n Congratulations !");
	won.setFont(font);
	won.setCharacterSize(20);
	won.setPosition(width / 2 - won.getGlobalBounds().width / 2, height / 2 - won.getGlobalBounds().height / 2);
	won.setFillColor(Color::Green);

	lost.setString("You have lost this game, \n better luck next time!");
	lost.setFont(font);
	lost.setCharacterSize(20);
	lost.setPosition(width / 2 - lost.getGlobalBounds().width / 2, height / 2 - lost.getGlobalBounds().height / 2);
	lost.setFillColor(Color::Red);

	score.setString("0");
	score.setFont(font);
	score.setCharacterSize(30);
	score.setPosition(width / 2 + score.getGlobalBounds().width / 2, height - 60);
	score.setFillColor(Color(0, 0, 100, 50));

	lives.setString("5");
	lives.setFont(font);
	lives.setCharacterSize(50);
	lives.setPosition(width / 2 - lives.getGlobalBounds().width / 2, height - 60);
	lives.setFillColor(Color(0, 0, 100, 50));

	fps.setString("0");
	fps.setFont(font);
	fps.setCharacterSize(30);
	fps.setPosition(fps.getGlobalBounds().width / 2, height - 40);
	fps.setFillColor(Color(52, 0, 100, 50));

	blip = Sound(soundBuffer1);
	blam = Sound(soundBuffer2);
	blap = Sound(soundBuffer3);
	blop = Sound(soundBuffer4);

	resetGame();
	gameState = INTRO;

	grid.setDimensions(7, 5);
	grid.left = borderSize + 5;
	grid.top = borderSize + 5;
	grid.width = width - 2 * borderSize - 10;
	grid.height = 250;
	grid.init();
}

void Game::display()
{
	window.clear(Color::White);
	switch (gameState)
	{
	case INTRO:
		window.draw(title);
		window.draw(start);
		break;
	case PLAYING:
		window.draw(left);
		window.draw(right);
		grid.display(window);
		window.draw(player);
		window.draw(ball);
		window.draw(score);
		window.draw(lives);
		window.draw(top);
		window.draw(bottom);
		window.draw(fps);
		break;
	case GAME_WON:
		window.draw(won);
		break;
	case GAME_LOST:
		window.draw(lost);
		break;
	}
	window.display();
}

void Game::updatePlayer()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		player.move(-moveDistance * updateTime / 50.0, 0);
	}
	else
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.move(moveDistance * updateTime / 50.0, 0);
		}
}

void Game::updateBall()
{
	ball.move(ballSpeed.x * updateTime, ballSpeed.y * updateTime);
}

void Game::checkCollisions()
{
	if (intersects(player, left) || intersects(player, right))
	{
		FloatRect l = left.getGlobalBounds();
		FloatRect r = right.getGlobalBounds();
		Vector2f p = player.getPosition();
		p.x = clamp(p.x, l.left + l.width + 5, r.left - player.getGlobalBounds().width - 5);
		player.setPosition(p.x, p.y);
		blap.play();
	}
	if (intersects(ball, top))
	{
		FloatRect t = top.getGlobalBounds();
		FloatRect b = ball.getGlobalBounds();
		ballSpeed.y = abs(ballSpeed.y);
		int u = t.top + t.height - b.top;
		ball.move(0, 2 * u);
		blop.play();
	}
	if (intersects(ball, left))
	{
		FloatRect l = left.getGlobalBounds();
		FloatRect b = ball.getGlobalBounds();
		ballSpeed.x = abs(ballSpeed.x);
		int u = l.left + l.width - b.left;
		b.left = l.left + l.width + u;
		ball.setPosition(b.left, b.top);
		blop.play();
	}

	if (intersects(ball, right))
	{
		FloatRect r = right.getGlobalBounds();
		FloatRect b = ball.getGlobalBounds();
		ballSpeed.x = -abs(ballSpeed.x);
		int u = b.left + b.width - r.left;
		b.left = r.left - b.width - u;
		ball.setPosition(b.left, b.top);
		blop.play();
	}

	if (intersects(ball, bottom))
	{
		blam.play();
		playerLives--;
		std::stringstream str;
		str << playerLives;
		lives.setString(str.str());
		lives.setPosition(width / 2 - lives.getGlobalBounds().width / 2, height - 60);
		resetBall();
	}
	if (intersects(ball, player))
	{
		FloatRect p = player.getGlobalBounds();
		FloatRect b = ball.getGlobalBounds();
		Vector2f o = Vector2f(p.left + p.width / 2, p.top + p.height / 2);
		Vector2f om = Vector2f(b.left + b.width / 2 - o.x, b.top + b.height / 2 - o.y);
		om.x /= p.width;
		om.y /= p.height;
		float angle = atan2(om.y, om.x);
		if (angle < -PI / 4 && angle > -3 * PI / 4) 
		{
			ballSpeed.y = -abs(ballSpeed.y);
			ballSpeed.x = (b.left + b.width / 2 - p.left - p.width / 2) / 200;
			int u = b.top + b.height - p.top;
			b.top = p.top - b.height - u;
			ball.setPosition(b.left, b.top);
			ballSpeed.x *= 1.02f;
			ballSpeed.y *= 1.02f;
			blip.play();
		}
	}
	if (grid.collide(ball, ballSpeed, playerScore))
	{
		blap.play();
		playerScore += 200;
		std::stringstream str;
		str << playerScore << " pts  ";
		score.setString(str.str());
		score.setPosition(width / 2 + score.getGlobalBounds().width / 2 - margin, height - 60);
	}
	if (grid.isGameWon())
		gameState = GAME_WON;
	if (playerLives <= 0)
		gameState = GAME_LOST;
}

void Game::resetGame()
{
	playerScore = 0;
	playerLives = 5;
	resetBall();
}
void Game::resetBall()
{
	FloatRect p = player.getGlobalBounds();
	FloatRect b = ball.getGlobalBounds();
	ball.setPosition(p.left + p.width / 2 + 5, p.top);
	ballSpeed.x = 0.1f;
	ballSpeed.y = -0.3f;
}

