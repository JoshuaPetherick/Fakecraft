
#include "Cube.hpp"

using namespace glm;

 GLuint VertexArrayID;
 GLuint programID;
 GLuint MatrixID;
 GLuint vertexbuffer;
 GLuint colourbuffer;
 GLuint colourbuffer2;

 static GLfloat g_colour_buffer_data[12*3*3];  // Colours of ground blocks
 static GLfloat g_colour_buffer_data2[12*3*3]; // Colours for house blocks
 static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

void Cube::init()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	programID = LoadShaders( "vertex.vs", "fragment.fs" );

	// One color for each vertex. They were generated randomly.
	for(int i = 0; i < 12*3; i++)
	{ 
		if(i >= 27 && i <= 32) //Co-ordinates for the top of the triangle
		{
			g_colour_buffer_data[3*i+0] = 0.039f;
			g_colour_buffer_data[3*i+1] = 0.186f;
			g_colour_buffer_data[3*i+2] = 0.059f;
		}
		else	
		{
 			g_colour_buffer_data[3*i+0] = 0.140f;
			g_colour_buffer_data[3*i+1] = 0.059f;
			g_colour_buffer_data[3*i+2] = 0.038f;
		}		
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

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs(window);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colours
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
 }

void Cube::drawGround(float xLength, float zLength)
{
	for(float z = 0; z < (2 * zLength); z += 2) {
	for(float x = 0; x < (2 * xLength); x += 2) {
		drawCube(x, 0.0f, z);
	}
	}

}

void Cube::drawHouse(float xLength, float zLength) //5 and 9, 10 and 18
{
	glDisableVertexAttribArray(1);
	
	// 2nd attribute buffer : colours
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer2);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 2, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
		
	for(float y = 2; y <= 10; y += 2) {
	for(float z = 2; z <= (zLength*2); z+= 2) {
	for(float x = 2; x <= (xLength*2); x += 2) {
		if((z == 6 && x == 2) && y < 6) { } //Creates a Doorway
		else if((z >= (zLength/2+3) && z <= (zLength/2+5)) && x == (xLength*2) && y == 4) { } //Creates a 2 cube long window
		else if( y > 6 && x == (xLength/2+1.5) && z == (zLength/2-0.5) ) { drawCube (x, y, z); } //Creates a Chimney
		else if( y <= 6 ){ drawCube(x, y, z); } 
		if((z >= 4 && z < (zLength*2)) && x != (xLength*2) && y < 6) { x = (xLength*2-2); }  //Creates the gaps between the walls	
	}
	}
	}
	
}

void Cube::drawCube(float x, float y, float z)
{
	mat4 ProjectionMatrix = getProjectionMatrix();
	mat4 ViewMatrix = getViewMatrix();
	mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // Draw the triangle ! 3 indices starting at 0 -> 1 triangle
}

void Cube::update( GLFWwindow* window )
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);	

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Cube::close()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colourbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
} 


