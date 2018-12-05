#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "Utils.h"
#include "MeshModel.h"
#include "RotationRules.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3 * viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, color);
		}
	}
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}

void Renderer::Render(const Scene& scene, const GUIStore& store)
{
	//#############################################
	//## You should override this implementation ##
	//## Here you should render the scene.       ##
	//#############################################

	drawMeshModels(scene, store);
	drawCameraModels(scene);
}

void Renderer::drawChess()  {
	// Draw a chess board in the middle of the screen
	/**/
	for (int i = 100; i < viewportWidth - 100; i++)
	{
		for (int j = 100; j < viewportHeight - 100; j++)
		{
			int mod_i = i / 50;
			int mod_j = j / 50;

			int odd = (mod_i + mod_j) % 2;
			if (odd)
			{
				putPixel(i, j, glm::vec3(0, 1, 0));
			}
			else
			{
				putPixel(i, j, glm::vec3(1, 0, 0));
			}
		}
	}

}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::plotLineHigh(int x1, int y1, int x2, int y2, const glm::vec3& color) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int xi = 1;
	if (deltaX < 0) {
		xi = -1;
		deltaX = -deltaX;
	}
	int d = 2 * deltaX - deltaY;
	int x = x1;
	int y = y1;
	while (y < y2) {
		putPixel(x, y, color);
		if (d > 0) {
			x = x + xi;
			d = d - 2 * deltaY;
		}
		d = d + 2 * deltaX;
		++y;
	}
}

void Renderer::plotLineLow(int x1, int y1, int x2, int y2, const glm::vec3& color) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int yi = 1;
	if (deltaY < 0) {
		yi = -1;
		deltaY = -deltaY;
	}
	int d = 2 * deltaY - deltaX;
	int x = x1;
	int y = y1;
	while (x < x2) {
		putPixel(x, y, color);
		if (d > 0) {
			y = y + yi;
			d = d - 2 * deltaX;
		}
		d = d + 2 * deltaY;
		++x;
	}
}

glm::vec2 Renderer::translatePointIndicesToPixels(const glm::vec4 & _point, const glm::mat4x4 & fullTransform)
{
	glm::vec4 point = fullTransform * _point;
	float wDivision = 1.0f;
	if (point[3] != 0.0f) {
		wDivision = (1 / point[3]);
	}
	float newX = point[0] * wDivision;
	newX = ((newX + 1) * viewportWidth) / 2.0f;
	float newY = point[1] * wDivision;
	newY = ((newY + 1) * viewportHeight) / 2.0f;
	//float newZ = point[2] / wDivision;
	//float newW = point[3] / wDivision;
	return glm::vec2(newX, newY);
}

void Renderer::drawLine(glm::vec2 point1, glm::vec2 point2, const glm::vec3& color) {
	int x1 = (int)point1[0], y1 = (int)point1[1];
	int x2 = (int)point2[0], y2 = (int)point2[1];
	if (std::abs(y2 - y1) < std::abs(x2 - x1)) {
		if (x1 > x2) {
			plotLineLow(x2, y2, x1, y1, color);
		}
		else {
			plotLineLow(x1, y1, x2, y2, color);
		}
	}
	else {
		if (y1 > y2) {
			plotLineHigh(x2, y2, x1, y1, color);
		}
		else {
			plotLineHigh(x1, y1, x2, y2, color);
		}
	}
}


void Renderer::drawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec3& color) 
{
	drawLine(p1, p2, color);
	drawLine(p1, p3, color);
	drawLine(p2, p3, color);
}

