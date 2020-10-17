#include "raylib.h"

//Define struct do fundo
typedef struct 
{
    Rectangle rect;
    Vector2 FundoLocal;
    Color Cor;
} Fundo;

//Define struct do jogador.
typedef struct 
{
    int currentFrame;
    Rectangle Explo;
    Rectangle hit;
    float Velocidade;
    Vector2 ExpLocal;
    Vector2 NaveLocal;
    Color Cor;
    bool Explodir;
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
    int currentFrame;
    Rectangle Explo;
    Rectangle hit;
    Rectangle rect;
    Vector2 ExpLocal;
    Vector2 Velocidade;
    Vector2 InimigoLocal;
    bool Explodir;
    bool Ativo;
    Color cor;
} Inimigo;

#define Num_Tiro 50
#define Num_Inimigos 50
#define Num_Cenario 4
#define Tipos_Inimigos 3

static bool gameOver = false;
static bool pause =  false;
static bool parado = false;
static bool victory = false;

static int Movimento = 2;
static int CadenciaTiro = 0;
static int TiposNave = 0;

static Fundo fundo[Num_Cenario];
static Jogador jogador;
static Tiro tiro[Num_Tiro];
static Inimigo inimigo[Num_Inimigos];

static int AtivarInimigos = 20;
static int Objetivo = 50;
static int InimigosMortes = 0;

static int framesCounter = 0;
static int framesSpeed = 10;

