#include <raylib.h>

//estruturas
typedef struct{
    Texture2D imagem;
    Rectangle frameRec;
    Vector2 posi;
    int vel;
    int lado;
    int linha;
}hero;
typedef struct{
    Texture2D imagem;
    Rectangle frameRec;
    Vector2 posi;
}chefe;
typedef struct{
    Texture2D imagem;
    Rectangle frameRec;
    Vector2 posi;
}laser;
typedef struct{
    Texture2D image;
}Imagem;

//globais
hero virtualGuy;
chefe computador;
laser armaLaserDir;
laser armaLaserEsc;

//partes da sala base
Imagem assets;
Rectangle tetoLinha;
Rectangle tetoColuna;
Rectangle chao;
Rectangle parede;
Rectangle tetoQuina1;
Rectangle tetoQuina2;
Rectangle tetoQuina3;
Rectangle tetoQuina4;
Rectangle tetoQuina3;
Rectangle paredeComputadorDir;
Rectangle paredeComputadorEsc;

//funções
//sala base do jogo
void inicializarSala(Texture2D cena);
void drawSala();
//personagem
void atualizarPersonagem();
void animaPersonagem();

void inicializarNivelBoss();
void atualizarNivelBoss();
void drawNivelBoss();


int BOSS(){
    int resultado4 = 0;

    Texture2D Industri = LoadTexture("assets/Boss/Industrial.png");
    Industri.width = Industri.width*2;
    Industri.height = Industri.height*2;
    Texture2D VirtualGuy = LoadTexture("assets/full.png");
    VirtualGuy.width = VirtualGuy.width*2;
    VirtualGuy.height = VirtualGuy.height*2;
    virtualGuy.imagem = VirtualGuy;
    Texture2D Computador = LoadTexture("assets/Boss/PC.png");
    Computador.width = Computador.width*6.4;
    Computador.height = Computador.height*6.4;
    computador.imagem = Computador;
    Texture2D Laser = LoadTexture("assets/Boss/laser.png");
    Laser.width = Laser.width*4;
    Laser.height = Laser.height*4;
    armaLaserDir.imagem = Laser;

    inicializarSala(Industri);
    inicializarNivelBoss();

    while(resultado4 == 0){
        if(IsKeyPressed(KEY_ZERO)){
            resultado4 = 1;
        }
        atualizarNivelBoss();
        atualizarPersonagem();
        drawNivelBoss();
    }

    UnloadTexture(Industri);
    UnloadTexture(VirtualGuy);
    UnloadTexture(Computador);
    UnloadTexture(Laser);
    return resultado4;
}


void inicializarNivelBoss(){
    virtualGuy.frameRec.width = virtualGuy.imagem.width/12;
    virtualGuy.frameRec.height = virtualGuy.imagem.height/14;
    virtualGuy.frameRec.x = virtualGuy.frameRec.width*0;
    virtualGuy.frameRec.y = virtualGuy.frameRec.height*2;
    virtualGuy.posi.x = GetScreenWidth()/2;
    virtualGuy.posi.y = GetScreenHeight()/2;
    virtualGuy.vel = 3;
    virtualGuy.lado = 1;
    virtualGuy.linha = 2;

    computador.frameRec.width = computador.imagem.width/5;
    computador.frameRec.height = computador.imagem.height/3;
    computador.frameRec.x = computador.frameRec.width*0;
    computador.frameRec.y = computador.frameRec.height*2;
    computador.posi.x = (GetScreenWidth()/2) - 48;
    computador.posi.y = chao.height*3;

    armaLaserDir.frameRec.width = armaLaserDir.imagem.width;
    armaLaserDir.frameRec.height = armaLaserDir.imagem.height/32;
    armaLaserDir.frameRec.x = 0;
    armaLaserDir.frameRec.y = armaLaserDir.frameRec.height*16;
    armaLaserDir.posi.x = GetScreenWidth()/2 + 64;
    armaLaserDir.posi.y = chao.height*6;
    armaLaserEsc.frameRec.width = armaLaserDir.imagem.width;
    armaLaserEsc.frameRec.height = armaLaserDir.imagem.height/32;
    armaLaserEsc.frameRec.x = 0;
    armaLaserEsc.frameRec.y = armaLaserDir.frameRec.height*0;
    armaLaserEsc.posi.x = (GetScreenWidth()-tetoLinha.width*17)/2;
    armaLaserEsc.posi.y = chao.height*6;
}
void atualizarNivelBoss(){
    static int frameCont  = 0;
    static int frameAtual  = 0;
    static int direction  = -1;

    frameCont++;
    if (frameCont >= 60/2){
        frameCont = 0;
        frameAtual++;
        if(frameAtual > 1){
            frameAtual = 0;
        }
        computador.frameRec.x = computador.frameRec.width*frameAtual;
    }

    //movimentar lasers
    if(direction == -1){
        if(armaLaserDir.posi.y <= chao.height*12){
            armaLaserDir.posi.y += 1;
            armaLaserEsc.posi.y += 1;
        }
        else
            direction = 1;
    }
    else{
        if(armaLaserDir.posi.y >= chao.height*6){
            armaLaserDir.posi.y -= 1;
            armaLaserEsc.posi.y -= 1;
        }
        else
            direction = -1;
    }
    
}
void drawNivelBoss(){
    BeginDrawing();
        ClearBackground(GREEN);
        
        drawSala();

        DrawTextureRec(computador.imagem, computador.frameRec, computador.posi, RAYWHITE);

        DrawTextureRec(virtualGuy.imagem, virtualGuy.frameRec, virtualGuy.posi, RAYWHITE);

        DrawTextureRec(armaLaserDir.imagem, armaLaserDir.frameRec, armaLaserDir.posi, RAYWHITE);
        DrawTextureRec(armaLaserDir.imagem, armaLaserEsc.frameRec, armaLaserEsc.posi, RAYWHITE);

    EndDrawing();
}

