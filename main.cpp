#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include "Animacao.h"

#define PALAVRACHAVE "helloworld"

// var
int windowWidth = 800;
int windowHeight = 600;
int chao = 401;

using namespace sf;
using namespace std;

typedef struct
{
	unsigned int row = 0;
	int vivo = 1;
	bool onGround = true;
	bool canJump = true;
	bool faceRight = true;
	float jumpHeight = 300;
	float xvel = 0;
	float yvel = 0;
	float xpos = 300;
	float ypos = chao;
}
Player;

int getSentido(Player p)
{
	int sentido;
	if(Keyboard::isKeyPressed(Keyboard::Right))
	{
		sentido = 1;
		if(p.xpos >= 1960) // atingiu a borda direita
		{
			if(!p.onGround)
				p.xvel = -2;
			sentido = 0;
		}
	}
	else if(Keyboard::isKeyPressed(Keyboard::Left))
	{
		sentido = -1;
		if(p.xpos <= 0) // atingiu a borda esquerda
		{
			if(!p.onGround)
				p.xvel = 2;
			sentido = 0;
		}
	}
	else
		sentido = 0;
	return sentido;
}

/// Aula 1 exercício 1:
float movimentoLateral(float aceleracao,Player p)
{
    ///Implemente aqui o codigo
    return 0;
}



/// Aula 1 exercício 2:
float calculaVelocidadePulo(float v0, float impulso, float gravidade, float delta_t)
{
    ///Implemente aqui o codigo
	return 0;
}

float calculaPosicaoPulo(float y0, float v, float delta_t)
{
    ///Implemente aqui o codigo
    return 400;
}

Player playerUpdate(Player p,bool playerUp,bool playerLeft,bool playerRight,float deltaTime)
{
    float impulso = 0;
    float gravidade = 700;
	p.xvel = 2;
	if(playerRight)
	{
		p.faceRight = true;
		p.row = 1;
	}

	if(playerLeft)
	{
		p.faceRight = false;
		p.row = 1;
	}

	if((playerUp) && (p.canJump))
	{
		p.onGround = false;
		p.canJump = false;
		impulso = -20000;
		p.row = 2;
	}

	if((p.onGround) && !(playerRight || playerLeft))
	{
		p.row = 0;
	}

	p.xvel = movimentoLateral(p.xvel,p);
	p.xpos += p.xvel;
	p.yvel = calculaVelocidadePulo(p.yvel, impulso, gravidade, deltaTime);
	p.ypos = calculaPosicaoPulo(p.ypos, p.yvel, deltaTime);

	 if(p.ypos >= 400)
	{
		p.onGround = true;
		p.canJump = true;
		p.ypos = 400;
	}
	else
		p.onGround = false;

	if(p.onGround)
	{
		p.yvel = 0;
	}
	return p;
}

int main()
{
	// Criando a câmera
	sf::View camera(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(512.0f,512.0f));

	// Criando as estruturas do player e do inimigo
	Player player;
	bool playerUp,playerLeft,playerRight = false;

	RenderWindow app(VideoMode(windowWidth, windowHeight), "Plataforma");

	RectangleShape playerRect(Vector2f(50.0f,50.0f));
	playerRect.setPosition(player.xpos,player.ypos);

	// Criando os sprites
	Texture background;
	Texture playerTexture;

	background.loadFromFile("data/images/background.jpg");
	playerTexture.loadFromFile("data/images/Tux.png");

	Sprite backgroundSprite (background);

	playerRect.setTexture(&playerTexture);

	// Define as animações
	Animacao playerAnimation(&playerTexture,Vector2u(3,9),0.3f);
	float deltaTime = 0.0f;
	Clock relogio;

	// Parâmetros padrão de texto
	Font font;
	font.loadFromFile("Caveat-Bold.ttf");
	Text text;
	text.setFont(font);

	// Inicia o loop do jogo
	while (app.isOpen())
	{
		// Process events
		app.setFramerateLimit(60);
		deltaTime = relogio.restart().asSeconds();

		sf::Event event;
		while (app.pollEvent(event))
		{
			// Close window: exit
			if (event.type == Event::Closed)
			{
				cout << "Closing game" << endl;
				app.close();
			}
		}

		playerRight = (Keyboard::isKeyPressed(Keyboard::Right));
		playerLeft = (Keyboard::isKeyPressed(Keyboard::Left));
		playerUp = (Keyboard::isKeyPressed(Keyboard::Up));

		// Atualizando o jogador e o inimigo
		player = playerUpdate(player,playerUp,playerLeft,playerRight,deltaTime);


		// Atualizando a animação do jogador e do inimigo
		playerRect.setTextureRect(playerAnimation.uvRect);
		playerAnimation.update(player.row,deltaTime,player.faceRight);
		playerRect.setPosition(player.xpos,player.ypos);


		// Clear screen
		app.clear();

		// Ajusta a câmera
		app.setView(camera);

		// Draw the sprite
		app.draw(backgroundSprite);

        app.draw(playerRect);
        if(player.xpos >= (windowWidth*0.35) && player.xpos <= (background.getSize().x - windowWidth * 0.35) )
            camera.setCenter(player.xpos,300);

		// Update the window
		app.display();

	}
	return EXIT_SUCCESS;
}

