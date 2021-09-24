#pragma once

#include <cmath>
#include "timer.h"
#include "window.h"
#include "graphics.h"

class App
{
public:
	bool Init(HINSTANCE hInstance, std::wstring title, std::wstring wnd_class, int width, int height) {
		// Start timer.
		timer.Start();

		// Init platform window.

		auto hwnd = window.Init(hInstance, title, wnd_class, width, height);
		if (!hwnd) {
			// TODO (frama): Add logger.
			return false;
		}

		// Init directx stuff...
		if (!gfx.Init(hwnd)) {
			// TODO (frama): Add logger.
			return false;
		}

		return true;
	}

	void Run() {
		while (window.ProcessMessages()) {
			do_one_frame();
		}
	}

	/*void Update() {
		double dt = timer.Elapsed();
		timer.Restart();
	}

	void Render() {

	}*/

private:
	void do_one_frame() {
		//const float c = sin((float)timer.Elapsed()) / 2.0f + 0.5f;

		gfx.ClearBuffer(0.0f, 0.0f, 1.0f);
		gfx.DrawTriangle();
		gfx.EndFrame();
	}

	Timer timer;
	Window window;
	Graphics gfx;
};
