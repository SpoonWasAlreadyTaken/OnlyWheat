#pragma once
#include "globals.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "FaultyUtilities.hpp"



struct Buffs {
    int valueAdd{};
    float valueMulti = 1;
    int spoilAdd{};
    float spoilMulti = 1;
    float growthAdd{};
    float growthMulti = 1;
    float effectAdd{};
    float effectMulti = 1;
    int radiusAdd{};
    float radiusMulti = 1;
};

class Tile
{
public:
    size_t ID{};
    size_t uID{};
    TileType type = empty;
    bool isBuilding = false;
    size_t size{};

    Buffs buff{};

    sf::Texture* texture = &tm.GetMissing();
    sf::Color color = sf::Color(255,255,255);

    sf::Vector2f position{};

    virtual void Update() {};
    virtual void Start() {};
    virtual void OnChange() {};
    virtual void Reset() {};    
    virtual Buffs GetBuff() { return buff; };
};


class Empty : public Tile
{
 public:
    Empty(size_t id, sf::Vector2f pos, size_t s)
    {
        type = empty;
        isBuilding = false;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("Empty");
        position = pos;
        size = s;
    }

};


class Dirt : public Tile
{
 public:
    Dirt(size_t id, sf::Vector2f pos, size_t s)
    {
        type = dirt;
        isBuilding = false;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("Dirt");
        position = pos;
        size = s;

        std::cout << "My " << id << " position: " << pos.x << "|" << pos.y << "\n";
    }

};

class TilledDirt : public Tile
{
public:
    TilledDirt(size_t id, sf::Vector2f pos, size_t s)
    {
        type = tilledDirt;
        isBuilding = false;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("TilledDirt");
        position = pos;
        size = s;
    }
};

class Wheat : public Tile
{
    sf::Texture* textureWheel[4];
	
    int age{};
	float growth{};

	float growthSpeed = 3;
	float baseGrowthSpeed = 3;
	float growthSpeedBonus = 1;

	int value = 1;
	int baseValue = 1;
	int valueBonus{};

	int spoilChance = 5;

public:
    Wheat(size_t id, sf::Vector2f pos, size_t s)
    {
        type = wheat;
        isBuilding = false;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("WheatAge1");
        textureWheel[0] = &tm.GetTexture("WheatAge1");
        textureWheel[1] = &tm.GetTexture("WheatAge2");
        textureWheel[2] = &tm.GetTexture("WheatAge3");
        textureWheel[3] = &tm.GetTexture("WheatDead");
        position = pos;
        size = s;
    }


    void Update() override 
    {
		growth += tT.deltaTime();

		if (growth > growthSpeed)
		{
			growth = 0;
			age++;

            if (age == 3 && RandomNumber(0, 100) > spoilChance)
			{
				age = 0;
				shop.AddMoney(value);
			}
            
            age = age * (age < 4);
            texture = textureWheel[age];

		}
    }
        

};

class Windmill : public Tile
{
    sf::Texture* textureWheel[3];

    int age{};
    float growth{};

    int radius = 1;
    int radiusBase = 1;
    float effect = 1;
    float effectBase = 1;


public:
    Windmill(size_t id, sf::Vector2f pos, size_t s)
    {
        type = windmill;
        isBuilding = true;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("Windmill1");
        textureWheel[0] = &tm.GetTexture("Windmill1");
        textureWheel[1] = &tm.GetTexture("Windmill2");
        textureWheel[2] = &tm.GetTexture("Windmill3");
        position = pos;
        size = s;

        buff.valueAdd = 1;
    }


    void Update() override 
    {
		growth += tT.deltaTime();

		if (growth > 0.08)
		{
            texture = textureWheel[age];
			growth = 0;
			age++;

			age = age * (age < 3);
        }
    }

    Buffs GetBuff() override {
        buff.valueAdd *= effect;
        return buff;
    }
};

class Well : public Tile
{
    int radius = 1;
    int radiusBase = 1;
    float effect = 1;
    float effectBase = 1;

public:
    Well(size_t id, sf::Vector2f pos, size_t s)
    {
        type = well;
        isBuilding = true;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("Well");
        position = pos;
        size = s;

        buff.growthMulti = 0.8;
    } 

    Buffs GetBuff() override {
        buff.growthMulti *= 1.f - (effect / (20 + effect));
        return buff;
    }
};

