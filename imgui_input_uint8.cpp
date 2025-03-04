#include "imgui_input8.h"
#include <imgui.h>

void InputUint8(const char* label, uint8_t* v)
{
	int n = *v;
	ImGui::InputInt(label, &n);
	*v = n;
}
