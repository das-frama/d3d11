#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

class Graphics {
public:
	~Graphics() = default;

	bool Init(HWND hwnd) {
		if (!init_directx(hwnd)) return false;

		return true;
	}

	void EndFrame() {
		swap->Present(1u, 0u);
	}

	void ClearBuffer(float r, float g, float b) {
		const float color[] = { r, g, b, 1.0f };
		context->ClearRenderTargetView(target.Get(), color);
	}

	void DrawTriangle() {
		struct Vertex {
			float x, y;
		};
		const Vertex vertices[] = {
			{ 0.0f, 0.5f},
			{ 0.5f,-0.5f},
			{-0.5f,-0.5f},
		};

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
		D3D11_BUFFER_DESC bd = { 0 };
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA sd = { 0 };
		sd.pSysMem = vertices;

		device->CreateBuffer(&bd, &sd, &vertex_buffer);
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		context->IASetVertexBuffers(0u, 1u, vertex_buffer.GetAddressOf(), &stride, &offset);

		// create vertex shader.
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		D3DReadFileToBlob(L"VertexShader.cso", &blob);
		device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader);
		// bind vertex shader.
		context->VSSetShader(vertex_shader.Get(), nullptr, 0u);

		// create pixel shader.
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
		D3DReadFileToBlob(L"PixelShader.cso", &blob);
		device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader);
		// bind pixel shader.
		context->PSSetShader(pixel_shader.Get(), nullptr, 0u);

		// bind render target.
		context->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);

		// configure viewport.
		D3D11_VIEWPORT vp = { 0 };
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1u, &vp);

		context->Draw((UINT)std::size(vertices), 0u);
	}


private:
	bool init_directx(HWND hwnd) {
		DXGI_SWAP_CHAIN_DESC scd = {  };
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&scd,
			&swap,
			&device,
			nullptr,
			&context
		);
		if (FAILED(hr)) {
			// todo (frama): add error logger.
			return false;
		}


		Microsoft::WRL::ComPtr<ID3D11Resource> back_buffer = nullptr;

		hr = swap->GetBuffer(0, __uuidof(ID3D11Resource), &back_buffer);
		if (FAILED(hr)) {
			// todo (frama): add error logger.
			return false;
		}

		hr = device->CreateRenderTargetView(back_buffer.Get(), nullptr, &target);
		if (FAILED(hr)) {
			// todo (frama): add error logger.
			return false;
		}

		back_buffer->Release();

		return true;
	}

	bool init_shaders() {

	}

	bool init_scene() {

	}

	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target = nullptr;
};