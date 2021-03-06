Some simple benchmarks comparing the Clang vector extension, SSE intrinsics, and a plain vector class for single-precision float vectors.

# Summary
The extension and intrinsics tests use 4 component vectors for better mapping to 128-bit SSE registers (loading/storing 4 floats requires one instruction, 3 requires two separate loads and some shuffling).

The structs, and arrays where implemented, have 3D and 4D versions selected by a preprocessor define. The 4D version more accurately represents the relative capabilities in general computation, the 3D version is more representative for 3D geometry, physics, and graphics.


Vector class: class/struct of scalars.

* Slowest. Even when wrapping a primitive vector and using intrinsics, there is a performance penalty, probably because primitive vectors can be passed through SSE registers.
* Platform and compiler independent.
* Can use 3-component vectors.

Array of scalars:

* Basically indistinguishable in performance from the vector class.
* Less convenient to work with.

Vector extension: OpenCL-style vector types in C/C++.

* Non-standard and compiler dependent (though Clang and GCC have similar extensions).
* Processor-independent.
* Convenient member access and composition syntax.
* Mixes well with intrinsics.
* Often comes close to or equals code using intrinsics in performance.

Intrinsics:

* Generally fastest, but sometimes by little or no margin over the vector extension.
* Use more limited non-standard language extensions: just 128-bit types.
* Processor-specific.
* Ugly code.

# Timings:

On my dual quad-core Mac Pro, clang version 3.2 (trunk 167613):

	Math tests

	Struct: 0.734957 s
	Array: 0.734050 s
	Extension: 0.479301 s
	Intrinsics: 0.480404 s


	Ray tracing tests

	Structs: 0.989610 s
	Extension: 0.756928 s
	Intrinsics: 0.677218 s
	Rays: 0.846294 s
	Ray+obj: 0.802400 s


	Particle system tests

	Structs: 0.865972 s
	extension: 0.570777 s
	intrinsics: 0.570669 s


# Notes:
SSE4.1 is supported by Harpertown (2008 MacPro3,1) and Q6600. It seems a reasonable minimum requirement for modern high-performance code.

http://sseplus.sourceforge.net/fntable.html

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
