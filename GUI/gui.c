//
// Created by magnus on 29/11/2025.
//

//Windows
// #include "external/raylib/src/raylib.h"

#include <stdio.h>
#include "gui.h"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "bfs_matrix.h"
#include "Input/validation.h"
#include "Adjacency_matrix.h"
#include "dfs_matrix.h"
#include "drawBoxedText.h"
#include "valence_check.h"
#include "toxicphore.h"
#include "Graph_representation.h"
#include "Graph_representation/Graph_representation.h"

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
#define TAB_BASE_COLOR_NORMAL   14
#define TAB_BASE_COLOR_FOCUSED 17
#define TAB_TEXT_COLOR_NORMAL   15
#define TAB_TEXT_COLOR_FOCUSED  18

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


//infobokse

static const char *smilesValidationText =
	"The SMILES validation module analyzes the input string through several "
	"linear scans (for-loops), each running in O(n) time. The first pass checks "
	"that every character is part of the allowed SMILES symbol set. A second "
	"scan uses simple stack logic to ensure that parentheses and brackets are "
	"properly opened and closed,";


static char smilesInput[256] = {0}; // måske skulle denne istedet for 256 så være defineret til maxinput??
/* det er vigtigt at vores smilesinput er static, fordi at husk på at vores loop kører 0 gange i sekunder, og dermed
 * så hvis ikke vores smilesinput var static så ville det simpelthen blive nulstillet for hvert frame */

static char substructures_input[256] = {0};
bool showValidationInfo = false;
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

// følgende funktioner er til substructure funktionen 
static int pressedsubstructures = 0; 
static int substructure_test = 0; 

