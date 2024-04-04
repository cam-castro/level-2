#include <iostream>
#include <map>
#include <string>
#include <locale>
#include "raylib.h"
#include <codecvt>
#include "CSVData.h"
#include "Lequel.h"

using namespace std;

int main(void){
    map<string, string> languageCodeNames;
    LanguageProfiles languages;

    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "NewCorpus");
    SetTargetFPS(60);

    Text text;
    int flagForText = 0;
    CSVData data;
    std::vector<std::string> campos(2);

    while(!WindowShouldClose()){
        if(IsKeyPressed(KEY_V) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL) ||
            IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER))){
            const char *clipboard = GetClipboardText();
            getTextFromString(clipboard, text);
            flagForText = 1;
        }

        if(IsFileDropped()){
            FilePathList droppedFiles = LoadDroppedFiles();

            if(droppedFiles.count == 1){
                getTextFromFile(droppedFiles.paths[0], text);

                flagForText = 1;

                UnloadDroppedFiles(droppedFiles);
            }
        }

        BeginDrawing();

        ClearBackground(BEIGE);

        DrawText("Add a New Corpus", 40, 80, 80, BROWN);
        DrawText("Copia y pega con Ctrl+V, o arrastra un archivo...", 100, 220, 24, BROWN);

        EndDrawing();

        if(flagForText){
            TrigramProfile perfilDeLenguaje = buildTrigramProfile(text);
            for(auto miembro : perfilDeLenguaje){
                campos[0] = miembro.first;
                campos[1] = to_string((int)miembro.second);
                data.push_back(campos);
            }
            DrawText("Language Added", 100, 300, 70, BROWN);
            EndDrawing();
            writeCSV("../klg.csv", data);
            WaitTime(5);
            CloseWindow();
        }
    }

    CloseWindow();

    return 0;
}