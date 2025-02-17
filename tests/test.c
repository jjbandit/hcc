
#include "../libhccstd/core.h"
#include "../libc-gpu/math.h"

typedef _Bool Bool;
typedef uint32_t U32;
typedef int32_t S32;
typedef float F32;
typedef vec2f Vec2;
typedef vec3f Vec3;
typedef vec4f Vec4;

// C = A /+*

// #define M_SUB(C) (C - 0.5f)

// A = M_SUB(A /+* )

// #define M_NEG(A) (-A)

// C = M_NEG(M_SUB(A /+* )) + 1

// #define M_SUB(C) (C - 0.5f)

#define ZERO_POINT_FIVE 0.5f

#define M_ADD(D) (D + ZERO_POINT_FIVE)

#define M_SUB(C) (C - ZERO_POINT_FIVE)

#define M_NEG(A) (-A)

#define M_SUPER(A, B, C, D) (M_NEG(M_NEG(M_ADD(A) * M_SUB(M_ADD(B)))) + M_ADD(C) + M_SUB(D))

#define TEST(A, B, ...) v4f(M_SUB(M_SUPER(A, B, A, B)), M_ADD(M_SUB(M_SUPER(1 + __VA_ARGS__, __VA_ARGS__ + 1))), __VA_ARGS__)

#define G(H) H
#define A(D) G(D) + D
#define B(E) G(A(G(E)))
#define C(F) A(G(B(F)))

#define TEST2(A) C(A)

#define TEST3(A, ...) A; __VA_ARGS__

#define HASH_HASH # ## #
#define MKSTR(a) # a
#define IN_BETWEEN(a) MKSTR(a)
#define JOIN(c, d) IN_BETWEEN(c HASH_HASH d)

#define IDENTIFIER test

#define ENENE_(a, b) a ## b

#define ENENE(a, b) ENENE_(a, b)

#ifdef COND
#ifdef COND2
#define COND_TEST 1.f
#else
#define COND_TEST 0.75f
#endif
#else
#ifdef COND2
#define COND_TEST 0.25f
#else
#define COND_TEST 0.f
#endif
#endif

#include "test.h"

#define COND(A) A * A

#define INDIRECTION defined(TEST_H)

#if INDIRECTION
#define COND_TEST1 1.f
#elif 0
#define COND_TEST1 0.5f
#else
#define COND_TEST1 0.f
#endif

#ifdef COND
#define COND_TEST2 1.f
#else
#define COND_TEST2 0.f
#endif

HCC_DEFINE_RASTERIZER_STATE(
	BillboardRasterizerState,
	(POSITION, Vec4, position),
	(INTERP,   Vec4, color),
	(NOINTERP, Vec4, flat_color),
	(NOINTERP, U32,  tri_idx)
);

vertex BillboardRasterizerState billboard_shader_vertex(const HccVertexInput input) {
	Vec4 vertices[6] = {
		v4f(-1.f, -1.f, 0.25f, 1.f),
		v4f( 1.f, -1.f, 0.25f, 1.f),
		v4f( 1.f,  1.f, 0.25f, 1.f),

		v4f( 1.f,  1.f, 0.25f, 1.f),
		v4f(-1.f,  1.f, 0.25f, 1.f),
		v4f(-1.f, -1.f, 0.25f, 1.f),
	};

	Vec4 colors[3] = {
		v4f(1.f, 0.f, 0.f, 1.f),
		v4f(0.f, 1.f, 0.f, 1.f),
		v4f(0.f, 0.f, 1.f, 1.f),
	};

	BillboardRasterizerState state;
	state.position = vertices[input.vertex_idx];
	state.color = colors[input.vertex_idx % 3];
	state.flat_color = v4f(1.f, 0.f, 1.f, 1.f);
	state.tri_idx = input.vertex_idx / 3;
	return state;
}

typedef enum NamedEnum TypedefNamedEnum;
enum NamedEnum {
	NAMED_ENUM_VALUE,
};

enum {
	ZERO,
	ONE,
	TWO = 2,
	THREE,
	TEN = 10,
	ELEVEN,

	MINUS_FIVE = -5,
	MINUS_FOUR,
};

U32 global = 1;

struct {
	U32 omg;
} global_struct;

U32 add_u32(U32 a, U32 b) {
	return a + b;
}

inline U32 inlined_add_u32(U32 a, U32 b) {
	return a + b;
}

typedef struct AddOp AddOp;
struct AddOp {
	U32 a;
	U32 b;
};

U32 add_op_u32(AddOp op) {
	return op.a + op.b;
}

