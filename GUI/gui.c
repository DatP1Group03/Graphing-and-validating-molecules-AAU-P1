//
// Created by magnus on 29/11/2025.
//


/*
 * GUI main module (raylib + raygui)
 *
 * This file contains the global UI state used across multiple tabs:
 *  - Input validation
 *  - Adjacency matrix view
 *  - Stability/valence check view
 *  - Graph traversal (BFS/DFS) view
 *  - Graph rendering view
 *  - Substructure/toxicophore detection view
 *  - Node feature matrix view
 *
 * Important note about state:
 * raylib redraws the entire screen every frame inside the main loop, so any values
 * that must persist between frames (input buffers, toggle states, cached results)
 * are stored as static/global variables.
 */

#include <stdio.h>
#include "gui.h"
#include "raylib.h"
#include "raygui.h"
#include "Input/validation.h"
#include "Adjacency_matrix.h"
#include "dfs_matrix.h"
#include "draw_text_boxed.h"
#include "valence_check.h"
#include "toxicphore.h"
#include "Graph_representation.h"
#include "Graph_representation/Graph_representation.h"
#include "smiles_nodefeature/SMILESNODE.h"

/*
 * Reference links used during development:
 *  - raygui header (h-raylib mirror): https://hackage.haskell.org/package/h-raylib-5.1.1.0/src/raylib/examples/shapes/raygui.h
 *  - raylib cheatsheet: https://www.raylib.com/cheatsheet/cheatsheet.html
 *  - raygui upstream header: https://github.com/raysan5/raygui/blob/master/src/raygui.h
 */
/*
 * UI constants:
 * MaxInput defines the size of the text buffer used by raygui text boxes.
 * raygui requires the user to provide a fixed-size char buffer, and it is the user's
 * responsibility to ensure it is large enough for expected input.
 */
#define MaxInput 256 

/*
 * raygui style indices:
 * raygui stores style settings in a global style array indexed by:
 *   (control_id, property_id)
 * We define TAB control and relevant property indices here so we can theme tabs
 * using GuiSetStyle(TAB, property, value).
 */
#define TAB  17
#define TAB_BASE_COLOR_NORMAL   14
#define TAB_BASE_COLOR_FOCUSED 17
#define TAB_TEXT_COLOR_NORMAL   15
#define TAB_TEXT_COLOR_FOCUSED  18

/*
 * Shared UI colors used for validation and status rendering across multiple tabs.
 */

Color goodGreen = (Color){ 0, 180, 120, 255 };
Color softRed   = (Color){ 200, 60, 60, 255 };
Color grayText  = DARKGRAY; //DARKGRAY er en foruddefineret farve fra raylib, og i raylib er den prædefineret til #define DARKGRAY (Color){ 80, 80, 80, 255 }

/*
 * Algorithm tab state (BFS/DFS):
 * These variables cache traversal results so we do not recompute them every frame.
 */

static int bfsOrder[MAX_ATOMS];
static int bfsCount = 0;
static bool bfsRan = false;
static bool dfsRan = false;
static char bfsLog[4096];   // tekstlog til GUI


/*
 * Tab "info box" toggles:
 * Each boolean controls whether the corresponding help/info popup is currently visible.
 */
static const char *smilesValidationText =
	"The SMILES validation module analyzes the input string through several "
	"linear scans (for-loops), each running in O(n) time. The first pass checks "
	"that every character is part of the allowed SMILES symbol set. A second "
	"scan uses simple stack logic to ensure that parentheses and brackets are "
	"properly opened and closed,";


static const char *info_adj_matrix_text = 
	"The SMILES-to_adjacency module builds a square matrix sixed by the atom count in the validated SMILES string. It scans the SMILES character by character and whenever it finds a new atom, it connects it to the correct previous atom (handling branches via parentheses). Ring numbers are tracked so the second occurence closes the ring by adding a bond between the stored atom and the current atom. Bond order is set from the bond symbol (-, =, #), defaulting to single bonds when no symbol is present. Finally, the matrix is mirrored to make is symmetric, since bonds are recorded in one direction during  the scan."; 
static const char *info_stability_text = 
	"The Stability Check visualizes the molecule’s valence validation. First, the program builds an adjacency matrix from the validated SMILES input. It then runs the valence check to compute, for each atom, how many bonds it has (including bond order) and whether this exceeds the allowed valence for that element. The result is displayed as a chain of atoms with their bond count and a VALID/ERROR label per atom. This makes it easy to see exactly where the structure becomes chemically invalid."; 
static const char *info_algorithm_text =
	"DFS does the following: DFS (Stands for Depth-First Search) goes along a path as deeply as possible along each branch before backtracking. DFS starts at a chosen node and visits it. It then looks at all the neighbours (all nodes it has connection to) of that node and follows the first neighbour it finds.From that neighbour, it again looks at its neighbours and continues going forward as long as there is an unvisited neighbour to follow. When it reaches a node where all neighbours have already been visited, it backtracks — meaning it goes back to the node it came from, and continues with the next neighbour there. DFS continues this pattern of:  1. go foward to an unvisited neighbour 2. backtrack when stuck 3. continue from the previous node, until all reachable nodes have been visited, we are now going to perform DFS on the molecule. BFS does the following:  BFS starts at a chosen node and visits it. Instead of going deep, BFS looks at all the neighbours of that node first. After visiting those neighbours, it then visits all the neighbours of those neighbours, and so on. You can think of BFS as exploring the graph layer by layer: 1. First the start node 2. Then all nodes directly connected to it  3. Then all nodes connected to those nodes 4. And so on, BFS keeps track of the nodes it still needs to visit in a queue, which ensures that the earliest discovered nodes are processed first. BFS continues this pattern of moving outward in waves until all reachable nodes have been visited.";
