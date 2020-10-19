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

}Envment;

Envment base;
Envment tiles[8];
Texture2D tilesTxt;
Rectangle tilesRect;

Texture2D portalFase2;
Rectangle portalrect;

Texture2D backgroundFase2;
Rectangle backgroundFase2rect;
    
Camera2D cameraJumper = {0};
int frames=0;
    int playerTxtFrames = 0;
    int framecontPlayer = 0;
    int framespeedPlayer = 9;

    int portalFrames=0;
    int framecontportal=0;
    int portalmode=0;

    
    int i = 0;
    int rando = 0;
    int randwidth = 0;
    int singleTile = 50;
    int segundos = 0;
    bool repetido = false;
    bool lost = false;
    bool portalativo = true;
Player player = {0};


void initfase2(void)
{
SetConfigFlags(FLAG_MSAA_4X_HINT);
InitPhysics();
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
//portalrect.y = portalFase2.height/3;


PhysicsBody base = CreatePhysicsBodyRectangle((Vector2){ 0, 50 }, 450, 100, 10);
base->enabled = false;

    cameraJumper.target = (Vector2) {0,0};
    cameraJumper.zoom = 0.7f;
    cameraJumper.offset = (Vector2) {larguraDaTela/2,AlturaDaTela/2};
    
    
}
    
 void Refresh(){
    //ATUALIZANDO FRAMES PARA SPRITES
    frames++;

    if(frames%60==0)segundos++;

     if (segundos>4) portalativo = false;
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
            /*if(portalmode==1){
                portalrect.y = 0;
            }*/
            if(portalmode>=2){
                portalrect.y =  2*portalFase2.height/3;
            }   
            if(portalmode==3)portalativo = false;
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
       
        if(!portalativo){
        
        if(frames%120==0){
       // frames = 0;
         rando = GetRandomValue(-225,255);
         randwidth = GetRandomValue(0,10);
         if(randwidth<3){
             randwidth = singleTile;
         }
         else if(randwidth<8){
             randwidth = singleTile*2;
         }
         else{
             randwidth = singleTile*3;
         }

        tiles[i].body= CreatePhysicsBodyRectangle((Vector2){rando,cameraJumper.target.y-100},randwidth,50,10);
        tiles[i].body->enabled = false;
        tiles[i].rect.x = rando-(randwidth/2);    
        tiles[i].rect.y = cameraJumper.target.y-(50/2)-100;   
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
        
        
        // MOVIMENTACAO

        if (IsKeyDown(KEY_RIGHT)){ player.body->velocity.x = VELOCITY;player.direita = true;}
        else if (IsKeyDown(KEY_LEFT)){ player.body->velocity.x = -VELOCITY;player.direita = false;}
        else player.body->velocity.x = 0;
        if (IsKeyDown(KEY_UP) && player.body->isGrounded) player.body->velocity.y = -VELOCITY*5;

        
        

        //ATUALIZACAO DA CAMERA
        cameraJumper.target.y -= 1.2f;
        if(player.body->position.y>=cameraJumper.target.y+AlturaDaTela*1.5) lost = true;
        if(IsKeyPressed(KEY_ZERO)) lost = true;
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
    
 }
    

      void Desenhar(){
        BeginDrawing();
        BeginMode2D(cameraJumper);
            ClearBackground(RAYWHITE);

           //DESENHO DO MAPA
            for(int x=0;x<8;x++){
                int rectsize = tiles[x].rect.width/50;
               for(int y = 0;y<rectsize;y++){
                DrawTextureRec(tilesTxt,tilesRect,(Vector2){tiles[x].rect.x+(y*50),tiles[x].rect.y},WHITE);
            
                }
            }

            //DESENHO DO PLAYER

            //if(!portalativo)
            DrawTextureRec(player.txt,player.rect,(Vector2){player.body->position.x-25,player.body->position.y-25},WHITE);
            DrawText(TextFormat("%d", segundos),cameraJumper.target.x+700,cameraJumper.target.y+400,50,RED);
            if(portalativo)DrawTextureRec(portalFase2,portalrect,(Vector2){player.body->position.x-120,player.body->position.y-100},WHITE);
            EndMode2D();
            

            
        EndDrawing();
        
    }
void fechar(){
UnloadTexture(player.txt);
UnloadTexture(tiles->txt);  
ClosePhysics();       
     
}
   
int Fase2(){
    initfase2();
while(!lost){
    RunPhysicsStep();
    Refresh();
    Desenhar();
}
    fechar();
return 0;
}