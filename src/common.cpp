#include <std_include.hpp>

#include "common.hpp"

namespace tool
{
	std::int64_t align_value(const std::int64_t val, const std::int64_t alignment)
	{
		const auto mask = alignment - 1;
		return (val + (-val & mask));
	}
}
