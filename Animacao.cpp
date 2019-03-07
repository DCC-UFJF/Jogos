#include <iostream>
#include "Animacao.h"

Animacao::Animacao(sf::Texture* texture, sf::Vector2u imageCount, float switchtime)
{
	this->imageCount = imageCount;
	this->switchTime = switchtime;
	totalTime = 0.0f;
	currentImage.x = 0;

	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);

	inicializaSprites();
}

void Animacao::inicializaSprites()
{
    int largura = uvRect.width;
    int altura = uvRect.height;

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            spritesheet[i][j].x = j * largura;
            spritesheet[i][j].y = i * altura;

            spritesheet[i][j].w = largura;
            spritesheet[i][j].h = altura;
        }
    }
}

void Animacao::update(int row, float deltaTime, bool faceRight)
{
	currentImage.y = row;
	totalTime += deltaTime;

	if(totalTime >= switchTime)
	{
	    // já é tempo de mudar
		totalTime -= switchTime;
		currentImage.x++;

		if(currentImage.x >= imageCount.x)
			currentImage.x = 0;
	}

    atualizaSpriteNaTela(faceRight);
}

void Animacao::atualizaSpriteNaTela(bool faceRight)
{
	uvRect.top = spritesheet[currentImage.y][currentImage.x].y;

	if(faceRight)
	{
        uvRect.left = spritesheet[currentImage.y][currentImage.x].x;
        uvRect.width = abs(uvRect.width);
	}
	else
	{
        uvRect.left = (spritesheet[currentImage.y][currentImage.x].x + abs(uvRect.width));
        uvRect.width = -abs(uvRect.width);
	}
}
