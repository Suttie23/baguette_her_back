#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_settings.h"
#include "scenes/scene_controls.h"

using namespace std;

MenuScene menu;
SettingsScene settings;
ControlsScene controls;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

int main() {
  Engine::Start(1280, 720, "Baguette Her Back!",&menu);
}