static const char *info_graph_text = 
	"This module converts a validated SMILES string into a visual graph representation of the molecule. Atom positions are computed sequentially, where rings are placed using rotational geometry and side chains are handled via temporary branching states. Bond connections and bond order are determined from the adjacency matrix. Finally, atoms are drawn as nodes and bonds as lines, producing a complete 2D molecular structure.";
static const char *info_substructures_text =
	" This process determines whether a smaller molecular structure exists within a larger molecule. Both structures are modeled as graphs, where atoms represent nodes and bonds represent edges. A depth-first search attempts to map each atom in the substructure to a unique atom in the molecule. The substructure is considered found only if atom types and bond connections match exactly."; 
static const char *info_nodefeature_info =
	"This process converts a validated SMILES string into a structured numerical representation of atoms. Each atom is identified and assigned chemical properties such as atomic number, valence, and aromaticity. These properties are stored row-wise in a node feature matrix, where each row represents one atom. The matrix is validated to ensure all features are chemically valid before being used in later steps.";


bool show_adj_matrix_info = false; 
bool show_stability_info = false; 
bool show_algorithms_info = false; 
bool show_graph_view_info = false; 
bool show_substructures_info = false; 
bool show_node_feature_info = false;

/*
 * Input buffers:
 * Must be static because the raylib loop redraws every frame. If these were local,
 * they would reset each frame and the text box would lose its contents.
 */
static char smilesInput[256] = {0}; 
static char substructures_input[256] = {0};

/*
 * Input/validation state:
 * inputValid: true only if the current SMILES passed validate_smiles().
 * moleculeValidated: true if validation has been run at least once (even if it failed).
 * editMode: raygui text box state; true when the user is actively typing into the input box.
 */
bool showValidationInfo = false;
static bool inputValid = false; 
static bool editMode = false;
static bool moleculeValidated= false; 


/*
 * One-shot execution flags:
 * Some computations are expensive and should not run every frame.
 * These flags ensure the computation is performed once after input changes.
 */
static bool val_flag = false; 
static bool moleculeLoaded = false; 
static bool graphComputed = false;




/*
 * Validation module outputs:
 * error_count and errors[] are filled by validate_smiles() to describe problems in the input.
 * These are global so multiple tabs can display validation errors consistently.
 */
static int error_count; 
static Error errors[MAX_ERRORS]; 

/*
 * UI font loaded at startup and reused by all drawing functions.
 * Stored globally to avoid reloading and to keep rendering consistent across tabs.
 */
Font uiFont;

/*
 * Substructure/toxicophore tab state:
 */
static int pressedsubstructures = 0; 
static int substructure_test = 0; 

/*
 * Stability/valence check tab state:
 * molecule and smile_size are produced during parsing and used by run_valence_check().
 * Keeping them cached prevents rebuilding the molecule every frame.
 */
static Symbol *molecule = NULL; 
static int smile_size = 0; 

/*
 * Graph view cached values:
 * atomcountstactic caches the atom count so it can be reused across tabs without recomputation.
 * cached_cycle_count stores the number of detected cycles (used by graph layout / traversal view).
 */
static int atomcountstactic;
static int cached_cycle_count = 0; 


/*
 * runGUI()
 * Main entry point for the GUI application.
 *
 * This function initializes raylib/raygui, configures global GUI styling, and then
 * runs the main render loop until the user closes the window.
 *
 * The loop follows the standard raylib pattern:
 *   BeginDrawing() -> draw everything for the current frame -> EndDrawing()
 * Because the screen is redrawn from scratch every frame, persistent UI state is stored
 * in static/global variables (input buffers, toggles, cached results, etc.).
 *
 * Returns 0 on normal exit.
 */
