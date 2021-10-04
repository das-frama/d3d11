#pragma once
#include "pch.h"

class GridTerrain
{
public:
	~GridTerrain();

    void Init(ID3D11Device* device, int width, int height);
	void Render(ID3D11DeviceContext* device_context);

private:
	ID3D11Buffer* vertex_buffer;
	ID3D11Buffer* index_buffer;

	int vertex_count;
	int index_count;

	int width;
	int height;

private:
	struct VertexType
	{
		DX::XMFLOAT3 position;
		DX::XMFLOAT4 color;
	};
};

