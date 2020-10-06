#include <raylib.h>

int main(){
    int windowWidth = 800, windowHeight = 450;
    int fase = 1;

    InitWindow(windowWidth, windowHeight, "Não temos um nome");
    
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        switch(fase){
           case 1:
            break;
           case 2:
            break;
           case 3:
            break;
           case 4:
            break; 
        }


        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(fase){
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break; 
            }

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}