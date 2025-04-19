#pragma once

class Button
{
public:
	int id;

	float sizeX;
	float sizeY;

	sf::Vector2f position;

	sf::Texture texture;

	Button(sf::Vector2f pos, float x, float y, int i, sf::Texture t)
	{
		position = pos;
		sizeX = x;
		sizeY = y;

		id = i;
		texture = t;
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

	void AddButton(sf::Vector2f pos, float x, float y, sf::Texture t)
	{
		buttons.emplace_back(pos, x, y, idNext, t);
		idNext++;
	}

private:
};