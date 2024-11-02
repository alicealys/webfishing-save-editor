#pragma once

#include "utils/buffer.hpp"

namespace tool
{
	class serializer
	{
	public:
		void write_value(const nlohmann::ordered_json& value);
		void write(const nlohmann::ordered_json& value, const std::string& path);

	private:
		utils::write_buffer buffer_;

	};
}