void atualizarPersonagem(){
    animaPersonagem();

    if(IsKeyDown(KEY_UP)){
        if(virtualGuy.posi.y > chao.height*4.5){
            virtualGuy.posi.y -= virtualGuy.vel;
        }
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 0;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 1;
        }
    }
    if(IsKeyDown(KEY_DOWN)){
        if(virtualGuy.posi.y < chao.height*12){
            virtualGuy.posi.y += virtualGuy.vel;
        }
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 0;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 1;
        }
    }
    if(IsKeyDown(KEY_LEFT)){
        virtualGuy.lado = -1;
        if(virtualGuy.posi.x > (GetScreenWidth()-tetoLinha.width*17)/2 -10){
            virtualGuy.posi.x -= virtualGuy.vel;
        } 
        virtualGuy.linha = 1;
    }
    if(IsKeyDown(KEY_RIGHT)){
        virtualGuy.lado = 1;
        if(virtualGuy.posi.x < GetScreenWidth()/2 + 216){
            virtualGuy.posi.x += virtualGuy.vel;
        }
        virtualGuy.linha = 0;
    }

    if(IsKeyReleased(KEY_UP)){
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 2;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 3;
        }
    }
    if(IsKeyReleased(KEY_DOWN)){
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 2;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 3;
        }
    }
    if(IsKeyReleased(KEY_LEFT)){
        virtualGuy.linha = 3;
    }
    if(IsKeyReleased(KEY_RIGHT)){
        virtualGuy.linha = 2;
    }
}
void animaPersonagem(){
    static int frameCont  = 0;
    static int frameAtual  = 0;
    static int animaVel;
    static int numeroImagens;

    switch(virtualGuy.linha){
        case 0:
            animaVel = 12;
            numeroImagens = 11;
            break;
        case 1:
            animaVel = 12;
            numeroImagens = 11;
            break;
        case 2:
            animaVel = 10;
            numeroImagens = 10;
            break;
        case 3:
            animaVel = 10;
            numeroImagens = 10;
            break;
    }
    
    virtualGuy.frameRec.y = virtualGuy.frameRec.height*virtualGuy.linha;

    frameCont++;
    if (frameCont >= 60/animaVel){
        frameCont = 0;
        frameAtual++;
        if(frameAtual > numeroImagens){
            frameAtual = 0;
        }
        virtualGuy.frameRec.x = virtualGuy.frameRec.width*frameAtual;
    }
}

