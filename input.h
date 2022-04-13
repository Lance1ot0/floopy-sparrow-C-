#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>

using namespace sf;

//Création de la class input
class Input
{
	struct Button { bool space, keyL, keyS, keyH,  leftClick, escape; };

public:
	// Prototype du constructeur
	Input();

	// Prototype
	Button GetButton(void) const;
	void InputHandler(Event event, RenderWindow& window);

private:

	Button button;

};

#endif