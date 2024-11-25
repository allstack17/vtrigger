#ifndef _GUI_H_
#define _GUI_H_

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "triggerbot.h"

bool GuiMain(ConfigFileData&);

#endif	/* _GUI_H_ */