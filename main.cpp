#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

#define LARGURA_TABULEIRO 10
#define ALTURA_TABULEIRO 20
#define BORDA_TABULEIRO 0
#define TAMANHO_BLOCO 30
#define BORDA_BLOCO 3

using namespace sf;


typedef struct Ponto
{
    int x;
    int y;
};

typedef struct Retangulo
{
    Ponto p1;
    Ponto p2;
};

Retangulo criaRetangulo(Ponto p1, Ponto p2);
Retangulo moveRetangulo(Retangulo r, int dx, int dy);

typedef struct Bloco
{
    Retangulo r;
    int x;
    int y;
    int ativo = 1;
    sf::RectangleShape shape;
};

typedef struct Tetramino
{
    Bloco blocos[4];
    Bloco centro;
    int tipo;
    int dir_rotacao;
    int muda_rotacao;
};

typedef struct Tabuleiro
{
    int largura;
    int altura;
    int borda;
    Bloco grade[LARGURA_TABULEIRO][ALTURA_TABULEIRO];
    //Falta definir canvas,grid,the board e status_msg
};

Bloco criaBloco(Ponto p, sf::Color cor);
int podeMoverBloco(Bloco b, Tabuleiro tab, int dx, int dy);
Bloco moveBloco(Bloco b, int dx, int dy);

void criaTetraI(Ponto coords[4], Ponto centro);
void criaTetraJ(Ponto coords[4], Ponto centro);
void criaTetraL(Ponto coords[4], Ponto centro);
void criaTetraO(Ponto coords[4], Ponto centro);
void criaTetraS(Ponto coords[4], Ponto centro);
void criaTetraT(Ponto coords[4], Ponto centro);
void criaTetraZ(Ponto coords[4], Ponto centro);
Tetramino criaTetramino(Ponto centro, int tipo);
Tetramino moveTetramino(Tetramino t, int dx, int dy);
int podeMoverTetramino(Tetramino t, Tabuleiro tab, int dx, int dy);
int podeRotacionarTetramino(Tetramino t, Tabuleiro tab);
Tetramino rotacionaTetramino(Tetramino t, Tabuleiro tab);

Tabuleiro criaTabuleiro(int largura, int altura, int borda);
int podeMoverPara(Tabuleiro tab, int x, int y);
Tabuleiro adicionaAoTabuleiro(Tabuleiro tab, Tetramino t);
Tabuleiro deletaLinha(Tabuleiro tab, int y);
int linhaCompleta(Tabuleiro tab, int y);
Tabuleiro desceLinhas(Tabuleiro tab, int yInicial);
Tabuleiro removeLinhasCompletas(Tabuleiro tab);

Tabuleiro criaTabuleiro(int largura, int altura, int borda)
{
    int i, j;
    Tabuleiro tab;
    tab.largura = largura;
    tab.altura = altura;
    tab.borda = borda;

    for(int i = 0; i < largura; i++)
    {
        for(int j = 0; j < altura; j++)
        {
            tab.grade[i][j].x = -1;
            tab.grade[i][j].y = -1;
            tab.grade[i][j].ativo = 0;
        }
    }
    return tab;
}

int podeMoverPara(Tabuleiro tab, int x, int y)
{
    if((x < tab.borda) || (x >= tab.largura+tab.borda) || (y >= tab.altura))
        return 0;
    else if(tab.grade[x][y].ativo)
        return 0;
    else
        return 1;
}

Tabuleiro adicionaAoTabuleiro(Tabuleiro tab, Tetramino t)
{
    for(int i = 0; i < 4; i++)
    {
        tab.grade[t.blocos[i].x-tab.borda][t.blocos[i].y] = t.blocos[i];
    }
    return tab;
}

Tabuleiro deletaLinha(Tabuleiro tab, int y)
{
    int x;
    for(x = 0; x < tab.largura; x++)
    {
        tab.grade[x][y].x = -1;
        tab.grade[x][y].y = -1;
        tab.grade[x][y].ativo = 0;
    }

    return tab;
}

int linhaCompleta(Tabuleiro tab, int y)
{
    int x;
    for(x = 0; x < tab.largura; x++)
    {
        if(!tab.grade[x][y].ativo)
            return 0;
    }
    return 1;
}

