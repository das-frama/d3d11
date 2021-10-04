#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    
}

void Camera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void Camera::Render()
{
	DX::XMFLOAT3 up, position, look_at;
	DX::XMVECTOR up_vector, position_vector, look_at_vector;
	DX::XMMATRIX rotation_matrix;
	float yaw, pitch, roll;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	up_vector = DX::XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	//position = this->position;
	// Load it into a XMVECTOR structure.
	position_vector = DX::XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;

	// Load it into a XMVECTOR structure.
	look_at_vector = DX::XMLoadFloat3(&look_at);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotation_matrix = DX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	look_at_vector = DX::XMVector3TransformCoord(look_at_vector, rotation_matrix);
	up_vector = DX::XMVector3TransformCoord(up_vector, rotation_matrix);

	// Translate the rotated camera position to the location of the viewer.
	look_at_vector = DX::XMVectorAdd(position_vector, look_at_vector);

	// Finally create the view matrix from the three updated vectors.
	view_matrix = DX::XMMatrixLookAtLH(position_vector, look_at_vector, up_vector);
}

void Camera::RenderBaseViewMatrix()
{
	DX::XMFLOAT3 up, position, look_at;
	DX::XMVECTOR up_vector, position_vector, look_at_vector;
	DX::XMMATRIX rotationMatrix;
	float yaw, pitch, roll;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	up_vector = XMLoadFloat3(&up);

	// Load it into a XMVECTOR structure.
	position_vector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;

	// Load it into a XMVECTOR structure.
	look_at_vector = XMLoadFloat3(&look_at);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = DX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	look_at_vector = DX::XMVector3TransformCoord(look_at_vector, rotationMatrix);
	up_vector = DX::XMVector3TransformCoord(up_vector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	look_at_vector = DX::XMVectorAdd(position_vector, look_at_vector);

	// Finally create the view matrix from the three updated vectors.
	base_view_matrix = DX::XMMatrixLookAtLH(position_vector, look_at_vector, up_vector);
}
