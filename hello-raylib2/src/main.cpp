#include <iostream>
#include "../include/raylib/raylib.h"
#include "../include/raylib/raymath.h"
#include "../include/opensimplexnoise.hpp"

using namespace std;

class XMesh {
    Mesh m;

public:
    XMesh() {
        m = {0};
    }

private:
    void AllocateMeshData(Mesh* mesh, int triangleCount)
    {
        mesh->vertexCount = triangleCount * 3;
        mesh->triangleCount = triangleCount;

        mesh->vertices = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
        mesh->texcoords = (float*)MemAlloc(mesh->vertexCount * 2 * sizeof(float));
        mesh->normals = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
    }
};

double noise(int x, int y) {
    auto n = OpenSimplexNoise(0);
    auto layer1 = n.Evaluate(x/50.0f, y/50.0f, 0);
    auto layer2 = n.Evaluate(x/10.0f, y/10.0f, 1.0);
    auto layer3 = n.Evaluate(x/3.0f, y/3.0f, 2.0);
    auto val = layer1*2 + layer2/2 + layer3/10;
    return val;
}

void initCamera(Camera& c) {
    c.position = (Vector3){ 0.0f, 10.0f, 0.0f };
    c.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    c.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    c.fovy = 60.0f;
    c.projection = CAMERA_PERSPECTIVE;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    initCamera(camera);

    SetCameraMode(camera, CAMERA_FIRST_PERSON);
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        UpdateCamera(&camera);

        // Draw
        BeginDrawing();

            ClearBackground(SKYBLUE);

            BeginMode3D(camera);
            {
                DrawGrid(100, 1.0f);
            }
            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    // De-Initialization

    CloseWindow();

    return 0;
}