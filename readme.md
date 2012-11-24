Some simple benchmarks comparing the Clang vector extension, SSE intrinsics, and a plain vector class.

Some notes:
SSE4.1 is supported by Harpertown (2008 MacPro3,1) and Q6600. It seems a reasonable minimum requirement for modern high-performance code.

Header files for intrinsics:

*	`mmintrin.h`:  MMX
*	`xmmintrin.h`: SSE
*	`emmintrin.h`: SSE2
*	`pmmintrin.h`: SSE3
*	`tmmintrin.h`: SSSE3
*	`smmintrin.h`: SSE4.1
*	`nmmintrin.h`: SSE4.2
*	`ammintrin.h`: SSE4A
*	`wmmintrin.h`: AES
*	`immintrin.h`: AVX
