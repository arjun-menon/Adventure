#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow *rw = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Adventure");
    rw->clear(sf::Color(0, 0, 0));
    
    while(rw->isOpen()) {
        sf::Event event;
        while (rw->pollEvent(event))
        {
                // Close window : exit
                if (event.type == sf::Event::Closed)
                        rw->close();

                // Resize event : adjust viewport
                if (event.type == sf::Event::Resized) {
                        //window_size = Cd(Event.Size.Width, Event.Size.Height);
                        //rw->SetSize(Event.Size.Width, Event.Size.Height);

                        //glViewport(0, 0, event.size.width, event.size.height);
                }
        }
                
        rw->clear();
        rw->display();
    }
    
    return 0;
}
