/*******************************************************************************************
*
*   Physac - Physics movement
*
*   NOTE 1: Physac requires multi-threading, when InitPhysics() a second thread is created to manage physics calculations.
*   NOTE 2: Physac requires static C library linkage to avoid dependency on MinGW DLL (-static -lpthread)
*
*   Use the following line to compile:
*
*   gcc -o $(main).exe $(physics) -s -static  /
       -lraylib -lpthread -lglfw3 -lopengl32 -lgdi32 -lopenal32 -lwinmm /
       -std=c99 -Wl,--subsystem,windows -Wl,-allow-multiple-definition
*
*   Copyright (c) 2016-2018 Victor Fisac
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>
#include<math.h>
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"

#define VELOCITY    0.5f
PhysicsBody generation(int size){
    while(1){

    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    int frames=0;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Physac [raylib] - Physics movement");
    SetTargetFPS(120);   
    // Physac logo drawing position
    int logoX = screenWidth - MeasureText("Physac", 30) - 10;
    int logoY = 15;

    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ 0, 50 }, 450, 100, 10);
    int i = 0;
    bool repetido = false;
    PhysicsBody tiles[8];
        /*{CreatePhysicsBodyRectangle((Vector2){ -125, -100},100 , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ 75,-100  },100 , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ -250,- 250 },50 , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ 0,-250  },50 , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ 225,- 300 }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ -125,-350  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},
        {CreatePhysicsBodyRectangle((Vector2){ ,-  }, , 50, 10)},*/

        
    
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -255,225 }, 10, screenHeight*2, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ 255,225}, 10, screenHeight*2, 10);
    Camera2D camera = {0};
    camera.target = (Vector2) {0,0};
    camera.zoom = 0.5f;
    camera.offset = (Vector2) {screenWidth/2,screenHeight/2};
    int rand = 0;
    int randwidth = 0;
    

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ 0,0 }, 50, 50, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
    //camera
    
    
                // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        RunPhysicsStep();
        frames++;
        if(frames>=200){
        frames = 0;
         rand = GetRandomValue(-225,255);
         randwidth = GetRandomValue(50,100);
        tiles[i]= CreatePhysicsBodyRectangle((Vector2){rand,camera.target.y},randwidth,50,10);
        tiles[i]->enabled = false;
        
        i++;
        if(i>7) {
            i = 0;
            repetido = true;
        }if(repetido) DestroyPhysicsBody(tiles[i]);
        Vector2 bpos = body->position;
        Vector2 veloc = body->velocity;
        DestroyPhysicsBody(body);
        body = CreatePhysicsBodyRectangle(bpos,50,50,1);
        body->freezeOrient = true; 
        body->velocity = veloc;
        }
        
        
        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT)) body->velocity.x = VELOCITY;
        else if (IsKeyDown(KEY_LEFT)) body->velocity.x = -VELOCITY;
        else body->velocity.x = 0;

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded) body->velocity.y = -VELOCITY*5;

        camera.target.x = body->position.x;
        camera.target.y -= 0.5f;
        wallLeft->position.y -= 0.5f;
        wallRight->position.y -= 0.5f;
         
       
    
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        BeginMode2D(camera);
            ClearBackground(BLACK);

            

            // Draw created physics bodies
            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                PhysicsBody body = GetPhysicsBody(i);

                int vertexCount = GetPhysicsShapeVerticesCount(i);
                for (int j = 0; j < vertexCount; j++)
                {
                    // Get physics bodies shape vertices to draw lines
                    // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                    Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                    int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                    Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                    DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
                }
            }
            
            EndMode2D();
            DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
            DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);

            DrawText("Physac", logoX, logoY, 30, WHITE);
            DrawText("Powered by", logoX + 50, logoY - 7, 10, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    ClosePhysics();       // Unitialize physics

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}