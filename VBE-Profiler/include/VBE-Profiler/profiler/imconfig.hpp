#ifndef IMCONFIG_HPP
#define IMCONFIG_HPP

#include <VBE/math.hpp>

//-----------------------------------------------------------------------------
// USER IMPLEMENTATION
// This file contains compile-time options for ImGui.
// Other options (memory allocation overrides, callbacks, etc.) can be set at runtime via the ImGuiIO structure - ImGui::GetIO().
//-----------------------------------------------------------------------------

//---- Define your own ImVector<> type if you don't want to use the provided implementation defined in imgui.h
//#include <vector>
//#define ImVector  std::vector
//#define ImVector  MyVector

//---- Define assertion handler. Defaults to calling assert().
//#define IM_ASSERT(_EXPR)  MyAssert(_EXPR)

//---- Don't implement default clipboard handlers for Windows (so as not to link with OpenClipboard() and others Win32 functions)
//#define IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS

//---- Include imgui_user.inl at the end of imgui.cpp so you can include code that extends ImGui using its private data/functions.
//#define IMGUI_INCLUDE_IMGUI_USER_INL

//---- Include imgui_user.h at the end of imgui.h
//#define IMGUI_INCLUDE_IMGUI_USER_H

//---- Define implicit cast operators to convert back<>forth from your math types and ImVec2/ImVec4.
#define IM_VEC2_CLASS_EXTRA                                                 \
		ImVec2(const vec2f& f) { x = f.x; y = f.y; }                       \
		operator vec2f() const { return vec2f(x,y); }

#define IM_VEC4_CLASS_EXTRA                                                 \
		ImVec4(const vec4f& f) { x = f.x; y = f.y; z = f.z; w = f.w; }     \
		operator vec4f() const { return vec4f(x,y,z,w); }

//---- Freely implement extra functions within the ImGui:: namespace.
//---- Declare helpers or widgets implemented in imgui_user.inl or elsewhere, so end-user doesn't need to include multiple files.
//---- e.g. you can create variants of the ImGui::Value() helper for your low-level math types.
/*
namespace ImGui
{
	void    Value(const char* prefix, const MyVec2& v, const char* float_format = NULL);
	void    Value(const char* prefix, const MyVec4& v, const char* float_format = NULL);
}
*/

#endif //IMCONFIG_HPP
