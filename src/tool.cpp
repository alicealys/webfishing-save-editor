#include <std_include.hpp>

#include "tool.hpp"

#include "deserializer.hpp"
#include "serializer.hpp"

#include <utils/io.hpp>
#include <utils/flags.hpp>
#include <utils/string.hpp>

namespace tool
{
	namespace
	{
		std::string get_base_name(const std::string& file)
		{
			const auto last_slash = file.find_last_of("/\\");
			const auto dot = file.find_last_of('.');
			return file.substr(last_slash + 1, (dot - last_slash) - 1);
		}

		void deserialize(const std::string& name, const std::string& data)
		{
			const auto target_file = std::format("{}.json", name);

			deserializer deserializer(data.data());
			deserializer.dump(target_file);
		}

		void serialize(const std::string& name, const std::string& data)
		{
			const auto target_file = std::format("{}.save", name);
			const auto json = nlohmann::ordered_json::parse(data);

			serializer serializer;
			serializer.write(json, target_file);
		}

		void start_unsafe(int argc, char** argv)
		{
			if (argc < 2)
			{
				printf("usage: webfishing-se <file>\n");
				return;
			}

			const std::string file = argv[1];
			std::string data;

			if (!utils::io::read_file(file, &data))
			{
				throw std::runtime_error("file not found");
			}

			const auto base_name = get_base_name(file);

			if (file.ends_with(".json"))
			{
				serialize(base_name, data);
			}
			else if (file.ends_with(".save"))
			{
				deserialize(base_name, data);
			}
			else
			{
				throw std::runtime_error("unknown file type");
			}
		}
	}

	void main(int argc, char** argv)
	{
		try
		{
			start_unsafe(argc, argv);
		}
		catch (const std::exception& e)
		{
			printf("error: %s\n", e.what());
			system("pause");
		}
	}
}
