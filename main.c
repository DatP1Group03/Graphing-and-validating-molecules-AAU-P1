#include <stdio.h>

#include "bfs_matrix.h"
#include "dfs_matrix.h"
#include "Input/validation.h"
#include "Adjacency_matrix.h"
#include "valence_check.h"

#include "external/raylib/src/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



#define MaxInput 256
#define TAB  17
#define TAB_BASE_COLOR_NORMAL   14
#define TAB_BASE_COLOR_FOCUSED 17
#define TAB_TEXT_COLOR_NORMAL   15
#define TAB_TEXT_COLOR_FOCUSED  18
Color goodGreen = (Color){ 0, 180, 120, 255 };
Color softRed   = (Color){ 200, 60, 60, 255 };
Color grayText  = DARKGRAY; //DARKGRAY er en foruddefineret farve fra raylib, og i raylib er den prædefineret til #define DARKGRAY (Color){ 80, 80, 80, 255 }

//bfs
#define MAX_ATOMS 128
static int bfsOrder[MAX_ATOMS];
static int bfsCount = 0;
static bool bfsRan = false;
static bool dfsRan = false;
static char bfsLog[4096];   // tekstlog til GUI


static char smilesInput[256] = {0}; // måske skulle denne istedet for 256 så være defineret til maxinput??
/* det er vigtigt at vores smilesinput er static, fordi at husk på at vores loop kører 0 gange i sekunder, og dermed
 * så hvis ikke vores smilesinput var static så ville det simpelthen blive nulstillet for hvert frame */
static bool inputValid = false; // dette værdi som sættes når vi har valideret den i vores tab "Input validation". Variablen kan bruges i andre faner, f.eks. giver det ikke mening at forsøge at lave matrixe hvis vores streng er forkert.
static bool editMode = false; /* Raygui's tekstbokse bruger en "editMode" tilstand. Når editmode = true så betyder det
at tekstboksen er aktiv, og tasturet skriver i den. når editmode= false, så er den inaktiv, og keyboard-input
går istedet til raylib (hotkeys) denne skal altså bruges til vores boks hvor vi skriver strengen. Igen er det vigtigt at den er static
således at den ikke nulstilles for hver gang. */
// static bool answered = false; anvendes ikke i koden? har kommenteret den ud nu. Skal eventuelt slettes.
static bool val_flag = false; // dette "flag" skal sikre at valence check kun kører en gang.
// static bool end_flag = false; anvendes ikke i koden? har kommenteret den ud, men kan kun finde den i clear hvor den igen sættes til false
static bool moleculeLoaded = false; // denne fortæller om molekylet er generet og klar, anvendes som state i stability, algorithm og graph view.
static bool moleculeValidated= false; //fortæller om valideringen ER blevet udført (ikke om den var gyldig!)
Font uiFont; /* Font er en struct type som er defineret i raylib og ser sådan her ud:
typedef struct Font {
int baseSize;
int glyphCount;
int glyphPadding;
Texture2D texture;      // texture atlas containing the glyphs
Rectangle *recs;        // rectangles for each glyph
GlyphInfo *glyphs;      // info for each glyph (codepoint, offsets…)
} Font;
og vi laver dermed en variabel ved navn uiFont. Denne skal anvendes når vi initiliaserer vores vindue. */


// Tab functions
void DrawTab_InputValidation();
void DrawTab_AdjacencyMatrix();
void DrawTab_StabilityCheck();
void DrawTab_AlgorithmVisualization();
void DrawTab_GraphView();
void Clear();



