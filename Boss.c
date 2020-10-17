#include <raylib.h>

//estruturas
typedef struct{
    Texture2D imagem;
    Rectangle frameRec;
    Vector2 posi;
    int vel;
    int lado;
    int linha;
    Rectangle colisao;
    int dano;
}hero;
typedef struct{
    Texture2D imagem;
    Rectangle frameRec;
    Vector2 posi;
    Rectangle colisao;
}chefe;
typedef struct{
    Texture2D imagem;
    Rectangle frameRec;
    Vector2 posi;
    int atirar;
    int vel;
    int tiroVel;
    Rectangle colisao;
}laser;
typedef struct{
    Texture2D imagemArma;
    Texture2D imagemTiro;
    Rectangle frameRec;
    Vector2 posi;
    int explodir;
}canhao;
typedef struct{
    Texture2D layer1;
    Texture2D layer2;
    Texture2D layer3;
    Texture2D layer4;
    Texture2D layer5;
    Texture2D layer6;
    Texture2D layer7;
    Texture2D layer8;
}Fundo;
typedef struct{
    Texture2D image;
}Imagem;

//globais
hero virtualGuy;
chefe computador;
laser armaLaserDir;
laser armaLaserEsc;
canhao armaCanhao;
Fundo fundo;

int lasersAtivos = 1;
int canhaoAtiva = 0;

//partes da sala base
Fundo Back;
Rectangle arvore1;
Rectangle arvore2;
Rectangle plataformaChao;
Rectangle plataformaAr;
Rectangle placa;

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
//cenario
void drawFundo();
void animaFundo();
//sala base do jogo
void inicializarSala(Texture2D cena);
void drawSala();
//laser movimentos
void drawLaser();
void animaLaser();
void moveLaser();
//canhao
void animaCanhao();
void atiraCanhao();
//personagem
void atualizarPersonagem();
void animaPersonagem();
//rotina da luta
void rotinaJogo();

void inicializarNivelBoss();
void atualizarNivelBoss();
void drawNivelBoss();


int BOSS(){
    int resultado4 = 0;

    Texture2D Industri = LoadTexture("assets/Boss/Industrial.png");
    Industri.width = Industri.width*2;
    Industri.height = Industri.height*2;
    Texture2D Computador = LoadTexture("assets/Boss/PC.png");
    Computador.width = Computador.width*6.4;
    Computador.height = Computador.height*6.4;
    computador.imagem = Computador;

    inicializarSala(Industri);
    inicializarNivelBoss();

    while(resultado4 == 0){
        if(IsKeyPressed(KEY_ZERO)){
            resultado4 = 1;
        }
        if(IsKeyPressed(KEY_SPACE)){
        }

        atualizarNivelBoss();
        atualizarPersonagem();

        drawNivelBoss();
    }

    UnloadTexture(Industri);
    UnloadTexture(virtualGuy.imagem);
    UnloadTexture(Computador);
    UnloadTexture(armaLaserDir.imagem);
    UnloadTexture(armaCanhao.imagemArma);
    UnloadTexture(armaCanhao.imagemTiro);
    UnloadTexture(Back.layer1);
    UnloadTexture(Back.layer2);
    UnloadTexture(Back.layer3);
    UnloadTexture(Back.layer4);
    UnloadTexture(Back.layer5);
    UnloadTexture(Back.layer6);
    UnloadTexture(Back.layer7);
    UnloadTexture(Back.layer8);

    return resultado4;
}


