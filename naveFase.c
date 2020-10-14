#include "raylib.h"

//Define struct do jogador.
typedef struct 
{
    Rectangle hit;
    float Velocidade;
    Vector2 NaveLocal;
    Color Cor;
} Jogador;

//Define struct do tiro.
typedef struct 
{
    Rectangle rect;
    Rectangle hit;
    Vector2 Velocidade;
    Vector2 TiroLocal;
    bool Ativo;
    Color Cor;
} Tiro;

//Define struct do inimigo.
typedef struct 
{
    Rectangle rect;
    Vector2 InimigoLocal;
    bool Ativo;
    Color cor;
} Inimigo;

#define Num_Tiro 50
#define Num_Inimigos 50

static bool gameOver = false;
static bool pause =  false;
static bool parado = false;
static bool victory = false;

static int Movimento = 2;
static int Bala = 0;
static int CadenciaTiro = 0;

static Jogador jogador;
static Tiro tiro[Num_Tiro];
static Inimigo inimigo[Num_Inimigos];

static int AtivarInimigos = 10;
static int InimigosMortes = 0;

static Texture2D Nave[5];
static Texture2D Tiros[6];

//Inicializa
static void Iniciar(void);   
//Atualiza     
static void Atualizar(void);  
//Desenha    
static void Desenhando(void);
//Descarrega
static void Descarregar(void);

void Iniciar(void)
{
    jogador.NaveLocal.x = GetScreenWidth()/2;
    jogador.NaveLocal.y = GetScreenHeight()/2;
    jogador.hit.width = 50;
    jogador.hit.height = 60;
    jogador.Cor = WHITE;
    jogador.Velocidade = 6.0;

    Nave[0] = LoadTexture("assets/Fase3/Nave3E.png");
    Nave[1] = LoadTexture("assets/Fase3/Nave2E.png");
    Nave[2] = LoadTexture("assets/Fase3/Nave1.png");
    Nave[3] = LoadTexture("assets/Fase3/Nave2D.png");
    Nave[4] = LoadTexture("assets/Fase3/Nave3D.png");  

    for (int i = 0; i < Num_Tiro; i++)
    {
        tiro[i].rect.width = 23;
        tiro[i].rect.height = 29;
        tiro[i].hit.width = 23;
        tiro[i].hit.height = 29;
        tiro[i].Velocidade.x = 0;
        tiro[i].Velocidade.y = -10;
        tiro[i].TiroLocal.x = 0;
        tiro[i].TiroLocal.y = 0;
        tiro[i].Ativo = false;
        tiro[i].Cor = MAROON;
    }

    Tiros[0] = LoadTexture("assets/Fase3/Tiro1.png");
    Tiros[1] = LoadTexture("assets/Fase3/Tiro2.png");
    Tiros[2] = LoadTexture("assets/Fase3/Tiro3.png");
    Tiros[3] = LoadTexture("assets/Fase3/Tiro4.png");
    Tiros[4] = LoadTexture("assets/Fase3/Tiro5.png");
    Tiros[5] = LoadTexture("assets/Fase3/Tiro6.png");
    

    for (int i = 0; i < Num_Inimigos; i++)
    {
        inimigo[i].rect.width = 64;
        inimigo[i].rect.height = 64;
        inimigo[i].InimigoLocal.y = 5;
        inimigo[i].Ativo = true;
        inimigo[i].cor = BLUE;
    }  
}

