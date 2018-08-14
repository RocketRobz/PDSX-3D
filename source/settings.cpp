#include "settings.h"
#include "inifile.h"

#include <unistd.h>
#include <string>
using std::string;
using std::wstring;

#include <3ds.h>

static CIniFile settingsini( "sdmc:/3ds/PDSX.ini" );

// Settings
Settings_t settings;

void LoadSettings(void) {
	// UI settings.
	settings.pseudoEmulation.border = settingsini.GetInt("SETTINGS", "BORDER", 1);
}

/**
 * Save settings.
 */
void SaveSettings(void) {
	// UI settings.
	//settingsini.SetInt("SETTINGS", "BORDER", settings.pseudoEmulation.border);
}
