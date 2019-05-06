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

typedef struct
{
	int fileiraAnimacao = 0;
	int vivo = 1;
	bool onGround = true;
	bool canJump = false;
	int apontaDireita = 0;
	float jumpHeight = 0;
	float xvel = -3;
	float yvel = 0;
	float xpos = 930;
	float ypos = 401;
}
inimigo;

typedef struct
{
	bool visivel = true;
	char letra = 'a';
	float x = 150;
	float y = 350;
}
letraStruct;

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
	int direcao;
	float movimento;
	direcao = getSentido(p);
	movimento = (aceleracao * direcao);
    return movimento;
}

/// Aula 1 exercício 2:
float calculaVelocidadePulo(float v0, float impulso, float gravidade, float delta_t)
{
	return v0 + (impulso + gravidade) * delta_t;
}

float calculaPosicaoPulo(float y0, float v, float delta_t)
{
    return y0 + v * delta_t;
}

/// Aula 3 exercício 1:
int comparar(char* chave, char* teste)
{
	int i;
	for (i = 0; chave[i] != '\0' && teste[i] != '\0'; i++)
		if (chave[i] != teste[i])
			return 0;
	if (chave[i] == '\0' && teste[i] == '\0')
		return 1;
	else
		return 0;
}

/// Aula 3 exercício 2:
void atualizaInventario(char palavra[], char lista[], int quant[])
{
	lista[0] = '\0'; // essa linha deve ser informada aos calouros
	int i;
	for (i = 0; palavra[i] != '\0'; i++)
	{
		int j = 0;
		while (lista[j] != '\0' && lista[j] != palavra[i])
			j++;
		if (lista[j] == '\0')
		{
			lista[j + 1] = '\0';
			lista[j] = palavra[i];
			quant[j] = 0;
		}
		quant[j]++;
	}
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

    //float v0 = p.yvel;
	if((playerUp) && (p.canJump))
	{
		p.onGround = false;
		p.canJump = false;
		//p.yvel = calculaPulo(0.1,2,p.jumpHeight);
		impulso = -20000;
		//p.yvel += impulso * deltaTime;
		p.row = 2;
	}

	if((p.onGround) && !(playerRight || playerLeft))
	{
		p.row = 0;
	}

	//if(p.onGround)
	//{
		//p.yvel = 0;
	//}
	//else
		//p.yvel += 10.0f * deltaTime;
		//p.yvel += gravidade * deltaTime;

	p.xvel = movimentoLateral(p.xvel,p);
	p.xpos += p.xvel;
	p.yvel = calculaVelocidadePulo(p.yvel, impulso, gravidade, deltaTime);
	p.ypos = calculaPosicaoPulo(p.ypos, p.yvel, deltaTime);
	//p.ypos += p.yvel;

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
///exercicio 2 aula 4
inimigo atualizaInimigo (inimigo p,bool enemyLeft,bool enemyRight,float deltaTime)
{
	if(p.apontaDireita || !p.apontaDireita)
		p.fileiraAnimacao = 1;

	if(p.xpos < 0)
	{
		p.xvel *= -1;
		p.apontaDireita = 1;
	}

	if(p.xpos > 1000)
	{
		p.xvel *= -1;
		p.apontaDireita = 0;
	}

	p.xpos = p.xpos + p.xvel;
	return p;
}
///

void mataPlayer(Player &p)
{
    p.vivo = 0;
}

void mataInimigo (inimigo &e)
{
    e.vivo = 0;
}

void quica (Player &p)
{
    p.yvel -= 500;
}

int main()
{
	// Criando a câmera
	sf::View camera(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(512.0f,512.0f));

	// Criando as estruturas do player e do inimigo
	Player player;
	bool playerUp,playerLeft,playerRight = false;
	bool limparInventario = false;
	inimigo enemy;
	bool enemyLeft = true;
	bool enemyRight = false;

	// Inicializando srand
	srand(time(NULL));

	// Criando o conteúdo da aula 2
	int numeroDeLetras = 20;
	letraStruct letraDados[numeroDeLetras];
	char* palavraChave = (char*) PALAVRACHAVE; // adiciona automaticamente '\0'
	vector<char> listaDeLetrasAux;
	char listaDeLetras[numeroDeLetras];
	int tamanhoPalavraChave = strlen(palavraChave);
	char palavra[tamanhoPalavraChave];
	char inventarioLetras[numeroDeLetras];
	int inventarioLetrasQuant[numeroDeLetras];
	palavra[0] = '\0';
	inventarioLetras[0] = '\0';
	for (int i = 0; i < tamanhoPalavraChave; i++)
		listaDeLetrasAux.push_back(palavraChave[i]);
	for (int i = tamanhoPalavraChave; i < numeroDeLetras; i++)
		listaDeLetrasAux.push_back(palavraChave[rand() % tamanhoPalavraChave]);

	// Embaralhando a lista
	for (int i = 0; i < numeroDeLetras; i++)
	{
		int r = rand() % (numeroDeLetras - i);
		listaDeLetras[i] = listaDeLetrasAux[r];
		listaDeLetrasAux.erase(listaDeLetrasAux.begin() + r);
	}

	for (int i = 0; i < numeroDeLetras; i++)
	{
		letraDados[i].letra = listaDeLetras[i];
		letraDados[i].x = 50 + i * 80;
	}

	// Mudar a cor do inimigo
	sf::Color salmon(255, 128, 128);
	sf::Color green(66, 244, 125);

	RenderWindow app(VideoMode(windowWidth, windowHeight), "Plataforma");

	RectangleShape playerRect(Vector2f(50.0f,50.0f));
	playerRect.setPosition(player.xpos,player.ypos);

	RectangleShape enemyRect(Vector2f(50.0f,50.0f));
	enemyRect.setPosition(enemy.xpos,enemy.ypos);

	vector<RectangleShape> letra;
	for (int i = 0; i < numeroDeLetras; i++)
	{
		letra.push_back(RectangleShape(Vector2f(20.0f, 20.0f)));
		letra[i].setPosition(letraDados[i].x, letraDados[i].y);
	}

	vector<RectangleShape> letraPalavra;
	vector<RectangleShape> letraResposta;
	vector<RectangleShape> letraInventario;
	for (int i = 0; i < tamanhoPalavraChave; i++)
	{
		letraPalavra.push_back(RectangleShape(Vector2f(20.0f, 20.0f)));
		letraResposta.push_back(RectangleShape(Vector2f(20.0f, 20.0f)));
		letraInventario.push_back(RectangleShape(Vector2f(20.0f, 20.0f)));
	}

	// Criando os sprites
	Texture background;
	Texture playerTexture;
	Texture enemyTexture;
	Texture letraTexture;
	Texture letraSemCorTexture;

	background.loadFromFile("data/images/background.jpg");
	playerTexture.loadFromFile("data/images/Tux.png");
	enemyTexture.loadFromFile("data/images/Tux.png");
	letraTexture.loadFromFile("data/images/alfabeto.jpg");
	letraSemCorTexture.loadFromFile("data/images/alfabeto_preto_e_branco.jpg");

	Sprite backgroundSprite (background);

	playerRect.setTexture(&playerTexture);
	enemyRect.setTexture(&enemyTexture);
	for (int i = 0; i < numeroDeLetras; i++)
		letra[i].setTexture(&letraTexture);
	for (int i = 0; i < tamanhoPalavraChave; i++)
	{
		letraPalavra[i].setTexture(&letraTexture);
		letraResposta[i].setTexture(&letraSemCorTexture);
		letraInventario[i].setTexture(&letraTexture);
		letraInventario[i].setFillColor(Color::Cyan);
	}

	// Cor do inimigo
	enemyRect.setFillColor(salmon);

	// Define as animações
	Animacao playerAnimation(&playerTexture,Vector2u(3,9),0.3f);
	Animacao enemyAnimation(&enemyTexture,Vector2u(3,9),0.3f);
	float deltaTime = 0.0f;
	Clock relogio;

	// Define o retângulo da textura da letra
	for (int i = 0; i < numeroDeLetras; i++)
		letra[i].setTextureRect(IntRect((letraDados[i].letra - 'a') % 6 * 96, (letraDados[i].letra - 'a') / 6 * 96, 96, 96));
	for (int i = 0; i < tamanhoPalavraChave; i++)
		letraResposta[i].setTextureRect(IntRect((palavraChave[i] - 'a') % 6 * 96, (palavraChave[i] - 'a') / 6 * 96, 96, 96));

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
		limparInventario = Keyboard::isKeyPressed(Keyboard::Q);

		// Atualizando o jogador e o inimigo
		player = playerUpdate(player,playerUp,playerLeft,playerRight,deltaTime);
		enemy = atualizaInimigo(enemy, enemyLeft, enemyRight, deltaTime);

		// Atualizando as letras
		if (limparInventario)
		{
			palavra[0] = '\0';
			for (int i = 0; i < numeroDeLetras; i++)
				letraDados[i].visivel = true;
			atualizaInventario(palavra, inventarioLetras, inventarioLetrasQuant);
		}

		// Atualizando a animação do jogador e do inimigo
		playerRect.setTextureRect(playerAnimation.uvRect);
		playerAnimation.update(player.row,deltaTime,player.faceRight);
		//playerRect.move(player.xvel,player.yvel);
		playerRect.setPosition(player.xpos,player.ypos);


		enemyRect.setTextureRect(enemyAnimation.uvRect);
		enemyAnimation.update(enemy.fileiraAnimacao,deltaTime,enemy.apontaDireita);
		enemyRect.move(enemy.xvel,enemy.yvel);

		bool colidindoPorCima;
		bool colidindoDeFrente;

	    // Variáveis para a verificação de colisão:
	    float posXPlayer = playerRect.getPosition().x;
	    float posYPlayer = playerRect.getPosition().y;
	    float larguraPlayer = playerRect.getGlobalBounds().width + posXPlayer;
	    float alturaPlayer = playerRect.getGlobalBounds().height + posYPlayer;
	    float velVertical = player.yvel;
	    int pulando = player.onGround;

	    float posXInimigo = enemyRect.getPosition().x;
	    float posYInimigo = enemyRect.getPosition().y;
	    float larguraInimigo = enemyRect.getGlobalBounds().width + posXInimigo;
	    float alturaInimigo = enemyRect.getGlobalBounds().height + posYInimigo;

	    float alturaMinima = enemyRect.getGlobalBounds().height;


		// Só checa colisão se ambos inimigo e jogador ainda estiverem vivos
		if(enemy.vivo == 1 && player.vivo == 1)
		{
	            if (posXPlayer < larguraInimigo && larguraPlayer > posXInimigo && posYPlayer < alturaInimigo && alturaPlayer > posYInimigo)
	            {
	                if (pulando == 0 && posYPlayer >= alturaMinima && velVertical >= 0)
	                {
	                    colidindoPorCima = 1;
	                    mataInimigo(enemy);
	                    quica(player);
	                }
	                else
	                {
	                    colidindoDeFrente = 1;
	                    mataPlayer(player);
	                }
	            }
		}

		// Colisão entre jogador e letra (se letra estiver visível)
		for (int i = 0; i < numeroDeLetras; i++)
		{
			if (letraDados[i].visivel && player.vivo == 1)
			{
				sf::FloatRect forma1 = playerRect.getGlobalBounds();
				sf::FloatRect forma2 = letra[i].getGlobalBounds();

				if (playerRect.getPosition().x < letra[i].getPosition().x + forma2.width &&
						playerRect.getPosition().x + forma1.width > letra[i].getPosition().x &&
						playerRect.getPosition().y < letra[i].getPosition().y + forma2.height &&
						forma1.height + playerRect.getPosition().y > letra[i].getPosition().y)
				{
					int n = strlen(palavra);
					if (n < tamanhoPalavraChave)
					{
						letraDados[i].visivel = false;
						palavra[n] = letraDados[i].letra;
						palavra[n + 1] = '\0';
						letraPalavra[n].setTextureRect(IntRect((letraDados[i].letra - 'a') % 6 * 96, (letraDados[i].letra - 'a') / 6 * 96, 96, 96));
						atualizaInventario(palavra, inventarioLetras, inventarioLetrasQuant);
					}
				}
			}
		}

		// Clear screen
		app.clear();

		// Ajusta a câmera
		app.setView(camera);
		//camera.setCenter(1000, 300); // Caso o jogador esteja morto, a câmera vai pro meio do mapa

		// Draw the sprite
		app.draw(backgroundSprite);

		// Só desenha o player se não tiver sido tocado de frente
		if (!colidindoDeFrente)
		{
			app.draw(playerRect);
			if(player.xpos >= (windowWidth*0.35) && player.xpos <= (background.getSize().x - windowWidth * 0.35) )
	            camera.setCenter(player.xpos,300);
//            else if(player.xpos < windowWidth*0.35)
//                camera.setCenter(windowWidth*0.35, 300);
//            else
//                camera.setCenter(background.getSize().x - windowWidth*0.5, 300);
		}

		// Só desenha o inimigo se não tiver sido tocado por cima
		if (!colidindoPorCima)
			app.draw(enemyRect);

		// Só desenha a letra se não tiver sido pega ainda
		for (int i = 0; i < numeroDeLetras; i++)
			if (letraDados[i].visivel)
				app.draw(letra[i]);

		// Desenha as letras da resposta
		for (int i = 0; i < tamanhoPalavraChave; i++)
		{
			int padx = 20 + camera.getCenter().x - camera.getSize().x / 2;
			int pady = 20 + camera.getCenter().y - camera.getSize().y / 2;
			letraResposta[i].setPosition(i * 30 + padx, pady);
			app.draw(letraResposta[i]);
		}

		// Desenha as letras que o jogador pegou
		int n = strlen(palavra);
		bool acertou = comparar(palavraChave, palavra);
		for (int i = 0; i < n; i++)
		{
			int padx = 20 + camera.getCenter().x - camera.getSize().x / 2;
			int pady = 50 + camera.getCenter().y - camera.getSize().y / 2;
			letraPalavra[i].setPosition(i * 30 + padx, pady);
			if (acertou){
				letraPalavra[i].setFillColor(green);
				text.setFillColor(Color::Green);
			    text.setCharacterSize(72);
			    text.setString("Você conseguiu!");
			    int x = camera.getCenter().x - text.getLocalBounds().width  / 2;
			    int y = camera.getCenter().y - text.getLocalBounds().height / 2;
			    text.setPosition(x, y - 72);
			    app.draw(text);
			    text.setCharacterSize(36);
			    text.setString("Pressione ALT+F4 para sair");
			    x = camera.getCenter().x - text.getLocalBounds().width  / 2;
			    y = camera.getCenter().y - text.getLocalBounds().height / 2;
			    text.setPosition(x, y);
			    app.draw(text);
		}
			else
				letraPalavra[i].setFillColor(salmon);
			app.draw(letraPalavra[i]);
		}

		// Atualiza o inventário
		n = strlen(inventarioLetras);
		for (int i = 0; i < n; i++)
		{
			letraInventario[i].setTexture(&letraSemCorTexture);
			letraInventario[i].setTextureRect(IntRect((inventarioLetras[i] - 'a') % 6 * 96, (inventarioLetras[i] - 'a') / 6 * 96, 96, 96));
		}

		// Desenha as letras que o jogador pegou em ordem e a quantidade delas
		for (int i = 0; i < n; i++)
		{
			int padx = 20 + camera.getCenter().x - camera.getSize().x / 2;
			int pady = 80 + camera.getCenter().y - camera.getSize().y / 2;
			letraInventario[i].setPosition(i * 30 + padx, pady);
			app.draw(letraInventario[i]);
			text.setFillColor(Color::Black);
			text.setCharacterSize(18);
			text.setString(to_string(inventarioLetrasQuant[i]));
			text.setPosition(i * 30 + padx + 9, pady + 5);
			app.draw(text);
		}

		// Game over
		if (!player.vivo)
		{
			text.setFillColor(Color::Red);
			text.setCharacterSize(72);
			text.setString("Fim do jogo!");
			int x = camera.getCenter().x - text.getLocalBounds().width  / 2;
			int y = camera.getCenter().y - text.getLocalBounds().height / 2;
			text.setPosition(x, y - 72);
			app.draw(text);
			text.setCharacterSize(36);
			text.setString("Pressione ALT+F4 para sair");
			x = camera.getCenter().x - text.getLocalBounds().width  / 2;
			y = camera.getCenter().y - text.getLocalBounds().height / 2;
			text.setPosition(x, y);
			app.draw(text);
		}

		// Update the window
		app.display();

	}
	return EXIT_SUCCESS;
}