void inicializarNivelBoss(){
    virtualGuy.imagem = LoadTexture("assets/full.png");
    virtualGuy.imagem.width = virtualGuy.imagem.width*2;
    virtualGuy.imagem.height = virtualGuy.imagem.height*2;
    virtualGuy.frameRec.width = virtualGuy.imagem.width/12;
    virtualGuy.frameRec.height = virtualGuy.imagem.height/14;
    virtualGuy.frameRec.x = virtualGuy.frameRec.width*0;
    virtualGuy.frameRec.y = virtualGuy.frameRec.height*2;
    virtualGuy.posi.x = GetScreenWidth()/2;
    virtualGuy.posi.y = GetScreenHeight()/2;
    virtualGuy.vel = 3;
    virtualGuy.lado = 1;
    virtualGuy.dano = 0;
    virtualGuy.linha = 2;
    virtualGuy.colisao.x = virtualGuy.posi.x;
    virtualGuy.colisao.y = virtualGuy.posi.y;
    virtualGuy.colisao.width = virtualGuy.frameRec.width-32;
    virtualGuy.colisao.height = virtualGuy.frameRec.height-40;

    computador.frameRec.width = computador.imagem.width/5;
    computador.frameRec.height = computador.imagem.height/3;
    computador.frameRec.x = computador.frameRec.width*0;
    computador.frameRec.y = computador.frameRec.height*2;
    computador.posi.x = (GetScreenWidth()/2) - chao.width*1.5;
    computador.posi.y = chao.height*6;
    computador.colisao.width = computador.frameRec.width - 32;
    computador.colisao.height = computador.frameRec.height/1.5;
    computador.colisao.x = computador.posi.x + 16;
    computador.colisao.y = computador.posi.y;

    armaLaserDir.imagem = LoadTexture("assets/Boss/laser.png");
    armaLaserDir.imagem.width = armaLaserDir.imagem.width*4;
    armaLaserDir.imagem.height = armaLaserDir.imagem.height*4;
    armaLaserDir.frameRec.width = armaLaserDir.imagem.width/3;
    armaLaserDir.frameRec.height = armaLaserDir.imagem.height/32;
    armaLaserDir.frameRec.x = armaLaserDir.frameRec.width*2;
    armaLaserDir.frameRec.y = armaLaserDir.frameRec.height*16;
    armaLaserDir.posi.x = GetScreenWidth()/2 + 333;
    armaLaserDir.posi.y = chao.height*8;
    armaLaserDir.atirar = 0;
    armaLaserDir.vel = 2;
    armaLaserDir.tiroVel = 18;
    armaLaserDir.colisao.width = chao.width*17;
    armaLaserDir.colisao.height = 32;
    armaLaserDir.colisao.x = (GetScreenWidth()-tetoLinha.width*17)/2;
    armaLaserEsc.frameRec.width = armaLaserDir.imagem.width/3;
    armaLaserEsc.frameRec.height = armaLaserDir.imagem.height/32;
    armaLaserEsc.frameRec.x = 0;
    armaLaserEsc.frameRec.y = armaLaserDir.frameRec.height*0;
    armaLaserEsc.posi.x = (GetScreenWidth()-tetoLinha.width*25)/2;
    armaLaserEsc.posi.y = chao.height*17 + 4;
    armaLaserEsc.atirar = 0;
    armaLaserEsc.vel = 1.8;
    armaLaserEsc.tiroVel = 20;
    armaLaserEsc.colisao.width = chao.width*17;
    armaLaserEsc.colisao.height = 32;
    armaLaserEsc.colisao.x = (GetScreenWidth()-tetoLinha.width*17)/2;

    armaCanhao.imagemArma = LoadTexture("assets/Boss/canhao.png");
    armaCanhao.imagemArma.width = armaCanhao.imagemArma.width*2;
    armaCanhao.imagemArma.height = armaCanhao.imagemArma.height*2;
    armaCanhao.imagemTiro = LoadTexture("assets/Boss/tudo.png");
    armaCanhao.imagemTiro.width = armaCanhao.imagemTiro.width*2;
    armaCanhao.imagemTiro.height = armaCanhao.imagemTiro.height*2;
    armaCanhao.frameRec.width = armaCanhao.imagemTiro.width/11;
    armaCanhao.frameRec.height = armaCanhao.imagemTiro.height;
    armaCanhao.frameRec.x = armaCanhao.frameRec.height*0;
    armaCanhao.frameRec.y = 0;
    armaCanhao.posi.x = GetScreenWidth()/2 - (armaCanhao.frameRec.width/2);
    armaCanhao.posi.y = GetScreenHeight()/2 + (chao.height*4.8);

    Back.layer1 = LoadTexture("assets/Boss/Clouds_01.png");
    Back.layer1.width = GetScreenWidth();
    Back.layer1.height = Back.layer1.height*3;
    Back.layer2 = LoadTexture("assets/Boss/Clouds_02.png");
    Back.layer2.width = GetScreenWidth();
    Back.layer2.height = Back.layer2.height*3;
    Back.layer3 = LoadTexture("assets/Boss/Grass.png");
    Back.layer3.width = Back.layer3.width*2;
    Back.layer3.height = Back.layer3.height*2;
    Back.layer7 = LoadTexture("assets/Boss/Placa1.png");
    Back.layer7.width = Back.layer7.width*3;
    Back.layer7.height = Back.layer7.height*3;
    placa.width = Back.layer7.width/8;
    placa.height = Back.layer7.height;
    placa.x = placa.width*0;
    placa.y = 0;

    Back.layer4 = LoadTexture("assets/Boss/tree.png");
    Back.layer4.width = Back.layer4.width*3;
    Back.layer4.height = Back.layer4.height*3;
    arvore1.width = Back.layer4.width/8;
    arvore1.height = Back.layer4.height;
    arvore1.x = arvore1.width*0;
    arvore1.y = 0;
    Back.layer8 = LoadTexture("assets/Boss/tree2.png");
    Back.layer8.width = Back.layer8.width*3;
    Back.layer8.height = Back.layer8.height*3;
    arvore2.width = Back.layer8.width/8;
    arvore2.height = Back.layer8.height;
    arvore2.x = arvore2.width*0;
    arvore2.y = 0;

    Back.layer5 = LoadTexture("assets/Boss/PlataformaChao.png");
    Back.layer5.width = Back.layer5.width*3;
    Back.layer5.height = Back.layer5.height*3;
    plataformaChao.width = Back.layer5.width/8;
    plataformaChao.height = Back.layer5.height;
    plataformaChao.x = plataformaChao.width*0;
    plataformaChao.y = 0;
    Back.layer6 = LoadTexture("assets/Boss/PlataformaAr.png");
    Back.layer6.width = Back.layer6.width*3;
    Back.layer6.height = Back.layer6.height*3;
    plataformaAr.width = Back.layer6.width/8;
    plataformaAr.height = Back.layer6.height;
    plataformaAr.x = plataformaAr.width*0;
    plataformaAr.y = 0;
}
void atualizarNivelBoss(){
    animaFundo();

    animaPersonagem();

    animaLaser();
    moveLaser();

    animaCanhao();
    atiraCanhao();
    
    rotinaJogo();

    if(CheckCollisionCircleRec((Vector2){armaCanhao.posi.x + (armaCanhao.frameRec.width/2), armaCanhao.posi.y + (armaCanhao.frameRec.height/2)}, 5.0, computador.colisao)){
        computador.frameRec.x = computador.frameRec.width*4;
    }
    
    static int frameCont  = 0;
    static int frameAtual  = 0;
    frameCont++;
    if (frameCont >= 60/2){
        frameCont = 0;
        frameAtual++;
        if(frameAtual > 1){
            frameAtual = 0;
        }
        computador.frameRec.x = computador.frameRec.width*frameAtual;
    }
}
void drawNivelBoss(){
    BeginDrawing();
        drawFundo();

        drawSala();
        DrawTextureRec(computador.imagem, computador.frameRec, computador.posi, RAYWHITE);
        DrawTextureRec(virtualGuy.imagem, virtualGuy.frameRec, virtualGuy.posi, RAYWHITE);
        drawLaser();
        DrawTexture(armaCanhao.imagemArma, GetScreenWidth()/2 - (armaCanhao.imagemArma.width/2), GetScreenHeight()/2 + (chao.height*7), RAYWHITE);
        DrawTextureRec(armaCanhao.imagemTiro, armaCanhao.frameRec, armaCanhao.posi, GREEN);
        
        //DrawRectangleLines(virtualGuy.colisao.x, virtualGuy.colisao.y, virtualGuy.colisao.width, virtualGuy.colisao.height, BLUE);
        //DrawCircleLines(armaCanhao.posi.x + (armaCanhao.frameRec.width/2), armaCanhao.posi.y + (armaCanhao.frameRec.height/2), 10.0, RED);
    EndDrawing();
}

