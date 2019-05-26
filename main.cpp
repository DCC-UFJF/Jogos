#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "Animacao.h"

#define PALAVRACHAVE "helloworld"
#define N 5

// var
int windowWidth = 800;
int windowHeight = 600;
int chao = 401;
int bordaEsq = 0;
int bordaDir = 1940;

bool resultadoRanking=true;

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
    int pontuacao = 100;
    bool enemyLeft = true;
    bool enemyRight = false;
    bool colidindoPorCima = false;
    bool colidindoDeFrente = false;
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

/*void reiniciarLevel(Player& player, inimigo inimigos[], int pontuacao[], int tam)
{

    player.vivo = 1;
	player.onGround = true;
	player.canJump = true;
    player.faceRight = true;
	player.jumpHeight = 300;
	player.xvel = 0;
	player.yvel = 0;
	player.xpos = 300;
	player.ypos = chao;

	for(int i=0; i<N; i++)
    {
        inimigos[i].vivo = i+1;
        pontuacao[i] = 100*(i+1);
        inimigos[i].xvel *= 1;
        inimigos[i].xpos *= i+1;
        inimigos[i].apontaDireita = 0;
        inimigos[i].jumpHeight = 0;

    }

}*/

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

int colidiu(float player_x1, float player_y1, float player_x2, float player_y2, float plat_x1, float plat_y1, float plat_x2, float plat_y2)
{
    float player_dx = player_x2 - player_x1;
    float player_dy = player_y2 - player_y1;
    float plat_dx = plat_x2 - plat_x1;
    float plat_dy = plat_y2 - plat_y1;

    float player_cx = player_x1 + player_dx * 0.5f;
    float player_cy = player_y1 + player_dy * 0.5f;
    float plat_cx = plat_x1 + plat_dx * 0.5f;
    float plat_cy = plat_y1 + plat_dy * 0.5f;

    if(fabs(plat_cx - player_cx) <= (player_dx + plat_dx)*0.5f &&
            (fabs(plat_cy - player_cy) <= (player_dy + plat_dy)*0.5f))
        return true;
    else
        return false;
}

char sentidoColisao(float player_x1, float player_y1, float player_x2, float player_y2, float plat_x1, float plat_y1, float plat_x2, float plat_y2)
{
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

        if(p.onGround)
        {
            p.yvel = 0;
        }
        return p;
    }
}

inimigo moveInimigo(inimigo i)
{
    float xpos, xvel, apontaDireita;
    xpos = i.xpos;
    xvel = i.xvel;
    apontaDireita = i.apontaDireita;
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
    e.vivo -= 1;
}

void quica (Player &p)
{
    p.yvel -= 500;
}

//Salvar ranking do jogo

fstream ranking;
int pontuacaoJogadores[5];

void defineRankingInicioJogo()
{
    string str;
    int numero, contador=0;
    while(!ranking.eof())
    {
        ranking>>str;

        if(str=="lugar:")
        {
            ranking>>numero;
            pontuacaoJogadores[contador]=numero;
            contador++;
        }
    }
    for(int i = 0; i<5; i++)
    {
        cout<<pontuacaoJogadores[i]<<endl;
    }
}

///aula 3 exercicio 3
void defineRankingFinalJogo(int pontuacaoNova)
{
    int aux;

    for(int i = 0; i<5; i++)
    {
        if(pontuacaoNova > pontuacaoJogadores[i])
        {
            aux = pontuacaoJogadores[i];
            pontuacaoJogadores[i] = pontuacaoNova;
            pontuacaoNova = aux;
        }
    }

}

void novoTextoRanking()
{
    string str;

    str =
        "Primeiro lugar: " + to_string(pontuacaoJogadores[0])
        +"\nSegundo lugar: " +  to_string(pontuacaoJogadores[1])
        +"\nTerceiro lugar: " + to_string(pontuacaoJogadores[2])
        +"\nQuarto lugar: " +  to_string(pontuacaoJogadores[3])
        +"\nQuinto lugar: " + to_string(pontuacaoJogadores[4]);

    ranking.close();
    ranking.open("data/ranking.txt");

    ranking<<str<<endl;

}

