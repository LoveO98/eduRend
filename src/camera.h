/**
 * @file camera.h
 * @brief Basic camera class
*/

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vec\vec.h"
#include "vec\mat.h"

/**
 * @brief Manages camera data, also handles generation of view and projection matrices.
*/
class Camera
{
public:
	/**
	 * @brief Creates a camera.
	 * @param[in] vertical_fov Vertical field of view.
	 * @param[in] aspect_ratio Aspect ratio, calculate from screen width / sceen height.
	 * @param[in] near_plane Near plane distance.
	 * @param[in] far_plane Far plane distance, must be larger than the near plane.
	*/
	inline constexpr Camera(float vertical_fov, float aspect_ratio, float near_plane, float far_plane, float pitch_sens, float yaw_sens) noexcept 
		: m_vertical_fov(vertical_fov), m_aspect_ratio(aspect_ratio), m_near_plane(near_plane), m_far_plane(far_plane), m_position(0.0f), m_pitch(0), m_yaw(0), m_pitch_sens(pitch_sens), m_yaw_sens(yaw_sens), m_rotation(1.0f) {}

	/**
	 * @brief Move the camera to a new position
	 * @param[in] position New position for the camera
	*/
	void MoveTo(const linalg::vec3f& position) noexcept;

	/**
	 * @brief Move the camera along a vector
	 * @param[in] direction Direction to move along
	*/
	void Move(const linalg::vec4f& direction, const float velocity) noexcept;

	/**
	* @brief Rotates the camera to a new position
	* @param[in] pitch Amount of pitch to set the camera to
	* @param[in] yaw Amount of yaw to set the camera to
	*/
	void RotateTo(const long& pitch, const long& yaw) noexcept;

	/**
	 * @brief Rotates the camera view
	 * @param[in] pitch Amount to pitch the camera
	 * @param[in] yaw Amount to yaw the camera
	*/
	void Rotate(const long& pitch, const long& yaw) noexcept;

	/**
	 * @brief Changes the mouse sensitivity
	 * @param[in] pitch Amount to change the pitch sensitivity
	 * @param[in] yaw Amount to change the yaw sensitivity
	*/
	void ChangeSens(const float& pitch_sens, const float& yaw_sens) noexcept;

	/**
	 * @brief Changes the camera aspect ratio.
	 * @param[in] aspect_ratio New aspect ratio, calculate with width / height
	*/
	inline void SetAspect(float aspect_ratio) noexcept { m_aspect_ratio = aspect_ratio; }

	/**
	 * @brief Get the World-to-View matrix of the camera.
	 * @return World-to-View matrix.
	*/
	linalg::mat4f WorldToViewMatrix() const noexcept;


	/**
	* @brief Get the View-to-World matrix of the camera
	* @return View-to-World matrix
	*/
	linalg::mat4f ViewToWorldMatrix() const noexcept;

	/**
	 * @brief get the Matrix transforming from View space to Clip space
	 * @return Projection matrix.
	 * 
	 * @note In a performance sensitive situation this matrix should be precomputed if possible
	*/
	linalg::mat4f ProjectionMatrix() const noexcept;

	linalg::vec4f GetCamPosVec4f() const noexcept;

private:
	// Aperture attributes
	float m_vertical_fov;
	float m_aspect_ratio;

	// Clip planes in view space coordinates
	// Evrything outside of [m_near_plane, m_far_plane] is clipped away on the GPU side
	// m_near_plane should be > 0
	// m_far_plane should depend on the size of the scene
	// This range should be kept as tight as possibly to improve
	// numerical precision in the z-buffer
	float m_near_plane;
	float m_far_plane;

	linalg::vec3f m_position;
	linalg::vec4f m_movement;
	float m_pitch;
	float m_yaw;
	float m_pitch_sens;
	float m_yaw_sens;
	linalg::mat4f m_rotation;
};

#endif