void Renderer::drawMeshModels(const Scene& scene, const GUIStore& store) {
	glm::vec3 redColor(1.0f, 0.0f, 0.0f);
	glm::vec3 blueColor(0.0f, 1.0f, 1.0f);
	glm::vec3 greenColor(0.0f, 1.0f, 0.0f);
	glm::vec3 yellowColor(1.0f, 1.0f, 0.0f);
	Camera activeCam = scene.getActiveCamera();

	int index = -1;
	std::vector<std::shared_ptr<MeshModel>> models = scene.getSceneModels();
	for each (std::shared_ptr<MeshModel> model in models)
	{
		std::vector<glm::vec2> textures = model->getTextures();
		std::vector<glm::vec3> normals = model->getNormals();
		std::vector<glm::vec3> vertices = model->getVertices();
		std::vector<Face> faces = model->getFaces();
		++index;
		Utils::Normals whichNormal = store.getModelNormalStatus(index);
		for each (Face face in faces)
		{
			int v1 = face.GetVertexIndex(0);
			float x1 = vertices[v1].x , y1 = vertices[v1].y, z1 = vertices[v1].z;
			int v2 = face.GetVertexIndex(1);
			float x2 = vertices[v2].x , y2 = vertices[v2].y , z2 = vertices[v2].z;
			int v3 = face.GetVertexIndex(2);
			float x3 = vertices[v3].x, y3 = vertices[v3].y , z3 = vertices[v3].z;
			
			glm::vec4 w1 = glm::vec4(x1, y1, z1, 1.0f);
			glm::vec4 w2 = glm::vec4(x2, y2, z2, 1.0f);
			glm::vec4 w3 = glm::vec4(x3, y3, z3, 1.0f);
			
			glm::mat4x4 camTransformation = activeCam.getProjectionTransformation() * activeCam.getViewTransformationInverse();
			glm::mat4x4 modelTransform = model->GetWorldTransformation();
			glm::mat4x4 completeTransform = camTransformation * modelTransform;

			glm::vec2 p1 = translatePointIndicesToPixels(w1, completeTransform);
			glm::vec2 p2 = translatePointIndicesToPixels(w2, completeTransform);
			glm::vec2 p3 = translatePointIndicesToPixels(w3, completeTransform);

			drawTriangle(p1, p2, p3, redColor);

			/* --------------------------------------------------------------------------------------------- */
			/* Normal calculations */

			switch (whichNormal) {
			case Utils::Normals::PerFACE:
			{
				//float normalX1 = (w1.x + w2.x + w3.x)/3.0f, normalY1 = (w1.y + w2.y + w3.y) / 3.0f, normalZ1 = (w1.z + w2.z + w3.z) / 3.0f;
				//glm::vec3 normal1 = glm::vec3(normalX1, normalY1, normalZ1);
				//glm::vec3 a = glm::vec3(w1.x, w1.y, w1.z);
				//glm::vec3 b = glm::vec3(w2.x, w2.y, w2.z);
				//glm::vec3 c = glm::vec3(w3.x, w3.y, w3.z);
				//glm::vec3 endNormal = glm::cross(b - a, c - a);
				//glm::vec4 finish = glm::vec4(normal1.x, normal1.y, normal1.x + endNormal.x, normal1.y)
			}
				break;
			case Utils::Normals::PerVERTEX:
			{
				int normalIndex1 = face.GetNormalIndex(0);
				float normalX1 = normals[normalIndex1].x, normalY1 = normals[normalIndex1].y, normalZ1 = normals[normalIndex1].z;
				int normalIndex2 = face.GetNormalIndex(1);
				float normalX2 = normals[normalIndex2].x, normalY2 = normals[normalIndex2].y, normalZ2 = normals[normalIndex2].z;
				int normalIndex3 = face.GetNormalIndex(2);
				float normalX3 = normals[normalIndex3].x, normalY3 = normals[normalIndex3].y, normalZ3 = normals[normalIndex3].z;

				glm::vec4 normal1 = glm::vec4(normalX1, normalY1, normalZ1, 0.0f) + w1;
				glm::vec4 normal2 = glm::vec4(normalX2, normalY2, normalZ2, 0.0f) + w2;
				glm::vec4 normal3 = glm::vec4(normalX3, normalY3, normalZ3, 0.0f) + w3;

				float len = 0.1f;
				normal1 *= len;
				normal2 *= len;
				normal3 *= len;

				glm::vec2 pixelNormal1 = translatePointIndicesToPixels(normal1, completeTransform);
				glm::vec2 pixelNormal2 = translatePointIndicesToPixels(normal2, completeTransform);
				glm::vec2 pixelNormal3 = translatePointIndicesToPixels(normal3, completeTransform);

				drawNormalForVertex(p1, pixelNormal1, completeTransform, blueColor);
				drawNormalForVertex(p2, pixelNormal2, completeTransform, blueColor);
				drawNormalForVertex(p3, pixelNormal3, completeTransform, blueColor);
			}
				break;
			case Utils::Normals::NONE:
			{
				//do nothing.
			}
				break;
			default:
			{
				//do nothing.
			}
				break;
			}
		}
	}
}


void Renderer::drawCameraModels(const Scene& scene) 
{
	glm::vec3 blackColor(0.0f, 0.0f, 0.0f);
	Camera activeCam = scene.getActiveCamera();
	std::vector<Camera> camList = scene.GetCameras();
	for each(Camera cam in camList) {
		if (scene.GetActiveCameraIndex() != cam.getIndex()) {
			glm::vec4 position = cam.getEyeVector();
			MeshModel model = (Utils::LoadMeshModel("C:\\Users\\Eli\\Desktop\\University\\3 Third Year\\Semester 1\\Computational Graphics\\project-the-hurricane-troupe\\Data\\camera.obj"));
			model.setPosition(position.x, position.y, position.z);
			std::vector<glm::vec2> textures = model.getTextures();
			std::vector<glm::vec3> normals = model.getNormals();
			std::vector<glm::vec3> vertices = model.getVertices();
			std::vector<Face> faces = model.getFaces();
			for each (Face face in faces)
			{
				int v1 = face.GetVertexIndex(0);
				float x1 = vertices[v1].x, y1 = vertices[v1].y, z1 = vertices[v1].z;
				int v2 = face.GetVertexIndex(1);
				float x2 = vertices[v2].x, y2 = vertices[v2].y, z2 = vertices[v2].z;
				int v3 = face.GetVertexIndex(2);
				float x3 = vertices[v3].x, y3 = vertices[v3].y, z3 = vertices[v3].z;

				glm::vec4 w1 = glm::vec4(x1, y1, z1, 1.0f);
				glm::vec4 w2 = glm::vec4(x2, y2, z2, 1.0f);
				glm::vec4 w3 = glm::vec4(x3, y3, z3, 1.0f);

				glm::mat4x4 camTransformation = activeCam.getProjectionTransformation() * activeCam.getViewTransformationInverse();
				glm::mat4x4 modelTransform = model.GetWorldTransformation();
				glm::mat4x4 completeTransform = camTransformation * modelTransform;

				glm::vec2 p1 = translatePointIndicesToPixels(w1, completeTransform);
				glm::vec2 p2 = translatePointIndicesToPixels(w2, completeTransform);
				glm::vec2 p3 = translatePointIndicesToPixels(w3, completeTransform);

				drawTriangle(p1, p2, p3, blackColor);
			}
		}
	}
}

void Renderer::drawNormalForVertex(glm::vec2& point, glm::vec2& normal, glm::mat4x4& endTransform, glm::vec3& color) {
	drawLine(point, normal, color);
}


void drawNormalForFace() {

}
