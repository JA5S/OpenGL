#include "TestQuadBatch.h"

#include "GLErrorManager.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

namespace test {

	TestQuadBatch::TestQuadBatch()
		: m_Translation(200, 200, 0),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), //first 4 floats are window boundaries
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		//vertex positions (x, y) and color (r, g, b, a)
		float vertices[] = {
			//Quad 1
			-150.0f, -50.0f, //0
			 -50.0f, -50.0f, //1
			 -50.0f,  50.0f, //2
			-150.0f,  50.0f, //3

			//Quad 2
			 50.0f, -50.0f, //4
			150.0f, -50.0f, //5
			150.0f,  50.0f, //6
			 50.0f,  50.0f  //7
		};

		unsigned int indices[]{
			0, 1, 2, 2, 3, 0, //Quad 1
			4, 5, 6, 6, 7, 4  //Quad 2
		};

		m_VAO = std::make_unique<VertexArray>(); //vertex array object (vao)
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 2 * 8 * sizeof(float)); //vertex buffer

		//provide specification of the vertices (tells gpu how to read the provided info)
		VertexBufferLayout layout;
		layout.Push<float>(2);

		//add buffer and layout to vertex array
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12); //index buffer

		//create and bind shader
		m_Shader = std::make_unique<Shader>("Primitive.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.8f, 0.4f, 1.0f); //set rgba vec4 in fragment shader
	}

	TestQuadBatch::~TestQuadBatch()
	{
	}

	void TestQuadBatch::OnUpdate(float deltaTime)
	{
		
	}

	void TestQuadBatch::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.1, 0.8f, 0.4f, 1.0f);
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestQuadBatch::OnImGUI()
	{
		//edit model translation
		ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}