#pragma once

#include "HyperCore/Core.hpp"
#include "HyperRendering/RenderContext.hpp"
#include "HyperRendering/SceneRecorder.hpp"

#include "Platform/Rendering/OpenGL33/Buffers/OpenGL33FrameBuffer.hpp"

namespace Hyperion
{
	class OpenGL33SceneRecorder : public SceneRecorder
	{
	private:
		Scope<OpenGL33FrameBuffer> m_FrameBuffer;

	public:
		OpenGL33SceneRecorder(Ref<TextureManager> textureManager, GLFWwindow* window);

		virtual void InitRecording() override;
		virtual void StartRecording() override;
		virtual void EndRecording() override;

		virtual void RenderImage() override;
	};
}
