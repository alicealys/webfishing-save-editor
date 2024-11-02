#pragma once

namespace tool
{
	std::int64_t align_value(const std::int64_t val, const std::int64_t alignment);

	enum gotot_type : std::uint32_t
	{
		type_null = 0,
		type_boolean = 1,
		type_integer = 2,
		type_float = 3,
		type_string = 4,
		type_vec2 = 5,
		type_rect2 = 6,
		type_vec3 = 7,
		type_transform2d = 8,
		type_plane = 9,
		type_quat = 10,
		type_aabb = 11,
		type_basis = 12,
		type_transform3d = 13,
		type_color = 14,
		type_nodepath = 15,
		type_rid = 16,
		type_object = 17,
		type_dictionary = 18,
		type_array = 19,
		// ...
		type_set_flag = 0x10000
	};
}
