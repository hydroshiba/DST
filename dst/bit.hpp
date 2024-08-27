#ifndef DST_BIT_HPP_
#define DST_BIT_HPP_

#include <type_traits>
#include <functional>

namespace dst {

namespace bit {

static constexpr unsigned char lookup[256] = {
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

#if __cplusplus >= 201703L

template<typename _type, std::size_t _width = (sizeof(_type) - 1) << 3>
inline constexpr std::enable_if_t<std::is_integral_v<_type>, std::size_t>
log(_type value) {
	if constexpr(_width == 0)
		return lookup[value];
	else {
		if(value >> _width) return _width + lookup[value >> _width];
		return log<_type, _width - 8>(value);
	}
}

#else

template<typename _type>
inline typename std::enable_if<std::is_integral<_type>::value, std::size_t>::type
log(_type value) {
	unsigned char temp = 0;

	if(sizeof(_type) > 4) {
		if(temp = value >> 56) return 56 + lookup[temp];
		if(temp = value >> 48) return 48 + lookup[temp];
		if(temp = value >> 40) return 40 + lookup[temp];
		if(temp = value >> 32) return 32 + lookup[temp];
	}

	if(temp = value >> 24) return 24 + lookup[temp];
	if(temp = value >> 16) return 16 + lookup[temp];
	if(temp = value >> 8) return 8 + lookup[temp];
	
	return lookup[value];
}

#endif

template<typename _type>
inline constexpr typename std::enable_if<std::is_integral<_type>::value, std::size_t>::type
msb(_type value) {
	return std::size_t(1) << log(value);
}

template<typename _type>
inline constexpr typename std::enable_if<std::is_integral<_type>::value, std::size_t>::type
segment_size(_type start, _type end) {
	// Assume half-open range
	return msb((end - 1) ^ start) << 1;
}

} // namespace bit

} // namespace dst

#endif	