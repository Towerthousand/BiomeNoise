#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <VBE-Scenegraph/scenegraph/GameObject.hpp>
#include <VBE/geometry/Frustum.hpp>

class Camera : public GameObject {
	public:
		Camera(const std::string &cameraName = "", const vec3f& pos = vec3f(0.0f));
		virtual ~Camera();

		void lookInDir(const vec3f& direction);
		void rotateLocal(float angle, const vec3f& axis);
		void rotateGlobal(float angle, const vec3f& axis);
		void rotateAround(float angle, const vec3f& axis, const vec3f& center);

		vec3f getWorldPos() const;
		vec3f getForward() const;
		mat4f getView() const;
		const Frustum& getFrustum() const;
		void recalculateFrustum();

		vec3f pos = vec3f(0.0f);
		mat4f projection = mat4f(1.0f);
	protected:
		virtual void update(float deltaTime) override;

		mat4f rotation = mat4f(1.0f);
		Frustum frustum;
};

#endif // CAMERA_HPP
