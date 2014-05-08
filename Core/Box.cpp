#include "Box.h"
#include "Assets.h"

namespace Core
{
	namespace Shapes
	{
		
		Box::Box()
		{
		}


		Box::~Box()
		{
		}


		Mesh* Box::GenerateMesh()
		{
			glm::vec3 HalfLengths(0.5, 0.5, 0.5);

			std::vector<unsigned short> indices;
			std::vector<glm::vec3> indexed_vertices;
			std::vector<glm::vec3> indexed_normals;

			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, HalfLengths.y, -HalfLengths.z)); // 2
			indexed_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, HalfLengths.y, HalfLengths.z)); // 5
			indexed_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			indexed_vertices.push_back(glm::vec3(HalfLengths.x, HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, HalfLengths.y, HalfLengths.z)); // 8
			indexed_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			indexed_vertices.push_back(glm::vec3(HalfLengths.x, HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, HalfLengths.y, -HalfLengths.z)); // 11
			indexed_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, -HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, -HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, -HalfLengths.y, -HalfLengths.z)); // 14
			indexed_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, -HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, -HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(-HalfLengths.x, -HalfLengths.y, HalfLengths.z)); // 17
			indexed_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			indexed_vertices.push_back(glm::vec3(HalfLengths.x, -HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, -HalfLengths.y, HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, -HalfLengths.y, HalfLengths.z)); // 20
			indexed_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

			indexed_vertices.push_back(glm::vec3(HalfLengths.x, -HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, -HalfLengths.y, -HalfLengths.z));
			indexed_vertices.push_back(glm::vec3(HalfLengths.x, -HalfLengths.y, -HalfLengths.z)); // 23
			indexed_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
			indexed_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

			// Face 1 - Y+
			indices.push_back(4);
			indices.push_back(7);
			indices.push_back(1);
			indices.push_back(1);
			indices.push_back(7);
			indices.push_back(10);

			// Face 2 - Y-
			indices.push_back(16);
			indices.push_back(13);
			indices.push_back(19);
			indices.push_back(19);
			indices.push_back(13);
			indices.push_back(22);

			// Face 3 - Z-
			indices.push_back(2);
			indices.push_back(11);
			indices.push_back(14);
			indices.push_back(14);
			indices.push_back(11);
			indices.push_back(23);

			// Face 4 - X-
			indices.push_back(15);
			indices.push_back(3);
			indices.push_back(12);
			indices.push_back(12);
			indices.push_back(3);
			indices.push_back(0);

			// Face 5 - Z+
			indices.push_back(5);
			indices.push_back(17);
			indices.push_back(8);
			indices.push_back(8);
			indices.push_back(17);
			indices.push_back(20);

			// Face 6 - X+
			indices.push_back(6);
			indices.push_back(18);
			indices.push_back(9);
			indices.push_back(9);
			indices.push_back(18);
			indices.push_back(21);

			return new Mesh(indices, indexed_vertices, indexed_normals);
		}


		float Box::CalculateVolume(glm::vec3 scale)
		{
			return 8.0f * scale.x * scale.y * scale.z;
		}


		glm::vec3 Box::CalculateCenterOfMass(glm::vec3 scale)
		{
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}

		
		glm::vec3 Box::CalculateInertia(glm::vec3 scale)
		{
			return (1.0f / 12.0f) * glm::vec3(scale.y*scale.y + scale.z*scale.z, scale.x*scale.x + scale.z*scale.z, scale.x*scale.x + scale.y*scale.y);
		}

	}
}
