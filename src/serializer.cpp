#include <std_include.hpp>

#include "serializer.hpp"
#include "common.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace tool
{
	void serializer::write_value(const nlohmann::ordered_json& value)
	{
		const auto type = value.type();

		switch (type)
		{
		case nlohmann::json::value_t::null:
			this->buffer_.write(type_null);
			return;
		case nlohmann::json::value_t::boolean:
			this->buffer_.write(type_boolean);
			this->buffer_.write(value.get<std::uint32_t>());
			return;
		case nlohmann::json::value_t::number_integer:
		case nlohmann::json::value_t::number_unsigned:
			this->buffer_.write(type_integer);
			this->buffer_.write(value.get<std::uint32_t>());
			return;
		case nlohmann::json::value_t::number_float:
			this->buffer_.write(type_float);
			this->buffer_.write(value.get<float>());
			return;
		case nlohmann::json::value_t::string:
		{
			const auto string = value.get<std::string>();
			const auto len = string.size();
			const auto padded_len = static_cast<std::size_t>(align_value(len, 4));

			this->buffer_.write(type_string);
			this->buffer_.write(static_cast<std::uint32_t>(len));
			this->buffer_.write_string(string);

			for (auto i = 0u; i < (padded_len - len); i++)
			{
				this->buffer_.write<std::uint8_t>(0);
			}
			
			return;
		}
		case nlohmann::json::value_t::object:
		{
			const auto size = value.size();

			if (size == 2 && value.contains("x") && value.contains("y"))
			{
				const auto& x = value.at("x");
				const auto& y = value.at("y");

				this->buffer_.write(type_vec2);
				this->buffer_.write(x.get<float>());
				this->buffer_.write(y.get<float>());
			}
			else
			{
				this->buffer_.write(type_dictionary);
				this->buffer_.write(static_cast<std::uint32_t>(size));

				const auto items = value.items();

				for (const auto& [key, val] : items)
				{
					if (key.starts_with("0x"))
					{
						const auto key_int = std::strtoull(key.data(), nullptr, 16);
						this->buffer_.write(type_integer | type_set_flag);
						this->buffer_.write(key_int);
					}
					else
					{
						this->write_value(key);
					}

					this->write_value(val);
				}
			}

			return;
		}
		case nlohmann::json::value_t::array:
		{
			const auto size = value.size();

			this->buffer_.write(type_array);
			this->buffer_.write(static_cast<std::uint32_t>(size));

			for (auto i = 0ull; i < size; i++)
			{
				this->write_value(value[i]);
			}

			return;
		}
		}

		throw std::runtime_error(std::format("unsupported json type {}", static_cast<std::uint32_t>(type)));
	}

	void serializer::write(const nlohmann::ordered_json& value, const std::string& path)
	{
		this->write_value(value);

		utils::write_buffer file;
		file.write<std::uint32_t>(static_cast<std::uint32_t>(this->buffer_.size()));

		utils::io::write_file(path, file);
		utils::io::write_file(path, this->buffer_, true);
	}
}