//rotina do jogo
void rotinaJogo(){
    static int frameCont  = 0;
    static int frameAtual  = 0;

    if(canhaoAtiva == 0){
        frameCont++;
        if (frameCont >= 60){
            frameCont = 0;
            frameAtual++;
            if(frameAtual > 2){
                frameAtual = 0;
                armaCanhao.frameRec.x = armaCanhao.frameRec.height*0;
                armaCanhao.frameRec.y = 0;
                armaCanhao.posi.x = GetScreenWidth()/2 - (armaCanhao.frameRec.width/2);
                armaCanhao.posi.y = GetScreenHeight()/2 + (chao.height*4.8);
            }
            if(frameAtual == 2){
                canhaoAtiva = 1;
            }
            armaCanhao.frameRec.x = armaCanhao.frameRec.width*frameAtual;
        }
    }
}

//personagem movimentos
void atualizarPersonagem(){
    if(virtualGuy.dano <= 0){ 
        virtualGuy.colisao.x = virtualGuy.posi.x + 18;
        virtualGuy.colisao.y = virtualGuy.posi.y + 18;
    }
    if(virtualGuy.dano > 0){ 
        virtualGuy.colisao.x = 0;
        virtualGuy.colisao.y = 0;
    }

    if(virtualGuy.dano >= 0)
        virtualGuy.dano--;

    if(IsKeyDown(KEY_UP)){
        if(virtualGuy.posi.y > chao.height*8){
            virtualGuy.posi.y -= virtualGuy.vel;
        }
        if(virtualGuy.dano <= 0){     
            if(virtualGuy.lado == 1){
                virtualGuy.linha = 0;
            }
            if(virtualGuy.lado == -1){
                virtualGuy.linha = 1;
            }
        }
    }
    if(IsKeyDown(KEY_DOWN)){
        if(virtualGuy.posi.y < chao.height*17){
            virtualGuy.posi.y += virtualGuy.vel;
        }
        if(virtualGuy.dano <= 0){ 
            if(virtualGuy.lado == 1){
                virtualGuy.linha = 0;
            }
            if(virtualGuy.lado == -1){
                virtualGuy.linha = 1;
            }
        }
    }
    if(IsKeyDown(KEY_LEFT)){
        virtualGuy.lado = -1;
        if(virtualGuy.posi.x > (GetScreenWidth()-tetoLinha.width*25)/2 -10){
            virtualGuy.posi.x -= virtualGuy.vel;
        } 
        if(virtualGuy.dano <= 0){     
            virtualGuy.linha = 1;
        }
        if(virtualGuy.dano > 0){     
            virtualGuy.linha = 5;
        }
    }
    if(IsKeyDown(KEY_RIGHT)){
        virtualGuy.lado = 1;
        if(virtualGuy.posi.x < GetScreenWidth()/2 + 344){
            virtualGuy.posi.x += virtualGuy.vel;
        }
        if(virtualGuy.dano <= 0){ 
            virtualGuy.linha = 0;
        }
        if(virtualGuy.dano > 0){     
            virtualGuy.linha = 4;
        }
    }

    if(IsKeyReleased(KEY_UP)){
        if(virtualGuy.dano <= 0){     
            if(virtualGuy.lado == 1){
                virtualGuy.linha = 2;
            }
            if(virtualGuy.lado == -1){
                virtualGuy.linha = 3;
            }
        }
    }
    if(IsKeyReleased(KEY_DOWN)){
        if(virtualGuy.dano <= 0){    
            if(virtualGuy.lado == 1){
                virtualGuy.linha = 2;
            }
            if(virtualGuy.lado == -1){
                virtualGuy.linha = 3;
            }
        }
    }
    if(IsKeyReleased(KEY_LEFT)){
        if(virtualGuy.dano <= 0){ 
            virtualGuy.linha = 3;
        }
    }
    if(IsKeyReleased(KEY_RIGHT)){
        if(virtualGuy.dano <= 0){     
            virtualGuy.linha = 2;
        }
    }

    if(virtualGuy.dano == 0){
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 2;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 3;
        }
    }

    if(CheckCollisionRecs(virtualGuy.colisao, armaLaserEsc.colisao)){
        virtualGuy.dano = 60;
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 4;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 5;
        }
    }
    if(CheckCollisionRecs(virtualGuy.colisao, armaLaserDir.colisao)){
        virtualGuy.dano = 60;
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 4;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 5;
        }
    }
    if(CheckCollisionCircleRec((Vector2){armaCanhao.posi.x + (armaCanhao.frameRec.width/2), armaCanhao.posi.y + (armaCanhao.frameRec.height/2)}, 10.0, virtualGuy.colisao)){
        virtualGuy.dano = 60;
        armaCanhao.explodir = 1;
        canhaoAtiva = 0;
        if(virtualGuy.lado == 1){
            virtualGuy.linha = 4;
        }
        if(virtualGuy.lado == -1){
            virtualGuy.linha = 5;
        }
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
        case 4:
            animaVel = 20;
            numeroImagens = 6;
            break;
        case 5:
            animaVel = 20;
            numeroImagens = 6;
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

//laser funcionar
void drawLaser(){
    Rectangle armaLaserDirMeio;
    armaLaserDirMeio.width = armaLaserDir.frameRec.width;
    armaLaserDirMeio.height = armaLaserDir.frameRec.height;
    armaLaserDirMeio.x = armaLaserDir.frameRec.width*1;
    armaLaserDirMeio.y = armaLaserDir.frameRec.y;
    Rectangle armaLaserDirFim;
    armaLaserDirFim.width = armaLaserEsc.frameRec.width;
    armaLaserDirFim.height = armaLaserEsc.frameRec.height;
    armaLaserDirFim.x = armaLaserDir.frameRec.width*0;
    armaLaserDirFim.y = armaLaserDir.frameRec.y;
    Rectangle armaLaserEscMeio;
    armaLaserEscMeio.width = armaLaserEsc.frameRec.width;
    armaLaserEscMeio.height = armaLaserEsc.frameRec.height;
    armaLaserEscMeio.x = armaLaserEsc.frameRec.width*1;
    armaLaserEscMeio.y = armaLaserEsc.frameRec.y;
    Rectangle armaLaserEscFim;
    armaLaserEscFim.width = armaLaserEsc.frameRec.width;
    armaLaserEscFim.height = armaLaserEsc.frameRec.height;
    armaLaserEscFim.x = armaLaserEsc.frameRec.width*2;
    armaLaserEscFim.y = armaLaserEsc.frameRec.y;

    DrawTextureRec(armaLaserDir.imagem, armaLaserEsc.frameRec, armaLaserEsc.posi, RAYWHITE);
    for(int i = 0; i < 10; i++){
        DrawTextureRec(armaLaserDir.imagem, armaLaserEscMeio, (Vector2){armaLaserEsc.posi.x + armaLaserEsc.frameRec.width*(i+1), armaLaserEsc.posi.y}, RAYWHITE);
    }
    DrawTextureRec(armaLaserDir.imagem, armaLaserEscFim, (Vector2){armaLaserEsc.posi.x + armaLaserEsc.frameRec.width*10.7, armaLaserEsc.posi.y}, RAYWHITE);

    DrawTextureRec(armaLaserDir.imagem, armaLaserDir.frameRec, armaLaserDir.posi, RAYWHITE);
    for(int i = 0; i < 10; i++){
        DrawTextureRec(armaLaserDir.imagem, armaLaserDirMeio, (Vector2){armaLaserDir.posi.x - armaLaserDir.frameRec.width*(i+1), armaLaserDir.posi.y}, RAYWHITE);
    }
    DrawTextureRec(armaLaserDir.imagem, armaLaserDirFim, (Vector2){armaLaserDir.posi.x - armaLaserDir.frameRec.width*10.7, armaLaserDir.posi.y}, RAYWHITE);
}
void animaLaser(){
    static int frameAtualEsc = 0;
    static int frameContEsc = 0;
    static int frameAtualDir = 16;
    static int frameContDir = 0;

    if(armaLaserEsc.atirar != 0){
        frameContEsc++;
        if(frameContEsc >= 60/10){
            frameContEsc = 0;
            frameAtualEsc++;
            if(frameAtualEsc == 5){
                armaLaserEsc.colisao.y = armaLaserEsc.posi.y + (armaLaserEsc.frameRec.height/4);
            }
            if(frameAtualEsc == 11){
                armaLaserEsc.colisao.y = 0;
            }
            if(frameAtualEsc > 15){
                frameAtualEsc = 0;
                armaLaserEsc.atirar = 0;
            }
            armaLaserEsc.frameRec.y = armaLaserEsc.frameRec.height*frameAtualEsc;
        }
    }
    if(armaLaserDir.atirar != 0){
        frameContDir++;
        if(frameContDir >= 60/10){
            frameContDir = 0;
            frameAtualDir++;
            if(frameAtualDir == 21){
                armaLaserDir.colisao.y = armaLaserDir.posi.y + (armaLaserDir.frameRec.height/4);
            }
            if(frameAtualDir == 27){
                armaLaserDir.colisao.y = 0;
            }
            if(frameAtualDir > 31){
                frameAtualDir = 16;
                armaLaserDir.atirar = 0;
            }
            armaLaserDir.frameRec.y = armaLaserDir.frameRec.height*frameAtualDir;
        }
    }
}
void moveLaser(){
    static int directionLaserDir = 1;
    static int directionLaserEsc = -1;
    static int pararLaserDir = 0;
    static int pararLaserEsc = 0;
    static int esperarDir = 0;
    static int esperarEsc = 0;
    static int descansarDir = 0;
    static int descansarEsc = 0;

    if(lasersAtivos != 0){
        descansarDir--;
        if(descansarDir <=0){
            if(armaLaserDir.posi.y - (armaLaserDir.frameRec.height/2) > virtualGuy.posi.y-(2* virtualGuy.frameRec.height/4) &&
               armaLaserDir.posi.y - (armaLaserDir.frameRec.height/2)< virtualGuy.posi.y - (virtualGuy.frameRec.height/3)){
                pararLaserDir = 1;
                descansarDir = 100;
            }
        }
        if(pararLaserDir == 1){
            esperarDir++;
            if(esperarDir == armaLaserDir.tiroVel){
                armaLaserDir.atirar = 1;
            }
            if(esperarDir == armaLaserDir.tiroVel + 120){
                pararLaserDir = 0;
                esperarDir = 0;
            }
        }

        descansarEsc--;
        if(descansarEsc <=0){
            if(armaLaserEsc.posi.y - (armaLaserEsc.frameRec.height/2) > virtualGuy.posi.y-(2* virtualGuy.frameRec.height/4) &&
               armaLaserEsc.posi.y - (armaLaserEsc.frameRec.height/2) < virtualGuy.posi.y - (virtualGuy.frameRec.height/3)){
                pararLaserEsc = 1;
                descansarEsc = 100;
            }
        }
        if(pararLaserEsc == 1){
            esperarEsc++;
            if(esperarEsc == armaLaserEsc.tiroVel){
                armaLaserEsc.atirar = 1;
            }
            if(esperarEsc == armaLaserEsc.tiroVel+ 120){
                pararLaserEsc = 0;
                esperarEsc = 0;
            }
        }
    }

    if(directionLaserDir == 1){
        if(pararLaserDir == 0){
            armaLaserDir.posi.y += armaLaserDir.vel;
        }

        if(armaLaserDir.posi.y >= chao.height*17 + 4){
            directionLaserDir = -1;
        }
    }
    if(directionLaserDir == -1){
        if(pararLaserDir == 0){
            armaLaserDir.posi.y -= armaLaserDir.vel;
        }
        if(armaLaserDir.posi.y <= chao.height*8){
            directionLaserDir = 1;
        }
    }

    if(directionLaserEsc == 1){
        if(pararLaserEsc == 0){
            armaLaserEsc.posi.y += armaLaserEsc.vel;
        }

        if(armaLaserEsc.posi.y >= chao.height*17 + 4){
            directionLaserEsc = -1;
        }
    }
    if(directionLaserEsc == -1){
        if(pararLaserEsc == 0){
            armaLaserEsc.posi.y -= armaLaserEsc.vel;
        }
        if(armaLaserEsc.posi.y <= chao.height*8){
            directionLaserEsc = 1;
        }
    }
}

//canhao funcionar
void animaCanhao(){
    static int frameCont  = 0;
    static int frameAtual  = 3;

    if(armaCanhao.explodir != 0){
        frameCont++;
        if (frameCont >= 60/10){
            frameCont = 0;
            frameAtual++;
            if(frameAtual > 10){
                frameAtual = 2;
                armaCanhao.posi.x = GetScreenWidth()/2 - (armaCanhao.frameRec.width/2);
                armaCanhao.posi.y = GetScreenHeight()*2;
                armaCanhao.explodir = 0;
            }
            armaCanhao.frameRec.x = armaCanhao.frameRec.width*frameAtual;
        }
    }
}
void atiraCanhao(){
    static float x, y;
    static float dx, dy;
    static float A, B;
    int tempo = 30;


    if(canhaoAtiva == 0){
        x = virtualGuy.posi.x;
        y = virtualGuy.posi.y;
        dx = x - armaCanhao.posi.x;
        dy = y - armaCanhao.posi.y;
        A = dx/tempo;
        B = dy/tempo;
    }
    else{
        armaCanhao.posi.x += A;
        armaCanhao.posi.y += B;

        
        if(armaCanhao.posi.x > (GetScreenWidth()/2) + chao.width*11){
            canhaoAtiva = 0;
            armaCanhao.explodir = 1;
        }
        if(armaCanhao.posi.x < (GetScreenWidth()/2) - chao.width*14){
            canhaoAtiva = 0;
            armaCanhao.explodir = 1;
        }
        if(armaCanhao.posi.y < chao.height*6.5){
            canhaoAtiva = 0;
            armaCanhao.explodir = 1;
        }
        if(armaCanhao.posi.y > chao.height*17){
            canhaoAtiva = 0;
            armaCanhao.explodir = 1;
        } 
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
    int comeX = (GetScreenWidth()-tetoLinha.width*25)/2;
    int comeY = tetoLinha.height*4;

    for(int i = 0; i < 25; i++)
        DrawTextureRec(assets.image, tetoLinha, (Vector2){comeX + tetoLinha.width*i, comeY}, RAYWHITE);
    for(int i = 0; i < 25; i++)
        DrawTextureRec(assets.image, tetoLinha, (Vector2){comeX + tetoLinha.width*i, comeY + tetoLinha.height*15}, RAYWHITE);
    for(int j = 0; j < 14; j++)
        DrawTextureRec(assets.image, tetoColuna, (Vector2){comeX - tetoColuna.width, comeY + tetoColuna.height*(j+1)}, RAYWHITE);
    for(int j = 0; j < 14; j++)
        DrawTextureRec(assets.image, tetoColuna, (Vector2){comeX + tetoColuna.width*25, comeY + tetoColuna.height*(j+1)}, RAYWHITE);
    
    DrawTextureRec(assets.image, tetoQuina1, (Vector2){comeX - tetoQuina1.width, comeY}, RAYWHITE);
    DrawTextureRec(assets.image, tetoQuina2, (Vector2){comeX + tetoQuina2.width*25, comeY}, RAYWHITE);
    DrawTextureRec(assets.image, tetoQuina3, (Vector2){comeX - tetoQuina3.width, comeY + tetoQuina3.height*15}, RAYWHITE);
    DrawTextureRec(assets.image, tetoQuina4, (Vector2){comeX + tetoQuina4.width*25, comeY + tetoQuina4.height*15}, RAYWHITE);

    for(int j = 0; j < 10; j++){
        for(int i = 0; i < 25; i++){
            DrawTextureRec(assets.image, chao, (Vector2){comeX + chao.width*i, comeY + chao.height*(j + 5)}, RAYWHITE);
        }
    }
    for(int j = 0; j < 4; j++){
        for(int i = 0; i < 25; i++){
            DrawTextureRec(assets.image, parede, (Vector2){comeX + parede.width*i, comeY + parede.height*(j + 1)}, RAYWHITE);
        }
    }

    DrawTextureRec(assets.image, paredeComputadorEsc, (Vector2){comeX + paredeComputadorEsc.width*11, comeY + paredeComputadorEsc.height*2}, RAYWHITE);
    DrawTextureRec(assets.image, paredeComputadorDir, (Vector2){comeX + paredeComputadorDir.width*13, comeY + paredeComputadorDir.height*2}, RAYWHITE);
}

//Background
void drawFundo(){
    ClearBackground((Color){102, 191, 205});
    DrawRectangle(0, GetScreenHeight()/2 -chao.height*2, GetScreenWidth(), GetScreenHeight(), DARKGRAY);

    DrawTexture(Back.layer1, 0, 68, RAYWHITE);
    DrawTexture(Back.layer2, 0, 35, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*0, 0, 64, 64}, (Vector2){50, 300}, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*1, 0, 64, 64}, (Vector2){120, 500}, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*2, 0, 64, 64}, (Vector2){10, 640}, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*3, 0, 64, 64}, (Vector2){620, 680}, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*4, 0, 64, 64}, (Vector2){1120, 400}, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*5, 0, 64, 64}, (Vector2){820, 620}, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*6, 0, 64, 64}, (Vector2){1220, 350}, RAYWHITE);
    DrawTextureRec(Back.layer3, (Rectangle){64*7, 0, 64, 64}, (Vector2){1070, 550}, RAYWHITE);

    DrawTextureRec(Back.layer5, plataformaChao, (Vector2){1050, 100}, RAYWHITE);
    DrawTextureRec(Back.layer6, plataformaAr, (Vector2){50, -10}, RAYWHITE);

    DrawTextureRec(Back.layer7, placa, (Vector2){1050, 52}, RAYWHITE);

    DrawTextureRec(Back.layer4, arvore1, (Vector2){100, 170}, RAYWHITE);
    DrawTextureRec(Back.layer8, arvore2, (Vector2){20, 400}, RAYWHITE);
    DrawTextureRec(Back.layer4, arvore1, (Vector2){1200, 500}, RAYWHITE);
}
void animaFundo(){
    static int frameCont1  = 0;
    static int frameAtual1  = 0;
    frameCont1++;
    if (frameCont1 >= 60/5){
        frameCont1 = 0;
        frameAtual1++;
        if(frameAtual1 > 7){
            frameAtual1 = 0;
        }
        arvore1.x = arvore1.width*frameAtual1;
    }
    static int frameCont2  = 0;
    static int frameAtual2  = 0;
    frameCont2++;
    if (frameCont2 >= 60/5){
        frameCont2 = 0;
        frameAtual2++;
        if(frameAtual2 > 7){
            frameAtual2 = 0;
        }
        arvore2.x = arvore2.width*frameAtual1;
    }
    static int frameContAviso  = 0;
    static int frameAtualAviso  = 0;
    frameContAviso++;
    if (frameContAviso >= 60/20){
        frameContAviso = 0;
        frameAtualAviso++;
        if(frameAtualAviso > 7){
            frameAtualAviso = 0;
        }
        placa.x = placa.width*frameAtual1;
    }


    static int cont1  = 0;
    static int frameCont3  = 0;
    static int frameAtual3  = 0;
    frameCont3++;
    if (frameCont3 >= 60/10){
        frameCont3 = 0;

        if(cont1 == 0){
            frameAtual3++;
            if(frameAtual3 > 6){
                cont1 = 1;
            }
        }
        else if(cont1 == 100){
            frameAtual3--;
            if(frameAtual3 == 0){
                cont1 = -100;
            } 
        }
        else{
            cont1++;
        }
        plataformaChao.x = plataformaChao.width*frameAtual3;
    }
    static int cont2  = 0;
    static int frameCont4  = 0;
    static int frameAtual4  = 0;
    frameCont4++;
    if (frameCont4 >= 60/10){
        frameCont4 = 0;

        if(cont2 == 0){
            frameAtual4++;
            if(frameAtual4 > 6){
                cont2 = 1;
            }
        }
        else if(cont2 == 200){
            frameAtual4--;
            if(frameAtual4 == 0){
                cont2 = -200;
            } 
        }
        else{
            cont2++;
        }
        plataformaAr.x = plataformaAr.width*frameAtual3;
    }
}