//
// Created by quan on 26/10/2019.
//

#include "update_context.h"

update_context::update_context(sf::RenderWindow& window, ball_game& game) : window(window), game(game) {
	time = sf::Clock();
	elapsed_time = 0.f;
}

void update_context::update() {
	elapsed_time = time.restart().asSeconds();
}