static Texture2D Cenario;
static Texture2D Nave[5];
static Texture2D Tiros;
static Texture2D Adversario[Tipos_Inimigos];
static Texture2D ExplosaoI;

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

    //Fundo
    for (int i = 0; i < Num_Cenario; i++)
    {
    fundo[i].rect.height = GetScreenHeight();
    fundo[i].rect.width = GetScreenWidth()/2;

    if(i < Num_Cenario/2)
    {
        fundo[i].FundoLocal.x = (GetScreenWidth()/2)*i;
        fundo[i].FundoLocal.y = 0;
    }

    else
    {
        fundo[i].FundoLocal.x = GetScreenWidth()/2*(i-(Num_Cenario/2));
        fundo[i].FundoLocal.y = -GetScreenHeight();
    }
    

    fundo[i].Cor = WHITE;
    }

    Cenario = LoadTexture("assets/Fase3/Fundo.png");
    Cenario.width = GetScreenWidth()/2;
    Cenario.height = GetScreenHeight();

    //Jogador
    jogador.NaveLocal.x = GetScreenWidth()/2;
    jogador.NaveLocal.y = GetScreenHeight()/2;

    jogador.hit.width = 50;
    jogador.hit.height = 60;

    jogador.Explo.width = 65;
    jogador.Explo.height = 64;
    
    jogador.Explodir = false;

    jogador.currentFrame = 0;

    jogador.Cor = WHITE;
    jogador.Velocidade = 6.0;

    Nave[0] = LoadTexture("assets/Fase3/Nave3E.png");
    Nave[1] = LoadTexture("assets/Fase3/Nave2E.png");
    Nave[2] = LoadTexture("assets/Fase3/Nave1.png");
    Nave[3] = LoadTexture("assets/Fase3/Nave2D.png");
    Nave[4] = LoadTexture("assets/Fase3/Nave3D.png");  

    //Tiro
    for (int i = 0; i < Num_Tiro; i++)
    {
        tiro[i].rect.width = 23;
        tiro[i].rect.height = 29;

        tiro[i].hit.width = 23;
        tiro[i].hit.height = 29;

        tiro[i].Velocidade.y = -10;
        tiro[i].Ativo = false;
        tiro[i].Cor = MAROON;
    }

    Tiros = LoadTexture("assets/Fase3/Tiro1.png");
    
    //Inimigo
    for (int i = 0; i < Num_Inimigos; i++)
    {
        inimigo[i].rect.width = 64;
        inimigo[i].rect.height = 64;

        inimigo[i].hit.width = 64;
        inimigo[i].hit.height = 64;
        inimigo[i].hit.x = GetRandomValue(0, GetScreenWidth() - 64);
        inimigo[i].hit.y = GetRandomValue(-GetScreenHeight(), - 64);

        inimigo[i].Explo.width = 65;
        inimigo[i].Explo.height = 64;

        inimigo[i].Explodir = false;

        inimigo[i].Velocidade.x = 0;
        inimigo[i].Velocidade.y = GetRandomValue(8, 12);

        inimigo[i].currentFrame = 0;
        inimigo[i].Ativo = true;
        inimigo[i].cor = BLUE;
    }  

    Adversario[0] = LoadTexture("assets/Fase3/Inimigo1.png");
    Adversario[1] = LoadTexture("assets/Fase3/Inimigo2.png");
    Adversario[2] = LoadTexture("assets/Fase3/Inimigo3.png");

    ExplosaoI = LoadTexture("assets/Fase3/explosao.png"); 

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
            if (InimigosMortes == Objetivo) victory = true;
        }
    }
    if (IsKeyPressed(KEY_ENTER))
        {
            Iniciar();
            gameOver = false;
        }
    if (!parado)
    {
    //Comportamento do fundo
        for (int i = 0; i < Num_Cenario; i++)
        {
            fundo[i].FundoLocal.y += 8;
            if(fundo[i].FundoLocal.y >= GetScreenHeight())
            {
                fundo[i].FundoLocal.y = -GetScreenHeight();
            }
        }
        

    //Movimentacao, comportamento e movimentando os sprites do jogador.
    if(!jogador.Explodir)
    {
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
    }    

    //Colisao jogador inimigo
    for (int i = 0; i < AtivarInimigos; i++)
    {
        if (CheckCollisionRecs(jogador.hit, inimigo[i].hit))
        {
            jogador.ExpLocal.x = jogador.hit.x;
            jogador.ExpLocal.y = jogador.hit.y;
            jogador.Explodir = true;
        }     
    }

    //Tiro.
    if(!jogador.Explodir)
    {
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
                    if (CheckCollisionRecs(tiro[i].hit, inimigo[j].hit))
                    {
                        tiro[i].Ativo = false;

                        if(inimigo[j].Velocidade.y == 8)
                        {
                            inimigo[j].Explodir = false;
                        }
                        else
                        {
                            inimigo[j].Explodir = true;
                        

                        inimigo[j].ExpLocal.x = inimigo[j].hit.x;
                        inimigo[j].ExpLocal.y = inimigo[j].hit.y;


                        inimigo[j].hit.x = GetRandomValue(GetScreenWidth(), GetScreenWidth() + 1000);
                        inimigo[j].hit.y = GetRandomValue(0, GetScreenHeight() - inimigo[j].hit.height);

                        CadenciaTiro = 0;
                        InimigosMortes++;
                        }
                        

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
            inimigo[i].hit.y += inimigo[i].Velocidade.y ;
            inimigo[i].InimigoLocal.y = inimigo[i].hit.y;
            inimigo[i].InimigoLocal.x = inimigo[i].hit.x;

            if (inimigo[i].hit.y > GetScreenHeight() + 70)
            {
                inimigo[i].Velocidade.y = GetRandomValue(8, 12);

                inimigo[i].hit.x = GetRandomValue(0, GetScreenWidth() - 64);
                inimigo[i].hit.y = GetRandomValue(-GetScreenHeight(), - 64);
            }
        }
    }

    //Explosão
        framesCounter++;
        //do inimigo
        for(int j = 0; j < AtivarInimigos; j++)
        {
            if(inimigo[j].Explodir)
            {
                if(framesCounter >= (60/framesSpeed))
                {   
                    inimigo[j].currentFrame++;

                    if (inimigo[j].currentFrame > 8)
                    {
                        inimigo[j].currentFrame = 0;
                        inimigo[j].Explodir = false; 

                    }
                    inimigo[j].Explo.x = inimigo[j].currentFrame*65;
                        
                }
            }
        }
        //do jogador
        if(jogador.Explodir)
        {
            if(framesCounter >= (60/framesSpeed))
            {   
                jogador.currentFrame++;

                if (jogador.currentFrame > 8)
                {
                    jogador.currentFrame = 0;
                    gameOver = true; 

                }
                jogador.Explo.x = jogador.currentFrame*65;
                    
            }
        }

        
        if(framesCounter >= (60/framesSpeed))
        {   
            framesCounter = 0;
        }

    }
}

