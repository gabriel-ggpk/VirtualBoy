#include <raylib.h>
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"
#define VELOCITY    0.5f
const int larguraDaTela = 1280;
const int AlturaDaTela = 720;

typedef struct Player
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
Camera2D cameraJumper = {0};

    

int frames=0;
    int txtframes = 0;
    int framecont = 0;
    int framespeed=12;
    int i = 0;
    int random = 0;
    int randwidth = 0;
    int singleTile = 50;
    bool repetido = false;
    bool lost = false;

Player player = {0};


void initfase2(void)
{
SetConfigFlags(FLAG_MSAA_4X_HINT);
InitPhysics();
// SetPhysicsGravity(0.0f, 1.1f);
player.txt = LoadTexture("assets/Fase2/half.png");
player.txt.width =600;
player.txt.height =240;
player.rect = (Rectangle) {0,0,player.txt.width/12,player.txt.height/4};
player.position = (Vector2){0,0};
player.body = CreatePhysicsBodyRectangle((Vector2){ 0,0 }, player.rect.width,player.rect.height, 1);
player.body->freezeOrient = true;
for(int z=0;z<8;z++){
tiles[z].txt = LoadTexture("assets/Fase2/castleTiles.png");
tiles[z].txt.width = 525;
tiles[z].txt.height = 375;
tiles[z].rect = (Rectangle){0,0,2*tiles[z].txt.width/21,2*tiles[z].txt.height/15};
tiles[z].rect.x = 2*tiles[z].txt.width/21;
tiles[z].rect.y = 2*tiles[z].txt.height/15;
}

    PhysicsBody base = CreatePhysicsBodyRectangle((Vector2){ 0, 50 }, 450, 100, 10);
    base->enabled = false;

    cameraJumper.target = (Vector2) {0,0};
    cameraJumper.zoom = 0.7f;
    cameraJumper.offset = (Vector2) {larguraDaTela/2,AlturaDaTela/2};
    
    
}
    
 void Refresh(){
    //ATUALIZANDO FRAMES PARA SPRITES
        txtframes++;
        if(txtframes>=120/framespeed){
            txtframes = 0;
            framecont++;
            if(framecont>=11){
                framecont = 0;
            }
        player.rect.x = framecont*player.txt.width/12;
        }

        //SPAWN DE PLATADORMAS

        frames++;
        if(frames>=200){
        frames = 0;
         random = GetRandomValue(-225,255);
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

        tiles[i].body= CreatePhysicsBodyRectangle((Vector2){random,cameraJumper.target.y+100},randwidth,50,10);
        tiles[i].body->enabled = false;
        tiles[i].rect.x = random-(randwidth/2);    
        tiles[i].rect.y = cameraJumper.target.y-(50/2)+100;   
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

        //ATUALIZACAO DAS SPRITES

        if(player.body->velocity.x==0 && player.direita){  
            player.rect.y = 2*player.txt.height/4;
        }
        else if(player.body->velocity.x==0&& !player.direita) player.rect.y = 3*player.txt.height/4;
        else{
        if(player.direita) player.rect.y = 0;
        else player.rect.y = player.txt.height/4;
        }
        

        //ATUALIZACAO DA CAMERA
        cameraJumper.target.y -= 0.6f;
        if(player.body->position.y>=cameraJumper.target.y+AlturaDaTela*1.5) lost = true;
       
    
 }
    

      void Desenhar(){
        BeginDrawing();
        BeginMode2D(cameraJumper);
            ClearBackground(BLACK);

           //DESENHO DO MAPA
            for(int x=0;x<8;x++){
                int rectsize = tiles[x].rect.width/50;
               for(int y = 0;y<rectsize;y++){
                DrawTextureRec(tiles[x].txt,tiles[x].rect,(Vector2){tiles[x].rect.x+(y*50),tiles[x].rect.y},WHITE);
            
                }
            }

            //DESENHO DO PLAYER

            DrawTextureRec(player.txt,player.rect,(Vector2){player.body->position.x-25,player.body->position.y-25},WHITE);
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