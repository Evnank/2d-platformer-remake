#include <SFML/Graphics.hpp>
class GAME{
private:
	
public:
	void RUN();
	
};
void GAME::RUN(){
		sf::RenderWindow window( sf::VideoMode( { 200, 200 } ), "SFML works!" );
		sf::CircleShape shape( 100.f );
		shape.setFillColor( sf::Color::Green );

		while ( window.isOpen() )
		{
			while ( const std::optional event = window.pollEvent() )
			{
				if ( event->is<sf::Event::Closed>() )
					window.close();
			}

			window.clear();
			window.draw( shape );
			window.display();
		}
	}



int main()
{
	GAME game;
	game.RUN();
}
