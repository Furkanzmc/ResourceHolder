#include "SFML/Graphics.hpp"
#include "ResourceHolder.h"

enum Textures {
    A,
    B,
    C,
    D
};

int main()
{
    // Create a stream and open a file that is inside "MyWonderfulArchive.7z".
    PhysFsStream wonderfullStream("B.zip");//Or use PhysFsStream::addSearchPath(const char *compressedFilePath)
    wonderfullStream.open("A.jpg");//Open a file inside of the zip file
    ResourceHolder<sf::Texture, Textures> rc;
    //You can use only ResourceHolder::load function if you don't want to load assets from stream
    rc.loadFromStream(Textures::A, wonderfullStream);
    wonderfullStream.open("B.jpg");
    rc.loadFromStream(Textures::B, wonderfullStream);
    wonderfullStream.open("C.jpg");
    rc.loadFromStream(Textures::C, wonderfullStream);
    wonderfullStream.open("D.png");
    rc.loadFromStream(Textures::D, wonderfullStream);
    sf::Sprite sprite;
    // Create a window and display the sprite
    sf::RenderWindow app(sf::VideoMode(1368, 768), "PhysFS", sf::Style::Default);
    app.setFramerateLimit(60);//Set the framerate to 60
    while(app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                app.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                app.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                sprite.setTexture(rc.get(Textures::A));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                sprite.setTexture(rc.get(Textures::B));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                sprite.setTexture(rc.get(Textures::C));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                sprite.setTexture(rc.get(Textures::D));
            }
        }
        app.clear();
        app.draw(sprite);
        app.display();
    }
    return 0;
}
