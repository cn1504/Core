#include "Mesh.h"
#include "Assets.h"

namespace Core
{

	Mesh::Mesh(std::vector<unsigned short> indices, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> indexed_normals)
	{
		IndexSize = (GLsizei)indices.size();
		
		// Load vectors into VBOs
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");

		// Generate a buffer for the indices as well
		glGenBuffers(1, &ElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Indices buffer object.");
	}


	Mesh::Mesh(Mesh* other)
	{
		VertexBuffer = other->VertexBuffer;
		NormalBuffer = other->NormalBuffer;
		ElementBuffer = other->ElementBuffer;
	}


	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &VertexBuffer);
		glDeleteBuffers(1, &NormalBuffer);
		glDeleteBuffers(1, &ElementBuffer);
	}

	
	void Mesh::EnableBuffers()
	{
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : normals
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glVertexAttribPointer(
			1,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_TRUE,            // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		Debug::GLError("ERROR: Could not bind the mesh buffers.");
	}


	void Mesh::Render()
	{
		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,               // mode
			IndexSize,                  // count
			GL_UNSIGNED_SHORT,          // type
			(void*)0                    // element array buffer offset
			);
		Debug::GLError("ERROR: Could not draw the elements.");
	}

	
	void Mesh::DisableBuffers()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		Debug::GLError("ERROR: Could not disable the mesh buffers.");
	}


	void Mesh::Load()
	{
	}


	void Mesh::Update()
	{
	}

}