Tabuleiro desceLinhas(Tabuleiro tab, int yInicial)
{
    int x, y;
    for(y = yInicial; y >= 0; y--)
    {
        for(x = 0; x < tab.largura; x++)
        {
            if(tab.grade[x][y].ativo)
            {
                Bloco b = tab.grade[x][y];
                tab.grade[x][y].ativo = 0;
                b = moveBloco(b, 0, 1);
                tab.grade[x][y+1] = b;
            }
        }
    }
    return tab;
}

Tabuleiro removeLinhasCompletas(Tabuleiro tab)
{
    int y;

    for(y = 0; y < tab.altura; y++)
    {
        if(linhaCompleta(tab, y))
        {
            tab = deletaLinha(tab, y);
            tab = desceLinhas(tab, y-1);
        }
    }

    return tab;
}

Retangulo criaRetangulo(Ponto p1, Ponto p2)
{
    Retangulo r;
    r.p1 = p1;
    r.p2 = p2;
    return r;
}

Retangulo moveRetangulo(Retangulo r, int dx, int dy)
{
    r.p1.x += dx;
    r.p1.y += dy;
    r.p2.x += dx;
    r.p2.y += dy;

    return r;
}

Bloco criaBloco(Ponto p, sf::Color cor)
{
    Bloco b;
    Ponto p1, p2;
    b.x = p.x;
    b.y = p.y;

    p1.x = (p.x*TAMANHO_BLOCO + BORDA_BLOCO);
    p1.y = (p.y*TAMANHO_BLOCO + BORDA_BLOCO);
    p2.x = (p1.x + TAMANHO_BLOCO);
    p2.y = (p1.y + TAMANHO_BLOCO);

//    printf("P1: (%d, %d)\n", p1.x, p1.y);
//    printf("P2: (%d, %d)\n", p2.x, p2.y);

    b.r = criaRetangulo(p1,p2);
    b.shape.setPosition(p.x*TAMANHO_BLOCO, p.y*TAMANHO_BLOCO);
    b.shape.setSize(sf::Vector2f(p2.x - p1.x, p2.y - p1.y));
    b.shape.setFillColor(cor);

    b.ativo = 1;

    return b;
}

int podeMoverBloco(Bloco b, Tabuleiro tab, int dx, int dy)
{
    if(podeMoverPara(tab, b.x + dx, b.y + dy))
        return 1;
    else
        return 0;
}

Bloco moveBloco(Bloco b, int dx, int dy)
{
    b.x += dx;
    b.y += dy;

//    printf("MOVE: (%d, %d)\n", b.x, b.y);

    b.r = moveRetangulo(b.r, dx * TAMANHO_BLOCO, dy * TAMANHO_BLOCO);
    b.shape.move(dx * TAMANHO_BLOCO, dy * TAMANHO_BLOCO);
    return b;
}

void criaTetraI(Ponto coords[4], Ponto centro)
{
    coords[0].x = centro.x-2;
    coords[0].y = centro.y;
    coords[1].x = centro.x-1;
    coords[1].y = centro.y;
    coords[2].x = centro.x;
    coords[2].y = centro.y;
    coords[3].x = centro.x+1;
    coords[3].y = centro.y;
}

void criaTetraJ(Ponto coords[4], Ponto centro)
{
    coords[0].x = centro.x-1;
    coords[0].y = centro.y;
    coords[1].x = centro.x;
    coords[1].y = centro.y;
    coords[2].x = centro.x+1;
    coords[2].y = centro.y;
    coords[3].x = centro.x+1;
    coords[3].y = centro.y+1;
}

void criaTetraL(Ponto coords[4], Ponto centro)
{
    coords[0].x = centro.x-1;
    coords[0].y = centro.y;
    coords[1].x = centro.x;
    coords[1].y = centro.y;
    coords[2].x = centro.x+1;
    coords[2].y = centro.y;
    coords[3].x = centro.x-1;
    coords[3].y = centro.y+1;
}

