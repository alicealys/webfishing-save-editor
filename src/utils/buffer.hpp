#pragma once

namespace utils
{
	class read_buffer
	{
	public:
		read_buffer(const char* buf);

		template <typename T>
		T read()
		{
			const auto val = *reinterpret_cast<T*>(reinterpret_cast<std::uint64_t>(this->buf_ + this->off_));
			this->advance(sizeof(T));
			return val;
		}

		std::string read_string(const std::size_t len);

		void advance(const std::size_t amount);

	private:
		const char* buf_{};
		std::size_t off_{};

	};

	class write_buffer : public std::string
	{
	public:
		template <typename T>
		void write(const T& value)
		{
			T val = value;
			this->append(reinterpret_cast<char*>(reinterpret_cast<std::uint64_t>(&val)), sizeof(T));
		}

		void write_string(const char* string, const std::size_t len);
		void write_string(const std::string& string);

	};
}