/* følgende funktion skal kører hele loopet. Således at man blot kalder på denne indtil den bliver 0. Dette gør det nemt at kører programmet blot fra terminalen */
int runGUI() {
    /* Initwindow (fra RAYLIB) åbner et OS-vindue (x11/wayland på linux, win32 på windows, cocoa på macos). Initiliaserer opengl-context
     * forbedereder raylib til at tegne
     * sætter vinduets størrelse (900,600)
     * sætter titlen i titellinjen vha. af string
     * nulstiller alle interne state (input-buffer, frame timer textures, fonts osv).  */

    InitWindow(1200, 1000, "S-SMILES");
    SetTargetFPS(60); // fortæller raylib, at hovedløkken skal forsøge at køre 60 frames per second (FPS). Raylib måler tiden for hver frame, kalder WaitTime(), sørge for at løkken aldrig kører hurtigere end den angivne fps
    TraceLog(LOG_INFO, "CWD: %s", GetWorkingDirectory());
    /* tracelog er raylibs indbyggede logging-funktion, som bruges til at skrive debug- eller statusbeskeder til terminalen.
    * Den svarer til printf, men med ekstra features:

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
	"Substructures",
    };
    int tabCount = sizeof(tabs) / sizeof(tabs[0]);  // finder hvor mange elementer vi har i vores char tabs array.

    uiFont = LoadFontEx("../GUI/roboto.ttf", 30, NULL, 0);  // loader fonten georgia.ttf med størrelse 30 px. NULL,0 betyder at reaylib selv genere glyphs for standard ASCII.
    GuiSetFont(uiFont); // istedet for standard font så er det georgia der sk

    // Set these once (global UI styles)
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15); // Sætter tekststørrelsen for alle w
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1); // Alle widgets får en kant/bo
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x555555ff); // Definerer farven på kanter/linjer i GUI’en.  0x555555ff = mørk grå (hex: R=55, G=5

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
        Rectangle clearTab = {1005, 20, 140, 30};
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
	    case 5: DrawTab_Substructures(); break; 
        }
	

        EndDrawing(); // Afslutter framen og viser den på skærmen
    }

    CloseWindow(); // Rydder raylib op og lukker vinduet
    return 0;
}

void DrawTab_InputValidation()
{


    GuiToggle((Rectangle){830, 550, 60, 25}, "?", &showValidationInfo);

    if(showValidationInfo){
        DrawTextEx(uiFont,"Input Validation Process", (Vector2){30,80},30,2, BLACK);
        DrawTextBoxed(
        uiFont,
        smilesValidationText,
        (Rectangle){ 40, 140, 820, 280 }, // x, y, width, height
        30,
        2,
        true,
        DARKGRAY
         );


        DrawTextEx(uiFont,"User Example:", (Vector2){50, 480},30,2, BLACK);
        if (!moleculeValidated) {

        }
        else {
            moleculeValidated = true;
            inputValid = validate_smiles(smilesInput);

            val_flag = false;
            moleculeLoaded = false;

            Vector2 start = {250, 480};
            float fontSize = 30;
            float spacing = 2;

            // Tegn Smiles
            DrawTextEx(uiFont, smilesInput, start, fontSize, spacing, BLACK);

            // Bredde af tekst
            Vector2 size = MeasureTextEx(uiFont, smilesInput, fontSize, spacing);

            // Start af linjer er lig med start af smiles + bredde + lidt ekstra plads
            float gap = 12.0f;
            float beginPosx = start.x + size.x + gap;

            // midt y pos
            float midY = start.y + fontSize * 0.5f;

            // Mellemrum imellem linjer
            float dy = 50;

            // STArt
            Vector2 beginPos = {beginPosx, midY};

            // slutpunkter
            Vector2 b1 = {beginPosx + 80, midY - dy};
            Vector2 b2 = {beginPosx + 80, midY};
            Vector2 b3 = {beginPosx + 80, midY + dy};
            Vector2 b4 = {beginPosx + 80, midY + dy * 2};

            // Tegn linjer
            DrawLineV(beginPos, b1, BLACK);
            DrawLineV(beginPos, b2, BLACK);
            DrawLineV(beginPos, b3, BLACK);
            DrawLineV(beginPos, b4, BLACK);

            if (is_permitted(smilesInput)) {
                DrawTextEx(uiFont, "PERMITTED CHARACTERS",
                           (Vector2){ b1.x + 20, b1.y - 20 }, fontSize - 10, spacing, GREEN);
            } else {
                DrawTextEx(uiFont, "ILLEGAL CHARACTER",
                           (Vector2){ b1.x + 20, b1.y - 20 }, fontSize - 10, spacing, RED);
            }


            if (closed_brackets(smilesInput)) {
                DrawTextEx(uiFont, "BALANCED PARENTHESES",
                           (Vector2){ b2.x + 20, b2.y - 20 }, fontSize - 10, spacing, GREEN);
            } else {
                DrawTextEx(uiFont, "UNBALANCED PARENTHESES",
                           (Vector2){ b2.x + 20, b2.y - 20 }, fontSize - 10, spacing, RED);
            }


            if (ring_closed(smilesInput)) {
                DrawTextEx(uiFont, "RING STRUCTURE VALID",
                           (Vector2){ b3.x + 20, b3.y - 20 }, fontSize - 10, spacing, GREEN);
            } else {
                DrawTextEx(uiFont, "INVALID RING CLOSURE",
                           (Vector2){ b3.x + 20, b3.y - 20 }, fontSize - 10, spacing, RED);
            }


            if (misc_check(smilesInput)) {
                DrawTextEx(uiFont, "MISC STRUCTURE VALID",
                           (Vector2){ b4.x + 20, b4.y - 20 }, fontSize - 10, spacing, GREEN);
            } else {
                DrawTextEx(uiFont, "MISC STRUCTURE ERROR",
                           (Vector2){ b4.x + 20, b4.y - 20 }, fontSize - 10, spacing, RED);
            }
        }
        inputValid = validate_smiles(smilesInput);
    }


    if (!showValidationInfo) {

        DrawTextEx(uiFont,"Input Validation Tab", (Vector2){30,80},30,2, BLACK);

        GuiLabel((Rectangle){30, 130, 120, 25}, "SMILES Input:");
        if (GuiTextBox((Rectangle){160, 130, 300, 25}, smilesInput, 256, editMode)){
        editMode = !editMode;
    }

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

	/* Scroll opsætning til matrixen */ 
	static Vector2 scroll = {0,0}; // static fordi den skal huskes mellem frames. dermed skal den kun initiliaserer en gang. Scroll vil løbende blive ændret, men skal beholdes mellem hver frame.  
	
	int screenW = GetScreenWidth(); // henter størrelsen på hele vinduet, dette gør at vi ikke skal huske at justere denne hvis vi ændrer vinduet.  
	int screenH = GetScreenHeight(); 

	// hvor på skærmen scroll-området skal være
	Rectangle bounds = {
		30, // x -position - scrollpanelet starter 30 pixels fra venstre kant 
		140, // y-position - scrollpanalet starter 140 pixel fra toppen - så der er plads til vores titel osv. 
		screenW - 60, // bredde (lidt margin i siderne (60 grundet vores position 30 til hver side.) 
		screenH - 180, // højde (så der er plads til overskrift osv). 
	}; 

	int cell_size = 30; 

	// hvor stort indholdet potentielt er (matrix + lidt ekstra plads) 
	int contentWidth = (atom_count + 2) * cell_size +80 ; // vi har atom_count kolonner +2 (en kolonne til række-indeks, en til "luft") - og vores celle er 30 pixel bred og høj, dernæst har vi 80 så vi ikke rammer kanten med tekst. 
	int contentHeight = (atom_count +4) * cell_size +80; 

	// hvor stort indholdet potentielt kan være - hvis content er større end bounds -> scrollbars kommer frem 
	Rectangle content = {
		0, 0,  // origin i panelet
		contentWidth, 
		contentHeight, 
	}; 

	Rectangle view = {0}; // et tomt rektangel, dette skal vores GuiscrollPanel udfylde. Det bliver det synlgie udsnit af indholdet. 

	GuiScrollPanel(bounds, "Adjacency Matrix", content, &scroll, &view); // tegnet scrollpanelets ramme, tegner scrollbarerene (vertikal og horizontal), opdaterer scroll.x og scroll.y i det at man scroller og returnerer i vew det område vi så må tegne i 

	//
	//Tegn Matrizen inde i scroll panelet 
	//
	BeginScissorMode(view.x, view.y, view.width, view.height);

	// startposition for tegningen inde i panelet
	double startX = bounds.x + 60 + scroll.x; // bounds = hvor panelet begynder på skærmen, plus lidt margin +60, og +scroll.x flytter indeholdet med scrollbaren. 
	double startY = bounds.y + 40 + scroll.y; 

	/** Eksempel på hvordan det virker. Lad os sige at vores scrollpanel ligger her bounds = {x=30, y=140, w=840, h=700} 
	 * vi scroller nu 100 pixel ned 
	 * scroll.y = -100 
	 * så vil vores starty for denne frame være 
	 * startY = 140+40+(-100) = 80
	 * */

	// vi laver et array udelukkende med atom symboler i rækkefølge således vi kan paste det ind
	char smile_symbols[atom_count]; 
	fill_atom_symbols_from_smile(smilesInput, smile_symbols, atom_count); 
	// jeg har her lavet en kolonne overskrift til at starte med, fordi så synes jeg det er nemmere at se ift om indeksering sker korrekt. Dette kan altid ændres
    for (int col = 0; col < atom_count; col++) {
        DrawText(TextFormat("%c", smile_symbols[col]), // TEXT format er ligesom raylibs version af printf (nemmere sprintf).
            startX + (col +1)*cell_size, startY, 18, BLACK);

        // vi vil også gerne have en linje lige under.  der kan anvendes DrawLineEx (som anvender vector, men vi kan her vælge tykkelse derfor vælges denne).
        // de ekstra +20 jeg har sat på et grundet fontsize er 18 så vi skal forbi talenne.
        DrawLineEx((Vector2){startX, startY+26}, (Vector2){startX +20+ (col +1)*cell_size, startY+26},2, BLACK);
	
	DrawText(TextFormat("%d", col), 
		startX + (col+1)*cell_size+12, startY+14,
	  	4,
	  	BLACK);
	
        DrawText(TextFormat("%c", smile_symbols[col]),
                 80 + (24*(col+1)),
                 startY + (atom_count+2) * cell_size,
                 18,
                 DARKGRAY);
	
	DrawText(TextFormat("%d", col), 
		90 + (24*(col+1)), 
	  	startY+ 14 + (atom_count+2) * cell_size,
	  	1,
	  	DARKGRAY);
    }
    // Række-overskrifter
    for (int row = 0; row < atom_count; row++) {
        // Række-label
        DrawText(TextFormat("%c", smile_symbols[row]),
                 startX,
                 startY + (row + 1) * cell_size,
                 18,
                 BLACK);
	
        DrawText(TextFormat("%d", row),
                 startX+12,
                 startY + (row + 1) * cell_size+14,
                 4,
                 BLACK);

        //linje for at adskille række label fremfor selve matrixen
        DrawLineEx((Vector2){startX+26, startY+20}, (Vector2){startX+26, startY+20 + (row + 1) * cell_size},2, BLACK);


        // Celler i rækken
        for (int col = 0; col < atom_count; col++) {
            int value = adjacency_matrix[row][col];

            DrawText(TextFormat("%d", value),
                        startX+6 + (col + 1) * cell_size,
                        startY+6 + (row + 1) * cell_size,
                        18,
                        BLACK);
        }
    }

    // lille forklarende tekst der forklarer atom count og hvilket input vi ser for:
   
    DrawTextEx(uiFont,
	       "SMILES:",
               (Vector2){30, startY + (atom_count + 2) * cell_size},
               18,
               2,
               DARKGRAY);

    DrawTextEx(uiFont,
               TextFormat("Atoms: %d", atom_count),
               (Vector2){30, startY+ 35 + (atom_count + 2) * cell_size},
               18,
               2,
               DARKGRAY);
	EndScissorMode(); 
}
void DrawTab_StabilityCheck()
{
    DrawTextEx(uiFont,"Stability Check Tab", (Vector2){30, 80,}, 30,2, BLACK);
    int radius =30;
    int dist_to_increment = 3*radius;

    /* jeg har valgt at kommenterer dette ud, jeg tror det gror på en fejl. create_adjacency kalder selv på
     * find_adjacency. og create_adjacency forventer et array allkoeret med størrelse og ikke en tom pointer.
     * jeg har derfor tilladt mig at ændrer i det.
     *
    int *adj = find_adjacency(smilesInput, atom_count);
    create_adjacency_matrix(smilesInput, atom_count, adj);
    */

    
	// Scroll opsætning til matrixen 
	static Vector2 scroll_stability = {0,0}; 
	int screenW_stability = GetScreenWidth();
	int screenH_stability = GetScreenHeight(); 
	
	Rectangle bounds_stability = {
		30, 
		220,
		screenW_stability - 60, 
		screenH_stability - 260,
	}; 
	
	int contentWidth_stability =atom_count * 90 +200; // bare en værdi jeg har trukket ud af hatten
	int content_height_stability = 200+200; // Den bruger tre linjer pr atom til at forklarer DFS. +80 for lidt margin
	
	Rectangle content_stability = {
		0, 0,
		contentWidth_stability, 
		content_height_stability,
	}; 

	Rectangle view_stability = {0}; 

	GuiScrollPanel(bounds_stability, "Stability", content_stability, &scroll_stability, &view_stability); 
	
	BeginScissorMode(view_stability.x, view_stability.y, view_stability.width, view_stability.height);
    
	int y = bounds_stability.y + scroll_stability.y + 250; // 250 i margin til at starte med 
        int x = bounds_stability.x + scroll_stability.x + 100; 

    int adj[atom_count][atom_count];
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

		EndScissorMode(); 
}


