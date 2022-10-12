#include "Demo.h"
using namespace std;
#include <math.h>

Demo::Demo() {

}

Demo::~Demo() {
}

void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	CreateWayPoint();
	stepX = wayPointX[0];
	stepZ = wayPointZ[0];
	goalPosX = wayPointX[1];
	goalPosZ = wayPointZ[1];

	BuildColoredBody();
	BuildColoredCube();
	BuildColoredTail();
	BuildColored2ndMotor();

	BuildColoredPlane();
}

void Demo::CreateWayPoint() {
	int segmentCount = 8;
	float px[] = { -11, -13, -15, -15, -13, -11, 11, 13, 15,  15,  13,  11 };
	float pz[] = { -15, -13, -11,  11,  13,  15, 15, 13, 11, -11, -13, -15 };

	int ii = 0;
	// 1
	for (int i = 0; i < segmentCount; i++) {
		float t = (float)i / segmentCount;
		float x = ((1 - t) * (1 - t) * px[0]) + (2 * (1 - t) * t * px[1]) + (t * t * px[2]);
		float z = ((1 - t) * (1 - t) * pz[0]) + (2 * (1 - t) * t * pz[1]) + (t * t * pz[2]);

		wayPointX[i] = x;
		wayPointZ[i] = z;
	}
	// 2
	for (int i = 8; i < 8 + segmentCount; i++) {
		float t = (float)(i - 8) / segmentCount;
		float x = ((1 - t) * (1 - t) * px[3]) + (2 * (1 - t) * t * px[4]) + (t * t * px[5]);
		float z = ((1 - t) * (1 - t) * pz[3]) + (2 * (1 - t) * t * pz[4]) + (t * t * pz[5]);

		wayPointX[i] = x;
		wayPointZ[i] = z;
	}
	// 3
	for (int i = 16; i < 16 + segmentCount; i++) {
		float t = (float)(i - 16) / segmentCount;
		float x = ((1 - t) * (1 - t) * px[6]) + (2 * (1 - t) * t * px[7]) + (t * t * px[8]);
		float z = ((1 - t) * (1 - t) * pz[6]) + (2 * (1 - t) * t * pz[7]) + (t * t * pz[8]);

		wayPointX[i] = x;
		wayPointZ[i] = z;
	}
	// 4
	for (int i = 24; i < 24 + segmentCount; i++) {
		float t = (float)(i - 24) / segmentCount;
		float x = ((1 - t) * (1 - t) * px[9]) + (2 * (1 - t) * t * px[10]) + (t * t * px[11]);
		float z = ((1 - t) * (1 - t) * pz[9]) + (2 * (1 - t) * t * pz[10]) + (t * t * pz[11]);

		wayPointX[i] = x;
		wayPointZ[i] = z;
	}

	//printf("%f - %f\n", wayPointX[i], wayPointZ[i]);
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);

	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);

	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);

	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &EBO5);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Demo::Update(double deltaTime) {
	angle += (float)((deltaTime * 1.5f) / 100);

	if (stepX < goalPosX)
		stepX += (float)((deltaTime * 1.5f) / 100);
	else if (stepX > goalPosX)
		stepX -= (float)((deltaTime * 1.5f) / 100);
	else
		stepX = goalPosX;

	if (stepZ < goalPosZ)
		stepZ += (float)((deltaTime * 1.5f) / 100);
	else if (stepZ > goalPosZ)
		stepZ -= (float)((deltaTime * 1.5f) / 100);
	else
		stepZ = goalPosZ;

	float tlr = .1f;
	if (stepX >= goalPosX - tlr && stepX <= goalPosX + tlr && stepZ >= goalPosZ - tlr && stepZ <= goalPosZ + tlr) {
		if (goalPosI >= 31)
			goalPosI = 0;
		else
			goalPosI++;

		goalPosX = wayPointX[goalPosI];
		goalPosZ = wayPointZ[goalPosI];
	}
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 50, 5), glm::vec3(5, 0, 5), glm::vec3(0, 1, 0));
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 5, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredBody();
	DrawColoredCube();
	DrawColoredTail();
	DrawColored2ndMotor();
	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

