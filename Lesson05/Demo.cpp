#include "Demo.h"
#include <vector>



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();

	BuildPointBezier();
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
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}



}

void Demo::Update(double deltaTime) {
	angle += (float) ((deltaTime * 12.0f) / 1000);
	/* if (forward) {
		translateX += (float)((deltaTime * 5.0f) / 1000);
		if (translateX >= 20) {
			forward = false;
		}
	}
	else {
		translateX -= (float)((deltaTime * 5.0f) / 1000);
		if (translateX <= 0) {
			forward = true;
		}
	}
	if (up) {
		translateY += (float)((deltaTime * 5.0f) / 1000);
		if (translateY >= 20) {
			up = false;
		}
	}
	else {
		translateY -= (float)((deltaTime * 5.0f) / 1000);
		if (translateY <= 0) {
			up = true;
		}
	}
	*/
	translateX = xCalculated[indexNow];
	translateZ = zCalculated[indexNow];
	
	// Jumlah SegmentCount
	indexNow++;
	if (indexNow == xCalculated.size()) {
		indexNow = 0;
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
	glm::mat4 view = glm::lookAt(glm::vec3(0, 40, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredCube(0, 0, 3, 0, 6, 3, 3);

	//rangka atas
	DrawColoredCube(0, 0, 5, 0, 1, 3, 1);
	
	//propeler atas 1
	DrawColoredCube(1, 0, 6.75, 0, 10, 0.5, 0.5);

	//propeler atas 2
	DrawColoredCube(1, 0, 6.75, 0, 0.5, 0.5, 10);
	
	//rangka belakang
	DrawColoredCube(0, 3, 3, 0, 12, 1.5, 1.5);
	
	//propeler belakang 1
	DrawColoredCube(2, 7.75, 2.75, 0.75, 3.5, 0.5, 0.5);

	//propeler belakang 2
	DrawColoredCube(2, 7.75, 2.75, 0.75, 0.5, 3.5, 0.5);
	
	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildPointBezier() {

	float xPoints[12] = 
	{
		5.0f, 10.0f, 10.0f, 
		10.0f, 10.0f, 5.0f, 
		-5.0f, -10.0f, -10.0f, 
		-10.0f, -10.0f, -5.0f,
	};

	float zPoints[12] = 
	{
		10.0f, 10.0f, 5.0f,
		-5.0f, -10.0f, -10.0f,
		-10.0f, -10.0, -5.0f,
		5.0f, 10.0f, 10.0f,
	};
	
	
	float t = 0.0;
	int xChecker = 0;
	int zChecker = 0;

	for (int j = 0; j < 12; j = j + 3) {
		for (int i = 0; i <= segmentPoint; i++) {
			t = (float)i / segmentPoint;
			float x = ((1 - t) * (1 - t) * xPoints[j]) + (2 * (1 - t) * t * xPoints[j + 1]) + (t * t * xPoints[j + 2]);
			xCalculated.push_back(x);
			float z = ((1 - t) * (1 - t) * zPoints[j]) + (2 * (1 - t) * t * zPoints[j + 1]) + (t * t * zPoints[j + 2]);
			zCalculated.push_back(z);

			// std::cout << "\nx : " << xCalculated[i] << "\n";
			// std::cout << "\nz : " << zCalculated[i] << "\n";
		}

		float destinationX = xPoints[j + 3];
		float destinationZ = zPoints[j + 3];
		if (j == 9) {
			destinationX = xPoints[0];
			destinationZ = zPoints[0];
		}
		
		float distanceX = destinationX - xPoints[j + 2];
		std::cout << "\ndestinationX : " << destinationX << "\n";
		std::cout << "\ndistanceX : " << distanceX << "\n";
		float distanceZ = destinationZ - zPoints[j + 2];
		std::cout << "\ndestinationZ : " << destinationZ << "\n";
		std::cout << "\ndistanceZ : " << distanceZ << "\n";

		int totalPointDistance = 0;
		if (distanceX == 0) {
			std::cout << "xChangeLurus: " << "HItung Z" << "\n";
			totalPointDistance = abs(round(distanceZ * 2));
			std::cout << "\ntotalPointDistanceZ : " << totalPointDistance << "\n";
			for (int i = 0; true; i++) {
				float tLurus = (float) i / totalPointDistance;
				// std::cout << "\ntLurus : " << tLurus << "\n";
				xCalculated.push_back(xPoints[j + 2]);

				float z = 0;
				int masukZ = 0;
				if (distanceZ > 0) {
					masukZ++;
					// std::cout << "masukZLurus: " << masukZ << "\n";
					// std::cout << "distanceZ: " << distanceZ << "\n";
					// std::cout << "destinationZ: " << destinationZ << "\n";
					z = zPoints[j + 2] + tLurus;
					zCalculated.push_back(z);
					std::cout << "zCalculated+1: " << zCalculated[i + 1] << "\n";
					zChecker++;
					if (zCalculated[i + 1] > destinationZ) {
						// std::cout << "break? " << true << "\n";
						// bool isBreak = zCalculated[i + 1] < destinationZ;
						// std::cout << "destinationZ: " << destinationZ << "\n";
						// std::cout << isBreak << "\n";
						break;
					}
				}
				else {
					z = zPoints[j + 2] - tLurus;
					zCalculated.push_back(z);
					zChecker--;
					if (zCalculated[i+1] < destinationZ) break;
				}
			}
		} else {
			std::cout << "xChangeLurus: " << "HItung X" << "\n";
			totalPointDistance = abs(round(distanceX * 2));
			std::cout << "\ntotalPointDistanceX : " << totalPointDistance << "\n";
			for (int i = 0; true; i++) {
				float tLurus = (float) i / totalPointDistance;
				// std::cout << "\ntLurus : " << tLurus << "\n";
				zCalculated.push_back(zPoints[j + 2]);
				
				float x = 0;
				if (distanceX > 0) {
					x = xPoints[j + 2] + tLurus;
					xCalculated.push_back(x);
					xChecker++;
					if (xCalculated[i+1] > destinationX) break;
				}
				else {
					x = xPoints[j + 2] - tLurus;
					xCalculated.push_back(x);
					xChecker--;
					if (xCalculated[i+1] < destinationX) break;
				}
			}
		}
	}

	for (int i = 0; i < xCalculated.size(); i++) {
		// std::cout << "xCaluclated: " << xCalculated[i] << "\n";
		// std::cout << "zCaluclated: " << zCalculated[i] << "\n";
	}

	std::cout << "xChangeLurus: " << xChecker << "\n";
	std::cout << "zChangeLurus: " << zChecker << "\n";

}

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
		-0.5, -0.5, 0.5, 0, 0,  // 0
		0.5, -0.5, 0.5, 1, 0,   // 1
		0.5,  0.5, 0.5, 1, 1,   // 2
		-0.5,  0.5, 0.5, 0, 1,  // 3

		// right
		0.5,  0.5,  0.5, 0, 0,  // 4
		0.5,  0.5, -0.5, 1, 0,  // 5
		0.5, -0.5, -0.5, 1, 1,  // 6
		0.5, -0.5,  0.5, 0, 1,  // 7

		// back
		-0.5, -0.5, -0.5, 0, 0, // 8 
		0.5,  -0.5, -0.5, 1, 0, // 9
		0.5,   0.5, -0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// left
		-0.5, -0.5, -0.5, 0, 0, // 12
		-0.5, -0.5,  0.5, 1, 0, // 13
		-0.5,  0.5,  0.5, 1, 1, // 14
		-0.5,  0.5, -0.5, 0, 1, // 15

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 16
		-0.5, 0.5,  0.5, 1, 0,  // 17
		-0.5, 0.5, -0.5, 1, 1,  // 18
		0.5, 0.5, -0.5, 0, 1,   // 19

		// bottom
		-0.5, -0.5, -0.5, 0, 0, // 20
		0.5, -0.5, -0.5, 1, 0,  // 21
		0.5, -0.5,  0.5, 1, 1,  // 22
		-0.5, -0.5,  0.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
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

void Demo::DrawColoredCube(
	int rotation,
	double xPos,
	double yPos,
	double zPos,
	double xScale,
	double yScale,
	double zScale
)
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(xPos - translateX , yPos, zPos + translateZ));

	if (rotation == 1) {
		model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	}
	else if (rotation == 2) {
		model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
	}

	model = glm::scale(model, glm::vec3(xScale, yScale, zScale));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

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
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,


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
	app.Start("Transformation: Transform Cube", 1200, 720, true, false);
}