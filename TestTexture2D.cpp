#include "TestTexture2D.h"

#include "GLErrorManager.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

namespace test {

	TestTexture2D::TestTexture2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 400, 0),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), //first 4 floats are window boundaries
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		//vertex positions and textures
	   float vertices[] = {
		   -50.0f, -50.0f, 0.0f, 0.0f,//0
			50.0f, -50.0f, 1.0f, 0.0f,//1
			50.0f,  50.0f, 1.0f, 1.0f,//2
		   -50.0f,  50.0f, 0.0f, 1.0f //3
	   };

	   unsigned int indices[]{
		   0, 1, 2,
		   2, 3, 0,
	   };

	   //blender
	   GLCall(glEnable(GL_BLEND));
	   GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	   m_VAO = std::make_unique<VertexArray>(); //vertex array object (vao)
	   m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float)); //vertex buffer

	   //provide specification of the vertices (tells gpu how to read the provided info)
	   VertexBufferLayout layout;
	   layout.Push<float>(2);
	   layout.Push<float>(2);

	   //add buffer and layout to vertex array
	   m_VAO->AddBuffer(*m_VertexBuffer, layout);

	   m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6); //index buffer

	   //create and bind shader
	   m_Shader = std::make_unique<Shader>("Basic.shader");
	   m_Shader->Bind();

	   //texture
	   m_Texture = std::make_unique<Texture>("Sword.png");
	   m_Texture->Bind();
	   m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		//first model
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		//second model
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGUI()
	{
		//edit model translation
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}