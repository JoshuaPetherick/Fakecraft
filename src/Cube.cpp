
#include "Cube.hpp"

using namespace glm;

 static GLfloat g_colour_buffer_data[12*3*3];  // Colours of ground blocks
 static GLfloat g_colour_buffer_data2[12*3*3]; // Colours for house blocks
 
 // Vertexs for the cube
 static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};
 // Connects the vertexs to make the cube 
 static const GLuint element_buffer[] = {
		0, 1, 2,
		2, 3, 0,
		3, 2, 6,
		6, 7, 3,
		7, 6, 5,
		5, 4, 7,
		4, 5, 1,
		1, 0, 4,
		4, 0, 3,
		3, 7, 4,
		1, 5, 6,
		6, 2, 1
};

void Cube::init()
{
	//Length of buffer
	elementbufferlength = 36; 
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	programID = LoadShaders( "vertex.vs", "fragment.fs" );

	// One color for each vertex. They were generated randomly.
	for(int i = 0; i < 12*3; i++)
	{ 
		g_colour_buffer_data[3*i+0] = 0.039f;
		g_colour_buffer_data[3*i+1] = 0.186f;
		g_colour_buffer_data[3*i+2] = 0.059f;		
	};

	// One color for each vertex. They were generated randomly.
	for(int i = 0; i < 12*3; i++)
	{ 
 		g_colour_buffer_data2[3*i+0] = 0.182f;
		g_colour_buffer_data2[3*i+1] = 0.155f;
		g_colour_buffer_data2[3*i+2] = 0.060f;		
	};

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_buffer), element_buffer, GL_STATIC_DRAW);

	glGenBuffers(1, &colourbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_colour_buffer_data), g_colour_buffer_data, GL_STATIC_DRAW);
	
	glGenBuffers(1, &colourbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_colour_buffer_data2), g_colour_buffer_data2, GL_STATIC_DRAW);	

}

void Cube::setupDraw(GLFWwindow* window)
 {
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	glUseProgram(programID);

	// Runs the void in controls, for keyboard and mouse input
	cameraControls(window);

	// Attribute buffer for vertex
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. This matches the layout in the shaders.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Attribute buffer for ground colours
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
	glVertexAttribPointer(
		1,                                // attribute 1. This matches the layout in the shaders.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
 }

void Cube::drawGround(float xLength, float zLength)
{
	//This does not contain a Y co-ordinate because the ground must all be level
	for(float z = 0; z < (2 * zLength); z += 2) {
	 for(float x = 0; x < (2 * xLength); x += 2) {
		drawCube(x, 0.0f, z);
	 }
	}

}

void Cube::drawHouse(float xLength, float zLength) //5 and 9, 10 and 18
{
	// Disables the previous colour attributes, so it can be refilled with
	// the new colour attributes.
	glDisableVertexAttribArray(1);
	
	// Attribute buffer for house colours
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer2);
	glVertexAttribPointer(
		1,                                // attribute 1. This matches the layout in the shaders.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
		
	for(float y = 2; y <= 6; y += 2) {  // Controls the Y position
	 for(float z = 2; z <= (zLength*2); z+= 2) { //Controls the Z position
	  for(float x = 2; x <= (xLength*2); x += 2) { //Controls the X Position
		if((z == 6 && x == 2) && y < 6) { } //Creates a Doorway

		else if((z >= (zLength/2+3) && z <= (zLength/2+5)) && x == (xLength*2) && y == 4) { } //Creates a 2 cube long window

		else if( y <= 6 ){ drawCube(x, y, z); } 

		if((z >= 4 && z < (zLength*2)) && x != (xLength*2) && y < 6) { x = (xLength*2-2); }  //Creates the gaps between the walls	
	  }
	 }
	}
	
}

void Cube::drawCube(float x, float y, float z)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	mat4 ProjectionMatrix = getProjectionMatrix();
	mat4 ViewMatrix = getViewMatrix();
	mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glDrawElements(GL_TRIANGLES, elementbufferlength, GL_UNSIGNED_INT, (void*)0); // Draw the triangle ! 3 indices starting at 0 -> 1 triangle
}

void Cube::update( GLFWwindow* window )
{
	// Disables the vertex and house colour attribs
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);	

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Cube::close()
{
	// Cleans up by deleting the buffers
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colourbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
} 


