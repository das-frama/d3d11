#include "app.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	App app;
	if (!app.Init(hInstance, L"Hello", L"ITMO_CG", 1920, 1080)) {
		// todo (frama): add logger.
		return EXIT_FAILURE;
	}
	app.Run();

	return EXIT_SUCCESS;
}
