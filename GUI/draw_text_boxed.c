
#include "draw_text_boxed.h"
#include <string.h>   // strlen (hvis du bruger det)
#include <stdlib.h>   // malloc/free (hvis du bruger det)

void DrawTextBoxed(Font font, const char *text, Rectangle rec,
                   float fontSize, float spacing, bool wordWrap, Color tint)
{
    // Minimal fallback: draw text without wrapping
    // (You can paste the full wrapping version from the raylib example here)
    DrawTextEx(font, text, (Vector2){rec.x, rec.y}, fontSize, spacing, tint);
}
