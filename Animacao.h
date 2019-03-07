#include <SFML/Graphics.hpp>

typedef struct FrameAnimacao
{
    float x, y, w, h;
};

class Animacao
{
	public:

		Animacao(sf::Texture* texture, sf::Vector2u imageCount, float switchtime);

		void update(int row,float deltaTime, bool faceRight);
		sf::IntRect uvRect;

		void inicializaSprites();
		void atualizaSpriteNaTela(bool faceRight);

	private:

		sf::Vector2u imageCount;
		sf::Vector2u currentImage;

		float totalTime;
		float switchTime;

		FrameAnimacao spritesheet[9][3];
};
