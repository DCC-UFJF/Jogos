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
    Bloco blocos[4];
    Bloco centro;
    int tipo;
    int dir_rotacao;
    int shift_rotacao;
    //Falta definir rotações
};

typedef struct Board
{
    int largura;
    int altura;
    int borda;
    //Falta definir canvas,grid,the board e status_msg
};

Retangulo criaRetangulo(Ponto p1, Ponto p2)
{
    Retangulo r;
    r.p1 = p1;
    r.p2 = p2;
    return r;
}

Bloco criaBloco(Ponto p)
{
    Bloco b;
    Ponto p1, p2;
    b.x = p.x;
    b.y = p.y;

    p1.x = (p.x*TAMANHO_BLOCO + BORDA_BLOCO);
    p1.y = (p.y*TAMANHO_BLOCO + BORDA_BLOCO);
    p2.x = (p1.x + TAMANHO_BLOCO);
    p2.y = (p1.y + TAMANHO_BLOCO);

    b.r = criaRetangulo(p1,p2);
    ///falta implementar cor

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

    switch(tipo)
    {
    case 0:
        criaTetraI(coords,centro);
        t.shift_rotacao = 1;
        t.centro.x = coords[2].x;
        t.centro.y = coords[2].y;
        break;
    case 1:
        criaTetraJ(coords,centro);
        t.shift_rotacao = 0;
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
        break;
    case 2:
        criaTetraL(coords,centro);
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
        t.shift_rotacao = 0;
        break;
    case 3:
        criaTetraO(coords,centro);
        t.centro.x = coords[0].x;
        t.centro.y = coords[0].y;
        //falta definir o override
        break;
    case 4:
        criaTetraS(coords,centro);
        t.shift_rotacao = 1;
        t.dir_rotacao = -1;
        t.centro.x = coords[0].x;
        t.centro.y = coords[0].y;
        break;
    case 5:
        criaTetraT(coords,centro);
        t.shift_rotacao = 0;
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
        break;
    case 6:
        criaTetraZ(coords,centro);
        t.shift_rotacao = 1;
        t.dir_rotacao = -1;
        t.centro.x = coords[1].x;
        t.centro.y = coords[1].y;
    }

    for(int i=0; i<4; i++)
    {
        t.blocos[i] = criaBloco(coords[i]);
    }

    return t;
}

Board criaBoard(int largura, int altura, int borda)
{
    Board b;
    b.largura = largura;
    b.altura = altura;
    b.borda = borda;

//falta definir outras variáveis e desenhar coisas.
    return b;
}

int podeMover(Board b, int x, int y)
{
    if((x < b.borda) || (x >=b.largura+b.borda) || (y >= b.altura))
    {
        return 0;
    }
    //else if(x,y in self.grid) return 0; Implementar essa linha
    else
        return 1;
}

void adicionaForma(Board b, Tetramino t)
{
    Bloco blocos[4];
    for(int i=0; i<4; i++)
    {
        blocos[i] = t.blocos[i];
    }
    //for(int i=0; i<4; i++)
    //{
        //b.grid =(blocos[i].x, blocos[i].y)
    //}

}

void deletaLinha(Board b, int y)
{


}

int verificarLinhaCompleta(Board b, int y)
{


}

void moveLinhaInferior(Board b, int yInicial)
{


}

void removeLinhaCompleta(Board b)
{


}

void gameOver(Board b)
{
printf("Game Over");
}

void pause(Board b)
{

}



int main()
{
    int larguraBoard = 10;
    int alturaBoard = 20;
    int bordaBoard = 2;
    int delayInicial = 1000;



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