void Atualizar(void)
{
    if(!gameOver)
    {
        if (IsKeyPressed('P')){
            pause = !pause;
            parado = !parado;
        }    

        if (!pause)
        {
            if (InimigosMortes == AtivarInimigos) victory = true;
        }
    }
    if (IsKeyPressed(KEY_ENTER))
        {
            Iniciar();
            gameOver = false;
        }
    if (!parado)
    {
    //Movimentacao, comportamento e movimentando os sprites.
    if (IsKeyDown(KEY_RIGHT)){
         jogador.NaveLocal.x += jogador.Velocidade;
         Movimento += 1;
        }
    if(Movimento >= 4) Movimento = 4;
    if (IsKeyReleased(KEY_RIGHT)) Movimento = 2;
    if (jogador.NaveLocal.x >= GetScreenWidth() - 64) jogador.NaveLocal.x = GetScreenWidth() - 64;

    if (IsKeyDown(KEY_LEFT)){
         jogador.NaveLocal.x -= jogador.Velocidade;
         Movimento -= 1;
        }
    if(Movimento <= 0) Movimento = 0;
    if (IsKeyReleased(KEY_LEFT)) Movimento = 2;
    if (jogador.NaveLocal.x <= 0) jogador.NaveLocal.x = 0;
    
    if (IsKeyDown(KEY_UP)) jogador.NaveLocal.y -= jogador.Velocidade;
    if (jogador.NaveLocal.y <= 0) jogador.NaveLocal.y = 0;

    if (IsKeyDown(KEY_DOWN))  jogador.NaveLocal.y += jogador.Velocidade;
    if (jogador.NaveLocal.y >= GetScreenHeight() - 64) jogador.NaveLocal.y = GetScreenHeight() - 64;

    //Colisao jogador inimigo
    for (int i = 0; i < AtivarInimigos; i++)
    {
        if (CheckCollisionRecs(jogador.hit, inimigo[i].rect)) gameOver = true;
    }

    //Tiro.
    if (IsKeyDown(KEY_SPACE))
    {
        CadenciaTiro += 4;
        for (int i = 0; i < Num_Tiro; i++)
        {
            if (!tiro[i].Ativo && CadenciaTiro % 36 == 0)
            {
                tiro[i].TiroLocal.x = jogador.NaveLocal.x + 20;
                tiro[i].TiroLocal.y = jogador.NaveLocal.y + 16;

                tiro[i].hit.x = jogador.NaveLocal.x + 20;
                tiro[i].hit.y = jogador.NaveLocal.y + 16;

                tiro[i].Ativo = true;
                break;
            }
        }
    }
    for (int i = 0; i < Num_Tiro; i++)
    {
        if (tiro[i].Ativo)
        {
            tiro[i].TiroLocal.y += tiro[i].Velocidade.y;
            tiro[i].hit.y += tiro[i].Velocidade.y;

            for (int j = 0; j < AtivarInimigos; j++)
            {
                if (inimigo[j].Ativo)
                {
                    //Colisao bala inimigo
                    if (CheckCollisionRecs(tiro[i].hit, inimigo[j].rect))
                    {
                        tiro[i].Ativo = false;
                        inimigo[j].rect.x = GetRandomValue(GetScreenWidth(), GetScreenWidth() + 1000);
                        inimigo[j].rect.y = GetRandomValue(0, GetScreenHeight() - inimigo[j].rect.height);
                        CadenciaTiro = 0;
                        InimigosMortes++;

                    }

                    if (tiro[i].TiroLocal.y <= -30) 
                    {
                        tiro[i].Ativo = false;
                        CadenciaTiro = 0;
                    }
                }
            }
        }
    }

    //Inimigo.
    for (int i = 0; i < AtivarInimigos; i++)
    {
        if (inimigo[i].Ativo)
        {
            inimigo[i].rect.y += inimigo[i].InimigoLocal.y;

            if (inimigo[i].rect.y > GetScreenHeight())
            {
                inimigo[i].rect.x = GetRandomValue(0, GetScreenWidth() - 64);
                inimigo[i].rect.y = GetRandomValue(-GetScreenHeight(), - 64);
            }
        }
    }
    }
}

void Desenhando(void)
{
    //HitBox do jogador
    jogador.hit.x = jogador.NaveLocal.x + 6;
    jogador.hit.y = jogador.NaveLocal.y + 2;


    BeginDrawing();
        ClearBackground(RAYWHITE);

        if(!gameOver)
        {

            DrawText("Mova a nave com as setas", 15, 15, 25, DARKGRAY);

            DrawRectangleRec(jogador.hit, BLACK);

            for (int i = 0; i < Num_Tiro; i++)
            {
                if (tiro[i].Ativo)
                DrawTextureRec(Tiros[Bala], tiro[i].rect, tiro[i].TiroLocal, WHITE);
            }

            for (int i = 0; i < AtivarInimigos; i++)
            {
            if (inimigo[i].Ativo)
            DrawRectangleRec(inimigo[i].rect, inimigo[i].cor);
            }

            DrawTexture(Nave[Movimento], jogador.NaveLocal.x, jogador.NaveLocal.y, WHITE);

            if (victory)
            {
                parado = true;
            }
        
            if (pause){
                DrawText("PAUSE", GetScreenWidth()/2 - MeasureText("PAUSE", 40)/2, GetScreenHeight()/2 - 40, 40, GRAY);
                parado = true;
            }    
        }
        else DrawText("PRECIONE [ENTER] PARA JOGAR", GetScreenWidth()/2 - MeasureText("PRECIONE [ENTER] PARA JOGAR", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);
        

    EndDrawing();
}    

void Descarregar(void)
{
    UnloadTexture(Nave[Movimento]);
    UnloadTexture(Tiros[0]);
}

int Fase3()
{
    int retorno3 = 0;
    Iniciar();
    
    while ( retorno3 == 0  )
    {
        if(IsKeyPressed(KEY_ZERO)){
            retorno3 = 0;
        }
        else if(victory){
            retorno3 = 2;
        }
        Atualizar();
        Desenhando();
    }
    Descarregar();
    
    return retorno3;

}