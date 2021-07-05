#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>
#include <stdlib.h>
#include <array>

class Application 
{
    public:
        Application();
        void run();


    private:
        void processEvents();
        void update();
        void render();

        float getBPM(unsigned amountToConsider) const;
        std::string getStringBPM(float BPM) const;


    private:
        static const sf::Time TimePerFrame;

        sf::RenderWindow mWindow;

        // KPS related
        std::array<unsigned, 60> mBuffer;
        std::array<sf::RectangleShape, 60> mGraphicsBuffer;
        unsigned mBufferPointer;
        unsigned mKeysPerSecond;
        sf::Text mKPS;

        // BPM related
        std::array<unsigned, 15> mLastKPSs;
        unsigned mKPSBufferPointer;
        sf::Text mBPM;

        // Graphical elements
        float mWidth;
        float mSpaceBetween;
        float mHeight;

        // Assets
        sf::Font mFont;
        
};