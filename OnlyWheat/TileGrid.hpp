#pragma once




class Tile
{
public:
	int id;
	int type; // 0 = dirt, 1 = tilled dirt, 2 = wheat
	int sprite;
	int order = 0;
	
	Tile(int i, int t)
	{
		id = i;
		type = t;
	}

	int GetSprite()
	{
		switch (type)
		{
		case 0:
			return type;
			break;
		case 1:
			return type;
			break;
		case 2:
			return type + age;
			break;
		}
	}

	void Update()
	{
		if (type == 2)
		{
			if (RandomNumber(0, 100) < 1)
			{
				age++;
			}

			if (age > 2)
			{
				AddMoney(1);
				age = 0;
			}
		}
	}

	void Upgrade()
	{
		std::cout << "ahh";
		if (type < 2)
		{
			type++;
		}
	}

private:

	int age = 0;
};




class TileGrid
{
public:
	std::vector<Tile> grid;

	TileGrid(int x, int y, int s, int wX, int wY)
	{
		gridX = x;
		gridY = y;

		tileSize = s * 2;

		gridCount = x * y;

		screenX = wX;
		screenY = wY;

		grid.reserve(gridCount);

		for (int i = 0; i < gridCount; i++)
		{
			grid.emplace_back(i, 0);
		}
	}

	sf::Vector2f GetTilePosition(int index) // gets the position of a grid tile depending on its index
	{
		int y = (int)(index / gridX);
		int x = index - y * gridX;
		int posX = ((tileSize + 1) * (x - gridX / 2)) + screenX / 2;
		int posY = ((tileSize + 1) * (y - gridY / 2)) + screenY / 2;
		return sf::Vector2f(posX, posY);
	}

private:
	int gridCount;
	int gridX;
	int gridY;

	int tileSize;

	int screenX;
	int screenY;
};