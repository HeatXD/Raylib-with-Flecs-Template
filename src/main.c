/*******************************************************************************************
********************************************************************************************/

#include "raylib.h"
#include "header/config.h"
#include "header/components.h"
#include "ecs/flecs.h"

void BeforeDrawing(ecs_iter_t *it){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawFPS(20,20);
}
void AfterDrawing(ecs_iter_t *it){
    EndDrawing();
}
void Move(ecs_iter_t *it){
    Position2 *p = ecs_column(it, Position2, 1); 

    for (int i = 0; i < it->count; i++)
    {   
        if (IsKeyDown(KEY_UP))
            p[i].y-=10;
        if (IsKeyDown(KEY_DOWN))
            p[i].y+=10;
        if (IsKeyDown(KEY_LEFT))
            p[i].x-=10;
        if (IsKeyDown(KEY_RIGHT))
            p[i].x+=10;
    }
}
void DrawSquare(ecs_iter_t *it){
    Position2 *p = ecs_column(it, Position2, 1); 
    Size2 *s = ecs_column(it, Size2, 2); 
    ColorType *c = ecs_column(it, ColorType, 3); 

    for (int i = 0; i < it->count; i++)
    {        
        DrawRectangleLines(p[i].x, p[i].y, s[i].width,s[i].height,c[i].color);
    }

}
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NAME);// Create a window (Raylib)
    SetTargetFPS(60);    // Set our game to run at 60 frames-per-second (Raylib)
    //--------------------------------------------------------------------------------------
    //Create The ecs world (Flecs)
    ecs_world_t *world = ecs_init();
    //Declare components (Flecs)
    ECS_COMPONENT(world, Position2);
    ECS_COMPONENT(world, Size2);
    ECS_COMPONENT(world, ColorType);
    // Create a tag for each phase in the custom pipeline. (Flecs)
    ECS_TAG(world, PreUpdate);
    ECS_TAG(world, OnUpdate);
    ECS_TAG(world, PostUpdate);
    ECS_TAG(world, PreDraw);
    ECS_TAG(world, OnDraw);
    ECS_TAG(world, PostDraw);
    // Create the pipeline (Flecs)
    ECS_PIPELINE(world,CustomPipeline,PreUpdate,OnUpdate,PostUpdate,PreDraw,OnDraw,PostDraw);
    // Make sure the world uses the correct pipeline (Flecs)
    ecs_set_pipeline(world,CustomPipeline);
    //Creating a rendering entity to handle rendering pre and post draw
    ECS_TAG(world, RENDERER);
    ECS_ENTITY(world, RenderEntity, RENDERER);
    //Create a test entity and set variabels (Flecs)
    ECS_ENTITY(world, TestEntity, Position2, Size2, ColorType);
    ecs_set(world, TestEntity, Position2, {20, 20});
    ecs_set(world, TestEntity, Size2, {100, 100});
    ecs_set(world, TestEntity, ColorType, {RED});
    //Define ecs systems (Flecs)
    //Pre Update Systems
    //On Update Systems
    ECS_SYSTEM(world, Move, OnUpdate, Position2);
    //Post Update Systems
    //Pre Draw Systems
    ECS_SYSTEM(world,BeforeDrawing,PreDraw,RENDERER);
    // On Draw Systems
    ECS_SYSTEM(world,DrawSquare, OnDraw, Position2, Size2, ColorType);
    // Post Draw Systems
    ECS_SYSTEM(world,AfterDrawing,PostDraw,RENDERER);

    // Main game loop (Raylib)
    while (ecs_progress(world, 0) && !WindowShouldClose());    // Detect window close button or ESC key (Raylib)
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();           // Close window and OpenGL context (Raylib)
    return ecs_fini(world); //Clean up resources (Flecs)
    //--------------------------------------------------------------------------------------
}