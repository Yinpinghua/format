#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace util
{
	class arg_base
	{
	public:
		arg_base() = default;
		virtual ~arg_base() = default;
		virtual void format(std::ostringstream& ss, const std::string& fmt) = 0;
	};

	template <class T>
	class arg : public arg_base
	{
	public:
		arg(T arg) : arg_(arg) {}
		virtual ~arg() = default;
		virtual void format(std::ostringstream& ss, const std::string& fmt)
		{
			ss << arg_;
		}
	private:
		T arg_;
	};

	class arg_array : public std::vector < arg_base* >
	{
	public:
		arg_array() = default;
		~arg_array()
		{
			std::for_each(begin(), end(), [](arg_base* p) { delete p; p = nullptr;});
		}
	};

	static void format_item(std::ostringstream& ss, const std::string& item, const arg_array& args)
	{
		int index = 0;
		int alignment = 0;
		std::string fmt;

		char* endptr = nullptr;
		index = strtol(&item[0], &endptr, 10);
		if (index < 0 || index >= args.size())
		{
			return;
		}

		if (*endptr == ',')
		{
			alignment = strtol(endptr + 1, &endptr, 10);
			if (alignment > 0)
			{
				ss << std::right << std::setw(alignment);
			}
			else if (alignment < 0)
			{
				ss << std::left << std::setw(-alignment);
			}
		}

		if (*endptr == ':')
		{
			fmt = endptr + 1;
		}

		args[index]->format(ss, fmt);

		return;
	}

	template <class T>
	static void transfer(arg_array& argArray, T t)
	{
		argArray.push_back(new arg<T>(t));
	}

	template <class T, typename... Args>
	static void transfer(arg_array& argArray, T t, Args&&... args)
	{
		transfer(argArray, t);
		transfer(argArray, args...);
	}

	template <typename... Args>
	std::string format(const std::string& format_str, Args&&... args)
	{
		if (sizeof...(args) == 0)
		{
			return format_str;
		}

		arg_array argArray;
		transfer(argArray, args...);
		size_t start = 0;
		size_t pos = 0;
		std::ostringstream ss;
		while (true)
		{
			pos = format_str.find('{', start);
			if (pos == std::string::npos)
			{
				ss << format_str.substr(start);
				break;
			}

			ss << format_str.substr(start, pos - start);
			if (format_str[pos + 1] == '{')
			{
				ss << '{';
				start = pos + 2;
				continue;
			}

			start = pos + 1;
			pos = format_str.find('}', start);
			if (pos == std::string::npos)
			{
				ss << format_str.substr(start - 1);
				break;
			}

			format_item(ss, format_str.substr(start, pos - start), argArray);
			start = pos + 1;
		}

		return ss.str();
	}
}
