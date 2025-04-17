#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>

#include "FaultyUtilities.hpp"




// settings:

int gridSizeX = 10;
int gridSizeY = 10;

float tileSize = 32;

int windowX = 1920;
int windowY = 1080;



// variable definitions

int tileCount;

int money = 50;

//size_t vertexBuffer = 100000;
//sf::VertexArray quad(sf::PrimitiveType::Triangles, vertexBuffer);
std::vector<sf::Texture> textures;


// function definitions
//void Draw(sf::RenderWindow& window);

void AddMoney(int m);

#include "TileGrid.hpp";


TileGrid tileGrid = TileGrid(gridSizeX, gridSizeY, tileSize, windowX, windowY);


int main()
{
    tileCount = gridSizeX * gridSizeY;

    sf::RenderWindow window(sf::VideoMode({ (uint32_t)(windowX), (uint32_t)(windowY) }), "Only Wheat");

    /*
    for (int i = 0; i < vertexBuffer / 6; i++)
    {
        // texture cordinte assignment
        int index = i * 6;
        quad[index].texCoords = sf::Vector2f(0, 0);
        quad[index + 1].texCoords = sf::Vector2f(0, 32);
        quad[index + 2].texCoords = sf::Vector2f(32, 0);

        quad[index + 3].texCoords = sf::Vector2f(0, 0);
        quad[index + 4].texCoords = sf::Vector2f(32, 32);
        quad[index + 5].texCoords = sf::Vector2f(32, 0);
    }
    */

    // fps code
    sf::Clock clock;
    float deltaTime = 0;
    int fps = 0;
    int steps = 0;
    int avgFPS = 0;


    // text code
    sf::Font font;
    font.openFromFile("./Assets\\VIRUST.ttf"); // set to the font path


    sf::Text fpsText(font, "60");
    sf::Text moneyText(font, "$ " + std::to_string(money));
    moneyText.setFillColor(sf::Color(0, 255, 0));
    fpsText.setPosition(sf::Vector2f(5, 5));
    moneyText.setPosition(sf::Vector2f(5, 55));

    for (int i = 0; i < 5; i++)
    {
        textures.emplace_back();
    }

    textures[0].loadFromFile("./Assets\\Dirt.png");
    textures[1].loadFromFile("./Assets\\TilledDirt.png");
    textures[2].loadFromFile("./Assets\\WheatAge1.png");
    textures[3].loadFromFile("./Assets\\WheatAge2.png");
    textures[4].loadFromFile("./Assets\\WheatAge3.png");


    for (int i = 0; i < textures.size(); i++)
    {
        textures[i].generateMipmap();
        //textures[i].setSmooth(true);
    }


    sf::Sprite sprite(textures[0]);

    float actionLockout = 0;

    // main loop
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        actionLockout -= deltaTime;
        fps += 1.f / (deltaTime);

        if (steps == 10)
        {
            avgFPS = fps / steps;
            fpsText.setString("FPS: " + std::to_string(avgFPS));
            moneyText.setString("$ " + std::to_string(money));
            fps = 0;
            steps = 0;
        }
        steps++;


        for (int i = 0; i < tileCount; i++)
        {
            tileGrid.grid[i].Update();
        }


        while (const std::optional event = window.pollEvent())
        {
            if (actionLockout <= 0)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    float correct = windowX / window.getSize().x;
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                    std::cout << mousePos.x << "  " << mousePos.y << "\n";

                    for (int i = 0; i < tileCount; i++)
                    {
                        if (tileGrid.GetTilePosition(i).x + tileSize * 2 > mousePos.x && tileGrid.GetTilePosition(i).x < mousePos.x && tileGrid.GetTilePosition(i).y + tileSize * 2 > mousePos.y && tileGrid.GetTilePosition(i).y < mousePos.y)
                        {
                            tileGrid.grid[i].Upgrade();
                            break;
                        }
                    }

                }

                actionLockout = 0.03;
            } 
        }

        

        window.clear();

        for (int i = 0; i < tileCount; i++)
        {
            sprite.setTexture(textures[tileGrid.grid[i].GetSprite()]);

            sprite.setPosition(tileGrid.GetTilePosition(i));
            sprite.setScale(sf::Vector2f(1 * (tileSize / 16), 1 * (tileSize / 16)));

            window.draw(sprite);
        }

        window.draw(fpsText);
        window.draw(moneyText);
        

        window.display();

        window.setFramerateLimit(60);

        //std::cout << "help \n";
    }
}

/*
void Draw(sf::RenderWindow& window)
{
    for (int i = 0; i < tileCount; i++)
    {
        int const index = i * 6;

        // color assignment
        sf::Color color = sf::Color(255, 255, 255);

        quad[index].color = color;
        quad[index + 1].color = color;
        quad[index + 2].color = color;

        quad[index + 3].color = color;
        quad[index + 4].color = color;
        quad[index + 5].color = color;

        // position assignment
        quad[index].position = sf::Vector2f(tileGrid.GetTilePosition(i).x - tileSize, tileGrid.GetTilePosition(i).y - tileSize);
        quad[index + 1].position = sf::Vector2f(tileGrid.GetTilePosition(i).x + tileSize, tileGrid.GetTilePosition(i).y - tileSize);
        quad[index + 2].position = sf::Vector2f(tileGrid.GetTilePosition(i).x - tileSize, tileGrid.GetTilePosition(i).y + tileSize);

        quad[index + 3].position = sf::Vector2f(tileGrid.GetTilePosition(i).x + tileSize, tileGrid.GetTilePosition(i).y - tileSize);
        quad[index + 4].position = sf::Vector2f(tileGrid.GetTilePosition(i).x - tileSize, tileGrid.GetTilePosition(i).y + tileSize);
        quad[index + 5].position = sf::Vector2f(tileGrid.GetTilePosition(i).x + tileSize, tileGrid.GetTilePosition(i).y + tileSize);

    }
        
    window.draw(quad, &sprite);
}
*/

void AddMoney(int m)
{
    money += m;
}