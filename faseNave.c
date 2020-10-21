#include "raylib.h"
#include <math.h>

//Define struct do fundo
typedef struct
{
    Rectangle rect;
    Vector2 FundoLocal;

} Espaco;

//Define struct da vida
typedef struct
{
    Vector2 VidaLocal;
} BarraVida;

//Define struct da esmeralda
typedef struct
{
    Rectangle rect;
    Vector2 EsmeraldaLocal;
} Esmeralda;

//Define struct do jogador.
typedef struct
{
    int currentFrame;
    Rectangle Explo;
    Rectangle hit;
    float Velocidade;
    Vector2 ExpLocal;
    Vector2 NaveLocal;
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
} Inimigo;

//Define struct do portal
typedef struct
{
    int currentFrameX;
    int currentFrameY;
    Rectangle Teletransporte;
    Vector2 PortalLocal;
    bool Teletransportar;
} Portal;

#define Num_Tiro 50
#define Num_Inimigos 50
#define Num_Cenario 4
#define Tipos_Inimigos 3

static bool Cutscene = true;
static bool Endscene = false;
static bool Invulneravel = false;
static bool pause = false;
static bool parado = false;
static bool victory = false;
static bool Transparencia = false;
static bool Cristal = false;

static int Vida = 3;
static int gameOver = 0;
static int Movimento = 2;
static int CadenciaTiro = 0;
static int TiposNave = 0;
static int Invulnerabilidade = 60;
static int Piscar = 0;

static Espaco espaco[Num_Cenario];
static Jogador jogador;
static Tiro tiro[Num_Tiro];
static Inimigo inimigo[Num_Inimigos];
static Portal portal;
static BarraVida vida[3];
static Esmeralda pedra;

static int AtivarInimigos = 20;
static int Objetivo = 50;
static int InimigosMortes = 0;

static int framesCounter = 0;
static int framesSpeed = 10;
static int ContadorFrame = 0;
static int VelocidadeFrame = 8;

static Texture2D Cenario;
static Texture2D Nave[5];
static Texture2D Tiros;
static Texture2D Adversario[Tipos_Inimigos];
static Texture2D ExplosaoI;
static Texture2D Aparecer;
static Texture2D Diamante;

static Sound Atirar;
static Sound ExplodirI;
static Sound ExplodirJ;
static Sound Dano;
static Sound Trilha;

//Inicializa
static void Iniciar(void);

//Teletransporte
static void Teletransporte(void);

//Atualiza
static void Atualizar(void);

//Desenha
static void Desenhando(void);

//Descarrega
static void Descarregar(void);

