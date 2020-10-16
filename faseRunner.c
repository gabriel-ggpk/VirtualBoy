#include <raylib.h>

//#define PHYSAC_IMPLEMENTATION
//#define PHYSAC_NO_THREADS
#include "physac.h"

typedef struct{
    Texture2D texture;
    PhysicsBody body;
    float height;
    float width;
    Rectangle rec;
    Vector2 position;
    Rectangle collision;
    int frame;
}Personagem;

typedef struct {
    Texture2D image;
    PhysicsBody body;
}Assets;

const int screenWidth = 1280, screenHeight = 720;
float frames_counter, frames_countdown = 0, walking = 0, frame_spawn;
int countdown = 3, timer = 121, frameline = 0, limit = 0;
bool spawn_set = false, dead = false, down = false;
float downx = 1, downy = 1;

Camera2D camera = {0};
Personagem personagem;
Personagem rock[3];
Personagem bird[3];
Assets ground;
Assets background;

void initGame(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    //InitWindow(screenWidth, screenHeight, "Janela");
    //ToggleFullscreen();
    InitPhysics();
    SetPhysicsGravity(0.0f, 1.1f);

    ground.image = LoadTexture("assets/FaseRunner/ground.png");
    ground.body = CreatePhysicsBodyRectangle((Vector2){screenWidth / 2, GetScreenHeight() - (ground.image.height*1.5 - 18)}, 200000, 100, 10);
    ground.body->enabled = false;
    background.image = LoadTexture("assets/FaseRunner/bg.png");

    personagem.body = CreatePhysicsBodyRectangle((Vector2){400, GetScreenHeight() -ground.image.height*1.5 - 10}, 20, 28, 1);
    personagem.body->freezeOrient = true;
    personagem.texture = LoadTexture("assets/full.png");
    //personagem.texture.width = personagem.texture.width*2;
    //personagem.texture.height = personagem.texture.height*2;
    personagem.width = personagem.texture.width/12;
    personagem.height = personagem.texture.height/14;
    personagem.frame = 0;

    for(int i = 0; i < 3; i++){        
        rock[i].texture = LoadTexture("assets/FaseRunner/rock.png");
        rock[i].width = rock[i].texture.width * 2.5;
        rock[i].height = rock[i].texture.height * 2.5;
        rock[i].position.x = -100;
        rock[i].position.y = -100;

        bird[i].texture = LoadTexture("assets/FaseRunner/bird.png");
        bird[i].width = bird[i].texture.width/8; 
        bird[i].height = bird[i].texture.height/3;
        bird[i].frame = 0 + i * 3;
        bird[i].position.x = -100;
        bird[i].position.y = -100;
    }

    camera.target = personagem.body->position;
    camera.offset = (Vector2){screenWidth/2, screenHeight/2};
    camera.rotation = 0.0f;
    camera.zoom = 1.2f;
}

void contador(){
    frames_counter += GetFrameTime(), frames_countdown += GetFrameTime(), walking = GetFrameTime();
    if(frames_countdown >= 1){
        countdown--;
        frames_countdown = 0;
        if(countdown < 0 && timer > -1 && !dead) timer--;
        if(countdown == 0) personagem.frame = 0;
    }

    if(countdown < 1 && walking >= 0.005 && !dead){
        if(timer > -1) personagem.body->position.x += 5.0f;
        for(int i = 0; i < 3; i++)
            bird[i].position.x -= 1.2f;

        walking = 0;
    }

    if(frames_counter >= 0.05){
        if(dead){
            if(personagem.frame < 7) personagem.frame++;
            frames_counter = 0;
        }
        else if(down){
            if(frames_counter >= 0.035){
                frames_counter = 0;
                personagem.frame++;
                if(personagem.frame == 6){
                    down = false;
                    personagem.body->orient = 0;
                    downx = 1, downy = 1;
                    personagem.frame = 0;
                }
            }
        }
        else{
            personagem.frame++;
            personagem.frame %= 12 - limit;
            frames_counter = 0;
        }
        for(int i = 0; i < 3; i++){
            bird[i].frame++;
            bird[i].frame %= 8;
        }
    }
}

