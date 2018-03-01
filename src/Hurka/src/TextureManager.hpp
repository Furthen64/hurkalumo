#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Constants.hpp"

using namespace sf;


/// Inside the game code you use strings to find textures
/// The maps themselves use simple numbers for ease of writing maps but don't use them here in the code!

class TextureManager
{
public:

    TextureManager();

    void pushTexture(std::string _name, Texture _texture);

    Texture getTexture(std::string _key);

    bool applyTexture(std::string textureName, Texture *texture);

    bool applyTextureById(unsigned int _textureId, Texture *texture);

    std::string getTextureNameByIndex(int nr);  // Only used by Map to get the string name

    std::unordered_map<std::string,Texture> getTextureMap();

    static void validateTextureManager(TextureManager *textureMgr)
    {
        std::cout << "\n\nvalidateTextureManager *****\n";
        std::cout << "textureMap.size()= " << textureMgr->getTextureMap().size() << "\n";
        std::cout << "adress of object= " << textureMgr << "\n";

    }

private:
    std::string cn = "TextureManager.cpp";

    std::unordered_map<std::string,Texture> textureMap;


};

#endif
