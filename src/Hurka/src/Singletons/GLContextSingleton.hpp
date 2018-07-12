#ifndef GLCONTEXTSINGLETON_H
#define GLCONTEXTSINGLETON_H


#include <string>
#include <iostream>
/*
#include <unordered_map>
#include <SFML/Graphics.hpp>
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


    int doSomething();

private:

    GLContextSingleton() { std::cout << "GLContextSingleton has been allocated - SINGLETON - \n"; }
    ~GLContextSingleton() { std::cout << "GLContextSingleton has been destroyed - SINGLETON - \n";}

    // private copy constructor and assignment operator
    GLContextSingleton(const GLContextSingleton&);
    GLContextSingleton& operator=(const GLContextSingleton&);

    static GLContextSingleton *m_instanceSingleton;

    std::string cn = "GLContextSingleton.cpp";
};

#endif
