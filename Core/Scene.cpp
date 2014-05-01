#include "Scene.h"
#include "Window.h"
#include "Box.h"
#include "Sphere.h"
#include "Spinner.h"
#include "Assets.h"

namespace Core
{

	Scene::Scene()
	{
		GeometryRB = new RenderBuffer(glm::vec3(0.0f), 2, true);
		Debug::GLError("ERROR: Could not complete geometry renderbuffer.");

		MeshShader = new Shader("Shaders/mesh.vert", "Shaders/material.frag");
		FXAAShader = new Shader("Shaders/fspassthrough.vert", "Shaders/fxaa.frag");
		SphereShader = new Shader("Shaders/mesh.vert", "Shaders/sphere.frag");
		Debug::GLError("ERROR: Could not complete shader compilation.");


		auto c = new Entity();
		c->Transform.Position = glm::vec3(0.0f, 2.0f, -10.0f);
		Camera = new Core::Camera();
		c->AddComponent(Camera);
		Entities.push_back(c);


		auto e = new Entity();
		Shapes::Box box(0.5f);
		e->AddComponent(box.GenerateMesh());
		e->AddComponent(new Material());
		e->AddComponent(new Test::Spinner());
		Entities.push_back(e);
		
		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 2.0f, -1.0f);
		e->AddComponent(box.GenerateMesh());
		e->AddComponent(new Material());
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 2.0f, -2.0f);
		Shapes::Sphere sphere;
		e->AddComponent(sphere.GenerateMesh());
		e->AddComponent(new Material());
		e->GetComponent<Material>()->DiffuseColor = glm::vec3(0.0f, 0.5f, 1.0f);
		Entities.push_back(e);

		// Must be after camera is created
		ResizeRenderBuffers();

		for (auto e : Entities)
			e->Load();
	}


	Scene::~Scene()
	{
		delete GeometryRB;
		delete MeshShader;
		delete FXAAShader;

		for (auto e : Entities)
		{
			delete e;
		}
	}


	void Scene::Update()
	{
		// Update all entities
		for (auto e : Entities)
			e->Update();

		GeometryRB->MakeCurrent();
		GeometryRB->Clear();
		MeshShader->MakeCurrent();

		glm::mat4 P = Camera->GetProjectionMatrix();
		glm::mat4 V = Camera->GetViewMatrix();
		for (auto e : Entities)
		{
			if (e->IsRenderable()) {
				glm::mat4 M = e->Transform.ToMatrix();
				glm::mat4 MV = V * M;
				glm::mat4 MVP = P * MV;

				auto r = e->GetRenderable();
				if (r == Assets::Meshes["UnitSphere"])
				{
					SphereShader->MakeCurrent();

					glUniformMatrix4fv(SphereShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP)); 
					glUniformMatrix4fv(SphereShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
					glUniform4fv(SphereShader->GetUL("DiffuseColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->DiffuseColor, 1.0f)));
					glUniform3fv(SphereShader->GetUL("SpherePosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
					glUniform1f(SphereShader->GetUL("Radius"), 0.5f);

					r->EnableBuffers();
					r->Render();
					r->DisableBuffers();

					MeshShader->MakeCurrent();
				}
				else
				{
					glUniformMatrix4fv(MeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix4fv(MeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
					glUniform4fv(MeshShader->GetUL("DiffuseColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->DiffuseColor, 1.0f)));

					r->EnableBuffers();
					r->Render();
					r->DisableBuffers();
				}
			}
		}

		// Perform final pass to back buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		FXAAShader->MakeCurrent();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(0));
		glUniform1i(FXAAShader->GetUL("sourceTexture"), 0);
		glUniform2f(FXAAShader->GetUL("frameSize"), (GLfloat)Settings::Window::Width, (GLfloat)Settings::Window::Height);
		SQuad.Render();

		glUseProgram(0);
	}


	void Scene::ResizeRenderBuffers()
	{
		GeometryRB->Rebuild();
		Camera->UpdateProjection();
	}

}