#pragma region Main Motor
void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1.5, .6, .125, 0, 0,  // 0
		1.5, .6, .125, 1, 0,   // 1
		1.5,  .65, .125, 1, 1,   // 2
		-1.5,  .66, .125, 0, 1,  // 3

		// right
		1.5,  .65,  .125, 0, 0,  // 4
		1.5,  .65, -.125, 1, 0,  // 5
		1.5, .6, -.125, 1, 1,  // 6
		1.5, .6,  .125, 0, 1,  // 7

		// back
		-1.5, .6, -.125, 0, 0, // 8 
		1.5,  .6, -.125, 1, 0, // 9
		1.5,   .65, -.125, 1, 1, // 10
		-1.5,  .65, -.125, 0, 1, // 11

		// left
		-1.5, .6, -.125, 0, 0, // 12
		-1.5, .6,  .125, 1, 0, // 13
		-1.5, .65,  .125, 1, 1, // 14
		-1.5, .65, -.125, 0, 1, // 15

		// upper
		1.5, .65,  .125, 0, 0,   // 16
		-1.5, .65,  .125, 1, 0,  // 17
		-1.5, .65, -.125, 1, 1,  // 18
		1.5, .65, -.125, 0, 1,   // 19

		// bottom
		-1.5, .6, -.125, 0, 0, // 20
		1.5, .6, -.125, 1, 0,  // 21
		1.5, .6,  .125, 1, 1,  // 22
		-1.5, .6,  .125, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,  // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;

	model = glm::translate(model, glm::vec3(stepX, 3, stepZ));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
#pragma endregion

#pragma region Body
void Demo::BuildColoredBody() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-.5, -.5, .5, 0, 0,  // 0
		.5, -.5, .5, 1, 0,   // 1
		.5,  .5, .5, 1, 1,   // 2
		-.5,  .5, .5, 0, 1,  // 3

		// right
		.5,  .5,  .5, 0, 0,  // 4
		.5,  .5, -.5, 1, 0,  // 5
		.5, -.5, -.5, 1, 1,  // 6
		.5, -.5,  .5, 0, 1,  // 7

		// back
		-.5, -.5, -.5, 0, 0, // 8 
		.5,  -.5, -.5, 1, 0, // 9
		.5,   .5, -.5, 1, 1, // 10
		-.5,  .5, -.5, 0, 1, // 11

		// left
		-.5, -.5, -.5, 0, 0, // 12
		-.5, -.5,  .5, 1, 0, // 13
		-.5,  .5,  .5, 1, 1, // 14
		-.5,  .5, -.5, 0, 1, // 15

		// upper
		.5, .5,  .5, 0, 0,   // 16
		-.5, .5,  .5, 1, 0,  // 17
		-.5, .5, -.5, 1, 1,  // 18
		.5, .5, -.5, 0, 1,   // 19

		// bottom
		-.5, -.5, -.5, 0, 0, // 20
		.5, -.5, -.5, 1, 0,  // 21
		.5, -.5,  .5, 1, 1,  // 22
		-.5, -.5,  .5, 0, 1, // 23

		// -----
		// front
		-.1, .5, .1, 0, 0,  // 0
		.1, .5, .1, 1, 0,   // 1
		.1,  .6, .1, 1, 1,   // 2
		-.1,  .6, .1, 0, 1,  // 3

		// right
		.1,  .6,  .1, 0, 0,  // 4
		.1,  .6, -.1, 1, 0,  // 5
		.1, .5, -.1, 1, 1,  // 6
		.1, .5,  .1, 0, 1,  // 7

		// back
		-.1, .5, -.1, 0, 0, // 8 
		.1,  .5, -.1, 1, 0, // 9
		.1,   .6, -.1, 1, 1, // 10
		-.1,  .6, -.1, 0, 1, // 11

		// left
		-.1, .5, -.1, 0, 0, // 12
		-.1, .5,  .1, 1, 0, // 13
		-.1, .6,  .1, 1, 1, // 14
		-.1, .6, -.1, 0, 1, // 15

		// upper
		.1, .6,  .1, 0, 0,   // 16
		-.1, .6,  .1, 1, 0,  // 17
		-.1, .6, -.1, 1, 1,  // 18
		.1, .6, -.1, 0, 1,   // 19

		// bottom
		-.1, .5, -.1, 0, 0, // 20
		.1, .5, -.1, 1, 0,  // 21
		.1, .5,  .1, 1, 1,  // 22
		-.1, .5,  .1, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,  // bottom

		24,  25,  26,  24,  26,  27,   // front
		28,  29,  30,  28,  30,  31,   // right
		32,  33,  34, 32,  34, 35,  // back
		36, 38, 37, 36, 39, 38,  // left
		40, 42, 41, 40, 43, 42,  // upper
		44, 46, 45, 44, 47, 46,  // bottom
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredBody()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;

	model = glm::translate(model, glm::vec3(stepX, 3, stepZ));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36 * 2, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
#pragma endregion

