#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>

#include "globals.hpp"



#include "TileGrid.hpp"
#include "UI.hpp"




TileGrid tileGrid = TileGrid(gridSizeX, gridSizeY, tileSize, windowX, windowY);
UI ui = UI();

std::vector<std::string> buttonsToAdd = {"Pick", "Hoe", "WheatUI", "WindmillUI", "WellUI", "ScarecrowUI", "PumpjackUI", "EmptyBox", "EmptyBox"};




int main()
{
    if (!tm.LoadTextures()) { std::cout << "Failed to load textures\n"; return 1; } //ALWAYS FIRST

    tileCount = gridSizeX * gridSizeY;

    sf::RenderWindow window(sf::VideoMode({ (uint32_t)(windowX), (uint32_t)(windowY) }), "Only Wheat");

    // creating game constructs
    
    tileGrid.CreateGrid();

    for (int i = 0; i < buttonsToAdd.size(); i++)
    {
        ui.AddButton(sf::Vector2f(((float)windowX / 2) + (i * tileSize * 2) - (float)tileSize * 2 * 9 / 2, windowY - (tileSize * 3)), tileSize, tileSize, buttonsToAdd[i]);
    }

    // fps code
    sf::Clock clock;
    int fps = 0;
    int steps = 0;
    int avgFPS = 0; 


    // text code
    sf::Font font;
    (void)font.openFromFile("./Assets/VIRUST.ttf");

    sf::Text fpsText(font, "60");
    sf::Text timeScaleText(font, "1");
    sf::Text moneyText(font, "$ " + std::to_string(shop.GetMoney()));
    sf::Text lsdText(font, "Press \"L\" for LSD Mode");
    sf::Text numberText(font, "0");
    sf::Text incomeText(font, "Income: ");
    moneyText.setFillColor(sf::Color(0, 255, 0));
    fpsText.setPosition(sf::Vector2f(10, 5));
    timeScaleText.setPosition(sf::Vector2f(10,900));
    timeScaleText.setScale(sf::Vector2f(0.5, 0.5));
    moneyText.setPosition(sf::Vector2f(10,55));
    incomeText.setPosition(sf::Vector2f(10, 105));
    lsdText.setPosition(sf::Vector2f(10, 1000));
    lsdText.setScale(sf::Vector2f(0.5, 0.5));
    numberText.setScale(sf::Vector2f(0.5, 0.5));




    
    sf::Sprite sprite(tm.GetMissing());

    float actionLockout = 0;

    // main loop
    while (window.isOpen())
    {
        tT.Anchor(clock.restart().asSeconds());
        actionLockout -= tT.rawDeltaTime();
        fps += 1.f / (tT.rawDeltaTime());

        if (steps == 10)
        {
            avgFPS = fps / steps;
            fpsText.setString("FPS: " + std::to_string(avgFPS));
            timeScaleText.setString(std::to_string(tT.timeScale));
            moneyText.setString("$ " + std::to_string(shop.GetMoney()));
            fps = 0;
            steps = 0;
        }
        steps++;

        
        for (int i = 0; i < tileCount; i++) { tileGrid.grid[i]->Update(); }
        


        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) { window.close(); }

            if (actionLockout <= 0)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

                    //std::cout << mousePos.x << "  " << mousePos.y << "\n";

                    for (int i = 0; i < tileCount; i++)
                    {
                        if (tileGrid.grid[i]->position.x + tileSize * 2 > mousePos.x && tileGrid.grid[i]->position.x < mousePos.x && tileGrid.grid[i]->position.y + tileSize * 2 > mousePos.y && tileGrid.grid[i]->position.y < mousePos.y)
                        {
                            switch(selectedTool)
                            {
                                case none:
                                    break;
                                case pick:
                                    switch (tileGrid.grid[i]->type)
                                    {
                                        case dirt:
                                            break;
                                        case wheat:
                                            tileGrid.ChangeTile(i, dirt);
                                            break;
                                        default:
                                            shop.SellTile(tileGrid.grid[i]->type);
                                            tileGrid.ChangeTile(i, dirt);
                                            break;
                                    }
                                    break;
                                case hoe:
                                    if (tileGrid.grid[i]->type == dirt) tileGrid.ChangeTile(i, tilledDirt);
                                    break;
                                case plantWheat:
                                    if (tileGrid.grid[i]->type == tilledDirt) { 
                                        if (shop.Buy(shop.GetTileCost(wheat))) tileGrid.ChangeTile(i, wheat); 
                                    };
                                    break;
                                case placeWindmill:
                                    if (tileGrid.grid[i]->type == dirt) { 
                                        if (shop.Buy(shop.GetTileCost(windmill))) tileGrid.ChangeTile(i, windmill);
                                    };
                                    break;
                                case placeWell:
                                    if (tileGrid.grid[i]->type == dirt) { 
                                        if (shop.Buy(shop.GetTileCost(well))) tileGrid.ChangeTile(i, well);
                                    };
                                    break; 
                                case placeScarecrow:
                                    if (tileGrid.grid[i]->type == dirt) { 
                                        if (shop.Buy(shop.GetTileCost(scarecrow))) tileGrid.ChangeTile(i, scarecrow);
                                    };
                                    break; 
                                case placePumpjack:
                                    if (tileGrid.grid[i]->type == dirt) { 
                                        if (shop.Buy(shop.GetTileCost(pumpjack))) tileGrid.ChangeTile(i, pumpjack);
                                    };
                                    break;
                            }
                        }
                    }   
                    for (int i = 0; i < ui.buttons.size(); i++)
                    {
                        if (ui.buttons[i].position.x + ui.buttons[i].sizeX * 2 > mousePos.x && ui.buttons[i].position.x < mousePos.x && ui.buttons[i].position.y + ui.buttons[i].sizeY * 2 > mousePos.y && ui.buttons[i].position.y < mousePos.y)
                        {
                            switch (i)
                            {
                            case 0:
                                selectedTool = pick;
                                ui.selectedButton = 0;
                                break;
                            case 1:
                                selectedTool = hoe;
                                ui.selectedButton = 1;
                                break;
                            case 2:
                                selectedTool = plantWheat;
                                ui.selectedButton = 2;
                                break;
                            case 3:
                                selectedTool = placeWindmill;
                                ui.selectedButton = 3;
                                break;
                            case 4:
                                selectedTool = placeWell;
                                ui.selectedButton = 4;
                                break;
                            case 5:
                                selectedTool = placeScarecrow;
                                ui.selectedButton = 5;
                                break;
                            case 6:
                                selectedTool = placePumpjack;
                                ui.selectedButton = 6;
                                break;
                            }
                        }
                    }

                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
                {
                    selectedTool = pick;
                    ui.selectedButton = 0;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
                {
                    selectedTool = hoe;
                    ui.selectedButton = 1;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
                {
                    selectedTool = plantWheat;
                    ui.selectedButton = 2;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
                {
                    selectedTool = placeWindmill;
                    ui.selectedButton = 3;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5))
                {
                    selectedTool = placeWell;
                    ui.selectedButton = 4;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6))
                {
                    selectedTool = placeScarecrow;
                    ui.selectedButton = 5;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7))
                {
                    selectedTool = placePumpjack;
                    ui.selectedButton = 6;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
                {
                    lsdMode = !lsdMode;

                    if (lsdMode == false) sprite.setColor(sf::Color(255,255,255));
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
                {
                    tT.timeScale += 0.05;
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
                {
                    tT.timeScale -= 0.05;
                    tT.timeScale = tT.timeScale * (tT.timeScale >= 0);
                }

                actionLockout = 0.01;
            } 
        }


        window.clear();

        for (int i = 0; i < tileCount; i++)
        {
            sprite.setTexture(*tileGrid.grid[i]->texture);

            sprite.setPosition(tileGrid.grid[i]->position);
            sprite.setScale(sf::Vector2f(1 * (tileSize / 16), 1 * (tileSize / 16)));

            if (lsdMode == true)
            {
                float d = tT.elapsedTime() + i * 0.001;
                sprite.setColor(sf::Color(cos(d * acos(0) * 3) * 100 + 155, cos(d * acos(0) * 5) * 100 + 155, cos(d * acos(0) * 10) * 100 + 155));
            }

            window.draw(sprite);
            //window.draw(numberText);
        }

        for (int i = 0; i < ui.buttons.size(); i++)
        {
            if (lsdMode == true)
            {
                float d = tT.elapsedTime() + i * 0.001;
                sprite.setColor(sf::Color(cos(d * acos(0) * 3) * 100 + 155, cos(d * acos(0) * 5) * 100 + 155, cos(d * acos(0) * 10) * 100 + 155));
            }

            sprite.setTexture(*ui.buttons[i].texture);

            sprite.setPosition(ui.buttons[i].position);

            sprite.setScale(sf::Vector2f(2, 2));

            if (i == ui.selectedButton) sprite.setColor(sf::Color(100, 100, 200));

            window.draw(sprite);

            sprite.setColor(sf::Color(255, 255, 255));
        }



        window.draw(fpsText);
        window.draw(timeScaleText);
        window.draw(moneyText);

        if (!lsdMode) lsdText.setFillColor(sf::Color(cos(tT.elapsedTime() * acos(0) * 3) * 100 + 155, cos(tT.elapsedTime() * acos(0) * 5) * 100 + 155, cos(tT.elapsedTime() * acos(0) * 10) * 100 + 155));
        else lsdText.setFillColor(sf::Color(255, 255, 255));
        window.draw(incomeText);
        
        window.draw(lsdText);

        window.display();

        //window.setFramerateLimit(6000);

        incomeText.setString("Income: ");
    }
}
