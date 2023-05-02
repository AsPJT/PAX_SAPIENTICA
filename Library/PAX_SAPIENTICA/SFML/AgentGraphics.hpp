/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SFML_AGENT_GRAPHICS_HPP
#define PAX_SAPIENTICA_SFML_AGENT_GRAPHICS_HPP

/*##########################################################################################

##########################################################################################*/

#include <vector>

#include <SFML/Graphics.hpp>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>

namespace paxs
{
    constexpr int width = 800;
    constexpr int height = 600;

    class AgentGraphics
    {
    public:
        sf::RenderWindow window;

        AgentGraphics(const std::string& window_name) : window(sf::VideoMode(width, height), window_name){}
        void agentsDraw(const std::vector<Agent>& agents)
        {
            window.clear();
            for (auto& agent : agents)
            {
                sf::CircleShape shape(1);
                shape.setFillColor(sf::Color::Red);
                shape.setPosition(agent.getX(), agent.getY());
                window.draw(shape);
            }
            window.display();
        }
    private:
        sf::Vector2f convertCoordinate(double x, double y)
        {
            return sf::Vector2f((float) x, (float) y);
        }
    };
}

#endif // !PAX_SAPIENTICA_SFML_AGENT_GRAPHICS_HPP
