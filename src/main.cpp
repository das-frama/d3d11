#include "pch.h"

#include "App.h"
#include "graphics/DXException.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	try {
		App app(hInstance, 1920, 1080);

		MSG msg = { 0 };
		while (msg.message != WM_QUIT) {
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// TODO handle game loop with deltatime and framerate
			 app.Update(0.0f);
			 app.Draw();
		}

		_CrtDumpMemoryLeaks();

		return static_cast<int>(msg.wParam);
	} catch (const DXException& e) {
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBoxA(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return EXIT_FAILURE;
}
