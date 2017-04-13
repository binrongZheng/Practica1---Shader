//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "myShader.h"
#include <SOIL.h>

using namespace std;
const GLint WIDTH = 800, HEIGHT = 800;
bool WIREFRAME = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void DrawVAO();

int main() {
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al inicializar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	//set function when callback
	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	//fondo	
	
	//cargamos los shader
	Shader myShader ("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	
	GLfloat VertexBufferObject[] = {
		-0.5f,  0.5f, 0.0f, 0.0f,1.0f,1.0f,   // Top Left - color
		0.5f,  0.5f, 0.0f,	0.0f,1.0f,1.0f, // Top Right - color
		-0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,  // Bottom Left - color
		0.5f, -0.5f, 0.0f,	0.0f,1.0f,0.0f  // Bottom Right - color
		
		
	};

	GLuint FragmentBufferObject[] = {
		0,1,2,
		1,2,3
	};
	
	// Crear los VBO, VAO y EBO	//Declarar el VBO y el EBO	
	GLuint EBO, VBO, VAO;
	//reservar memoria para el VAO, VBO y EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	//Establecer el objeto
	glBindVertexArray(VAO);{
		//Enlazar el buffer con openGL
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FragmentBufferObject), FragmentBufferObject, GL_STATIC_DRAW);

		//Establecer las propiedades de los vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0); 
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//liberar el buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	} 
	//liberar el buffer de vertices
	glBindVertexArray(0);

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{

		//Camara
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10.f, 10.f, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0, 1.0, 1.0, 1.0);

		//establecer el shader
		myShader.USE();

		//Ejercicio2
		GLfloat timeValue = glfwGetTime(); 
		GLfloat valor = abs((sin(timeValue)/2));//-0.5   -  0.5 
		
		GLint variableShader = glGetUniformLocation(myShader.Program, "offset");
		glUniform1f(variableShader, valor);
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5; // SHADER 0.5-1.0
		GLint vertexColorLocation = glGetUniformLocation(myShader.Program, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//pintar el VAO
		glBindVertexArray(VAO);
		DrawVAO();
		glBindVertexArray(0);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	exit(EXIT_SUCCESS);
}


void DrawVAO() {
	
	if (WIREFRAME) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		WIREFRAME = !WIREFRAME;
}