/* The following function is our BFS algorithm. We input the following:
 * int n this is the number of nodes that we are dealing with. This is also why we see
 * that an array of size [n][n] must be input.
 * const int adj[n][n] is our adjacency matrix as input. It is made const so that
 * We are not going to make any changes to it.
 * int src:the node we want to start at. This can range from 0 to n-1
 * int bfs[] is our array that we input into
 * */
int bfs_matrix_drawtext_ONLYFORUSEINRAYGUI(int n, const int adj[n][n], int src, int bfs[]) {
    int visited[n]; //matrix for keeping track of visited nodes. The node is equal to subscript and is indicated if visited with 0 or 1.
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    int queue[n]; // queue is an array consisting of the elements we have seen but not yet visited
    int front = 0, rear = 0; //
    /* front is how many times we have dequeued an element from the queue. so actually how far BFS has come in the queue
     * rear moves when we add to the queue. rear = number of elements in queue + front*/

    int count = 0; // counts how many nodes we have visited.
    
	// Scroll opsætning til matrixen 
	static Vector2 scroll_bfs = {0,0}; 
	int screenW_bfs = GetScreenWidth();
	int screenH_bfs = GetScreenHeight(); 
	
	Rectangle bounds_bfs = {
		30, 
		220,
		screenW_bfs - 60, 
		screenH_bfs - 260,
	}; 
	
	int contentWidth_bfs =1800; // bare en værdi jeg har trukket ud af hatten
	int content_height_bfs = 20* n * 3 + 80; // Den bruger tre linjer pr atom til at forklarer DFS. +80 for lidt margin
	
	Rectangle content_bfs = {
		0, 0,
		contentWidth_bfs, 
		content_height_bfs,
	}; 

	Rectangle view_bfs = {0}; 

	GuiScrollPanel(bounds_bfs, "BFS", content_bfs, &scroll_bfs, &view_bfs); 
	
	BeginScissorMode(view_bfs.x, view_bfs.y, view_bfs.width, view_bfs.height);
    
	int lineheight = bounds_bfs.y + scroll_bfs.y + 30; // 20 i margin til at starte med 
        int x_bfs = bounds_bfs.x + scroll_bfs.x + 10; 
    DrawText(TextFormat("Our start node is %d", src), x_bfs, lineheight, 20, BLACK);
    lineheight += 20;
    visited[src] = 1;
    queue[rear++] = src; // we set queue[back (here 0)] = src, and since we have added an element to the queue then increments back to 1.

    int dont_print_bfs_list_on_first = 1;
    while ( front < rear ) {
        // the following is simply for printing the bfs.
        int no_comma_on_first = 1;

        // Print BFS-listen efter første iteration

        if (!dont_print_bfs_list_on_first) {
            char bfsLine[256];
            int offset = snprintf(bfsLine, sizeof(bfsLine),
                                  "BFS list currently: {");

            for (int j = 0; j < count; j++) {
                offset += snprintf(bfsLine + offset,
                                   sizeof(bfsLine) - offset,
                                   "%s%d",
                                   (j == 0 ? "" : ", "),
                                   bfs[j]);
            }

            snprintf(bfsLine + offset,
                     sizeof(bfsLine) - offset,
                     "}");

            DrawText(bfsLine, x_bfs, lineheight, 20, BLACK);
            lineheight += 20;
        }
        dont_print_bfs_list_on_first = 0;

        // and here we print our queue
        char queueLine[256];
        int qOff = snprintf(queueLine, sizeof(queueLine),
                            "In our queue we currently have the following: {");

        no_comma_on_first = 1;
        for (int i = front; i < rear; i++) {
            qOff += snprintf(queueLine + qOff,
                             sizeof(queueLine) - qOff,
                             "%s%d",
                             (no_comma_on_first ? "" : ", "),
                             queue[i]);
            no_comma_on_first = 0;
        }

        snprintf(queueLine + qOff,
                 sizeof(queueLine) - qOff,
                 "}");

        DrawText(queueLine, x_bfs, lineheight, 20, BLACK);
        lineheight += 20;

        int u = queue[front]; // The first time, queue[0] is set equal to u. U will be used to determine the first element in our array.
        front++; //we now increment our front by 1, since we have just dequeued to u.
        bfs[count] = u; // our queue element which is in u is an element in bfs.
        count++; // we have added a node and therefore our count is advanced.
        char visitLine[256];
        int vOff = snprintf(visitLine, sizeof(visitLine),
                            "We are visiting node %d and it is connected to unvisited elements: {",
                            u);

        no_comma_on_first = 1;
        for (int v = 0; v < n; v++) {
            if (adj[u][v] >= 1 && visited[v] == 0) {
                visited[v] = 1;
                queue[rear++] = v;

                vOff += snprintf(visitLine + vOff,
                                 sizeof(visitLine) - vOff,
                                 "%s%d",
                                 (no_comma_on_first ? "" : ", "),
                                 v);
                no_comma_on_first = 0;
            }
        }

        snprintf(visitLine + vOff,
                 sizeof(visitLine) - vOff,
                 "}");

        DrawText(visitLine, x_bfs, lineheight, 20, BLACK);
        lineheight += 20;
    }
    EndScissorMode(); 

    return count; // number of visited nodes - a quick way to see if visited = n, if they are then the molecule is complete.

}


