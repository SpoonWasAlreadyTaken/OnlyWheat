#pragma once


void AddMoney(int m);


class Tile
{
public:
	int id;
	int type; // 0 = dirt, 1 = tilled dirt, 2 = wheat, 3 = windmill, 4 = well, 5 = scarecrow, 6 = pumpjack;

	float size;

	sf::Vector2f velocity = sf::Vector2f(0, 0);

	sf::Vector2f position;

	sf::Vector2f startingPosition;

	bool movement = false;

	bool isBuilding;
	
	Tile(int i, int t, sf::Vector2f pos, float s, bool b)
	{
		id = i;
		type = t;

		velocity = sf::Vector2f(RandomNumber(-50, 50), RandomNumber(-50, 50));

		position = pos;

		size = s;

		startingPosition = pos;

		isBuilding = b;
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

		if (movement == true) Move(dT);
	}

	void Move(float dT)
	{
		position += velocity * dT;

		if (position.x < 0) velocity.x *= -1;
		if (position.x > 1920 - size) velocity.x *= -1;
		if (position.y < 0) velocity.y *= -1;
		if (position.y > 1080 - size) velocity.y *= -1;
	}

	void Change(int t)
	{
		type = t;

		age = 0;
		growth = 0;

		switch(t)
		{
		case 3:
			isBuilding = true;
			break;
		case 4:
			isBuilding = true;
			break;
		case 5:
			isBuilding = true;
			break;
		case 6:
			isBuilding = true; 
			break;
		default:
			isBuilding = false;
			break;
		}

		Reset();
	}

	void PrintStats()
	{
		std::cout << "///////////\n" << "ID: " << id << "\n" << "Type: " << type << "\n" << "Building: " << isBuilding << "\n" << "Size: " << size << "\n" << "Radius: " << radius << "\n" << "Effect: " << effect << "\n" << "Age: " << age << "\n" << "Growth Speed: " << growthSpeed << "\n" << "Value: " << value << "\n" << "Spoil Chance: " << spoilChance << "\n";
	}

	void Reset()
	{
		radius = baseRadius + radiusBonus * (type != 6);
		if (radius > gridSizeX && radius > gridSizeY) radius = gridSizeX * (gridSizeX >= gridSizeY) + gridSizeY * (gridSizeX < gridSizeY);
		effect = baseEffect + effectBonus;
		if (effect < 0) effect = 0;

		growthSpeed = baseGrowthSpeed * growthSpeedBonus;
		value = baseValue + valueBonus;
	}

	int radius = 1;
	int baseRadius = 1;
	int radiusBonus = 0;

	int effect = 1;
	int baseEffect = 1;
	int effectBonus = 0;

	// wheat
	int age = 0;
	float growth = 0;

	float growthSpeed = 3;
	float baseGrowthSpeed = 3;
	float growthSpeedBonus = 1;

	int value = 1;
	int baseValue = 1;
	int valueBonus = 0;

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
			grid.emplace_back(i, 0, GetTilePosition(i), tileSize, false);
		}
	}

	void UpdateTile(int index)
	{
		DoEffect(index);
	}

	void UpdateEffect(int index)
	{
		UndoEffect(index);
		grid[index].Reset();
		DoEffect(index);
	}


	void RemoveBuilding(int index)
	{
		if (grid[index].isBuilding) UndoEffect(index);
	}

	void EnableMove()
	{
		for (int i = 0; i < gridCount; i++)
		{
			grid[i].movement = !grid[i].movement;
			grid[i].position = grid[i].startingPosition;
			grid[i].velocity = sf::Vector2f(RandomNumber(-150, 150), RandomNumber(-150, 150));
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



	void DoEffect(int index)
	{
		int type = grid[index].type;

		int edge = (grid[index].radius * 2) + 1;
		int size = edge * edge;

		int x = (- edge / 2) - 1;
		int y = -edge / 2;

		int shiftIndex = index + (y * gridX) + x;

		for (int i = 0; i < size; i++)
		{
			//std::cout << "X: " << x << " Y: " << y << "\n";
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
				grid[shiftIndex].valueBonus += grid[index].effect;
				grid[shiftIndex].Reset();
				//std::cout << "Changed value to: " << grid[shiftIndex].value << " for tile: " << shiftIndex << "\n";
				break;
			case 4:
				grid[shiftIndex].growthSpeedBonus *= pow((1 * 0.75), grid[index].effect);
				grid[shiftIndex].Reset();
				//std::cout << "Changed growth speed to: " << grid[shiftIndex].growthSpeed << " for tile: " << shiftIndex << "\n";
				break;
			case 5:
				grid[shiftIndex].spoilChance += grid[index].effect * 10;
				grid[shiftIndex].Reset();
				//std::cout << "Set spoilage to: " << grid[shiftIndex].spoilChance << " for tile: " << shiftIndex << "\n";
				break;
			case 6:
				if (grid[shiftIndex].type != grid[index].type)
				{
					grid[shiftIndex].radiusBonus += grid[index].effect;
					if (grid[shiftIndex].isBuilding) UpdateEffect(shiftIndex);
					grid[shiftIndex].Reset();
					//std::cout << "Set area to: " << grid[shiftIndex].radius << " for tile: " << shiftIndex << "\n";
				}
				break;
			default:
				return;
			}

		}
	}

	void UndoEffect(int index)
	{
		int type = grid[index].type;

		int edge = (grid[index].radius * 2) + 1;
		int size = edge * edge;

		int x = (-edge / 2) - 1;
		int y = -edge / 2;

		int shiftIndex = index + (y * gridX) + x;

		for (int i = 0; i < size; i++)
		{
			//std::cout << "X: " << x << " Y: " << y << "\n";
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
				grid[shiftIndex].valueBonus -= grid[index].effect;
				grid[shiftIndex].Reset();
				//std::cout << "Changed value to: " << grid[shiftIndex].value << " for tile: " << shiftIndex << "\n";
				break;
			case 4:
				grid[shiftIndex].growthSpeedBonus *= pow((1 / 0.75), grid[index].effect);
				grid[shiftIndex].Reset();
				//std::cout << "Changed growth speed to: " << grid[shiftIndex].growthSpeed << " for tile: " << shiftIndex << "\n";
				break;
			case 5:
				grid[shiftIndex].spoilChance = grid[index].effect * -10;
				grid[shiftIndex].Reset();
				//std::cout << "Set spoilage to: " << grid[shiftIndex].spoilChance << " for tile: " << shiftIndex << "\n";
				break;
			case 6:
				if (grid[shiftIndex].type != grid[index].type)
				{
					grid[shiftIndex].radiusBonus -= grid[index].effect;
					if (grid[shiftIndex].isBuilding) UpdateEffect(shiftIndex);
					grid[shiftIndex].Reset();
					//std::cout << "Set area to: " << grid[shiftIndex].radius << " for tile: " << shiftIndex << "\n";
				}
				break;
			default:
				return;
			}

		}
	}
};