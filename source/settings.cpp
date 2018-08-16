#include "settings.h"
#include "inifile.h"

#include <unistd.h>
#include <string>
using std::string;
using std::wstring;

#include <3ds.h>

static CIniFile settingsini( "sdmc:/3ds/PDSX.ini" );

// 3D offsets. (0 == Left, 1 == Right)
Offset3D offset3D[2] = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},	// PS model (L)
						{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};	// PS model (R)

// Settings
Settings_t settings;

void LoadSettings(void) {
	settings.pseudoEmulation.border = settingsini.GetInt("SETTINGS", "BORDER", 1);
	settings.pseudoEmulation.modeOrder = settingsini.GetInt("SETTINGS", "SCREEN_ORDER", 0);
}

/**
 * Save settings.
 */
void SaveSettings(void) {
	settingsini.SetInt("SETTINGS", "BORDER", settings.pseudoEmulation.border);
	settingsini.SetInt("SETTINGS", "SCREEN_ORDER", settings.pseudoEmulation.modeOrder);
}
