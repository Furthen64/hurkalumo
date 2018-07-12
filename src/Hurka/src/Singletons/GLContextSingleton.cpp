#include "GLContextSingleton.hpp"


int doSomething()
{
}



/*

#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "../Utils.hpp"


///
/// SINGLETON
///



/// 2018-06-22  jörgen engström         Now loads all textures that are listed in a .txt file


// Wishlist:
// automatically assign a name to a texture by its filename?

// Wishlist:
// WE could doooo it like this? inside the garden.txt file, it can say
// "T001" which means TREE001 so it maps the first tree texture to that T001
// "H001" which means HOUSE001 , maps it to the first house texture   etc.



// Returns 0 on ok , -1 on failure
// (-+)
int TextureManager::loadTextures()
{
    sf::Texture txt;
    bool failed = false;


    // Read a textfile with all the textures listed in it

    std::string texturesFileList = getFullUri("data\\textures.txt");
    std::string line;
    std::string textureFullUri;
    std::string textureName;
    std::list<std::string> textureList;




    std::ifstream myfile (texturesFileList);
    if (myfile.is_open())
    {
       while ( myfile.good() )
       {
          getline (myfile,line);

          // Is it a whitespace line? ignore it
          if(line != "") {

                  // Create the correct names for the datastructure

                  textureName = "data\\textures\\";
                  textureName += line;
                  textureName += ".png";

                  textureFullUri = getFullUri(textureName);



                  if(!txt.loadFromFile(textureFullUri)) {
                        std::cout << " texture load from file failed with \"" << textureFullUri << "\"\n";
                        failed = true;

                  }


                  // Put it in the ADT
                  pushTexture(line, txt);

            }

        }

        myfile.close();

    }  else {
        std::cout << "ERROR " << cn << " Unable to open file " << texturesFileList << "\n";
        return -1;
    }

    if(failed) {
            return -1;
    }


    return 0;
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
//(--) buggy for now
bool TextureManager::applyTexture(std::string textureName, sf::Texture *texture, bool startGLContext)
{
    bool result = false;

    std::unordered_map<std::string, sf::Texture>::const_iterator got = textureMap.find(textureName);

    if(got == textureMap.end()) {
        std::cout << "ERROR " << cn << " applyTextureById could not find the texture: \"" << textureName << "\"!\n";
    } else {

        // Dereference our *texture and set it to the texture we have loaded into the hashmap

        (*texture) = textureMap[textureName];       // #CR24 - This thing needs a GL Context to work. Without it it will spit out GL_FLUSH errors!


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
*/
