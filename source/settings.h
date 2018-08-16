#ifndef PDSX_SETTINGS_H
#define PDSX_SETTINGS_H

#include <string>
#include "graphic.h"
#include "pp2d/pp2d.h"

/** Settings **/

// Color settings.
// Use SET_ALPHA() to replace the alpha value.
#define SET_ALPHA(color, alpha) ((((alpha) & 0xFF) << 24) | ((color) & 0x00FFFFFF))

// 3D offsets.
typedef struct _Offset3D {
	float level;
} Offset3D;
extern Offset3D offset3D[2];	// 0 == Left; 1 == Right

typedef struct _Settings_t {
	// TODO: Use int8_t instead of int?
	struct {
		int border;		// 0 = None (16:10), 1 = Left/Right black bars (4:3)
		int modeOrder;
	} pseudoEmulation;
} Settings_t;
extern Settings_t settings;

/**
 * Load settings.
 */
void LoadSettings(void);

/**
 * Save settings.
 */
void SaveSettings(void);

#endif /* PDSX_SETTINGS_H */
