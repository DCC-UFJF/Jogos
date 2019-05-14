#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "Animacao.h"

#define PALAVRACHAVE "helloworld"

// var
int windowWidth = 800;
int windowHeight = 600;
int chao = 401;
int bordaEsq = 0;
int bordaDir = 1940;

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
	float x;
	float y;
	float w;
	float h;
	RectangleShape rectShape;
}
plataforma;

typedef struct
{
	bool visivel = true;
	char letra = 'a';
	float x = 150;
	float y = 350;
}
letraStruct;

int apertouEsq()
{
    if(Keyboard::isKeyPressed(Keyboard::Left))
        return 1;
    else
        return 0;
}

int apertouDir()
{
    if(Keyboard::isKeyPressed(Keyboard::Right))
        return 1;
    else
        return 0;
}

int apertouZ()
{
    if(Keyboard::isKeyPressed(Keyboard::Z))
        return 1;
    else
        return 0;
}

/// Exercicio colis�o de borda
int colisaoBorda(int posicaoPlayer)
{
    int colidindo = 0;
    if(posicaoPlayer >= bordaDir && apertouDir())
    {
        colidindo = 1;
    }
    if(posicaoPlayer <= bordaEsq && apertouEsq())
    {
        colidindo = 1;
    }
    return colidindo;
}

/// Exerc�cio colis�o com objetos parte 1
int colidiu(float player_x1, float player_y1, float player_x2, float player_y2, float plat_x1, float plat_y1, float plat_x2, float plat_y2)
{
	// x1 � o lado esquerdo e x2 o direito. y1 � o lado de cima e y2 o de baixo
//	int x = 0, y = 0;
//	x = (player_x1 >= plat_x1 && player_x1 <= plat_x2) ? 1 : x;
//	x = (player_x2 >= plat_x1 && player_x2 <= plat_x2) ? 1 : x;
//	y = (player_y1 >= plat_y1 && player_y1 <= plat_y2) ? 1 : y;
//	y = (player_y2 >= plat_y1 && player_y2 <= plat_y2) ? 1 : y;
//
//	// Para configurar uma colis�o precisa colidir em x e em y
//	return (x && y);

    float player_dx = player_x2 - player_x1;
    float player_dy = player_y2 - player_y1;
    float plat_dx = plat_x2 - plat_x1;
    float plat_dy = plat_y2 - plat_y1;

    float player_cx = player_x1 + player_dx * 0.5f;
    float player_cy = player_y1 + player_dy * 0.5f;
    float plat_cx = plat_x1 + plat_dx * 0.5f;
    float plat_cy = plat_y1 + plat_dy * 0.5f;

//    cout << "player: (" << player_cx << ", " << player_cy << ", " << player_dx << ", " << player_dy << ")" << endl;
//    cout << "platform: (" << plat_cx << ", " << plat_cy << ", " << plat_dx << ", " << plat_dy << ")" << endl;

    if(fabs(plat_cx - player_cx) <= (player_dx + plat_dx)*0.5f &&
      (fabs(plat_cy - player_cy) <= (player_dy + plat_dy)*0.5f))
        return true;
    else
        return false;
}

/// Exerc�cio colis�o com objetos parte 2
char sentidoColisao(float player_x1, float player_y1, float player_x2, float player_y2, float plat_x1, float plat_y1, float plat_x2, float plat_y2)
{
	// Calcula qual o lado (esquerda, direita, cima, baixo) mais pr�ximo do jogador
	// As letras l, r, u, d, e significam respectivamente left, right, up, down, error
	float l = fabs(player_x2 - plat_x1);
	float r = fabs(player_x1 - plat_x2);
	float u = fabs(player_y2 - plat_y1);
	float d = fabs(player_y1 - plat_y2);
	if (l < r && l < u && l < d)
		return 'l';
	if (r < l && r < u && r < d)
		return 'r';
	if (u < l && u < r && u < d)
		return 'u';
	if (d < l && d < r && d < u)
		return 'd';
	return 'e';
//    if(player_x2 >= plat_x1 && player_x1 < plat_x2)
//        return 'l';
//    else if(player_x1 <= plat_x2 && player_x2 > plat_x1)
//        return 'r';
//    else if(player_y2 >= plat_y1 && player_y1 < plat_y2)
//        return 'u';
//    else if(player_y1 <= plat_y2 && player_y2 > plat_y1)
//        return 'd';
//    else return 'e';
}

