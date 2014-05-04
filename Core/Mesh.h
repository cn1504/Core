#pragma once
#include "Core.h"
#include "Renderable.h"

namespace Core
{

	class Mesh
		: public Renderable
	{
	protected:
		GLsizei IndexSize;
		GLuint VertexBuffer;
		GLuint NormalBuffer;
		GLuint ElementBuffer;

	public:
		Mesh(std::vector<unsigned short> indices, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> indexed_normals);
		Mesh(Mesh* other);
		virtual ~Mesh();
		virtual void Load();
		virtual void Update();

		virtual void EnableBuffers();
		virtual void Render();
		virtual void DisableBuffers();
	};

}

