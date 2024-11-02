#include <std_include.hpp>

#include "deserializer.hpp"
#include "common.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace tool
{
	deserializer::deserializer(utils::read_buffer buffer)
		: buffer_(buffer)
	{
		const auto size = this->buffer_.read<std::uint32_t>();
		if (size < 4)
		{
			throw std::runtime_error("invalid save file");
		}
	}

	nlohmann::ordered_json deserializer::read_value()
	{
		const auto type_val = this->buffer_.read<std::uint32_t>();
		const auto base_type = type_val & 0xFFFF;
		const auto flag = type_val >> 16;

		switch (base_type)
		{
		case type_null:
			return {};
		case type_boolean:
			return this->buffer_.read<std::uint32_t>() == 1;
		case type_integer:
			return (flag == 0 ? this->buffer_.read<std::int32_t>() : this->buffer_.read<std::int64_t>());
		case type_float:
			return (flag == 0 ? this->buffer_.read<float>() : this->buffer_.read<double>());
		case type_string:
		{
			const auto len = this->buffer_.read<std::uint32_t>();
			const auto padded_len = align_value(len, 4);
			const auto string = this->buffer_.read_string(len);
			this->buffer_.advance(padded_len - len);
			return string;
		}
		case type_vec2:
		{
			nlohmann::ordered_json vec;
			vec["x"] = this->buffer_.read<float>();
			vec["y"] = this->buffer_.read<float>();
			return vec;
		}
		case type_dictionary:
		{
			auto dict = nlohmann::ordered_json::object();
			const auto size = this->buffer_.read<std::uint32_t>();

			for (auto i = 0u; i < size; i++)
			{
				const auto key_val = this->read_value();

				std::string key;
				if (key_val.is_string())
				{
					key = key_val.get<std::string>();
				}
				else if (key_val.is_number_integer())
				{
					key = utils::string::va("0x%08X", key_val.get<std::uint32_t>());
				}
				else
				{
					throw std::runtime_error(std::format("invalid dictionary key {}", key_val.type_name()));
				}

				dict[key] = this->read_value();
			}

			return dict;
		}
		case type_array:
		{
			auto arr = nlohmann::ordered_json::array();

			const auto size = this->buffer_.read<std::uint32_t>();

			for (auto i = 0u; i < size; i++)
			{
				const auto value = this->read_value();
				arr[i] = value;
			}

			return arr;
		}
		}

		throw std::runtime_error(std::format("unsupported save value type {}", base_type));
	}

	void deserializer::dump(const std::string& path)
	{
		const auto json = this->read_value();
		const auto str = json.dump(4);
		utils::io::write_file(path, str);
	}
}
