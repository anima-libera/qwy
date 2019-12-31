
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header camera.hpp - Defines the Camera class */

#ifndef __qwy_camera_header__
#define __qwy_camera_header__

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace qwy
{

	class Camera
	{
	public:
		Camera();
		~Camera();

		/* Compute the matrices to take into account vector modifications (called automatically) */
		void update_matrices();

		/* Compute the vectors to take into account angle modifications (called automatically) */
		void update_vectors();

		/* Set the camera aspect ratio and update the matrices
		 * (note that it must be the same as the rendering area (the window), and that it's w/h) */
		void set_aspect_ratio(float aspect_ratio);

		/* Set the camera position and update the matrices */
		void set_position(float x, float y, float z);

		/* Move the camera position and update the matrices */
		void move_position(float x, float y, float z);
		void move_position(glm::vec3 const& vec);

		/* Modify the camera angles and update vectors and matrices */
		void move_angles(float horizontal_angle_increasement, float vertical_angle_increasement);

		/* Modify the fov value relatively by adding it the passed augmentation (in radians) */
		void augment_fov(float fov_augmentation);

		/* Get the specified attributes */
		float get_horizontal_angle() const {return m_horizontal_angle;}
		float get_vertical_angle() const {return m_vertical_angle;}
		glm::vec3 const& get_direction() const;
		glm::vec3 const& get_right() const;
		float get_fov() const {return m_fov;}
		glm::mat4 const& get_matrix() const;
		
		/* Get some horizontal normalized vector components */
		glm::vec3 const get_horizontal_direction() const;
		glm::vec3 const get_horizontal_right() const;

	private:
		/* Camera view */
		float m_horizontal_angle;
		float m_vertical_angle;
		glm::vec3 m_position;
		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;
		glm::mat4 m_view_matrix;
		
		/* Camera projection */
		float m_fov;
		float m_aspect_ratio;
		float m_near_plane;
		float m_far_plane;
		glm::mat4 m_projection_matrix;
		
		/* Camera final matrix */
		glm::mat4 m_matrix;
	};

}

#endif /* __qwy_camera_header__ */