class Scarecrow : public Tile
{
    int radius = 1;
    int radiusBase = 1;
    float effect = 1;
    float effectBase = 1;
    

public:
    Scarecrow(size_t id, sf::Vector2f pos, size_t s)
    {
        type = scarecrow;
        isBuilding = true;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("Scarecrow");
        position = pos;
        size = s;

        buff.spoilAdd = -10;
    } 

    Buffs GetBuff() override {
        buff.spoilAdd *= effect;
        return buff;
    }
};

class Pumpjack : public Tile
{
    int radius = 1;
    int radiusBase = 1;
    float effect = 1;
    float effectBase = 1;


public:
    Pumpjack(size_t id, sf::Vector2f pos, size_t s)
    {
        type = pumpjack;
        isBuilding = true;
        ID = id;
        uID = GetUID();
        texture = &tm.GetTexture("Pumpjack");
        position = pos;
        size = s;

        buff.radiusAdd = 1;
    } 

    Buffs GetBuff() override {
        buff.radiusAdd += effect;
        return buff;
    }
};




class TileGrid
{
public:
	std::vector<std::unique_ptr<Tile>> grid;

	TileGrid(int _gridX, int _gridY, int _tileSize, int _windowX, int _windowY)
	{
		gridX = _gridX;
		gridY = _gridY;

		tileSize = _tileSize * 2;

		gridCount = _gridX * _gridY;

		screenX = _windowX;
		screenY = _windowY;

		grid.reserve(gridCount);
	}

    void CreateGrid()
    {
        for (int i = 0; i < gridCount; i++)
		{
			grid.push_back(std::make_unique<Dirt>(i, GetTilePosition(i), tileSize));
		}
    }

    void RemoveBuilding(size_t index)
    {
        grid[index] = std::make_unique<Dirt>(index, GetTilePosition(index), tileSize);
    }
    
    void ChangeTile(size_t index, TileType type)
    {
        switch (type)
        {
        case dirt:
        grid[index] = std::make_unique<Dirt>(index, GetTilePosition(index), tileSize);
            break;
        case tilledDirt:
        grid[index] = std::make_unique<TilledDirt>(index, GetTilePosition(index), tileSize);
            break;
        case wheat:
        grid[index] = std::make_unique<Wheat>(index, GetTilePosition(index), tileSize);
            break;
        case windmill:
        grid[index] = std::make_unique<Windmill>(index, GetTilePosition(index), tileSize);
            break;
        case well:
        grid[index] = std::make_unique<Well>(index, GetTilePosition(index), tileSize);
            break;
        case scarecrow:
        grid[index] = std::make_unique<Scarecrow>(index, GetTilePosition(index), tileSize);
            break;
        case pumpjack:
        grid[index] = std::make_unique<Pumpjack>(index, GetTilePosition(index), tileSize);
            break;
        case empty:
        grid[index] = std::make_unique<Empty>(index, GetTilePosition(index), tileSize);
            break;
        }
    }


	sf::Vector2f GetTilePosition(int index) // gets the position of a grid tile depending on its index
	{
		float y = (int)(index / gridX);
		float x = (int)(index - y * gridX);
		float posX = ((float)(tileSize + 0) * (x - (float)gridX / 2)) + (float)screenX / 2;
		float posY = ((float)(tileSize + 0) * (y - (float)gridY / 2)) + (float)screenY / 2;
		return sf::Vector2f(posX, posY);
	}

private:
	size_t gridCount;
	size_t gridX;
	size_t gridY;

	int tileSize;

	size_t screenX;
	size_t screenY;

    std::vector<size_t> GetArea(size_t index, size_t radius)
    {
        std::vector<size_t> area;
        size_t edge = (radius * 2) + 1;
        size_t size = edge * edge;
        area.reserve(size);

        for (size_t i = 0; i < size; i++) {
            int y = -radius + (i / edge);
            int x = -radius + i - ((i / edge) * edge);

            if (index % gridX == 0 && x < 0) continue;
            if ((index + 1) % gridX == 0 && x > 0) continue;

            size_t shiftIndex = index + x + (y * gridX);
            if (index != shiftIndex && shiftIndex < gridCount) area.emplace_back(shiftIndex);
        }

        return area;
    }



































/*

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
*/
};
