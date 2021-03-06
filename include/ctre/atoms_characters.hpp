#ifndef CTRE__ATOMS_CHARACTERS__HPP
#define CTRE__ATOMS_CHARACTERS__HPP

#include "utility.hpp"
#include <cstdint>

namespace ctre {
	
// sfinae check for types here

template <typename T> class MatchesCharacter {
	template <typename Y, typename CharT> static auto test(CharT c) -> decltype(Y::match_char(c), std::true_type());
	template <typename> static auto test(...) -> std::false_type;
public:
	template <typename CharT> static inline constexpr bool value = decltype(test<T>(std::declval<CharT>()))();
};



template <auto V> struct character {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT value) noexcept {
		return value == V;
	}
};

struct any {
	template <typename CharT> CTRE_FORCE_INLINE static constexpr bool match_char(CharT) noexcept { return true; }
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
};

template <typename... Content> struct negate {
	template <typename CharT> inline static constexpr bool match_char(CharT value) noexcept {
		return !(Content::match_char(value) || ... || false);
	}
};

// I know word_chars and others can be expressed with set<> and range<>, but this makes the regex bad readable when debugging, please keep this in mind when touching this code in future :)

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
};
template <auto... Cs> struct enumeration {
	template <typename CharT> inline static constexpr bool match_char(CharT value) noexcept {
		return ((value == Cs) || ... || false);
	}
};

}

#endif
