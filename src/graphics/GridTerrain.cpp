#include "pch.h"
#include "GridTerrain.h"
#include "DXException.h"

void GridTerrain::Init(ID3D11Device *device, int width, int height)
{
	this->width = width;
	this->height = height;
	
	VertexType *vertices = nullptr;
	UINT *indices = nullptr;

	D3D11_BUFFER_DESC vertex_bd, index_bd;
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;
	DX::XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	// Calculate the number of vertices in the terrain.
	index_count = vertex_count = (width - 1) * (height - 1) * 8;
	vertices = new VertexType[vertex_count];
	if (!vertices)
		return;
	indices = new UINT[index_count];
	if (!indices)
		return;

	// Load the vertex array and index array with data.
	int index = 0;
	float pos_x = 0, pos_z = 0;
	for (int j = 0; j < (height - 1); j++)
	{
		for (int i = 0; i < (width - 1); i++)
		{
			// Line 1 - Upper left.
			pos_x = (float)i;
			pos_z = (float)(j + 1);

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 1 - Upper right.
			pos_x = (float)(i + 1);
			pos_z = (float)(j + 1);

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 2 - Upper right
			pos_x = (float)(i + 1);
			pos_z = (float)(j + 1);

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 2 - Bottom right.
			pos_x = (float)(i + 1);
			pos_z = (float)j;

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 3 - Bottom right.
			pos_x = (float)(i + 1);
			pos_z = (float)j;

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 3 - Bottom left.
			pos_x = (float)i;
			pos_z = (float)j;

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 4 - Bottom left.
			pos_x = (float)i;
			pos_z = (float)j;

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 4 - Upper left.
			pos_x = (float)i;
			pos_z = (float)(j + 1);

			vertices[index].position = DX::XMFLOAT3(pos_x, 0.0f, pos_z);
			vertices[index].color = color;
			indices[index] = index;
			index++;
		}
	}

	// Set up the description of the static vertex buffer.
	{
		vertex_bd.Usage = D3D11_USAGE_DEFAULT;
		vertex_bd.ByteWidth = sizeof(VertexType) * vertex_count;
		vertex_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_bd.CPUAccessFlags = 0;
		vertex_bd.MiscFlags = 0;
		vertex_bd.StructureByteStride = 0;
	}

	// Give the subresource structure a pointer to the vertex data.
	{
		vertex_data.pSysMem = vertices;
		vertex_data.SysMemPitch = 0;
		vertex_data.SysMemSlicePitch = 0;
	}

	// Now create the vertex buffer.
	HRESULT hr;
	{
		hr = device->CreateBuffer(&vertex_bd, &vertex_data, &vertex_buffer);
		ThrowIfFailed(CALL_INFO, hr);
	}

	// Set up the description of the static index buffer.
	{
		index_bd.Usage = D3D11_USAGE_DEFAULT;
		index_bd.ByteWidth = sizeof(unsigned long) * index_count;
		index_bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_bd.CPUAccessFlags = 0;
		index_bd.MiscFlags = 0;
		index_bd.StructureByteStride = 0;
	}

	// Give the subresource structure a pointer to the index data.
	{
		index_data.pSysMem = indices;
		index_data.SysMemPitch = 0;
		index_data.SysMemSlicePitch = 0;
	}

	// Create the index buffer.
	{
		hr = device->CreateBuffer(&index_bd, &index_data, &index_buffer);
		ThrowIfFailed(CALL_INFO, hr);
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

GridTerrain::~GridTerrain()
{
	// Release the index buffer.
	if (index_buffer)
	{
		index_buffer->Release();
		index_buffer = 0;
	}

	// Release the vertex buffer.
	if (vertex_buffer)
	{
		vertex_buffer->Release();
		vertex_buffer = 0;
	}
}

void GridTerrain::Render(ID3D11DeviceContext *device_context)
{
	unsigned int stride = sizeof(VertexType);
	;
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case lines.
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}