void movimentacao(){
    if(IsKeyPressed(KEY_UP) && personagem.body->isGrounded && !dead && personagem.body->orient == 0 && timer > 1){
        personagem.body->velocity.y = -0.5f;
        personagem.frame = 0;
    }
    if(IsKeyPressed(KEY_DOWN) && !dead && timer > 1){
        if(!personagem.body->isGrounded) personagem.body->velocity.y = 0.8f;
        else{
            personagem.body->orient = 1.57;
            down = true;
            personagem.frame = 0;
            downx = 1.4, downy = 0.7;
        }
    }
    if(timer > 0){
        camera.target.x = personagem.body->position.x + 350;
        camera.target.y = screenHeight - 320;
    }
}

void spawn(){
    if(countdown < 2) frame_spawn += GetFrameTime();

    if(frame_spawn >= 3.25 && timer >= 2 && !dead){
        int rand_spawn, rand_spawn1;
        if(spawn_set){
            spawn_set = false;
            rand_spawn = GetRandomValue(1, 2);
        }
        else rand_spawn = GetRandomValue(1, 3);

        rand_spawn1 = GetRandomValue(1, 3);

        if(rand_spawn == 1){
            bird[0].position.x = personagem.body->position.x + screenWidth - 200;
            bird[0].position.y = screenHeight - ground.image.height*1.5 -80 -personagem.height;
            if(rand_spawn1 == 1){
                bird[1].position.x = personagem.body->position.x + screenWidth -50;
                bird[1].position.y = screenHeight -ground.image.height*1.5 -120 -personagem.height;
            }
            else{
                bird[2].position.x = personagem.body->position.x + screenWidth - 50;
                bird[2].position.y = screenHeight -ground.image.height*1.5 -40 -personagem.height;
                if(rand_spawn1 == 3){
                    rock[0].position.x = personagem.body->position.x + screenWidth - 300;
                    rock[0].position.y = screenHeight -ground.image.height*1.5 -personagem.height -rock[2].height;
                }
            }
        }
        else if(rand_spawn == 2){
            bird[0].position.x = personagem.body->position.x + screenWidth -200;
            bird[0].position.y = screenHeight -ground.image.height*1.5 -80 -personagem.height;
            bird[1].position.x = personagem.body->position.x + screenWidth -200;
            bird[1].position.y = screenHeight -ground.image.height*1.5 -120 -personagem.height;
            bird[2].position.x = personagem.body->position.x + screenWidth -200;
            bird[2].position.y = screenHeight -ground.image.height*1.5 -40 -personagem.height;
        }
        else{
            rock[0].position.x = personagem.body->position.x + screenWidth - 300;
            rock[0].position.y = screenHeight -ground.image.height*1.5 -personagem.height -rock[2].height;
            if(rand_spawn1 == 1){
                rock[1].position.x = personagem.body->position.x +screenWidth;
                rock[1].position.y = screenHeight -ground.image.height*1.5 -personagem.height -rock[2].height;
                spawn_set = true;
            }else if(rand_spawn1 == 2){
                rock[1].position.x = personagem.body->position.x + screenWidth;
                rock[1].position.y = screenHeight -ground.image.height*1.5 -personagem.height -rock[2].height;
                rock[2].position.x = personagem.body->position.x + screenWidth + 300;
                rock[2].position.y = screenHeight -ground.image.height*1.5 -personagem.height -rock[2].height;
                spawn_set = true;
            }
        }
        frame_spawn = 0;
    }
}

void colisao(){
    Rectangle personagemC = {personagem.body->position.x - 9 * downx, personagem.body->position.y - 14 * downy, 20 * downx, 28 * downy};    
    personagem.collision = personagemC;

    for(int i = 0; i < 3; i++){
        Rectangle rockC = {rock[i].position.x, rock[i].position.y, rock[i].width - 8, rock[i].height - 10};
        rock[i].collision = rockC;
    
        Rectangle birdC = {bird[i].position.x, bird[i].position.y, bird[i].width, bird[i].height + 1};
        bird[i].collision = birdC;
    }

    if(timer > 0 && !dead){
        for(int i = 0; i < 3; i++){
            if(!dead) dead = CheckCollisionRecs(personagem.collision, rock[i].collision);
            if(!dead) dead = CheckCollisionRecs(personagem.collision, bird[i].collision);
        }
        if(dead){
            personagem.body->enabled = false;
            personagem.frame = 0;
        }
    }
}

