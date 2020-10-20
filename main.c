#include <raylib.h>

#include "Boss.c"
#include "faseNave.c"
#include "fase2.c"
#include "faseRunner.c"

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"


int main(){
    int windowWidth = 1280, windowHeight = 720;
    int fase = 0;
    int framesTexto = 0;
    const char textoInicial[27] = "teste de inscricao de tela";
    /*const char textoFase1[];
    const char textoFase2[]; 
    const char textoFase3[];
    const char textoPreBoss[];
    const char textoVitoria[];
    */  

    /*bool gema1adquirida = 0;
    bool gema2adquirida = 0;
    bool gema3adquirida = 0;*/

    InitWindow(windowWidth, windowHeight, "Não temos um nome");
    //InitAudioDevice();

    Texture2D button = LoadTexture("assets/Button.png");
    button.width = button.width*5;
    button.height = button.height*5;
    int BotaoHeight = button.height/3;
    int BotaoWidth = button.width/2;
    Rectangle sourceRec;
    sourceRec.x = 0;
    sourceRec.y = BotaoHeight;
    sourceRec.width = BotaoWidth;
    sourceRec.height = BotaoHeight;
    Rectangle btnBounds = { GetScreenWidth()/2 -BotaoWidth/2, 400, button.width/2, BotaoHeight};

    int btnState = 0;
    Vector2 mousePoint = { 0.0f, 0.0f };

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        framesTexto++;
        switch(fase){
           case 0:
            /*if(IsKeyPressed(KEY_ONE))
                fase = 1;
            if(IsKeyPressed(KEY_TWO))
                fase = 2;
            if(IsKeyPressed(KEY_THREE))
                fase = 3;
            if(IsKeyPressed(KEY_FOUR))
                fase = 4;*/
            mousePoint = GetMousePosition();
            
            if (CheckCollisionPointRec(mousePoint, btnBounds)){
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                    btnState = 1;
                if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
                    btnState = 2;
            }
            else
                btnState = 0;


            if(btnState == 1)
                sourceRec.x = BotaoWidth; 
            if(btnState == 2)
                sourceRec.x = 0;
            
            BeginDrawing();
                ClearBackground(RAYWHITE);
<<<<<<< HEAD
                DrawRectangleLinesEx(btnBounds, 2, BLUE);
                DrawTextureRec(button, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE);
                //DrawText("Escolha uma fase 1, 2, 3 ou 4", 150, windowHeight/2, 20, BLACK);
=======
                DrawText(TextSubtext(textoInicial,0,framesTexto/10),150,250,20,BLACK);
                DrawText("Escolha uma fase 1, 2, 3 ou 4", 150, windowHeight/2, 20, BLACK);
>>>>>>> 1181ccda52e94bbf0dc46aa720624d4bbdef2d83
            EndDrawing();
            break;

           case 1:
            if(faseRunner() == 1){
                fase = 0;
            }
            break;

           case 2:
            if(Fase2() == 2){
                fase = 0;
                //gema2adquirida = true;
            }
            else if(Fase2()==3) fase = 0;
            else fase = 2;
            break;

           case 3:
            if(Fase3() == 1){
                fase = 0;
            }
            else if(Fase3() == 2){
                fase = 4;
                //gema3adquirida = true;
            }
            break;

           case 4:
            if(BOSS() == 1){
                fase = 0;
            }
            break;
        }
    }

    UnloadTexture(button);

    //CloseAudioDevice();
    CloseWindow();

    return 0;
}