int dfs_matrix_onlyforgui(int startnode, int n, const int adj[n][n], int dfsmatrix[], int visited[], int parent[], int cycles[][2], int *cycle_count, int count, int *pLineHeight, int baseX, int baseY) {
    // we mark node as visited, by using our matrix visited, we also insert ved node into the bfs matrix for the "list" of traversal.


    visited[startnode] = 1;
    dfsmatrix[count++] = startnode;

    // Tekst: "We mark: X as visited and add it to our list. and its neighbours are { ... }"
    char line[256];
    int offset = snprintf(line, sizeof(line),
                          "We mark: %d as visited and add it to our list. and its neighbours are { ",
                          startnode);

    int no_comma_on_first = 1;
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] >= 1) {
            offset += snprintf(line + offset,
                               sizeof(line) - offset,
                               "%s%d",
                               (no_comma_on_first ? "" : ", "),
                               i);
            no_comma_on_first = 0;
        }
    }
    snprintf(line + offset, sizeof(line) - offset, " }");

    DrawText(line, baseX, baseY + *pLineHeight, 20, BLACK);
    *pLineHeight += 20;

    // 2) Gå igennem naboer – DFS step + forklarende tekst
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] >= 1 && visited[i] != 1) {
            // Tekst: "we are now going to visit i. Our DFS list consist of: { ... }"
            char dfsListLine[256];
            int off = snprintf(dfsListLine, sizeof(dfsListLine),
                               "We are now going to visit %d. Our DFS list consist of: { ",
                               i);

            int first = 1;
            for (int j = 0; j < count; j++) {
                off += snprintf(dfsListLine + off,
                                sizeof(dfsListLine) - off,
                                "%s%d",
                                (first ? "" : ", "),
                                dfsmatrix[j]);
                first = 0;
            }
            snprintf(dfsListLine + off, sizeof(dfsListLine) - off, " }");

            DrawText(dfsListLine, baseX, baseY + *pLineHeight, 20, BLACK);
            *pLineHeight += 20;

            parent[i] = startnode;
            count = dfs_matrix_onlyforgui(i, n, adj, dfsmatrix, visited, parent, cycles, cycle_count, count, pLineHeight, baseX, baseY);
        }
        /* we can check here if there is a cycle! We can do this because we know that if we traverse the neighbors of a node,
         * and we find a neighbor who has already been visited, and that this is not our "parents", i.e. where we came from, then we have a
         * cycle in which we can then go back. We can set this up as the following conditions
         * however, I have added an extra condition, namely that startnode < i, because then it will only be detected once :) otherwise you will
         * make it detect first around to the right, and then also where you go left, i.e. detect 1 --> 3 and then 3 --> 1.
         */
        else if (adj[startnode][i] >= 1 && visited[i] == 1 && i != parent[startnode] && startnode < i) {
            char cycleLine[256];
		int already_seen = 0; 

			for (int k = 0; k < *cycle_count; k++){
			if ( (cycles[k][0] == startnode && cycles[k][1] == i) || cycles[k][1] == startnode && cycles[k][0] == i){
					already_seen = 1; 
					break; 
				}
			}

			if (already_seen == 0){
				cycles[*cycle_count][0] = startnode; 
				cycles[*cycle_count][1] = i;
			

            		snprintf(cycleLine, sizeof(cycleLine),
                     		"Cycle detected: Node %d (parent %d) has an edge to already visited node %d (parent %d)",
                     		startnode, parent[startnode], i, parent[i]);

				*cycle_count += 1; 
			

            		DrawText(cycleLine, baseX, baseY + *pLineHeight, 20, RED);
            		*pLineHeight += 20;

			}

		}
    }

    // 3) Backtracking-tekst
    if (parent[startnode] != -1) {
        char backLine[256];
        snprintf(backLine, sizeof(backLine),
                 "Since all neighbours of %d are now visited, we backtrack to node %d.",
                 startnode, parent[startnode]);
        DrawText(backLine, baseX, baseY + *pLineHeight, 20, DARKGRAY);
        *pLineHeight += 20;
    } else {
        char doneLine[256];
        snprintf(doneLine, sizeof(doneLine),
                 "Since all neighbours of %d are now visited, DFS from the root is complete here.",
                 startnode);
        DrawText(doneLine, baseX, baseY + *pLineHeight, 20, DARKGRAY);
        *pLineHeight += 20;
    }
    return count;
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
        bfs_matrix_drawtext_ONLYFORUSEINRAYGUI(atom_count, adjacency_matrix, 0, bfs);
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

	int cycle_count = 0; 
	int cycles[atom_count][2]; 
		
        for (int i = 0; i < atom_count; i++) {
            parent[i] = -1;
        }

	  static Vector2 scrollDFS = { 0, 0 };

    	int screenW = GetScreenWidth();
    	int screenH = GetScreenHeight();

    	Rectangle bounds = (Rectangle){
       	 	30,
       		 220,             // lige under knapperne
        	screenW - 60,
        	screenH - 260
    	};

    	// Hvor meget tekst DFS max kan lave:
    	// groft: ~3 linjer per node + lidt ekstra
    	int contentHeight = 3 * 20 * atom_count + 200;  // 20 px pr linje
    	int contentWidth  = 900 + atom_count*20;              // bare noget fornuftigt, DFS-tekst er mest vertikal

    	Rectangle content = (Rectangle){
        0, 0,
        contentWidth,
        contentHeight
    	};

   	 Rectangle view = {0};

   	 GuiScrollPanel(bounds, "DFS explanation", content, &scrollDFS, &view);

    	BeginScissorMode((int)view.x, (int)view.y, (int)view.width, (int)view.height);

    	int lineHeight = 0;

    	int baseX = (int)(bounds.x + 10 + scrollDFS.x);
    	int baseY = (int)(bounds.y + 30 + scrollDFS.y);

    	int startnode = 0; // eller hvad du bruger som root
    	int count = 0;

    	count = dfs_matrix_onlyforgui(
        	startnode,
       		atom_count,
        	adjacency_matrix,
        	dfsmatrix,
        	visited,
        	parent,
        	cycles,
        	&cycle_count,
        	count,
        	&lineHeight,
        	baseX,
        	baseY
    );

    EndScissorMode();

    }


}