void Desenhando(void)
{
    //HitBox do jogador
    jogador.hit.x = jogador.NaveLocal.x + 6;
    jogador.hit.y = jogador.NaveLocal.y + 2;


    BeginDrawing();
        ClearBackground(WHITE);

        if(!gameOver)
        {       
            //Desenhando fundo
            for (int i = 0; i < Num_Cenario; i++)
            {               
                DrawTextureRec(Cenario, fundo[i].rect, fundo[i].FundoLocal, WHITE);
            }

            //Desenhando pontuacao
            DrawText(TextFormat("INIMIGOS: %i ", (Objetivo - InimigosMortes)), 15, 15, 30, MAGENTA);

            //Desenhando tiro
            for (int i = 0; i < Num_Tiro; i++)
            {
                if (tiro[i].Ativo)
                DrawTextureRec(Tiros, tiro[i].rect, tiro[i].TiroLocal, WHITE);
            }

            //Desenhando inimigo
            for (int i = 0; i < AtivarInimigos; i++)
            {
                if (inimigo[i].Ativo)
                {

                    if(inimigo[i].Velocidade.y == 8)
                    {
                        TiposNave = 0;
                    }
                    
                    else if(inimigo[i].Velocidade.y <= 10)
                    {
                        TiposNave = 1;
                    }

                    else 
                    {
                        TiposNave = 2;
                    }
                    DrawTextureRec(Adversario[TiposNave], inimigo[i].rect, inimigo[i].InimigoLocal, WHITE);
                    
                    
                    
                    //Desenhando explosão inimigo
                    if(inimigo[i].Explodir)
                    {
                        DrawTextureRec(ExplosaoI, inimigo[i].Explo, inimigo[i].ExpLocal, GOLD);
                    }    
                }
            }

            //Desenhando jogador
            if(!jogador.Explodir)
            {
            DrawTexture(Nave[Movimento], jogador.NaveLocal.x, jogador.NaveLocal.y, WHITE);
            }
            //Desenhando Explosão jogador
            else if(jogador.Explodir)
            {
                DrawTextureRec(ExplosaoI, jogador.Explo, jogador.ExpLocal, ORANGE);
            }

            if (victory)
            {
                parado = true;
            }
        
            if (pause){
                DrawText("PAUSE", GetScreenWidth()/2 - MeasureText("PAUSE", 40)/2, GetScreenHeight()/2 - 40, 40, WHITE);
                parado = true;
            }    
        }
        else
        {
            DrawText("PRECIONE [ENTER] PARA JOGAR", GetScreenWidth()/2 - MeasureText("PRECIONE [ENTER] PARA JOGAR", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
            InimigosMortes = 0;
        }
        

    EndDrawing();
}    

void Descarregar(void)
{
    UnloadTexture(Nave[Movimento]);
    UnloadTexture(Tiros);
    UnloadTexture(Adversario[Tipos_Inimigos]);
    UnloadTexture(ExplosaoI);
}

int Fase3()
{
    int retorno3 = 0;
    Iniciar();
    
    while ( retorno3 == 0  )
    {
        if(IsKeyPressed(KEY_ZERO)){
            retorno3 = 1;
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