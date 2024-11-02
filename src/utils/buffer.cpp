#include <std_include.hpp>

#include "buffer.hpp"

namespace utils
{
	read_buffer::read_buffer(const char* buf)
		: buf_(buf)
	{
	}

	std::string read_buffer::read_string(const std::size_t len)
	{
		std::string data;
		data.append(this->buf_ + this->off_, len);
		this->advance(len);
		return data;
	}

	void read_buffer::advance(const std::size_t amount)
	{
		this->off_ += amount;
	}

	void write_buffer::write_string(const char* string, const std::size_t len)
	{
		this->append(string, len);
	}

	void write_buffer::write_string(const std::string& string)
	{
		this->write_string(string.data(), string.size());
	}
}