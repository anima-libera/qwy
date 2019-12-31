
#include "camera.hpp"

#include "utility/math.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


qwy::Camera::Camera():
	m_horizontal_angle(0.0f),
	m_vertical_angle(0.0f),
	m_fov(qwy::deg_to_rad(60.0f)),
	m_aspect_ratio(4.0f/3.0f),
	m_near_plane(0.1f),
	m_far_plane(300.0f)
{
	update_vectors();
	update_matrices();
}

qwy::Camera::~Camera()
{}

/* Compute the matrices to take into account parameter modifications (called automatically) */
void qwy::Camera::update_matrices()
{
	m_view_matrix = glm::lookAt(
		m_position,
		m_position + m_direction, /* Camera target */
		m_up
	);
	m_projection_matrix = glm::perspective(
		m_fov,
		m_aspect_ratio,
		m_near_plane,
		m_far_plane
	);
	m_matrix = m_projection_matrix * m_view_matrix;
}

/* Compute the vectors to take into account angle modifications (called automatically) */
void qwy::Camera::update_vectors()
{
	/* Update the direction */
	m_direction.x = cos(m_vertical_angle) * sin(m_horizontal_angle);
	m_direction.y = sin(m_vertical_angle);
	m_direction.z = cos(m_vertical_angle) * cos(m_horizontal_angle);

	/* Update the right direction */
	m_right.x = -cos(m_horizontal_angle);
	m_right.y = 0;
	m_right.z = sin(m_horizontal_angle);

	/* Update the up direction */
	m_up = glm::cross(m_right, m_direction);
}

/* Set the camera aspect ratio and update the matrices
 * (note that it must be the same as the rendering area (the window), and that it's w/h) */
void qwy::Camera::set_aspect_ratio(float aspect_ratio)
{
	m_aspect_ratio = aspect_ratio;
	update_matrices();
}

/* Set the camera position and update the matrices */
void qwy::Camera::set_position(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	update_matrices();
}

/* Move the camera position and update the matrices */
void qwy::Camera::move_position(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	update_matrices();
}
void qwy::Camera::move_position(glm::vec3 const& vec)
{
	m_position += vec;
	update_matrices();
}

/* Modify the camera angles and update vectors and matrices */
void qwy::Camera::move_angles(float horizontal_angle_increasement, float vertical_angle_increasement)
{
	m_horizontal_angle += horizontal_angle_increasement;
	m_vertical_angle += vertical_angle_increasement;
	m_vertical_angle = qwy::restrict_to_interval(m_vertical_angle, -qwy::tau/4.0f, qwy::tau/4.0f);
	update_vectors();
	update_matrices();
}

/* Modify the fov value relatively by adding it the passed augmentation (in radians) */
void qwy::Camera::augment_fov(float fov_augmentation)
{
	m_fov += fov_augmentation;
	update_matrices();
}

/* Get the specified attributes */
glm::vec3 const& qwy::Camera::get_direction() const
{
	return m_direction;
}
glm::vec3 const& qwy::Camera::get_right() const
{
	return m_right;
}
glm::mat4 const& qwy::Camera::get_matrix() const
{
	return m_matrix;
}

/* Get some horizontal normalized vector components */
glm::vec3 const qwy::Camera::get_horizontal_direction() const
{
	glm::vec3 horizontal_direction(0.0f, 0.0f, 0.0f);

	/* If the m_direction vector if collinear to the y direction, the we have to use the up vector */
	if (m_direction.y >= 0.99f)
	{
		horizontal_direction.x = -m_up.x;
		horizontal_direction.z = -m_up.z;
	}
	else if (m_direction.y <= -0.99f)
	{
		horizontal_direction.x = m_up.x;
		horizontal_direction.z = m_up.z;
	}
	else
	{
		horizontal_direction.x = m_direction.x;
		horizontal_direction.z = m_direction.z;
	}

	return glm::normalize(horizontal_direction);
}
glm::vec3 const qwy::Camera::get_horizontal_right() const
{
	glm::vec3 horizontal_right(m_right.x, 0.0f, m_right.z);
	return glm::normalize(horizontal_right);
}
