#include <raylib.h>

int main(){
    int windowWidth = 800, windowHeight = 450;
    int fase = 0;

    InitWindow(windowWidth, windowHeight, "Não temos um nome");
    
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        
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
                DrawText("Escolha uma fase 1, 2, 3 ou 4", 150, windowHeight/2, 20, BLACK);
            EndDrawing();
            break;
           case 1:
            if(IsKeyPressed(KEY_ZERO))
                fase = 0;
            BeginDrawing();
                DrawText("Aperte 0 para voltar", 150, windowHeight/2, 20, BLACK);
                ClearBackground(RED);
            EndDrawing();
            break;
           case 2:
            if(IsKeyPressed(KEY_ZERO))
                fase = 0;
            BeginDrawing();
                DrawText("Aperte 0 para voltar", 150, windowHeight/2, 20, BLACK);
                ClearBackground(BLUE);
            EndDrawing();
            break;
           case 3:
            if(IsKeyPressed(KEY_ZERO))
                fase = 0;
            BeginDrawing();
                DrawText("Aperte 0 para voltar", 150, windowHeight/2, 20, BLACK);
                ClearBackground(YELLOW);
            EndDrawing();
            break;
           case 4:
            if(IsKeyPressed(KEY_ZERO))
                fase = 0;
            BeginDrawing();
                DrawText("Aperte 0 para voltar", 150, windowHeight/2, 20, BLACK);
                ClearBackground(GREEN);
            EndDrawing();
            break; 
        }
    }

    CloseWindow();
    
    return 0;
}