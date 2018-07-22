#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <exception>
#include "../Constants.hpp"


/// Inside the game code you use strings to find textures
/// The maps themselves use simple numbers for ease of writing maps but don't use them here in the code!


// Example usage:
//

//     TextureManager *textureMgr;
//     textureMgr = textureMgr->getInstance();
//     int returnStatus = textureMgr->loadTextures();        <-- loads all the textures listed in a textfile
//                                                          and puts them in a map



//     TextureManager *textureMgr;
//     textureMgr = textureMgr->getInstance();
//     textureName = textureMgr->getTextureNameByIndex( 101 );
//
//




class TextureManager
{
public:


    // Singleton specific Functions

    static TextureManager* getInstance() {

        return (!m_instanceSingleton) ?
            m_instanceSingleton = new TextureManager :
            m_instanceSingleton;
    }


    // Special functions
    int loadTextures();


    // Basic functions
    void dump();

    void pushTexture(std::string _name, sf::Texture _texture);
    sf::Texture getTexture(std::string _key);

    bool applyTexture(std::string textureName, sf::Texture *texture, bool startGLContext);
    bool applyTextureById(unsigned int _textureId, sf::Texture *texture, bool startGLContext);





    std::string getTextureNameByIndex(int nr);  // Only used by Map to get the string name

    std::unordered_map<std::string,sf::Texture> getTextureMap();

    int nrOfTextures();

    static void validateTextureManager(TextureManager *textureMgr)
    {
        std::cout << "\n\nvalidateTextureManager *****\n";
        std::cout << "textureMap.size()= " << textureMgr->getTextureMap().size() << "\n";
        std::cout << "adress of object= " << textureMgr << "\n";

    }

private:

    // Singleton specific:

        TextureManager() {  }
        ~TextureManager() { }


        // private copy constructor and assignment operator
        TextureManager(const TextureManager&);
        TextureManager& operator=(const TextureManager&);

        static TextureManager *m_instanceSingleton;


    // Regular private members

        bool loadedTextures = false;

        std::string cn = "TextureManager.cpp";

        std::unordered_map<std::string,sf::Texture> textureMap;

};

#endif
