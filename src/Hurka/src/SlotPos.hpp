#ifndef SLOTPOS_H
#define SLOTPOS_H

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"

#include "HPos.hpp"



class SlotPos
{
public:
    SlotPos(HPos *_hpos);
    HPos *hpos;
    void draw( sf::RenderTarget& rt, HPos *viewHPos);
    void dump(std::string ind);

private:
    int textureID;
    sf::Texture texture;
    sf::IntRect textureSize;
    std::string textureName="SLOTPOS";
    sf::Sprite sprite;
    std::string cn = "SlotPos.cpp";
};

#endif