void criaTetraO(Ponto coords[4], Ponto centro)
{
    coords[0].x = centro.x;
    coords[0].y = centro.y;
    coords[1].x = centro.x-1;
    coords[1].y = centro.y;
    coords[2].x = centro.x;
    coords[2].y = centro.y+1;
    coords[3].x = centro.x-1;
    coords[3].y = centro.y+1;
}

void criaTetraS(Ponto coords[4], Ponto centro)
{
    coords[0].x = centro.x;
    coords[0].y = centro.y;
    coords[1].x = centro.x;
    coords[1].y = centro.y+1;
    coords[2].x = centro.x+1;
    coords[2].y = centro.y;
    coords[3].x = centro.x-1;
    coords[3].y = centro.y+1;
}

void criaTetraT(Ponto coords[4], Ponto centro)
{
    coords[0].x = centro.x-1;
    coords[0].y = centro.y;
    coords[1].x = centro.x;
    coords[1].y = centro.y;
    coords[2].x = centro.x+1;
    coords[2].y = centro.y;
    coords[3].x = centro.x;
    coords[3].y = centro.y+1;
}

void criaTetraZ(Ponto coords[4], Ponto centro)
{
    coords[0].x = centro.x-1;
    coords[0].y = centro.y;
    coords[1].x = centro.x;
    coords[1].y = centro.y;
    coords[2].x = centro.x;
    coords[2].y = centro.y+1;
    coords[3].x = centro.x+1;
    coords[3].y = centro.y+1;
}

Tetramino criaTetramino(Ponto centro, int tipo)
{
    Tetramino t;
    Ponto coords[4];

    sf::Color cor;

    switch(tipo)
    {
    case 0:
        criaTetraI(coords,centro);
        t.muda_rotacao = 1;
        t.dir_rotacao = 1;
        t.centro.x = coords[2].x;
        t.centro.y = coords[2].y;
        cor = sf::Color::Blue;
        break;
    case 1:
        criaTetraJ(coords,centro);
        t.muda_rotacao = 0;
        t.dir_rotacao = 1;
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
        cor = sf::Color(255, 100, 100);
        break;
    case 2:
        criaTetraL(coords,centro);
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
        t.muda_rotacao = 0;
        t.dir_rotacao = 1;
        cor = sf::Color::Cyan;
        break;
    case 3:
        criaTetraO(coords,centro);
        t.centro.x = coords[0].x;
        t.centro.y = coords[0].y;
        t.dir_rotacao = 0;
        t.muda_rotacao = 0;
        cor = sf::Color::Red;
        break;
    case 4:
        criaTetraS(coords,centro);
        t.muda_rotacao = 1;
        t.dir_rotacao = -1;
        t.centro.x = coords[0].x;
        t.centro.y = coords[0].y;
        cor = sf::Color::Green;
        break;
    case 5:
        criaTetraT(coords,centro);
        t.muda_rotacao = 0;
        t.dir_rotacao = 1;
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
        cor = sf::Color::Yellow;
        break;
    case 6:
        criaTetraZ(coords,centro);
        t.muda_rotacao = 1;
        t.dir_rotacao = -1;
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
        cor = sf::Color::Magenta;
    }

    for(int i=0; i<4; i++)
    {
        t.blocos[i] = criaBloco(coords[i], cor);
    }

    return t;
}

Tetramino moveTetramino(Tetramino t, int dx, int dy)
{
    int i;
    for(i = 0; i < 4; i++)
        t.blocos[i] = moveBloco(t.blocos[i], dx, dy);
    return t;
}

int podeMoverTetramino(Tetramino t, Tabuleiro tab, int dx, int dy)
{
    int i;
    for(i = 0; i < 4; i++)
    {
        if(!podeMoverBloco(t.blocos[i], tab, dx, dy))
            return 0;
    }
    return 1;
}

int podeRotacionarTetramino(Tetramino t, Tabuleiro tab)
{
    int i;
    int x, y;
    Bloco centro = t.blocos[1];

    for(i = 0; i < 4; i++)
    {
        if(i != 1)
        {
            x = centro.x - t.dir_rotacao*centro.y + t.dir_rotacao*t.blocos[i].y;
            y = centro.y + t.dir_rotacao*centro.x - t.dir_rotacao*t.blocos[i].x;

            if(!podeMoverPara(tab, x, y))
                return 0;
        }
    }
    return 1;
}