int runGUI() {
   /*
     * InitWindow() creates an OS-level window and OpenGL context:
     *  - Linux: X11/Wayland
     *  - Windows: Win32
     *  - macOS: Cocoa
     * It also initializes internal raylib state (input, timers, textures, etc.).
     */
	InitWindow(1400, 1000, "S-SMILES");

    /*
     * Tell raylib to target 60 FPS. raylib will internally delay frames so the loop
     * does not run faster than the requested rate.
     */
	SetTargetFPS(60); 

    /*
     * TraceLog() is raylib’s built-in logging function (like printf, but with levels,
     * consistent formatting, and cross-platform behavior).
     */
	TraceLog(LOG_INFO, "CWD: %s", GetWorkingDirectory());

    /*
     * Tab bar state:
     * activeTab tracks which tab is currently selected (0 = first tab).
     */
	int activeTab = 0;


    /*
     * Tab labels passed to GuiTabBar().
     */
	const char *tabs[] = {
		"Input Validation",
		"Adjacency Matrix",
		"Stability",
		"Algorithms",
		"Graph View",
		"Substructures",
		"Node feature",
	};
	int tabCount = sizeof(tabs) / sizeof(tabs[0]);  // founds out how many tabs we have (so we dont have to hardcode it).
    /*
     * Load and apply UI font (used by raygui widgets and custom DrawTextEx calls).
     * Note: the font file path is relative to the working directory.
     */
	uiFont = LoadFontEx("../GUI/roboto.ttf", 30, NULL, 0);  // loader fonten georgia.ttf med størrelse 30 px. NULL,0 betyder at reaylib selv genere glyphs for standard ASCII.
	GuiSetFont(uiFont); // istedet for standard font så er det georgia der sk

    /*
     * Global GUI style configuration:
     * These settings affect most controls unless overridden per-control (e.g., TAB styles).
     */
	GuiSetStyle(DEFAULT, TEXT_SIZE, 15); // Sætter tekststørrelsen for alle w
	GuiSetStyle(DEFAULT, BORDER_WIDTH, 1); // Alle widgets får en kant/bo
	GuiSetStyle(DEFAULT, LINE_COLOR, 0x555555ff); // Definerer farven på kanter/linjer i GUI’en.  0x555555ff = mørk grå (hex: R=55, G=5


    /*
     * TAB-specific style configuration (padding, borders, colors for normal/focused state).
     */
	GuiSetStyle(TAB, TEXT_PADDING, 8); // Indre padding i fanerne (afstand mellem tab-kant og teksten) = 8 px.
	GuiSetStyle(TAB, BORDER_WIDTH, 1); // Tab-elementerne får en 1 px border (kan være forskellig fra default).
	GuiSetStyle(TAB, BASE_COLOR_NORMAL, 0xE8E8E8FF); // Når en tab ikke er valgt, er dens baggrundsfarve  0xE8E8E8FF = lys grå (normal state).
	GuiSetStyle(TAB, BASE_COLOR_FOCUSED, 0xC0FAC0FF); // Når en tab er valgt/hover, bruges 0xC0FAC0FF = lys grøn.
	GuiSetStyle(TAB, TEXT_COLOR_FOCUSED, 0x006600FF); // Tekstfarven i den aktive faneblad bliver mørk grøn #006600.




    /*
     * Main draw loop: runs until the window is closed (ESC, window close button, etc.).
     */
	while (!WindowShouldClose()) {
        	BeginDrawing();  // Starter en ny frame – alt tegning mellem BeginDrawing/EndDrawing
        	ClearBackground((Color){ 255, 250, 250 }); // Sætter baggrundsfarven (meget lys rosa/hvid)
	
        /*
         * Temporarily override BUTTON style to draw the CLEAR button as red.
         * We save old values and restore them afterward so other buttons are unaffected.
         */
		int oldBaseNormal = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
		int oldBaseFocused = GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED);
		int oldTextNormal = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);

        /*
         * CLEAR button: resets all GUI state (input text, cached results, flags, etc.).
         * GuiButton() draws the button and returns true only on the frame it is clicked.
         */
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xCC4444FF);   // Normal baggrundsfarve: mørk rød
		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xDD6666FF);  // Når hover/focus: lidt lysere rød
		GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF); // Tekstfarve: hvid

        // Definerer rektanglet (position og størrelse) for CLEAR-knappen
		Rectangle clearTab = {1200, 20, 140, 30};
        // Tegner en knap med teksten "CLEAR". Hvis den klikkes, kaldes Clear()
		if (GuiButton(clearTab, "CLEAR")) {
			Clear();
		}
        // Keyboard-genvej: CTRL + Z gør det samme som at trykke CLEAR
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
			Clear();
		}
        /* Restore original button style. */
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, oldBaseNormal);
		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, oldBaseFocused);
		GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, oldTextNormal);

        /*
         * Tab bar styling: example of conditional styling based on application state.
         * If the input is valid, the focused tab is highlighted in green.
         */
		GuiSetStyle(TAB + 0, TAB_BASE_COLOR_NORMAL, 0xCCCCCCFF); // default gray
		if (inputValid){
			GuiSetStyle(TAB + 0, TAB_BASE_COLOR_FOCUSED, 0x4CAF50FF);
		}
	/*
         * Draw the tab bar and update activeTab based on user interaction.
         */
		GuiTabBar(
			(Rectangle){20, 20, 860, 30},
			tabs,
			tabCount,
			&activeTab
		);

        /*
         * Dispatch drawing based on the currently selected tab.
         * Each DrawTab_* function draws the full contents for that tab in the current frame.
         */
		switch (activeTab){
			case 0: DrawTab_InputValidation(); break;
			case 1: DrawTab_AdjacencyMatrix(); break;
			case 2: DrawTab_StabilityCheck(); break;
			case 3: DrawTab_AlgorithmVisualization(); break;
			case 4: DrawTab_GraphView(); break;
			case 5: DrawTab_Substructures(); break;
			case 6: DrawTab_Nodefeature(); break; 
		}
		EndDrawing(); // Afslutter framen og viser den på skærmen
		}
	CloseWindow(); // Rydder raylib op og lukker vinduet
	return 0;
}
/*
 * DrawTab_InputValidation()
 * Tab 0: Validates the SMILES string entered by the user.
 *
 * The tab supports two modes:
 *  - Info mode: shows an explanation panel and a visual validation breakdown.
 *  - Normal mode: shows input textbox, Validate button, and error list/status.
 *
 * When validation is run, we also reset cached downstream computations
 * (valence check, graph rendering) so other tabs recompute using the new input.
 */
