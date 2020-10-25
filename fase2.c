#include <raylib.h>
//#define PHYSAC_IMPLEMENTATION
//#define PHYSAC_NO_THREADS
#include "physac.h"
#define VELOCITY    0.5f
const int larguraDaTela = 1280;
const int AlturaDaTela = 720;

typedef struct
{
    Texture2D txt;
    Rectangle rect;
    Vector2 position;
    PhysicsBody body;
    bool direita;
    

}Player;


typedef struct Enviroment
{
    Texture2D txt;
    Rectangle rect;
    PhysicsBody body;
    bool isMoving;
    int movementspeed;

}Envment;

Envment base;
Envment tiles[8];

Texture2D tilesTxt;
Rectangle tilesRect;

Texture2D portalFase2;
Rectangle portalrect;

Texture2D backgroundFase2;
Rectangle backgroundFase2rect;

Rectangle cristal;
Rectangle cristalrect;
Texture2D cristaltxt;
    
Camera2D cameraJumper;

Music musicfase2;
Sound jumpfase2;
    float paralaxFase2;
    int frames;
    int playerTxtFrames;
    int framecontPlayer ;
    int framespeedPlayer ;

    int portalFrames;
    int framecontportal;
    int portalmode;
    
    int framecontCristal;
    
    int i;
    int rando;
    int randwidth;
    int singleTile;
    int segundos;

    bool repetido;
    bool lost;
    bool victoryFase2;
    bool gotCristal;
    bool portalativo;
    bool endgame;
    bool replayfase2;
    bool endofmap;
    bool movingPiece;
    Vector2 cristal2position;
Player player;


