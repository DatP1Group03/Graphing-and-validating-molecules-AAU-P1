#include <stdio.h>

#include "bfs_matrix.h"
#include "dfs_matrix.h"
#include "Input/validation.h"
#include "Adjacency_matrix.h"
#include "valence_check.h"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

/* Links indtil videre fundet:
 * https://hackage.haskell.org/package/h-raylib-5.1.1.0/src/raylib/examples/shapes/raygui.h
 * https://www.raylib.com/cheatsheet/cheatsheet.html
 * https://github.com/raysan5/raygui/blob/master/src/raygui.h
 * */

/* Følgende er en forklaring der nok burde være i rapporten men for min egen læring (og tænker også alle andre da
 * det er vigtigt til eksamen at vi alle ved hvordan raylib og raygui fungerer.
 * Raylib er et bibliotek som bliver brugt til grafik, input, lyd og vinduer (det er beregnet til at lave spil).
 * Det fungerer ved at der er en hoved-løkke som kører 60 gange i sekunder. Dette er vores while(!WindowShouldclose()).
 * hver iteration af denne løkke er lig med en frame i vores vindue. Raylib er lavet således at man "tegner" ting, og
 * dette gøres altså fra scraft hver eneste gang den laver en iteration.
 * Raylib håndterer følgende: tekst, linjer, cirkler, billeder, 3D modeller, sprites
 * Raygui: knapper, tekstbokse, tabs, sliders, labels og checkboxes.
 * raygui kræver egentlig ikke ekstra, det bruger bare raylibs tegninger.
 * Raygui er anderlees end klassisk UI-frameworks ved at der ingen UI-objekter er, istedet kalder man på funktioner, der
 * både tegnet UI-elementet og opdateret state, og returenere en værdi af en interaktion.
 * En knap lavet på følgende måde:
* if (GuiButton((Rectangle){10, 10, 100, 30}, "Click me")) {
// Knap blev trykket i denne frame
}
*altså når bruger trykket på knappen så vil funktionen Guibutton returener true.
*
**/

/* raylib og raygui, er bygget op på ideen om at alle "states" styres af brugeren, derfor placeres programstate og UI
 * konstanter såsom farvedefinitioner osv. som globale variable der er tilgængelige i hele loopet.
 * Følgende er programmets konstanter. */
#define MaxInput 256 /* Dennes formål er oprette en konstant værdi her 256, som skal bruges  som bufferstørrelse til tekstinput
her tillader vi maksimalt 256 karakterer. Raygui tekstbokse kræver en fat bufferlængde.
"“I implementeringen af GuiTextBox() i raygui anvendes en char-array buffer, som brugeren skal stille til rådighed. Biblioteket modtager pointer til buffer og dens kapacitet, og det er op til brugeren at sikre, at bufferen er tilstrækkelig stor.”"
*/

/*fra raygui.h (https://github.com/raysan5/raygui/blob/master/src/raygui.h)  er der følgende forklaring på dette:
RAYGUI STYLE (guiStyle)
raygui uses a global data array for all gui style properties (allocated on data segment by default),
*       when a new style is loaded, it is loaded over the global style... but a default gui style could always be
*       recovered with GuiLoadStyleDefault() function, that overwrites the current style to the default one
*
*       The global style array size is fixed and depends on the number of controls and properties:
*
*           static unsigned int guiStyle[RAYGUI_MAX_CONTROLS*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED)];
*
*       guiStyle size is by default: 16*(16 + 8) = 384 int = 384*4 bytes = 1536 bytes = 1.5 KB
*
*       Note that the first set of BASE properties (by default guiStyle[0..15]) belong to the generic style
*       used for all controls, when any of those base values is set, it is automatically populated to all
*       controls, so, specific control values overwriting generic style should be set after base values
*
*       altså raygui har et globalt array som er 2 dimensionelt, hver "control-type" har et tal-ID og hver style-egenskab har et tal-id.
*       så når vi gerne vil have tabs i vores program så jf. raygui.h så disse egenskaber i index 17 (control id).  og derfor definerer vi tab til 17.
*       så når vi senere hen vil sætte "stilen" for hvordan programmet skal tegnes så kan vi vha. af
*       GuiSetStyle(TAB, BASE_COLOR_FOCUSED, 0x4CAF50FF);
*       og derfor ved guisetstyle automatisk at vi vil "finde style-arrayey for kontrol nr 17 (tab), og skriv 0x4CAF50FF ind på property-indekseet
*       BASE_COLOR_FOCUSED.
*       */
#define TAB  17
#define TAB_BASE_COLOR_NORMAL   14 // property index for "normal background color"
#define TAB_BASE_COLOR_FOCUSED 17 // property index for "focused background color"
#define TAB_TEXT_COLOR_NORMAL   15 // property index for "normal text color"
#define TAB_TEXT_COLOR_FOCUSED  18 // property index for focused text color"