void DrawTab_InputValidation(){
	GuiToggle((Rectangle){1090, 22, 60, 25}, "?", &showValidationInfo);
	if(showValidationInfo){
		DrawTextEx(uiFont,"Input Validation Process", (Vector2){30,80},30,2, BLACK);
		DrawTextBoxed(
			uiFont,
			smilesValidationText,
			(Rectangle){ 40, 140, 820, 640 }, // x, y, width, height
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
			inputValid = validate_smiles(smilesInput, &error_count, errors);
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

			if (is_permitted(smilesInput, &error_count, errors)) {
				DrawTextEx(uiFont, "PERMITTED CHARACTERS",
	       				(Vector2){ b1.x + 20, b1.y - 20 }, fontSize - 10, spacing, GREEN);
			} else {
				DrawTextEx(uiFont, "ILLEGAL CHARACTER",
	       				(Vector2){ b1.x + 20, b1.y - 20 }, fontSize - 10, spacing, RED);
			}


			if (closed_brackets(smilesInput, &error_count,errors)) {
				DrawTextEx(uiFont, "BALANCED PARENTHESES",
	       				(Vector2){ b2.x + 20, b2.y - 20 }, fontSize - 10, spacing, GREEN);
			} else {
				DrawTextEx(uiFont, "UNBALANCED PARENTHESES",
					(Vector2){ b2.x + 20, b2.y - 20 }, fontSize - 10, spacing, RED);
			}


			if (ring_closed(smilesInput, &error_count, errors)) {
				DrawTextEx(uiFont, "RING STRUCTURE VALID",
					(Vector2){ b3.x + 20, b3.y - 20 }, fontSize - 10, spacing, GREEN);
			} else {
				DrawTextEx(uiFont, "INVALID RING CLOSURE",
					(Vector2){ b3.x + 20, b3.y - 20 }, fontSize - 10, spacing, RED);
			}


			if (misc_check(smilesInput, &error_count, errors)) {
				DrawTextEx(uiFont, "MISC STRUCTURE VALID",
					(Vector2){ b4.x + 20, b4.y - 20 }, fontSize - 10, spacing, GREEN);
			} else {
				DrawTextEx(uiFont, "MISC STRUCTURE ERROR",
	       				(Vector2){ b4.x + 20, b4.y - 20 }, fontSize - 10, spacing, RED);
			}
		}
		inputValid = validate_smiles(smilesInput, &error_count, errors);
	}

	if (!showValidationInfo) {
		DrawTextEx(uiFont,"Input Validation Tab", (Vector2){30,80},30,2, BLACK);
		GuiLabel((Rectangle){30, 130, 120, 25}, "SMILES Input:");
		if (GuiTextBox((Rectangle){160, 130, 300, 25}, smilesInput, 256, editMode)){
			editMode = !editMode;
		}
		// Validate button
		if (GuiButton((Rectangle){470, 130, 100, 25}, "Validate")){
			moleculeValidated = true;
			inputValid = validate_smiles(smilesInput, &error_count, errors);
			atomcountstactic = get_atom_count(smilesInput);
			graphComputed = false;
			val_flag = false;
			moleculeLoaded = false;
			TraceLog(LOG_INFO, "Validate pressed. Input: %s | Valid=%d", smilesInput, inputValid);
			inputValid = validate_smiles(smilesInput, &error_count, errors); // smilesInput var den variable vi brugt i textbox, så vi får teksten fra boxen og validerer.
			TraceLog(LOG_INFO, "Validate pressed. Input: %s | Valid=%d", smilesInput, inputValid); //TraceLog er en måde i raylib at skrive i terminalen, sådan når man skriver og tester koden så kan vi følge med hvad der sker i programmet.
			}
		if (moleculeValidated) {
			if (inputValid ) {
				DrawTextEx(uiFont, "Valid SMILES", (Vector2){30, 180}, 30, 0, goodGreen);
				DrawTextEx(uiFont, "INPUT SIZE DOES NOT EXCEED MAX_INPUT OF 100", (Vector2){30, 230}, 15, 0, goodGreen);
				DrawTextEx(uiFont, "VALID PARENTHESES", (Vector2){30, 250}, 15, 0, goodGreen);
				DrawTextEx(uiFont, "NO ILLEGAL CHARACTERS", (Vector2){30, 270}, 15, 0, goodGreen);
				DrawTextEx(uiFont, "NO UNCLOSED RINGS", (Vector2){30, 290}, 15, 0, goodGreen);
				smile_size = count_smiles(smilesInput);
			}
			else {
				DrawTextEx(uiFont, "INVALID SMILES:", (Vector2){30, 180}, 25, 2, softRed);
				if (!inputValid) {
					DrawTextEx(uiFont, "INVALID SMILES:", (Vector2){30, 180}, 25, 2, softRed);
					int y = 220;
					for (int i = 0; i < error_count; i++) {
						char buffer[256];
						snprintf(buffer, sizeof(buffer), "%s (position %d)",
	       						get_error_message(i, &error_count, errors),
							get_error_position(i, &error_count, errors));
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

/*
 * DrawTab_AdjacencyMatrix()
 * Tab 1: Builds and displays the adjacency matrix for the validated SMILES string.
 *
 * Uses a scroll panel because the matrix can be larger than the visible window.
 * The matrix is regenerated from smilesInput each frame (could be cached if desired),
 * but only when inputValid is true.
 */
void DrawTab_AdjacencyMatrix(){
	// Hvis input ikke er gyldigt, giver det ingen mening at vise matrix
	if (!inputValid) {
		DrawTextEx(uiFont,"Please enter and validate a valid SMILES in the Input tab first.",
			(Vector2){30,130}, 18,2, RED);
		return; // vi returner her fordi vi skal ikke tegne mere hvis ikke den er valid, funktionen skal stoppe.
		}
	GuiToggle((Rectangle){1090, 22, 60, 25}, "?", &show_adj_matrix_info);
	if(show_adj_matrix_info){
		DrawTextEx(uiFont,"Adjacency matrix Process", (Vector2){30,80},30,2, BLACK);
		DrawTextBoxed(
			uiFont,
			info_adj_matrix_text,
			(Rectangle){ 40, 140, 820, 640 }, // x, y, width, height
			30,
			2,
			true,
			DARKGRAY
		);
	} else {
		// Overskrift til tabben
		DrawText("Adjacency Matrix Tab", 30, 80, 25, BLACK);
		// Hvis den er valid så skal vi have lavet adjacency matrix ud fra adjacency_matrix.h
		// // vi laver vores adjacency_matrix ud fra atom_count i det vi godt kan kende størrelsen
		int adjacency_matrix[atomcountstactic][atomcountstactic];
		create_adjacency_matrix(smilesInput, atomcountstactic, adjacency_matrix);

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
		int contentWidth = (atomcountstactic + 2) * cell_size +80 ; // vi har atom_count kolonner +2 (en kolonne til række-indeks, en til "luft") - og vores celle er 30 pixel bred og høj, dernæst har vi 80 så vi ikke rammer kanten med tekst.
		int contentHeight = (atomcountstactic +4) * cell_size +80;

		// hvor stort indholdet potentielt kan være - hvis content er større end bounds -> scrollbars kommer frem 
		Rectangle content = {
			0, 0,  // origin i panelet
			contentWidth, 
			contentHeight, 
		}; 

		Rectangle view = {0}; // et tomt rektangel, dette skal vores GuiscrollPanel udfylde. Det bliver det synlgie udsnit af indholdet. 

		GuiScrollPanel(bounds, "Adjacency Matrix", content, &scroll, &view); // tegnet scrollpanelets ramme, tegner scrollbarerene (vertikal og horizontal), opdaterer scroll.x og scroll.y i det at man scroller og returnerer i vew det område vi så må tegne i 

		//Tegn Matrizen inde i scroll panelet 
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
		char smile_symbols[atomcountstactic];
		fill_atom_symbols_from_smile(smilesInput, smile_symbols, atomcountstactic);
		// jeg har her lavet en kolonne overskrift til at starte med, fordi så synes jeg det er nemmere at se ift om indeksering sker korrekt. Dette kan altid ændres
		for (int col = 0; col < atomcountstactic; col++) {
			DrawText(TextFormat("%c", smile_symbols[col]), // TEXT format er ligesom raylibs version af printf (nemmere sprintf).
				startX + (col +1)*cell_size, startY, 18, BLACK);
			// vi vil også gerne have en linje lige under.  der kan anvendes DrawLineEx (som anvender vector, men vi kan her vælge tykkelse derfor vælges denne).
			// // de ekstra +20 jeg har sat på et grundet fontsize er 18 så vi skal forbi talenne.
			DrawLineEx((Vector2){startX, startY+26}, (Vector2){startX +20+ (col +1)*cell_size, startY+26},2, BLACK);
			DrawText(TextFormat("%d", col), startX + (col+1)*cell_size+12, startY+14, 4, BLACK);

			DrawText(TextFormat("%c", smile_symbols[col]), 80 + (24*(col+1)), startY + (atomcountstactic+2) * cell_size, 18, DARKGRAY);

			DrawText(TextFormat("%d", col), 90 + (24*(col+1)), startY+ 14 + (atomcountstactic+2) * cell_size, 1, DARKGRAY);
		}
		// Række-overskrifter
		for (int row = 0; row < atomcountstactic; row++) {
			// Række-label
			DrawText(TextFormat("%c", smile_symbols[row]), startX, startY + (row + 1) * cell_size, 18, BLACK);	 
			DrawText(TextFormat("%d", row), startX+12, startY + (row + 1) * cell_size+14, 4, BLACK);
			//linje for at adskille række label fremfor selve matrixen
			DrawLineEx((Vector2){startX+26, startY+20}, (Vector2){startX+26, startY+20 + (row + 1) * cell_size},2, BLACK);
			// Celler i rækken
			for (int col = 0; col < atomcountstactic; col++) {
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
			(Vector2){30, startY + (atomcountstactic + 2) * cell_size},
	     		18,
			2,
			DARKGRAY);

		DrawTextEx(uiFont,
	     		TextFormat("Atoms: %d", atomcountstactic),
			(Vector2){30, startY+ 35 + (atomcountstactic + 2) * cell_size},
			18,
			2,
			DARKGRAY);
		EndScissorMode(); 
	}
}

/*
 * DrawTab_StabilityCheck()
 * Tab 2: Visualizes the valence check result for the current molecule.
 *
 * This tab runs run_valence_check() only once per validated molecule (val_flag + moleculeLoaded)
 * and then renders each atom with:
 *  - its symbol,
 *  - its computed bond count,
 *  - a VALID/ERROR label depending on illegalValence.
 *
 * A scroll panel is used so long molecules remain viewable.
 */
void DrawTab_StabilityCheck(){
	GuiToggle((Rectangle){1090, 22, 60, 25}, "?", &show_stability_info);

	if(show_stability_info){
		DrawTextEx(uiFont,"Stability info Process", (Vector2){30,80},30,2, BLACK);
		DrawTextBoxed(
			uiFont,
			info_stability_text,
			(Rectangle){ 40, 140, 820, 640 }, // x, y, width, height
			30,
			2,
			true,
			DARKGRAY
		);
	}
	else {
		DrawTextEx(uiFont,"Stability Check Tab", (Vector2){30, 80,}, 30,2, BLACK);
		int radius =30;
		int dist_to_increment = 3*radius;

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
	
		int contentWidth_stability =atomcountstactic * 90 +200; // bare en værdi jeg har trukket ud af hatten
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

		int adj[atomcountstactic][atomcountstactic];
		create_adjacency_matrix(smilesInput, atomcountstactic, adj);
		// skal kun køre en gang, derfor sentinel TEST "C=O=C
		if (!val_flag && inputValid && !moleculeLoaded) {
			run_valence_check(&molecule, smile_size, atomcountstactic, smilesInput, adj);
			val_flag = true;
			moleculeLoaded = true;
		}
		if (moleculeLoaded && molecule != NULL && inputValid) {
			for (int i = 0; i < smile_size; i++) {
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
					if (i < count_smiles(smilesInput) - 1 && (molecule[i+1].atomChar == '-' || isalpha(molecule[i+1].atomChar))) {
						DrawLine(x+radius,y,x+dist_to_increment-radius,y,BLACK);
					}
					if (i != count_smiles(smilesInput)-1 && molecule[i+1].atomChar == '=') {
						DrawLine(x+radius,y+2,x+dist_to_increment-radius,y+2,BLACK);
						DrawLine(x+radius,y,x+dist_to_increment-radius,y,BLACK);
					}
					if (i != count_smiles(smilesInput)-1 && molecule[i+1].atomChar == '#') {
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
	 * * rear moves when we add to the queue. rear = number of elements in queue + front*/
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
			snprintf(bfsLine + offset, sizeof(bfsLine) - offset, "}");

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
		snprintf(queueLine + qOff, sizeof(queueLine) - qOff, "}");
		DrawText(queueLine, x_bfs, lineheight, 20, BLACK);
		lineheight += 20;
		int u = queue[front]; // The first time, queue[0] is set equal to u. U will be used to determine the first element in our array.
		front++; //we now increment our front by 1, since we have just dequeued to u
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

/*
 * dfs_matrix_onlyforgui()
 * Recursive DFS implementation used ONLY for GUI explanation output.
 *
 * It logs each DFS step (visit, neighbour list, recursion, backtracking) by drawing
 * text lines, and it records:
 *  - dfsmatrix[] traversal order,
 *  - visited[] markers,
 *  - parent[] tree,
 *  - cycles[][] (simple undirected back-edge detection for already-visited neighbours).
 *
 * pLineHeight is used as a "cursor" for where the next explanation line should be drawn.
 *
 * Returns the number of visited nodes so far (count).
 */
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
				off += snprintf(dfsListLine + off, sizeof(dfsListLine) - off, "%s%d", (first ? "" : ", "), dfsmatrix[j]);
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
		snprintf(backLine, sizeof(backLine), "Since all neighbours of %d are now visited, we backtrack to node %d.", startnode, parent[startnode]);
		DrawText(backLine, baseX, baseY + *pLineHeight, 20, DARKGRAY);
		*pLineHeight += 20;
	} else {
		char doneLine[256];
		snprintf(doneLine, sizeof(doneLine), "Since all neighbours of %d are now visited, DFS from the root is complete here.", startnode);
		DrawText(doneLine, baseX, baseY + *pLineHeight, 20, DARKGRAY);
		*pLineHeight += 20;
	}
	return count;
}

/*
 * DrawTab_AlgorithmVisualization()
 * Tab 3: Lets the user run and view step-by-step BFS or DFS explanations.
 *
 * The user triggers either algorithm via a button:
 *  - "Run BFS" -> prints queue states and visited nodes
 *  - "Run DFS" -> prints recursion/backtracking and cycle detection messages
 *
 * Requires inputValid = true; otherwise the tab shows a message and returns early.
 */
void DrawTab_AlgorithmVisualization() {
	GuiToggle((Rectangle){1090, 22, 60, 25}, "?", &show_algorithms_info);

	if(show_algorithms_info){
		DrawTextEx(uiFont,"Algorithm Process", (Vector2){30,80},30,2, BLACK);
		DrawTextBoxed(uiFont, info_algorithm_text, (Rectangle){ 40, 140, 820, 900 }, // x, y, width, height
		22, 2, true, DARKGRAY);
	}
	else {
		//tegner blot titlen
		DrawText("Algorithm Visualization Tab", 30, 80, 25, BLACK);

		// det er også et krav her at der et gyldigt input. den returner altså man kan ikke trykke på noget her.
		if (!inputValid) {
			DrawText("Please enter a valid SMILES on the first tab.", 30, 130, 20, RED);
			return;
		}

		// Knap til at starte BFS
		// // har lavet en variabel der hedder bfsRan (defineret på linje 118) den gør at teksten afhænger om man har kørt BFS eller ikke. Hvis true så står der run BFS again, hvis false (standard) så står der blot BFS run.
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
			if (atomcountstactic <= 0) { // blot en lille sikring er egentlig irrelevant da hvis ingen valid input så kommer man aldrig hertil
				bfsLog[0] = '\0';
				bfsCount = 0;
				bfsRan = false;
				return;
			}
			if (atomcountstactic > MAX_ATOMS) atomcountstactic = MAX_ATOMS;  // sikkerhed
			// Lav VLA adjacency matrix
			int adjacency_matrix[atomcountstactic][atomcountstactic];
			int bfs[atomcountstactic];
			create_adjacency_matrix(smilesInput, atomcountstactic, adjacency_matrix);
			bfs_matrix_drawtext_ONLYFORUSEINRAYGUI(atomcountstactic, adjacency_matrix, 0, bfs);
		}
		if (dfsRan) {
			// vi skal have lavet vores adjacency matrixe ( ikke sikkert at vi har været forbi tabben adjacency matrix).

			if (atomcountstactic <= 0) { // blot en lille sikring er egentlig irrelevant da hvis ingen valid input så kommer man aldrig hertil
				bfsLog[0] = '\0';
				bfsCount = 0;
				bfsRan = false;
				return;
			}
			if (atomcountstactic > MAX_ATOMS) atomcountstactic = MAX_ATOMS;  // sikkerhed
			// // Lav VLA adjacency matrix

			int adjacency_matrix[atomcountstactic][atomcountstactic];
			create_adjacency_matrix(smilesInput, atomcountstactic, adjacency_matrix);
			int dfsmatrix[atomcountstactic];
			int visited[atomcountstactic];
			int parent[atomcountstactic];
			int cycle_count = 0; 
			int cycles[atomcountstactic][2];
		
			for (int i = 0; i < atomcountstactic; i++) {
				parent[i] = -1;
			}

			static Vector2 scrollDFS = { 0, 0 };
			int screenW = GetScreenWidth();
			int screenH = GetScreenHeight();

			Rectangle bounds = (Rectangle){ 30,
				220,             // lige under knapperne
				screenW - 60,
				screenH - 260
			};

			// Hvor meget tekst DFS max kan lave:
			// // groft: ~3 linjer per node + lidt ekstra
			int contentHeight = 3 * 20 * atomcountstactic + 200;  // 20 px pr linje
			int contentWidth  = 900 + atomcountstactic*20;              // bare noget fornuftigt, DFS-tekst er mest vertikal

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
			int atom_count = count_atoms(smilesInput);

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


}

/*
 * DrawTab_GraphView()
 * Tab 4: Renders a 2D visual graph layout of the molecule:
 *  - adjacency matrix provides bond connections and bond order
 *  - node features provide atom types for coloring (C/O/N etc.)
 *
 * To avoid recomputing cycles/DFS every frame, the tab caches cycle_count
 * and uses graphComputed as a one-shot flag per validated input.
 */
void DrawTab_GraphView(){
	GuiToggle((Rectangle){1090, 22, 60, 25}, "?", &show_graph_view_info);
	if(show_graph_view_info){
		DrawTextEx(uiFont,"Graph Process", (Vector2){30,80},30,2, BLACK);
		DrawTextBoxed(
			uiFont,
			info_graph_text,
			(Rectangle){ 40, 140, 820, 640 }, // x, y, width, height
			30,
			2,
			true,
			DARKGRAY
		);
	}
	else {
		DrawText("Graph View Tab", 30, 80, 25, BLACK);
		if (!inputValid) {
			DrawTextEx(uiFont,
	      			"Please enter and validate a valid SMILES in the Input tab first.",
				(Vector2){30,160}, 18,2, RED);
			return; // vi returner her fordi vi skal ikke tegne mere hvis ikke den er valid, funktionen skal stoppe.
			}
		// Explaining the color of atom
		int legend_y = 93; 
		int x = 260; 
		int gap_Circle_to_text = 4; 
		int gap_group = 75; 
		// C (black) 
		DrawCircle(x,legend_y, RADIUS, BLACK);
		DrawText(":C", x + RADIUS + gap_Circle_to_text, legend_y - 10, 20, BLACK); 
		// O (red) 
		x += gap_group;
		DrawCircle(x,legend_y, RADIUS, RED);
		DrawText(":O", x + RADIUS + gap_Circle_to_text, legend_y - 10, 20, BLACK); 
		// N (blue) 
		x += gap_group;
		DrawCircle(x,legend_y, RADIUS, BLUE);
		DrawText(":N", x + RADIUS + gap_Circle_to_text, legend_y - 10, 20, BLACK); 
		int adjacency_matrix[atomcountstactic][atomcountstactic];
		create_adjacency_matrix(smilesInput,atomcountstactic,adjacency_matrix);
		int dfsmatrix[atomcountstactic];
		int visited[atomcountstactic];
		int parent[atomcountstactic];
		int cycle_count = 0;
		int cycles[atomcountstactic][2];
		int startnode = 0; // eller hvad du bruger som root
		int count = 0;
		for (int i = 0; i < atomcountstactic; i++) {
			visited[i] = 0;
			parent[i] = -1;
		}
		if (!graphComputed){
			cycle_count = 0; 
			dfs_matrix(startnode,atomcountstactic,adjacency_matrix,dfsmatrix,visited,parent,cycles, &cycle_count,count);
			cached_cycle_count = cycle_count;
			graphComputed = true; 
		}
		char atoms[MAX_ATOMS][3] = {0};
		double node_matrix[atomcountstactic][MAX_FEATURES];
		int n_atoms = parse_SMILES(smilesInput, atoms, &error_count, errors);
		if (n_atoms <= 0) {
			DrawTextEx(uiFont, "Could not parse SMILES into atoms.", (Vector2){30,160}, 18, 2, RED);
			return;
		}
		build_node_matrix(atoms, n_atoms, node_matrix, &error_count, errors);
		draw_molecule(smilesInput, atomcountstactic, adjacency_matrix,cached_cycle_count, node_matrix);
	}
}

/*
 * DrawTab_Substructures()
 * Tab 5: Tests whether a user-provided substructure (toxicophore) exists inside
 * the current main SMILES molecule.
 *
 * The result is displayed as either success (green) or failure (red).
 * Requires a valid main SMILES input.
 */
void DrawTab_Substructures(){

	GuiToggle((Rectangle){1090, 22, 60, 25}, "?", &show_substructures_info);

	if(show_substructures_info){
		DrawTextEx(uiFont,"Input Validation Process", (Vector2){30,80},30,2, BLACK);
		DrawTextBoxed(
			uiFont,
			info_substructures_text,
			(Rectangle){ 40, 140, 820, 640 }, // x, y, width, height
			30,
			2,
			true,
			DARKGRAY
		);
	}
	else {
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
}

/*
 * DrawTab_Nodefeature()
 * Tab 6: Builds and displays a node feature matrix for each atom in the SMILES string.
 *
 * Each row corresponds to one atom, and columns represent computed features
 * (e.g., atomic number, valence, aromaticity).
 *
 * A scroll panel is used to handle wide matrices and long molecules.
 */
void DrawTab_Nodefeature() {
	GuiToggle((Rectangle){1090, 22, 60, 25}, "?", &show_node_feature_info);

	if(show_node_feature_info){
		DrawTextEx(uiFont,"Node feature Process", (Vector2){30,80},30,2, BLACK);
		DrawTextBoxed(
			uiFont,
			info_nodefeature_info,
			(Rectangle){ 40, 140, 820, 640 }, // x, y, width, height
			30,
			2,
			true,
			DARKGRAY);
	}
	else {
		DrawText("Node Feature Matrix Tab", 30, 80, 25, BLACK);
		if (!inputValid) {
			DrawTextEx(uiFont,
	      			"Please enter and validate a valid SMILES in the Input tab first.",
	      			(Vector2){30,130}, 18, 2, RED);
			return;
		}
		char atoms[MAX_ATOMS][3] = {0};
		double node_matrix[MAX_ATOMS][MAX_FEATURES] = {0};
		int n_atoms = parse_SMILES(smilesInput, atoms, &error_count, errors);
		if (n_atoms <= 0) {
			DrawTextEx(uiFont, "Could not parse SMILES into atoms.", (Vector2){30,160}, 18, 2, RED);
			return;
		}
		build_node_matrix(atoms, n_atoms, node_matrix, &error_count, errors);
		// 2) Scroll opsætning (samme ide som adjacency)
		static Vector2 scroll = {0,0};
		int screenW = GetScreenWidth();
		int screenH = GetScreenHeight();
		Rectangle bounds = {
			30,
			140,
			screenW - 60,
			screenH - 180
		};
		// layout
		const int cellW = 120;
		const int cellH = 30;
		const int leftLabelW = 160;               // plads til "idx + atom"
		const int features = MAX_FEATURES;        // fx 3
		int contentWidth  = leftLabelW + features * cellW + 80;
		int contentHeight = (n_atoms + 2) * cellH + 120;  // + header + footer luft

		Rectangle content = {0,0, (float)contentWidth, (float)contentHeight};
		Rectangle view = {0};
		GuiScrollPanel(bounds, "Node Feature Matrix", content, &scroll, &view);

		BeginScissorMode((int)view.x, (int)view.y, (int)view.width, (int)view.height);

		float startX = bounds.x + 20 + scroll.x;
		float startY = bounds.y + 40 + scroll.y;

		// 3) Header row
		DrawTextEx(uiFont, "Node", (Vector2){startX, startY}, 18, 2, BLACK);
		// navne til features 
		const char *featureNames[MAX_FEATURES] = {
			"Atomic #",
			"Valence",
			"Aromatic"
			// hvis MAX_FEATURES > 3
			};

		for (int f = 0; f < features; f++) {
			const char *name = (f < 3) ? featureNames[f] : TextFormat("F%d", f);
			DrawTextEx(uiFont, name, (Vector2){startX + leftLabelW + f * cellW, startY}, 18, 2, BLACK);
		}
		// streg under header
		DrawLineEx((Vector2){startX, startY + 24}, (Vector2){startX + leftLabelW + features * cellW, startY + 24}, 2, BLACK);
		// 4) Rows
		for (int i = 0; i < n_atoms; i++) {
			// venstre label: "i  C"
			DrawTextEx(uiFont,
	      			TextFormat("%d   %s", i, atoms[i]),
	      			(Vector2){startX, startY + (i + 1) * cellH}, 18, 2, BLACK);
			// celler med features
			for (int f = 0; f < features; f++) {
				int v = (int)node_matrix[i][f];
				DrawTextEx(uiFont, TextFormat("%d", v),
	       				(Vector2){startX + leftLabelW + f * cellW + 10,
	       				startY + (i + 1) * cellH},
	       				18, 2, BLACK);
			}
		}

		// 5) Footer info
		DrawTextEx(uiFont,
	     		TextFormat("SMILES: %s", smilesInput),
			(Vector2){startX, startY + (n_atoms + 2) * cellH + 10},
			18, 2, DARKGRAY);

		DrawTextEx(uiFont,
			TextFormat("Atoms: %d", n_atoms),
			(Vector2){startX, startY + (n_atoms + 2) * cellH + 35},
	     		18, 2, DARKGRAY);
		EndScissorMode();
	}
}
/*
 * Clear()
 * Resets the entire application state to "startup defaults".
 *
 * This clears:
 *  - input buffers,
 *  - validation flags/results,
 *  - cached molecule/valence computation,
 *  - traversal state (BFS/DFS),
 *  - cached graph layout state.
 *
 * Also frees any dynamically allocated memory used by the valence module.
 */
void Clear() {
	smilesInput[0] = '\0';
	substructures_input[0] = '\0'; 
	graphComputed = false; 
	inputValid = false;
	moleculeValidated = false;
	moleculeLoaded = false;
	val_flag = false;
	atomcountstactic = 0;
	free_valency_memory(&molecule); 
	bfsRan = false;
	dfsRan = false;
	bfsCount = 0;
	bfsLog[0] = '\0';
}
