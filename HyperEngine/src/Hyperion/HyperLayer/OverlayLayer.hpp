#pragma once

#include <string>

#include "HyperEvents/Event.hpp"
#include "HyperRendering/RenderContext.hpp"
#include "HyperUtilities/Timestep.hpp"

namespace Hyperion 
{
	class OverlayLayer
	{
	protected:
		std::string m_LayerName;
		Ref<RenderContext> m_RenderContext;

	public:
		OverlayLayer(const std::string& name = "Default OverlayLayer");
		virtual ~OverlayLayer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnEvent(Event& event) { }
		virtual void OnTick(int currentTick) {}
		virtual void OnUpdate(Timestep timeStep) {}
		virtual void OnRender() {}

		void SetRenderContext(Ref<RenderContext> renderContext);
		const Ref<RenderContext> GetRenderContext() const;

		void SetName(const std::string& layerName);
		const std::string& GetName() const;
	};
}
