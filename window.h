#pragma once
#include <Windows.h>
#include <string>

class Window {
public:
	~Window() {
		if (hwnd != nullptr) {
			UnregisterClass(wnd_class.c_str(), hInstance);
			DestroyWindow(hwnd);
		}
	}

	HWND Init(HINSTANCE hInstance, const std::wstring& title, const std::wstring& wnd_class, int width, int height) {
		this->hInstance = hInstance;
		this->title = title;
		this->wnd_class = wnd_class;
		this->width = width;
		this->height = height;

		// Register wnd class.
		{
			WNDCLASSEX wc = { 0 };
			// Setup the windows class with default settings.
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = handle_win32_message;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = wnd_class.c_str();
			wc.cbSize = sizeof(WNDCLASSEX);
			RegisterClassEx(&wc);
		}

		// Create window.
		{
			hwnd = CreateWindowEx(0, //default Window style
				this->wnd_class.c_str(), //Window class name
				this->title.c_str(), //Window title
				WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Windows style
				0, //Window X position
				0, //Window Y position
				width, //Window width
				height, //Window height
				nullptr, //Handle to parent of this window (if first - has no parents)
				nullptr, //Handle to menu or child window identifier
				hInstance, //Handle to the instance of module to be used with this window
				nullptr); //Param to create window
			if (hwnd == nullptr) {
				return nullptr;
			}
		}

		ShowWindow(hwnd, SW_SHOW);
		SetFocus(hwnd);
		ShowCursor(true);

		return hwnd;
	}

	bool ProcessMessages() {
		// Handle the windows messages.
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.
		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Check if the window was closed.
		if (msg.message == WM_NULL) {
			if (!IsWindow(hwnd)) {
				hwnd = nullptr;
				UnregisterClass(wnd_class.c_str(), hInstance);
				return false;
			}
		}

		return true;
	}


private:
	static LRESULT CALLBACK
		handle_win32_message(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}

	HINSTANCE hInstance = nullptr;
	HWND hwnd = nullptr;
	std::wstring title;
	std::wstring wnd_class;
	int width;
	int height;
};
