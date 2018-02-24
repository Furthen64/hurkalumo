#include <string>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Constants.hpp"

using namespace sf;

class TextureManager
{
public:

    TextureManager();

    void pushTexture(std::string _name, Texture _texture);

    Texture getTexture(std::string _key);

    bool applyTexture(std::string textureName, Texture *texture);


private:

    std::unordered_map<std::string,Texture> textureMap;


};
