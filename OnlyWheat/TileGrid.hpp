#pragma once


void AddMoney(int m);

class Tile
{
public:
	int id;
	int type; // 0 = dirt, 1 = tilled dirt, 2 = wheat, 3 = windmill, 4 = well, 5 = scarecrow, 6 = pumpjack;

	int order = 0;
	
	Tile(int i, int t)
	{
		id = i;
		type = t;

		Change(0);
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
		case 3:
			return 6 + age;
			break;
		case 4:
			return 9;
			break;
		case 5:
			return 10;
			break;
		case 6:
			return 11;
			break;
		}
	}

	void Update(float dT)
	{
		if (type == 2) Wheat(dT);

		if (type == 3) Windmill(dT);
	}

	void Change(int t)
	{
		type = t;

		order = 0;

		age = 0;
		growth = 0;

		switch(t)
		{
		case 3:
			order = 1;
			break;
		case 4:
			order = 1;
			break;
		case 5:
			order = 1;
			break;
		case 6:
			order = 3;
			break;
		}

		Reset();
	}

	void Reset()
	{
		growthSpeed = baseGrowthSpeed;
		value = baseValue;

		radius = baseRadius;
		effect = baseEffect;
	}

	int radius = 1;
	int baseRadius = 1;

	int effect = 1;
	int baseEffect = 1;

	// wheat
	int age = 0;
	float growth = 0;

	float growthSpeed;
	float baseGrowthSpeed = 3;

	int value = 1;
	int baseValue = 1;

	int spoilChance = 5;

private:

	void Wheat(float &dt)
	{
		growth += dt;

		if (growth > growthSpeed)
		{
			growth = 0;
			age++;

			if (age == 3 && RandomNumber(0, 100) > spoilChance)
			{
				age = 0;
				AddMoney(value);
			}

			if (age > 3) age = 0;
		}
	}

	void Windmill(float &dt)
	{
		growth += dt;

		if (growth > .08)
		{
			growth = 0;
			age++;

			if (age > 2) age = 0;
		}
	}
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

	void UpdateTiles()
	{
		for (int i = 0; i < grid.size(); i++)
		{
			grid[i].Reset();
		}

		for (int o = 5; o > 0; o--)
		{
			for (int i = 0; i < grid.size(); i++)
			{
				if (grid[i].order != o) continue;
				switch (grid[i].type)
				{
				case 3:
					RadiusEffect(i, grid[i].type);
					break;
				case 4:
					RadiusEffect(i, grid[i].type);
					break;
				case 5:
					RadiusEffect(i, grid[i].type);
					break;
				case 6:
					RadiusEffect(i, grid[i].type);
					break;
				}
			}
		}
	}

	sf::Vector2f GetTilePosition(int index) // gets the position of a grid tile depending on its index
	{
		float y = index / gridX;
		float x = index - y * gridX;
		float posX = ((float)(tileSize + 0) * (x - (float)gridX / 2)) + (float)screenX / 2;
		float posY = ((float)(tileSize + 0) * (y - (float)gridY / 2)) + (float)screenY / 2;
		return sf::Vector2f(posX, posY);
	}

private:
	int gridCount;
	int gridX;
	int gridY;

	int tileSize;

	int screenX;
	int screenY;



	void RadiusEffect(int index, int type)
	{
		int edge = (grid[index].radius * 2) + 1;
		int size = edge * edge;

		int x = (- edge / 2) - 1;
		int y = -edge / 2;

		int shiftIndex = index + (y * gridX) + x;

		for (int i = 0; i < size; i++)
		{
			std::cout << "X: " << x << " Y: " << y << "\n";
			x++;
			if (x > edge / 2)
			{

				y++;

				x = -edge / 2;
			}

			shiftIndex = index + (y * gridX) + x;

			if (index % gridX == 0 && x < 0) continue;
			if ((index + 1) % gridX == 0 && x > 0) continue;

			if (shiftIndex == index || shiftIndex < 0 || shiftIndex >= gridCount) continue;

			switch (type)
			{
			case 3:
				grid[shiftIndex].value += grid[index].effect;
				std::cout << "Changed value to: " << grid[shiftIndex].value << " for tile: " << shiftIndex << "\n";
				break;
			case 4:
				grid[shiftIndex].growthSpeed *= 1 - (grid[index].effect * 0.25);
				//std::cout << "Changed growth speed to: " << grid[shiftIndex].growthSpeed << " for tile: " << shiftIndex << "\n";
				break;
			case 5:
				grid[shiftIndex].spoilChance = -1;
				//std::cout << "Set spoilage to: " << grid[shiftIndex].spoilChance << " for tile: " << shiftIndex << "\n";
				break;
			case 6:
				grid[shiftIndex].radius += grid[index].effect;
				//std::cout << "Set area to: " << grid[shiftIndex].radius << " for tile: " << shiftIndex << "\n";
				break;
			}

		}
	}
};