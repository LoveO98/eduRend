#include "Camera.h"

using namespace linalg;

void Camera::MoveTo(const vec3f& position) noexcept
{
	m_position = position;
}

void Camera::Move(const vec4f& direction, const float velocity) noexcept
{
	m_movement = ViewToWorldMatrix() * direction * velocity;
	m_position += {m_movement.x, m_movement.y, m_movement.z};
}

void Camera::RotateTo(const long& pitch, const long& yaw) noexcept
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_rotation = mat4f::rotation(0, yaw, pitch);
}

void Camera::Rotate(const long& pitch, const long& yaw) noexcept
{
	m_pitch -= pitch * m_yaw_sens;
	m_pitch = clamp(m_pitch, -1.57f, 1.57f);
	m_yaw -= yaw * m_pitch_sens;
	m_rotation = mat4f::rotation(0, m_yaw, m_pitch);
}

mat4f Camera::WorldToViewMatrix() const noexcept
{
	return transpose(m_rotation) * mat4f::translation(-m_position);
}

mat4f Camera::ViewToWorldMatrix() const noexcept
{
	return mat4f::translation(m_position) * m_rotation;
}

void Camera::ChangeSens(const float& pitch_sens, const float& yaw_sens) noexcept
{
	m_pitch_sens += pitch_sens;
	m_yaw_sens += pitch_sens;
}

linalg::vec4f Camera::GetCamPosVec4f() const noexcept
{
	return { m_position, 0 };
}


// ------ Comment from withing the WorldToViewMatrix method ------
// Assuming a camera's position and rotation is defined by matrices T(p) and R,
	// the View-to-World transform is T(p)*R (for a first-person style camera).
	//
	// World-to-View then is the inverse of T(p)*R;
	//		inverse(T(p)*R) = inverse(R)*inverse(T(p)) = transpose(R)*T(-p)
	// Since now there is no rotation, this matrix is simply T(-p)



mat4f Camera::ProjectionMatrix() const noexcept
{
	return mat4f::projection(m_vertical_fov, m_aspect_ratio, m_near_plane, m_far_plane);
}