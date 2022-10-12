#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture,
		VBO2, VAO2, EBO2, texture2,
		VBO3, VAO3, EBO3, texture3,
		VBO4, VAO4, EBO4, texture4,
		VBO5, VAO5, EBO5, texture5;
	float angle = 0,
		goalPosX,
		goalPosZ,
		stepX,
		stepZ;
	int goalPosI = 1;
	float *temp = new float[32];
	float *wayPointX = new float[32];
	float *wayPointZ = new float[32];

	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);

	void CreateWayPoint();

	void BuildColoredCube();
	void DrawColoredCube();

	void BuildColoredBody();
	void DrawColoredBody();

	void BuildColoredTail();
	void DrawColoredTail();

	void BuildColored2ndMotor();
	void DrawColored2ndMotor();

	void BuildColoredPlane();
	void DrawColoredPlane();
};

