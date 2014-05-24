#include "Scene.h"
#include "Window.h"
#include "Box.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Spinner.h"
#include "Assets.h"
#include "LightSource.h"
#include "Text.h"

#include "BunnyMesh.h"

namespace Core
{

	Scene::Scene(Core::Window* window)
	{
		Window = window;

		// Load RenderBuffers
		GeometryRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 4, true);
		LightRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 2, false);
		ShadowRB = new ShadowRenderBuffer();
		BufferCombineRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1, false);
		Debug::GLError("ERROR: Could not complete renderbuffers.");
		ShadowRB->Rebuild();

		// Load Shader Programs
		NOAAShader = new Shader("Shaders/fspassthrough.vert", "Shaders/noaa.frag");
		FXAAShader = new Shader("Shaders/fspassthrough.vert", "Shaders/fxaa.frag");
		MeshShader = new Shader("Shaders/mesh.vert", "Shaders/material.frag");
		SphereShader = new Shader("Shaders/mesh.vert", "Shaders/sphere.frag");
		CylinderShader = new Shader("Shaders/mesh.vert", "Shaders/cylinder.frag");
		LightShader = new Shader("Shaders/mesh.vert", "Shaders/light.frag");
		BufferCombineShader = new Shader("Shaders/fspassthrough.vert", "Shaders/combinebuffers.frag");
		FontShader = new Shader("Shaders/font.vert", "Shaders/font.frag");
		Debug::GLError("ERROR: Could not complete shader compilation.");
		
		// Rendering settings
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		Debug::GLError("ERROR: Could not set OpenGL depth testing options");

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		Debug::GLError("ERROR: Could not set OpenGL culling options");

		// Initialize Physics
		PhysicsWorld = new DynamicsWorld();

		// Load standard assets
		Shapes::Box box; 
		Cube = Assets::Meshes["Cube"] = box.GenerateMesh();
		Shapes::Sphere sphere;
		Sphere = Assets::Meshes["Sphere"] = sphere.GenerateMesh();
		Shapes::Cylinder cylinder; 
		Cylinder = Assets::Meshes["Cylinder"] = cylinder.GenerateMesh();

		Assets::LoadStandardAssets();
		fpsText = new Text("FPS: " + std::to_string((int)Time::FPS), 10, Settings::Window::Height - 20, Assets::Fonts["Consolas16"]);

		// Load Scene Objects
		auto c = new Entity();
		c->Transform.Position = glm::vec3(0.0f, 2.0f, -10.0f);
		Camera = new Core::Camera();
		c->AddComponent(Camera);
		Window->Input->SetCameraEntity(c);
		Entities.push_back(c);

		auto e = new Entity();
		e->Transform.Position = glm::vec3(-1.0f, 0.5f, -1.0);
		e->AddComponent(Assets::Meshes["Cube"]);
		e->AddComponent(Assets::Materials["Silver"]);
		auto fb = new RigidBody(PhysicsWorld, Assets::Materials["Silver"], new Shapes::Box);
		e->AddComponent(fb);
		Entities.push_back(e);
		
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 10; k++)
				{
					e = new Entity();
					e->Transform.Position = glm::vec3(2.0f * i, 1.0f + 2.0f * j, 2.0f * k);
					e->AddComponent(Assets::Meshes["Cube"]);
					e->AddComponent(Assets::Materials["Brass"]);
					fb = new RigidBody(PhysicsWorld, Assets::Materials["Brass"], new Shapes::Box);
					e->AddComponent(fb);
					Entities.push_back(e);
				}
			}
		}

		e = new Entity();
		e->Transform.Position = glm::vec3(1.0f, 2.0f, -2.0f);
		e->AddComponent(Assets::Meshes["Sphere"]);
		e->AddComponent(Assets::Materials["Gold"]); 
		fb = new RigidBody(PhysicsWorld, Assets::Materials["Gold"], new Shapes::Sphere);
		e->AddComponent(fb);
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(2.0f, 1.0f, -2.0f);
		e->Transform.Scale = glm::vec3(1.0f, 2.0f, 1.0f);
		e->AddComponent(Assets::Meshes["Cylinder"]);
		e->AddComponent(Assets::Materials["Copper"]);
		fb = new RigidBody(PhysicsWorld, Assets::Materials["Copper"], new Shapes::Cylinder);
		e->AddComponent(fb);
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, -0.5f, 0.0);
		e->Transform.Scale = glm::vec3(100.0f, 1.0f, 100.0f);
		e->AddComponent(Assets::Meshes["Cube"]);
		e->AddComponent(Assets::Materials["Concrete"]);
		fb = new RigidBody(PhysicsWorld, Assets::Materials["Concrete"], new Shapes::Box, RigidBody::Type::STATIC);
		e->AddComponent(fb);
		Entities.push_back(e);


		e = new Entity();
		e->Transform.Position = glm::vec3(25.0f, 10.0f, 25.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 10.0f, 25.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(10.0f, 25.0f, 0.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(25.0f, 10.0f, 0.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 25.0f, 10.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(-2.0f, 1.5f, 0.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);


		e = new Entity();
		e->Transform.Position = glm::vec3(2.0f, 1.0f, 4.0f);
		e->Transform.Scale = glm::vec3(0.5f, 1.8f, 0.5f);
		e->AddComponent(Assets::Meshes["Cube"]);
		e->AddComponent(Assets::Materials["HumanSkin"]);
		fb = new RigidBody(PhysicsWorld, Assets::Materials["HumanSkin"], new Shapes::Box);
		e->AddComponent(fb);
		Window->Input->SetPlayerEntity(e);
		Entities.push_back(e);

		// Soft Box Test
		e = new Entity();
		e->AddComponent(Assets::Materials["Aluminum"]);

		btSoftBody*	psb = btSoftBodyHelpers::CreateFromTriMesh(PhysicsWorld->softBodyWorldInfo, gVerticesBunny,
			&gIndicesBunny[0][0],
			BUNNY_NUM_TRIANGLES);
		btSoftBody::Material*	pm = psb->appendMaterial();
		pm->m_kLST = 0.5;
		pm->m_flags -= btSoftBody::fMaterial::DebugDraw;
		psb->generateBendingConstraints(2, pm);
		psb->m_cfg.piterations = 2;
		psb->m_cfg.kDF = 0.5;
		psb->randomizeConstraints();
		psb->scale(btVector3(2, 2, 2));
		psb->translate(btVector3(-3, 2, -3));
		psb->setTotalMass(100, true);

		SoftBody* sb = new SoftBody(PhysicsWorld, Assets::Materials["Aluminum"], psb);
		e->AddComponent(sb);
		Entities.push_back(e);


		// Must be after camera is created
		ResizeRenderBuffers();

		for (auto e : Entities)
			e->Load();
	}


	Scene::~Scene()
	{
		delete GeometryRB;
		delete LightRB;
		delete ShadowRB;
		delete BufferCombineRB; 

		delete NOAAShader;
		delete FXAAShader;
		delete MeshShader;
		delete SphereShader;
		delete CylinderShader;
		delete LightShader;
		delete BufferCombineShader;
		delete FontShader;

		for (auto e : Entities)
		{
			delete e;
		}

		Assets::Clear();
	}


	void Scene::Update()
	{
		// Order of Operations:
		// 1. Update all entities
		// 2. Tick Physics
		// 3. Render Scene

		for (auto e : Entities)
			e->Update();
		
		PhysicsWorld->Update();

		P = Camera->GetProjectionMatrix();
		V = Camera->GetViewMatrix();

		RenderGeometry();
		RenderLight();
		RenderPost();
	}
	

	void Scene::RenderGeometry()
	{
		GeometryRB->MakeCurrent();
		GeometryRB->Clear();

		for (auto e : Entities)
		{
			if (e->IsRenderable()) {

				glm::mat4 M = e->Transform.ToMatrix();
				glm::mat4 MV = V * M;
				glm::mat4 MVP = P * MV;

				auto r = e->GetRenderable();
				if (r == Sphere)
				{
					SphereShader->MakeCurrent();

					glUniformMatrix4fv(SphereShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix4fv(SphereShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(SphereShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));
					glUniform4fv(SphereShader->GetUL("DiffuseColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->DiffuseColor * e->GetComponent<Material>()->DiffuseIntensity, 1.0f)));
					glUniform4fv(SphereShader->GetUL("SpecularColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->SpecularColor * e->GetComponent<Material>()->SpecularIntensity, 1.0f)));
					glUniform3fv(SphereShader->GetUL("SpherePosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
					glUniform1f(SphereShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);
					glUniform1f(SphereShader->GetUL("SpecularHardness"), e->GetComponent<Material>()->SpecularHardness);

					r->EnableBuffers();
					r->Render();
					r->DisableBuffers();
				}
				else if (r == Cylinder)
				{
					CylinderShader->MakeCurrent();

					glUniformMatrix4fv(CylinderShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix4fv(CylinderShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(CylinderShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));
					glUniform4fv(CylinderShader->GetUL("DiffuseColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->DiffuseColor * e->GetComponent<Material>()->DiffuseIntensity, 1.0f)));
					glUniform4fv(CylinderShader->GetUL("SpecularColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->SpecularColor * e->GetComponent<Material>()->SpecularIntensity, 1.0f)));
					glUniform3fv(CylinderShader->GetUL("Direction"), 1, glm::value_ptr(glm::normalize(glm::vec3(MV * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)))));
					glUniform3fv(CylinderShader->GetUL("Center"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
					glUniform1f(CylinderShader->GetUL("Length"), r->Entity->Transform.Scale.y);
					glUniform1f(CylinderShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);
					glUniform1f(CylinderShader->GetUL("SpecularHardness"), e->GetComponent<Material>()->SpecularHardness);

					r->EnableBuffers();
					r->Render();
					r->DisableBuffers();
				}
				else
				{
					MeshShader->MakeCurrent();

					glUniformMatrix4fv(MeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix4fv(MeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(MeshShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));
					glUniform4fv(MeshShader->GetUL("DiffuseColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->DiffuseColor * e->GetComponent<Material>()->DiffuseIntensity, 1.0f)));
					glUniform4fv(MeshShader->GetUL("SpecularColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->SpecularColor * e->GetComponent<Material>()->SpecularIntensity, 1.0f)));
					glUniform1f(MeshShader->GetUL("SpecularHardness"), e->GetComponent<Material>()->SpecularHardness);

					r->EnableBuffers();
					r->Render();
					r->DisableBuffers();
				}
			}
		}
		
	}


	void Scene::RenderLight()
	{
		LightRB->MakeCurrent();
		LightRB->Clear();

		LightShader->MakeCurrent();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetDepthTexture());
		glUniform1i(LightShader->GetUL("DepthTexture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(1)); 
		glUniform1i(LightShader->GetUL("NormalTexture"), 1);
		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(3));
		glUniform1i(LightShader->GetUL("HardnessTexture"), 2);

		glUniform2f(LightShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(LightShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(glm::inverse(P)));

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);	// Additive Blending
		glCullFace(GL_FRONT);

		for (auto e : Entities)
		{
			auto r = e->GetComponent<LightSource>();
			if (r != nullptr)
			{
				glm::mat4 M = e->Transform.ToMatrix();
				glm::mat4 MV = V * M;
				glm::mat4 MVP = P * MV;
				
				glUniformMatrix4fv(LightShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(LightShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniform4fv(LightShader->GetUL("LightColor"), 1, glm::value_ptr(glm::vec4(r->Color, 1.0f)));
				glUniform3fv(LightShader->GetUL("LightPosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0))));
				float lightRadius = e->Transform.Scale.x / 2;
				glUniform1f(LightShader->GetUL("LightRadius"), lightRadius);

				Cube->EnableBuffers();
				Cube->Render();
				Cube->DisableBuffers();
			}
		}

		glCullFace(GL_BACK);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Reset Blending
	}


	void Scene::RenderPost()
	{
		// Combine render buffer output into final texture
		BufferCombineRB->MakeCurrent();
		BufferCombineRB->Clear();

		BufferCombineShader->MakeCurrent();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(0));
		glUniform1i(BufferCombineShader->GetUL("DiffuseTexture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(2));
		glUniform1i(BufferCombineShader->GetUL("SpecularTexture"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(3));
		glUniform1i(BufferCombineShader->GetUL("SpecularHardnessTexture"), 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, LightRB->GetOutputTexture(0));
		glUniform1i(BufferCombineShader->GetUL("DiffuseLightTexture"), 3);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, LightRB->GetOutputTexture(1));
		glUniform1i(BufferCombineShader->GetUL("SpecularLightTexture"), 4);

		glUniform3fv(BufferCombineShader->GetUL("AmbientLight"), 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));

		SQuad.Render();

		// Perform antialiasing pass(es)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (Settings::Video::FXAA > 0)
		{
			FXAAShader->MakeCurrent();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, BufferCombineRB->GetOutputTexture(0));
			glUniform1i(FXAAShader->GetUL("sourceTexture"), 0);
			glUniform2f(FXAAShader->GetUL("frameSize"), (GLfloat)Settings::Window::Width, (GLfloat)Settings::Window::Height);
			SQuad.Render();
		}
		else {
			NOAAShader->MakeCurrent();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, BufferCombineRB->GetOutputTexture(0));
			glUniform1i(NOAAShader->GetUL("sourceTexture"), 0);
			SQuad.Render();
		}

		RenderUI();

		glUseProgram(0);
	}

	
	void Scene::RenderUI()
	{
		glViewport(0, 0, Settings::Window::Width, Settings::Window::Height);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_DEPTH_TEST);

		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);
		//glDisable(GL_ALPHA_TEST);

		if (Settings::Video::ShowFPS)
		{
			FontShader->MakeCurrent();
			fpsText->UpdateText("FPS: " + std::to_string((int)Time::FPS));
			fpsText->Render(FontShader);
		}

		//glDisable(GL_BLEND);
		//glEnable(GL_ALPHA_TEST);
		glEnable(GL_DEPTH_TEST);
		Debug::GLError("ERROR: failed to render ui.");
	}


	void Scene::ResizeRenderBuffers()
	{
		GeometryRB->Rebuild();
		LightRB->Rebuild();
		BufferCombineRB->Rebuild(); 

		for (auto e : Entities)
		{
			auto c = e->GetComponent<Core::Camera>();
			if (c != nullptr)
			{
				c->UpdateProjection();
			}
		}
	}
	

	void Scene::SetActiveCamera(Core::Camera* camera)
	{
		Camera = camera;
	}

}