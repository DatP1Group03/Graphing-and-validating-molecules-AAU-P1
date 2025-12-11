#ifndef PROJEKT_KODE_DRAWBOXEDTEXT_H
#define PROJEKT_KODE_DRAWBOXEDTEXT_H

#include <stdbool.h>
#include "raylib.h"

void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec,
                             float fontSize, float spacing, bool wordWrap,
                             Color tint, int selectStart, int selectLength,
                             Color selectTint, Color selectBackTint);

void DrawTextBoxed(Font font, const char *text, Rectangle rec,
                   float fontSize, float spacing, bool wordWrap, Color tint);

#endif