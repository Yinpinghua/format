format
======

A simple header-only C++11 std::string formatter

Usage:
------

```c++

	double d_value = 1.284;
	float f_value = 1.2f;
	uint32_t uint_value = 96040;
	std::string test = util::format("This is a nice string with numbers {0} and strings {1} nicely formatted {2} hehe da {3}", 123, f_value, d_value, uint_value);
std::string test = util::Format("{0, 20}", "Formatting is nice!");
```