void Iniciar(void)
{

    //Som
    Atirar = LoadSound("sounds/FaseNave/gun.wav");
    ExplodirI = LoadSound("sounds/FaseNave/explodeA.wav");
    ExplodirJ = LoadSound("sounds/FaseNave/explodeJ.wav");
    Dano = LoadSound("sounds/FaseNave/dano.wav");
    Trilha = LoadSound("sounds/FaseNave/trilha.mp3");

    //Fundo
    for (int i = 0; i < Num_Cenario; i++)
    {
        espaco[i].rect.height = GetScreenHeight();
        espaco[i].rect.width = GetScreenWidth() / 2;

        if (i < Num_Cenario / 2)
        {
            espaco[i].FundoLocal.x = (GetScreenWidth() / 2) * i;
            espaco[i].FundoLocal.y = 0;
        }

        else
        {
            espaco[i].FundoLocal.x = GetScreenWidth() / 2 * (i - (Num_Cenario / 2));
            espaco[i].FundoLocal.y = -GetScreenHeight();
        }
    }

    Cenario = LoadTexture("assets/Fase3/Fundo.png");
    Cenario.width = GetScreenWidth() / 2;
    Cenario.height = GetScreenHeight();

    //Barra de vida
    for (int i = 0; i < Vida; i++)
    {
        vida[i].VidaLocal.x = 65 * i;
        vida[i].VidaLocal.y = 0;
    }

    //Esmeralda
    pedra.rect.height = 16;
    pedra.rect.width = 16;
    pedra.rect.x = 0;
    pedra.rect.y = 0;
    pedra.EsmeraldaLocal.x = GetScreenWidth()/2;
    pedra.EsmeraldaLocal.y = -30;

    Diamante = LoadTexture("assets/cristalVerde.png");

    //Jogador
    jogador.NaveLocal.x = (GetScreenWidth() / 2) - 65;
    jogador.NaveLocal.y = GetScreenHeight() / 2 + GetScreenHeight() / 3;

    jogador.hit.width = 50;
    jogador.hit.height = 60;

    jogador.Explo.width = 65;
    jogador.Explo.height = 64;

    jogador.Explodir = false;

    jogador.currentFrame = 0;

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
        inimigo[i].hit.y = GetRandomValue(-GetScreenHeight(), -64);

        inimigo[i].Explo.width = 65;
        inimigo[i].Explo.height = 64;

        inimigo[i].Explodir = false;

        inimigo[i].Velocidade.x = 0;
        inimigo[i].Velocidade.y = GetRandomValue(8, 12);

        inimigo[i].currentFrame = 0;
        inimigo[i].Ativo = true;
    }

    Adversario[0] = LoadTexture("assets/Fase3/Inimigo1.png");
    Adversario[1] = LoadTexture("assets/Fase3/Inimigo2.png");
    Adversario[2] = LoadTexture("assets/Fase3/Inimigo3.png");

    ExplosaoI = LoadTexture("assets/Fase3/explosao.png");

    //Portal
    portal.currentFrameX = 0;
    portal.currentFrameY = 1;
    portal.Teletransporte.width = 480;
    portal.Teletransporte.height = 128;
    portal.Teletransporte.y = 128;
    portal.PortalLocal.x = jogador.NaveLocal.x - 200;
    portal.PortalLocal.y = jogador.NaveLocal.y - 50;
    portal.Teletransportar = true;

    Aparecer = LoadTexture("assets/Portal.png");
    Aparecer.height = 384;
    Aparecer.width = 3840;
}

void Teletransporte(void)
{
    //Portal
    ContadorFrame++;
    if (portal.Teletransportar)
    {

        if (ContadorFrame >= (60 / VelocidadeFrame))
        {
            ContadorFrame = 0;
            portal.currentFrameX++;

            if (portal.currentFrameX > 7)
            {
                portal.currentFrameX = 0;

                if (portal.currentFrameY == 1)
                {
                    portal.currentFrameY = 0;
                }
                else if (portal.currentFrameY == 0)
                {
                    portal.currentFrameY = 2;
                }
                else if (portal.currentFrameY == 2)
                {
                    if(Cutscene)
                    {
                        portal.currentFrameY = 1;
                        Cutscene = false;
                        gameOver = 1;
                    }

                    else
                    {
                        victory = true;
                    }
                    portal.Teletransportar = false;
                }
            }
            portal.Teletransporte.x = portal.currentFrameX * 480;
            portal.Teletransporte.y = portal.currentFrameY * 128;
        }
    }

}

