#include "TextureManager.hpp"
// (--)
TextureManager::TextureManager()
{

    Texture txt;

    // Houses

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_001.png");
    pushTexture("HOUSE001", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_002.png");
    pushTexture("HOUSE002", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_003.png");
    pushTexture("HOUSE003", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_004.png");
    pushTexture("HOUSE004", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_005.png");
    pushTexture("HOUSE005", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_006.png");
    pushTexture("HOUSE006", txt);


    // Road

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_001.png");
    pushTexture("ROAD001", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_002.png");
    pushTexture("ROAD002", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_003.png");
    pushTexture("ROAD003", txt);


    // Trees


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


// Converts the integer to string version of textures in the library
std::string textureName(int nr)
{
    switch(nr){
        case 1: return "HOUSE001";
        case 2: return "HOUSE002";
        case 3: return "HOUSE003";
        case 4: return "HOUSE004";
        case 5: return "HOUSE005";
        case 6: return "HOUSE006";

        case 7: return "ROAD001";
        case 8: return "ROAD002";
        case 9: return "ROAD003";

        case 10: return "TREE001";
        case 11: return "TREE002";
        case 12: return "TREE003";
    }

    return "";
}