Tetramino rotacionaTetramino(Tetramino t, Tabuleiro tab)
{
    int i;
    int x, y;
    Bloco centro = t.blocos[1];

    for(i = 0; i < 4; i++)
    {
        if(i != 1)
        {
            x = centro.x - t.dir_rotacao*centro.y + t.dir_rotacao*t.blocos[i].y;
            y = centro.y + t.dir_rotacao*centro.x - t.dir_rotacao*t.blocos[i].x;

            t.blocos[i] = moveBloco(t.blocos[i], x - t.blocos[i].x, y - t.blocos[i].y);
        }
    }

    if(t.muda_rotacao)
        t.dir_rotacao *= -1;

    return t;
}

void gameOver(Tabuleiro b)
{
printf("Game Over");
}

void pause(Tabuleiro b)
{

}

Tetramino criaNovoTetramino(Tabuleiro tab)
{
    int tipo = rand() % 7;

    Ponto centro;
    centro.x = tab.largura / 2;
    centro.y = 0;

    return criaTetramino(centro, tipo);
}

Tetramino anima(Tabuleiro tab, Tetramino t, int dx, int dy)
{
//    if(podeMover(tab, t, dx, dy))
//    {
//        t = moveTetramino(t, dx, dy);
//    }
//    else if(dy == 1)
//    {
//        tab = adicionaAoTabuleiro(tab, t);
//    }
//    return t;
}

void desenhaBloco(sf::RenderWindow& window, Bloco b)
{
    window.draw(b.shape);
}

void desenhaTetramino(sf::RenderWindow& window, Tetramino t)
{
    int i;
    for(int i = 0; i < 4; i++)
        desenhaBloco(window, t.blocos[i]);
}

void desenhaTabuleiro(sf::RenderWindow& window, Tabuleiro tab)
{
    int x, y;
    for(x = 0; x < tab.largura; x++)
    {
        for(y = 0; y < tab.altura; y++)
        {
            if(tab.grade[x][y].ativo)
                desenhaBloco(window, tab.grade[x][y]);
        }
    }
}

int main()
{
    int delayInicial = 1000;
    int nlinhas, dx, dy;

    Tabuleiro tab;
    Tetramino tetraminoAtual, proximoTetramino;

    tab = criaTabuleiro(LARGURA_TABULEIRO, ALTURA_TABULEIRO, BORDA_TABULEIRO);

    srand(time(0));

    tetraminoAtual = criaNovoTetramino(tab);
    proximoTetramino = criaNovoTetramino(tab);

    // Create the main window
    //RenderWindow window(sf::VideoMode(400, 600), "Tetris!");
    RenderWindow window(sf::VideoMode( (LARGURA_TABULEIRO + BORDA_TABULEIRO*2) * TAMANHO_BLOCO,
                                       (ALTURA_TABULEIRO  + BORDA_TABULEIRO*2) * TAMANHO_BLOCO), "Tetris!");
    window.setFramerateLimit(15);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        dx = 0, dy = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            dx = 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            dx = -1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            dy = 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            tetraminoAtual = rotacionaTetramino(tetraminoAtual, tab);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            while(podeMoverTetramino(tetraminoAtual, tab, 0, 1))
                tetraminoAtual = moveTetramino(tetraminoAtual, 0, 1);
        }

//        for(int i = 0; i < 4; i++)
//            printf("(%d, %d)\n", tetraminoAtual.blocos[i].x, tetraminoAtual.blocos[i].y);

        if(podeMoverTetramino(tetraminoAtual, tab, dx, dy))
        {
            tetraminoAtual = moveTetramino(tetraminoAtual, dx, dy);
        }
        else if(dy == 1)
        {
            tab = adicionaAoTabuleiro(tab, tetraminoAtual);
            tetraminoAtual = proximoTetramino;
            proximoTetramino = criaNovoTetramino(tab);
            //if(not drawShape)

            tab = removeLinhasCompletas(tab);
        }

        // Clear screen
        window.clear();

        desenhaTetramino(window, tetraminoAtual);
        desenhaTabuleiro(window, tab);

        window.display();
    }
    return EXIT_SUCCESS;
}
