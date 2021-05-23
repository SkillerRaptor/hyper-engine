#pragma once

#include <type_traits>

namespace Math
{
	template <typename T>
	class Vector3
	{
	public:
		static_assert(std::is_arithmetic_v<T>, "Invalid template type for Vector3!");
	
	public:
		Vector3()
			: x{ static_cast<T>(0) }
			, y{ static_cast<T>(0) }
			, z{ static_cast<T>(0) }
		{
		}
		
		Vector3(T scalar)
			: x{ scalar }
			, y{ scalar }
			, z{ scalar }
		{
		}
		
		Vector3(T x, T y, T z)
			: x{ x }
			, y{ y }
			, z{ z }
		{
		}
	
	public:
		union
		{
			T data[3];
			
			struct
			{
				T x;
				T y;
				T z;
			};
			
			struct
			{
				T r;
				T g;
				T b;
			};
			
			struct
			{
				T s;
				T t;
				T p;
			};
		};
	};
	
	using Vec3 = Vector3<float>;
	using Vec3f = Vector3<float>;
	using Vec3d = Vector3<double>;
	using Vec3i = Vector3<int>;
	using Vec3ui = Vector3<unsigned int>;
}
