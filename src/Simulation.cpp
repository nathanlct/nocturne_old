#include "Simulation.hpp"


Simulation::Simulation(World* world) :
    world(world)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window = new sf::RenderWindow(sf::VideoMode(WIN_SIZE.x, WIN_SIZE.y),
                                  "Nocturne", sf::Style::Default, settings);
    
    main_view = sf::View(sf::FloatRect(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y - 50.0f));
    status_bar_view = sf::View(sf::FloatRect(0.0f, 0.0f, WIN_SIZE.x, 50.0f));
    main_view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, (WIN_SIZE.y - 50.0f) / WIN_SIZE.y));
    status_bar_view.setViewport(sf::FloatRect(0.0f, (WIN_SIZE.y - 50.0f) / WIN_SIZE.y, 1.0f, 50.0f / WIN_SIZE.y));

    if (!font.loadFromFile("../resources/Verdana.ttf"))
    {
        std::cout << "ERROR: couldn't load font." << std::endl;
    }

    mouse_pos = sf::Vector2f(0.0f, 0.0f);
    dragging = false;
}

bool Simulation::handleEvents() {
    sf::Event event;
    while(window->pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                window->close();
                return false;
                break;
            case sf::Event::Resized:
                main_view.reset(sf::FloatRect(0, 0, event.size.width, event.size.height - 50.0f));
                status_bar_view.reset(sf::FloatRect(0, 0, event.size.width, 50.0f));
                main_view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, (event.size.height - 50.0f) / event.size.height));
                status_bar_view.setViewport(sf::FloatRect(0.0f, (event.size.height - 50.0f) / event.size.height, 1.0f, 50.0f / event.size.height));
                break;
            case sf::Event::MouseWheelScrolled:
                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    float delta = event.mouseWheelScroll.delta;
                    main_view.zoom(1 + delta / 100.0f);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left) {
                    mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                    dragging = true;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if(event.mouseButton.button == sf::Mouse::Left) {
                    dragging = false;
                }
                break;
            case sf::Event::MouseMoved:
            {
                if(dragging) {
                    sf::Vector2f new_mouse_pos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
                    sf::Vector2f diff = new_mouse_pos - mouse_pos;
                    main_view.move(- diff);
                    mouse_pos = new_mouse_pos;
                }
                break;
            }
            default:
                break;
        }
    }

    return true;
}

bool Simulation::step(float elapsed) {
    window->clear(sf::Color::White);

    world->step();

    window->setView(main_view);
    window->draw(*world);

    window->setView(status_bar_view);

    sf::RectangleShape status_bar_bg({(float)window->getSize().x, 50.0f});
    status_bar_bg.setPosition(0.0f, 0.0f);
    status_bar_bg.setFillColor(sf::Color(50, 50, 50));
    window->draw(status_bar_bg);

    sf::Vector2i rel = sf::Mouse::getPosition(*window);
    sf::Vector2f abs = window->mapPixelToCoords(rel, main_view);

    std::ostringstream string_stream;
    string_stream << "Relative (" << rel.x << ", " << rel.y << ")px - ";
    string_stream << "\tAbsolute (" << abs.x << ", " << abs.y << ")px = ";
    string_stream << "\t(" << abs.x / m2px << ", " << - (abs.y - WIN_SIZE.y) / m2px << ")m";
    std::string text_str = string_stream.str();

    sf::Text text;
    text.setFont(font);
    text.setPosition(10, 10);
    text.setString(text_str);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    window->draw(text);

    window->display();

    return handleEvents();
}

bool Simulation::isRunning() {
    return window->isOpen();
}