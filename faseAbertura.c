#include <raylib.h>

int abertura(int gemasColetadas){
    int retorno = 0;
    int AnimaPort = 0;

    int framesTexto = 0;
    const char textoInicial[] = "Finalmente! você acordou!              \nCalma, calma.\nSei que nada parece fazer sentido agora            \nem resumo eu criei você para pegar certos artefatos para mim\nEntao...                 \nMÃOS A OBRA!!!";
    const char textofase1[] ="Texto pos fase1";
    const char textofase2[] ="Texto pos fase2";
    const char textofase3[] ="Texto pos fase3";
    const char textofase4[] ="texto pos fase4";
    Texture2D PC = LoadTexture("assets/Boss/PC.png");
    PC.width = PC.width*10;
    PC.height = PC.height*10;
    Rectangle framePC;
    framePC.width = PC.width/5;
    framePC.height = PC.height/3;
    framePC.x = framePC.width*0;
    framePC.y = framePC.height*1;
    Texture2D Guy = LoadTexture("assets/full.png");
    PC.width = PC.width;
    PC.height = PC.height;
    Rectangle frameGuy;
    frameGuy.width = Guy.width/12;
    frameGuy.height = Guy.height/14;
    frameGuy.x = frameGuy.width*0;
    frameGuy.y = frameGuy.height*2;
    Texture2D Port = LoadTexture("assets/Portal.png");
    Port.width = Port.width*2;
    Port.height = Port.height*1;
    Rectangle framePort;
    framePort.width = Port.width/8;
    framePort.height = Port.height/3;
    framePort.x = framePort.width*0;
    framePort.y = framePort.height*1;
    int framePc  = 0;
    int portCont = 0;
    int portAtual  = 0;

    while(retorno == 0){
        framesTexto++;

        framePc++;
        if (framePc >= 60/2){
            framePc = 0;
            if(framePC.x == 0){
                framePC.x = framePC.width;
            }
            else{
                framePC.x = 0;
            }
        }

        if(AnimaPort == 1){
            portCont++;
            if (portCont >= 60/8){
                portCont = 0;
                portAtual++;
                if(portAtual > 7){
                    portAtual = 0;
                    if(framePort.y == framePort.height)
                        framePort.y = 0;
                    else if(framePort.y == 0)
                        framePort.y = framePort.height*2;
                    else{
                        AnimaPort = 0;
                        retorno = 1;
                    }
                }
                framePort.x = framePort.width*portAtual;
            }
        }

        if(IsKeyPressed(KEY_ENTER))
            AnimaPort = 1;

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureRec(PC, framePC, (Vector2){GetScreenWidth()/2 -framePC.width/2, 20}, RAYWHITE);
            DrawText("Aperte enter", 580, 0, 20, WHITE);
            if(framePort.y == framePort.height*1)
                DrawTextureRec(Guy, frameGuy, (Vector2){GetScreenWidth()/2 -frameGuy.width/2, 200}, RAYWHITE);
            if(AnimaPort > 0)
                DrawTextureRec(Port, framePort, (Vector2){GetScreenWidth()/2 -framePort.width/2, 180}, RAYWHITE);
            if(gemasColetadas==0) DrawText(TextSubtext(textoInicial,0,framesTexto/5),150,250,20,WHITE);
            if(gemasColetadas==1) DrawText(TextSubtext(textofase1,0,framesTexto/5),150,250,20,WHITE);
            if(gemasColetadas==2) DrawText(TextSubtext(textofase2,0,framesTexto/5),150,250,20,WHITE);
            if(gemasColetadas==3) DrawText(TextSubtext(textofase3,0,framesTexto/5),150,250,20,WHITE);
            if(gemasColetadas==4) DrawText(TextSubtext(textofase4,0,framesTexto/5),150,250,20,WHITE);
            
        EndDrawing();
    }

    UnloadTexture(PC);
    UnloadTexture(Guy);
    UnloadTexture(Port);
    if(gemasColetadas==0)return 0;
    if(gemasColetadas==1)return 1;
    if(gemasColetadas==2)return 2;
    if(gemasColetadas==3)return 3;
    if(gemasColetadas==4)return 4;
    
}