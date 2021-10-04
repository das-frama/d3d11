#pragma once

#include "pch.h"

struct VertexShader
{
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> constant_buffers;
};

struct PixelShader
{
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> constant_buffers;
};
