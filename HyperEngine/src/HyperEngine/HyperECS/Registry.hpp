#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "HyperCore/Core.hpp"
#include "HyperECS/Entity.hpp"
#include "HyperECS/SparseSet.hpp"

namespace HyperECS
{
	class Registry
	{
	private:
		struct PoolData
		{
			uint64_t Id;
			HyperCore::Ref<SparseSet> Pool{};
		};

	private:
		std::vector<Entity> m_Entities = {};
		std::vector<PoolData> m_Pools = {};
		Entity m_Available{ Null };

	public:
		Registry() = default;
		~Registry();

		Entity Create();
		void Destroy(const Entity entity);

		bool IsValid(const Entity entity) const;

		template<typename T, typename... Args>
		T& AddComponent(const Entity entity, Args&&... args)
		{
			if (HasComponent<T>(entity))
				throw std::runtime_error("The provided entity has already the specified component!");

			size_t componentId{ typeid(T).hash_code() };
			PoolData* pool = FindPool(componentId);
			if (pool == nullptr)
				pool = &m_Pools.emplace_back(PoolData{ static_cast<uint64_t>(componentId), HyperCore::CreateRef<SparseSetImpl<T>>() });
			
			HyperCore::Ref<SparseSetImpl<T>> sparsePool = std::static_pointer_cast<SparseSetImpl<T>>(pool->Pool);
			
			if (pool->Pool->Contains(entity))
				throw std::runtime_error("The provided entity exists already in the pool!");

			return sparsePool->Emplace(entity, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent(const Entity entity)
		{
			size_t componentId{ typeid(T).hash_code() };
			PoolData* pool = FindPool(componentId);
			if (pool == nullptr)
				throw std::runtime_error("The pool for specified component does not exists!");
			
			if (!pool->Pool->Contains(entity))
				throw std::runtime_error("The provided entity does not exists in the pool!");

			if (!HasComponent<T>(entity))
				throw std::runtime_error("The provided entity has not the specified component!");
			
			pool->Pool->Remove(entity);
		}

		void RemoveAllComponents(const Entity entity);

		template<typename T>
		T& GetComponent(const Entity entity)
		{
			size_t componentId{ typeid(T).hash_code() };
			PoolData* pool = FindPool(componentId);
			if (pool == nullptr)
				throw std::runtime_error("The pool for specified component does not exists!");

			if (!HasComponent<T>(entity))
				throw std::runtime_error("The provided entity has not the specified component!");

			return *static_cast<T*>(pool->Pool->Get(entity));
		}

		template<typename T>
		bool HasComponent(const Entity entity)
		{
			size_t componentId{ typeid(T).hash_code() };
			PoolData* pool = FindPool(componentId);
			if (pool == nullptr)
				return false;
			
			return pool->Pool->Contains(entity);
		}

		template<class... T>
		std::vector<Entity> Each()
		{
			std::vector<Entity> entities;
			for (const Entity& entity : m_Entities)
			{
				bool shouldSkip = false;

				(IterateEntities(shouldSkip, entity, ((T*) nullptr)), ...);

				if (shouldSkip)
					continue;

				entities.push_back(entity);
			}
			return entities;
		}

		inline const std::vector<Entity>& GetEntities() const
		{
			return m_Entities;
		}

		static inline EntityTraits::EntityType GetEntityId(const Entity entity) noexcept
		{
			return EntityTraits::EntityType{ entity & EntityTraits::EntityMask };
		}

		static inline EntityTraits::VersionType GetEntityVersion(const Entity entity) noexcept
		{
			return EntityTraits::VersionType{ static_cast<EntityTraits::VersionType>(entity >> static_cast<EntityTraits::EntityType>(EntityTraits::EntityShift)) };
		}

	private:
		Entity GenerateIdentifier();
		Entity RecycleIdentifier();
		PoolData* FindPool(uint64_t id);

		template <class T>
		void IterateEntities(bool& shouldSkip, const Entity& entity, T* v)
		{
			using C = decltype(*v);
			if (shouldSkip)
				return;
			if (!HasComponent<C>(entity))
				shouldSkip = true;
		}
	};
}
