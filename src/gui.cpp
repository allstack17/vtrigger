/*
 * ----- TriggerBot GUI -----
 * 
 * Works on: ImGui + OpenGL3 + GLFW.
 * 
 */

#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <sstream>

#include "gui.h"
#include "triggerbot.h"
#include "common.h"

/* src: triggerbot.cpp */
extern bool TRIGGER_MAIN_LOOP;

namespace global {
	HHOOK hook;
	bool render_window = true;
}

static bool GLFWInit();
static void Render(GLFWwindow*, ConfigFileData&);

/* GuiMain */
bool GuiMain(ConfigFileData& cfg) {
	if (!GLFWInit())
		return false;

	GLFWwindow* pwnd = glfwGetCurrentContext();

	Render(pwnd, cfg);

	/* glfw cleanup */
	glfwDestroyWindow(pwnd);
	glfwTerminate();

	return true;
}

#define INPUT_FIELD(a, b, var, limit, flag) {																			\
	old = var;																											\
	flag ? ImGui::InputScalar(a, ImGuiDataType_U8, &var, nullptr, nullptr, nullptr, ImGuiInputTextFlags_CharsDecimal)	\
		 : ImGui::InputInt(a, (int*)&var, 0, 0, ImGuiInputTextFlags_CharsDecimal);										\
	if (var < 0 || var > limit) var = old;																				\
	else if (old != var) TRIGGER_MAIN_LOOP = false;																		\
}

void Render(GLFWwindow* pwnd, ConfigFileData& cfg) {
	/* imgui init */
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(pwnd, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	/* gui inited => hide console */
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	/* 
	 * 'VK_INSERT' to display/hide window
	 * 'VK_END' to switch TRIGGER_MAIN_LOOP
	 */
	SetWindowsHookExA(WH_KEYBOARD_LL,
		[](int code, WPARAM wp, LPARAM lp)-> LRESULT {
			if (code == HC_ACTION && wp == WM_KEYDOWN) {

				KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lp;
				if (kbd->vkCode == VK_INSERT)
					global::render_window = global::render_window ? false : true;

				else if (kbd->vkCode == VK_END) {
					TRIGGER_MAIN_LOOP = TRIGGER_MAIN_LOOP ? false : true;
					if (TRIGGER_MAIN_LOOP)
						global::render_window = true;
				}

			}
			return CallNextHookEx(global::hook, code, wp, lp);
		}, nullptr, 0
	);

	/* avoid glfw window resize */
	int height;
	/* protect fields from invalid data */
	int old;

	/* loop begin */
	while (!glfwWindowShouldClose(pwnd)) {
		glfwPollEvents();

		glfwGetWindowSize(pwnd, nullptr, &height);

		if (global::render_window) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			/* imgui begin */
			ImGui::Begin("Trigger", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			/* set imgui window params once */
			ImGui::SetWindowSize(
				{ WINDOW_X_SIZE, WINDOW_Y_SIZE }, ImGuiCond_Once);
			ImGui::SetWindowPos({ 0, 0 }, ImGuiCond_Once);
			ImGui::SetWindowCollapsed(false, ImGuiCond_Once);

			/* widgets */
			if (!ImGui::IsWindowCollapsed()) {
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.7f, 0.9f, 1.0f, 0.5f });
				ImGui::SetCursorPosX(WINDOW_X_SIZE / 2 - (ImGui::CalcTextSize("C O N F I G").x / 2));
				ImGui::Text("C O N F I G");
				ImGui::PopStyleColor();

				ImGui::SetNextItemWidth(100);
				INPUT_FIELD(" X (200)", "zone_x", cfg._zone_x, 200, true);

				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				INPUT_FIELD(" Y (200)", "zone_y", cfg._zone_y, 200, true);

				ImGui::SetNextItemWidth(100);
				INPUT_FIELD(" Tolerance (100)", "tolerance", cfg._tolerance, 100, true);

				ImGui::SetNextItemWidth(100);
				INPUT_FIELD(" Delay (1000)", "delay", cfg._delay, 1000, false);

				ImGui::SetNextItemWidth(100);
				INPUT_FIELD(" Hotkey (default 'LAlt')", "hotkey", cfg._hotkey, 255, true);

				ImGui::SetNextItemWidth(100);
				INPUT_FIELD(" Second key (default 'K')", "second_key", cfg._second_key, 255, true);

				ImGui::SetCursorPosX(WINDOW_X_SIZE / 2 - (ImGui::CalcTextSize("'INSERT' show/hide :: 'END' start/stop").x / 2));
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.7f, 0.9f, 1.0f, 0.5f });
				ImGui::Text("'INSERT' show/hide :: 'END' start/stop");
				ImGui::PopStyleColor();

				/* exit button */
				ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f, 0.5f, 0.8f, 0.5f });
				if (ImGui::Button("EXIT", { 100, 30 }))
					glfwSetWindowShouldClose(pwnd, true);
				ImGui::PopStyleColor();

				/* start/stop button */
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, TRIGGER_MAIN_LOOP ?
					ImVec4(1.0f, 0.2f, 0.2f, 0.5f) : ImVec4{ 0.2f, 0.8f, 0.2f, 0.5f }
				);
				if (ImGui::Button(TRIGGER_MAIN_LOOP ? "STOP" : "START", { 220, 30 }))
					TRIGGER_MAIN_LOOP = TRIGGER_MAIN_LOOP ? false : true;
				ImGui::PopStyleColor();
			}

			/* no glfw window (auto resize to imgui) */
			if (height != (int)ImGui::GetWindowHeight()) {
				height = (int)ImGui::GetWindowHeight();
				glfwSetWindowSize(pwnd, WINDOW_X_SIZE, height);
			}

			/* imgui end */
			ImGui::End();

			/* render */
			ImGui::Render();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(pwnd);
		}

		/* ignore loop 'INSERT' */
		else {
			if (height)
				glfwSetWindowSize(pwnd, 0, 0);
			__sleep(5);
		}

	}
	/* loop end */

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool GLFWInit() {
	if (!glfwInit())
		return false;

	/* glfw window flags */
	glfwWindowHint(GLFW_DECORATED,				 0);
	glfwWindowHint(GLFW_RESIZABLE,				 0);
	glfwWindowHint(GLFW_FLOATING,				 1);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,   4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,   5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pwnd = glfwCreateWindow(
		WINDOW_X_SIZE, WINDOW_Y_SIZE,
		"gui", nullptr, nullptr
	);
	if (!pwnd) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(pwnd);
	glfwSwapInterval(1);

	return true;
}