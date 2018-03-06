#include "TextureManager.hpp"

 ///
 /// SINGLETON
 ///



// TODO: Make this automatically assign a name to it?
// WE could doooo it like this? inside the .txt file, it can say
// "T001" which means TREE001 so it maps the first tree texture to that T001
// "H001" which means HOUSE001 , maps it to the first house texture   etc.

// (--)




void TextureManager::loadTextures()
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

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_004.png");
    pushTexture("ROAD004", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_005.png");
    pushTexture("ROAD005", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_006.png");
    pushTexture("ROAD006", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_007.png");
    pushTexture("ROAD007", txt);



    // Trees


    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TREE_001.png");
    pushTexture("TREE001", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TREE_002.png");
    pushTexture("TREE002", txt);


    // Grass
    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRASS_001.png");
    pushTexture("GRASS001", txt);

    // Bus
    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\BUS_001.png");
    pushTexture("BUS001", txt);

}



// Converts the integer to string version of textures in the library


/// TODO: Automatisera detta :x s� det inte blir n�gra mer human errors av att man skriver fel
std::string TextureManager::getTextureNameByIndex(int nr)
{

    switch(nr){
        case 000: return "GRASS001";
        case 001: return "HOUSE001";
        case 002: return "HOUSE002";
        case 003: return "HOUSE003";
        case 004: return "HOUSE004";
        case 005: return "HOUSE005";
        case 006: return "HOUSE006";

        case 101: return "ROAD001";
        case 102: return "ROAD002";
        case 103: return "ROAD003";
        case 104: return "ROAD004";
        case 105: return "ROAD005";
        case 106: return "ROAD006";
        case 107: return "ROAD007";

        case 301: return "TREE001";
        case 302: return "TREE002";

        case 401: return "BUS001";


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
// FIXME beh�ver felhantering om vi inte hittar keyn
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




// (-+)
/// You supply id=001 and it looks up the texture name "HOUSE001"  and then assigns that to the texture pointer

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
