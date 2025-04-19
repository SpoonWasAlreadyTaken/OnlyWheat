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

int gridSizeX = 12;
int gridSizeY = 12;

float tileSize = 32;

int windowX = 1920;
int windowY = 1080;

// classes

#include "TileGrid.hpp";
#include "UI.hpp"

// variable definitions

int tileCount;

int money = 50000;
int moneyGen = 0;

int selectedTool = 0;

bool lsdMode = false;

//size_t vertexBuffer = 100000;
//sf::VertexArray quad(sf::PrimitiveType::Triangles, vertexBuffer);
std::vector<sf::Texture> textures;
std::vector<sf::Texture> uiTextures;
sf::Texture emptyBoxTexture;


// function definitions
//void Draw(sf::RenderWindow& window);

void AddMoney(int m);




TileGrid tileGrid = TileGrid(gridSizeX, gridSizeY, tileSize, windowX, windowY);
UI ui = UI();


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
    float elapsedTime = 0;
    int fps = 0;
    int steps = 0;
    int avgFPS = 0;


    // text code
    sf::Font font;
    font.openFromFile("./Assets\\VIRUST.ttf"); // set to the font path

    int moneyM = 0;

    float minute = 0;

    sf::Text fpsText(font, "60");
    sf::Text moneyText(font, "$ " + std::to_string(money));
    sf::Text lsdText(font, "Press ''L'' for LSD Mode");
    sf::Text numberText(font, "0");
    sf::Text incomeText(font, "Income: ");
    moneyText.setFillColor(sf::Color(0, 255, 0));
    fpsText.setPosition(sf::Vector2f(5, 5));
    moneyText.setPosition(sf::Vector2f(5, 55));
    incomeText.setPosition(sf::Vector2f(5, 105));
    lsdText.setPosition(sf::Vector2f(5, 1000));
    lsdText.setScale(sf::Vector2f(0.5, 0.5));
    numberText.setScale(sf::Vector2f(0.5, 0.5));

    for (int i = 0; i < 12; i++)
    {
        textures.emplace_back();
    }

    textures[0].loadFromFile("./Assets\\Dirt.png");
    textures[1].loadFromFile("./Assets\\TilledDirt.png");
    textures[2].loadFromFile("./Assets\\WheatAge1.png");
    textures[3].loadFromFile("./Assets\\WheatAge2.png");
    textures[4].loadFromFile("./Assets\\WheatAge3.png");
    textures[5].loadFromFile("./Assets\\WheatDead.png");
    textures[6].loadFromFile("./Assets\\Windmill.png");
    textures[7].loadFromFile("./Assets\\Windmill1.png");
    textures[8].loadFromFile("./Assets\\Windmill2.png");
    textures[9].loadFromFile("./Assets\\Well.png");
    textures[10].loadFromFile("./Assets\\Scarecrow.png");
    textures[11].loadFromFile("./Assets\\Pumpjack.png");

    for (int i = 0; i < 9; i++)
    {
        uiTextures.emplace_back();
    }

    emptyBoxTexture.loadFromFile("./Assets\\Empty.png");
    uiTextures[0].loadFromFile("./Assets\\Pick.png");
    uiTextures[1].loadFromFile("./Assets\\Hoe.png");
    uiTextures[2].loadFromFile("./Assets\\WheatUI.png");
    uiTextures[3].loadFromFile("./Assets\\WindmillUI.png");
    uiTextures[4].loadFromFile("./Assets\\WellUI.png");
    uiTextures[5].loadFromFile("./Assets\\ScarecrowUI.png");
    uiTextures[6].loadFromFile("./Assets\\PumpjackUI.png"); 
    uiTextures[7].loadFromFile("./Assets\\Empty.png");
    uiTextures[8].loadFromFile("./Assets\\Empty.png");



    for (int i = 0; i < 9; i++)
    {
        ui.AddButton(sf::Vector2f((windowX / 2) + (i * 64) - 64 * 9 / 2, 950), 32, 32, uiTextures[i]);
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
            tileGrid.grid[i].Update(deltaTime);
        }
        


        while (const std::optional event = window.pollEvent())
        {
            if (actionLockout <= 0)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    float correct = windowX / window.getSize().x;
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                    //std::cout << mousePos.x << "  " << mousePos.y << "\n";

                    for (int i = 0; i < tileCount; i++)
                    {
                        if (tileGrid.grid[i].position.x + tileSize * 2 > mousePos.x && tileGrid.grid[i].position.x < mousePos.x && tileGrid.grid[i].position.y + tileSize * 2 > mousePos.y && tileGrid.grid[i].position.y < mousePos.y)
                        {
                            switch (selectedTool)
                            {
                            case 0:
                                switch (tileGrid.grid[i].type)
                                {
                                case 0:
                                    break;
                                case 1:
                                    break;
                                case 2:
                                    money += 5;
                                    break;
                                case 3:
                                    money += 30;
                                    break;
                                case 4:
                                    money += 20;
                                    break;
                                case 5:
                                    money += 10;
                                    break;
                                case 6:
                                    money += 100;
                                    break;
                                }
                                tileGrid.RemoveBuilding(i);
                                tileGrid.grid[i].Change(0);
                                break;
                            case 1:
                                if (tileGrid.grid[i].type == 0)
                                {
                                    tileGrid.grid[i].Change(1);
                                    tileGrid.UpdateTile(i);
                                }
                                break;
                            case 2:
                                if (tileGrid.grid[i].type == 1 && money >= 10)
                                {
                                    tileGrid.grid[i].Change(2);
                                    money -= 10;
                                    tileGrid.UpdateTile(i);
                                }
                                break;
                            case 3:
                                if (tileGrid.grid[i].type == 0 && money >= 50)
                                {
                                    tileGrid.grid[i].Change(3);
                                    money -= 50;
                                    tileGrid.UpdateTile(i);
                                }
                                break;
                            case 4:
                                if (tileGrid.grid[i].type == 0 && money >= 35)
                                {
                                    tileGrid.grid[i].Change(4);
                                    money -= 35;
                                    tileGrid.UpdateTile(i);
                                }
                                break;
                            case 5:
                                if (tileGrid.grid[i].type == 0 && money >= 15)
                                {
                                    tileGrid.grid[i].Change(5);
                                    money -= 15;
                                    tileGrid.UpdateTile(i);
                                }
                                break;
                            case 6:
                                if (tileGrid.grid[i].type == 0 && money >= 150)
                                {
                                    tileGrid.grid[i].Change(6);
                                    money -= 150;
                                    tileGrid.UpdateTile(i);
                                }
                                break;
                            }
                            tileGrid.grid[i].PrintStats();
                            break;
                        }
                    }

                    for (int i = 0; i < ui.buttons.size(); i++)
                    {
                        if (ui.buttons[i].position.x + ui.buttons[i].sizeX * 2 > mousePos.x && ui.buttons[i].position.x < mousePos.x && ui.buttons[i].position.y + ui.buttons[i].sizeY * 2 > mousePos.y && ui.buttons[i].position.y < mousePos.y)
                        {
                            switch (i)
                            {
                            case 0:
                                selectedTool = 0;
                                ui.selectedButton = 0;
                                break;
                            case 1:
                                selectedTool = 1;
                                ui.selectedButton = 1;
                                break;
                            case 2:
                                selectedTool = 2;
                                ui.selectedButton = 2;
                                break;
                            case 3:
                                selectedTool = 3;
                                ui.selectedButton = 3;
                                break;
                            case 4:
                                selectedTool = 4;
                                ui.selectedButton = 4;
                                break;
                            case 5:
                                selectedTool = 5;
                                ui.selectedButton = 5;
                                break;
                            case 6:
                                selectedTool = 6;
                                ui.selectedButton = 6;
                                break;
                            }
                        }
                    }

                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
                {
                    selectedTool = 0;
                    ui.selectedButton = 0;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
                {
                    selectedTool = 1;
                    ui.selectedButton = 1;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
                {
                    selectedTool = 2;
                    ui.selectedButton = 2;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
                {
                    selectedTool = 3;
                    ui.selectedButton = 3;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5))
                {
                    selectedTool = 4;
                    ui.selectedButton = 4;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6))
                {
                    selectedTool = 5;
                    ui.selectedButton = 5;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7))
                {
                    selectedTool = 6;
                    ui.selectedButton = 6;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
                {
                    lsdMode = !lsdMode;

                    if (lsdMode == false) sprite.setColor(sf::Color(255,255,255));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M))
                {
                    tileGrid.EnableMove();
                }

                actionLockout = 0.01;
            } 
        }

        window.clear();

        for (int i = 0; i < tileCount; i++)
        {
            sprite.setTexture(textures[tileGrid.grid[i].GetSprite()]);

            sprite.setPosition(tileGrid.grid[i].position);
            //numberText.setPosition(tileGrid.GetTilePosition(i));
            //numberText.setString(std::to_string(i));
            sprite.setScale(sf::Vector2f(1 * (tileSize / 16), 1 * (tileSize / 16)));

            if (lsdMode == true)
            {
                sprite.setColor(sf::Color(cos(elapsedTime * acos(0) * 3) * 100 + 155, cos(elapsedTime * acos(0) * 5) * 100 + 155, cos(elapsedTime * acos(0) * 10) * 100 + 155));
            }

            window.draw(sprite);
            //window.draw(numberText);
        }

        for (int i = 0; i < ui.buttons.size(); i++)
        {
            if (lsdMode == true)
            {
                sprite.setColor(sf::Color(cos(elapsedTime * acos(0) * 3) * 100 + 155, cos(elapsedTime * acos(0) * 5) * 100 + 155, cos(elapsedTime * acos(0) * 10) * 100 + 155));
            }

            sprite.setTexture(ui.buttons[i].texture);

            sprite.setPosition(ui.buttons[i].position);
            //sprite.setPosition(sf::Vector2f((windowX / 2) + (i * 64) - 64 * 9 / 2, 950));

            sprite.setScale(sf::Vector2f(2, 2));

            if (i == ui.selectedButton) sprite.setColor(sf::Color(100, 100, 200));

            window.draw(sprite);

            sprite.setColor(sf::Color(255, 255, 255));
        }

        window.draw(fpsText);
        window.draw(moneyText);

        if (!lsdMode) lsdText.setFillColor(sf::Color(cos(elapsedTime * acos(0) * 3) * 100 + 155, cos(elapsedTime * acos(0) * 5) * 100 + 155, cos(elapsedTime * acos(0) * 10) * 100 + 155));
        else lsdText.setFillColor(sf::Color(255, 255, 255));
        window.draw(lsdText);
        window.draw(incomeText);
        

        window.display();

        window.setFramerateLimit(6000);



        elapsedTime += deltaTime;
        minute += deltaTime;

        incomeText.setString("Income: " + std::to_string((int)((moneyGen / minute) * 60)) + " / min");

        if (minute > 60)
        {
            minute = 0;
            moneyGen = 0;
        }

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
    moneyGen += m;
}