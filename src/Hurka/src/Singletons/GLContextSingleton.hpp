#ifndef GLCONTEXTSINGLETON_H
#define GLCONTEXTSINGLETON_H


#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
/*
#include <unordered_map>

#include <exception>
#include "../Constants.hpp"
*/

class GLContextSingleton
{
public:

    static GLContextSingleton* getInstance() {

        return (!m_instanceSingleton) ?
            m_instanceSingleton = new GLContextSingleton :
            m_instanceSingleton;
    }

    sf::Context sfContext;

    int doSomething();

private:

    GLContextSingleton() { }
    ~GLContextSingleton() { }

    // private copy constructor and assignment operator
    GLContextSingleton(const GLContextSingleton&);
    GLContextSingleton& operator=(const GLContextSingleton&);

    static GLContextSingleton *m_instanceSingleton;

    std::string cn = "GLContextSingleton.cpp";
};

#endif