#pragma region Tail
void Demo::BuildColoredTail() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		.5, -.2, .2, 0, 0,  // 0
		2, -.2, .2, 1, 0,   // 1
		2,  .2, .2, 1, 1,   // 2
		.5,  .2, .2, 0, 1,  // 3

		// right
		2,  .2,  .2, 0, 0,  // 4
		2,  .2, -.2, 1, 0,  // 5
		2, -.2, -.2, 1, 1,  // 6
		2, -.2,  .2, 0, 1,  // 7

		// back
		.5, -.2, -.2, 0, 0, // 8 
		2,  -.2, -.2, 1, 0, // 9
		2,   .2, -.2, 1, 1, // 10
		.5,  .2, -.2, 0, 1, // 11

		// left
		.5, -.2, -.2, 0, 0, // 12
		.5, -.2,  .2, 1, 0, // 13
		.5,  .2,  .2, 1, 1, // 14
		.5,  .2, -.2, 0, 1, // 15

		// upper
		2, .2,  .2, 0, 0,   // 16
		.5, .2,  .2, 1, 0,  // 17
		.5, .2, -.2, 1, 1,  // 18
		2, .2, -.2, 0, 1,   // 19

		// bottom
		.5, -.2, -.2, 0, 0, // 20
		2, -.2, -.2, 1, 0,  // 21
		2, -.2,  .2, 1, 1,  // 22
		.5, -.2,  .2, 0, 1, // 23

		// ------
		// front
		1.75, -.1, .3, 0, 0,  // 0
		1.85, -.1, .3, 1, 0,   // 1
		1.85,  .1, .3, 1, 1,   // 2
		1.75,  .1, .3, 0, 1,  // 3

		// right
		1.85,  .05,  .3, 0, 0,  // 4
		1.85,  .05, .2, 1, 0,  // 5
		1.85, -.05, .2, 1, 1,  // 6
		1.85, -.05,  .3, 0, 1,  // 7

		// back
		1.75, -.05, .2, 0, 0, // 8 
		1.85, -.05, .2, 1, 0, // 9
		1.85, .05, .2, 1, 1, // 10
		1.75, .05, .2, 0, 1, // 11

		// left
		1.75, -.05, .2, 0, 0, // 12
		1.75, -.05,  .3, 1, 0, // 13
		1.75,  .05,  .3, 1, 1, // 14
		1.75,  .05, .2, 0, 1, // 15

		// upper
		1.85, .05,  .3, 0, 0,   // 16
		1.85, .05,  .3, 1, 0,  // 17
		1.85, .05, .2, 1, 1,  // 18
		1.85, .05, .2, 0, 1,   // 19

		// bottom
		1.75, -.05, .2, 0, 0, // 20
		1.85, -.05, .2, 1, 0,  // 21
		1.85, -.05,  .3, 1, 1,  // 22
		1.75, -.05,  .3, 0, 1, // 23
	};

	unsigned int indices[] = {
		// --- BODY ---
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,  // bottom

		24, 25, 26, 24, 26, 27,  // front
		28, 29, 30, 28, 30, 31,  // right
		32, 33, 34, 32, 34, 35,  // back
		36, 38, 37, 36, 39, 38,  // left
		40, 42, 41, 40, 43, 42,  // upper
		44, 46, 45, 44, 47, 46,  // bottom
	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredTail()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(stepX, 3, stepZ));
	
	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36 * 2, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
#pragma endregion

#pragma region 2nd Motor
void Demo::BuildColored2ndMotor() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-.4, -.1, .4, 0, 0,  // 0
		.4, -.1, .4, 1, 0,  // 1
		.4,  .1, .4, 1, 1,  // 2
		-.4,  .1, .4, 0, 1,  // 3

		// right
		.4,  .1,  .4, 0, 0,  // 4
		.4,  .1, .3, 1, 0,  // 5
		.4, -.1, .3, 1, 1,  // 6
		.4, -.1,  .4, 0, 1,  // 7

		// back
		-.4, -.1, .3, 0, 0, // 8 
		.4, -.1, .3, 1, 0, // 9
		.4, .1, .3, 1, 1, // 10
		-.4, .1, .3, 0, 1, // 11

		// left
		-.4, -.1, .3, 0, 0, // 12
		-.4, -.1,  .4, 1, 0, // 13
		-.4,  .1,  .4, 1, 1, // 14
		-.4,  .1, .3, 0, 1, // 15

		// upper
		.4, .1,  .4, 0, 0,   // 16
		.4, .1,  .4, 1, 0,  // 17
		.4, .1, .3, 1, 1,  // 18
		.4, .1, .3, 0, 1,   // 19

		// bottom
		-.4, -.1, .3, 0, 0, // 20
		.4, -.1, .3, 1, 0,  // 21
		.4, -.1, .4, 1, 1,  // 22
		-.4, -.1, .4, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,  // bottom
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColored2ndMotor()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;

	model = glm::translate(model, glm::vec3(5.5 + stepX, 3, stepZ));
	model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
#pragma endregion

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -.5, -50.0,  0,  0,
		 50.0, -.5, -50.0, 50,  0,
		 50.0, -.5,  50.0, 50, 50,
		-50.0, -.5,  50.0,  0, 50,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Kelompok Terserah", 800, 600, false, false);
}