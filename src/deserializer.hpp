#pragma once

#include "utils/buffer.hpp"

namespace tool
{
	class deserializer
	{
	public:
		deserializer(utils::read_buffer buffer);

		nlohmann::ordered_json read_value();
		void dump(const std::string& path);

	private:
		utils::read_buffer buffer_;

	};
}
