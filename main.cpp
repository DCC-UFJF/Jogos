#include <SFML/Graphics.hpp>
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

typedef struct Bloco
{
    Retangulo r;
    int x;
    int y;
    //FALTA IMPLEMENTAR CAN_MOVE E MOVE
};

typedef struct Tetramino
{
    Ponto Tipo[4];
    int direcao;
    int rotacao;
    //Falta definir rotações
}

Retangulo criaRetangulo(Ponto p1, Ponto p2)
{
    Retangulo r;
    r.p1 = p1;
    r.p2 = p2;
    return r;
}


Bloco criaBloco(Bloco b, int x, int y)
{
    Ponto p1, p2;
    b.x = x;
    b.y = y;

    p1.x = (x*TAMANHO_BLOCO + BORDA_BLOCO);
    p1.y = (y*TAMANHO_BLOCO + BORDA_BLOCO);
    p2.x = (p1.x + TAMANHO_BLOCO);
    p2.y = (p1.y + TAMANHO_BLOCO);

    b.r = criaRetangulo(p1,p2);
    ///falta implementar cor

    return b;
}

int main()
{

    // Create the main window
    RenderWindow window(sf::VideoMode(400, 600), "Tetris!");


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
        // Clear screen
        window.clear();



        window.display();
    }
    return EXIT_SUCCESS;
}
