#include "Scene.h"
#include "Window.h"
#include "Box.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Spinner.h"
#include "Assets.h"
#include "LightSource.h"
#include "Text.h"
#include "AudioListener.h"

#include "BunnyMesh.h"

namespace Core
{

	Scene::Scene(Core::Window* window)
	{
		Window = window;

		// Load RenderBuffers
		GeometryRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 5, true);
		LightRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 2, false);
		ShadowRB = new ShadowRenderBuffer();
		GlowMapHorizontalRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1, false);
		GlowMapVerticalRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1, false);
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
		LightWithShadowShader = new Shader("Shaders/mesh.vert", "Shaders/lightwshadow.frag");
		BufferCombineShader = new Shader("Shaders/fspassthrough.vert", "Shaders/combinebuffers.frag");
		FontShader = new Shader("Shaders/font.vert", "Shaders/font.frag");
		ShadowMeshShader = new Shader("Shaders/mesh.vert", "Shaders/shadow.frag");
		ShadowSphereShader = new Shader("Shaders/mesh.vert", "Shaders/shadowsphere.frag");
		ShadowCylinderShader = new Shader("Shaders/mesh.vert", "Shaders/shadowcylinder.frag");
		BlurShader = new Shader("Shaders/fspassthrough.vert", "Shaders/blur.frag");
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
		consoleText = new Text("", 10, 10, Assets::Fonts["Consolas16"]);

		
		LoadScene("");


		// Must be after camera is created
		ResizeRenderBuffers();

		for (auto e : Entities)
			e->Load();

		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("Done loading scene:");
			Debug::Log("\t" + std::to_string(Entities.size()) + " entities.");
			Debug::Log("\t" + std::to_string(Lights.size()) + " lights.");
		}
	}


	Scene::~Scene()
	{
		delete GeometryRB;
		delete LightRB;
		delete ShadowRB;
		delete GlowMapHorizontalRB;
		delete GlowMapVerticalRB;
		delete BufferCombineRB; 

		delete NOAAShader;
		delete FXAAShader;
		delete MeshShader;
		delete SphereShader;
		delete CylinderShader;
		delete LightShader;
		delete LightWithShadowShader;
		delete BufferCombineShader;
		delete FontShader;
		delete ShadowMeshShader;
		delete ShadowSphereShader;
		delete ShadowCylinderShader;
		delete BlurShader;

		for (auto e : Entities)
		{
			delete e;
		}

		Assets::Clear();

		delete fpsText;
		delete consoleText;
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

		AudioListener.Update();

		P = Camera->GetProjectionMatrix();
		V = Camera->GetViewMatrix();
		
		RenderShadows();
		RenderGeometry();
		RenderLight();
		RenderBloom();
		RenderPost();
		RenderUI();
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
					glUniform4fv(SphereShader->GetUL("EmanativeColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->EmanativeColor * e->GetComponent<Material>()->EmanativeIntensity, 1.0f)));

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
					glUniform4fv(CylinderShader->GetUL("EmanativeColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->EmanativeColor * e->GetComponent<Material>()->EmanativeIntensity, 1.0f)));

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
					glUniform4fv(MeshShader->GetUL("EmanativeColor"), 1, glm::value_ptr(glm::vec4(e->GetComponent<Material>()->EmanativeColor * e->GetComponent<Material>()->EmanativeIntensity, 1.0f)));

					r->EnableBuffers();
					r->Render();
					r->DisableBuffers();
				}
			}
		}
		
	}


	void Scene::RenderShadows()
	{
		ShadowRB->MakeCurrent();		
		
		//glCullFace(GL_FRONT);

		for (auto l : Lights)
		{
			if (l->CastsShadow())
			{
				const glm::vec3 xp(1.0f, 0.0f, 0.0f);
				const glm::vec3 xn(-1.0f, 0.0f, 0.0f);
				const glm::vec3 yp(0.0f, 1.0f, 0.0f);
				const glm::vec3 yn(0.0f, -1.0f, 0.0f);
				const glm::vec3 zp(0.0f, 0.0f, 1.0f);
				const glm::vec3 zn(0.0f, 0.0f, -1.0f);

				glm::mat4 lvm[6] = { 
					glm::lookAt(l->Entity->Transform.Position, l->Entity->Transform.Position + xp, yn),
					glm::lookAt(l->Entity->Transform.Position, l->Entity->Transform.Position + xn, yn),
					glm::lookAt(l->Entity->Transform.Position, l->Entity->Transform.Position + yp, zp),
					glm::lookAt(l->Entity->Transform.Position, l->Entity->Transform.Position + yn, zn),
					glm::lookAt(l->Entity->Transform.Position, l->Entity->Transform.Position + zp, yn),
					glm::lookAt(l->Entity->Transform.Position, l->Entity->Transform.Position + zn, yn) 
				};

				l->Projection = glm::perspective(90.0f, 1.0f, 0.001f, l->Entity->Transform.Scale.x);
				
				// For each face of the cube map
				for (int i = 0; i < 6; i++)
				{
					// Set up render target
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, l->ShadowMap->GetID(), 0);

					if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
						Debug::Error("Shadow framebuffer incomplete.");

					glClear(GL_DEPTH_BUFFER_BIT);
					Debug::GLError("ERROR: Could not initialize the shadow framebuffer.");
					
					// Get objects within light range
					auto objs = l->GetLitObjects();

					// Render them all
					for (auto e : *objs)
					{
						if (e->IsRenderable()) {

							glm::mat4 M = e->Transform.ToMatrix();
							glm::mat4 MV = lvm[i] * M;
							glm::mat4 MVP = l->Projection * MV;

							auto r = e->GetRenderable();
							if (r == Sphere)
							{
								ShadowSphereShader->MakeCurrent();

								glUniformMatrix4fv(ShadowSphereShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
								glUniformMatrix4fv(ShadowSphereShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
								glUniformMatrix4fv(ShadowSphereShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(l->Projection));
								glUniform3fv(ShadowSphereShader->GetUL("SpherePosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
								glUniform1f(ShadowSphereShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);
								
								r->EnableBuffers();
								r->Render();
								r->DisableBuffers();
							}
							else if (r == Cylinder)
							{
								ShadowCylinderShader->MakeCurrent();

								glUniformMatrix4fv(ShadowCylinderShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
								glUniformMatrix4fv(ShadowCylinderShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
								glUniformMatrix4fv(ShadowCylinderShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(l->Projection));
								glUniform3fv(ShadowCylinderShader->GetUL("Direction"), 1, glm::value_ptr(glm::normalize(glm::vec3(MV * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)))));
								glUniform3fv(ShadowCylinderShader->GetUL("Center"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
								glUniform1f(ShadowCylinderShader->GetUL("Length"), r->Entity->Transform.Scale.y);
								glUniform1f(ShadowCylinderShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);
								
								r->EnableBuffers();
								r->Render();
								r->DisableBuffers();
							}
							else
							{
								ShadowMeshShader->MakeCurrent();

								glUniformMatrix4fv(ShadowMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
								glUniformMatrix4fv(ShadowMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
								
								r->EnableBuffers();
								r->Render();
								r->DisableBuffers();
							}

						}
					}
				}
			}
		}

		//glCullFace(GL_BACK);
	}


	void Scene::RenderLight()
	{
		LightRB->MakeCurrent();
		LightRB->Clear();


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetDepthTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(1));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(3));


		LightShader->MakeCurrent();
		glUniform1i(LightShader->GetUL("DepthTexture"), 0);
		glUniform1i(LightShader->GetUL("NormalTexture"), 1);
		glUniform1i(LightShader->GetUL("HardnessTexture"), 2);
		glUniform2f(LightShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(LightShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(glm::inverse(P)));



		LightWithShadowShader->MakeCurrent();
		glUniform1i(LightWithShadowShader->GetUL("DepthTexture"), 0);
		glUniform1i(LightWithShadowShader->GetUL("NormalTexture"), 1);
		glUniform1i(LightWithShadowShader->GetUL("HardnessTexture"), 2);
		glUniform2f(LightWithShadowShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(LightWithShadowShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(glm::inverse(P)));
		glUniformMatrix4fv(LightWithShadowShader->GetUL("ViewInverse"), 1, GL_FALSE, glm::value_ptr(glm::inverse(V)));


		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);	// Additive Blending
		glCullFace(GL_FRONT);

		for (auto r : Lights)
		{
			auto e = r->Entity;
			glm::mat4 M = glm::translate(e->Transform.Position) * glm::scale(e->Transform.Scale);
			glm::mat4 MV = V * M;
			glm::mat4 MVP = P * MV;

			if (r->CastsShadow() && r->GetLitObjects()->size() > 0)
			{
				LightWithShadowShader->MakeCurrent();

				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_CUBE_MAP, r->ShadowMap->GetID());
				glUniform1i(LightWithShadowShader->GetUL("ShadowTexture"), 3);

				glUniformMatrix4fv(LightWithShadowShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(LightWithShadowShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniform4fv(LightWithShadowShader->GetUL("LightColor"), 1, glm::value_ptr(glm::vec4(r->Color, 1.0f)));
				glUniform3fv(LightWithShadowShader->GetUL("LightPosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0))));
				float lightRadius = e->Transform.Scale.x / 2;
				glUniform1f(LightWithShadowShader->GetUL("LightRadius"), lightRadius);

				glUniformMatrix4fv(LightWithShadowShader->GetUL("LightViewMatrix"), 1, GL_FALSE, glm::value_ptr(glm::translate(-e->Transform.Position)));
				glUniformMatrix4fv(LightWithShadowShader->GetUL("LightProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(r->Projection));
			}
			else
			{
				LightShader->MakeCurrent();

				glUniformMatrix4fv(LightShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(LightShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniform4fv(LightShader->GetUL("LightColor"), 1, glm::value_ptr(glm::vec4(r->Color, 1.0f)));
				glUniform3fv(LightShader->GetUL("LightPosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0))));
				float lightRadius = e->Transform.Scale.x / 2;
				glUniform1f(LightShader->GetUL("LightRadius"), lightRadius);

			}

			Cube->EnableBuffers();
			Cube->Render();
			Cube->DisableBuffers();
		}

		glCullFace(GL_BACK);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Reset Blending
	}

	
	void Scene::RenderBloom()
	{
		GlowMapHorizontalRB->MakeCurrent();
		GlowMapHorizontalRB->Clear();

		BlurShader->MakeCurrent();
		glUniform2f(BlurShader->GetUL("texelSize"), 1.0f / ((float)Settings::Window::Width), 1.0f / ((float)Settings::Window::Height));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(4));
		glUniform1i(BlurShader->GetUL("textureSample"), 0);

		glUniform1i(BlurShader->GetUL("blurAmount"), 32);
		glUniform1f(BlurShader->GetUL("blurScale"), 2.0f);
		glUniform1f(BlurShader->GetUL("blurStrength"), 1.0f);

		glUniform1i(BlurShader->GetUL("orientation"), 0);

		SQuad.Render();

		GlowMapVerticalRB->MakeCurrent();
		GlowMapVerticalRB->Clear();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GlowMapHorizontalRB->GetOutputTexture(0));
		glUniform1i(BlurShader->GetUL("textureSample"), 0);

		glUniform1i(BlurShader->GetUL("blurAmount"), 32);
		glUniform1f(BlurShader->GetUL("blurScale"), 2.0f);
		glUniform1f(BlurShader->GetUL("blurStrength"), 1.0f);

		glUniform1i(BlurShader->GetUL("orientation"), 1);

		SQuad.Render();
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

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, GlowMapVerticalRB->GetOutputTexture(0));
		glUniform1i(BufferCombineShader->GetUL("GlowTexture"), 5);

		glUniform3fv(BufferCombineShader->GetUL("AmbientLight"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));

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

		if (Settings::Misc::ShowConsole)
		{
			FontShader->MakeCurrent();
			consoleText->Render(FontShader);
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
		GlowMapHorizontalRB->Rebuild();
		GlowMapVerticalRB->Rebuild();

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


	void Scene::AddLight(LightSource* l)
	{
		Lights.push_back(l);
	}


	void Scene::AppendConsole(std::string msg)
	{
		consoleLines.push(msg);
		if (consoleLines.size() > 3)
		{
			consoleLines.pop();
		}

		std::string text = "";
		std::queue<std::string> temp;
		size_t count = consoleLines.size();
		for (size_t i = 0; i < count; i++)
		{
			text += consoleLines.front() + "\n";
			temp.push(consoleLines.front());
			consoleLines.pop();
		}

		consoleLines.swap(temp);
		consoleText->UpdateText(text);
	}

	
	void Scene::LoadScene(std::string sceneFile)
	{
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
			for (int j = 0; j < 5; j++)
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
		e->AddComponent(new LightSource(this, glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 10.0f, 25.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(this, glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(10.0f, 25.0f, 0.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(this, glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(25.0f, 10.0f, 0.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(this, glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 25.0f, 10.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(this, glm::vec3(1.0f, 1.0f, 1.0f)));
		Entities.push_back(e);

		e = new Entity();
		e->Transform.Position = glm::vec3(-2.0f, 1.5f, 0.0f);
		e->Transform.Scale = glm::vec3(30.0f, 30.0f, 30.0f);
		e->AddComponent(new LightSource(this, glm::vec3(1.0f, 1.0f, 1.0f), true));
		Entities.push_back(e);


		e = new Entity();
		e->Transform.Position = glm::vec3(-2.0f, 1.0f, -4.0f);
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
	}

}