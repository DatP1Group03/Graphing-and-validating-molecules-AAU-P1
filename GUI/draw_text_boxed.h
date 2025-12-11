
#pragma once
#include "raylib.h"

void DrawTextBoxed(Font font, const char *text, Rectangle rec,
                   float fontSize, float spacing, bool wordWrap, Color tint);