void desenho(){
    if(dead) frameline = 4, limit = 2;
    else if(personagem.body->orient != 0) frameline = 6, limit = 6;
    else if(IsKeyPressed(KEY_UP) || !personagem.body->isGrounded) frameline = 10, limit = 11;
    else if(countdown < 1 && timer > -1) frameline = 0, limit = 0;
    else frameline = 2, limit = 1;

    Rectangle personagemrec = {personagem.width * personagem.frame, personagem.height * frameline, personagem.width, personagem.height};
    personagem.rec = personagemrec;

    BeginDrawing();
    ClearBackground(BLACK);
        BeginMode2D(camera);

        int ground_size = 100000/ground.image.width;
        for(int i = -8; i < ground_size; i++){
            DrawTextureEx(ground.image, (Vector2){ground.image.width * 2.5 * i, GetScreenHeight() - ground.image.height*2.5 +personagem.height}, 0.0f, 2.5f, RAYWHITE);
            DrawTextureEx(background.image, (Vector2){background.image.width * 1.6 * i, 92}, 0.0f, 1.6f, RAYWHITE);
        }

        DrawTextureRec(
            personagem.texture,
            personagem.rec,
            (Vector2){personagem.body->position.x - 17, personagem.body->position.y - 17},
            RAYWHITE);

        for(int i = 0; i < 3; i++){
            DrawTextureEx(rock[i].texture, (Vector2){rock[i].position.x, rock[i].position.y}, 0.0f, 2.5f, RAYWHITE);
            Rectangle birdrec = {bird[i].width * bird[i].frame, bird[i].height, bird[i].width, bird[i].height};
            bird[i].rec = birdrec;            
            DrawTextureRec(
                bird[i].texture,
                bird[i].rec,
                (Vector2){bird[i].position.x, bird[i].position.y},
                RAYWHITE);   
        }
        
        EndMode2D();

    if(countdown >= 0){
            DrawText(TextFormat("%d", countdown), screenWidth/2 - 25, screenHeight/2 - 50, 100, RAYWHITE);
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.4f));
            DrawText("Pressione seta para cima para pular", screenWidth/2 - 170, screenHeight - 100, 20, RAYWHITE);
            DrawText("Pressione seta para baixo para se abaixar", screenWidth/2 - 200, screenHeight - 78, 20, RAYWHITE);
        }
        else{
            if(timer > 0) DrawText(TextFormat("Time: %d", timer), 10, 10, 20, RAYWHITE);
            else DrawText("Time: 0", 10, 10, 20, RAYWHITE);
        }

        //Desenhando o fim de jogo;
        if(dead){
            DrawText("YOU LOSE", screenWidth/2 - 250, screenHeight/2 - 50, 100, RAYWHITE);
            DrawText("Pressione enter para voltar ao menu", screenWidth/2 - 170, screenHeight - 100, 20, RAYWHITE);
            //if(IsKeyPressed(KEY_ENTER)) return 2;
        }    
        else if(timer < 0) DrawText("YOU WIN", screenWidth/2 - 225, screenHeight/2 - 50, 100, RAYWHITE);

    EndDrawing();
}

void gameRunner(){
    RunPhysicsStep();
    contador();
    movimentacao();
    spawn();
    colisao();
    desenho();
}

int faseRunner(void){
    int retornoRunner = 0;

    initGame();

    while(retornoRunner == 0){
        if(IsKeyPressed(KEY_ZERO)){
            retornoRunner = 1;
        }
        if(IsKeyPressed(KEY_SPACE)){
        }
        
        gameRunner();
    }

    UnloadTexture(personagem.texture);
    UnloadTexture(rock->texture);
    UnloadTexture(bird->texture);
    UnloadTexture(ground.image);
    UnloadTexture(background.image);

    CloseWindow();
    ClosePhysics();

    return retornoRunner;
}
