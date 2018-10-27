#include <ctre.hpp>
#include <iostream>

std::ostream& operator<<(std::ostream& os, ctre::partial_ordering ord) {
	using namespace ctre;
	switch (ord) {
		case partial_ordering::less:
			return os << "less";
		case partial_ordering::greater:
			return os << "greater";
		case partial_ordering::equal:
			return os << "equal";
		case partial_ordering::unordered:
			return os << "unordered";
		default:
			return os << "err";
	}
}

int main() {
	using namespace ctre::literals;
	constexpr auto pattern = "abc[1-35-7]|xyz[b-dx-z]";
	constexpr auto re = "abc[1-35-7]|xyz[b-dx-z]"_ctre;

	std::cout << "Checking ordering against pattern: \"" << pattern << '"' << std::endl;

	auto check = [&](auto string, auto expected) {
		std::cout << "Subject: \"" << string
		          << "\",Expected: " << expected
			  << ", ctre: " << ctre::partial_ordering(re.match(string))
			  << std::endl;
	};
	
	check("aaa", "less");
	check("abc1", "equal");
	check("cba1", "unordered");
	check("abc4", "unordered");
	check("ab", "less");
	check("xyzb", "equal");
	check("xyzzz", "greater");
}