/* FORKLARING PÅ HVAD NORMAL OG FOCUSED BETYDER:
 * ud fra raygui.h så er dette en enum type. Raygui bruger 4 style states for alle ui-elementer.
 * normal, focused, pressed, disable.
 *
 * Normal: dette er standardtilstanden, hvor:
 * - knappen/tabben/textboxen er synlig
 * - brugeren peger ikke på den
 * - den er ikke aktiv/valgt
 * - der sker ikke noget med musen over den
 * - altså normal er hvordan UI'et ser ud i hvile
 *
 * Focused: Ui-elementet er fremhævet, fordi brugeren peger på det altså:
 * - musen er over elementet (hover)
 * - eller elementet er det "aktive element i keyboard-navigation
 * Dette bruges til:
 * - highlight
 * - hovereffekt
 * - "det element du er ved at interagere med"
 *
 * PRESSED: UI-elementet er i state "der trykkes på det"
 * altså user holder musknappen nede, knappen er i aktivt brug
 * dette bruges til visuel feedback ved klik, f.eks. at knappen bliver mørkere farve når den bliver klikket.
 *
 * DISABLED: UI-elementet er slået fra og er ikke synlig for brugeren.
 */
/* følgende definerer nogle farve. i vores raylib kan man definerer farvestruktur ud fra følgende struct:
* typedef struct Color {
unsigned char r;
unsigned char g;
unsigned char b;
unsigned char a;    // alpha (gennemsigtighed)
} Color;
* så her definerer vi nogle farver ug fra rgb og hvor gennemsigtig farven skal være. */
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
    /* Initwindow (fra RAYLIB) åbner et OS-vindue (x11/wayland på linux, win32 på windows, cocoa på macos). Initiliaserer opengl-context
     * forbedereder raylib til at tegne
     * sætter vinduets størrelse (900,600)
     * sætter titlen i titellinjen vha. af string
     * nulstiller alle interne state (input-buffer, frame timer textures, fonts osv).  */
    InitWindow(900, 600, "S-SMILES");
    SetTargetFPS(60); // fortæller raylib, at hovedløkken skal forsøge at køre 60 frames per second (FPS). Raylib måler tiden for hver frame, kalder WaitTime(), sørge for at løkken aldrig kører hurtigere end den angivne fps
    TraceLog(LOG_INFO, "CWD: %s", GetWorkingDirectory());
    /* tracelog er raylibs indbyggede logging-funktion, som bruges til at skrive debug- eller statusbeskeder til terminalen.

    Den svarer til printf, men med ekstra features:

    forskellige log-niveauer (INFO, WARNING, ERROR, DEBUG)

    farvet output i terminalen

    mulighed for global log-filtering

    konsistent logging på alle platforme

    Kort sagt:DEN gør at når vi kører fra clion kan vi se f.eks. "INFO Validate pressed. INPUT: CCCC | Valid=1 */

    /* følgende par linjer kode har med vores tab. VI har først vores activeTab som er en variable der skal holde styr
     * på hvilken tab der er aktiv i GUI'en. Dette gøres ud fra en char array med stringe som svarer til titlerne. her
     * starter vi altså med index 0 (activetab = 0) som betyder vi starter på input validation.  */
    int activeTab = 0;

    /* tabs er et statisk array, hvor hvert element er en strengpeger (const char*).
     * Selvom arrayet ikke selv er en pointer-til-pointer, decayer det automatisk til const char** når det bruges som argument til en funktion.
     * Dette gør det muligt for raygui at modtage tab-tekster som en liste af C-strings. */
    /* forklaring skrevet selv: raygui initiliaserer tabs vha. guiTabBar, denne forventer følgende
     * GuiTabBar(Rectangle bounds, const char **text, int count, int *active);
     * altså vi har en liste af string disse er af typen char *. En liste af tekster er derfor char *[N], når det gives som funktion-argument så i c
     * repræsenteres det som char**.
     *
     */
    const char *tabs[] = {
        "Input Validation",
        "Adjacency Matrix",
        "Stability",
        "Algorithms",
        "Graph View",
    };
    int tabCount = sizeof(tabs) / sizeof(tabs[0]); // finder hvor mange elementer vi har i vores char tabs array.

    uiFont = LoadFontEx("rec/georgia.ttf", 30, NULL, 0); // loader fonten georgia.ttf med størrelse 30 px. NULL,0 betyder at reaylib selv genere glyphs for standard ASCII.
    GuiSetFont(uiFont); // istedet for standard font så er det georgia der skal bruges.

    // Set these once (global UI styles)
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15); // Sætter tekststørrelsen for alle widgets til 15 px.
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1); // Alle widgets får en kant/border på 1 pixel.
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x555555ff); // Definerer farven på kanter/linjer i GUI’en.  0x555555ff = mørk grå (hex: R=55, G=55, B=55, A=FF)

    /* Style for TAB-widget specifikt */
    GuiSetStyle(TAB, TEXT_PADDING, 8); // Indre padding i fanerne (afstand mellem tab-kant og teksten) = 8 px.
    GuiSetStyle(TAB, BORDER_WIDTH, 1); // Tab-elementerne får en 1 px border (kan være forskellig fra default).
    GuiSetStyle(TAB, BASE_COLOR_NORMAL, 0xE8E8E8FF); // Når en tab ikke er valgt, er dens baggrundsfarve  0xE8E8E8FF = lys grå (normal state).
    GuiSetStyle(TAB, BASE_COLOR_FOCUSED, 0xC0FAC0FF); // Når en tab er valgt/hover, bruges 0xC0FAC0FF = lys grøn.
    GuiSetStyle(TAB, TEXT_COLOR_FOCUSED, 0x006600FF); // Tekstfarven i den aktive faneblad bliver mørk grøn #006600.





    while (!WindowShouldClose()) // Hoved-loop: kører indtil brugeren lukker vinduet (ESC, kryds, osv.)
    {
        BeginDrawing();  // Starter en ny frame – alt tegning mellem BeginDrawing/EndDrawing
        ClearBackground((Color){ 255, 250, 250 }); // Sætter baggrundsfarven (meget lys rosa/hvid)

        // Gemmer de nuværende styles for BUTTON, så vi kan restore dem senere
        int oldBaseNormal = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
        int oldBaseFocused = GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED);
        int oldTextNormal = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);

        // Midlertidig style til "CLEAR"-knappen (rød med hvid tekst)
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xCC4444FF);   // Normal baggrundsfarve: mørk rød
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xDD6666FF);  // Når hover/focus: lidt lysere rød
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF); // Tekstfarve: hvid

        // Definerer rektanglet (position og størrelse) for CLEAR-knappen
        Rectangle clearTab = {740, 20, 140, 30};
        /* rectangle er en struct i raylib som ser ud på følgende:
        * typedef struct Rectangle {
        float x;
        float y;
        float width;
        float height;
        } Rectangle;
        hvor felterne er: x: horisontal position (pixel fra venstre)
        y vertikal position (pixel fra toppen)
        width (bredde i pixel)
        height - højde i pixel) */

        // Tegner en knap med teksten "CLEAR". Hvis den klikkes, kaldes Clear()
        if (GuiButton(clearTab, "CLEAR")) {
            Clear();
        }
        /* syntax er bool GuiButton(Rectangle bounds, const char *text);
        Den gør to ting i én funktion:
        1. Tegner knappen på skærmen med den givne tekst
        2. Returnerer true i dén frame hvor knappen blev klikket
        så når brugeren har klikket så er sætnigen true og så kører vi clear().
        Clear er vores egen funktion og er egentlig blot en funktion som sætter alle vores variable i programmet tilbage fra start (ingen matrix osv).
        */
        // Keyboard-genvej: CTRL + Z gør det samme som at trykke CLEAR
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
            Clear();
        }
        // Restore de gamle styles, så andre knapper ikke arver den røde stil
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, oldBaseNormal);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, oldBaseFocused);
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, oldTextNormal);
        // TAB-styles: vi styler fanebladene i tabbaren

        // Sæt standard baggrundsfarve for tab (ikke valgt) til grå
        GuiSetStyle(TAB + 0, TAB_BASE_COLOR_NORMAL, 0xCCCCCCFF); // default gray
        // (TAB + 0 bruges fordi raygui bruger “controls + state” som indeksering – TAB er basis)
        // Hvis input er gyldigt, gør vi tab'en grøn når den er aktiv/focused
        if (inputValid)
        {
            GuiSetStyle(TAB + 0, TAB_BASE_COLOR_FOCUSED, 0x4CAF50FF);
        }

        // Tegner selve tab-baren (faneblade)
        GuiTabBar(
            (Rectangle){20, 20, 860, 30},
            tabs,
            tabCount,
            &activeTab
        );
        // Alt efter hvilken tab der er aktiv, kalder vi en “draw”-funktion for den side
        switch (activeTab)
        {
            case 0: DrawTab_InputValidation(); break;
            case 1: DrawTab_AdjacencyMatrix(); break;
            case 2: DrawTab_StabilityCheck(); break;
            case 3: DrawTab_AlgorithmVisualization(); break;
            case 4: DrawTab_GraphView(); break;
        }

        EndDrawing(); // Afslutter framen og viser den på skærmen
    }

    CloseWindow(); // Rydder raylib op og lukker vinduet
    return 0;
}