//sala base 
void inicializarSala(Texture2D cena){
    assets.image = cena;

    tetoLinha.width = assets.image.width/12;
    tetoLinha.height = assets.image.height/17;
    tetoLinha.x = tetoLinha.width*5;
    tetoLinha.y = tetoLinha.height*0;
    tetoColuna.width = assets.image.width/12;
    tetoColuna.height = assets.image.height/17;
    tetoColuna.x = tetoColuna.width*6;
    tetoColuna.y = tetoColuna.height*0;

    tetoQuina1.width = assets.image.width/12;
    tetoQuina1.height = assets.image.height/17;
    tetoQuina1.x = tetoQuina1.width*3;
    tetoQuina1.y = tetoQuina1.height*0;
    tetoQuina2.width = assets.image.width/12;
    tetoQuina2.height = assets.image.height/17;
    tetoQuina2.x = tetoQuina2.width*4;
    tetoQuina2.y = tetoQuina2.height*0;
    tetoQuina3.width = assets.image.width/12;
    tetoQuina3.height = assets.image.height/17;
    tetoQuina3.x = tetoQuina3.width*3;
    tetoQuina3.y = tetoQuina3.height*1;
    tetoQuina4.width = assets.image.width/12;
    tetoQuina4.height = assets.image.height/17;
    tetoQuina4.x = tetoQuina4.width*4;
    tetoQuina4.y = tetoQuina4.height*1;

    chao.width = assets.image.width/12;
    chao.height = assets.image.height/17;
    chao.x = chao.width*8;
    chao.y = chao.height*1;
    parede.width = assets.image.width/12;
    parede.height = assets.image.height/17;
    parede.x = parede.width*4;
    parede.y = parede.height*2;

    paredeComputadorDir.width = assets.image.width/12;
    paredeComputadorDir.height = assets.image.height/17;
    paredeComputadorDir.x = paredeComputadorDir.width*4;
    paredeComputadorDir.y = paredeComputadorDir.height*0;
    paredeComputadorEsc.width = assets.image.width/12;
    paredeComputadorEsc.height = assets.image.height/17;
    paredeComputadorEsc.x = paredeComputadorEsc.width*3;
    paredeComputadorEsc.y = paredeComputadorEsc.height*0;

}
void drawSala(){
    int comeX = (GetScreenWidth()-tetoLinha.width*17)/2;
    int comeY = tetoLinha.height*2;

    for(int i = 0; i < 17; i++)
        DrawTextureRec(assets.image, tetoLinha, (Vector2){comeX + tetoLinha.width*i, comeY}, RAYWHITE);
    for(int i = 0; i < 17; i++)
        DrawTextureRec(assets.image, tetoLinha, (Vector2){comeX + tetoLinha.width*i, comeY + tetoLinha.height*12}, RAYWHITE);
    for(int j = 0; j < 11; j++)
        DrawTextureRec(assets.image, tetoColuna, (Vector2){comeX - tetoColuna.width, comeY + tetoColuna.height*(j+1)}, RAYWHITE);
    for(int j = 0; j < 11; j++)
        DrawTextureRec(assets.image, tetoColuna, (Vector2){comeX + tetoColuna.width*17, comeY + tetoColuna.height*(j+1)}, RAYWHITE);
    DrawTextureRec(assets.image, tetoQuina1, (Vector2){comeX - tetoQuina1.width, comeY}, RAYWHITE);
    DrawTextureRec(assets.image, tetoQuina2, (Vector2){comeX + tetoQuina2.width*17, comeY}, RAYWHITE);
    DrawTextureRec(assets.image, tetoQuina3, (Vector2){comeX - tetoQuina3.width, comeY + tetoQuina3.height*12}, RAYWHITE);
    DrawTextureRec(assets.image, tetoQuina4, (Vector2){comeX + tetoQuina4.width*17, comeY + tetoQuina4.height*12}, RAYWHITE);

    for(int j = 0; j < 8; j++){
        for(int i = 0; i < 17; i++){
            DrawTextureRec(assets.image, chao, (Vector2){comeX + chao.width*i, comeY + chao.height*(j + 4)}, RAYWHITE);
        }
    }
    for(int j = 0; j < 3; j++){
        for(int i = 0; i < 17; i++){
            DrawTextureRec(assets.image, parede, (Vector2){comeX + parede.width*i, comeY + parede.height*(j + 1)}, RAYWHITE);
        }
    }
    DrawTextureRec(assets.image, paredeComputadorEsc, (Vector2){comeX + paredeComputadorEsc.width*7, comeY + paredeComputadorEsc.height}, RAYWHITE);
    DrawTextureRec(assets.image, paredeComputadorDir, (Vector2){comeX + paredeComputadorDir.width*9, comeY + paredeComputadorDir.height}, RAYWHITE);
}