void initfase2(void)
{

SetConfigFlags(FLAG_MSAA_4X_HINT);
InitPhysics();
SetPhysicsGravity(0,9.81f);

    repetido = false;
     lost = false;
     victoryFase2 = false;
     gotCristal = false;
     portalativo = true;
     endgame =false;
     replayfase2 = false;
    endofmap = false;
     paralaxFase2=0.5;
     frames=0;
     playerTxtFrames = 0;
     framecontPlayer = 0;
     framespeedPlayer = 9;

     portalFrames=0;
     framecontportal=0;
     portalmode=0;

    framecontCristal = 0;

    musicfase2 = LoadMusicStream("sounds/Fase2/fase2M.mp3");
    SetMusicVolume(musicfase2,0.007f);
    PlayMusicStream(musicfase2);
    jumpfase2 = LoadSound("sounds/Fase2/jump.wav");
    SetSoundVolume(jumpfase2,0.07f);
    
     i = 0;
     rando = 0;
     randwidth = 0;
     singleTile = 50;
     segundos = 0;

     cristaltxt = LoadTexture("assets/cristalVermelho.png");
     cristaltxt.width = 200;
     cristaltxt.height = 50;
     cristal = (Rectangle){0,0,cristaltxt.width/4,cristaltxt.height};
     cristalrect = (Rectangle){0,0,cristaltxt.width/4,cristaltxt.height};

//TEXTURA DO PLAYER

player.txt = LoadTexture("assets/Fase2/half.png");
player.txt.width =600;
player.txt.height =240;
player.rect = (Rectangle) {0,0,player.txt.width/12,player.txt.height/4};
player.position = (Vector2){0,0};
player.body = CreatePhysicsBodyRectangle((Vector2){ 0,0 }, player.rect.width,player.rect.height, 1);
player.body->freezeOrient = true;
player.body->velocity.x = 0;

//TEXTURA DAS PLATAFORMAS

tilesTxt  = LoadTexture("assets/Fase2/castleTiles.png");
tilesTxt.width = 525;
tilesTxt.height = 375;
tilesRect = (Rectangle){0,0,2*tilesTxt.width/21,2*tilesTxt.height/15};
tilesRect.x = 2*tilesTxt.width/21;
tilesRect.y = 2*tilesTxt.height/15;



portalFase2 = LoadTexture("assets/Portal.png");
portalFase2.height *=3;
portalFase2.width *=4;
portalrect = (Rectangle){0,0,portalFase2.width/8,portalFase2.height/3};

backgroundFase2 = LoadTexture("assets/Fase2/window.png");
backgroundFase2.width *=10 ;
backgroundFase2.height *=10;
backgroundFase2rect = (Rectangle) {0,0,backgroundFase2.width,backgroundFase2.height};

PhysicsBody base = CreatePhysicsBodyRectangle((Vector2){ 0, 50 }, 450, 100, 10);
base->enabled = false;

    cameraJumper.target = (Vector2) {0,-100};
    cameraJumper.zoom = 0.7f;
    cameraJumper.offset = (Vector2) {larguraDaTela/2,AlturaDaTela/2};
    
   
}
    
 void Refresh(){
     
    //ATUALIZANDO FRAMES PARA SPRITES
    frames++;
    if(victoryFase2){
        if(frames%15==0)framecontCristal++;
        if(framecontCristal>=4)framecontCristal=0;
        cristal.x = framecontCristal*cristaltxt.width/4;
    }
    if(frames%60==0)segundos++;

     if (segundos>4) portalativo = false;
     if(gotCristal){
            portalFrames++;
            if(portalFrames>=60/8){
                portalFrames=0;
                framecontportal++;
                    if(framecontportal>=8){
                portalmode++;
                framecontportal=0;
            }
            portalrect.x = framecontportal*portalFase2.width/8;
            if(portalmode>=2){
                portalrect.y =  2*portalFase2.height/3;
            }   
            if(portalmode==3)endgame = true;
        }
     }
         

 if(portalativo == true){
            portalFrames++;
            if(portalFrames>=60/8){
                portalFrames=0;
                framecontportal++;
                    if(framecontportal>=8){
                portalmode++;
                framecontportal=0;
            }
            portalrect.x = framecontportal*portalFase2.width/8;
            if(portalmode>=2){
                portalrect.y =  2*portalFase2.height/3;
            }   
            if(portalmode==3){
                portalativo = false;
            portalrect.y =  0;
            portalmode = 0;
            portalFrames=0;
            }
        }
 }
        playerTxtFrames++;
        if(playerTxtFrames>=60/framespeedPlayer ){
            playerTxtFrames = 0;
            framecontPlayer++;
            if(framecontPlayer>=9){
                framecontPlayer = 0;
            }
        player.rect.x = framecontPlayer*player.txt.width/12;
        }
        //SPAWN DE PLATADORMAS
       
        if(!portalativo&&!lost&&!gotCristal&&!victoryFase2){
        
        if(frames%100==0){
       // frames = 0;
         rando = GetRandomValue(-225,255);
         randwidth = GetRandomValue(0,10);
         if(randwidth<3){
             randwidth = singleTile;
             movingPiece = false;
         }
         else if(randwidth<8){
             randwidth = singleTile*2;
             movingPiece = false;
         }
         else if(randwidth<9){
             randwidth = singleTile*3;
             movingPiece = false;
         }
         else{
             randwidth = singleTile*3;
             movingPiece = true;
         }

        tiles[i].body= CreatePhysicsBodyRectangle((Vector2){rando,cameraJumper.target.y-90},randwidth,50,10);
        tiles[i].body->enabled = false;
        tiles[i].isMoving = movingPiece;
        tiles[i].movementspeed = 5;
        tiles[i].rect.x = rando-(randwidth/2);    
        tiles[i].rect.y = cameraJumper.target.y-(50/2)-90;   
        tiles[i].rect.width = randwidth;  
        tiles[i].rect.height = 50; 
        i++;

        //CHECANDO SE HA PLATAFORMAS DEMAIS JA SPAWNADAS

        if(i>7) {
            i = 0;
            repetido = true;
        }
        //DESTRUIR PLATAFORMAS ANTIGAS

        if(repetido) DestroyPhysicsBody(tiles[i].body);

        //DESTRUI E RECONSTRUIR O PLAYER PARA QUE ELE COLIDA COM AS PLATAFORMAS

        Vector2 bpos = player.body->position;
        Vector2 veloc = player.body->velocity;
        DestroyPhysicsBody(player.body);
        player.body = CreatePhysicsBodyRectangle(bpos,player.rect.width,player.rect.height,1);
        player.body->freezeOrient = true; 
        player.body->velocity = veloc;
        

       
        }
         for(int z=0;z<8;z++){
    
            if(tiles[z].isMoving){
                
                tiles[z].body->position.x += tiles[z].movementspeed;
                tiles[z].rect.x += tiles[z].movementspeed;
                if(tiles[z].body->position.x>= 255)tiles[z].movementspeed = -tiles[z].movementspeed;
                if(tiles[z].body->position.x<= -255)tiles[z].movementspeed = -tiles[z].movementspeed;
                if(player.body->isGrounded&&CheckCollisionRecs((Rectangle){player.body->position.x-player.rect.width/2,player.body->position.y-player.rect.height/2,player.rect.width,player.rect.height},tiles[z].rect)){

                    player.body->position.x +=tiles[z].movementspeed;
                }
            }
        }
        if(gotCristal&&!endofmap){
            cristal2position =(Vector2){player.body->position.x-120,player.body->position.y-100};
            endofmap = true;
        }
        
        

        //ATUALIZACAO DA CAMERA
        cameraJumper.target.y -= 1.2f;
        if(cameraJumper.target.y<(-200)) paralaxFase2 -=0.8;
        }
        if(player.body->position.y>=cameraJumper.target.y+AlturaDaTela*1.5) lost = true;

        // MOVIMENTACAO
if(!portalativo){
        if (IsKeyDown(KEY_RIGHT)){ player.body->velocity.x = VELOCITY*1.1;player.direita = true;}
        else if (IsKeyDown(KEY_LEFT)){ player.body->velocity.x = -VELOCITY*1.1;player.direita = false;}
        else player.body->velocity.x = 0;
        if (IsKeyDown(KEY_UP) && player.body->isGrounded){
            PlaySound(jumpfase2);
             player.body->velocity.y = -VELOCITY*5;
             
        }
}
        
        
        //ATUALIZACAO DAS SPRITES

        if(player.body->velocity.x==0 && player.direita){  
            player.rect.y = 2*player.txt.height/4;
        }
        else if(player.body->velocity.x==0&& !player.direita) player.rect.y = 3*player.txt.height/4;
        else{
        if(player.direita) player.rect.y = 0;
        else player.rect.y = player.txt.height/4;
        }

        if(segundos>=50){
        victoryFase2 = true;
        cristalrect.x = 0;
        cristalrect.y = cameraJumper.target.y-350;
        }

        //if(gotCristal&&!portalativo) endgame=true;

        if(lost&&IsKeyPressed(KEY_R)){
            lost = false;
            endgame=true;
            replayfase2 = true;
        }
        if(CheckCollisionPointRec(player.body->position,cristalrect)) gotCristal = true;
 }
    

      void Desenhar(){
        BeginDrawing();
        BeginMode2D(cameraJumper);
            ClearBackground(RAYWHITE);
            DrawTexture(backgroundFase2,-backgroundFase2.width/2,-backgroundFase2.height+AlturaDaTela/1.5+paralaxFase2,WHITE);
           //DESENHO DO MAPA
            for(int x=0;x<8;x++){
                int rectsize = tiles[x].rect.width/50;
               for(int y = 0;y<rectsize;y++){
                DrawTextureRec(tilesTxt,tilesRect,(Vector2){tiles[x].rect.x+(y*50),tiles[x].rect.y},WHITE);
            
                }
            }
            if(victoryFase2&&!gotCristal){
                DrawTextureRec(cristaltxt,cristal,(Vector2){0,cameraJumper.target.y-350},WHITE);
            }
            
            
            //DESENHO DO PLAYER

            if(!lost&&!gotCristal){
            DrawTextureRec(player.txt,player.rect,(Vector2){player.body->position.x-25,player.body->position.y-25},WHITE);
            }
            if(portalativo)DrawTextureRec(portalFase2,portalrect,(Vector2){player.body->position.x-120,player.body->position.y-100},WHITE);
            if(gotCristal)DrawTextureRec(portalFase2,portalrect,(Vector2){-100,cameraJumper.target.y-450},WHITE);
    

            
            EndMode2D();
            DrawText(TextFormat("%d", segundos),1100,600,50,RED);
            /*if(victoryFase2&&gotCristal){
                DrawRectangleGradientH(0,0,1280,720,DARKGRAY,WHITE);
                DrawText("YOU WON!!!",300,360,100,PURPLE);
                DrawText("pressione 0 para voltar",100,700,20,RED);
            }*/
             if(lost){
                DrawRectangle(0,0,1280,720,DARKGRAY);
                DrawText("você perdeu!!!",300,300,100,WHITE);
                DrawText("pressione R para tentar de novo",100,700,20,WHITE);
            }
            
        EndDrawing();
        
    }
void fechar(){
UnloadTexture(player.txt);
UnloadTexture(tilesTxt);
for(int z=0;z<8;z++){
    DestroyPhysicsBody(tiles[z].body);
    tiles[z].rect = (Rectangle){0,0,0,0};
            
}
ClosePhysics();       
    UnloadMusicStream(musicfase2);
    UnloadSound(jumpfase2);
}
   
int Fase2(){
    initfase2();
while(!endgame){
    RunPhysicsStep();
    UpdateMusicStream(musicfase2);
    Refresh();
    Desenhar();
}
    fechar();
    if(gotCristal) return 2;
    else return 1;
    
}