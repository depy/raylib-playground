#include <iostream>
#include "../include/raylib/raylib.h"
#include "../include/raylib/raymath.h"
#include "../include/opensimplexnoise.hpp"

// #define MAX_COLUMNS 20

using namespace std;

double noise(int x, int y) {
    auto n = OpenSimplexNoise(0);
    auto layer1 = n.Evaluate(x/50.0f, y/50.0f, 0);
    auto layer2 = n.Evaluate(x/10.0f, y/10.0f, 1.0);
    auto layer3 = n.Evaluate(x/3.0f, y/3.0f, 2.0);
    auto val = layer1*2 + layer2/2 + layer3/10;
    return val;
}

class Map {
public:
    vector<vector<double>> data;

    Map(int w, int h) {
        generate(w, h);
    }

    void draw2D(int dx, int dy, int pixelSize) {
        double minOffset = (-1 * minValue);
        double normConst = ((-1 * minValue) + maxValue); 
        for(int j = 0; j < 100; j++) {
            for(int i = 0; i < 100; i++) {
                double n = (data.at(j).at(i) + minOffset) / normConst;
                unsigned char v = (unsigned char)(n * 255.0);
                Color color = (Color){v, v, v, 255};
                int x = i * pixelSize;
                int y = j * pixelSize;
                DrawRectangle(x + dx, y + dy, pixelSize, pixelSize, color);
            }
        } 
    }

private:
    double minValue = 999.9;
    double maxValue = -999.9;

    void generate(int w, int h) {
        for(int j = 0; j < h; j++) {
            vector<double> row;
            for(int i = 0; i < w; i++) {
                double n = noise(i, j);
                if(n < minValue) { minValue = n; }
                if(n > maxValue) { maxValue = n; }
                row.push_back(n);
            }
            data.push_back(row);
        }
        cout << "Min: " << minValue << " Max: " << maxValue << " Rat: " << ((-1 * minValue) + maxValue) << endl;
    }
};

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

    int w = 255;
    int h = 255;
    Map map = Map(w, h);
    vector<vector<double>> m = map.data;

    Mesh mesh = GenMeshPlane(100, 100, w-1, h-1);
    
    for(int i = 0; i < mesh.vertexCount * 3; i += 3) {
        int y = (i/3) % w;
        int x = (i/3) / h;
        auto n = m.at(y).at(x);
        mesh.vertices[i + 1] = n * 5;
    }
    
    UpdateMeshBuffer(mesh, 0, mesh.vertices, sizeof(float) * mesh.vertexCount * 3, 0);

    Material material = LoadMaterialDefault();
    Matrix matrix = MatrixIdentity();
    Model model = LoadModelFromMesh(mesh);
    
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
                DrawModel(model, (Vector3){-5.0f, 0.0f, -5.0f }, 1, GRAY);
                DrawModelWires(model, (Vector3){ -5.0f, 0.0f, -5.0f }, 1, BLACK);
                DrawPlane(Vector3{0, -2, 0}, Vector2{500, 500}, BLUE);
                // DrawGrid(100, 1.0f);
            }
            EndMode3D();

            int size = 2;
            int xOffset = screenWidth - (100 * size);

            map.draw2D(xOffset, 0, size); 

            DrawFPS(10, 10);
        EndDrawing();
    }

    // De-Initialization

    CloseWindow();

    return 0;
}