void DrawTab_InputValidation()
{
    // Overskrift til tabben
    DrawTextEx(uiFont,"Input Validation Tab", (Vector2){30,80},30,2, BLACK);
    /* vector2 er en struct med to floats i raylib, som bruges til at angive 2d-posiitioner i raylib er der defineret:
    typedef struct Vector2 {
    float x;
    float y;
} Vector2;
*/
    // Label foran tekstfeltet
    GuiLabel((Rectangle){30, 130, 120, 25}, "SMILES Input:");

    // Tekstboks til SMILES-input
    if (GuiTextBox((Rectangle){160, 130, 300, 25}, smilesInput, 256, editMode))
        editMode = !editMode;
    /*
      GuiTextBox:
      - Tegner tekstfeltet
      - Skriver brugerens input ind i `smilesInput`
      - Returnerer true, når brugeren "afslutter" editing (Enter/tab/click)
      - Her: når den returnerer true, toggler du editMode, altså guitextbox() bliver true i den frame hvor brugeren afslutter
      tekstredigeringen ved at enten gør følgende: tab, enter eller klikke udenfor tekstfeltet.
    */

    // Validate-knap
    if (GuiButton((Rectangle){470, 130, 100, 25}, "Validate"))
    {
        moleculeValidated = true;
        inputValid = validate_smiles(smilesInput); // smilesInput var den variable vi brugt i textbox, så vi får teksten fra boxen og validerer.
        TraceLog(LOG_INFO, "Validate pressed. Input: %s | Valid=%d", smilesInput, inputValid); //TraceLog er en måde i raylib at skrive i terminalen, sådan når man skriver og tester koden så kan vi følge med hvad der sker i programmet.
    }
    /*
      Når brugeren klikker "Validate":
      - Sætter moleculeValidated = true → nu må vi vise resultatet
      - Kalder validate_smiles(...) → sætter inputValid til true/false
    */

    // Hvis der ER forsøgt valideret
    if (moleculeValidated) {

        if (inputValid ) {
            // GRØN succes-tekst + liste over checks der er OK
            DrawTextEx(uiFont, "Valid SMILES", (Vector2){30, 180}, 30, 0, goodGreen);

            DrawTextEx(uiFont, "INPUT SIZE DOES NOT EXCEED MAX_INPUT OF 100", (Vector2){30, 230}, 15, 0, goodGreen);
            DrawTextEx(uiFont, "VALID PARENTHESES", (Vector2){30, 250}, 15, 0, goodGreen);
            DrawTextEx(uiFont, "NO ILLEGAL CHARACTERS", (Vector2){30, 270}, 15, 0, goodGreen);
            DrawTextEx(uiFont, "NO UNCLOSED RINGS", (Vector2){30, 290}, 15, 0, goodGreen);
        }
        else {
            // RØD fejl-overskrift
            DrawTextEx(uiFont, "INVALID SMILES:", (Vector2){30, 180}, 25, 2, softRed);

            int count = get_error_count();  // (du bruger faktisk ikke 'count' her) - SKAL DET FJERNES?

            if (!inputValid) {
                // Du tegner "INVALID SMILES:" én gang til her (det er lidt dobbelt)
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
        // Før man har trykket validate:
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

