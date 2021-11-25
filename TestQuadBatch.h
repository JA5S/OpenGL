#pragma once
#include "Test.h"

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace test {

	class TestQuadBatch : public Test
	{
	public:
		TestQuadBatch();
		~TestQuadBatch();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGUI() override;
	private:
		std::unique_ptr <VertexArray> m_VAO;
		std::unique_ptr <VertexBuffer> m_VertexBuffer;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <Shader> m_Shader;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation;
	};

}