#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "Animacao.h"

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

/// EXERCÍCIO 1: Colisão com as bordas da tela
int colisaoBorda(int posicaoPlayer)
{
    /// INCLUA SEU CÓDIGO AQUI (mude o retorno se necessário)
    return 0;
}

/// EXERCÍCIO 2: Colisão com objetos: Parte 1
int colidiu(float player_x1, float player_y1, float player_x2, float player_y2, float plat_x1, float plat_y1, float plat_x2, float plat_y2)
{
    /// INCLUA SEU CÓDIGO AQUI (mude o retorno se necessário)
	return 0;
}

/// EXERCÍCIO 3: Colisão com objetos: Parte 2
char sentidoColisao(float player_x1, float player_y1, float player_x2, float player_y2, float plat_x1, float plat_y1, float plat_x2, float plat_y2)
{
	// Calcula qual o lado (esquerda, direita, cima, baixo) mais próximo do jogador
	// As letras l, r, u, d, e significam respectivamente left, right, up, down, error
	/// INCLUA SEU CÓDIGO AQUI (mude o retorno se necessário)
	return 'e';
}

int getSentido(Player p)
{
    int sentido;
    bool colidindo;
    if(Keyboard::isKeyPressed(Keyboard::Right))
    {
        sentido = 1;
        colidindo = colisaoBorda(p.xpos);
        if (colidindo == true)
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
        if (colidindo == true)
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

	// Colisão com as plataformas
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
		}
	}

    if(p.onGround)
    {
        p.yvel = 0;
    }
    return p;
}

/// EXERCÍCIO 4: Movimentar inimigo em um padrão fixo
inimigo moveInimigo(inimigo i)
{
    float xpos, xvel, apontaDireita;
    xpos = i.xpos;
    xvel = i.xvel;
    apontaDireita = i.apontaDireita;

    /// COLOQUE SEU CÓDIGO ABAIXO (não alterar as linhas já implementadas)



    /// COLOQUE SEU CÓDIGO ACIMA (não alterar as linhas já implementadas)

    i.xpos = xpos;
    i.xvel = xvel;
    i.apontaDireita = apontaDireita;

    return i;
}

/// EXERCÍCIO 5: Fazer o inimigo perseguir o jogador
inimigo moveInimigo(inimigo i, Player p)
{
    float ix, ivel, apontaDireita, px, pvel;
    ix = i.xpos;
    ivel = i.xvel;
    apontaDireita = i.apontaDireita;
    px = p.xpos;
    pvel = p.xvel;

    /// COLOQUE SEU CÓDIGO ABAIXO (não alterar as linhas já implementadas)



    /// COLOQUE SEU CÓDIGO ACIMA (não alterar as linhas já implementadas)

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
    // Criando a câmera
    sf::View camera(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(512.0f,512.0f));

    // Criando as estruturas do player e do inimigo
    Player player;
    bool playerUp,playerLeft,playerRight = false;
    inimigo enemy;
    bool colidindoPorCima = false;
    bool colidindoDeFrente = false;

	// Criando as plataformas
	vector<plataforma> plataformas;
	ifstream file("data/plataformas.txt");
	if (file.is_open())
	{
		// O arquivo txt com os dados das plataformas é escrito da seguinte forma
		// Primeira linha: número inteiro de plataformas que existem
		// Linhas seguintes: quatro números separados por espaços que indicam respectivamente x, y, w (largura) e h (altura)
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

    // Mudar a cor do inimigo
    sf::Color salmon(255, 128, 128);
    sf::Color green(66, 244, 125);

    RenderWindow app(VideoMode(windowWidth, windowHeight), "Plataforma");

    RectangleShape playerRect(Vector2f(50.0f,50.0f));
    playerRect.setPosition(player.xpos, player.ypos);

    RectangleShape enemyRect(Vector2f(50.0f,50.0f));
    enemyRect.setPosition(enemy.xpos, enemy.ypos);

    // Criando os sprites
    Texture background;
    Texture playerTexture;
    Texture enemyTexture;
	Texture platformTexture;

    background.loadFromFile("data/images/background.jpg");
    playerTexture.loadFromFile("data/images/Tux.png");
    enemyTexture.loadFromFile("data/images/Tux.png");
	platformTexture.loadFromFile("data/images/gelo.png");

    Sprite backgroundSprite (background);

    playerRect.setTexture(&playerTexture);
    enemyRect.setTexture(&enemyTexture);
	for (int i = 0; i < plataformas.size(); i++)
		plataformas[i].rectShape.setTexture(&platformTexture);

    // Cor do inimigo
    enemyRect.setFillColor(salmon);

    // Define as animações
    Animacao playerAnimation(&playerTexture,Vector2u(3,9),0.3f);
    Animacao enemyAnimation(&enemyTexture,Vector2u(3,9),0.3f);
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
        player = playerUpdate(player,playerUp,playerLeft,playerRight,deltaTime, plataformas);
        enemy = moveInimigo(enemy, player);

        // Atualizando a animação do jogador e do inimigo
        playerRect.setTextureRect(playerAnimation.uvRect);
        playerAnimation.update(player.row,deltaTime,player.faceRight);
        playerRect.setPosition(player.xpos,player.ypos);


        enemyRect.setTextureRect(enemyAnimation.uvRect);
        enemyAnimation.update(1,deltaTime,enemy.apontaDireita);
        enemyRect.move(enemy.xvel,enemy.yvel);

        // Variáveis para a verificação de colisão:
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


        // Só checa colisão se ambos inimigo e jogador ainda estiverem vivos
        if(enemy.vivo == 1 && player.vivo == 1)
        {
            /// EXERCÍCIO 6: Tratar colisão entre inimigo e jogador

            /// COLOQUE SEU CÓDIGO ABAIXO




            /// COLOQUE SEU CÓDIGO ACIMA
        }

        // Clear screen
        app.clear();

        // Ajusta a câmera
        app.setView(camera);
        //camera.setCenter(1000, 300); // Caso o jogador esteja morto, a câmera vai pro meio do mapa

        // Draw the sprite
        app.draw(backgroundSprite);

		// Desenha as plataformas
		for (int i = 0; i < plataformas.size(); i++)
			app.draw(plataformas[i].rectShape);

        // Só desenha o player se não tiver sido tocado de frente
        if (player.vivo)
        {
            app.draw(playerRect);
            if(player.xpos >= (windowWidth*0.35) && player.xpos <= (background.getSize().x - windowWidth * 0.35) )
                camera.setCenter(player.xpos,300);
        }

        // Só desenha o inimigo se não tiver sido tocado por cima
        if (enemy.vivo)
            app.draw(enemyRect);

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

