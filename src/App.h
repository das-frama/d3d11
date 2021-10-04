#pragma once

#include "Timer.h"
#include "graphics/Graphics.h"

class App {
public:
	App(HINSTANCE& hInstance, int width, int height);
	~App();

	void Update(float dt);
	void Draw();
	void Clear() const;

private:
	// Init.
	void init_window(HINSTANCE& hInstance);
	void init_directx11();
	void init_imgui();

	// Handle win32.
	static LRESULT CALLBACK window_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND hwnd;
	const wchar_t* class_name;

	int width;
	int height;

	//  DirectX Objects needed for any interaction with the API
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	Graphics gfx;
	//Timer timer;
	//Window window;
	// App stuff...
};
