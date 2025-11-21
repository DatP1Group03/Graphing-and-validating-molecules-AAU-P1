#include <stdio.h>

#include "bfs_matrix.h"
#include "Input/validation.h"
#include "valence_check.h"

#include "external/raylib/src/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


#define MaxInput 256
#define TAB  17
#define MaxInput 100
#define TAB_BASE_COLOR_NORMAL   14
#define TAB_BASE_COLOR_FOCUSED 17
#define TAB_TEXT_COLOR_NORMAL   15
#define TAB_TEXT_COLOR_FOCUSED  18
Color goodGreen = (Color){ 0, 180, 120, 255 };
Color softRed   = (Color){ 200, 60, 60, 255 };
Color grayText  = DARKGRAY;

static char smilesInput[256] = {0};
static bool inputValid = false;
static bool editMode = false;
static bool answered = false;
static bool val_flag = false;
static bool end_flag = false;
static bool moleculeLoaded = false;
static bool moleculeValidated= false;
Font uiFont;

// Tab functions
void DrawTab_InputValidation();
void DrawTab_AdjacencyMatrix();
void DrawTab_StabilityCheck();
void DrawTab_AlgorithmVisualization();
void DrawTab_GraphView();
void Clear();



int main(void)
{

    InitWindow(900, 600, "S-SMILES");
    SetTargetFPS(60);
    TraceLog(LOG_INFO, "CWD: %s", GetWorkingDirectory());
    int activeTab = 0;

    const char *tabs[] = {
        "Input Validation",
        "Adjacency Matrix",
        "Stability",
        "Algorithms",
        "Graph View",
    };
    int tabCount = sizeof(tabs) / sizeof(tabs[0]);

    uiFont = LoadFontEx("rec/georgia.ttf", 30, NULL, 0);
    GuiSetFont(uiFont);

    // Set these once (global UI styles)
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x555555ff);

    GuiSetStyle(TAB, TEXT_PADDING, 8);
    GuiSetStyle(TAB, BORDER_WIDTH, 1);
    GuiSetStyle(TAB, BASE_COLOR_NORMAL, 0xE8E8E8FF);
    GuiSetStyle(TAB, BASE_COLOR_FOCUSED, 0xC0FAC0FF);
    GuiSetStyle(TAB, TEXT_COLOR_FOCUSED, 0x006600FF);





    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color){ 255, 250, 250 });
        int oldBaseNormal = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
        int oldBaseFocused = GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED);
        int oldTextNormal = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);

        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xCC4444FF);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xDD6666FF);
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

        Rectangle clearTab = {740, 20, 140, 30};
        if (GuiButton(clearTab, "CLEAR")) {
            Clear();
        }
        //Shortcut til Clear
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
            Clear();
        }

        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, oldBaseNormal);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, oldBaseFocused);
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, oldTextNormal);

        GuiSetStyle(TAB + 0, TAB_BASE_COLOR_NORMAL, 0xCCCCCCFF); // default gray

        if (inputValid)
        {
            GuiSetStyle(TAB + 0, TAB_BASE_COLOR_FOCUSED, 0x4CAF50FF);
        }

        GuiTabBar(
            (Rectangle){20, 20, 860, 30},
            tabs,
            tabCount,
            &activeTab
        );

        switch (activeTab)
        {
            case 0: DrawTab_InputValidation(); break;
            case 1: DrawTab_AdjacencyMatrix(); break;
            case 2: DrawTab_StabilityCheck(); break;
            case 3: DrawTab_AlgorithmVisualization(); break;
            case 4: DrawTab_GraphView(); break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawTab_InputValidation()
{

    DrawTextEx(uiFont,"Input Validation Tab", (Vector2){30,80},30,2, BLACK);

    GuiLabel((Rectangle){30, 130, 120, 25}, "SMILES Input:");


    if (GuiTextBox((Rectangle){160, 130, 300, 25}, smilesInput, 256, editMode))
        editMode = !editMode;

    // Validate button
    if (GuiButton((Rectangle){470, 130, 100, 25}, "Validate"))
    {
        moleculeValidated = true;
        inputValid = validate_smiles(smilesInput);
        TraceLog(LOG_INFO, "Validate pressed. Input: %s | Valid=%d", smilesInput, inputValid);
    }


    if (moleculeValidated) {

        if (inputValid ) {
            DrawTextEx(uiFont, "Valid SMILES", (Vector2){30, 180}, 30, 0, goodGreen);

            DrawTextEx(uiFont, "INPUT SIZE DOES NOT EXCEED MAX_INPUT OF 100", (Vector2){30, 230}, 15, 0, goodGreen);
            DrawTextEx(uiFont, "VALID PARENTHESES", (Vector2){30, 250}, 15, 0, goodGreen);
            DrawTextEx(uiFont, "NO ILLEGAL CHARACTERS", (Vector2){30, 270}, 15, 0, goodGreen);
            DrawTextEx(uiFont, "NO UNCLOSED RINGS", (Vector2){30, 290}, 15, 0, goodGreen);
        }
        else {
            DrawTextEx(uiFont, "INVALID SMILES:", (Vector2){30, 180}, 25, 2, softRed);
            int count = get_error_count();
            if (!inputValid) {
                DrawTextEx(uiFont, "INVALID SMILES:", (Vector2){30, 180}, 25, 2, softRed);

                int y = 220;
                int cnt = get_error_count();

                for (int i = 0; i < cnt; i++) {
                    char buffer[256];
                    snprintf(buffer, sizeof(buffer), "%s (position %d)",
                             get_error_message(i),
                             get_error_position(i));

                    DrawTextEx(uiFont, buffer, (Vector2){30, y}, 20, 2, RED);
                    y += 40;
                }
            }
        }
    }
    else {
        DrawTextEx(uiFont, "SMILES INPUT REQUIRED", (Vector2){30, 180}, 20, 2, BLACK);
    }

}

void DrawTab_AdjacencyMatrix()
{
    DrawText("Adjacency Matrix Tab", 30, 80, 25, BLACK);

}

void DrawTab_StabilityCheck()
{
    DrawTextEx(uiFont,"Stability Check Tab", (Vector2){30, 80,}, 30,2, BLACK);
    int y = 250;
    int x = 100;
    int radius =30;
    int dist_to_increment = 3*radius;

    //test matrix
    int adj[3][3] = {
        {0, 2, 0},
        {2, 0, 2},
        {0, 2, 0}
    };

    // skal kun køre en gang, derfor sentinel TEST "C=O=C
    if (!val_flag && inputValid && !moleculeLoaded) {
        run_valence_check(count_atoms(smilesInput), smilesInput,adj);
        val_flag = true;
        moleculeLoaded = true;
    }
        if (moleculeLoaded && molecule != NULL && inputValid) {
           for (int i = 0; i < smiles_input_size; i++) {
            Color atomColor = BLACK;

            if (isalpha(molecule[i].atomChar)){
                char current = molecule[i].atomChar; // hold styr på nuværende atom(kun pga readability)
                if (current == 'O') atomColor = RED;
                if (current == 'N') atomColor = BLUE;
                if (current == 'S') atomColor = YELLOW;
                if (current == 'P') atomColor = ORANGE;
                if (current == 'H') atomColor = WHITE;
                DrawCircleLines(x, y, radius, atomColor); // tegn cirkel, x += i slutningen af loopet for at rykke cirkel
                DrawTextEx(uiFont, TextFormat("%c", molecule[i].atomChar),
                (Vector2){x - 4, y - 12}, 15, 2, BLACK); // atom karakter under
                DrawText("Bonds", x-15, y+60, 10, BLACK); //antal bindinger under
                DrawText(TextFormat("%d",  molecule[i].bondAmount),
                x, y + 40, 10, BLACK);
                if (molecule[i].illegalValence == 1) {
                    DrawTextEx(uiFont,"ERROR", (Vector2){x-15, y+80,}, 15,2, softRed);
                }
                else {
                    DrawTextEx(uiFont,"VALID", (Vector2){x-15, y+80,}, 15,2, DARKGREEN);
                }
                if (i < smiles_input_size - 1 && (molecule[i+1].atomChar == '-' || isalpha(molecule[i+1].atomChar))) {
                    DrawLine(x+radius,y,x+dist_to_increment-radius,y,BLACK);
                }
                if (i != smiles_input_size-1 && molecule[i+1].atomChar == '=') {
                    DrawLine(x+radius,y+2,x+dist_to_increment-radius,y+2,BLACK);
                    DrawLine(x+radius,y,x+dist_to_increment-radius,y,BLACK);
                }
                if (i != smiles_input_size-1 && molecule[i+1].atomChar == '#') {
                    DrawLine(x+radius,y+4,x+dist_to_increment-radius,y+4,BLACK);
                    DrawLine(x+radius,y+2,x+dist_to_increment-radius,y+2,BLACK);
                    DrawLine(x+radius,y,x+dist_to_increment-radius,y,BLACK);
                }
                x+= dist_to_increment;
           }

           }
        }


}


void DrawTab_AlgorithmVisualization()
{
    DrawText("Algorithm Visualization Tab", 30, 80, 25, BLACK);

    if (GuiButton((Rectangle){30, 130, 120, 30}, "Start"))
    {
        //Har ingen ide hvordan det her skal implementeres,
    }

}

void DrawTab_GraphView()
{
    DrawText("Graph View Tab", 30, 80, 25, BLACK);

    //Skal laves senere

}

void Clear() {
    smilesInput[0] = '\0';

    inputValid = false;
    moleculeValidated = false;
    moleculeLoaded = false;
    val_flag = false;
    end_flag = false;


    smiles_input_size = 0;
    atom_count = 0;
    smiles_size = 0;

    free(atomIndices);
    free(molecule);
    atomIndices = NULL;
    molecule = NULL;


}
