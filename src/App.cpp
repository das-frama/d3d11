#include "pch.h"
#include "App.h"

#include "graphics/DXException.h"

App::App(HINSTANCE &hInstance, int width, int height)
	: class_name(L"itmo_cglabs"), hwnd(nullptr), width(width), height(height)
{
	init_window(hInstance);
	init_directx11();
	init_imgui();

	grid.Init(device.Get(), width, height);
}

App::~App() {}

void App::Update(float dt)
{
}

void App::Draw()
{
	Clear();


	grid.Render(context.Get());

	swap_chain->Present(1u, 0u);
}

void App::Clear() const {
	const float color[] = { 0.35f, 0.35f, 0.35f, 1.0f };
	context->ClearRenderTargetView(target.Get(), color);
	context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void App::init_window(HINSTANCE &hInstance)
{
	// Register window class
	{
		WNDCLASSEX wc;
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = window_callback;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = class_name;
		wc.hIconSm = nullptr;
		RegisterClassEx(&wc);
	}

	// Create window instance
	{
		hwnd = CreateWindowEx(
			0, class_name, L"Solar System & Catamari",
			WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			nullptr, nullptr, hInstance, nullptr);
		if (hwnd == nullptr)
		{
			throw DX_LAST_ERROR_EXCEPTION;
		}
	}

	ShowWindow(hwnd, SW_SHOW);
}

void App::init_directx11()
{
	// Get viewport size
	RECT rc;
	GetClientRect(hwnd, &rc);
	const int width = rc.right - rc.left;
	const int height = rc.bottom - rc.top;

	// SwapChain and Context description
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1; // Double buffering
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT createDeviceFlags = 0;
#ifndef NDEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;

	// Create DirectX device.
	{
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags, nullptr, 0,
			D3D11_SDK_VERSION, &sd, &swap_chain,
			&device, nullptr, &context);
		ThrowIfFailed(CALL_INFO, hr);
	}

	// Get back buffer.
	Microsoft::WRL::ComPtr<ID3D11Resource> back_buffer;
	{
		hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), &back_buffer);
		ThrowIfFailed(CALL_INFO, hr);
		hr = device->CreateRenderTargetView(back_buffer.Get(), nullptr, &target);
		ThrowIfFailed(CALL_INFO, hr);
	}

	// Create the depth stencil state.
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	{
		D3D11_DEPTH_STENCIL_DESC ds_desc = {};
		ds_desc.DepthEnable = true;
		ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
		hr = device->CreateDepthStencilState(&ds_desc, &depth_stencil_state);
		ThrowIfFailed(CALL_INFO, hr);
	}

	// Bind depth stencil state.
	context->OMSetDepthStencilState(depth_stencil_state.Get(), 1);

	// Create depth stencil texture.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil;
	{
		D3D11_TEXTURE2D_DESC desc_depth;
		desc_depth.Width = width;
		desc_depth.Height = height;
		desc_depth.MipLevels = 1;
		desc_depth.ArraySize = 1;
		desc_depth.Format = DXGI_FORMAT_D32_FLOAT;
		desc_depth.SampleDesc.Count = 1;
		desc_depth.SampleDesc.Quality = 0;
		desc_depth.Usage = D3D11_USAGE_DEFAULT;
		desc_depth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc_depth.CPUAccessFlags = 0;
		desc_depth.MiscFlags = 0;
		hr = device->CreateTexture2D(&desc_depth, nullptr, &depth_stencil);
		ThrowIfFailed(CALL_INFO, hr);
	}

	// Create the depth stencil view.
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc_dsv = {};
		desc_dsv.Format = DXGI_FORMAT_D32_FLOAT;
		desc_dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc_dsv.Texture2D.MipSlice = 0;
		hr = device->CreateDepthStencilView(depth_stencil.Get(), &desc_dsv, &depth_stencil_view);
		ThrowIfFailed(CALL_INFO, hr);
	}

	// Setup the viewport.
	{
		D3D11_VIEWPORT vp;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1, &vp);
	}

	// Bind what will not change for a while.
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->OMSetRenderTargets(1u, target.GetAddressOf(), depth_stencil_view.Get());
	}
}

void App::init_imgui()
{
}

LRESULT CALLBACK
App::window_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	// 	return true;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	// App* me = (App*) (GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// if (me && App::isContexInit && !ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) {
	// 	return me->memberWndProc(hWnd, msg, wParam, lParam);
	// }

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