int main()
{
    //Criando sistema de ranking;
    ranking.open("data/ranking.txt");

    defineRankingInicioJogo();

    // Criando a câmera
    sf::View camera(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(512.0f,512.0f));

    // Criando as estruturas do player e do inimigo
    Player player;
    bool playerUp,playerLeft,playerRight = false;
    bool limparInventario = false;
    inimigo inimigos[N];
    int pontuacao[N];
    int pontuacaoNova = 0;

    for(int i=0; i<N; i++)
    {
        inimigos[i].vivo = i+1;
        pontuacao[i] = 100*(i+1);
        inimigos[i].xvel *= 1;
        inimigos[i].xpos *= i+1;
    }

    bool enemyLeft = true;
    bool enemyRight = false;
    bool colidindoPorCima = false;
    bool colidindoDeFrente = false;

    // Criando as plataformas
    vector<plataforma> plataformas;
    ifstream file("data/plataformas.txt");
    if (file.is_open())
    {
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

    // Criando o conteúdo da aula 2
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

    vector<RectangleShape> inimigosRect;
    for(int i = 0; i<N; i++)
    {
        RectangleShape inimigoRect(Vector2f(50.0f,50.0f));
        inimigoRect.setPosition(inimigos[i].xpos, inimigos[i].ypos);
        inimigosRect.push_back(inimigoRect);
    }

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

    vector<Texture> inimigosTexture;
    for(int i=0; i<N; i++)
    {
        Texture inimigoTexture;
        inimigoTexture.loadFromFile("data/images/Tux.png");

        inimigosTexture.push_back(inimigoTexture);
    }

    Sprite backgroundSprite (background);

    playerRect.setTexture(&playerTexture);
    for(int i = 0; i < inimigosRect.size(); i++)
        inimigosRect[i].setTexture(&inimigosTexture[i]);

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

    for(int i = 0; i < N; i++)
    {
        Color cor(255*i,225*(N-i), 225*i/2);
        inimigosRect[i].setFillColor(cor);
    }

    // Define as animações
    Animacao playerAnimation(&playerTexture,Vector2u(3,9),0.3f);

    vector<Animacao> inimigosAnimation;
    for(int i = 0; i<N; i++)
    {
        Animacao inimigoAnimacao(&enemyTexture,Vector2u(3,9),0.3f);
        inimigosAnimation.push_back(inimigoAnimacao);
    }

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

    float inimigosX1[N];
    float inimigosY1[N];
    float inimigosX2[N];
    float inimigosY2[N];

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
        for(int i = 0; i<N; i++)
            inimigos[i] = moveInimigo(inimigos[i], player);

        // Atualizando as letras
        if (limparInventario)
        {
            palavra[0] = '\0';
            for (int i = 0; i < numeroDeLetras; i++)
                letraDados[i].visivel = true;
            atualizaInventario(palavra, inventarioLetras, inventarioLetrasQuant);
            limparInventario = false;
        }

        // Atualizando a animação do jogador e do inimigo
        playerRect.setTextureRect(playerAnimation.uvRect);
        playerAnimation.update(player.row,deltaTime,player.faceRight);
        playerRect.setPosition(player.xpos,player.ypos);


        for(int i = 0; i<N; i++)
        {
            inimigosRect[i].setTextureRect(inimigosAnimation[i].uvRect);
            inimigosAnimation[i].update(inimigos[i].fileiraAnimacao,deltaTime,inimigos[i].apontaDireita);
            inimigosRect[i].move(inimigos[i].xvel,inimigos[i].yvel);
        }

        // Variáveis para a verificação de colisão:
        float playerX1 = playerRect.getPosition().x;
        float playerY1 = playerRect.getPosition().y;
        float playerX2 = playerRect.getGlobalBounds().width + playerX1;
        float playerY2 = playerRect.getGlobalBounds().height + playerY1;
        float velVertical = player.yvel;
        int pulando = player.onGround;

        for(int i = 0; i < N; i++)
        {
            inimigosX1[i] = inimigosRect[i].getPosition().x;
            inimigosY1[i] = inimigosRect[i].getPosition().y;
            inimigosX2[i] = inimigosRect[i].getGlobalBounds().width + inimigosX1[i];
            inimigosY2[i] = inimigosRect[i].getGlobalBounds().height + inimigosY1[i];
        }

        float alturaMinima = inimigosRect[0].getGlobalBounds().height;


        // Só checa colisão se ambos inimigo e jogador ainda estiverem vivos
        for(int i = 0; i < N; i++)
        {
            if(inimigos[i].vivo > 0 && player.vivo == 1)
            {
                if(colidiu(playerX1, playerY1, playerX2, playerY2, inimigosX1[i], inimigosY1[i], inimigosX2[i], inimigosY2[i]))
                {
                    if (pulando == 0 && playerY1 >= alturaMinima && velVertical >= 0)
                    {
                        //colidindoPorCima = 1;
                        mataInimigo(inimigos[i]);
                        quica(player);

                        if(inimigos[i].vivo <= 0)
                        {
                            pontuacaoNova+=pontuacao[i];
                            cout<<pontuacaoNova<<endl;
                        }

                    }
                    else
                    {
                        //colidindoDeFrente = 1;
                        mataPlayer(player);
                    }
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

        for(int i=0; i<N; i++)
            if (inimigos[i].vivo)
                app.draw(inimigosRect[i]);

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
            if (acertou)
            {
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
            if(resultadoRanking)
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
                text.setCharacterSize(36);
                text.setString("Pressione S para salvar sua pontuação");
                x = camera.getCenter().x - text.getLocalBounds().width  / 2;
                y = camera.getCenter().y + text.getLocalBounds().height;
                text.setPosition(x, y);
                app.draw(text);
                text.setCharacterSize(30);
                text.setString("Pressione R reiniciar o jogo");
                x = camera.getCenter().x - text.getLocalBounds().width  / 2 ;
                y = camera.getCenter().y + 4*text.getLocalBounds().height;
                text.setPosition(x, y);
                app.draw(text);
                if((Keyboard::isKeyPressed(Keyboard::S)))
                {
                    defineRankingFinalJogo(pontuacaoNova);
                    novoTextoRanking();
                    resultadoRanking = false;
                }
                if((Keyboard::isKeyPressed(Keyboard::R)))
                {
                    //reiniciarLevel(player,inimigos,pontuacao,N);
                    pontuacaoNova = 0;
                    limparInventario = true;

                }
            }
            else
            {
                string str;
                str =
                    "Primeiro lugar: " + to_string(pontuacaoJogadores[0])
                    +"\nSegundo lugar: " +  to_string(pontuacaoJogadores[1])
                    +"\nTerceiro lugar: " + to_string(pontuacaoJogadores[2])
                    +"\nQuarto lugar: " +  to_string(pontuacaoJogadores[3])
                    +"\nQuinto lugar: " + to_string(pontuacaoJogadores[4]);
                text.setFillColor(Color::Red);
                text.setCharacterSize(36);
                text.setString(str);
                int x = camera.getCenter().x - text.getLocalBounds().width  / 2;
                int y = camera.getCenter().y - text.getLocalBounds().height / 2;
                text.setPosition(x, y - 72);
                app.draw(text);
            }


        }

        // Update the window
        app.display();

    }
    return EXIT_SUCCESS;
}