HCC_DEFINE_FRAGMENT_STATE(
	BillboardFragment,
	(Vec4, color)
);

U32 g_multiple, g_var;
U32 g_multiple_ass = 1, g_var_ign = 2;

typedef S32 signed_int;
typedef struct Named { S32 i[1]; } named_wrapped_signed_int;
typedef struct { F32 i[2]; } wrapped_float2;
typedef struct { F32 i[4]; } wrapped_float4;
fragment BillboardFragment billboard_shader_fragment(const HccFragmentInput input, const BillboardRasterizerState state) {
	typedef struct Struct TypedefStruct;
	struct Struct {
		U32 a;
		struct {
			F32 k;
			S32 d;
		};
		union {
			U32 k;
			wrapped_float2 d;
		} named;
		union {
			U32 another;
			struct {
				U32 something;
				U32 something_else;
				union {
					wrapped_float2 f2[22];
					wrapped_float4 f4[11];
				};
			};
		};
	};

	struct Struct test;
	U32 nnnn;
	TypedefStruct typedef_test;
	struct Inline { S32 a; struct Struct b[3]; } testa;
	struct Inline reuse;

	struct EnumInStruct {
		enum EnumInStruct {
			VALUE_IN_STRUCT,
		} enum_in_struct;
		U32 t;
	};

	TypedefStruct board[4][4] = {
		[0][1].named.k = 7u,

		[0][2] = {
			22u,
			.named = { 88u }
		},

		[1] = {
			{
				22u
			},
			[1].something_else = 12u,
			[2].f2 = { [12].i[1] = 34.f },
			[3] = { .another = 299u }
		}
	};

	TypedefStruct st = {
		66u,
		{
			.k = 44.f,
			.d = -22,
		},
		{
			77u,
			.d = { .i = { [1] = 33.f, [0] = 99.f } }
		},
		.something_else = 123u,
		.f2 = { { .i = { 123.f } }, [1] = { { 12.f } } },
		.f2[0].i[1] = 512.f
	};

	st = (TypedefStruct){ 12u };

	enum Named named_enum = NAMED_ENUM_VALUE;
	named_enum = 7;

	enum EnumInStruct enum_in_struct = VALUE_IN_STRUCT;

	struct EnumInStruct wrapped_enum_in_struct;
	wrapped_enum_in_struct.enum_in_struct = VALUE_IN_STRUCT;

	test.f2[0].i[0] = 1.f;
	test.f2[1].i[1] = 0.3f;

	F32 red = test.f4[0].i[0];
	F32 blue = test.f4[0].i[3];

	static const F32 static_red = 0.5f;

	Bool bool_ = 79798789;
	U32 uint = 0;
	S32 sint = 0;
	F32 float_ = 0;

	bool_ = uint;
	bool_ = uint;
	bool_ = sint;
	bool_ = float_;

	uint = uint;
	uint = bool_;
	uint = sint;
	uint = float_;

	sint = bool_;
	sint = uint;
	sint = float_;

	float_ = bool_;
	float_ = uint;
	float_ = sint;

	uint = uint * bool_ ? sint ? sint - 1 : sint + 1 : uint;

	uint = 0;
	while (uint < 0) {
		uint += 1;
	}
	red = uint == 0;

	uint = 0; do {
		uint += 1;
	} while (uint < 0);
	blue = uint == 1;

	uint = 0;
	red = ++uint == 1;

	uint = 0;
	blue = uint++ == 0;

	F32 green = uint == 1;

	uint = 2;
	red = --uint == 1;

	uint = 2;
	blue = uint-- == 2;

	green = uint == 1;

	Vec4 vec = v4f(1.f, 0.f, 0.f, 1.f);
	red = vec.x == 1.f;

	vec.xy = v2f(0.f, 1.f);
	blue = vec.r == 0.f && vec.g == 1.f;

	float t = ZERO_POINT_FIVE + TEST2(ZERO_POINT_FIVE);

	TEST3(ZERO_POINT_FIVE);

	AddOp add_op = { 2, 2 };
	red = add_op_u32(add_op) == 4 && inlined_add_u32(1, 1) == 2;

	red = copysignf(2.f, -1.f) <= -2.f;
	red = sinf((float)state.tri_idx) <= 1.f;
	red = isinf(INFINITY);
	red = isnan(NAN);

	U32 multiple, var;
	U32 multiple_ass = 1, var_ign = 2;

	BillboardFragment frag;
	frag.color = state.tri_idx ? state.flat_color : state.color;
	//frag.color = v4f(red, 0.f, 0.f, 1.f);
	return frag;
}