int getSentido(Player p)
{
	int sentido;
	bool colidindo;
	if(Keyboard::isKeyPressed(Keyboard::Right))
	{
		sentido = 1;
		colidindo = colisaoBorda(p.xpos);
		if(colidindo == true) // atingiu a borda direita
		{
			if(!p.onGround)
				p.xvel = -2;
			sentido = 0;
		}
	}
	else if(Keyboard::isKeyPressed(Keyboard::Left))
	{
		sentido = -1;
		colidindo = colisaoBorda(p.xpos);
		if(colidindo == true) // atingiu a borda esquerda
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

float movimentoLateral(float aceleracao,Player p)
{
	int direcao;
	float movimento;
	direcao = getSentido(p);
	movimento = (aceleracao * direcao);
    return movimento;
}

float calculaVelocidadePulo(float v0, float impulso, float gravidade, float delta_t)
{
	return v0 + (impulso + gravidade) * delta_t;
}

float calculaPosicaoPulo(float y0, float v, float delta_t)
{
    return y0 + v * delta_t;
}

/// Aula 3 exerc�cio 1:
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

/// Aula 3 exerc�cio 2:
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

Player playerUpdate(Player p,bool playerUp,bool playerLeft,bool playerRight,float deltaTime, vector<plataforma> plat)
{
    float impulso = 0;
    float gravidade = 700;
	p.xvel = 5;
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

	float aux = p.ypos;
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

	// Colis�o com as plataformas
	float w = 50;
	float h = 50;
	for (int i = 0; i < plat.size(); i++)
	{
		if (colidiu(p.xpos, p.ypos, p.xpos + w, p.ypos + h, plat[i].x, plat[i].y, plat[i].w + plat[i].x, plat[i].h + plat[i].y))
		{
			char c = sentidoColisao(p.xpos, p.ypos, p.xpos + w, p.ypos + h, plat[i].x, plat[i].y, plat[i].w + plat[i].x, plat[i].h + plat[i].y);
			if ((c == 'l' && p.xvel > 0) || (c == 'r' && p.xvel < 0))
				p.xpos -= p.xvel;
            if(c == 'u')
            {
                p.ypos = aux;
                p.onGround = true;
                p.canJump = true;
            }
            if(c == 'd')
                p.ypos = aux;
//			else if (!(c == 'd' && p.yvel > 0))
//			{
//				p.ypos = aux;
//				if (c == 'u')
//				{
//					p.onGround = true;
//					p.canJump = true;
//				}
//			}
		}
	}

	if(p.onGround)
	{
		p.yvel = 0;
	}
	return p;
}

inimigo moveInimigo(inimigo i)
{
    float xpos, xvel, apontaDireita;
    xpos = i.xpos;
    xvel = i.xvel;
    apontaDireita = i.apontaDireita;

    /// COLOQUE SEU C�DIGO AQUI
    if(xpos < bordaEsq)
    {
        xvel *= -1;
        apontaDireita = 1;
    }

    if(xpos > 1000)
    {
        xvel *= -1;
        apontaDireita = 0;
    }

    xpos = xpos + xvel;

    i.xpos = xpos;
    i.xvel = xvel;
    i.apontaDireita = apontaDireita;

    return i;
}

inimigo moveInimigo(inimigo i, Player p)
{
    float ix, ivel, apontaDireita, px, pvel;
    ix = i.xpos;
    ivel = i.xvel;
    apontaDireita = i.apontaDireita;
    px = p.xpos;
    pvel = p.xvel;

    /// COLOQUE SEU C�DIGO AQUI
    if((px - ix)*ivel < 0 && fabs(px - ix) > 100)
    {
        apontaDireita = !apontaDireita;
        ivel *= -1;
    }

    ix = ix + ivel;
    i.xpos = ix;
    i.xvel = ivel;
    i.apontaDireita = apontaDireita;

    return i;
}

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
	// Criando a c�mera
	sf::View camera(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(512.0f,512.0f));

	// Criando as estruturas do player e do inimigo
	Player player;
	bool playerUp,playerLeft,playerRight = false;
	bool limparInventario = false;
	inimigo enemy;
	bool enemyLeft = true;
	bool enemyRight = false;
	bool colidindoPorCima = false;
    bool colidindoDeFrente = false;

	// Criando as plataformas
	vector<plataforma> plataformas;
	ifstream file("data/plataformas.txt");
	if (file.is_open())
	{
		// O arquivo txt com os dados das plataformas � escrito da seguinte forma
		// Primeira linha: n�mero inteiro de plataformas que existem
		// Linhas seguintes: quatro n�meros separados por espa�os que indicam respectivamente x, y, w (largura) e h (altura)
		int n;
		file >> n;
		for (int i = 0; i < n; i++)
		{
			plataforma p;
			file >> p.x;
			file >> p.y;
			file >> p.w;
			file >> p.h;
			p.rectShape.setPosition(p.x, p.y);
			p.rectShape.setSize(Vector2f(p.w, p.h));
			plataformas.push_back(p);
		}
	}

	// Inicializando srand
	srand(time(NULL));

	// Criando o conte�do da aula 2
	int numeroDeLetras = 20;
	letraStruct letraDados[numeroDeLetras];
	char* palavraChave = (char*) PALAVRACHAVE; // adiciona automaticamente '\0'
	vector<char> listaDeLetrasAux;
	char listaDeLetras[numeroDeLetras];
	int tamanhoPalavraChave = strlen(palavraChave);
	char palavra[tamanhoPalavraChave+1];
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
	Texture platformTexture;
	Texture letraTexture;
	Texture letraSemCorTexture;

	background.loadFromFile("data/images/background.jpg");
	playerTexture.loadFromFile("data/images/Tux.png");
	enemyTexture.loadFromFile("data/images/Tux.png");
	platformTexture.loadFromFile("data/images/gelo.png");
	letraTexture.loadFromFile("data/images/alfabeto.jpg");
	letraSemCorTexture.loadFromFile("data/images/alfabeto_preto_e_branco.jpg");

	Sprite backgroundSprite (background);

	playerRect.setTexture(&playerTexture);
	enemyRect.setTexture(&enemyTexture);
	for (int i = 0; i < plataformas.size(); i++)
		plataformas[i].rectShape.setTexture(&platformTexture);
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

	// Define as anima��es
	Animacao playerAnimation(&playerTexture,Vector2u(3,9),0.3f);
	Animacao enemyAnimation(&enemyTexture,Vector2u(3,9),0.3f);
	float deltaTime = 0.0f;
	Clock relogio;

	// Define o ret�ngulo da textura da letra
	for (int i = 0; i < numeroDeLetras; i++)
		letra[i].setTextureRect(IntRect((letraDados[i].letra - 'a') % 6 * 96, (letraDados[i].letra - 'a') / 6 * 96, 96, 96));
	for (int i = 0; i < tamanhoPalavraChave; i++)
		letraResposta[i].setTextureRect(IntRect((palavraChave[i] - 'a') % 6 * 96, (palavraChave[i] - 'a') / 6 * 96, 96, 96));

	// Par�metros padr�o de texto
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
		player = playerUpdate(player,playerUp,playerLeft,playerRight,deltaTime, plataformas);
		enemy = moveInimigo(enemy, player);

		// Atualizando as letras
		if (limparInventario)
		{
			palavra[0] = '\0';
			for (int i = 0; i < numeroDeLetras; i++)
				letraDados[i].visivel = true;
			atualizaInventario(palavra, inventarioLetras, inventarioLetrasQuant);
		}

		// Atualizando a anima��o do jogador e do inimigo
		playerRect.setTextureRect(playerAnimation.uvRect);
		playerAnimation.update(player.row,deltaTime,player.faceRight);
		playerRect.setPosition(player.xpos,player.ypos);


		enemyRect.setTextureRect(enemyAnimation.uvRect);
		enemyAnimation.update(enemy.fileiraAnimacao,deltaTime,enemy.apontaDireita);
		enemyRect.move(enemy.xvel,enemy.yvel);

	    // Vari�veis para a verifica��o de colis�o:
        float playerX1 = playerRect.getPosition().x;
        float playerY1 = playerRect.getPosition().y;
        float playerX2 = playerRect.getGlobalBounds().width + playerX1;
        float playerY2 = playerRect.getGlobalBounds().height + playerY1;
        float velVertical = player.yvel;
        int pulando = player.onGround;

        float inimigoX1 = enemyRect.getPosition().x;
        float inimigoY1 = enemyRect.getPosition().y;
        float inimigoX2 = enemyRect.getGlobalBounds().width + inimigoX1;
        float inimigoY2 = enemyRect.getGlobalBounds().height + inimigoY1;

        float alturaMinima = enemyRect.getGlobalBounds().height;


		// S� checa colis�o se ambos inimigo e jogador ainda estiverem vivos
		if(enemy.vivo == 1 && player.vivo == 1)
        {
            if(colidiu(playerX1, playerY1, playerX2, playerY2, inimigoX1, inimigoY1, inimigoX2, inimigoY2))
            {
                if (pulando == 0 && playerY1 >= alturaMinima && velVertical >= 0)
                {
                    //colidindoPorCima = 1;
                    mataInimigo(enemy);
                    quica(player);
                }
                else
                {
                    //colidindoDeFrente = 1;
                    mataPlayer(player);
                }
            }
        }

		// Colis�o entre jogador e letra (se letra estiver vis�vel)
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

		// Ajusta a c�mera
		app.setView(camera);
		//camera.setCenter(1000, 300); // Caso o jogador esteja morto, a c�mera vai pro meio do mapa

		// Draw the sprite
		app.draw(backgroundSprite);

		// Desenha as plataformas
		for (int i = 0; i < plataformas.size(); i++)
			app.draw(plataformas[i].rectShape);

		// S� desenha o player se n�o tiver sido tocado de frente
		if (player.vivo)
        {
            app.draw(playerRect);
            if(player.xpos >= (windowWidth*0.35) && player.xpos <= (background.getSize().x - windowWidth * 0.35) )
                camera.setCenter(player.xpos,300);
        }

        // S� desenha o inimigo se n�o tiver sido tocado por cima
        if (enemy.vivo)
            app.draw(enemyRect);

		// S� desenha a letra se n�o tiver sido pega ainda
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
			    text.setString("Voc� conseguiu!");
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

		// Atualiza o invent�rio
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

