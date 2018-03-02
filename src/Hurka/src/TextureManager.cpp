#include "TextureManager.hpp"



// TODO: Make this automatically assign a name to it?
// WE could doooo it like this? inside the .txt file, it can say
// "T001" which means TREE001 so it maps the first tree texture to that T001
// "H001" which means HOUSE001 , maps it to the first house texture   etc.

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

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TREE_002.png");
    pushTexture("TREE002", txt);


    // Grass
    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRASS_001.png");
    pushTexture("GRASS001", txt);

}



// Converts the integer to string version of textures in the library

std::string TextureManager::getTextureNameByIndex(int nr)
{

    switch(nr){
        case 0: return "GRASS001";
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

        default: return "";
    }

    return "";
}






// (-+)
void TextureManager::pushTexture(std::string _name, Texture _texture)
{
    textureMap.insert( {_name, _texture});
}

// (-+)
// FIXME behöver felhantering om vi inte hittar keyn
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
        std::cout << "ERROR " << cn << " applyTextureById could not find the texture: \"" << textureName << "\"!\n";
    } else {
        // Dereference our *texture and set it to the texture we have loaded into the hashmap
        (*texture) = textureMap[textureName];
        result = true;
    }

    return result;
}




// (--)
bool TextureManager::applyTextureById(unsigned int _textureId, Texture *texture)
{
    bool result = false;


    // Convert the integer to the right string name

    std::string _textureName = getTextureNameByIndex(_textureId);

    if(_textureName == "") {
        std::cout << "ERROR " << cn << " applyTextureById cannot find texturename by id " << _textureId << "\n";
        return false;
    }


    // Now find it by searching for the string
    std::unordered_map<std::string, Texture>::const_iterator got = textureMap.find(_textureName);

    if(got == textureMap.end()) {
        std::cout << "ERROR " << cn << " applyTextureById could not find the texture: \"" << _textureName << "\"!\n";
    } else {
        // Dereference our *texture and set it to the texture we have loaded into the hashmap
        (*texture) = textureMap[_textureName];
        result = true;
    }

    return result;
}




std::unordered_map<std::string,Texture> TextureManager::getTextureMap()
{
    return textureMap;
}
