#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int AIRCRAFT_SPEED = 5;
const int OBSTACLE_SPEED = 5;

int main()
{
    // Create the window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Flight Game");

    // Load textures
    sf::Texture aircraftTexture;
    if (!aircraftTexture.loadFromFile("aircraft.png"))
    {
        std::cerr << "Failed to load aircraft.png" << std::endl;
        return 1;
    }

    sf::Texture obstacleTexture;
    if (!obstacleTexture.loadFromFile("obstacle.png"))
    {
        std::cerr << "Failed to load obstacle.png" << std::endl;
        return 1;
    }

    // Create sprites
    sf::Sprite aircraft(aircraftTexture);
    aircraft.setPosition((SCREEN_WIDTH - aircraftTexture.getSize().x) / 2, SCREEN_HEIGHT - aircraftTexture.getSize().y);

    sf::Sprite obstacle(obstacleTexture);

    // Random number generator for obstacle position
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> obstacleDist(0, SCREEN_WIDTH - obstacleTexture.getSize().x);

    int obstacleX = obstacleDist(mt);
    int obstacleY = -obstacleTexture.getSize().y;

    int score = 0;

    // Main game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Move the aircraft
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            aircraft.move(-AIRCRAFT_SPEED, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            aircraft.move(AIRCRAFT_SPEED, 0);
        }

        // Move the obstacle
        obstacle.move(0, OBSTACLE_SPEED);

        // Check for collision
        if (aircraft.getGlobalBounds().intersects(obstacle.getGlobalBounds()))
        {
            // Game over if collision occurs
            std::cout << "Game Over - Your Score: " << score << std::endl;
            window.close();
        }

        // Check if the obstacle passed the bottom of the screen
        if (obstacle.getPosition().y > SCREEN_HEIGHT)
        {
            // Respawn obstacle at a random position above the screen
            obstacleX = obstacleDist(mt);
            obstacleY = -obstacleTexture.getSize().y;

            score++;
        }

        // Clear the screen
        window.clear();

        // Draw sprites
        window.draw(aircraft);
        obstacle.setPosition(obstacleX, obstacleY);
        window.draw(obstacle);

        // Display the score
        sf::Font font;
        if (font.loadFromFile("arial.ttf"))
        {
            sf::Text scoreText("Score: " + std::to_string(score), font, 30);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
        }

        // Display everything
        window.display();
    }

    return 0;
}
