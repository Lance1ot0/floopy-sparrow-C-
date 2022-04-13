#include "input.h"


// Constructeur
Input::Input()
{
	button.space = button.keyL = button.keyS = button.keyH = button.leftClick = button.escape = false;
}

//Fonction qui accède à button (structure) et donne l'info private
Input::Button Input::GetButton(void) const
{
    return button;
}

// Fonction des gestion des inputs

void Input::InputHandler(Event event, RenderWindow& window)
{
    // Fermer la fenêtre si on clique sur la croix
    if (event.type == Event::Closed)
    {
        // On ferme la fenêtre
        window.close();
    }

    // Gestion des inputs
    if (event.type == Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case Keyboard::Escape:
            button.escape = true;
            break;
        case Keyboard::Space:
            button.space = true;
            break;
        case Keyboard::L:
            button.keyL = true;
            break;
        case Keyboard::S:
            button.keyS = true;
            break;
        case Keyboard::H:
            button.keyH = true;
            break;
        default:
            break;
        }
    }

    if (event.type == Event::KeyReleased)
    {
        switch (event.key.code)
        {
        case Keyboard::Escape:
            button.escape = false;
            break;
        case Keyboard::Space:
            button.space = false;
            break;
        case Keyboard::L:
            button.keyL = false;
            break;
        case Keyboard::S:
            button.keyS = false;
            break;
        case Keyboard::H:
            button.keyH = false;
            break;
        default:
            break;
        }
    }

    if (event.type == Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == Mouse::Left)
        {
            button.leftClick = true;
        }
    }

    if (event.type == Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == Mouse::Left)
        {
            button.leftClick = false;
        }
    }
}