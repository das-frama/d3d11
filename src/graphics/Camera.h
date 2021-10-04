#pragma once

//#include "Shaders.h"

class Camera
{
public:
	Camera();

    void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DX::XMFLOAT3 GetPosition() { return position; }
	DX::XMFLOAT3 GetRotation() { return rotation; }

    void Render();
	void GetViewMatrix(DX::XMMATRIX& vm) { view_matrix = vm; }

	void RenderBaseViewMatrix();
	void GetBaseViewMatrix(DX::XMMATRIX& bvm) { base_view_matrix = bvm; }
private:
	DX::XMMATRIX view_matrix;
	DX::XMMATRIX base_view_matrix;
	DX::XMFLOAT3 position = { 0, 0, 0 };
	DX::XMFLOAT3 rotation = { 0, 0, 0 };

	/*float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix, m_baseViewMatrix;*/

	float radius = 5;
	float up = 1;
	float phi = 0;
	float theta = 0;

	bool needs_to_be_updated = false;
};
