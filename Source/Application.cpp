#include "../Headers/Application.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60);

Application::Application()
: mWindow (sf::VideoMode((20 + 3) * 60 + 3, 600), "Buffer visualization")
, mBufferPointer(0U)
, mKeysPerSecond(0U)
, mKPSBufferPointer(0U)
, mWidth(20.f)
, mSpaceBetween(3.f)
, mHeight(10.f)
{
    mWindow.setKeyRepeatEnabled(false);   

    for (unsigned i = 0; i < mBuffer.size(); ++i)
    {
        mBuffer[i] = 0U;
        mGraphicsBuffer[i].setSize(sf::Vector2f(mWidth, mHeight));
        mGraphicsBuffer[i].setPosition((mWidth + mSpaceBetween) * i, 
            mWindow.getSize().y - 100);
        mGraphicsBuffer[i].setOrigin(0, mHeight);
    }

    for (auto &elem : mLastKPSs)
        elem = 0;


    if (!mFont.loadFromFile("../Media/One-Trick-Pony.ttf"))
    {
        std::cout << "Failed to load the font\n";
        exit(1);
    }
    mKPS.setFont(mFont);
    mBPM.setFont(mFont);

    mKPS.setString(std::to_string(0));
    mBPM.setString(std::to_string(0));

    mKPS.setCharacterSize(50);
    mBPM.setCharacterSize(50);

    mBPM.setPosition(0, mKPS.getPosition().y + mKPS.getLocalBounds().height + 20.f);
}

void Application::run() 
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen()) 
    {
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate >= TimePerFrame) 
        {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            update();
        }

        render();
    }
}

void Application::processEvents()
{
    sf::Keyboard::Key key1(sf::Keyboard::Z);
    sf::Keyboard::Key key2(sf::Keyboard::X);

    auto &elem = mBuffer[mBufferPointer];
    mKeysPerSecond -= elem;
    elem = 0;
    mGraphicsBuffer[mBufferPointer].setSize(sf::Vector2f(
        mWidth, mHeight * (elem + 1)));

    sf::Event event;
    while (mWindow.pollEvent(event)) 
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == key1 ||  event.key.code == key2)
            {
                ++mKeysPerSecond;
                ++elem;
            }
        }
        if (event.type == sf::Event::Closed
        || (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
            mWindow.close();
    }

    mKPS.setString("KPS: " + std::to_string(mKeysPerSecond));
    mBPM.setString(
        "BPM (1): "  + getStringBPM(getBPM(1))  + "\n" +
        "BPM (3): "  + getStringBPM(getBPM(3))  + "\n" +
        "BPM (5): "  + getStringBPM(getBPM(5))  + "\n" +
        "BPM (7): "  + getStringBPM(getBPM(7))  + "\n" +
        "BPM (9): "  + getStringBPM(getBPM(9))  + "\n" +
        "BPM (11): " + getStringBPM(getBPM(11)) + "\n" +
        "BPM (13): " + getStringBPM(getBPM(13)) + "\n" +
        "BPM (15): " + getStringBPM(getBPM(15)));
}

void Application::update() 
{
    static unsigned prevIndex;
    auto &elem = mBuffer[mBufferPointer];
    
    mGraphicsBuffer[mBufferPointer].setOrigin(0, mHeight * (elem + 1));
    mGraphicsBuffer[mBufferPointer].setSize(sf::Vector2f(mWidth, mHeight * (elem + 1)));
    mGraphicsBuffer[mBufferPointer].setFillColor(sf::Color::Red);
    mGraphicsBuffer[prevIndex].setFillColor(sf::Color::White);
    
    prevIndex = mBufferPointer;
    if (++mBufferPointer == 60)
        mBufferPointer = 0U;

    mLastKPSs[mKPSBufferPointer] = mKeysPerSecond;
    if (++mKPSBufferPointer == 15)
        mKPSBufferPointer = 0U;
}

void Application::render() 
{
    mWindow.clear();

    for (auto &elem : mGraphicsBuffer)
        mWindow.draw(elem);
    mWindow.draw(mKPS);
    mWindow.draw(mBPM);

    mWindow.display();
}

// Amount of last KPSes to consider
float Application::getBPM(unsigned amountToConsider) const
{
    unsigned ptr = mKPSBufferPointer;
    float BPM = 0.f;

    for (unsigned i = 0U; i < amountToConsider; ++i)
    {
        BPM += mLastKPSs[ptr] * 15;
        if (++ptr >= mLastKPSs.size())
            ptr = 0;
    }

    return BPM / amountToConsider;
}

std::string Application::getStringBPM(float BPM) const
{
    char *str = (char*)calloc(10, sizeof(char));
    sprintf(str, "%.2f", BPM);
    return std::string(str);
}
