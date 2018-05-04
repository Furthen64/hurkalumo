#include "TextureManager.hpp"

 ///
 /// SINGLETON
 ///



/// Wishlist   Make sure it cannot be ran twice,

/// Wishlist   Make it smarter, like:
        // TODO: Make this automatically assign a name to it?
        // WE could doooo it like this? inside the .txt file, it can say
        // "T001" which means TREE001 so it maps the first tree texture to that T001
        // "H001" which means HOUSE001 , maps it to the first house texture   etc.


void TextureManager::loadTextures()
{

    sf::Texture txt;

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

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_007.png");
    pushTexture("HOUSE007", txt);
/*
    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_008.png");
    pushTexture("HOUSE008", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_009.png");
    pushTexture("HOUSE009", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\HOUSE_010.png");
    pushTexture("HOUSE010", txt);

*/


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

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\ROAD_008.png");
    pushTexture("ROAD008", txt);



    // Trees


    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TREE_001.png");
    pushTexture("TREE001", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TREE_002.png");
    pushTexture("TREE002", txt);

    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\TREE_003.png");
    pushTexture("TREE003", txt);


    // Grass
    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\GRASS_001.png");
    pushTexture("GRASS001", txt);

    // Bus
    txt.loadFromFile("C:\\github\\lumo\\src\\Hurka\\bin\\Release\\BUS_001.png");
    pushTexture("BUS001", txt);

}



// Converts the integer to string version of textures in the library


/// TODO: Automatisera detta :x så det inte blir några mer human errors av att man skriver fel
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
        case 7: return "HOUSE007";
        /*case 8: return "HOUSE008";
        case 9: return "HOUSE009";
        case 10: return "HOUSE010";*/

        case 101: return "ROAD001";
        case 102: return "ROAD002";
        case 103: return "ROAD003";
        case 104: return "ROAD004";
        case 105: return "ROAD005";
        case 106: return "ROAD006";
        case 107: return "ROAD007";
        case 108: return "ROAD008";

        case 301: return "TREE001";
        case 302: return "TREE002";
        case 303: return "TREE003";


        case 401: return "BUS001";


        default:
            std::cout << cn << " could not find texture nr = " << nr << "\n";
            return "";
    }



    return "";
}





/// FIXME! Needs error handling if we can't find the key
/// (--)
sf::Texture TextureManager::getTexture(std::string _key)
{

    if(textureMap.size() <1) {
        std::cout << "ERROR " << cn << " The textures have not yet been loaded, and the program wants to find a texture. System failure!\n";
        throw std::exception();
    }
    return textureMap[_key];
}



/// Add texture object into the internal storage
/// (-+)
void TextureManager::pushTexture(std::string _name, sf::Texture _texture)
{
    textureMap.insert( {_name, _texture});
}


/// Given a key and an already loaded texture, put it into the internal storage
///(-+)
bool TextureManager::applyTexture(std::string textureName, sf::Texture *texture)
{
    bool result = false;

    std::unordered_map<std::string, sf::Texture>::const_iterator got = textureMap.find(textureName);

    if(got == textureMap.end()) {
        std::cout << "ERROR " << cn << " applyTextureById could not find the texture: \"" << textureName << "\"!\n";
    } else {
        // Dereference our *texture and set it to the texture we have loaded into the hashmap
        (*texture) = textureMap[textureName];
        result = true;
    }

    return result;
}





/// Update the datastructure with this texture pointer
/// Example: supply id=001 and it looks up the texture name "HOUSE001"  and then assigns that to the texture pointer
/// (-+)
bool TextureManager::applyTextureById(unsigned int _textureId, sf::Texture *texture)
{
    bool result = false;


    // Convert the integer to the right string name

    std::string _textureName = getTextureNameByIndex(_textureId);

    if(_textureName == "") {
        std::cout << "ERROR " << cn << " applyTextureById cannot find texturename by id " << _textureId << "\n";
        return false;
    }


    // Now find it by searching for the string
    std::unordered_map<std::string, sf::Texture>::const_iterator got = textureMap.find(_textureName);

    if(got == textureMap.end()) {
        std::cout << "ERROR " << cn << " applyTextureById could not find the texture: \"" << _textureName << "\"!\n";
    } else {
        // Dereference our *texture and set it to the texture we have loaded into the hashmap
        (*texture) = textureMap[_textureName];
        result = true;
    }

    return result;
}



// (TEST)
std::unordered_map<std::string,sf::Texture> TextureManager::getTextureMap()
{
    std::cout << "getTextureMap() not done! Makes a shallow copy?\n";
    return textureMap;
}



// (--)
int TextureManager::nrOfTextures()
{
    return textureMap.size();
}