void DrawTab_GraphView(){
    DrawText("Graph View Tab", 30, 80, 25, BLACK);
	if (!inputValid) {
		DrawTextEx(uiFont,
			"Please enter and validate a valid SMILES in the Input tab first.",
			(Vector2){30,160}, 18,2, RED);
		return; // vi returner her fordi vi skal ikke tegne mere hvis ikke den er valid, funktionen skal stoppe.
	}

	int adjacency_matrix[atomCount][atomCount];

	create_adjacency_matrix(smilesInput,atomCount,adjacency_matrix);

	int dfsmatrix[atomCount];
	int visited[atomCount];
	int parent[atomCount];
	int cycle_count = 0;
	int cycles[atomCount][2];
	int startnode = 0; // eller hvad du bruger som root
	int count = 0;



	dfs_matrix(startnode,atomCount,adjacency_matrix,dfsmatrix,visited,parent,cycles, &cycle_count,count);


	draw_molecule(smilesInput, atomCount, adjacency_matrix,cycle_count);


}

void DrawTab_Substructures(){
	DrawText("Find Substructures (toxicphores) in the molecule", 30, 80, 25, BLACK);

    // Hvis input ikke er gyldigt, giver det ingen mening at vise matrix
    if (!inputValid) {
        DrawTextEx(uiFont,
            "Please enter and validate a valid SMILES in the Input tab first.",
            (Vector2){30,160}, 18,2, RED);
        return; // vi returner her fordi vi skal ikke tegne mere hvis ikke den er valid, funktionen skal stoppe.
    }

	// vi skal have lavet et input felt ligesom i "input validation" til at input vores substructure smile 
	GuiLabel((Rectangle){30, 160, 150, 25}, "Substructure input:");

	//Teksboks til input
	if (GuiTextBox((Rectangle){220, 160, 300, 25}, substructures_input, 256, editMode)) {
		editMode = !editMode; 
	}

	if (GuiButton((Rectangle){530, 160,100,25}, "Test")){
		TraceLog(LOG_INFO, "Test pressed. Input: %s", substructures_input);
		pressedsubstructures = 1; 

		substructure_test = toxicphore_function(smilesInput, substructures_input);
	}

	if (pressedsubstructures){
		// hvis den er 1 så er der fundet match 
		if (substructure_test){
			//GRØN Success-tekst 
			DrawTextEx(uiFont, "The substructure was found!", (Vector2){30, 190}, 30, 0, goodGreen);
		}
		else {
            	DrawTextEx(uiFont, "The substructure was not found", (Vector2){30, 190}, 25, 2, softRed);
		}
	}

    DrawTextEx(uiFont,
               TextFormat("SMILES: %s", smilesInput),
               (Vector2){30, 130},
               18,
               2,
               DARKGRAY);
}

void Clear() {
    smilesInput[0] = '\0';
	substructures_input[0] = '\0'; 

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
