#include <SFML/Graphics.hpp>

class Animacao
{
	public:

		Animacao(sf::Texture* texture, sf::Vector2u imageCount, float switchtime);

		void update(int row,float deltaTime, bool faceRight);
		sf::IntRect uvRect;

	private:

		sf::Vector2u imageCount;
		sf::Vector2u currentImage;

		float totalTime;
		float switchTime;
};
