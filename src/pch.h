#pragma once

// Windows API
#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

// Debug
#include <exception>
#include <stdexcept>
#include <cassert>

// Streams
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

// Data structures
#include <vector>
#include <string>
#include <stdint.h>
#include <tuple>
#include <stdint.h>

// DirectX
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dxgidebug.h>
#include <wrl.h>
#include <d3dcompiler.h>

namespace DX = DirectX;

// Libs
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
