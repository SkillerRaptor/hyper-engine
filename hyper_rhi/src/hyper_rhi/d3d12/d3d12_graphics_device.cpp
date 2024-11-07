/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/d3d12/d3d12_graphics_device.hpp"

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/d3d12/d3d12_surface.hpp"

namespace hyper_rhi
{
    D3D12GraphicsDevice::D3D12GraphicsDevice(const GraphicsDeviceDescriptor &descriptor)
        : GraphicsDevice(descriptor)
        , m_factory(nullptr)
        , m_adapter(nullptr)
        , m_device(nullptr)
        , m_command_queue(nullptr)
        , m_allocator(nullptr)
    {
        if (m_debug_validation)
        {
            this->enable_debug_layers();
        }

        if (!m_debug_validation)
        {
            m_debug_label = false;
            m_debug_marker = false;
        }

        this->create_factory();
        this->choose_adapter();
        this->create_device();
        this->create_command_queue();
        this->create_allocator();

        HE_INFO("Created D3D12 Graphics Device");
    }

    std::shared_ptr<Surface> D3D12GraphicsDevice::create_surface(const hyper_platform::Window &window)
    {
        return std::make_shared<D3D12Surface>(*this, window);
    }

    std::shared_ptr<Buffer> D3D12GraphicsDevice::create_buffer(const BufferDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    std::shared_ptr<CommandList> D3D12GraphicsDevice::create_command_list()
    {
        HE_UNREACHABLE();
    }

    std::shared_ptr<ComputePipeline> D3D12GraphicsDevice::create_compute_pipeline(const ComputePipelineDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    std::shared_ptr<RenderPipeline> D3D12GraphicsDevice::create_render_pipeline(const RenderPipelineDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    std::shared_ptr<PipelineLayout> D3D12GraphicsDevice::create_pipeline_layout(const PipelineLayoutDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    std::shared_ptr<Sampler> D3D12GraphicsDevice::create_sampler(const SamplerDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    std::shared_ptr<ShaderModule> D3D12GraphicsDevice::create_shader_module(const ShaderModuleDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    std::shared_ptr<Texture> D3D12GraphicsDevice::create_texture(const TextureDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    std::shared_ptr<TextureView> D3D12GraphicsDevice::create_texture_view(const TextureViewDescriptor &descriptor)
    {
        HE_UNUSED(descriptor);

        HE_UNREACHABLE();
    }

    void D3D12GraphicsDevice::begin_frame(const std::shared_ptr<Surface> &surface, const uint32_t frame_index)
    {
        HE_UNUSED(surface);
        HE_UNUSED(frame_index);

        HE_UNREACHABLE();
    }

    void D3D12GraphicsDevice::end_frame() const
    {
        HE_UNREACHABLE();
    }

    void D3D12GraphicsDevice::execute(const std::shared_ptr<CommandList> &command_list) const
    {
        HE_UNUSED(command_list);

        HE_UNREACHABLE();
    }

    void D3D12GraphicsDevice::present(const std::shared_ptr<Surface> &surface) const
    {
        HE_UNUSED(surface);

        HE_UNREACHABLE();
    }

    void D3D12GraphicsDevice::wait_for_idle() const
    {
        HE_UNREACHABLE();
    }

    ComPtr<IDXGIFactory7> D3D12GraphicsDevice::factory() const
    {
        return m_factory;
    }

    ComPtr<ID3D12CommandQueue> D3D12GraphicsDevice::command_queue() const
    {
        return m_command_queue;
    }

    void D3D12GraphicsDevice::enable_debug_layers()
    {
        ComPtr<ID3D12Debug> debug_controller = nullptr;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
        {
            debug_controller->EnableDebugLayer();
            m_debug_validation = true;

            HE_TRACE("Enabled Debug Layers");
        }
        else
        {
            HE_WARN("Failed to enable requested Debug Layers");
        }
    }

    void D3D12GraphicsDevice::create_factory()
    {
        const UINT factory_flags = m_debug_validation ? DXGI_CREATE_FACTORY_DEBUG : 0;
        HE_DX_CHECK(CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_factory)));
        HE_ASSERT(m_factory != nullptr);

        HE_TRACE("Created Factory");
    }

    void D3D12GraphicsDevice::choose_adapter()
    {
        uint32_t adapter_index = 0;
        while (true)
        {
            HE_DX_CHECK(m_factory->EnumAdapterByGpuPreference(adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_adapter)));

            DXGI_ADAPTER_DESC1 description = {};
            HE_DX_CHECK(m_adapter->GetDesc1(&description));

            if (description.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }

            if (SUCCEEDED(D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr)))
            {
                HE_TRACE("Selected Adapter");
                break;
            }

            ++adapter_index;
        }
    }

    void D3D12GraphicsDevice::create_device()
    {
        HE_DX_CHECK(D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)));
        HE_ASSERT(m_device != nullptr);

        HE_TRACE("Created Device");
    }

    void D3D12GraphicsDevice::create_command_queue()
    {
        constexpr D3D12_COMMAND_QUEUE_DESC command_queue_descriptor = {
            .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
            .Priority = 0,
            .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
            .NodeMask = 0,
        };

        HE_DX_CHECK(m_device->CreateCommandQueue(&command_queue_descriptor, IID_PPV_ARGS(&m_command_queue)));
        HE_ASSERT(m_command_queue != nullptr);

        HE_TRACE("Created Command Queue");
    }

    void D3D12GraphicsDevice::create_allocator()
    {
        const D3D12MA::ALLOCATOR_DESC allocator_descriptor = {
            .Flags = static_cast<D3D12MA::ALLOCATOR_FLAGS>(
                D3D12MA::ALLOCATOR_FLAG_ALWAYS_COMMITTED | D3D12MA::ALLOCATOR_FLAG_DEFAULT_POOLS_NOT_ZEROED),
            .pDevice = m_device.Get(),
            .PreferredBlockSize = 0,
            .pAllocationCallbacks = nullptr,
            .pAdapter = m_adapter.Get(),
        };

        HE_DX_CHECK(D3D12MA::CreateAllocator(&allocator_descriptor, &m_allocator));
        HE_ASSERT(m_allocator != nullptr);

        HE_TRACE("Created Allocator");
    }
} // namespace hyper_rhi
