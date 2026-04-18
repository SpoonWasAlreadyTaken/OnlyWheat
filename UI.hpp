#pragma once
#include "globals.hpp"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>


class Button
{
public:
	size_t ID;
    size_t uID;

	float sizeX;
	float sizeY;

	sf::Vector2f position;

	sf::Texture* texture = &tm.GetMissing();

	Button(sf::Vector2f pos, float _sizeX, float _sizeY, int id, std::string name)
	{
		position = pos;
		sizeX = _sizeX;
		sizeY = _sizeY;

		ID = id;
        uID = GetUID();
		texture = &tm.GetTexture(name);
	}


private:
};

class UI
{
public:
	std::vector<Button> buttons;

	int idNext = 0;

	int selectedButton = 0;

	UI()
	{
		buttons.reserve(100);
	}

	void AddButton(sf::Vector2f pos, float _sizeX, float _sizeY, std::string name)
	{
		buttons.emplace_back(pos, _sizeX, _sizeY, idNext, name);
		idNext++;
	}

private:
};