void Atualizar(void)
{

    if (Cutscene)
    {
        Teletransporte();
    }

    if (gameOver == 3)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            Iniciar();
            gameOver = 1;
        }
    }

    if (gameOver == 1)
    {
        if (IsKeyPressed('P'))
        {
            pause = !pause;
            parado = !parado;
        }

        if (!pause)
        {
            
            if (InimigosMortes >= Objetivo)
            {
                
                //Cena final
                Endscene = true;

                //Explosão de todos inimigos
                for (int i = 0; i < AtivarInimigos; i++)
                {
                    inimigo[i].ExpLocal.x = inimigo[i].hit.x;
                    inimigo[i].ExpLocal.y = inimigo[i].hit.y;
                }

                //Movimentacao pedra
                if(pedra.EsmeraldaLocal.y != GetScreenHeight()/2)
                {
                    pedra.EsmeraldaLocal.y += 3;
                }

                else
                {
                    //Movimentacao jogador
                    static float x, y;
                    static float dx, dy;
                    static float A, B;
                    int tempo = 30;

                    x = jogador.NaveLocal.x;
                    y = jogador.NaveLocal.y;
                    dx = x - (pedra.EsmeraldaLocal.x - 25);
                    dy = y - (pedra.EsmeraldaLocal.y - 20);
                    A = dx/tempo;
                    B = dy/tempo;

                   
                    jogador.NaveLocal.x -= A;
                    jogador.NaveLocal.y -= B;
                    if( pow(dx, 2) < 16 )
                    {
                        if(dx > -10)
                        {
                            //Ativando portal
                            Cristal = true;
                            portal.PortalLocal.x = jogador.NaveLocal.x - 200;
                            portal.PortalLocal.y = jogador.NaveLocal.y - 50;
                            
                            portal.Teletransportar = true;
                            
                            Teletransporte();
                        }
                    }
                }
            }
        }

        if (!parado)
        {
            if(!Endscene)
            {
                if(!IsSoundPlaying(Trilha))
                {
                    PlaySound(Trilha);
                }
                
                //Comportamento do fundo
                for (int i = 0; i < Num_Cenario; i++)
                {
                    espaco[i].FundoLocal.y += 10;
                    if (espaco[i].FundoLocal.y >= GetScreenHeight())
                    {
                        espaco[i].FundoLocal.y = -GetScreenHeight();
                    }
                }

                //Movimentacao, comportamento e movimentando os sprites do jogador.
                if (!jogador.Explodir)
                {
                    if (IsKeyDown(KEY_RIGHT))
                    {
                        jogador.NaveLocal.x += jogador.Velocidade;
                        Movimento += 1;
                    }
                    if (Movimento >= 4)
                        Movimento = 4;
                    if (IsKeyReleased(KEY_RIGHT))
                        Movimento = 2;
                    if (jogador.NaveLocal.x >= GetScreenWidth() - 64)
                        jogador.NaveLocal.x = GetScreenWidth() - 64;

                    if (IsKeyDown(KEY_LEFT))
                    {
                        jogador.NaveLocal.x -= jogador.Velocidade;
                        Movimento -= 1;
                    }
                    if (Movimento <= 0)
                        Movimento = 0;
                    if (IsKeyReleased(KEY_LEFT))
                        Movimento = 2;
                    if (jogador.NaveLocal.x <= 0)
                        jogador.NaveLocal.x = 0;

                    if (IsKeyDown(KEY_UP))
                        jogador.NaveLocal.y -= jogador.Velocidade;
                    if (jogador.NaveLocal.y <= 0)
                        jogador.NaveLocal.y = 0;

                    if (IsKeyDown(KEY_DOWN))
                        jogador.NaveLocal.y += jogador.Velocidade;
                    if (jogador.NaveLocal.y >= GetScreenHeight() - 64)
                        jogador.NaveLocal.y = GetScreenHeight() - 64;
                }
            }

            //Colisao jogador inimigo
            if (Invulneravel == false && !Endscene)
            {
                for (int i = 0; i < AtivarInimigos; i++)
                {
                    if (CheckCollisionRecs(jogador.hit, inimigo[i].hit))
                    {
                        if (Vida == 1)
                        {
                            Vida--;
                            PlaySound(ExplodirJ);
                            jogador.ExpLocal.x = jogador.hit.x;
                            jogador.ExpLocal.y = jogador.hit.y;
                            jogador.Explodir = true;
                        }
                        else
                        {
                            PlaySound(Dano);
                            Vida--;
                            Invulneravel = true;
                        }
                    }
                }
            }
            else
            {
                Invulnerabilidade--;
                if (Invulnerabilidade == 0)
                {
                    Invulnerabilidade = 60;
                    Invulneravel = false;
                }
            }

            //Tiro.
            if (!jogador.Explodir)
            {
                if (IsKeyDown(KEY_SPACE))
                {
                    CadenciaTiro += 4;
                    for (int i = 0; i < Num_Tiro; i++)
                    {
                        if (!tiro[i].Ativo && CadenciaTiro % 36 == 0)
                        {
                            PlaySound(Atirar);

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
                            //Destruindo todos inimigos
                            if(InimigosMortes >= Objetivo)
                            {
                                for (int i = 0; i < AtivarInimigos; i++)
                                {
                                    PlaySound(ExplodirI);
                                    inimigo[i].Explodir = true;
                                    inimigo[i].ExpLocal.x = inimigo[i].hit.x;
                                    inimigo[i].ExpLocal.y = inimigo[i].hit.y;
                                    
                                }
                            }
                            //Colisao bala inimigo
                            else if (CheckCollisionRecs(tiro[i].hit, inimigo[j].hit))
                            {
                                tiro[i].Ativo = false;

                                if (inimigo[j].Velocidade.y == 8)
                                {
                                    inimigo[j].Explodir = false;
                                }
                                else
                                {
                                    PlaySound(ExplodirI);

                                    inimigo[j].Explodir = true;

                                    inimigo[j].ExpLocal.x = inimigo[j].hit.x;
                                    inimigo[j].ExpLocal.y = inimigo[j].hit.y;

                                    inimigo[j].hit.x = GetRandomValue(0, GetScreenWidth() - 64);
                                    inimigo[j].hit.y = GetRandomValue(-GetScreenHeight(), -64);

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
            
            if(!Endscene)
            {
                //Inimigo.
                for (int i = 0; i < AtivarInimigos; i++)
                {
                    if (inimigo[i].Ativo)
                    {
                        inimigo[i].hit.y += inimigo[i].Velocidade.y;
                        inimigo[i].InimigoLocal.y = inimigo[i].hit.y;
                        inimigo[i].InimigoLocal.x = inimigo[i].hit.x;

                        if (inimigo[i].hit.y > GetScreenHeight() + 70)
                        {
                            inimigo[i].Velocidade.y = GetRandomValue(8, 12);

                            if (inimigo[i].Velocidade.y == 8)
                            {
                                //Meteoro nasce perto do jogador
                                inimigo[i].hit.x = GetRandomValue(jogador.NaveLocal.x - (GetScreenWidth() / 8), jogador.NaveLocal.x + (GetScreenWidth() / 8));
                                inimigo[i].hit.y = GetRandomValue(-GetScreenHeight(), -64);
                            }
                            else
                            {
                                inimigo[i].hit.x = GetRandomValue(0, GetScreenWidth() - 64);
                                inimigo[i].hit.y = GetRandomValue(-GetScreenHeight(), -64);
                            }
                        }
                    }
                }
            }

            //Explosão
            framesCounter++;
            //do inimigo
            for (int j = 0; j < AtivarInimigos; j++)
            {
                if (inimigo[j].Explodir)
                {
                    if (framesCounter >= (60 / framesSpeed))
                    {
                        inimigo[j].currentFrame++;

                        if (inimigo[j].currentFrame > 8)
                        {
                            inimigo[j].currentFrame = 0;
                            inimigo[j].Explodir = false;
                        }
                        inimigo[j].Explo.x = inimigo[j].currentFrame * 65;
                    }
                }
            }

            if(!Endscene)
            {
                //do jogador
                if (jogador.Explodir)
                {
                    if (framesCounter >= (60 / framesSpeed))
                    {
                        jogador.currentFrame++;

                        if (jogador.currentFrame > 8)
                        {
                            jogador.currentFrame = 0;
                            gameOver = 3;
                        }
                        jogador.Explo.x = jogador.currentFrame * 65;
                    }
                }
            }

            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
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
    ClearBackground(WHITE);


    if (gameOver < 3)
    {
        //Desenhando fundo
        for (int i = 0; i < Num_Cenario; i++)
        {
            DrawTextureRec(Cenario, espaco[i].rect, espaco[i].FundoLocal, WHITE);
        }

        //Desenhando pontuacao
        DrawText(TextFormat("INIMIGOS: %i ", (Objetivo - InimigosMortes)), (5 * GetScreenWidth() / 12), 15, 30, WHITE);

        //Desenhando vida
        for (int i = 0; i < Vida; i++)
        {
            DrawTexture(Nave[2], vida[i].VidaLocal.x, vida[i].VidaLocal.y, WHITE);
        }

        //Desenhando Portal
        if (Cutscene)
        {

            if (portal.currentFrameY == 2)
            {
                DrawTexture(Nave[Movimento], jogador.NaveLocal.x, jogador.NaveLocal.y, WHITE);
            }
            DrawTextureRec(Aparecer, portal.Teletransporte, portal.PortalLocal, WHITE);
        }

        if (gameOver == 1)
        {
            if(!Endscene)
            {

            
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

                        if (inimigo[i].Velocidade.y == 8)
                        {
                            TiposNave = 0;
                        }

                        else if (inimigo[i].Velocidade.y <= 10)
                        {
                            TiposNave = 1;
                        }

                        else
                        {
                            TiposNave = 2;
                        }
                        DrawTextureRec(Adversario[TiposNave], inimigo[i].rect, inimigo[i].InimigoLocal, WHITE);

                        //Desenhando explosão inimigo
                        if (inimigo[i].Explodir)
                        {
                            DrawTextureRec(ExplosaoI, inimigo[i].Explo, inimigo[i].ExpLocal, GOLD);
                        }
                    }
                }

                //Desenhando jogador
                if (!jogador.Explodir)
                {
                    if (!Invulneravel)
                    {
                        DrawTexture(Nave[Movimento], jogador.NaveLocal.x, jogador.NaveLocal.y, WHITE);
                    }

                    else
                    {
                        Piscar++;
                        if (Transparencia)
                        {
                            DrawTexture(Nave[Movimento], jogador.NaveLocal.x, jogador.NaveLocal.y, BLANK);

                            if (Piscar == 5)
                            {
                                Piscar = 0;
                                Transparencia = false;
                            }
                        }
                        else
                        {
                            DrawTexture(Nave[Movimento], jogador.NaveLocal.x, jogador.NaveLocal.y, WHITE);

                            if (Piscar == 5)
                            {
                                Piscar = 0;
                                Transparencia = true;
                            }
                        }
                    }
                }

                //Desenhando Explosão jogador
                else if (jogador.Explodir)
                {
                    DrawTextureRec(ExplosaoI, jogador.Explo, jogador.ExpLocal, ORANGE);
                }

                if (victory)
                {
                    parado = true;
                }

                if (pause)
                {
                    DrawText("PAUSE", GetScreenWidth() / 2 - MeasureText("PAUSE", 40) / 2, GetScreenHeight() / 2 - 40, 40, WHITE);
                    parado = true;
                }
            }

            else
            {
                //Desenhando fundo
                for (int i = 0; i < Num_Cenario; i++)
                {
                    DrawTextureRec(Cenario, espaco[i].rect, espaco[i].FundoLocal, WHITE);
                }

                //Desenhando explosão inimigo
                for (int i = 0; i < AtivarInimigos; i++)
                {
                    if (inimigo[i].Explodir)
                    {
                        DrawTextureRec(ExplosaoI, inimigo[i].Explo, inimigo[i].ExpLocal, GOLD);
                    }
                }

                //Desenhando Pedra
                if(portal.currentFrameY != 2)
                {
                    DrawTextureRec(Diamante, pedra.rect , pedra.EsmeraldaLocal, WHITE);
                }
                
                //Desenhando Nave
                if(portal.currentFrameY != 2)
                {
                    DrawTexture(Nave[2], jogador.NaveLocal.x, jogador.NaveLocal.y, WHITE);
                }

                //Desenhando Portal
                if(Cristal && !victory)
                {
                    DrawTextureRec(Aparecer, portal.Teletransporte, portal.PortalLocal, WHITE);
                }
            }
        }
    }

    else if (gameOver == 3)
    {

        DrawText("PRECIONE [ENTER] PARA JOGAR", GetScreenWidth() / 2 - MeasureText("PRECIONE [ENTER] PARA JOGAR", 20) / 2, GetScreenHeight() / 2 - 50, 20, BLACK);
        InimigosMortes = 0;
    }

    EndDrawing();
}

void Descarregar(void)
{
    UnloadTexture(Cenario);
    UnloadTexture(Nave[5]);
    UnloadTexture(Tiros);
    UnloadTexture(Adversario[Tipos_Inimigos]);
    UnloadTexture(ExplosaoI);
    UnloadTexture(Aparecer);
    UnloadTexture(Diamante);
}

int Fase3()
{
    int retorno3 = 0;
    Iniciar();

    while (retorno3 == 0)
    {
        if (IsKeyPressed(KEY_ZERO))
        {
            retorno3 = 1;
        }
        else if (victory)
        {
            retorno3 = 2;
        }
        Atualizar();
        Desenhando();
    }
    Descarregar();

    return retorno3;
}