int main(void)
{

    int adj[6][6]; // Initalisere 2d array med countAtoms(smiles) som dimensioner
    create_adjacency_matrix("CC(OO)CC",6,adj); // Convert til adj
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            printf("%d ", adj[i][j]);
        }
        printf("\n");
    }

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

        val_flag = false;
        moleculeLoaded = false;

        TraceLog(LOG_INFO, "Validate pressed. Input: %s | Valid=%d", smilesInput, inputValid);
        inputValid = validate_smiles(smilesInput); // smilesInput var den variable vi brugt i textbox, så vi får teksten fra boxen og validerer.
        TraceLog(LOG_INFO, "Validate pressed. Input: %s | Valid=%d", smilesInput, inputValid); //TraceLog er en måde i raylib at skrive i terminalen, sådan når man skriver og tester koden så kan vi følge med hvad der sker i programmet.
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
    // Overskrift til tabben
    DrawText("Adjacency Matrix Tab", 30, 80, 25, BLACK);

    // Hvis input ikke er gyldigt, giver det ingen mening at vise matrix
    if (!inputValid) {
        DrawTextEx(uiFont,
            "Please enter and validate a valid SMILES in the Input tab first.",
            (Vector2){30,130}, 18,2, RED);
        return; // vi returner her fordi vi skal ikke tegne mere hvis ikke den er valid, funktionen skal stoppe.
    }

    // Hvis den er valid så skal vi have lavet adjacency matrix ud fra adjacency_matrix.h
    int atom_count = get_atom_count(smilesInput);
    // vi laver vores adjacency_matrix ud fra atom_count i det vi godt kan kende størrelsen
    int adjacency_matrix[atom_count][atom_count];

    create_adjacency_matrix(smilesInput, atom_count, adjacency_matrix);

    // nu har vi egentlig matrixen så nu skal vi egentlig "blot" have tegnet den som en tabel. men dette gøres egentlig som vi ville have gjort som et printf i terminalen
    int startX = 80; // hvor vi ønsker tabllen starter i x
    int startY = 170;
    int cell_size = 30; // størrelsen på hver "celle" hvis man så det ligesom et excelark

    // jeg har her lavet en kolonne overskrift til at starte med, fordi så synes jeg det er nemmere at se ift om indeksering sker korrekt. Dette kan altid ændres
    for (int col = 0; col < atom_count; col++) {
        DrawText(TextFormat("%d", col), // TEXT format er ligesom raylibs version af printf (nemmere sprintf).
            startX + (col +1)*cell_size, startY, 18, BLACK);

        // vi vil også gerne have en linje lige under.  der kan anvendes DrawLineEx (som anvender vector, men vi kan her vælge tykkelse derfor vælges denne).
        // de ekstra +20 jeg har sat på et grundet fontsize er 18 så vi skal forbi talenne.
        DrawLineEx((Vector2){startX, startY+20}, (Vector2){startX +20+ (col +1)*cell_size, startY+20},2, BLACK);
    }

    // Række-overskrifter
    for (int row = 0; row < atom_count; row++) {
        // Række-label
        DrawText(TextFormat("%d", row),
                 startX,
                 startY + (row + 1) * cell_size,
                 18,
                 BLACK);

        //linje for at adskille række label fremfor selve matrixen
        DrawLineEx((Vector2){startX+20, startY+20}, (Vector2){startX+20, startY+20 + (row + 1) * cell_size},2, BLACK);


        // Celler i rækken
        for (int col = 0; col < atom_count; col++) {
            int value = adjacency_matrix[row][col];

            DrawText(TextFormat("%d", value),
                        startX + (col + 1) * cell_size,
                        startY + (row + 1) * cell_size,
                        18,
                        BLACK);
        }
    }

    // lille forklarende tekst der forklarer atom count og hvilket input vi ser for:
    DrawTextEx(uiFont,
               TextFormat("Atoms: %d   SMILES: %s", atom_count, smilesInput),
               (Vector2){30, startY + (atom_count + 2) * cell_size},
               18,
               2,
               DARKGRAY);

}
void DrawTab_StabilityCheck()
{
    DrawTextEx(uiFont,"Stability Check Tab", (Vector2){30, 80,}, 30,2, BLACK);
    int y = 250;
    int x = 100;
    int radius =30;
    int dist_to_increment = 3*radius;

    int *adj = find_adjacency(smilesInput, atom_count);
    create_adjacency_matrix(smilesInput, atom_count, adj);

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


void DrawTab_AlgorithmVisualization() {
    //tegner blot titlen
    DrawText("Algorithm Visualization Tab", 30, 80, 25, BLACK);

    // det er også et krav her at der et gyldigt input. den returner altså man kan ikke trykke på noget her.
    if (!inputValid) {
        DrawText("Please enter a valid SMILES on the first tab.", 30, 130, 20, RED);
        return;
    }

    // Knap til at starte BFS
    // har lavet en variabel der hedder bfsRan (defineret på linje 118) den gør at teksten afhænger om man har kørt BFS eller ikke. Hvis true så står der run BFS again, hvis false (standard) så står der blot BFS run.
    if (GuiButton((Rectangle){30, 130, 160, 30}, bfsRan ? "Run BFS again" : "Run BFS")) {
        bfsRan = true; // sættes til true så resten af UI'et ved at vi har fået et resultat.
        dfsRan = false;
    }

    // Knap til starte DFS
    if (GuiButton((Rectangle){200, 130, 160, 30}, dfsRan ? "Run DFS again" : "Run DFS")) {
        bfsRan = false;
        dfsRan = true; // sættes til true så resten af UI'et ved at vi har fået et resultat.
    }


    if (!bfsRan && !dfsRan ) {
        DrawText("Press 'Run BFS' or 'Run DFS' to generate traversal and explanation.", 30, 190, 20, DARKGRAY);
        return;
    }

    //hvis vi kommer hertil så er det fordi bfsRan er true.
    if (bfsRan) {
        // vi skal have lavet vores adjacency matrixe ( ikke sikkert at vi har været forbi tabben adjacency matrix).
        int atom_count = get_atom_count(smilesInput);
        if (atom_count <= 0) { // blot en lille sikring er egentlig irrelevant da hvis ingen valid input så kommer man aldrig hertil
            bfsLog[0] = '\0';
            bfsCount = 0;
            bfsRan = false;
            return;
        }
        if (atom_count > MAX_ATOMS) atom_count = MAX_ATOMS;  // sikkerhed

        // Lav VLA adjacency matrix
        int adjacency_matrix[atom_count][atom_count];
        int bfs[atom_count];
        create_adjacency_matrix(smilesInput, atom_count, adjacency_matrix);
        bfs_matrix_drawtext_ONLYFORUSEINRAYGUI(atom_count, adjacency_matrix, 0, bfs, 0);
    }

    if (dfsRan) {
        // vi skal have lavet vores adjacency matrixe ( ikke sikkert at vi har været forbi tabben adjacency matrix).
        int atom_count = get_atom_count(smilesInput);
        if (atom_count <= 0) { // blot en lille sikring er egentlig irrelevant da hvis ingen valid input så kommer man aldrig hertil
            bfsLog[0] = '\0';
            bfsCount = 0;
            bfsRan = false;
            return;
        }
        if (atom_count > MAX_ATOMS) atom_count = MAX_ATOMS;  // sikkerhed

        // Lav VLA adjacency matrix
        int adjacency_matrix[atom_count][atom_count];
        create_adjacency_matrix(smilesInput, atom_count, adjacency_matrix);
        int dfsmatrix[atom_count];
        int visited[atom_count];
        int parent[atom_count];

        for (int i = 0; i < atom_count; i++) {
            parent[i] = -1;
        }

        int lineheight = 0;
        dfs_matrix_onlyforgui(0, atom_count, adjacency_matrix, dfsmatrix, visited, parent, 0, &lineheight);
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
    // end_flag = false; se linje 125

    smiles_input_size = 0;
    atom_count = 0;
    smiles_size = 0;

    free(atomIndices);
    free(molecule);
    atomIndices = NULL;
    molecule = NULL;

    bfsRan = false;
    dfsRan = false;
    bfsCount = 0;
    bfsLog[0] = '\0';
}

