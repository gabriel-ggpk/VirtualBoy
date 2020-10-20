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
    InitAudioDevice();

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        framesTexto++;
        switch(fase){
           case 0:
            if(IsKeyPressed(KEY_ONE))
                fase = 1;
            if(IsKeyPressed(KEY_TWO))
                fase = 2;
            if(IsKeyPressed(KEY_THREE))
                fase = 3;
            if(IsKeyPressed(KEY_FOUR))
                fase = 4;
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(TextSubtext(textoInicial,0,framesTexto/10),150,250,20,BLACK);
                DrawText("Escolha uma fase 1, 2, 3 ou 4", 150, windowHeight/2, 20, BLACK);
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

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
