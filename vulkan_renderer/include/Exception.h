#pragma once
#include <exception>
#include <string>
#include <sstream>

#define EXCEPT(...)  Exception(__LINE__,__FILE__,##__VA_ARGS__)
#define THROW_EXCEPT(...) throw EXCEPT(##__VA_ARGS__)

#define VK_EXCEPT(...)  VulkanException(__LINE__,__FILE__,##__VA_ARGS__)
#define THROW_VK_EXCEPT(...) throw VK_EXCEPT(##__VA_ARGS__)

enum VkResult;

//异常基类
class Exception : public std::exception
{
public:
	Exception(int line, const char* file) noexcept :
		line(line), file(file)
	{}
	explicit Exception(int line, const char* file, char const* const message) noexcept :
		line(line), file(file), std::exception(message)
	{}
	const char* what() const noexcept override
	{
		std::ostringstream oss;
		oss << "[Type       ] " << GetType() << std::endl
			<< "[Description] " << std::exception::what() << std::endl
			<< GetOriginString() << std::endl;
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	virtual const char* GetType() const noexcept
	{
		return "Exception";
	}

	std::string GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << file << std::endl
			<< "[Line] " << line;
		return oss.str();
	}
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;;
};

class VulkanException : public Exception
{
public:
	VulkanException(int line, const char* file) noexcept
		: Exception(line, file)
	{}

	explicit VulkanException(int line, const char* file, char const* const message, VkResult result) noexcept
		: Exception(line, file, message)
		, result(result)
	{}

	const char* what() const noexcept override
	{
		std::ostringstream oss;
		oss << "[Type       ] " << GetType() << std::endl
			<< "[Description] " << std::exception::what() << std::endl
			<< "[VkResult   ] " << std::hex << uint32_t(result) << std::dec << std::endl
			<< GetOriginString() << std::endl;
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	virtual const char* GetType() const noexcept
	{
		return "VulkanException";
	}

private:
	VkResult result;
};






