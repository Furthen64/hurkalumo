#include <stdlib.h>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <chrono>


using namespace sf;



/// //////////////////////////////////////////////
/// Globals


/// //////////////////////////////////////////////
/// Locomotive
class Locomotive
{
public:
    Locomotive(const Vector2f& _pos)
        :
        pos(_pos)
    {
        // ? ?
        texture.loadFromFile("C:\\fat64\\projects\\Hurka\\bin\\Release\\ANIM_001.png");
        sprite = Sprite(texture);
        sprite.setTextureRect( {0,0,30,30});



    }
    void draw( RenderTarget& rt) const
    {
        rt.draw( sprite );
    }
    void setDirectionConst( const Vector2f& dir)
    {
        vel = dir * speed; //??
    }

    void setDirection( Vector2f dir)
    {
        vel = dir * speed; //??
    }

    void update( float dt)
    {
        pos += vel * dt;
        sprite.setPosition(pos);
    }

    Vector2f getPos()
    {
        return pos;
    }

private:
    static constexpr float speed = 100.0f;  // move 100 px per second
    Vector2f pos;
    Vector2f vel = {0.0f, 0.0f};
    Texture texture;
    Sprite sprite;
};



/// ///////////////////////////////////////////////////////////////
/// Start

int main()
{
    // Setup Window
    RenderWindow window(sf::VideoMode(640, 480), "Lumo Editor 0.1-alpha");

    // Setup objects
    Locomotive loco({10.0f, 10.0f});

    // Setup timing
    auto tp = std::chrono::steady_clock::now();




    /// Main Loop

    while (window.isOpen())
    {




        /// Get events

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }



        float dt;
        {
            const auto newTp = std::chrono::steady_clock::now();
            dt = std::chrono::duration<float>(newTp - tp).count();
            tp = newTp;
        };


        /// Update

        // Get mouse input
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos_i = sf::Mouse::getPosition(window); // window is a sf::Window

            Vector2f mousePos_f = Vector2f();
            mousePos_f.x = mousePos_i.x;
            mousePos_f.y = mousePos_i.y;

            bool rightof = false;
            bool belowof = false;
            bool topof = false;
            bool leftof = false;
            Vector2f dir;

            // Figure out what +-1 to do with the position
            if(loco.getPos().x < mousePos_f.x) {
                rightof = true;
            }
            if(loco.getPos().x >= mousePos_f.x) {
                leftof = true;
            }
            if(loco.getPos().y < mousePos_f.y) {
                belowof = true;
            }
            if(loco.getPos().y >= mousePos_f.y) {
                topof = true;
            }

            if(rightof) {
                dir.x += 1.0f;
            }
            if(leftof) {
                dir.x -= 1.0f;
            }
            if(belowof) {
                dir.y += 1.0f;
            }
            if(topof) {
                dir.y -= 1.0f;
            }

            // if we are right of the object

            loco.setDirection(dir);


        }

        loco.update(dt);





        /// Render
        window.clear();

        // Draw sprites
        loco.draw(window);

        window.display();


    }

    return 0;
}

/*
KodBös:


    RectangleShape shape1(Vector2f(20.f,20.f));
    shape1.setPosition(Vector2f(50.f, 50.f));
    shape1.setFillColor(Color(100, 250, 50));

    shape1.setOutlineThickness(10);
    shape1.setOutlineColor(sf::Color(10, 10, 10));

  texture.loadFromFile("ANIM_001.png");
        sprite = Sprite(texture);
        sprite.setTextureRect( {0,0,30,30});




  sf::Rect rect1(0.f, 0.f);
    rect1.setFillColor(sf::Color(100, 250, 50));
    sf::Rect rect2(320.f,240.f);


     window.draw(shape1);
        window.draw(sprite);
*/


