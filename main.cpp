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

int apertouEsq()
{
if(Keyboard::isKeyPressed(Keyboard::Left))
    {
     return 0;
    }
else return 1;
}

int apertouDir()
{
if(Keyboard::isKeyPressed(Keyboard::Right))
    {
     return 0;
    }
else return 1;
}

///Exercicio colisão de borda
int colisaoBorda(int posicaoPlayer)
{
    int colidindo = 0;
    if(posicaoPlayer >= bordaDir)
    {
        colidindo = apertouEsq();
    }
    if(posicaoPlayer <= bordaEsq)
    {
        colidindo = apertouDir();
    }
    return colidindo;
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

///Possivel exercicio de atualizar inimigo (se possivel, remover a parte da struct)
inimigo atualizaInimigo (inimigo p)
{
    if(p.xpos < bordaEsq)
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

    // Mudar a cor do inimigo
    sf::Color salmon(255, 128, 128);
    sf::Color green(66, 244, 125);

    RenderWindow app(VideoMode(windowWidth, windowHeight), "Plataforma");

    RectangleShape playerRect(Vector2f(50.0f,50.0f));
    playerRect.setPosition(player.xpos,player.ypos);

    RectangleShape enemyRect(Vector2f(50.0f,50.0f));
    enemyRect.setPosition(enemy.xpos,enemy.ypos);

    // Criando os sprites
    Texture background;
    Texture playerTexture;
    Texture enemyTexture;

    background.loadFromFile("data/images/background.jpg");
    playerTexture.loadFromFile("data/images/Tux.png");
    enemyTexture.loadFromFile("data/images/Tux.png");

    Sprite backgroundSprite (background);

    playerRect.setTexture(&playerTexture);
    enemyRect.setTexture(&enemyTexture);

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
        player = playerUpdate(player,playerUp,playerLeft,playerRight,deltaTime);
        enemy = atualizaInimigo(enemy);

        // Atualizando a animação do jogador e do inimigo
        playerRect.setTextureRect(playerAnimation.uvRect);
        playerAnimation.update(player.row,deltaTime,player.faceRight);
        playerRect.setPosition(player.xpos,player.ypos);


        enemyRect.setTextureRect(enemyAnimation.uvRect);
        enemyAnimation.update(1,deltaTime,enemy.apontaDireita);
        enemyRect.move(enemy.xvel,enemy.yvel);

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
        ///exercicio colisão entre inimigo e player
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
        }

        // Só desenha o inimigo se não tiver sido tocado por cima
        if (!colidindoPorCima)
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

