#ifndef CTRE__ATOMS_CHARACTERS__HPP
#define CTRE__ATOMS_CHARACTERS__HPP

#include "utility.hpp"
#include "ordering.hpp"
#include <cstdint>

namespace ctre {
	
// sfinae check for types here

template <typename T> class MatchesCharacter {
	template <typename Y, typename CharT> static auto test(Y*, CharT c) -> decltype(Y::match_char(c), std::true_type());
	template <typename> static auto test(...) -> std::false_type;
public:
	template <typename CharT> static inline constexpr bool value = decltype(test<T>(nullptr, std::declval<CharT>()))();
};



template <auto V> struct character {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return value == V;
	}
	template <typename CharT> CTRE_FORCE_INLINE static constexpr partial_ordering compare_char(CharT value) noexcept {
		if ( V > value ) return partial_ordering::less;
		if ( V < value ) return partial_ordering::greater;
		return partial_ordering::equal;
	}
};

struct any {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept { return true; }
};

template <typename... Content> struct negative_set {
	template <typename CharT> inline static constexpr bool match_char(CharT value) noexcept {
		return !(Content::match_char(value) || ... || false);
	}
};

template <typename... Content> struct set {
	template <typename CharT> inline static constexpr bool match_char(CharT value) noexcept {
		return (Content::match_char(value) || ... || false);
	}
	template <typename CharT> inline static constexpr partial_ordering compare_char(CharT value) noexcept {
		if (match_char(value)) return partial_ordering::equal;
		if (((Content::compare_char(value) == partial_ordering::less) && ... && true))
			return partial_ordering::less;
		if (((Content::compare_char(value) == partial_ordering::greater) && ... && true))
			return partial_ordering::greater;
		return partial_ordering::unordered;
	}
};

template <typename... Content> struct negate {
	template <typename CharT> inline static constexpr bool match_char(CharT value) noexcept {
		return !(Content::match_char(value) || ... || false);
	}
};

struct word_chars {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9') || (value == '_');
	}
};

struct space_chars {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return value == ' ' || value == '\t' || value == '\n' || value == '\v' || value == '\f' || value == '\r';
	}
};

struct alphanum_chars {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') || (value >= '0' && value <= '9');
	}
};

struct alpha_chars {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');
	}
};

struct xdigit_chars {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return (value >= 'A' && value <= 'F') || (value >= 'a' && value <= 'f') || (value >= '0' && value <= '9');
	}
};

struct punct_chars {
	template <typename CharT> inline static constexpr bool match_char(CharT value) noexcept {
		return value == '!' || value == '"' || value == '#' || value == '$' || value == '%'
			|| value == '&' || value == '\''|| value == '(' || value == ')' || value == '*' || value == ','
			|| value == '-' || value == '.' || value == '/' || value == ':' || value == ';'
			|| value == '<' || value == '=' || value == '>' || value == '?' || value == '@' || value == '['
			|| value == '\\'|| value == ']' || value == '^' || value == '_' || value == '`'
			|| value == '{' || value == '|' || value == '}' || value == '~';
	}
};

struct digit_chars {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return (value >= '0' && value <= '9');
	}
};

struct ascii_chars {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return (value >= '\x00' && value <= '\x7F');
	}
};

template <auto A, auto B> struct char_range {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return (value >= A) && (value <= B);
	}
	template <typename CharT> CTRE_FORCE_INLINE static constexpr partial_ordering compare_char(CharT value) noexcept {
		if (value < A) return partial_ordering::less;
		if (value > B) return partial_ordering::greater;
		return partial_ordering::equal;
	}
};
template <auto... Cs> struct enumeration {
	template <typename CharT> inline static constexpr bool match_char(CharT value) noexcept {
		return ((value == Cs) || ... || false);
	}
};

}

#endif
