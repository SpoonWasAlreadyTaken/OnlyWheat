#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <filesystem>


// program vars
inline int gridSizeX = 12;
inline int gridSizeY = 12;

inline float tileSize = 32;

inline int windowX = 1920;
inline int windowY = 1080;




//game vars

inline int tileCount;

inline bool lsdMode = false;

inline size_t persistentID{};

enum TileType { empty = 0, dirt, tilledDirt, wheat, windmill, well, scarecrow, pumpjack };
enum SelectedTool { none = 0, pick, hoe, plantWheat, placeWindmill, placeWell, placeScarecrow, placePumpjack };
enum UiTypes {Box = 0};


inline SelectedTool selectedTool = none;


// functions
inline size_t GetUID() { return persistentID++; }


//classes
class ClockKeeper 
{
public:
    float timeScale = 1;

    void Anchor(float input) { RawDeltaTime = input; ElapsedTime += RawDeltaTime; DeltaTime = input * timeScale; }
    float rawDeltaTime() { return RawDeltaTime; }
    float elapsedTime() { return ElapsedTime; }
    float deltaTime() { return DeltaTime; }

private:
    float RawDeltaTime = 0;
    float ElapsedTime = 0;
    float DeltaTime = 0;
};


class Shop 
{
public: 
    bool canGoIntoDebt = false;

    Shop(int startingMoney) { money = startingMoney; }


    bool Buy(int value) {
        if (money >= value || canGoIntoDebt && money > 0) { money -= value; return true; }
        return false;
    }
    void AddMoney(int value) { money += value; }
    int GetMoney() { return money; }


    void SellTile(TileType type) { money += GetTileCost(type) * sellBack; }

    int GetTileCost(TileType type) {
        switch (type)
        {
            case wheat:
                return 10;
                break;
            case windmill:
                return 30;
                break;
            case well:
                return 50;
                break;
            case scarecrow:
                return 10;
            case pumpjack:
                return 350;
                break;
            default:
                return 0;
                break;
        }
    }


private:
    int money{};
    float sellBack = 0.75;
};



class TextureManager
{
    sf::Texture missingTexture; 
    std::unordered_map<std::string, sf::Texture> textures;


public:
    sf::Texture& GetMissing() { return missingTexture; }
    sf::Texture& GetTexture(const std::string& name) { return (textures.contains(name)) ? textures[name] : missingTexture; }

    bool LoadTextures()
    {
        if (!missingTexture.loadFromFile("./Assets/Missing.png")) return 0;

        for (auto entry : std::filesystem::directory_iterator("Assets")) {

            if (entry.path().extension().string() == ".png") { 
                #ifndef NDEBUG
                std::cout << "Trying to load: " << "\"" <<entry.path().stem().string() << "\"" << " From: " << entry.path().string(); 
                if (textures[entry.path().stem().string()].loadFromFile(entry.path().string())) {
                   std::cout << "  Success!\n";
                }
                else std::cout << "  Fail!\n";
                #endif

                #ifdef NDEBUG
                ()textures[entry.path().stem().string()].loadFromFile(entry.path().string());
                #endif
            }
        }

        return 1;
    }
};


inline ClockKeeper tT = ClockKeeper();
inline TextureManager tm = TextureManager();
inline Shop shop = Shop(50000);
