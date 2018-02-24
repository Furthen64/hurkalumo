#include "TextureManager.hpp"
// (--)
TextureManager::TextureManager()
{

    Texture txt;

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_001.png");
    pushTexture("HOUSE001", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_009.png");
    pushTexture("HOUSE009", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_001.png");
    pushTexture("ROAD001", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_002.png");
    pushTexture("ROAD002", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_003.png");
    pushTexture("ROAD003", txt);


     txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TREE_001.png");
    pushTexture("TREE001", txt);

}

void TextureManager::pushTexture(std::string _name, Texture _texture)
{
    textureMap.insert( {_name, _texture});
}

Texture TextureManager::getTexture(std::string _key)
{
    return textureMap[_key];
}

//(--)
bool TextureManager::applyTexture(std::string textureName, Texture *texture)
{
    bool result = false;

    std::unordered_map<std::string, Texture>::const_iterator got = textureMap.find(textureName);

    if(got == textureMap.end()) {
        std::cout << "applyTexture not found texture: \"" << textureName << "\"\n";
    } else {
        // Dereference our *texture and set it to the texture we have loaded into the hashmap
        (*texture) = textureMap[textureName];
        result = true;
    }

    return result;
}

