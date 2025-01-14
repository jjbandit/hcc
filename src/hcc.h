#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <setjmp.h>

typedef struct HccCompiler HccCompiler;
typedef struct HccCompilerSetup HccCompilerSetup;
typedef struct HccCodeFile HccCodeFile;
typedef struct HccPPIfSpan HccPPIfSpan;
typedef struct HccPPMacro HccPPMacro;

// ===========================================
//
//
// General
//
//
// ===========================================

typedef uint8_t   U8;
typedef uint16_t  U16;
typedef uint32_t  U32;
typedef uint64_t  U64;
typedef uintptr_t Uptr;
typedef int8_t    S8;
typedef int16_t   S16;
typedef int32_t   S32;
typedef int64_t   S64;
typedef intptr_t  Sptr;
typedef float     F32;
typedef double    F64;

#define U8_MAX  UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define S8_MIN  INT8_MIN
#define S8_MAX  INT8_MAX
#define S16_MIN INT16_MIN
#define S16_MAX INT16_MAX
#define S32_MIN INT32_MIN
#define S32_MAX INT32_MAX
#define S64_MIN INT64_MIN
#define S64_MAX INT64_MAX

typedef U16 HccAllocTag;
enum {
	HCC_ALLOC_TAG_NONE,

	HCC_ALLOC_TAG_MESSAGES,
	HCC_ALLOC_TAG_MESSAGE_STRINGS,
	HCC_ALLOC_TAG_STRING_TABLE_DATA,
	HCC_ALLOC_TAG_STRING_TABLE_ENTRIES,
	HCC_ALLOC_TAG_CONSTANT_TABLE_DATA,
	HCC_ALLOC_TAG_CONSTANT_TABLE_ENTRIES,
	HCC_ALLOC_TAG_STRING_BUFFER,
	HCC_ALLOC_TAG_INCLUDE_PATHS,
	HCC_ALLOC_TAG_CODE_FILES,
	HCC_ALLOC_TAG_PATH_TO_CODE_FILE_ID_MAP,
	HCC_ALLOC_TAG_CODE_FILE_LINE_CODE_START_INDICES,
	HCC_ALLOC_TAG_CODE_FILE_PP_IF_SPANS,
	HCC_ALLOC_TAG_CODE,

	HCC_ALLOC_TAG_PP_TOKENS,
	HCC_ALLOC_TAG_PP_TOKEN_LOCATION_INDICES,
	HCC_ALLOC_TAG_PP_TOKEN_VALUES,
	HCC_ALLOC_TAG_PP_MACROS,
	HCC_ALLOC_TAG_PP_MACRO_PARAMS,
	HCC_ALLOC_TAG_PP_MACRO_ARGS_STACK,
	HCC_ALLOC_TAG_PP_EXPAND_STACK,
	HCC_ALLOC_TAG_PP_STRINGIFY_BUFFER,
	HCC_ALLOC_TAG_PP_IF_SPAN_STACK,
	HCC_ALLOC_TAG_PP_MACRO_DECLARATIONS,

	HCC_ALLOC_TAG_TOKENGEN_TOKENS,
	HCC_ALLOC_TAG_TOKENGEN_TOKEN_LOCATION_INDICES,
	HCC_ALLOC_TAG_TOKENGEN_TOKEN_VALUES,
	HCC_ALLOC_TAG_TOKENGEN_TOKEN_LOCATIONS,
	HCC_ALLOC_TAG_TOKENGEN_PAUSED_FILE_STACK,
	HCC_ALLOC_TAG_TOKENGEN_OPEN_BRACKET_STACK,

	HCC_ALLOC_TAG_ASTGEN_FUNCTION_PARAMS_AND_VARIABLES,
	HCC_ALLOC_TAG_ASTGEN_FUNCTIONS,
	HCC_ALLOC_TAG_ASTGEN_EXPRS,
	HCC_ALLOC_TAG_ASTGEN_EXPR_LOCATIONS,
	HCC_ALLOC_TAG_ASTGEN_GLOBAL_VARIABLES,
	HCC_ALLOC_TAG_ASTGEN_FUNCTION_USED_FUNCTION_INDICES,
	HCC_ALLOC_TAG_ASTGEN_FUNCTION_USED_STATIC_VARIABLES,
	HCC_ALLOC_TAG_ASTGEN_ENTRY_POINT_FUNCTION_INDICES,
	HCC_ALLOC_TAG_ASTGEN_USED_FUNCTION_INDICES,
	HCC_ALLOC_TAG_ASTGEN_RECURSIVE_FUNCTION_INDICES_STACK,
	HCC_ALLOC_TAG_ASTGEN_ARRAY_DATA_TYPES,
	HCC_ALLOC_TAG_ASTGEN_COMPOUND_DATA_TYPES,
	HCC_ALLOC_TAG_ASTGEN_COMPOUND_FIELDS,
	HCC_ALLOC_TAG_ASTGEN_TYPEDEFS,
	HCC_ALLOC_TAG_ASTGEN_ENUM_DATA_TYPES,
	HCC_ALLOC_TAG_ASTGEN_ENUM_VALUES,
	HCC_ALLOC_TAG_ASTGEN_ORDERED_DATA_TYPES,
	HCC_ALLOC_TAG_ASTGEN_GLOBAL_DECLARATIONS,
	HCC_ALLOC_TAG_ASTGEN_STRUCT_DECLARATIONS,
	HCC_ALLOC_TAG_ASTGEN_UNION_DECLARATIONS,
	HCC_ALLOC_TAG_ASTGEN_ENUM_DECLARATIONS,
	HCC_ALLOC_TAG_ASTGEN_COMPOUND_TYPE_FIND_FIELDS,
	HCC_ALLOC_TAG_ASTGEN_CURLY_INITIALIZER_NESTED_CURLYS,
	HCC_ALLOC_TAG_ASTGEN_CURLY_INITIALIZER_NESTED_ELMTS,
	HCC_ALLOC_TAG_ASTGEN_CURLY_INITIALIZER_DESIGNATOR_INITIALIZERS,
	HCC_ALLOC_TAG_ASTGEN_CURLY_INITIALIZER_DESIGNATOR_INITIALIZER_ELMT_INDICES,
	HCC_ALLOC_TAG_ASTGEN_VARIABLE_STACK_STRINGS,
	HCC_ALLOC_TAG_ASTGEN_VARIABLE_STACK_VAR_INDICES,
	HCC_ALLOC_TAG_ASTGEN_FIELD_NAME_TO_TOKEN_IDX,

	HCC_ALLOC_TAG_IRGEN_FUNCTIONS,
	HCC_ALLOC_TAG_IRGEN_BASIC_BLOCKS,
	HCC_ALLOC_TAG_IRGEN_VALUES,
	HCC_ALLOC_TAG_IRGEN_INSTRUCTIONS,
	HCC_ALLOC_TAG_IRGEN_OPERANDS,
	HCC_ALLOC_TAG_IRGEN_FUNCTION_CALL_PARAM_DATA_TYPES,

	HCC_ALLOC_TAG_SPIRVGEN_TYPE_TABLE_DATA_TYPES,
	HCC_ALLOC_TAG_SPIRVGEN_TYPE_TABLE_ENTRIES,
	HCC_ALLOC_TAG_SPIRVGEN_OUT_CAPABILITIES,
	HCC_ALLOC_TAG_SPIRVGEN_OUT_ENTRY_POINTS,
	HCC_ALLOC_TAG_SPIRVGEN_OUT_DEBUG_INFO,
	HCC_ALLOC_TAG_SPIRVGEN_OUT_ANNOTATIONS,
	HCC_ALLOC_TAG_SPIRVGEN_OUT_TYPES_VARIABLES_CONSTANTS,
	HCC_ALLOC_TAG_SPIRVGEN_OUT_FUNCTIONS,

	HCC_ALLOC_TAG_COUNT,
};

const char* hcc_alloc_tag_strings[HCC_ALLOC_TAG_COUNT];

#ifndef HCC_ALLOC
#define HCC_ALLOC(tag, size, align) malloc(size)
#endif

#ifndef HCC_DEALLOC
#define HCC_DEALLOC(tag, ptr, size, align) (void)tag, free(ptr)
#endif

#define HCC_ALLOC_ELMT(T, tag) HCC_ALLOC(tag, sizeof(T), alignof(T))
#define HCC_DEALLOC_ELMT(T, tag, ptr) HCC_DEALLOC(tag, ptr, sizeof(T), alignof(T))
#define HCC_ALLOC_ARRAY(T, tag, count) HCC_ALLOC(tag, (count) * sizeof(T), alignof(T))
#define HCC_DEALLOC_ARRAY(T, tag, ptr, count) HCC_DEALLOC(tag, ptr, (count) * sizeof(T), alignof(T))

#ifndef static_assert
#define static_assert _Static_assert
#endif

#ifndef noreturn
#define noreturn _Noreturn
#endif

#ifndef HCC_ABORT
#define HCC_ABORT(...) _hcc_abort(__FILE__, __LINE__, __VA_ARGS__)
#endif

#ifndef HCC_ASSERT
#define HCC_ASSERT(cond, ...) if (HCC_UNLIKELY(!(cond))) _hcc_assert_failed(#cond, __FILE__, __LINE__, __VA_ARGS__)
#endif

#ifndef HCC_DEBUG_ASSERTIONS
#define HCC_DEBUG_ASSERTIONS 1
#endif

#define HCC_STRINGIFY(v) #v
#define HCC_CONCAT_0(a, b) a##b
#define HCC_CONCAT(a, b) HCC_CONCAT_0(a, b)

#if HCC_DEBUG_ASSERTIONS
#define HCC_DEBUG_ASSERT HCC_ASSERT
#else
#define HCC_DEBUG_ASSERT(cond, ...) (void)(cond)
#endif

#define HCC_DEBUG_ASSERT_NON_ZERO(value) HCC_DEBUG_ASSERT(value, "'%s' must be a non-zero value", #value)
#define HCC_DEBUG_ASSERT_ARRAY_BOUNDS(idx, count) (((idx) < (count)) ? (idx) : HCC_ABORT("idx '%zu' is out of bounds for an array of count '%zu'", (idx), (count)))
#define HCC_DEBUG_ASSERT_ARRAY_RESIZE(count, cap) HCC_DEBUG_ASSERT((count) <= (cap), "cannot resize array to count '%zu' when it has a capacity of '%zu'", (count), (cap))

#ifdef __GNUC__
#define HCC_LIKELY(expr) __builtin_expect((expr), 1)
#define HCC_UNLIKELY(expr) __builtin_expect((expr), 0)
#else
#define HCC_LIKELY(expr) expr
#define HCC_UNLIKELY(expr) expr
#endif

#if HCC_DEBUG_ASSERTIONS
#define HCC_UNREACHABLE(...) HCC_ABORT("unreachable code: " __VA_ARGS__);
#else
#define HCC_UNREACHABLE(...) __builtin_unreachable()
#endif

#define HCC_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define HCC_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define HCC_UNUSED(expr) ((void)(expr))

#ifndef alignof
#define alignof _Alignof
#endif

void _hcc_assert_failed(const char* cond, const char* file, int line, const char* message, ...);
noreturn Uptr _hcc_abort(const char* file, int line, const char* message, ...);

// align must be a power of 2
#define HCC_INT_ROUND_UP_ALIGN(i, align) (((i) + ((align) - 1)) & ~((align) - 1))
// align must be a power of 2
#define HCC_INT_ROUND_DOWN_ALIGN(i, align) ((i) & ~((align) - 1))

#define HCC_ARRAY_COUNT(array) (sizeof(array) / sizeof(*(array)))
#define HCC_IS_POWER_OF_TWO_OR_ZERO(v) ((((v) & ((v) - 1)) == 0))
#define HCC_IS_POWER_OF_TWO(v) (((v) != 0) && (((v) & ((v) - 1)) == 0))
#define HCC_PTR_ADD(ptr, by) (void*)((Uptr)(ptr) + (Uptr)(by))
#define HCC_PTR_SUB(ptr, by) (void*)((Uptr)(ptr) - (Uptr)(by))
#define HCC_PTR_DIFF(to, from) ((char*)(to) - (char*)(from))
// align must be a power of 2
#define HCC_PTR_ROUND_UP_ALIGN(ptr, align) ((void*)HCC_INT_ROUND_UP_ALIGN((Uptr)(ptr), align))
// align must be a power of 2
#define HCC_PTR_ROUND_DOWN_ALIGN(ptr, align) ((void*)HCC_INT_ROUND_DOWN_ALIGN((Uptr)(ptr), align))
#define HCC_ZERO_ELMT(ptr) memset(ptr, 0, sizeof(*(ptr)))
#define HCC_ONE_ELMT(ptr) memset(ptr, 0xff, sizeof(*(ptr)))
#define HCC_ZERO_ELMT_MANY(ptr, elmts_count) memset(ptr, 0, sizeof(*(ptr)) * (elmts_count))
#define HCC_ONE_ELMT_MANY(ptr, elmts_count) memset(ptr, 0xff, sizeof(*(ptr)) * (elmts_count))
#define HCC_ZERO_ARRAY(array) memset(array, 0, sizeof(array))
#define HCC_ONE_ARRAY(array) memset(array, 0xff, sizeof(array))
#define HCC_COPY_ARRAY(dst, src) memcpy(dst, src, sizeof(dst))
#define HCC_COPY_ELMT_MANY(dst, src, elmts_count) memcpy(dst, src, elmts_count * sizeof(*(dst)))
#define HCC_COPY_OVERLAP_ELMT_MANY(dst, src, elmts_count) memmove(dst, src, elmts_count * sizeof(*(dst)))
#define HCC_CMP_ARRAY(a, b) (memcmp(a, b, sizeof(a)) == 0)
#define HCC_CMP_ELMT(a, b) (memcmp(a, b, sizeof(*(a))) == 0)
#define HCC_CMP_ELMT_MANY(a, b, elmts_count) (memcmp(a, b, elmts_count * sizeof(*(a))) == 0)

#define HCC_DIV_ROUND_UP(a, b) (((a) / (b)) + ((a) % (b) != 0))

#define HCC_LEAST_SET_BIT_IDX_U32(bitset) __builtin_ctz(bitset)
#define HCC_LEAST_SET_BIT_REMOVE(bitset) ((bitset) & ((bitset) - 1))

#define HCC_DEFINE_ID(Name) typedef struct Name { uint32_t idx_plus_one; } Name
HCC_DEFINE_ID(HccCodeFileId);
HCC_DEFINE_ID(HccStringId);
HCC_DEFINE_ID(HccConstantId);
HCC_DEFINE_ID(HccFunctionTypeId);
HCC_DEFINE_ID(HccExprId);

static inline bool hcc_bitset64_is_set(U64* bitset, U32 idx) {
	U64 bit = (U64)1 << (idx & 63);
	return (bitset[idx >> 6] & bit) == bit;
}

static inline void hcc_bitset64_set(U64* bitset, U32 idx) {
	U64 bit = (U64)1 << (idx & 63);
	bitset[idx >> 6] |= bit;
}

static inline void hcc_bitset64_unset(U64* bitset, U32 idx) {
	bitset[idx >> 6] &= ~((U64)1 << (idx & 63));
}

#define HCC_COMPOUND_TYPE_NESTED_FIELD_CAP 16

static inline bool hcc_u64_checked_add(uint64_t a, uint64_t b, uint64_t* out) {
	if (b > (UINT64_MAX - a)) { return false; }
	*out = a + b;
	return true;
}

static inline bool hcc_s64_checked_add(int64_t a, int64_t b, int64_t* out) {
	if (a >= 0) {
		if (b > (INT64_MAX - a)) { return false; }
	} else {
		if (b < (INT64_MIN - a)) { return false; }
	}

	*out = a + b;
	return true;
}

static inline bool hcc_i64_checked_mul(uint64_t a, uint64_t b, uint64_t* out) {
	uint64_t r = a * b;
	if (a != 0 && b != 0 && a != r / b) {
		return false;
	}
	*out = r;
	return true;
}

static inline bool hcc_ascii_is_alpha(U32 byte) {
	return ((byte | 32u) - 97u) < 26u;
}

static inline bool hcc_ascii_is_digit(U32 byte) {
	return (byte - 16) <= 10;
}

static inline bool hcc_ascii_is_a_to_f(U32 byte) {
	return ((byte | 32u) - 97u) < 6u;
}

// ===========================================
//
//
// Platform Abstraction
//
//
// ===========================================

typedef U8 HccArch;
enum {
	HCC_ARCH_X86_64,

	HCC_ARCH_COUNT,
};

typedef U8 HccOS;
enum {
	HCC_OS_LINUX,
	HCC_OS_WINDOWS,

	HCC_OS_COUNT,
};

U32 onebitscount32(U32 bits);
U32 leastsetbitidx32(U32 bits);
void hcc_get_last_system_error_string(char* buf_out, U32 buf_out_size);
bool hcc_file_exist(char* path);
bool hcc_change_working_directory(char* path);
bool hcc_change_working_directory_to_same_as_this_file(char* path);
char* hcc_path_canonicalize(char* path);
bool hcc_path_is_absolute(char* path);
char* hcc_file_read_all_the_codes(char* path, U64* size_out);

// ===========================================
//
//
// String
//
//
// ===========================================

typedef struct HccString HccString;
struct HccString {
	char* data;
	uintptr_t size;
};

#define hcc_string(data, size) ((HccString) { data, size })
#define hcc_string_lit(lit) ((HccString) { lit, sizeof(lit) - 1 })
#define hcc_string_c(string) ((HccString) { string, strlen(string) })
#define hcc_string_c_path(string) ((HccString) { string, strlen(string) + 1 })
#define hcc_string_eq(a, b) ((a).size == (b).size && memcmp((a).data, (b).data, (a).size) == 0)
#define hcc_string_eq_c(a, c_string) ((a).size == strlen(c_string) && memcmp((a).data, c_string, (a).size) == 0)
#define hcc_string_eq_lit(a, lit) ((a).size == sizeof(lit) - 1 && memcmp((a).data, lit, (a).size) == 0)
static inline HccString hcc_string_slice_start(HccString string, Uptr start) {
	HCC_DEBUG_ASSERT_ARRAY_BOUNDS(start, string.size + 1);
	return hcc_string(string.data + start, string.size - start);
}
static inline HccString hcc_string_slice_end(HccString string, Uptr end) {
	HCC_DEBUG_ASSERT_ARRAY_BOUNDS(end, string.size + 1);
	return hcc_string(string.data, end);
}
static inline HccString hcc_string_slice(HccString string, Uptr start, Uptr end) {
	HCC_DEBUG_ASSERT_ARRAY_BOUNDS(start, string.size + 1);
	HCC_DEBUG_ASSERT_ARRAY_BOUNDS(end, string.size + 1);
	HCC_DEBUG_ASSERT(start <= end, "start of '%zu' must be less than end of '%zu'", start, end);
	return hcc_string(string.data + start, end - start);
}
#define hcc_string_find_lit(string, lit) hcc_string_find(string, lit, sizeof(lit) - 1)
static inline U32 hcc_string_find(HccString haystack, char* needle, U32 needle_size) {
	if (needle_size == 0 || haystack.size < needle_size) {
		return -1;
	}

	for (U32 idx = 0; idx <= haystack.size - needle_size;) {
		if (haystack.data[idx] == needle[0]) {
			bool is_match = true;
			U32 cmp_idx = 0;
			for (; cmp_idx < needle_size; cmp_idx += 1) {
				if (haystack.data[idx + cmp_idx] != needle[cmp_idx]) {
					is_match = false;
					break;
				}
			}
			if (is_match) {
				return idx;
			}
			idx += cmp_idx;
		} else {
			idx += 1;
		}
	}

	return -1;
}

// ===========================================
//
//
// Stack
//
//
// ===========================================

#define HCC_STACK_MAGIC_NUMBER 0x57ac4c71

typedef struct HccStackHeader HccStackHeader;
struct HccStackHeader {
	Uptr count;
	Uptr cap;
	HccAllocTag tag;
#if HCC_DEBUG_ASSERTIONS
	U32  magic_number;
	Uptr elmt_size;
#endif
};

#define HccStack(T) T*

#define hcc_stack_header(stack)  ((stack) ? (((HccStackHeader*)(stack)) - 1)   : NULL)
#define hcc_stack_count(stack)   ((stack) ? hcc_stack_header(stack)->count     : 0)
#define hcc_stack_cap(stack)     ((stack) ? hcc_stack_header(stack)->cap       : 0)
#define hcc_stack_clear(stack)   ((stack)  ? hcc_stack_header(stack)->count = 0 : 0)
#define hcc_stack_is_full(stack) (hcc_stack_count(stack) == hcc_stack_cap(stack))

#define hcc_stack_init(T, cap, tag) ((HccStack(T))_hcc_stack_init(cap, tag, sizeof(T)))
HccStack(void) _hcc_stack_init(Uptr cap, HccAllocTag tag, Uptr elmt_size);

#define hcc_stack_deinit(stack, tag) _hcc_stack_deinit(stack, tag, sizeof(*(stack))); (stack) = NULL
void _hcc_stack_deinit(HccStack(void) stack, HccAllocTag tag, Uptr elmt_size);

#if HCC_DEBUG_ASSERTIONS
#define hcc_stack_get(stack, idx) (&(stack)[HCC_DEBUG_ASSERT_ARRAY_BOUNDS(idx, hcc_stack_count(stack))])
#else
#define hcc_stack_get(stack, idx) (&(stack)[idx])
#endif
#define hcc_stack_get_first(stack) hcc_stack_get(stack, 0)
#define hcc_stack_get_last(stack) hcc_stack_get(stack, hcc_stack_count(stack) - 1)
#define hcc_stack_get_back(stack, back_idx) hcc_stack_get(stack, hcc_stack_count(stack) - (back_idx) - 1)
#define hcc_stack_get_or_null(stack, idx) ((idx) < hcc_stack_count(stack) ? &(stack)[idx] : NULL)
#define hcc_stack_get_next_push(stack) (&(stack)[hcc_stack_count(stack)])

#define hcc_stack_resize(stack, new_count) _hcc_stack_resize(stack, new_count, sizeof(*(stack)))
Uptr _hcc_stack_resize(HccStack(void) stack, Uptr new_count, Uptr elmt_size);

#define hcc_stack_insert(stack, idx) _hcc_stack_insert_many(stack, idx, 1, sizeof(*(stack)))
#define hcc_stack_insert_many(stack, idx, amount) _hcc_stack_insert_many(stack, idx, amount, sizeof(*(stack)))
void* _hcc_stack_insert_many(HccStack(void) stack, Uptr idx, Uptr amount, Uptr elmt_size);

#define hcc_stack_push(stack) (&(stack)[_hcc_stack_push_many(stack, 1, sizeof(*(stack)))])
#define hcc_stack_push_many(stack, amount) (&(stack)[_hcc_stack_push_many(stack, amount, sizeof(*(stack)))])
Uptr _hcc_stack_push_many(HccStack(void) stack, Uptr amount, Uptr elmt_size);

#define hcc_stack_pop(stack) _hcc_stack_pop_many(stack, 1, sizeof(*(stack)))
#define hcc_stack_pop_many(stack, amount) _hcc_stack_pop_many(stack, amount, sizeof(*(stack)))
void _hcc_stack_pop_many(HccStack(void) stack, Uptr amount, Uptr elmt_size);

#define hcc_stack_remove_swap(stack, idx) _hcc_stack_remove_swap_many(stack, idx, 1, sizeof(*(stack)))
#define hcc_stack_remove_swap_many(stack, idx, count) _hcc_stack_remove_swap_many(stack, idx, count, sizeof(*(stack)))
void _hcc_stack_remove_swap_many(HccStack(void) stack, Uptr idx, Uptr amount, Uptr elmt_size);

#define hcc_stack_remove_shift(stack, idx) _hcc_stack_remove_shift_many(stack, idx, 1, sizeof(*(stack)))
#define hcc_stack_remove_shift_many(stack, idx, count) _hcc_stack_remove_shift_many(stack, idx, count, sizeof(*(stack)))
void _hcc_stack_remove_shift_many(HccStack(void) stack, Uptr idx, Uptr amount, Uptr elmt_size);

void hcc_stack_push_char(HccStack(char) stack, char ch);
HccString hcc_stack_push_string(HccStack(char) stack, HccString string);
HccString hcc_stack_push_string_fmtv(HccStack(char) stack, char* fmt, va_list args);
#ifdef __GNUC__
HccString hcc_stack_push_string_fmt(HccStack(char) stack, char* fmt, ...) __attribute__ ((format (printf, 2, 3)));
#else
HccString hcc_stack_push_string_fmt(HccStack(char) stack, char* fmt, ...);
#endif

// ===========================================
//
//
// Hashing
//
//
// ===========================================

#define HCC_FNV_HASH_32_INITIAL 0x811c9dc5
U32 hcc_fnv_hash_32(char* bytes, U32 byte_count, U32 hash);
void hcc_generate_enum_hashes(char* array_name, char** strings, char** enum_strings, U32 enums_count);
void hcc_generate_hashes();
U32 hcc_string_to_enum_hashed_find(HccString string, U32* enum_hashes, U32 enums_count);

// ===========================================
//
//
// Hash Table
//
//
// ===========================================

#define HccHashTable(Key, Value) HccHashTable

typedef struct HccHashTable HccHashTable;
struct HccHashTable {
	U32* keys;
	U32* values;
	U32 count;
	U32 cap;
};

void hcc_hash_table_init(HccHashTable* hash_table, U32 cap, HccAllocTag tag);
bool hcc_hash_table_find(HccHashTable* hash_table, U32 key, U32* value_out);
bool hcc_hash_table_find_or_insert(HccHashTable* hash_table, U32 key, U32** value_ptr_out);
bool hcc_hash_table_remove(HccHashTable* hash_table, U32 key, U32* value_out);
void hcc_hash_table_clear(HccHashTable* hash_table);

// ===========================================
//
//
// Message: Error & Warn
//
//
// ===========================================

typedef U8 HccLang;
enum {
	HCC_LANG_ENG,

	HCC_LANG_COUNT,
};

typedef U32 HccMessageCode;
typedef U8 HccMessageType;
enum {
	HCC_MESSAGE_TYPE_ERROR = 0x1,
	HCC_MESSAGE_TYPE_WARN = 0x2,

	HCC_MESSAGE_TYPE_COUNT,
};

typedef HccMessageCode HccErrorCode;
enum {
	HCC_ERROR_CODE_NONE,

	//
	// tokengen
	HCC_ERROR_CODE_INVALID_TOKEN_MACRO_IDENTIFIER,
	HCC_ERROR_CODE_INVALID_TOKEN_MACRO_PARAM_IDENTIFIER,
	HCC_ERROR_CODE_DUPLICATE_MACRO_PARAM_IDENTIFIER,
	HCC_ERROR_CODE_INVALID_MACRO_PARAM_DELIMITER,
	HCC_ERROR_CODE_MACRO_PARAM_VA_ARG_NOT_LAST,
	HCC_ERROR_CODE_MACRO_ALREADY_DEFINED,
	HCC_ERROR_CODE_INVALID_INCLUDE_OPERAND,
	HCC_ERROR_CODE_TOO_MANY_INCLUDE_OPERANDS,
	HCC_ERROR_CODE_INCLUDE_PATH_IS_EMPTY,
	HCC_ERROR_CODE_INCLUDE_PATH_DOES_NOT_EXIST,
	HCC_ERROR_CODE_FAILED_TO_OPEN_FILE_FOR_READ,
	HCC_ERROR_CODE_CONDITION_HAS_NO_PP_TOKENS,
	HCC_ERROR_CODE_INVALID_PP_BINARY_OP,
	HCC_ERROR_CODE_INVALID_PP_UNARY_EXPR,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_CLOSE,
	HCC_ERROR_CODE_UNDEFINED_IDENTIFIER_IN_PP_EXPR,
	HCC_ERROR_CODE_EXPECTED_COLON_FOR_TERNARY_OP,
	HCC_ERROR_CODE_INVALID_PP_LINE_OPERANDS,
	HCC_ERROR_CODE_PP_LINE_MUST_BE_MORE_THAN_ZERO,
	HCC_ERROR_CODE_TOO_MANY_PP_LINE_OPERANDS,
	HCC_ERROR_CODE_PP_ERROR,
	HCC_ERROR_CODE_INVALID_PP_PRAGMA_OPERAND,
	HCC_ERROR_CODE_PP_PRAGMA_OPERAND_USED_IN_MAIN_FILE,
	HCC_ERROR_CODE_TOO_MANY_PP_PRAGMA_OPERANDS,
	HCC_ERROR_CODE_INVALID_TOKEN_PREPROCESSOR_DIRECTIVE,
	HCC_ERROR_CODE_INVALID_PREPROCESSOR_DIRECTIVE,
	HCC_ERROR_CODE_PP_ENDIF_BEFORE_IF,
	HCC_ERROR_CODE_PP_ELSEIF_CANNOT_FOLLOW_ELSE,
	HCC_ERROR_CODE_PP_IF_UNTERMINATED,
	HCC_ERROR_CODE_INVALID_PP_CONCAT_OPERANDS,
	HCC_ERROR_CODE_TOO_MANY_UNDEF_OPERANDS,
	HCC_ERROR_CODE_TOO_MANY_IFDEF_OPERANDS,
	HCC_ERROR_CODE_PP_DIRECTIVE_NOT_FIRST_ON_LINE,
	HCC_ERROR_CODE_INVALID_OCTAL_DIGIT,
	HCC_ERROR_CODE_MAX_UINT_OVERFLOW,
	HCC_ERROR_CODE_MAX_SINT_OVERFLOW,
	HCC_ERROR_CODE_MAX_SINT_OVERFLOW_DECIMAL,
	HCC_ERROR_CODE_MAX_FLOAT_OVERFLOW,
	HCC_ERROR_CODE_U_SUFFIX_ALREADY_USED,
	HCC_ERROR_CODE_U_SUFFIX_ON_FLOAT,
	HCC_ERROR_CODE_L_SUFFIX_ON_FLOAT,
	HCC_ERROR_CODE_LONG_DOUBLE_IS_UNSUPPORTED,
	HCC_ERROR_CODE_FLOAT_HAS_DOUBLE_FULL_STOP,
	HCC_ERROR_CODE_FLOAT_MUST_BE_DECIMAL,
	HCC_ERROR_CODE_FLOAT_SUFFIX_MUST_FOLLOW_DECIMAL_PLACE,
	HCC_ERROR_CODE_INVALID_INTEGER_LITERALS,
	HCC_ERROR_CODE_INVALID_FLOAT_LITERALS,
	HCC_ERROR_CODE_NO_BRACKETS_OPEN,
	HCC_ERROR_CODE_INVALID_CLOSE_BRACKET_PAIR,
	HCC_ERROR_CODE_UNCLOSED_STRING_LITERAL,
	HCC_ERROR_CODE_MACRO_STARTS_WITH_CONCAT,
	HCC_ERROR_CODE_STRINGIFY_MUST_BE_MACRO_PARAM,
	HCC_ERROR_CODE_INVALID_TOKEN,
	HCC_ERROR_CODE_INVALID_TOKEN_HASH_IN_PP_OPERAND,
	HCC_ERROR_CODE_EXPECTED_IDENTIFIER_PP_IF_DEFINED,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_CLOSE_DEFINED,
	HCC_ERROR_CODE_INVALID_USE_OF_VA_ARGS,
	HCC_ERROR_CODE_VA_ARGS_IN_MACRO_PARAMETER,
	HCC_ERROR_CODE_NOT_ENOUGH_MACRO_ARGUMENTS,
	HCC_ERROR_CODE_TOO_MANY_MACRO_ARGUMENTS,

	//
	// astgen
	HCC_ERROR_CODE_CANNOT_FIND_FIELD,
	HCC_ERROR_CODE_DUPLICATE_FIELD_IDENTIFIER,
	HCC_ERROR_CODE_INVALID_DATA_TYPE_FOR_CONDITION,
	HCC_ERROR_CODE_MISSING_SEMICOLON,
	HCC_ERROR_CODE_REDEFINITION_IDENTIFIER_GLOBAL,
	HCC_ERROR_CODE_EXPECTED_CURLY_OPEN_ENUM,
	HCC_ERROR_CODE_REIMPLEMENTATION,
	HCC_ERROR_CODE_EMPTY_ENUM,
	HCC_ERROR_CODE_EXPECTED_IDENTIFIER_ENUM_VALUE,
	HCC_ERROR_CODE_ENUM_VALUE_OVERFLOW,
	HCC_ERROR_CODE_ENUM_VALUE_INVALID_FORMAT,
	HCC_ERROR_CODE_ENUM_VALUE_INVALID_TERMINATOR_WITH_EXPLICIT_VALUE,
	HCC_ERROR_CODE_ENUM_VALUE_INVALID_TERMINATOR,
	HCC_ERROR_CODE_INTRINSIC_NOT_FOUND_FUNCTION,
	HCC_ERROR_CODE_INTRINSIC_NOT_FOUND_STRUCT,
	HCC_ERROR_CODE_INTRINSIC_NOT_FOUND_UNION,
	HCC_ERROR_CODE_INVALID_SPECIFIER_FOR_STRUCT,
	HCC_ERROR_CODE_INVALID_SPECIFIER_CONFIG_FOR_STRUCT,
	HCC_ERROR_CODE_NOT_AVAILABLE_FOR_UNION,
	HCC_ERROR_CODE_EXPECTED_CURLY_OPEN_COMPOUND_TYPE,
	HCC_ERROR_CODE_INVALID_SPECIFIER_FOR_STRUCT_FIELD,
	HCC_ERROR_CODE_INVALID_SPECIFIER_CONFIG_FOR_STRUCT_FIELD,
	HCC_ERROR_CODE_COMPOUND_FIELD_INVALID_TERMINATOR,
	HCC_ERROR_CODE_COMPOUND_FIELD_MISSING_NAME,
	HCC_ERROR_CODE_INTRINSIC_INVALID_COMPOUND_STRUCT_FIELDS_COUNT,
	HCC_ERROR_CODE_INTRINSIC_INVALID_COMPOUND_STRUCT_FIELD,
	HCC_ERROR_CODE_INTRINSIC_VECTOR_INVALID_SIZE_AND_ALIGN,
	HCC_ERROR_CODE_INTRINSIC_MATRIX_INVALID_SIZE_AND_ALIGN,
	HCC_ERROR_CODE_MISSING_RASTERIZER_STATE_SPECIFIER,
	HCC_ERROR_CODE_POSITION_ALREADY_SPECIFIED,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_OPEN_ALIGNAS,
	HCC_ERROR_CODE_INVALID_ALIGNAS_INT_CONSTANT,
	HCC_ERROR_CODE_INVALID_ALIGNAS_OPERAND,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_CLOSE_ALIGNAS,
	HCC_ERROR_CODE_ALIGNAS_REDUCES_ALIGNMENT,
	HCC_ERROR_CODE_UNSUPPORTED_RASTERIZER_STATE_DATA_TYPE,
	HCC_ERROR_CODE_UNSUPPORTED_FRAGMENT_STATE_DATA_TYPE,
	HCC_ERROR_CODE_POSITION_MUST_BE_VEC4_F32,
	HCC_ERROR_CODE_POSITION_NOT_SPECIFIED,
	HCC_ERROR_CODE_EXPECTED_TYPE_NAME,
	HCC_ERROR_CODE_UNSIGNED_OR_SIGNED_ON_NON_INT_TYPE,
	HCC_ERROR_CODE_COMPLEX_ON_NON_FLOAT_TYPE,
	HCC_ERROR_CODE_MULTIPLE_TYPES_SPECIFIED,
	HCC_ERROR_CODE_COMPLEX_UNSUPPORTED_AT_THIS_TIME,
	HCC_ERROR_CODE_UNSIGNED_AND_SIGNED,
	HCC_ERROR_CODE_ATOMIC_UNSUPPORTED_AT_THIS_TIME,
	HCC_ERROR_CODE_DUPLICATE_TYPE_SPECIFIER,
	HCC_ERROR_CODE_EXPECTED_IDENTIFIER_TYPEDEF,
	HCC_ERROR_CODE_INVALID_SPECIFIER_FOR_TYPEDEF,
	HCC_ERROR_CODE_INTRINSIC_NOT_FOUND_TYPEDEF,
	HCC_ERROR_CODE_INTRINSIC_INVALID_TYPEDEF,
	HCC_ERROR_CODE_TYPE_MISMATCH_IMPLICIT_CAST,
	HCC_ERROR_CODE_TYPE_MISMATCH,
	HCC_ERROR_CODE_UNSUPPORTED_BINARY_OPERATOR,
	HCC_ERROR_CODE_INVALID_CURLY_EXPR,
	HCC_ERROR_CODE_FIELD_DESIGNATOR_ON_ARRAY_TYPE,
	HCC_ERROR_CODE_EXPECTED_IDENTIFIER_FIELD_DESIGNATOR,
	HCC_ERROR_CODE_ARRAY_DESIGNATOR_ON_COMPOUND_TYPE,
	HCC_ERROR_CODE_EXPECTED_INTEGER_FOR_ARRAY_IDX,
	HCC_ERROR_CODE_ARRAY_INDEX_OUT_OF_BOUNDS,
	HCC_ERROR_CODE_ARRAY_DESIGNATOR_EXPECTED_SQUARE_BRACE_CLOSE,
	HCC_ERROR_CODE_EXPECTED_ASSIGN_OR_ARRAY_DESIGNATOR,
	HCC_ERROR_CODE_EXPECTED_ASSIGN_OR_FIELD_DESIGNATOR,
	HCC_ERROR_CODE_EXPECTED_ASSIGN,
	HCC_ERROR_CODE_UNARY_OPERATOR_NOT_SUPPORTED,
	HCC_ERROR_CODE_UNDECLARED_IDENTIFIER,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_CLOSE_EXPR,
	HCC_ERROR_CODE_INVALID_CAST,
	HCC_ERROR_CODE_INVALID_CURLY_INITIALIZER_LIST_END,
	HCC_ERROR_CODE_SIZEALIGNOF_TYPE_OPERAND_NOT_WRAPPED,
	HCC_ERROR_CODE_EXPECTED_EXPR,
	HCC_ERROR_CODE_NOT_ENOUGH_FUNCTION_ARGS,
	HCC_ERROR_CODE_TOO_MANY_FUNCTION_ARGS,
	HCC_ERROR_CODE_INVALID_FUNCTION_ARG_DELIMITER,
	HCC_ERROR_CODE_ARRAY_SUBSCRIPT_EXPECTED_SQUARE_BRACE_CLOSE,
	HCC_ERROR_CODE_EXPECTED_IDENTIFIER_FIELD_ACCESS,
	HCC_ERROR_CODE_MISSING_COLON_TERNARY_OP,
	HCC_ERROR_CODE_PARENTHISES_USED_ON_NON_FUNCTION,
	HCC_ERROR_CODE_SQUARE_BRACE_USED_ON_NON_ARRAY_DATA_TYPE,
	HCC_ERROR_CODE_FULL_STOP_USED_ON_NON_COMPOUND_DATA_TYPE,
	HCC_ERROR_CODE_CANNOT_ASSIGN_TO_CONST,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_OPEN_CONDITION_EXPR,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_CLOSE_CONDITION_EXPR,
	HCC_ERROR_CODE_EXPECTED_ARRAY_SIZE,
	HCC_ERROR_CODE_EXPECTED_INTEGER_CONSTANT_ARRAY_SIZE,
	HCC_ERROR_CODE_ARRAY_SIZE_CANNOT_BE_NEGATIVE,
	HCC_ERROR_CODE_ARRAY_SIZE_CANNOT_BE_ZERO,
	HCC_ERROR_CODE_ARRAY_DECL_EXPECTED_SQUARE_BRACE_CLOSE,
	HCC_ERROR_CODE_UNSUPPORTED_SPECIFIER,
	HCC_ERROR_CODE_SPECIFIER_ALREADY_BEEN_USED,
	HCC_ERROR_CODE_UNUSED_SPECIFIER,
	HCC_ERROR_CODE_UNSUPPORTED_INTRINSIC_TYPE_USED_IN_VARIABLE,
	HCC_ERROR_CODE_UNSUPPORTED_INTRINSIC_TYPE_USED_IN_PARAM,
	HCC_ERROR_CODE_UNSUPPORTED_INTRINSIC_TYPE_USED_IN_EXPR,
	HCC_ERROR_CODE_INVALID_SPECIFIER_VARIABLE_DECL,
	HCC_ERROR_CODE_INVALID_SPECIFIER_FUNCTION_DECL,
	HCC_ERROR_CODE_INVALID_SPECIFIER_FUNCTION_PARAM,
	HCC_ERROR_CODE_REDEFINITION_IDENTIFIER_LOCAL,
	HCC_ERROR_CODE_STATIC_VARIABLE_INITIALIZER_MUST_BE_CONSTANT,
	HCC_ERROR_CODE_INVALID_VARIABLE_DECL_TERMINATOR,
	HCC_ERROR_CODE_INVALID_ELSE,
	HCC_ERROR_CODE_INVALID_SWITCH_CONDITION_TYPE,
	HCC_ERROR_CODE_EXPECTED_CURLY_OPEN_SWITCH_STATEMENT,
	HCC_ERROR_CODE_EXPECTED_WHILE_CONDITION_FOR_DO_WHILE,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_OPEN_FOR,
	HCC_ERROR_CODE_EXPECTED_IDENTIFIER_FOR_VARIABLE_DECL,
	HCC_ERROR_CODE_EXPECTED_PARENTHESIS_CLOSE_FOR,
	HCC_ERROR_CODE_CASE_STATEMENT_OUTSIDE_OF_SWITCH,
	HCC_ERROR_CODE_SWITCH_CASE_VALUE_MUST_BE_A_CONSTANT,
	HCC_ERROR_CODE_EXPECTED_COLON_SWITCH_CASE,
	HCC_ERROR_CODE_DEFAULT_STATMENT_OUTSIDE_OF_SWITCH,
	HCC_ERROR_CODE_DEFAULT_STATEMENT_ALREADY_DECLARED,
	HCC_ERROR_CODE_EXPECTED_COLON_SWITCH_DEFAULT,
	HCC_ERROR_CODE_INVALID_BREAK_STATEMENT_USAGE,
	HCC_ERROR_CODE_INVALID_CONTINUE_STATEMENT_USAGE,
	HCC_ERROR_CODE_MULTIPLE_SHADER_STAGES_ON_FUNCTION,
	HCC_ERROR_CODE_VERTEX_SHADER_MUST_RETURN_RASTERIZER_STATE,
	HCC_ERROR_CODE_FRAGMENT_SHADER_MUST_RETURN_FRAGMENT_STATE,
	HCC_ERROR_CODE_EXPECTED_IDENTIFIER_FUNCTION_PARAM,
	HCC_ERROR_CODE_REDEFINITION_IDENTIFIER_FUNCTION_PARAM,
	HCC_ERROR_CODE_FUNCTION_SHADER_PROTOTYPE_VERTEX_INVALID_COUNT,
	HCC_ERROR_CODE_FUNCTION_SHADER_PROTOTYPE_VERTEX_INVALID_INPUT,
	HCC_ERROR_CODE_FUNCTION_SHADER_PROTOTYPE_FRAGMENT_INVALID_COUNT,
	HCC_ERROR_CODE_FUNCTION_SHADER_PROTOTYPE_FRAGMENT_INVALID_INPUT,
	HCC_ERROR_CODE_FUNCTION_SHADER_PROTOTYPE_FRAGMENT_INVALID_RASTERIZER_STATE,
	HCC_ERROR_CODE_INTRINSIC_INVALID_FUNCTION_RETURN_DATA_TYPE,
	HCC_ERROR_CODE_INTRINSIC_INVALID_FUNCTION_PARAMS_COUNT,
	HCC_ERROR_CODE_INTRINSIC_INVALID_FUNCTION_PARAM_DATA_TYPE,
	HCC_ERROR_CODE_FUNCTION_INVALID_TERMINATOR,
	HCC_ERROR_CODE_EXPECTED_SHADER_PARAM_TO_BE_CONST,
	HCC_ERROR_CODE_CANNOT_CALL_SHADER_FUNCTION,
	HCC_ERROR_CODE_CANNOT_CALL_UNIMPLEMENTED_FUNCTION,
	HCC_ERROR_CODE_REDEFINITION_OF_FUNCTION_MISMATCH_PARAM_DATA_TYPE,
	HCC_ERROR_CODE_REDEFINITION_OF_FUNCTION_TOO_MANY_PARAMETERS,
	HCC_ERROR_CODE_REDEFINITION_OF_FUNCTION_NOT_ENOUGH_PARAMETERS,
	HCC_ERROR_CODE_REDEFINITION_OF_FUNCTION_BODY_ALREADY_DECLARED,
	HCC_ERROR_CODE_FUNCTION_RECURSION,
	HCC_ERROR_CODE_UNEXPECTED_TOKEN_FUNCTION_PROTOTYPE_END,
	HCC_ERROR_CODE_UNEXPECTED_TOKEN,

	//
	// spirv

	HCC_ERROR_CODE_COUNT,
};

typedef HccMessageCode HccWarnCode;
enum {
	HCC_WARN_CODE_NONE,

	//
	// tokengen
	HCC_WARN_CODE_PP_WARNING,

	//
	// astgen
	HCC_WARN_CODE_CURLY_INITIALIZER_ON_SCALAR,
	HCC_WARN_CODE_UNUSED_INITIALIZER_REACHED_END,
	HCC_WARN_CODE_NO_DESIGNATOR_AFTER_DESIGNATOR,

	//
	// spirv

	HCC_WARN_CODE_COUNT,
};

typedef struct HccLocation HccLocation;
struct HccLocation {
	HccCodeFile* code_file;
	HccLocation* parent_location;
	HccPPMacro*  macro;
	U32          code_start_idx;
	U32          code_end_idx;
	U32          line_start;
	U32          line_end;
	U32          column_start;
	U32          column_end;

	HccString    display_path;
	U32          display_line;
};

typedef struct HccMessage HccMessage;
struct HccMessage {
	HccString      string; // points to HccMessageSys.message_strings
	HccMessageType type;
	HccMessageCode code;
	HccLocation*   location;
	HccLocation*   other_location;
};

typedef struct HccMessageSys HccMessageSys;
struct HccMessageSys {
	HccStack(HccMessage)  elmts;
	HccStack(HccMessage)  deferred_elmts;
	HccStack(HccLocation) locations;
	HccStack(char)        strings;
	HccMessageType        used_type_flags;
	bool                  next_is_deferred;
};

const char* hcc_message_type_lang_strings[HCC_LANG_COUNT][HCC_MESSAGE_TYPE_COUNT];
const char* hcc_message_type_ascii_color_code[HCC_MESSAGE_TYPE_COUNT];
const char* hcc_error_code_lang_fmt_strings[HCC_LANG_COUNT][HCC_ERROR_CODE_COUNT];
const char* hcc_warn_code_lang_fmt_strings[HCC_LANG_COUNT][HCC_WARN_CODE_COUNT];

void hcc_location_merge_apply(HccLocation* before, HccLocation* after);

void hcc_message_print_file_line(HccCompiler* c, HccLocation* location);
void hcc_message_print_pasted_buffer(HccCompiler* c, U32 line, U32 column);
void hcc_message_print_code_line(HccCompiler* c, HccLocation* location, U32 display_line_num_size, U32 line, U32 display_line);
void hcc_message_print_code(HccCompiler* c, HccLocation* location);
void hcc_message_print(HccCompiler* c, HccMessage* message);

void hcc_message_pushv(HccCompiler* c, HccMessageType type, HccMessageCode code, HccLocation* location, HccLocation* other_location, va_list va_args);
void hcc_message_push(HccCompiler* c, HccMessageType type, HccMessageCode code, HccLocation* location, HccLocation* other_location, ...);
void hcc_error_pushv(HccCompiler* c, HccErrorCode error_code, HccLocation* location, HccLocation* other_location, va_list va_args);
void hcc_error_push(HccCompiler* c, HccErrorCode error_code, HccLocation* location, HccLocation* other_location, ...);
void hcc_warn_pushv(HccCompiler* c, HccWarnCode warn_code, HccLocation* location, HccLocation* other_location, va_list va_args);
void hcc_warn_push(HccCompiler* c, HccWarnCode warn_code, HccLocation* location, HccLocation* other_location, ...);

void hcc_message_copy_deferred(HccCompiler* c, U32 start_idx, U32 count);

// ===========================================
//
//
// Code File
//
//
// ===========================================

typedef U32 HccCodeFileFlags;
enum {
	HCC_CODE_FILE_FLAGS_COMPILATION_UNIT =      0x1,
	HCC_CODE_FILE_FLAGS_PRAGMA_ONCE =           0x2,
	HCC_CODE_FILE_FLAGS_PARSED_ONCE_OR_MORE =   0x4,
	HCC_CODE_FILE_FLAGS_IS_MACRO_PASTE_BUFFER = 0x8,
};

typedef struct HccCodeFile HccCodeFile;
struct HccCodeFile {
	HccCodeFileFlags      flags;
	HccStringId           path_string_id;
	HccString             path_string;
	HccString             code;
	HccStack(U32)         line_code_start_indices;
	HccStack(HccPPIfSpan) pp_if_spans;
	U32                   pp_if_span_id;
};

HccCodeFile* hcc_code_file_init(HccCompiler* c, HccStringId path_string_id);
bool hcc_code_file_find_or_insert(HccCompiler* c, HccString path_string, HccCodeFileId* code_file_id_out, HccCodeFile** code_file_out);
U32 hcc_code_file_line_size(HccCodeFile* code_file, U32 line);
U32 hcc_code_file_lines_count(HccCodeFile* code_file);

// ===========================================
//
//
// Declarations
//
//
// ===========================================

typedef U32 HccDataType;
enum {
#define HCC_DATA_TYPE_BASIC_START HCC_DATA_TYPE_VOID
	HCC_DATA_TYPE_VOID,
	HCC_DATA_TYPE_BOOL,
	HCC_DATA_TYPE_CHAR,
	HCC_DATA_TYPE_SCHAR,
	HCC_DATA_TYPE_SSHORT,
	HCC_DATA_TYPE_SINT,
	HCC_DATA_TYPE_SLONG,
	HCC_DATA_TYPE_SLONGLONG,
	HCC_DATA_TYPE_UCHAR,
	HCC_DATA_TYPE_USHORT,
	HCC_DATA_TYPE_UINT,
	HCC_DATA_TYPE_ULONG,
	HCC_DATA_TYPE_ULONGLONG,
	HCC_DATA_TYPE_FLOAT,
	HCC_DATA_TYPE_DOUBLE,
#define HCC_DATA_TYPE_BASIC_END (HCC_DATA_TYPE_DOUBLE + 1)
#define HCC_DATA_TYPE_BASIC_COUNT (HCC_DATA_TYPE_BASIC_END - HCC_DATA_TYPE_BASIC_START)

	HCC_DATA_TYPE_ENUM,
	HCC_DATA_TYPE_STRUCT,
	HCC_DATA_TYPE_UNION,
	HCC_DATA_TYPE_ARRAY,
	HCC_DATA_TYPE_TYPEDEF,

	HCC_DATA_TYPE_GENERIC_FLOAT,

	HCC_DATA_TYPE_COUNT,
};

#define HCC_DATA_TYPE_IS_BASIC(type)              ((type) < HCC_DATA_TYPE_BASIC_END)
#define HCC_DATA_TYPE_IS_NON_VOID_BASIC(type)     ((type) > HCC_DATA_TYPE_VOID && (type) < HCC_DATA_TYPE_BASIC_END)
#define HCC_DATA_TYPE_IS_INT(type)                ((type) >= HCC_DATA_TYPE_CHAR && (type) <= HCC_DATA_TYPE_ULONGLONG)
#define HCC_DATA_TYPE_IS_SINT(c, type)            ( \
		((type) >= HCC_DATA_TYPE_SCHAR && (type) <= HCC_DATA_TYPE_SLONGLONG) || \
		((type) == HCC_DATA_TYPE_CHAR && !(c->flags & HCC_COMPILER_FLAGS_CHAR_IS_UNSIGNED)) \
	)

#define HCC_DATA_TYPE_IS_UINT(c, type)            ( \
		((type) >= HCC_DATA_TYPE_UCHAR && (type) <= HCC_DATA_TYPE_ULONGLONG) || \
		((type) == HCC_DATA_TYPE_CHAR && (c->flags & HCC_COMPILER_FLAGS_CHAR_IS_UNSIGNED)) \
	)

#define HCC_DATA_TYPE_IS_FLOAT(type)         ((type) >= HCC_DATA_TYPE_FLOAT && (type) <= HCC_DATA_TYPE_DOUBLE)
#define HCC_DATA_TYPE_IS_STRUCT(type)        (((type) & 0xff) == HCC_DATA_TYPE_STRUCT)
#define HCC_DATA_TYPE_IS_UNION(type)         (((type) & 0xff) == HCC_DATA_TYPE_UNION)
#define HCC_DATA_TYPE_IS_COMPOUND_TYPE(type) (HCC_DATA_TYPE_IS_STRUCT(type) || HCC_DATA_TYPE_IS_UNION(type))
#define HCC_DATA_TYPE_IS_ARRAY(type)         (((type) & 0xff) == HCC_DATA_TYPE_ARRAY)
#define HCC_DATA_TYPE_IS_COMPOSITE_TYPE(type)(HCC_DATA_TYPE_IS_STRUCT(type) || HCC_DATA_TYPE_IS_UNION(type) || HCC_DATA_TYPE_IS_ARRAY(type))
#define HCC_DATA_TYPE_IS_PVECTOR(type)       (HCC_DATA_TYPE_IS_STRUCT(type) && HCC_STRUCT_IDX_PVEC_START <= HCC_DATA_TYPE_IDX(type) && HCC_DATA_TYPE_IDX(type) < HCC_STRUCT_IDX_PVEC_END)
#define HCC_DATA_TYPE_IS_VECTOR(type)        (HCC_DATA_TYPE_IS_UNION(type) && HCC_UNION_IDX_VEC_START <= HCC_DATA_TYPE_IDX(type) && HCC_DATA_TYPE_IDX(type) < HCC_UNION_IDX_VEC_END)
#define HCC_DATA_TYPE_IS_PMATRIX(type)       (HCC_DATA_TYPE_IS_STRUCT(type) && HCC_STRUCT_IDX_PMAT_START <= HCC_DATA_TYPE_IDX(type) && HCC_DATA_TYPE_IDX(type) < HCC_STRUCT_IDX_PMAT_END)
#define HCC_DATA_TYPE_IS_MATRIX(type)        (HCC_DATA_TYPE_IS_UNION(type) && HCC_UNION_IDX_MAT_START <= HCC_DATA_TYPE_IDX(type) && HCC_DATA_TYPE_IDX(type) < HCC_UNION_IDX_MAT_END)
#define HCC_DATA_TYPE_IS_INTRINSIC(type)     (HCC_DATA_TYPE_IS_BASIC(type) || HCC_DATA_TYPE_IS_VECTOR(type) || HCC_DATA_TYPE_IS_MATRIX(type) || (type) == HCC_DATA_TYPE_HALF)
#define HCC_DATA_TYPE_IS_INTRINSIC_BASIC(type)(HCC_DATA_TYPE_IS_BASIC(type) || (type) == HCC_DATA_TYPE_HALF)
#define HCC_DATA_TYPE_IS_TYPEDEF(type)       (((type) & 0xff) == HCC_DATA_TYPE_TYPEDEF)
#define HCC_DATA_TYPE_IS_ENUM_TYPE(type)     (((type) & 0xff) == HCC_DATA_TYPE_ENUM)
#define HCC_DATA_TYPE_INIT(type, idx)        (((idx) << 8) | (type))
#define HCC_DATA_TYPE_IS_CONST(type)         (!!((type) & HCC_DATA_TYPE_CONST_MASK))
#define HCC_DATA_TYPE_IDX(type)              (((type) & HCC_DATA_TYPE_IDX_MASK) >> 8)
#define HCC_DATA_TYPE_CONST(type)            ((type) | HCC_DATA_TYPE_CONST_MASK)
#define HCC_DATA_TYPE_STRIP_CONST(type)      ((type) & ~HCC_DATA_TYPE_CONST_MASK)
#define HCC_DATA_TYPE_ENUM(idx)              HCC_DATA_TYPE_INIT(HCC_DATA_TYPE_ENUM, idx)
#define HCC_DATA_TYPE_STRUCT(idx)            HCC_DATA_TYPE_INIT(HCC_DATA_TYPE_STRUCT, idx)
#define HCC_DATA_TYPE_UNION(idx)             HCC_DATA_TYPE_INIT(HCC_DATA_TYPE_UNION, idx)
#define HCC_DATA_TYPE_ARRAY(idx)             HCC_DATA_TYPE_INIT(HCC_DATA_TYPE_ARRAY, idx)
#define HCC_DATA_TYPE_TYPEDEF(idx)           HCC_DATA_TYPE_INIT(HCC_DATA_TYPE_TYPEDEF, idx)
#define HCC_DATA_TYPE_PVECTOR(vec)           (HCC_DATA_TYPE_STRUCT | ((HCC_STRUCT_IDX_PVEC_START + (vec)) << 8))
#define HCC_DATA_TYPE_VECTOR(vec)            (HCC_DATA_TYPE_UNION | ((HCC_UNION_IDX_VEC_START + (vec)) << 8))
#define HCC_DATA_TYPE_PMATRIX(vec)           (HCC_DATA_TYPE_STRUCT | ((HCC_STRUCT_IDX_PMAT_START + (vec)) << 8))
#define HCC_DATA_TYPE_MATRIX(vec)            (HCC_DATA_TYPE_UNION | ((HCC_UNION_IDX_MAT_START + (vec)) << 8))
#define HCC_DATA_TYPE_PVECTOR_VEC(type)      (HCC_DATA_TYPE_IDX(type) - HCC_STRUCT_IDX_PVEC_START)
#define HCC_DATA_TYPE_VECTOR_VEC(type)       (HCC_DATA_TYPE_IDX(type) - HCC_UNION_IDX_VEC_START)
#define HCC_DATA_TYPE_PMATRIX_MAT(type)      (HCC_DATA_TYPE_IDX(type) - HCC_STRUCT_IDX_PMAT_START)
#define HCC_DATA_TYPE_MATRIX_MAT(type)       (HCC_DATA_TYPE_IDX(type) - HCC_UNION_IDX_MAT_START)
#define HCC_DATA_TYPE_UINT8                  HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_UINT8)
#define HCC_DATA_TYPE_UINT16                 HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_UINT16)
#define HCC_DATA_TYPE_UINT32                 HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_UINT32)
#define HCC_DATA_TYPE_UINT64                 HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_UINT64)
#define HCC_DATA_TYPE_UINTPTR                HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_UINTPTR)
#define HCC_DATA_TYPE_INT8                   HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_INT8)
#define HCC_DATA_TYPE_INT16                  HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_INT16)
#define HCC_DATA_TYPE_INT32                  HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_INT32)
#define HCC_DATA_TYPE_INT64                  HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_INT64)
#define HCC_DATA_TYPE_INTPTR                 HCC_DATA_TYPE_TYPEDEF(HCC_TYPEDEF_IDX_INTPTR)
#define HCC_DATA_TYPE_HALF                   HCC_DATA_TYPE_STRUCT(HCC_STRUCT_IDX_HALF)
#define HCC_DATA_TYPE_CONST_MASK             0x80000000
#define HCC_DATA_TYPE_IDX_MASK               0x7fffff00
#define HCC_DATA_TYPE_USE_USERLAND_VECTOR_OR_MATRIX UINT32_MAX

typedef U8 HccBasicTypeClass;
enum {
	HCC_BASIC_TYPE_CLASS_VOID,
	HCC_BASIC_TYPE_CLASS_BOOL,
	HCC_BASIC_TYPE_CLASS_UINT,
	HCC_BASIC_TYPE_CLASS_SINT,
	HCC_BASIC_TYPE_CLASS_FLOAT,

	HCC_BASIC_TYPE_CLASS_COUNT,
};

typedef union HccBasic HccBasic;
union HccBasic {
	int8_t   s8;
	int16_t  s16;
	int32_t  s32;
	int64_t  s64;
	uint8_t  u8;
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;
	float    f;
	double   d;
};

typedef U8 HccVec;
enum {
	HCC_VEC2BOOL,
	HCC_VEC2I8,
	HCC_VEC2I16,
	HCC_VEC2I32,
	HCC_VEC2I64,
	HCC_VEC2U8,
	HCC_VEC2U16,
	HCC_VEC2U32,
	HCC_VEC2U64,
	HCC_VEC2H,
	HCC_VEC2F,
	HCC_VEC2D,
	HCC_VEC3BOOL,
	HCC_VEC3I8,
	HCC_VEC3I16,
	HCC_VEC3I32,
	HCC_VEC3I64,
	HCC_VEC3U8,
	HCC_VEC3U16,
	HCC_VEC3U32,
	HCC_VEC3U64,
	HCC_VEC3H,
	HCC_VEC3F,
	HCC_VEC3D,
	HCC_VEC4BOOL,
	HCC_VEC4I8,
	HCC_VEC4I16,
	HCC_VEC4I32,
	HCC_VEC4I64,
	HCC_VEC4U8,
	HCC_VEC4U16,
	HCC_VEC4U32,
	HCC_VEC4U64,
	HCC_VEC4H,
	HCC_VEC4F,
	HCC_VEC4D,

	HCC_VEC_COUNT,
};

typedef U8 HccMat;
enum {
	HCC_MAT2X2F,
	HCC_MAT2X2D,
	HCC_MAT2X3F,
	HCC_MAT2X3D,
	HCC_MAT2X4F,
	HCC_MAT2X4D,
	HCC_MAT3X2F,
	HCC_MAT3X2D,
	HCC_MAT3X3F,
	HCC_MAT3X3D,
	HCC_MAT3X4F,
	HCC_MAT3X4D,
	HCC_MAT4X2F,
	HCC_MAT4X2D,
	HCC_MAT4X3F,
	HCC_MAT4X3D,
	HCC_MAT4X4F,
	HCC_MAT4X4D,

	HCC_MAT_COUNT,
};

typedef U8 HccIntrinsicType;
enum {
	HCC_INTRINSIC_TYPE_VOID,
	HCC_INTRINSIC_TYPE_BOOL,
	HCC_INTRINSIC_TYPE_S8,
	HCC_INTRINSIC_TYPE_S16,
	HCC_INTRINSIC_TYPE_S32,
	HCC_INTRINSIC_TYPE_S64,
	HCC_INTRINSIC_TYPE_U8,
	HCC_INTRINSIC_TYPE_U16,
	HCC_INTRINSIC_TYPE_U32,
	HCC_INTRINSIC_TYPE_U64,
	HCC_INTRINSIC_TYPE_F16,
	HCC_INTRINSIC_TYPE_F32,
	HCC_INTRINSIC_TYPE_F64,

	HCC_INTRINSIC_TYPE_VECTOR_START,
#define HCC_INTRINSIC_TYPE_VECTOR_END HCC_INTRINSIC_TYPE_MATRIX_START
	HCC_INTRINSIC_TYPE_MATRIX_START = HCC_INTRINSIC_TYPE_VECTOR_START + HCC_VEC_COUNT,
#define HCC_INTRINSIC_TYPE_MATRIX_END HCC_INTRINSIC_TYPE_COUNT

	HCC_INTRINSIC_TYPE_COUNT = HCC_INTRINSIC_TYPE_MATRIX_START + HCC_MAT_COUNT,
};
static_assert(HCC_VEC2BOOL == HCC_INTRINSIC_TYPE_BOOL - 1 && HCC_VEC2U64 == HCC_INTRINSIC_TYPE_U64 - 1, "hey you changed the order :(");

#define HCC_INTRINSIC_TYPE_FROM_VEC(vec) (HCC_INTRINSIC_TYPE_BOOL + (vec) % 12)
#define HCC_INTRINSIC_TYPE_FROM_MAT(mat) (HCC_INTRINSIC_TYPE_F32 + (mat) % 2)

typedef U16 HccIntrinsicBasicTypeMask;
#define HCC_INTRINSIC_BASIC_TYPE_MASK_SET(ptr, intrinsic_type) (*(ptr)) |= (1 << (intrinsic_type))
#define HCC_INTRINSIC_BASIC_TYPE_MASK_UNSET(ptr, intrinsic_type) (*(ptr)) &= ~(1 << (intrinsic_type))
#define HCC_INTRINSIC_BASIC_TYPE_MASK_IS_SET(v, intrinsic_type) ((bool)((v) & (1 << (intrinsic_type))))

typedef struct HccArrayDataType HccArrayDataType;
struct HccArrayDataType {
	HccDataType element_data_type;
	HccConstantId size_constant_id;
};

typedef U16 HccCompoundDataTypeFlags;
enum {
	HCC_COMPOUND_DATA_TYPE_FLAGS_IS_UNION =            0x1,
	HCC_COMPOUND_DATA_TYPE_FLAGS_IS_RASTERIZER_STATE = 0x2,
	HCC_COMPOUND_DATA_TYPE_FLAGS_IS_FRAGMENT_STATE =   0x4,
};

typedef struct HccCompoundDataType HccCompoundDataType;
struct HccCompoundDataType {
	U64                       size;
	U64                       align;
	U32                       identifier_token_idx;
	HccStringId               identifier_string_id;
	U32                       fields_start_idx;
	U16                       fields_count;
	U16                       largest_sized_field_idx;
	HccIntrinsicBasicTypeMask has_intrinsic_basic_types;
	HccCompoundDataTypeFlags  flags;
};

typedef U16 HccRasterizerStateFieldKind;
enum {
	HCC_RASTERIZER_STATE_FIELD_KIND_INTERP,
	HCC_RASTERIZER_STATE_FIELD_KIND_POSITION,
	HCC_RASTERIZER_STATE_FIELD_KIND_NOINTERP,
};

typedef struct HccCompoundField HccCompoundField;
struct HccCompoundField {
	HccStringId                 identifier_string_id;
	HccDataType                 data_type;
	U32                         identifier_token_idx;
	HccRasterizerStateFieldKind rasterizer_state_field_kind;
};

typedef struct HccEnumDataType HccEnumDataType;
struct HccEnumDataType {
	U32         identifier_token_idx;
	HccStringId identifier_string_id;
	U32         values_start_idx;
	U32         values_count;
};

typedef struct HccEnumValue HccEnumValue;
struct HccEnumValue {
	U32           identifier_token_idx;
	HccStringId   identifier_string_id;
	HccConstantId value_constant_id;
};

typedef struct HccTypedef HccTypedef;
struct HccTypedef {
	U32 identifier_token_idx;
	HccStringId identifier_string_id;
	HccDataType aliased_data_type;
};

//
// inherits HccDataType
typedef U32 HccDecl;
enum {
	HCC_DECL_FUNCTION = HCC_DATA_TYPE_COUNT,
	HCC_DECL_ENUM_VALUE,
	HCC_DECL_LOCAL_VARIABLE,
	HCC_DECL_GLOBAL_VARIABLE,
};
#define HCC_DECL_IS_DATA_TYPE(type) (((type) & 0xff) < HCC_DATA_TYPE_COUNT)
#define HCC_DECL_IS_FUNCTION(type) (((type) & 0xff) == HCC_DECL_FUNCTION)
#define HCC_DECL_IS_ENUM_VALUE(type) (((type) & 0xff) == HCC_DECL_ENUM_VALUE)
#define HCC_DECL_IS_LOCAL_VARIABLE(type) (((type) & 0xff) == HCC_DECL_LOCAL_VARIABLE)
#define HCC_DECL_IS_GLOBAL_VARIABLE(type) (((type) & 0xff) == HCC_DECL_GLOBAL_VARIABLE)
#define HCC_DECL_INIT(type, idx) HCC_DATA_TYPE_INIT(type, idx)
#define HCC_DECL_IDX(type) HCC_DATA_TYPE_IDX(type)

typedef struct HccVariable HccVariable;
struct HccVariable {
	HccStringId   identifier_string_id;
	HccDataType   data_type;
	HccConstantId initializer_constant_id; // if is_static
	U32           identifier_token_idx: 30;
	U32           is_static: 1;
	U32           is_const: 1;
};

typedef U8 HccFunctionShaderStage;
enum {
	HCC_FUNCTION_SHADER_STAGE_NONE,
	HCC_FUNCTION_SHADER_STAGE_VERTEX,
	HCC_FUNCTION_SHADER_STAGE_FRAGMENT,
	HCC_FUNCTION_SHADER_STAGE_GEOMETRY,
	HCC_FUNCTION_SHADER_STAGE_TESSELLATION,
	HCC_FUNCTION_SHADER_STAGE_COMPUTE,
	HCC_FUNCTION_SHADER_STAGE_MESHTASK,

	HCC_FUNCTION_SHADER_STAGE_COUNT,
};

typedef U8 HccFunctionFlags;
enum {
	HCC_FUNCTION_FLAGS_STATIC = 0x1,
	HCC_FUNCTION_FLAGS_CONST =  0x2,
	HCC_FUNCTION_FLAGS_INLINE = 0x4,
};

typedef struct HccFunction HccFunction;
struct HccFunction {
	U32                    identifier_token_idx;
	HccStringId            identifier_string_id;
	HccDataType            return_data_type;
	U32                    return_data_type_token_idx;
	U32                    params_start_idx;
	U16                    variables_count;
	U8                     params_count;
	HccFunctionFlags       flags;
	HccFunctionShaderStage shader_stage;
	HccExprId              block_expr_id;
	U32                    used_function_indices_start_idx;
	U32                    used_function_indices_count;
	U32                    used_static_variables_start_idx;
	U32                    used_static_variables_count;
	U32                    unsupported_basic_types_deferred_messages_start_idx;
	U32                    unsupported_basic_types_deferred_messages_count;
};

enum {
	HCC_TYPEDEF_IDX_UINT8,
	HCC_TYPEDEF_IDX_UINT16,
	HCC_TYPEDEF_IDX_UINT32,
	HCC_TYPEDEF_IDX_UINT64,
	HCC_TYPEDEF_IDX_UINTPTR,
	HCC_TYPEDEF_IDX_INT8,
	HCC_TYPEDEF_IDX_INT16,
	HCC_TYPEDEF_IDX_INT32,
	HCC_TYPEDEF_IDX_INT64,
	HCC_TYPEDEF_IDX_INTPTR,
	HCC_TYPEDEF_IDX_HALF,

	HCC_TYPEDEF_IDX_PVEC_START,
#define HCC_TYPEDEF_IDX_PVEC_END HCC_TYPEDEF_IDX_VEC_START

	HCC_TYPEDEF_IDX_VEC_START = HCC_TYPEDEF_IDX_PVEC_START + HCC_VEC_COUNT,
#define HCC_TYPEDEF_IDX_VEC_END HCC_TYPEDEF_IDX_PMAT_START

	HCC_TYPEDEF_IDX_PMAT_START = HCC_TYPEDEF_IDX_VEC_START + HCC_VEC_COUNT,
#define HCC_TYPEDEF_IDX_PMAT_END HCC_TYPEDEF_IDX_MAT_START

	HCC_TYPEDEF_IDX_MAT_START = HCC_TYPEDEF_IDX_PMAT_START + HCC_MAT_COUNT,
#define HCC_TYPEDEF_IDX_MAT_END (HCC_TYPEDEF_IDX_MAT_START + HCC_MAT_COUNT)

	HCC_TYPEDEF_IDX_VERTEX_INPUT = HCC_TYPEDEF_IDX_MAT_END,
	HCC_TYPEDEF_IDX_FRAGMENT_INPUT,

#define HCC_TYPEDEF_IDX_INTRINSIC_END HCC_TYPEDEF_IDX_USER_START
	HCC_TYPEDEF_IDX_USER_START,
};

enum {
	HCC_STRUCT_IDX_VERTEX_INPUT,
	HCC_STRUCT_IDX_FRAGMENT_INPUT,
	HCC_STRUCT_IDX_HALF,

#define HCC_STRUCT_IDX_INTRINSIC_END HCC_STRUCT_IDX_PVEC_START

	HCC_STRUCT_IDX_PVEC_START,
#define HCC_STRUCT_IDX_PVEC_END HCC_UNION_IDX_VEC_START

	HCC_UNION_IDX_VEC_START = HCC_STRUCT_IDX_PVEC_START + HCC_VEC_COUNT,
#define HCC_UNION_IDX_VEC_END HCC_STRUCT_IDX_PMAT_START

	HCC_STRUCT_IDX_PMAT_START = HCC_UNION_IDX_VEC_START + HCC_VEC_COUNT,
#define HCC_STRUCT_IDX_PMAT_END HCC_UNION_IDX_MAT_START

	HCC_UNION_IDX_MAT_START = HCC_STRUCT_IDX_PMAT_START + HCC_MAT_COUNT,
#define HCC_UNION_IDX_MAT_END HCC_COMPOUND_DATA_TYPE_IDX_USER_START

	HCC_COMPOUND_DATA_TYPE_IDX_USER_START = HCC_UNION_IDX_MAT_START + HCC_MAT_COUNT,
};

enum {
	//
	// libc-gpu/math.h
	//
	HCC_FUNCTION_IDX_ISINF,
	HCC_FUNCTION_IDX_ISNAN,
	HCC_FUNCTION_IDX_FMODF,
	HCC_FUNCTION_IDX_FMOD,
	HCC_FUNCTION_IDX_FABSF,
	HCC_FUNCTION_IDX_FABS,
	HCC_FUNCTION_IDX_FLOORF,
	HCC_FUNCTION_IDX_FLOOR,
	HCC_FUNCTION_IDX_CEILF,
	HCC_FUNCTION_IDX_CEIL,
	HCC_FUNCTION_IDX_ROUNDF,
	HCC_FUNCTION_IDX_ROUND,
	HCC_FUNCTION_IDX_TRUNCF,
	HCC_FUNCTION_IDX_TRUNC,
	HCC_FUNCTION_IDX_SINF,
	HCC_FUNCTION_IDX_SIN,
	HCC_FUNCTION_IDX_COSF,
	HCC_FUNCTION_IDX_COS,
	HCC_FUNCTION_IDX_TANF,
	HCC_FUNCTION_IDX_TAN,
	HCC_FUNCTION_IDX_ASINF,
	HCC_FUNCTION_IDX_ASIN,
	HCC_FUNCTION_IDX_ACOSF,
	HCC_FUNCTION_IDX_ACOS,
	HCC_FUNCTION_IDX_ATANF,
	HCC_FUNCTION_IDX_ATAN,
	HCC_FUNCTION_IDX_SINHF,
	HCC_FUNCTION_IDX_SINH,
	HCC_FUNCTION_IDX_COSHF,
	HCC_FUNCTION_IDX_COSH,
	HCC_FUNCTION_IDX_TANHF,
	HCC_FUNCTION_IDX_TANH,
	HCC_FUNCTION_IDX_ASINHF,
	HCC_FUNCTION_IDX_ASINH,
	HCC_FUNCTION_IDX_ACOSHF,
	HCC_FUNCTION_IDX_ACOSH,
	HCC_FUNCTION_IDX_ATANHF,
	HCC_FUNCTION_IDX_ATANH,
	HCC_FUNCTION_IDX_ATAN2F,
	HCC_FUNCTION_IDX_ATAN2,
	HCC_FUNCTION_IDX_FMAF,
	HCC_FUNCTION_IDX_FMA,
	HCC_FUNCTION_IDX_POWF,
	HCC_FUNCTION_IDX_POW,
	HCC_FUNCTION_IDX_EXPF,
	HCC_FUNCTION_IDX_EXP,
	HCC_FUNCTION_IDX_LOGF,
	HCC_FUNCTION_IDX_LOG,
	HCC_FUNCTION_IDX_EXP2F,
	HCC_FUNCTION_IDX_EXP2,
	HCC_FUNCTION_IDX_LOG2F,
	HCC_FUNCTION_IDX_LOG2,

	//
	// libhccstd/math.h
	//

#define HCC_FUNCTION_IDX_INTRINSIC_END HCC_FUNCTION_IDX_USER_START
	HCC_FUNCTION_IDX_USER_START,
};

enum {
	HCC_VERTEX_INPUT_VERTEX_INDEX,
	HCC_VERTEX_INPUT_INSTANCE_INDEX,
};

enum {
	HCC_FRAGMENT_INPUT_FRAG_COORD,
};

typedef struct HccIntrinsicTypedef HccIntrinsicTypedef;
struct HccIntrinsicTypedef {
	HccStringId string_id;
	HccDataType aliased_data_type;
};

typedef struct HccIntrinsicStructField HccIntrinsicStructField;
struct HccIntrinsicStructField {
	HccDataType data_type;
	HccString name;
};

typedef struct HccIntrinsicStruct HccIntrinsicStruct;
struct HccIntrinsicStruct {
	HccStringId string_id;
	U32 fields_count;
	HccIntrinsicStructField fields[16];
};

typedef struct HccIntrinsicFunction HccIntrinsicFunction;
struct HccIntrinsicFunction {
	HccStringId string_id;
	HccDataType return_data_type;
	U32         params_count;
	HccDataType param_data_types[16];
};

extern U8 hcc_basic_type_size_and_aligns_x86_64_linux[HCC_DATA_TYPE_BASIC_COUNT];
extern U64 hcc_basic_type_int_mins_x86_64_linux[HCC_DATA_TYPE_BASIC_COUNT];
extern U64 hcc_basic_type_int_maxes_x86_64_linux[HCC_DATA_TYPE_BASIC_COUNT];

extern U8 hcc_packed_vec_sizes[HCC_VEC_COUNT];
extern U8 hcc_packed_vec_aligns[HCC_VEC_COUNT];
extern U8 hcc_vec_size_and_aligns[HCC_VEC_COUNT];
extern U8 hcc_packed_mat_sizes[HCC_MAT_COUNT];
extern U8 hcc_packed_mat_aligns[HCC_MAT_COUNT];
extern U8 hcc_mat_sizes[HCC_MAT_COUNT];
extern U8 hcc_mat_aligns[HCC_MAT_COUNT];
extern const char* hcc_intrinsic_function_strings[HCC_FUNCTION_IDX_INTRINSIC_END];
extern char* hcc_function_shader_stage_strings[HCC_FUNCTION_SHADER_STAGE_COUNT];
extern HccIntrinsicTypedef hcc_intrinsic_typedefs[HCC_TYPEDEF_IDX_INTRINSIC_END];
extern HccIntrinsicStruct hcc_intrinsic_structs[HCC_STRUCT_IDX_INTRINSIC_END];
extern HccIntrinsicFunction hcc_intrinsic_functions[HCC_FUNCTION_IDX_INTRINSIC_END];
extern U8 hcc_data_type_basic_type_ranks[HCC_DATA_TYPE_BASIC_COUNT];
extern const char* hcc_data_type_basic_type_strings[HCC_DATA_TYPE_BASIC_COUNT];

enum {
	HCC_FUNCTION_IDX_IDX_USER_START,
};

HccString hcc_data_type_string(HccCompiler* c, HccDataType data_type);
void hcc_data_type_size_align(HccCompiler* c, HccDataType data_type, U64* size_out, U64* align_out);
void hcc_data_type_print_basic(HccCompiler* c, HccDataType data_type, void* data, FILE* f);
HccDataType hcc_data_type_unsigned_to_signed(HccCompiler* c, HccDataType data_type);
HccDataType hcc_data_type_signed_to_unsigned(HccCompiler* c, HccDataType data_type);
bool hcc_data_type_is_condition(HccCompiler* c, HccDataType data_type);
U32 hcc_data_type_composite_fields_count(HccCompiler* c, HccDataType data_type);
bool hcc_data_type_is_rasterizer_state(HccCompiler* c, HccDataType data_type);
bool hcc_data_type_is_fragment_state(HccCompiler* c, HccDataType data_type);
HccDataType hcc_data_type_from_intrinsic_type(HccCompiler* c, HccIntrinsicType intrinsic_type);
HccIntrinsicBasicTypeMask hcc_data_type_has_intrinsic_basic_types(HccCompiler* c, HccDataType data_type);
bool hcc_data_type_is_same_underlaying_type(HccCompiler* c, HccDataType a, HccDataType b);
bool hcc_data_type_is_same_bitwidth_int(HccCompiler* c, HccDataType a, HccDataType b);

U32 hcc_data_type_token_idx(HccCompiler* c, HccDataType data_type);

HccIntrinsicType hcc_intrinsic_type_from_data_type(HccCompiler* c, HccDataType data_type);
HccDataType hcc_vector_data_type_scalar(HccCompiler* c, HccDataType data_type);
HccDataType hcc_matrix_data_type_scalar(HccCompiler* c, HccDataType data_type);
HccDataType hcc_matrix_data_type_column_vector(HccDataType data_type);

HccString hcc_intrinsic_basic_type_mask_string(HccIntrinsicBasicTypeMask mask);

HccBasicTypeClass hcc_basic_type_class(HccCompiler* c, HccDataType data_type);
HccBasic hcc_basic_from_sint(HccCompiler* c, HccDataType data_type, S64 value);
HccBasic hcc_basic_from_uint(HccCompiler* c, HccDataType data_type, U64 value);
HccBasic hcc_basic_from_float(HccCompiler* c, HccDataType data_type, double value);

HccArrayDataType* hcc_array_data_type_get(HccCompiler* c, HccDataType data_type);
HccEnumDataType* hcc_enum_data_type_get(HccCompiler* c, HccDataType data_type);
HccCompoundDataType* hcc_compound_data_type_get(HccCompiler* c, HccDataType data_type);
HccTypedef* hcc_typedef_get(HccCompiler* c, HccDataType data_type);
HccDataType hcc_typedef_resolve_and_keep_const(HccCompiler* c, HccDataType data_type);
HccDataType hcc_typedef_resolve_and_strip_const(HccCompiler* c, HccDataType data_type);

U32 hcc_decl_token_idx(HccCompiler* c, HccDecl decl);
HccFunction* hcc_function_get(HccCompiler* c, HccDecl decl);
HccEnumValue* hcc_enum_value_get(HccCompiler* c, HccDecl decl);
HccVariable* hcc_global_variable_get(HccCompiler* c, HccDecl decl);
U32 hcc_variable_to_string(HccCompiler* c, HccVariable* variable, char* buf, U32 buf_size, bool color);
U32 hcc_function_to_string(HccCompiler* c, HccFunction* function, char* buf, U32 buf_size, bool color);

// ===========================================
//
//
// Common
//
//
// ===========================================

typedef U8 HccBinaryOp;
enum {
	HCC_BINARY_OP_ASSIGN,
	HCC_BINARY_OP_ADD,
	HCC_BINARY_OP_SUBTRACT,
	HCC_BINARY_OP_MULTIPLY,
	HCC_BINARY_OP_DIVIDE,
	HCC_BINARY_OP_MODULO,
	HCC_BINARY_OP_BIT_AND,
	HCC_BINARY_OP_BIT_OR,
	HCC_BINARY_OP_BIT_XOR,
	HCC_BINARY_OP_BIT_SHIFT_LEFT,
	HCC_BINARY_OP_BIT_SHIFT_RIGHT,

	//
	// logical operators (return a bool)
	HCC_BINARY_OP_EQUAL,
	HCC_BINARY_OP_NOT_EQUAL,
	HCC_BINARY_OP_LESS_THAN,
	HCC_BINARY_OP_LESS_THAN_OR_EQUAL,
	HCC_BINARY_OP_GREATER_THAN,
	HCC_BINARY_OP_GREATER_THAN_OR_EQUAL,

	HCC_BINARY_OP_LOGICAL_AND,
	HCC_BINARY_OP_LOGICAL_OR,

	HCC_BINARY_OP_TERNARY,
	HCC_BINARY_OP_COMMA,

	HCC_BINARY_OP_COUNT,
};

// ===========================================
//
//
// Token
//
//
// ===========================================

#define HCC_TOKEN_IDX_INVALID U32_MAX

typedef U8 HccToken;
enum {
	HCC_TOKEN_EOF,
	HCC_TOKEN_IDENT,
	HCC_TOKEN_STRING,
	HCC_TOKEN_INCLUDE_PATH_SYSTEM,
	HCC_TOKEN_BACK_SLASH,
	HCC_TOKEN_HASH,
	HCC_TOKEN_DOUBLE_HASH,
	HCC_TOKEN_MACRO_WHITESPACE,
	HCC_TOKEN_MACRO_PARAM,
	HCC_TOKEN_MACRO_STRINGIFY,
	HCC_TOKEN_MACRO_STRINGIFY_WHITESPACE,
	HCC_TOKEN_MACRO_CONCAT,
	HCC_TOKEN_MACRO_CONCAT_WHITESPACE,

	//
	// symbols
	//
#define HCC_TOKEN_BRACKET_START HCC_TOKEN_CURLY_OPEN
#define HCC_TOKEN_BRACKET_END (HCC_TOKEN_SQUARE_CLOSE + 1)
#define HCC_TOKEN_BRACKET_COUNT (HCC_TOKEN_BRACKET_END - HCC_TOKEN_BRACKET_START)
	HCC_TOKEN_CURLY_OPEN,
	HCC_TOKEN_CURLY_CLOSE,
	HCC_TOKEN_PARENTHESIS_OPEN,
	HCC_TOKEN_PARENTHESIS_CLOSE,
	HCC_TOKEN_SQUARE_OPEN,
	HCC_TOKEN_SQUARE_CLOSE,
	HCC_TOKEN_FULL_STOP,
	HCC_TOKEN_COMMA,
	HCC_TOKEN_SEMICOLON,
	HCC_TOKEN_COLON,
	HCC_TOKEN_PLUS,
	HCC_TOKEN_MINUS,
	HCC_TOKEN_FORWARD_SLASH,
	HCC_TOKEN_ASTERISK,
	HCC_TOKEN_PERCENT,
	HCC_TOKEN_AMPERSAND,
	HCC_TOKEN_PIPE,
	HCC_TOKEN_CARET,
	HCC_TOKEN_EXCLAMATION_MARK,
	HCC_TOKEN_QUESTION_MARK,
	HCC_TOKEN_TILDE,
	HCC_TOKEN_EQUAL,
	HCC_TOKEN_LESS_THAN,
	HCC_TOKEN_GREATER_THAN,

	//
	// grouped symbols
	//
	HCC_TOKEN_LOGICAL_AND,
	HCC_TOKEN_LOGICAL_OR,
	HCC_TOKEN_LOGICAL_EQUAL,
	HCC_TOKEN_LOGICAL_NOT_EQUAL,
	HCC_TOKEN_LESS_THAN_OR_EQUAL,
	HCC_TOKEN_GREATER_THAN_OR_EQUAL,
	HCC_TOKEN_BIT_SHIFT_LEFT,
	HCC_TOKEN_BIT_SHIFT_RIGHT,
	HCC_TOKEN_ADD_ASSIGN,
	HCC_TOKEN_SUBTRACT_ASSIGN,
	HCC_TOKEN_MULTIPLY_ASSIGN,
	HCC_TOKEN_DIVIDE_ASSIGN,
	HCC_TOKEN_MODULO_ASSIGN,
	HCC_TOKEN_BIT_SHIFT_LEFT_ASSIGN,
	HCC_TOKEN_BIT_SHIFT_RIGHT_ASSIGN,
	HCC_TOKEN_BIT_AND_ASSIGN,
	HCC_TOKEN_BIT_XOR_ASSIGN,
	HCC_TOKEN_BIT_OR_ASSIGN,
	HCC_TOKEN_INCREMENT,
	HCC_TOKEN_DECREMENT,

#define HCC_TOKEN_LIT_NUMBERS_START HCC_TOKEN_LIT_UINT
	HCC_TOKEN_LIT_UINT,
	HCC_TOKEN_LIT_ULONG,
	HCC_TOKEN_LIT_ULONGLONG,
	HCC_TOKEN_LIT_SINT,
	HCC_TOKEN_LIT_SLONG,
	HCC_TOKEN_LIT_SLONGLONG,
	HCC_TOKEN_LIT_FLOAT,
	HCC_TOKEN_LIT_DOUBLE,
#define HCC_TOKEN_LIT_NUMBERS_END (HCC_TOKEN_LIT_DOUBLE + 1)

	//
	// keywords
	//
#define HCC_TOKEN_KEYWORDS_START HCC_TOKEN_KEYWORD_VOID
	HCC_TOKEN_KEYWORD_VOID,
	HCC_TOKEN_KEYWORD_BOOL,
	HCC_TOKEN_KEYWORD_CHAR,
	HCC_TOKEN_KEYWORD_SHORT,
	HCC_TOKEN_KEYWORD_INT,
	HCC_TOKEN_KEYWORD_LONG,
	HCC_TOKEN_KEYWORD_FLOAT,
	HCC_TOKEN_KEYWORD_DOUBLE,
	HCC_TOKEN_KEYWORD_GENERIC_FLOAT,
	HCC_TOKEN_KEYWORD_UNSIGNED,
	HCC_TOKEN_KEYWORD_SIGNED,
	HCC_TOKEN_KEYWORD_COMPLEX,
	HCC_TOKEN_KEYWORD_ATOMIC,
	HCC_TOKEN_KEYWORD_RETURN,
	HCC_TOKEN_KEYWORD_IF,
	HCC_TOKEN_KEYWORD_ELSE,
	HCC_TOKEN_KEYWORD_DO,
	HCC_TOKEN_KEYWORD_WHILE,
	HCC_TOKEN_KEYWORD_FOR,
	HCC_TOKEN_KEYWORD_SWITCH,
	HCC_TOKEN_KEYWORD_CASE,
	HCC_TOKEN_KEYWORD_DEFAULT,
	HCC_TOKEN_KEYWORD_BREAK,
	HCC_TOKEN_KEYWORD_CONTINUE,
	HCC_TOKEN_KEYWORD_TRUE,
	HCC_TOKEN_KEYWORD_FALSE,
	HCC_TOKEN_KEYWORD_VERTEX,
	HCC_TOKEN_KEYWORD_FRAGMENT,
	HCC_TOKEN_KEYWORD_GEOMETRY,
	HCC_TOKEN_KEYWORD_TESSELLATION,
	HCC_TOKEN_KEYWORD_COMPUTE,
	HCC_TOKEN_KEYWORD_MESHTASK,
	HCC_TOKEN_KEYWORD_ENUM,
	HCC_TOKEN_KEYWORD_STRUCT,
	HCC_TOKEN_KEYWORD_UNION,
	HCC_TOKEN_KEYWORD_TYPEDEF,
	HCC_TOKEN_KEYWORD_STATIC,
	HCC_TOKEN_KEYWORD_CONST,
	HCC_TOKEN_KEYWORD_AUTO,
	HCC_TOKEN_KEYWORD_REGISTER,
	HCC_TOKEN_KEYWORD_VOLATILE,
	HCC_TOKEN_KEYWORD_EXTERN,
	HCC_TOKEN_KEYWORD_INLINE,
	HCC_TOKEN_KEYWORD_NO_RETURN,
	HCC_TOKEN_KEYWORD_SIZEOF,
	HCC_TOKEN_KEYWORD_ALIGNOF,
	HCC_TOKEN_KEYWORD_ALIGNAS,
	HCC_TOKEN_KEYWORD_STATIC_ASSERT,
	HCC_TOKEN_KEYWORD_RESTRICT,
	HCC_TOKEN_KEYWORD_INTRINSIC,
	HCC_TOKEN_KEYWORD_RASTERIZER_STATE,
	HCC_TOKEN_KEYWORD_FRAGMENT_STATE,
	HCC_TOKEN_KEYWORD_POSITION,
	HCC_TOKEN_KEYWORD_NOINTERP,
	HCC_TOKEN_KEYWORD_RO_BUFFER,
	HCC_TOKEN_KEYWORD_RW_BUFFER,
	HCC_TOKEN_KEYWORD_RO_IMAGE1D,
	HCC_TOKEN_KEYWORD_RW_IMAGE1D,
	HCC_TOKEN_KEYWORD_RO_IMAGE2D,
	HCC_TOKEN_KEYWORD_RW_IMAGE2D,
	HCC_TOKEN_KEYWORD_RO_IMAGE3D,
	HCC_TOKEN_KEYWORD_RW_IMAGE3D,
#define HCC_TOKEN_KEYWORDS_END HCC_TOKEN_COUNT
#define HCC_TOKEN_KEYWORDS_COUNT (HCC_TOKEN_KEYWORDS_END - HCC_TOKEN_KEYWORDS_START)
#define HCC_TOKEN_IS_KEYWORD(token) (HCC_TOKEN_KEYWORDS_START <= (token) && (token) < HCC_TOKEN_KEYWORDS_END)
#define HCC_TOKEN_IS_LIT_NUMBER(token) (HCC_TOKEN_LIT_NUMBERS_START <= (token) && (token) < HCC_TOKEN_LIT_NUMBERS_END)

	HCC_TOKEN_COUNT,
};

typedef union HccTokenValue HccTokenValue;
union HccTokenValue {
	HccConstantId constant_id;
	HccStringId   string_id;
	U32           macro_param_idx;
};

static_assert(sizeof(HccTokenValue) == sizeof(U32), "HccTokenValue has been designed around being 32 bits!");

typedef struct HccTokenBag HccTokenBag;
struct HccTokenBag {
	HccStack(HccToken)      tokens;
	HccStack(U32)           token_location_indices;
	HccStack(HccTokenValue) token_values;
};

typedef struct HccTokenCursor HccTokenCursor;
struct HccTokenCursor {
	U32         tokens_start_idx;
	U32         tokens_end_idx;
	U32         token_idx;
	U32         token_value_idx;
};

extern char* hcc_token_strings[HCC_TOKEN_COUNT];

U32 hcc_token_num_values(HccToken token);
bool hcc_token_concat_is_okay(HccToken before, HccToken after);

U32 hcc_token_cursor_tokens_count(HccTokenCursor* cursor);
HccLocation* hcc_token_bag_location_get(HccCompiler* c, HccTokenBag* bag, U32 token_idx);
void hcc_token_bag_stringify_single(HccCompiler* c, HccTokenBag* bag, HccTokenCursor* cursor, HccPPMacro* macro);
HccToken hcc_token_bag_stringify_single_or_macro_param(HccCompiler* c, HccTokenBag* bag, HccTokenCursor* cursor, U32 args_start_idx, HccTokenBag* args_src_bag, bool false_before_true_after);
HccStringId hcc_token_bag_stringify_range(HccCompiler* c, HccTokenBag* bag, HccTokenCursor* cursor, HccPPMacro* macro);

// ===========================================
//
//
// Preprocessor
//
//
// ===========================================

#define HCC_PP_TOKEN_IS_PREEXPANDED_MACRO_ARG_MASK 0x80000000

struct HccPPMacro {
	HccStringId identifier_string_id;
	HccLocation location;
	HccTokenCursor token_cursor;
	U32 params_start_idx: 23;
	U32 params_count: 8;
	U32 is_function: 1;
	U32 has_va_args: 1;
};

typedef U32 HccPPPredefinedMacro;
enum {
	HCC_PP_PREDEFINED_MACRO___FILE__,
	HCC_PP_PREDEFINED_MACRO___LINE__,
	HCC_PP_PREDEFINED_MACRO___COUNTER__,
	HCC_PP_PREDEFINED_MACRO___HCC__,
	HCC_PP_PREDEFINED_MACRO___HCC_GPU__,
	HCC_PP_PREDEFINED_MACRO___HCC_X86_64__,
	HCC_PP_PREDEFINED_MACRO___HCC_LINUX__,
	HCC_PP_PREDEFINED_MACRO___HCC_WINDOWS__,

	HCC_PP_PREDEFINED_MACRO_COUNT,
};

typedef U8 HccPPDirective;
enum {
	HCC_PP_DIRECTIVE_DEFINE,
	HCC_PP_DIRECTIVE_UNDEF,
	HCC_PP_DIRECTIVE_INCLUDE,
	HCC_PP_DIRECTIVE_IF,
	HCC_PP_DIRECTIVE_IFDEF,
	HCC_PP_DIRECTIVE_IFNDEF,
	HCC_PP_DIRECTIVE_ELSE,
	HCC_PP_DIRECTIVE_ELIF,
	HCC_PP_DIRECTIVE_ELIFDEF,
	HCC_PP_DIRECTIVE_ELIFNDEF,
	HCC_PP_DIRECTIVE_ENDIF,
	HCC_PP_DIRECTIVE_LINE,
	HCC_PP_DIRECTIVE_ERROR,
	HCC_PP_DIRECTIVE_WARNING,
	HCC_PP_DIRECTIVE_PRAGMA,

	HCC_PP_DIRECTIVE_COUNT,
};

//
// this represents the span from any preprocessor conditional (#if, #elif, #else, #endif etc)
// to the next preprocessor conditional.
typedef struct HccPPIfSpan HccPPIfSpan;
struct HccPPIfSpan {
	HccPPDirective directive;
	HccLocation    location;
	U32            first_id: 31; // a link to the span that is the original #if/n/def
	U32            has_else: 1;
	U32            prev_id;
	U32            next_id;
	U32            last_id; // set when this is the original #if/n/def to link to the matching #endif
};

typedef struct HccPPMacroArg HccPPMacroArg;
struct HccPPMacroArg {
	HccTokenCursor cursor;
	HccLocation* callsite_location;
};

typedef struct HccPPExpand HccPPExpand;
struct HccPPExpand {
	HccPPMacro*    macro;
	HccTokenCursor cursor;
};

typedef struct HccPPEval HccPPEval;
struct HccPPEval {
	union {
		U64 u64;
		S64 s64;
	};
	bool is_signed;
};

typedef struct HccPP HccPP;
struct HccPP {
	HccTokenBag                    macro_token_bag;
	HccStack(HccPPMacro)           macros;
	HccStack(HccStringId)          macro_params;
	HccStack(HccPPMacroArg)        macro_args_stack;
	HccStack(HccPPExpand)          expand_stack;
	HccStack(U32)                  expand_macro_idx_stack;
	HccStack(char)                 stringify_buffer;
	HccStack(HccPPIfSpan*)         if_span_stack;
	HccHashTable(HccStringId, U32) macro_declarations;
};

typedef struct HccPPSetup HccPPSetup;
struct HccPPSetup {
	U32 macro_tokens_cap;
	U32 macro_token_values_cap;
	U32 macros_cap;
	U32 macro_params_cap;
	U32 macro_args_stack_cap;
	U32 expand_stack_cap;
	U32 stringify_buffer_cap;
	U32 if_stack_cap;
};

typedef U8 HccPPExpandFlags;
enum {
	HCC_PP_EXPAND_FLAGS_DEST_IS_ORIGINAL_LOCATION = 0x1,
	HCC_PP_EXPAND_FLAGS_IS_ARGS                   = 0x2,
	HCC_PP_EXPAND_FLAGS_DEST_IS_ARGS              = 0x4,
};

char* hcc_pp_predefined_macro_identifier_strings[HCC_PP_PREDEFINED_MACRO_COUNT];
char* hcc_pp_directive_enum_strings[HCC_PP_DIRECTIVE_COUNT];
char* hcc_pp_directive_strings[HCC_PP_DIRECTIVE_COUNT];
U32 hcc_pp_directive_hashes[HCC_PP_DIRECTIVE_COUNT];

void hcc_pp_init(HccCompiler* c, HccCompilerSetup* setup);
HccPPMacro* hcc_pp_macro_get(HccCompiler* c, U32 macro_idx);
HccPPIfSpan* hcc_pp_if_span_get(HccCompiler* c, U32 if_span_id);
U32 hcc_pp_if_span_id(HccCompiler* c, HccPPIfSpan* if_span);
HccPPIfSpan* hcc_pp_if_span_push(HccCompiler* c, HccPPDirective directive);
void hcc_pp_if_found_if(HccCompiler* c, HccPPDirective directive);
HccPPIfSpan* hcc_pp_if_found_if_counterpart(HccCompiler* c, HccPPDirective directive);
void hcc_pp_if_found_endif(HccCompiler* c);
void hcc_pp_if_found_else(HccCompiler* c, HccPPDirective directive);
void hcc_pp_if_ensure_first_else(HccCompiler* c, HccPPDirective directive);
void hcc_pp_if_ensure_one_is_open(HccCompiler* c, HccPPDirective directive);

void hcc_pp_eval_binary_op(HccCompiler* c, U32* token_idx_mut, HccBinaryOp* binary_op_type_out, U32* precedence_out);
HccPPEval hcc_pp_eval_unary_expr(HccCompiler* c, U32* token_idx_mut, U32* token_value_idx_mut);
HccPPEval hcc_pp_eval_expr(HccCompiler* c, U32 min_precedence, U32* token_idx_mut, U32* token_value_idx_mut);
void hcc_pp_ensure_end_of_directive(HccCompiler* c, HccErrorCode error_code, HccPPDirective directive);

void hcc_pp_parse_define(HccCompiler* c);
void hcc_pp_parse_undef(HccCompiler* c);
void hcc_pp_parse_include(HccCompiler* c);
bool hcc_pp_parse_if(HccCompiler* c);
void hcc_pp_parse_defined(HccCompiler* c);
bool hcc_pp_parse_ifdef(HccCompiler* c, HccPPDirective directive);
void hcc_pp_parse_line(HccCompiler* c);
void hcc_pp_parse_error(HccCompiler* c);
void hcc_pp_parse_pragma(HccCompiler* c);
HccPPDirective hcc_pp_parse_directive_header(HccCompiler* c);
void hcc_pp_parse_directive(HccCompiler* c);
void hcc_pp_skip_false_conditional(HccCompiler* c, bool is_skipping_until_endif);
void hcc_pp_copy_expand_predefined_macro(HccCompiler* c, HccPPPredefinedMacro predefined_macro);
void hcc_pp_copy_expand_macro_begin(HccCompiler* c, HccPPMacro* macro, HccLocation* macro_callsite_location);
bool hcc_pp_is_callable_macro(HccCompiler* c, HccStringId ident_string_id, U32* macro_idx_out);
HccPPExpand* hcc_pp_expand_push_macro(HccCompiler* c, HccPPMacro* macro);
HccPPExpand* hcc_pp_expand_push_macro_arg(HccCompiler* c, U32 param_idx, U32 args_start_idx, HccLocation** callsite_location_out);
void hcc_pp_expand_pop(HccCompiler* c, HccPPExpand* expected_expand);
void hcc_pp_copy_expand_range(HccCompiler* c, HccPPExpand* expand, HccTokenBag* dst_bag, HccTokenBag* src_bag, HccTokenBag* alt_dst_bag, HccLocation* parent_or_child_location, HccLocation* grandparent_location, HccPPExpandFlags flags, HccPPMacro* expand_macro);
void hcc_pp_copy_expand_macro(HccCompiler* c, HccPPMacro* macro, HccLocation* macro_callsite_location, HccLocation* parent_location, HccPPExpand* arg_expand, HccTokenBag* args_src_bag, HccTokenBag* dst_bag, HccTokenBag* alt_dst_bag, HccPPExpandFlags flags);
U32 hcc_pp_process_macro_args(HccCompiler* c, HccPPMacro* macro, HccPPExpand* expand, HccTokenBag* src_bag, HccLocation* parent_location);
HccPPMacroArg* hcc_pp_push_macro_arg(HccCompiler* c, HccPPExpand* expand, HccTokenBag* src_bag, HccLocation* parent_location);
void hcc_pp_finalize_macro_arg(HccCompiler* c, HccPPMacroArg* arg, HccPPExpand* expand, HccTokenBag* src_bag);
void hcc_pp_attach_to_most_parent(HccCompiler* c, HccLocation* location, HccLocation* parent_location);

// ===========================================
//
//
// Token Generator
//
//
// ===========================================

typedef struct HccOpenBracket HccOpenBracket;
struct HccOpenBracket {
	HccToken close_token;
	HccLocation* open_token_location;
};

typedef U8 HccTokenGenRunMode;
enum {
	HCC_TOKENGEN_RUN_MODE_CODE,
	HCC_TOKENGEN_RUN_MODE_PP_DEFINE_REPLACEMENT_LIST,
	HCC_TOKENGEN_RUN_MODE_PP_INCLUDE_OPERAND,
	HCC_TOKENGEN_RUN_MODE_PP_IF_OPERAND,
	HCC_TOKENGEN_RUN_MODE_PP_OPERAND,
	HCC_TOKENGEN_RUN_MODE_PP_MACRO_ARGS,
	HCC_TOKENGEN_RUN_MODE_PP_CONCAT,
};

typedef struct HccPausedFile HccPausedFile;
struct HccPausedFile {
	U32         if_span_stack_count;
	HccLocation location;
};

typedef struct HccTokenGen HccTokenGen;
struct HccTokenGen {
	HccTokenGenRunMode       run_mode;
	HccTokenBag              token_bag;
	HccTokenBag*             dst_token_bag;
	HccStack(HccPausedFile)  paused_file_stack;
	HccStack(HccOpenBracket) open_bracket_stack;
	HccStack(HccLocation)    token_locations;

	//
	// data used when run_mode == HCC_TOKENGEN_RUN_MODE_PP_DEFINE_REPLACEMENT_LIST
	bool                     macro_is_function;
	bool                     macro_has_va_arg;
	HccStringId*             macro_param_string_ids;
	U32                      macro_params_count;
	U32                      macro_tokens_start_idx;

	HccLocation              location;
	char*                    code;      // is a local copy of location.code_file->code.data
	U32                      code_size; // is a local copy of location.code_file->code.size
	U32                      custom_line_dst;
	U32                      custom_line_src;

	S32 __counter__;
};

typedef struct HccTokenGenSetup HccTokenGenSetup;
struct HccTokenGenSetup {
	U32 tokens_cap;
	U32 token_values_cap;
	U32 token_locations_cap;
	U32 paused_file_stack_cap;
	U32 open_bracket_stack_cap;
};

void hcc_tokengen_init(HccCompiler* c, HccCompilerSetup* setup);
void hcc_tokengen_advance_column(HccCompiler* c, U32 by);
void hcc_tokengen_advance_newline(HccCompiler* c);
U32 hcc_tokengen_display_line(HccCompiler* c);
U32 hcc_tokengen_token_add(HccCompiler* c, HccToken token);
U32 hcc_tokengen_token_value_add(HccCompiler* c, HccTokenValue value);
void hcc_tokengen_count_extra_newlines(HccCompiler* c);
noreturn void hcc_tokengen_bail_error_1(HccCompiler* c, HccErrorCode error_code, ...);
noreturn void hcc_tokengen_bail_error_2_idx(HccCompiler* c, HccErrorCode error_code, U32 other_token_idx, ...);
noreturn void hcc_tokengen_bail_error_2_ptr(HccCompiler* c, HccErrorCode error_code, HccLocation* token_location, HccLocation* other_token_location, ...);
void hcc_tokengen_location_push(HccCompiler* c);
void hcc_tokengen_location_pop(HccCompiler* c);
void hcc_tokengen_location_setup_new_file(HccCompiler* c, HccCodeFile* code_file, bool change_dir);

bool hcc_tokengen_consume_backslash(HccCompiler* c);
void hcc_tokengen_consume_whitespace(HccCompiler* c);
void hcc_tokengen_consume_whitespace_and_newlines(HccCompiler* c);
void hcc_tokengen_consume_until_any_byte(HccCompiler* c, char* terminator_bytes);

HccString hcc_tokengen_parse_ident_from_string(HccCompiler* c, HccString string, HccErrorCode error_code);
HccString hcc_tokengen_parse_ident(HccCompiler* c, HccErrorCode error_code);


U32 hcc_tokengen_parse_num(HccCompiler* c, HccToken* token_out);
void hcc_tokengen_parse_string(HccCompiler* c, char terminator_byte, bool ignore_escape_sequences_except_double_quotes);
U32 hcc_tokengen_find_macro_param(HccCompiler* c, HccStringId ident_string_id);
void hcc_tokengen_consume_hash_for_define_replacement_list(HccCompiler* c);
bool hcc_tokengen_is_first_non_whitespace_on_line(HccCompiler* c);
void hcc_tokengen_bracket_open(HccCompiler* c, HccToken token, HccLocation* location);
void hcc_tokengen_bracket_close(HccCompiler* c, HccToken token, HccLocation* location);
void hcc_tokengen_run(HccCompiler* c, HccTokenBag* dst_token_bag, HccTokenGenRunMode run_mode);

void hcc_tokengen_print(HccCompiler* c, FILE* f);

// ===========================================
//
//
// Syntax Generator
//
//
// ===========================================

typedef U8 HccUnaryOp;
enum {
	HCC_UNARY_OP_LOGICAL_NOT,
	HCC_UNARY_OP_BIT_NOT,
	HCC_UNARY_OP_PLUS,
	HCC_UNARY_OP_NEGATE,
	HCC_UNARY_OP_PRE_INCREMENT,
	HCC_UNARY_OP_PRE_DECREMENT,
	HCC_UNARY_OP_POST_INCREMENT,
	HCC_UNARY_OP_POST_DECREMENT,

	HCC_UNARY_OP_COUNT,
};

typedef U8 HccExprType;
enum {
	HCC_EXPR_TYPE_NONE,

	HCC_EXPR_TYPE_CALL,
	HCC_EXPR_TYPE_ARRAY_SUBSCRIPT,
	HCC_EXPR_TYPE_CURLY_INITIALIZER,
	HCC_EXPR_TYPE_DESIGNATED_INITIALIZER,
	HCC_EXPR_TYPE_FIELD_ACCESS,
	HCC_EXPR_TYPE_CAST,

	HCC_EXPR_TYPE_LOCAL_VARIABLE,
	HCC_EXPR_TYPE_GLOBAL_VARIABLE,

	HCC_EXPR_TYPE_CONSTANT,
	HCC_EXPR_TYPE_DATA_TYPE,
	HCC_EXPR_TYPE_FUNCTION,
	HCC_EXPR_TYPE_CALL_ARG_LIST,
	HCC_EXPR_TYPE_STMT_IF,
	HCC_EXPR_TYPE_STMT_SWITCH,
	HCC_EXPR_TYPE_STMT_WHILE,
	HCC_EXPR_TYPE_STMT_FOR,
	HCC_EXPR_TYPE_STMT_CASE,
	HCC_EXPR_TYPE_STMT_DEFAULT,
	HCC_EXPR_TYPE_STMT_BREAK,
	HCC_EXPR_TYPE_STMT_CONTINUE,

	HCC_EXPR_TYPE_STMT_RETURN,
	HCC_EXPR_TYPE_STMT_BLOCK,

	//
	// binary ops
	HCC_EXPR_TYPE_BINARY_OP_START,
#define HCC_EXPR_TYPE_BINARY_OP(OP) (HCC_EXPR_TYPE_BINARY_OP_START + HCC_BINARY_OP_##OP)
	HCC_EXPR_TYPE_BINARY_OP_END = HCC_EXPR_TYPE_BINARY_OP_START + HCC_BINARY_OP_COUNT,

	//
	// unary ops
	HCC_EXPR_TYPE_UNARY_OP_START,
#define HCC_EXPR_TYPE_UNARY_OP(OP) (HCC_EXPR_TYPE_UNARY_OP_START + HCC_UNARY_OP_##OP)
	HCC_EXPR_TYPE_UNARY_OP_END = HCC_EXPR_TYPE_UNARY_OP_START + HCC_UNARY_OP_COUNT,
};

typedef struct HccExpr HccExpr;
struct HccExpr {
	union {
		//
		// shared header
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
		};

		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 idx;
		} function;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 expr_rel_idx;
		} unary;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 is_assignment: 1;
			U32 left_expr_rel_idx: 24;
			U32 right_expr_rel_idx;
		} binary;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 id;
		} constant;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 has_return_stmt: 1;
			U32 first_expr_rel_idx: 12;
			U32 variables_count: 12;
			U32 stmts_count;
		} stmt_block;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 first_expr_rel_idx;
		} curly_initializer;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 value_expr_rel_idx;
			// alt_next_expr_rel_idx is the designated_initializer_idx into HccAstGenCurlyInitializer.designated_initializers
		} designated_initializer;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 cond_expr_rel_idx: 25;
			U32 true_stmt_rel_idx;
		} if_;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 cond_expr_rel_idx: 25;
			U32 block_expr_rel_idx;
			// alt_next_expr_rel_idx is the default_case_expr_rel_idx
		} switch_;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 cond_expr_rel_idx: 25;
			U32 loop_stmt_rel_idx;
		} while_;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 init_expr_rel_idx: 12;
			U32 cond_expr_rel_idx: 13;
			U32 inc_expr_rel_idx: 16;
			U32 loop_stmt_rel_idx: 16;
		} for_;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 idx;
		} variable;
		struct {
			U32 type: 6; // HccExprType
			U32 is_stmt_block_entry: 1;
			U32 false_expr_rel_idx: 25;
			U32 cond_expr_rel_idx: 13;
			U32 true_expr_rel_idx: 19;
		} ternary;
	};

	union {
		struct { // if is_stmt_block_entry
			U16 alt_next_expr_rel_idx;
			U16 next_expr_rel_idx;
		};
		struct { // this is stored in the true statement of the if statement
			U32 false_stmt_rel_idx: 16;
			U32 true_and_false_stmts_have_return_stmt: 1;
			U32 __unused: 15;
		} if_aux;
		struct {
			U32 case_stmts_count: 17;
			U32 first_case_expr_rel_idx: 15;
		} switch_aux;
		HccDataType data_type;
	};
};

static_assert(sizeof(HccExpr) == 12, "HccExpr must be 12 bytes");

typedef struct HccSwitchState HccSwitchState;
struct HccSwitchState {
	HccExpr* switch_stmt;
	HccExpr* first_switch_case;
	HccExpr* prev_switch_case;
	HccExpr* default_switch_case;
	HccDataType switch_condition_type;
	U32 case_stmts_count;
};

typedef struct HccFieldAccess HccFieldAccess;
struct HccFieldAccess {
	HccDataType data_type;
	U32 idx;
};

typedef struct HccAstGenCurlyInitializerCurly HccAstGenCurlyInitializerCurly;
struct HccAstGenCurlyInitializerCurly {
	U32 nested_elmts_start_idx;
	bool found_designator;
};

typedef struct HccAstGenCurlyInitializerElmt  HccAstGenCurlyInitializerElmt;
struct HccAstGenCurlyInitializerElmt {
	HccDataType data_type; // this is the outer data type that we are initializing at this nested layer
	HccDataType resolved_data_type;
	U64 elmt_idx: 63; // this is the element index into the outer data type that we are initializing
	U64 had_explicit_designator_for_union_field: 1;
};

typedef struct HccAstGenDesignatorInitializer HccAstGenDesignatorInitializer;
struct HccAstGenDesignatorInitializer {
	U32 elmt_indices_start_idx;
	U32 elmt_indices_count;
};

typedef struct HccAstGenCurlyInitializerNested HccAstGenCurlyInitializerNested;
struct HccAstGenCurlyInitializerNested {
	HccExpr* prev_initializer_expr;
	HccExpr* first_initializer_expr;
	U32      nested_elmts_start_idx;
};

typedef struct HccAstGenCurlyInitializer HccAstGenCurlyInitializer;
struct HccAstGenCurlyInitializer {
	union {
		HccCompoundDataType* compound_data_type;
		HccArrayDataType* array_data_type;
	};
	HccCompoundField* compound_fields;
	HccDataType composite_data_type;
	HccDataType elmt_data_type;
	HccDataType resolved_composite_data_type;
	HccDataType resolved_elmt_data_type;
	U64 elmts_end_idx;

	//
	// a stack to keep track the nested curly initializer expressions.
	// these can happen in variable declaration or for compound literals.
	HccStack(HccAstGenCurlyInitializerNested) nested;

	//
	// a stack to keep track of when we open a new set of curly braces
	// and how to return back to the parent pair of curly braces
	HccStack(HccAstGenCurlyInitializerCurly) nested_curlys;

	//
	// a stack to keep track of each nested elements when we tunnel into nested data types
	// so we can tunnel out and resume from where we were
	HccStack(HccAstGenCurlyInitializerElmt) nested_elmts;

	HccExpr* prev_initializer_expr;
	HccExpr* first_initializer_expr;
	U32      nested_elmts_start_idx;

	//
	// each HCC_EXPR_TYPE_DESIGNATED_INITIALIZER node will reference
	// the designated_initializers array
	HccStack(HccAstGenDesignatorInitializer) designated_initializers;
	HccStack(U64) designated_initializer_elmt_indices;
};

typedef U8 HccSpecifier;
enum {
	HCC_SPECIFIER_STATIC,
	HCC_SPECIFIER_CONST,
	HCC_SPECIFIER_INLINE,
	HCC_SPECIFIER_NO_RETURN,

	HCC_SPECIFIER_INTRINSIC,
	HCC_SPECIFIER_RASTERIZER_STATE,
	HCC_SPECIFIER_FRAGMENT_STATE,
	HCC_SPECIFIER_POSITION,
	HCC_SPECIFIER_NOINTERP,

	HCC_SPECIFIER_VERTEX,
	HCC_SPECIFIER_FRAGMENT,

	HCC_SPECIFIER_COUNT,
};

typedef U16 HccSpecifierFlags;
enum {
	HCC_SPECIFIER_FLAGS_STATIC =           1 << HCC_SPECIFIER_STATIC,
	HCC_SPECIFIER_FLAGS_CONST =            1 << HCC_SPECIFIER_CONST,
	HCC_SPECIFIER_FLAGS_INLINE =           1 << HCC_SPECIFIER_INLINE,
	HCC_SPECIFIER_FLAGS_NO_RETURN =        1 << HCC_SPECIFIER_NO_RETURN,

	HCC_SPECIFIER_FLAGS_INTRINSIC =        1 << HCC_SPECIFIER_INTRINSIC,
	HCC_SPECIFIER_FLAGS_RASTERIZER_STATE = 1 << HCC_SPECIFIER_RASTERIZER_STATE,
	HCC_SPECIFIER_FLAGS_FRAGMENT_STATE =   1 << HCC_SPECIFIER_FRAGMENT_STATE,
	HCC_SPECIFIER_FLAGS_POSITION =         1 << HCC_SPECIFIER_POSITION,
	HCC_SPECIFIER_FLAGS_NOINTERP =         1 << HCC_SPECIFIER_NOINTERP,

	HCC_SPECIFIER_FLAGS_VERTEX =           1 << HCC_SPECIFIER_VERTEX,
	HCC_SPECIFIER_FLAGS_FRAGMENT =         1 << HCC_SPECIFIER_FRAGMENT,

	HCC_SPECIFIER_FLAGS_ALL_SHADER_STAGES = HCC_SPECIFIER_FLAGS_VERTEX | HCC_SPECIFIER_FLAGS_FRAGMENT,

	HCC_SPECIFIER_FLAGS_ALL_VARIABLE_SPECIFIERS = HCC_SPECIFIER_FLAGS_STATIC | HCC_SPECIFIER_FLAGS_CONST,
	HCC_SPECIFIER_FLAGS_ALL_FUNCTION_SPECIFIERS =
			HCC_SPECIFIER_FLAGS_INTRINSIC |
			HCC_SPECIFIER_FLAGS_STATIC    |
			HCC_SPECIFIER_FLAGS_CONST     |
			HCC_SPECIFIER_FLAGS_INLINE    |
			HCC_SPECIFIER_FLAGS_NO_RETURN |
			HCC_SPECIFIER_FLAGS_ALL_SHADER_STAGES,
	HCC_SPECIFIER_FLAGS_ALL_FUNCTION_PARAM_SPECIFIERS = HCC_SPECIFIER_FLAGS_CONST,
	HCC_SPECIFIER_FLAGS_ALL_STRUCT_SPECIFIERS = HCC_SPECIFIER_FLAGS_INTRINSIC | HCC_SPECIFIER_FLAGS_RASTERIZER_STATE | HCC_SPECIFIER_FLAGS_FRAGMENT_STATE,
	HCC_SPECIFIER_FLAGS_ALL_STRUCT_FIELD_SPECIFIERS = HCC_SPECIFIER_FLAGS_POSITION | HCC_SPECIFIER_FLAGS_NOINTERP,
	HCC_SPECIFIER_FLAGS_ALL_TYPEDEF_SPECIFIER = HCC_SPECIFIER_FLAGS_INTRINSIC,

	HCC_SPECIFIER_FLAGS_ALL =
		HCC_SPECIFIER_FLAGS_ALL_VARIABLE_SPECIFIERS     |
		HCC_SPECIFIER_FLAGS_ALL_FUNCTION_SPECIFIERS     |
		HCC_SPECIFIER_FLAGS_ALL_STRUCT_SPECIFIERS       |
		HCC_SPECIFIER_FLAGS_ALL_STRUCT_FIELD_SPECIFIERS |
		HCC_SPECIFIER_FLAGS_ALL_TYPEDEF_SPECIFIER       ,

	HCC_SPECIFIER_FLAGS_ALL_NON_VARIABLE_SPECIFIERS =      HCC_SPECIFIER_FLAGS_ALL & ~HCC_SPECIFIER_FLAGS_ALL_VARIABLE_SPECIFIERS,
	HCC_SPECIFIER_FLAGS_ALL_NON_FUNCTION_SPECIFIERS =      HCC_SPECIFIER_FLAGS_ALL & ~HCC_SPECIFIER_FLAGS_ALL_FUNCTION_SPECIFIERS,
	HCC_SPECIFIER_FLAGS_ALL_NON_FUNCTION_PARAM_SPECIFIERS = HCC_SPECIFIER_FLAGS_ALL & ~HCC_SPECIFIER_FLAGS_ALL_FUNCTION_PARAM_SPECIFIERS,
	HCC_SPECIFIER_FLAGS_ALL_NON_STRUCT_SPECIFIERS =        HCC_SPECIFIER_FLAGS_ALL & ~HCC_SPECIFIER_FLAGS_ALL_STRUCT_SPECIFIERS,
	HCC_SPECIFIER_FLAGS_ALL_NON_STRUCT_FIELD_SPECIFIERS =  HCC_SPECIFIER_FLAGS_ALL & ~HCC_SPECIFIER_FLAGS_ALL_STRUCT_FIELD_SPECIFIERS,
	HCC_SPECIFIER_FLAGS_ALL_NON_TYPEDEF_SPECIFIERS =       HCC_SPECIFIER_FLAGS_ALL & ~HCC_SPECIFIER_FLAGS_ALL_TYPEDEF_SPECIFIER,
};

typedef U16 HccTypeSpecfier;
enum {
	HCC_TYPE_SPECIFIER_VOID =          0x1,
	HCC_TYPE_SPECIFIER_BOOL =          0x2,
	HCC_TYPE_SPECIFIER_CHAR =          0x4,
	HCC_TYPE_SPECIFIER_SHORT =         0x8,
	HCC_TYPE_SPECIFIER_INT =           0x10,
	HCC_TYPE_SPECIFIER_LONG =          0x20,
	HCC_TYPE_SPECIFIER_LONGLONG =      0x40,
	HCC_TYPE_SPECIFIER_FLOAT =         0x80,
	HCC_TYPE_SPECIFIER_DOUBLE =        0x100,
	HCC_TYPE_SPECIFIER_UNSIGNED =      0x200,
	HCC_TYPE_SPECIFIER_SIGNED =        0x400,
	HCC_TYPE_SPECIFIER_COMPLEX =       0x800,
	HCC_TYPE_SPECIFIER_ATOMIC =        0x1000,
	HCC_TYPE_SPECIFIER_GENERIC_FLOAT = 0x2000,

	HCC_TYPE_SPECIFIER_TYPES =
		HCC_TYPE_SPECIFIER_VOID  |
		HCC_TYPE_SPECIFIER_BOOL  |
		HCC_TYPE_SPECIFIER_CHAR  |
		HCC_TYPE_SPECIFIER_SHORT |
		HCC_TYPE_SPECIFIER_INT   |
		HCC_TYPE_SPECIFIER_LONG  |
		HCC_TYPE_SPECIFIER_FLOAT |
		HCC_TYPE_SPECIFIER_DOUBLE,

	HCC_TYPE_SPECIFIER_FLOAT_TYPES =
		HCC_TYPE_SPECIFIER_FLOAT        |
		HCC_TYPE_SPECIFIER_DOUBLE       |
		HCC_TYPE_SPECIFIER_GENERIC_FLOAT,

	HCC_TYPE_SPECIFIER_LONG_DOUBLE = HCC_TYPE_SPECIFIER_LONG | HCC_TYPE_SPECIFIER_DOUBLE,
	HCC_TYPE_SPECIFIER_UNSIGNED_SIGNED = HCC_TYPE_SPECIFIER_UNSIGNED | HCC_TYPE_SPECIFIER_SIGNED,
};

typedef struct HccAstGen HccAstGen;
struct HccAstGen {
	HccSpecifierFlags specifier_flags;

	HccTokenBag token_bag;
	U32 token_read_idx;
	U32 token_value_read_idx;

	HccStack(HccVariable)                  function_params_and_variables;
	HccStack(HccFunction)                  functions;
	HccStack(U32)                          function_used_function_indices;
	HccStack(HccDecl)                      function_used_static_variables;
	HccStack(U32)                          entry_point_function_indices;
	HccStack(U32)                          used_function_indices;
	HccStack(U32)                          recursive_function_indices_stack;
	HccStack(HccExpr)                      exprs;
	HccStack(HccLocation)                  expr_locations;
	HccStack(HccVariable)                  global_variables;
	HccStack(HccArrayDataType)             array_data_types;
	HccStack(HccCompoundDataType)          compound_data_types;
	HccStack(HccCompoundField)             compound_fields;
	HccStack(HccTypedef)                   typedefs;
	HccStack(HccEnumDataType)              enum_data_types;
	HccStack(HccEnumValue)                 enum_values;
	HccStack(HccDataType)                  ordered_data_types;
	HccStack(HccFieldAccess)               compound_type_find_fields;
	HccHashTable(HccStringId, HccDecl)     global_declarations;
	HccHashTable(HccStringId, HccDataType) struct_declarations;
	HccHashTable(HccStringId, HccDataType) union_declarations;
	HccHashTable(HccStringId, HccDataType) enum_declarations;

	HccAstGenCurlyInitializer curly_initializer;

	HccDataType assign_data_type;

	HccExpr* stmt_block;
	HccFunction* function;
	U32 print_variable_base_idx;

	HccSwitchState switch_state;
	bool is_in_loop;

	HccStack(HccStringId) variable_stack_strings;
	HccStack(U32)         variable_stack_var_indices;
	U32                   next_var_idx;

	HccHashTable(HccStringId, U32) field_name_to_token_idx;
};

typedef struct HccAstGenSetup HccAstGenSetup;
struct HccAstGenSetup {
	U32 function_params_and_variables_cap;
	U32 functions_cap;
	U32 exprs_cap;
	U32 expr_locations_cap;
	U32 global_variables_cap;
	U32 function_used_function_indices_cap;
	U32 function_used_static_variables_cap;
	U32 entry_point_function_indices_cap;
	U32 used_function_indices_cap;
	U32 recursive_function_indices_stack_cap;
	U32 array_data_types_cap;
	U32 struct_data_types_cap;
	U32 union_data_types_cap;
	U32 compound_fields_cap;
	U32 compound_type_fields_cap;
	U32 typedefs_cap;
	U32 enum_data_types_cap;
	U32 enum_values_cap;
	U32 ordered_data_types_cap;
	U32 compound_type_find_fields_cap;
	U32 curly_initializer_nested_cap;
	U32 curly_initializer_nested_curlys_cap;
	U32 curly_initializer_nested_elmts_cap;
	U32 curly_initializer_designator_initializers_cap;
	U32 curly_initializer_designator_initializer_elmt_indices_cap;
	U32 variable_stack_cap;
};

extern HccToken hcc_specifier_tokens[HCC_SPECIFIER_COUNT];

void hcc_astgen_init(HccCompiler* c, HccCompilerSetup* setup);

void hcc_astgen_error_1(HccCompiler* c, HccErrorCode error_code, ...);
void hcc_astgen_error_2_idx(HccCompiler* c, HccErrorCode error_code, U32 other_token_idx, ...);
void hcc_astgen_error_2_ptr(HccCompiler* c, HccErrorCode error_code, HccLocation* other_location, ...);
void hcc_astgen_warn_1(HccCompiler* c, HccWarnCode warn_code, ...);
void hcc_astgen_warn_2_idx(HccCompiler* c, HccWarnCode warn_code, U32 other_token_idx, ...);
void hcc_astgen_warn_2_ptr(HccCompiler* c, HccWarnCode warn_code, HccLocation* other_location, ...);
noreturn void hcc_astgen_bail_error_1(HccCompiler* c, HccErrorCode error_code, ...);
noreturn void hcc_astgen_bail_error_1_merge_apply(HccCompiler* c, HccErrorCode error_code, HccLocation* location, ...);
noreturn void hcc_astgen_bail_error_2_idx(HccCompiler* c, HccErrorCode error_code, U32 other_token_idx, ...);
noreturn void hcc_astgen_bail_error_2_ptr(HccCompiler* c, HccErrorCode error_code, HccLocation* other_location, ...);

HccToken hcc_astgen_token_peek(HccCompiler* c);
HccToken hcc_astgen_token_peek_ahead(HccCompiler* c, U32 by);
void hcc_astgen_token_consume(HccCompiler* c, U32 amount);
HccToken hcc_astgen_token_next(HccCompiler* c);
void hcc_astgen_token_value_consume(HccCompiler* c, U32 amount);
HccTokenValue hcc_astgen_token_value_peek(HccCompiler* c);
HccTokenValue hcc_astgen_token_value_next(HccCompiler* c);

void hcc_astgen_data_type_found(HccCompiler* c, HccDataType data_type);
HccCompoundField* hcc_astgen_compound_data_type_find_field_by_name(HccCompiler* c, HccCompoundDataType* compound_data_type, HccStringId identifier_string_id);
HccCompoundField* hcc_astgen_compound_data_type_find_field_by_name_checked(HccCompiler* c, HccDataType data_type, HccCompoundDataType* compound_data_type, HccStringId identifier_string_id);
HccCompoundField* hcc_astgen_compound_data_type_find_field_by_name_recursive(HccCompiler* c, HccCompoundDataType* compound_data_type, HccStringId identifier_string_id);
void hcc_astgen_static_variable_usage_found(HccCompiler* c, HccDecl decl);
void hcc_astgen_insert_global_declaration(HccCompiler* c, HccStringId identifier_string_id, HccDecl decl);
void hcc_astgen_eval_cast(HccCompiler* c, HccExpr* expr, HccDataType dst_data_type);
bool hcc_stmt_has_return(HccExpr* stmt);
HccExpr* hcc_astgen_alloc_expr(HccCompiler* c, HccExprType type);
HccExpr* hcc_astgen_alloc_expr_many(HccCompiler* c, U32 amount);

const char* hcc_type_specifier_string(HccTypeSpecfier specifier);
void hcc_astgen_data_type_ensure_is_condition(HccCompiler* c, HccDataType data_type);
void hcc_astgen_compound_data_type_validate_field_names(HccCompiler* c, HccDataType outer_data_type, HccCompoundDataType* compound_data_type);
void hcc_astgen_validate_specifiers(HccCompiler* c, HccSpecifierFlags non_specifiers, HccErrorCode invalid_specifier_error_code);
void hcc_astgen_ensure_semicolon(HccCompiler* c);
void hcc_astgen_ensure_not_unsupported_basic_type(HccCompiler* c, U32 num_tokens_behind, HccErrorCode error_code, HccDataType data_type);
bool hcc_data_type_check_compatible_assignment(HccCompiler* c, HccDataType target_data_type, HccExpr** source_expr_mut);
void hcc_data_type_ensure_compatible_assignment(HccCompiler* c, U32 other_token_idx, HccDataType target_data_type, HccExpr** source_expr_mut);
bool hcc_data_type_check_compatible_arithmetic(HccCompiler* c, HccExpr** left_expr_mut, HccExpr** right_expr_mut);
void hcc_data_type_ensure_compatible_arithmetic(HccCompiler* c, U32 other_token_idx, HccExpr** left_expr_mut, HccExpr** right_expr_mut, HccToken operator_token);
void hcc_astgen_ensure_function_args_count(HccCompiler* c, HccFunction* function, U32 args_count);
HccDataType hcc_astgen_deduplicate_array_data_type(HccCompiler* c, HccDataType element_data_type, HccConstantId size_constant_id);
void _hcc_astgen_ensure_no_unused_specifiers(HccCompiler* c, char* what);
void hcc_astgen_ensure_no_unused_specifiers_data_type(HccCompiler* c);
void hcc_astgen_ensure_no_unused_specifiers_identifier(HccCompiler* c);

void hcc_astgen_variable_stack_open(HccCompiler* c);
void hcc_astgen_variable_stack_close(HccCompiler* c);
U32 hcc_astgen_variable_stack_add(HccCompiler* c, HccStringId string_id);
U32 hcc_astgen_variable_stack_find(HccCompiler* c, HccStringId string_id);

HccToken hcc_astgen_curly_initializer_start(HccCompiler* c, HccDataType data_type, HccDataType resolved_data_type, HccExpr* first_expr);
HccToken hcc_astgen_curly_initializer_open(HccCompiler* c);
HccToken hcc_astgen_curly_initializer_close(HccCompiler* c, bool is_finished);
bool hcc_astgen_curly_initializer_next_elmt(HccCompiler* c, HccDataType resolved_target_data_type);
HccToken hcc_astgen_curly_initializer_next_elmt_with_designator(HccCompiler* c);
void hcc_astgen_curly_initializer_nested_elmt_push(HccCompiler* c, HccDataType data_type, HccDataType resolved_data_type);
void hcc_astgen_curly_initializer_tunnel_in(HccCompiler* c);
void hcc_astgen_curly_initializer_tunnel_out(HccCompiler* c);
void hcc_astgen_curly_initializer_set_composite(HccCompiler* c, HccDataType data_type, HccDataType resolved_data_type);
HccExpr* hcc_astgen_curly_initializer_generate_designated_initializer(HccCompiler* c);

HccToken hcc_astgen_generate_specifiers(HccCompiler* c);
HccDataType hcc_astgen_generate_enum_data_type(HccCompiler* c);
HccDataType hcc_astgen_generate_compound_data_type(HccCompiler* c);
HccToken hcc_astgen_generate_type_specifiers(HccCompiler* c, HccLocation* location, HccTypeSpecfier* type_specifiers_mut);
bool hcc_astgen_generate_data_type(HccCompiler* c, HccDataType* type_out);
HccDataType hcc_astgen_generate_typedef(HccCompiler* c);
HccDataType hcc_astgen_generate_typedef_with_data_type(HccCompiler* c, HccDataType aliased_data_type);
void hcc_astgen_generate_implicit_cast(HccCompiler* c, HccDataType dst_data_type, HccExpr** expr_mut);
HccExpr* hcc_astgen_generate_unary_op(HccCompiler* c, HccExpr* inner_expr, HccUnaryOp unary_op, HccToken operator_token);
HccExpr* hcc_astgen_generate_unary_expr(HccCompiler* c);
void hcc_astgen_generate_binary_op(HccCompiler* c, HccExprType* binary_op_type_out, U32* precedence_out, bool* is_assignment_out);
HccExpr* hcc_astgen_generate_call_expr(HccCompiler* c, HccExpr* function_expr);
HccExpr* hcc_astgen_generate_array_subscript_expr(HccCompiler* c, HccExpr* array_expr);
HccExpr* hcc_astgen_generate_field_access_expr(HccCompiler* c, HccExpr* left_expr);
HccExpr* hcc_astgen_generate_ternary_expr(HccCompiler* c, HccExpr* cond_expr);
HccExpr* hcc_astgen_generate_expr_(HccCompiler* c, U32 min_precedence, bool no_comma_operator);
HccExpr* hcc_astgen_generate_expr(HccCompiler* c, U32 min_precedence);
HccExpr* hcc_astgen_generate_expr_no_comma_operator(HccCompiler* c, U32 min_precedence);
HccExpr* hcc_astgen_generate_cond_expr(HccCompiler* c);
HccDataType hcc_astgen_generate_variable_decl_array(HccCompiler* c, HccDataType element_data_type);
U32 hcc_astgen_generate_variable_decl(HccCompiler* c, bool is_global, HccDataType* data_type_mut, HccExpr** init_expr_out);
HccExpr* hcc_astgen_generate_variable_decl_expr(HccCompiler* c, HccDataType data_type);
HccExpr* hcc_astgen_generate_stmt(HccCompiler* c);
void hcc_astgen_generate_function(HccCompiler* c, HccDataType return_data_type, U32 data_type_token_idx);
void hcc_astgen_recurse_used_functions(HccCompiler* c, U32 function_idx);
void hcc_astgen_generate(HccCompiler* c);

void hcc_astgen_print_expr(HccCompiler* c, HccExpr* expr, U32 indent, FILE* f);
void hcc_astgen_print(HccCompiler* c, FILE* f);

// ===========================================
//
//
// IR Generation
//
//
// ===========================================

typedef U8 HccIROpCode;
enum {
	HCC_IR_OP_CODE_NO_OP,
	HCC_IR_OP_CODE_LOAD,
	HCC_IR_OP_CODE_STORE,
	HCC_IR_OP_CODE_LOAD_SHADER_STAGE_INPUT,
	HCC_IR_OP_CODE_LOAD_RASTERIZER_STATE,

	HCC_IR_OP_CODE_BINARY_OP_START,
#define HCC_IR_OP_CODE_BINARY_OP(OP) (HCC_IR_OP_CODE_BINARY_OP_START + HCC_BINARY_OP_##OP)
	HCC_IR_OP_CODE_BINARY_OP_END = HCC_IR_OP_CODE_BINARY_OP_START + HCC_BINARY_OP_COUNT,

	HCC_IR_OP_CODE_UNARY_OP_START,
#define HCC_IR_OP_CODE_UNARY_OP(OP) (HCC_IR_OP_CODE_UNARY_OP_START + HCC_UNARY_OP_##OP)
	HCC_IR_OP_CODE_UNARY_OP_END = HCC_IR_OP_CODE_UNARY_OP_START + HCC_UNARY_OP_COUNT,

	HCC_IR_OP_CODE_PHI,

	HCC_IR_OP_CODE_SWITCH,

	HCC_IR_OP_CODE_CONVERT,
	HCC_IR_OP_CODE_BITCAST,
	HCC_IR_OP_CODE_BITCAST_PTR,

	HCC_IR_OP_CODE_COMPOSITE_INIT,
	HCC_IR_OP_CODE_ACCESS_CHAIN,
	HCC_IR_OP_CODE_FUNCTION_CALL,
	HCC_IR_OP_CODE_FUNCTION_RETURN,
	HCC_IR_OP_CODE_SELECTION_MERGE,
	HCC_IR_OP_CODE_LOOP_MERGE,
	HCC_IR_OP_CODE_BRANCH,
	HCC_IR_OP_CODE_BRANCH_CONDITIONAL,
	HCC_IR_OP_CODE_UNREACHABLE,
	HCC_IR_OP_CODE_SELECT,
};

typedef struct HccIRValue HccIRValue;
struct HccIRValue {
	HccDataType data_type;
	U16 defined_instruction_idx;
	U16 last_used_instruction_idx;
};

typedef struct HccIRInstr HccIRInstr;
struct HccIRInstr {
	U16 operands_start_idx;
	U8 operands_count;
	U8 op_code;
};

typedef struct HccIRBasicBlock HccIRBasicBlock;
struct HccIRBasicBlock {
	U16 instructions_start_idx;
	U16 instructions_count;
};

//
// inherits HccDataType
typedef U32 HccIROperand;
enum {
	HCC_IR_OPERAND_VALUE = HCC_DATA_TYPE_COUNT,
	HCC_IR_OPERAND_CONSTANT,
	HCC_IR_OPERAND_CONSTANT_IMMEDIATE_U24,
	HCC_IR_OPERAND_BASIC_BLOCK,
	HCC_IR_OPERAND_LOCAL_VARIABLE,
	HCC_IR_OPERAND_GLOBAL_VARIABLE,
	HCC_IR_OPERAND_FUNCTION,
};
#define HCC_IR_OPERAND_IS_DATA_TYPE(operand) (((operand) & 0xff) < HCC_DATA_TYPE_COUNT)

#define HCC_IR_OPERAND_VALUE_INIT(value_idx) (((value_idx) << 8) | HCC_IR_OPERAND_VALUE)
#define HCC_IR_OPERAND_IS_VALUE(operand) (((operand) & 0xff) == HCC_IR_OPERAND_VALUE)
#define HCC_IR_OPERAND_VALUE_IDX(operand) HCC_DATA_TYPE_IDX(operand)

#define HCC_IR_OPERAND_CONSTANT_INIT(constant_id) (((constant_id) << 8) | HCC_IR_OPERAND_CONSTANT)
#define HCC_IR_OPERAND_IS_CONSTANT(operand) (((operand) & 0xff) == HCC_IR_OPERAND_CONSTANT)
#define HCC_IR_OPERAND_CONSTANT_ID(operand) ((HccConstantId) { .idx_plus_one = HCC_DATA_TYPE_IDX(operand) })

#define HCC_IR_OPERAND_CONSTANT_IMMEDIATE_U24_INIT(imm_u24) (((imm_u24) << 8) | HCC_IR_OPERAND_CONSTANT_IMMEDIATE_U24)
#define HCC_IR_OPERAND_IS_CONSTANT_IMMEDIATE_U24(operand) (((operand) & 0xff) == HCC_IR_OPERAND_CONSTANT_IMMEDIATE_U24)
#define HCC_IR_OPERAND_CONSTANT_IMMEDIATE_U24(operand) HCC_DATA_TYPE_IDX(operand)

#define HCC_IR_OPERAND_BASIC_BLOCK_INIT(basic_block_idx) (((basic_block_idx) << 8) | HCC_IR_OPERAND_BASIC_BLOCK)
#define HCC_IR_OPERAND_IS_BASIC_BLOCK(operand) (((operand) & 0xff) == HCC_IR_OPERAND_BASIC_BLOCK)
#define HCC_IR_OPERAND_BASIC_BLOCK_IDX(operand) HCC_DATA_TYPE_IDX(operand)

#define HCC_IR_OPERAND_LOCAL_VARIABLE_INIT(variable_idx) (((variable_idx) << 8) | HCC_IR_OPERAND_LOCAL_VARIABLE)
#define HCC_IR_OPERAND_GLOBAL_VARIABLE_INIT(variable_idx) (((variable_idx) << 8) | HCC_IR_OPERAND_GLOBAL_VARIABLE)
#define HCC_IR_OPERAND_VARIABLE_IDX(operand) HCC_DATA_TYPE_IDX(operand)
#define HCC_IR_OPERAND_IS_LOCAL_VARIABLE(operand) (((operand) & 0xff) == HCC_IR_OPERAND_LOCAL_VARIABLE)
#define HCC_IR_OPERAND_IS_GLOBAL_VARIABLE(operand) (((operand) & 0xff) == HCC_IR_OPERAND_GLOBAL_VARIABLE)

#define HCC_IR_OPERAND_FUNCTION_INIT(function_idx) (((function_idx) << 8) | HCC_IR_OPERAND_FUNCTION)
#define HCC_IR_OPERAND_FUNCTION_IDX(operand) HCC_DATA_TYPE_IDX(operand)
#define HCC_IR_OPERAND_IS_FUNCTION(operand) (((operand) & 0xff) == HCC_IR_OPERAND_FUNCTION)

typedef struct HccIRFunction HccIRFunction;
struct HccIRFunction {
	U32 basic_blocks_start_idx;
	U32 instructions_start_idx;
	U32 values_start_idx;
	U32 operands_start_idx;
	U32 call_param_data_types_start_idx;
	U16 basic_blocks_count;
	U16 instructions_count;
	U16 values_count;
	U16 operands_count;
	U16 call_param_data_types_count;
};

typedef struct HccIRBranchState HccIRBranchState;
struct HccIRBranchState {
	bool all_cases_return;
	U32 break_branch_linked_list_head;
	U32 break_branch_linked_list_tail;
	U32 continue_branch_linked_list_head;
	U32 continue_branch_linked_list_tail;
};

typedef struct HccIRGen HccIRGen;
struct HccIRGen {
	HccStack(HccIRFunction) functions;
	HccStack(HccIRBasicBlock) basic_blocks;
	HccStack(HccIRValue) values;
	HccStack(HccIRInstr) instructions;
	HccStack(HccIROperand) operands;
	HccStack(HccDataType) function_call_param_data_types;
	HccFunction* function;
	HccIRFunction* ir_function;

	HccIROperand last_operand;
	bool do_not_load_variable;
	HccDataType assign_data_type;
	HccIRBranchState branch_state;
};

typedef struct HccIRGenSetup HccIRGenSetup;
struct HccIRGenSetup {
	U32 functions_cap;
	U32 basic_blocks_cap;
	U32 values_cap;
	U32 instructions_cap;
	U32 operands_cap;
	U32 function_call_param_data_types_cap;
};

void hcc_irgen_init(HccCompiler* c, HccCompilerSetup* setup);

HccIRBasicBlock* hcc_irgen_current_basic_block(HccCompiler* c);
HccIRBasicBlock* hcc_irgen_add_basic_block(HccCompiler* c);
U16 hcc_irgen_add_value(HccCompiler* c, HccDataType data_type);
void hcc_irgen_add_instruction(HccCompiler* c, HccIROpCode op_code, HccIROperand* operands, U32 operands_count);
void hcc_irgen_remove_last_instruction(HccCompiler* c);
HccIROperand* hcc_irgen_add_operands_many(HccCompiler* c, U32 amount);
void hcc_irgen_shrink_last_operands_count(HccCompiler* c, U32 new_amount);
U16 hcc_irgen_basic_block_idx(HccCompiler* c, HccIRBasicBlock* basic_block);
HccDataType hcc_irgen_operand_data_type(HccCompiler* c, HccIRFunction* ir_function, HccIROperand ir_operand);

void hcc_irgen_generate_convert_to_bool(HccCompiler* c, HccIROperand cond_operand);
void hcc_irgen_generate_condition_expr(HccCompiler* c, HccExpr* cond_expr);
void hcc_irgen_generate_case_instructions(HccCompiler* c, HccExpr* first_stmt);
void hcc_irgen_generate_load(HccCompiler* c, HccDataType data_type, HccIROperand src_operand);
void hcc_irgen_generate_store(HccCompiler* c, HccIROperand dst_operand, HccIROperand src_operand);
void hcc_irgen_generate_bitcast(HccCompiler* c, HccDataType dst_data_type, HccIROperand src_operand, bool is_ptr);
void hcc_irgen_generate_bitcast_union_field(HccCompiler* c, HccDataType union_data_type, U32 field_idx, HccIROperand src_operand);
HccIROperand* hcc_irgen_generate_access_chain_start(HccCompiler* c, U32 count);
void hcc_irgen_generate_access_chain_end(HccCompiler* c, HccDataType data_type);
void hcc_irgen_generate_access_chain_instruction(HccCompiler* c, HccExpr* expr, U32 count);
void hcc_irgen_add_shader_param_load(HccCompiler* c, HccIROpCode op_code, HccDataType field_data_type, U32 field_idx);
void hcc_irgen_generate_instructions(HccCompiler* c, HccExpr* expr);
void hcc_irgen_generate_function(HccCompiler* c, U32 function_idx);
void hcc_irgen_generate(HccCompiler* c);

void hcc_irgen_print_operand(HccCompiler* c, HccIROperand operand, FILE* f);
void hcc_irgen_print(HccCompiler* c, FILE* f);

// ===========================================
//
//
// SPIR-V Generation
//
//
// ===========================================

typedef U16 HccSpirvOp;
enum {
	HCC_SPIRV_OP_NO_OP = 0,
	HCC_SPIRV_OP_EXTENSION = 10,
	HCC_SPIRV_OP_EXT_INST_IMPORT = 11,
	HCC_SPIRV_OP_EXT_INST = 12,
	HCC_SPIRV_OP_MEMORY_MODEL = 14,
	HCC_SPIRV_OP_ENTRY_POINT = 15,
	HCC_SPIRV_OP_EXECUTION_MODE = 16,
	HCC_SPIRV_OP_CAPABILITY = 17,
	HCC_SPIRV_OP_TYPE_VOID = 19,
	HCC_SPIRV_OP_TYPE_BOOL = 20,
	HCC_SPIRV_OP_TYPE_INT = 21,
	HCC_SPIRV_OP_TYPE_FLOAT = 22,
	HCC_SPIRV_OP_TYPE_VECTOR = 23,
	HCC_SPIRV_OP_TYPE_MATRIX = 24,
	HCC_SPIRV_OP_TYPE_ARRAY = 28,
	HCC_SPIRV_OP_TYPE_STRUCT = 30,
	HCC_SPIRV_OP_TYPE_POINTER = 32,
	HCC_SPIRV_OP_TYPE_FUNCTION = 33,

	HCC_SPIRV_OP_CONSTANT_TRUE = 41,
	HCC_SPIRV_OP_CONSTANT_FALSE = 42,
	HCC_SPIRV_OP_CONSTANT = 43,
	HCC_SPIRV_OP_CONSTANT_COMPOSITE = 44,
	HCC_SPIRV_OP_CONSTANT_NULL = 46,

	HCC_SPIRV_OP_FUNCTION = 54,
	HCC_SPIRV_OP_FUNCTION_PARAMETER = 55,
	HCC_SPIRV_OP_FUNCTION_END = 56,
	HCC_SPIRV_OP_FUNCTION_CALL = 57,
	HCC_SPIRV_OP_VARIABLE = 59,
	HCC_SPIRV_OP_LOAD = 61,
	HCC_SPIRV_OP_STORE = 62,
	HCC_SPIRV_OP_ACCESS_CHAIN = 65,
	HCC_SPIRV_OP_IN_BOUNDS_ACCESS_CHAIN = 66,
	HCC_SPIRV_OP_DECORATE = 71,
	HCC_SPIRV_OP_COMPOSITE_CONSTRUCT = 80,
	HCC_SPIRV_OP_CONVERT_F_TO_U = 109,
	HCC_SPIRV_OP_CONVERT_F_TO_S = 110,
	HCC_SPIRV_OP_CONVERT_S_TO_F = 111,
	HCC_SPIRV_OP_CONVERT_U_TO_F = 112,
	HCC_SPIRV_OP_U_CONVERT = 113,
	HCC_SPIRV_OP_S_CONVERT = 114,
	HCC_SPIRV_OP_F_CONVERT = 115,
	HCC_SPIRV_OP_BITCAST = 124,
	HCC_SPIRV_OP_S_NEGATE = 126,
	HCC_SPIRV_OP_F_NEGATE = 127,
	HCC_SPIRV_OP_I_ADD = 128,
	HCC_SPIRV_OP_F_ADD = 129,
	HCC_SPIRV_OP_I_SUB = 130,
	HCC_SPIRV_OP_F_SUB = 131,
	HCC_SPIRV_OP_I_MUL = 132,
	HCC_SPIRV_OP_F_MUL = 133,
	HCC_SPIRV_OP_U_DIV = 134,
	HCC_SPIRV_OP_S_DIV = 135,
	HCC_SPIRV_OP_F_DIV = 136,
	HCC_SPIRV_OP_U_MOD = 137,
	HCC_SPIRV_OP_S_MOD = 139,
	HCC_SPIRV_OP_F_MOD = 141,
	HCC_SPIRV_OP_ISNAN = 156,
	HCC_SPIRV_OP_ISINF = 157,
	HCC_SPIRV_OP_LOGICAL_EQUAL = 164,
	HCC_SPIRV_OP_LOGICAL_NOT_EQUAL = 165,
	HCC_SPIRV_OP_LOGICAL_OR = 166,
	HCC_SPIRV_OP_LOGICAL_AND = 167,
	HCC_SPIRV_OP_LOGICAL_NOT = 168,
	HCC_SPIRV_OP_SELECT = 169,
	HCC_SPIRV_OP_I_EQUAL = 170,
	HCC_SPIRV_OP_I_NOT_EQUAL = 171,
	HCC_SPIRV_OP_U_GREATER_THAN = 172,
	HCC_SPIRV_OP_S_GREATER_THAN = 173,
	HCC_SPIRV_OP_U_GREATER_THAN_EQUAL = 174,
	HCC_SPIRV_OP_S_GREATER_THAN_EQUAL = 175,
	HCC_SPIRV_OP_U_LESS_THAN = 176,
	HCC_SPIRV_OP_S_LESS_THAN = 177,
	HCC_SPIRV_OP_U_LESS_THAN_EQUAL = 178,
	HCC_SPIRV_OP_S_LESS_THAN_EQUAL = 179,
	HCC_SPIRV_OP_F_UNORD_EQUAL = 181,
	HCC_SPIRV_OP_F_UNORD_NOT_EQUAL = 183,
	HCC_SPIRV_OP_F_UNORD_LESS_THAN = 185,
	HCC_SPIRV_OP_F_UNORD_GREATER_THAN = 187,
	HCC_SPIRV_OP_F_UNORD_LESS_THAN_EQUAL = 189,
	HCC_SPIRV_OP_F_UNORD_GREATER_THAN_EQUAL = 191,
	HCC_SPIRV_OP_BITWISE_SHIFT_RIGHT_LOGICAL = 194,
	HCC_SPIRV_OP_BITWISE_SHIFT_RIGHT_ARITHMETIC = 195,
	HCC_SPIRV_OP_BITWISE_SHIFT_LEFT_LOGICAL = 196,
	HCC_SPIRV_OP_BITWISE_OR = 197,
	HCC_SPIRV_OP_BITWISE_XOR = 198,
	HCC_SPIRV_OP_BITWISE_AND = 199,
	HCC_SPIRV_OP_BITWISE_NOT = 200,
	HCC_SPIRV_OP_PHI = 245,
	HCC_SPIRV_OP_LOOP_MERGE = 246,
	HCC_SPIRV_OP_SELECTION_MERGE = 247,
	HCC_SPIRV_OP_LABEL = 248,
	HCC_SPIRV_OP_BRANCH = 249,
	HCC_SPIRV_OP_BRANCH_CONDITIONAL = 250,
	HCC_SPIRV_OP_SWITCH = 251,
	HCC_SPIRV_OP_RETURN = 253,
	HCC_SPIRV_OP_RETURN_VALUE = 254,
	HCC_SPIRV_OP_UNREACHABLE = 255,
};

typedef U16 HccSpirvGLSLSTD450Op;
enum {
	HCC_SPIRV_GLSL_STD_450_OP_ROUND = 1,
	HCC_SPIRV_GLSL_STD_450_OP_TRUNC = 3,
	HCC_SPIRV_GLSL_STD_450_OP_FABS = 4,
	HCC_SPIRV_GLSL_STD_450_OP_FLOOR = 8,
	HCC_SPIRV_GLSL_STD_450_OP_CEIL = 8,
	HCC_SPIRV_GLSL_STD_450_OP_SIN = 13,
	HCC_SPIRV_GLSL_STD_450_OP_COS = 14,
	HCC_SPIRV_GLSL_STD_450_OP_TAN = 15,
	HCC_SPIRV_GLSL_STD_450_OP_ASIN = 16,
	HCC_SPIRV_GLSL_STD_450_OP_ACOS = 17,
	HCC_SPIRV_GLSL_STD_450_OP_ATAN = 18,
	HCC_SPIRV_GLSL_STD_450_OP_SINH = 19,
	HCC_SPIRV_GLSL_STD_450_OP_COSH = 20,
	HCC_SPIRV_GLSL_STD_450_OP_TANH = 21,
	HCC_SPIRV_GLSL_STD_450_OP_ASINH = 22,
	HCC_SPIRV_GLSL_STD_450_OP_ACOSH = 23,
	HCC_SPIRV_GLSL_STD_450_OP_ATANH = 24,
	HCC_SPIRV_GLSL_STD_450_OP_ATAN2 = 25,
	HCC_SPIRV_GLSL_STD_450_OP_POW = 26,
	HCC_SPIRV_GLSL_STD_450_OP_EXP = 27,
	HCC_SPIRV_GLSL_STD_450_OP_LOG = 28,
	HCC_SPIRV_GLSL_STD_450_OP_EXP2 = 29,
	HCC_SPIRV_GLSL_STD_450_OP_LOG2 = 30,
	HCC_SPIRV_GLSL_STD_450_OP_SQRT = 31,
	HCC_SPIRV_GLSL_STD_450_OP_INVERSE_SQRT = 32,
	HCC_SPIRV_GLSL_STD_450_OP_FMA = 50,
};

enum {
	HCC_SPIRV_ADDRESS_MODEL_LOGICAL = 0,
	HCC_SPIRV_ADDRESS_MODEL_PHYSICAL_STORAGE_BUFFER_64 = 5348,
};

enum {
	HCC_SPIRV_MEMORY_MODEL_GLSL450 = 1,
	HCC_SPIRV_MEMORY_MODEL_VULKAN = 3,
};

enum {
	HCC_SPIRV_EXECUTION_MODE_ORIGIN_UPPER_LEFT = 7,
	HCC_SPIRV_EXECUTION_MODE_ORIGIN_LOWER_LEFT = 8,
};

enum {
	HCC_SPIRV_CAPABILITY_SHADER = 1,
	HCC_SPIRV_CAPABILITY_VULKAN_MEMORY_MODEL = 5345,
	HCC_SPIRV_CAPABILITY_PHYSICAL_STORAGE_BUFFER = 5347,
};

enum {
	HCC_SPIRV_STORAGE_CLASS_INPUT = 1,
	HCC_SPIRV_STORAGE_CLASS_UNIFORM = 2,
	HCC_SPIRV_STORAGE_CLASS_OUTPUT = 3,
	HCC_SPIRV_STORAGE_CLASS_PRIVATE = 6,
	HCC_SPIRV_STORAGE_CLASS_FUNCTION = 7,
};

enum {
	HCC_SPIRV_EXECUTION_MODEL_VERTEX                  = 0,
	HCC_SPIRV_EXECUTION_MODEL_TESSELLATION_CONTROL    = 1,
	HCC_SPIRV_EXECUTION_MODEL_TESSELLATION_EVALUATION = 2,
	HCC_SPIRV_EXECUTION_MODEL_GEOMETRY                = 3,
	HCC_SPIRV_EXECUTION_MODEL_FRAGMENT                = 4,
	HCC_SPIRV_EXECUTION_MODEL_GL_COMPUTE              = 5,
};

enum {
	HCC_SPIRV_SELECTION_CONTROL_NONE          = 0,
	HCC_SPIRV_SELECTION_CONTROL_FLATTERN      = 1,
	HCC_SPIRV_SELECTION_CONTROL_DONT_FLATTERN = 2,
};

enum {
	HCC_SPIRV_LOOP_CONTROL_NONE = 0,
};

enum {
	HCC_SPRIV_DECORATION_BUILTIN =  11,
	HCC_SPRIV_DECORATION_FLAT = 14,
	HCC_SPRIV_DECORATION_LOCATION = 30,
};

enum {
	HCC_SPIRV_BUILTIN_POSITION =               0,
	HCC_SPIRV_BUILTIN_POINT_SIZE =             1,
	HCC_SPIRV_BUILTIN_CLIP_DISTANCE =          3,
	HCC_SPIRV_BUILTIN_CULL_DISTANCE =          4,
	HCC_SPIRV_BUILTIN_VIEWPORT_INDEX =         10,
	HCC_SPIRV_BUILTIN_FRAG_COORD =             15,
	HCC_SPIRV_BUILTIN_POINT_COORD =            16,
	HCC_SPIRV_BUILTIN_LOCAL_INVOCATION_INDEX = 29,
	HCC_SPIRV_BUILTIN_VERTEX_INDEX =           42,
	HCC_SPIRV_BUILTIN_INSTANCE_INDEX =         43,
};

enum {
	HCC_SPIRV_FUNCTION_CTRL_NONE         = 0x0,
	HCC_SPIRV_FUNCTION_CTRL_INLINE       = 0x1,
	HCC_SPIRV_FUNCTION_CTRL_DONT_INLINE  = 0x2,
	HCC_SPIRV_FUNCTION_CTRL_PURE         = 0x4,
	HCC_SPIRV_FUNCTION_CTRL_CONST        = 0x8,
};

#define HCC_SPIRV_INSTR_OPERANDS_CAP 24

#define HCC_SPIRV_ID_FROM_INTRINSIC_TYPE(intrinsic_type) (intrinsic_type + 1)
#define HCC_SPIRV_ID_TO_INTRINSIC_TYPE(intrinsic_type) (intrinsic_type - 1)
#define HCC_SPIRV_ID_IS_INTRINSIC_TYPE(id) ((id) && (id) - 1 >= HCC_INTRINSIC_TYPE_BOOL && (id) - 1 < HCC_INTRINSIC_TYPE_COUNT)

typedef U8 HccSpirvTypeKind;
enum {
	HCC_SPIRV_TYPE_KIND_FUNCTION,
	HCC_SPIRV_TYPE_KIND_FUNCTION_VARIABLE,
	HCC_SPIRV_TYPE_KIND_STATIC_VARIABLE,
	HCC_SPIRV_TYPE_KIND_FUNCTION_VARIABLE_POINTER,
	HCC_SPIRV_TYPE_KIND_FUNCTION_VARIABLE_POINTER_INPUT,
	HCC_SPIRV_TYPE_KIND_FUNCTION_VARIABLE_POINTER_OUTPUT,
};

typedef struct HccSpirvTypeEntry HccSpirvTypeEntry;
struct HccSpirvTypeEntry {
	U32 data_types_start_idx;
	U32 spirv_id: 22;
	U32 data_types_count: 7;
	U32 kind: 3; // HccSpirvTypeKind
};

typedef struct HccSpirvTypeTable HccSpirvTypeTable;
struct HccSpirvTypeTable {
	HccStack(HccDataType) data_types;
	HccStack(HccSpirvTypeEntry) entries;
};

typedef struct HccSpirvGen HccSpirvGen;
struct HccSpirvGen {
	HccSpirvTypeTable type_table;

	HccStack(U32) out_capabilities;
	HccStack(U32) out_entry_points;
	HccStack(U32) out_debug_info;
	HccStack(U32) out_annotations;
	HccStack(U32) out_types_variables_constants;
	HccStack(U32) out_functions;

	U32 pointer_type_inputs_base_id;
	U32 pointer_type_outputs_base_id;
	U64 pointer_type_inputs_made_bitset[4];
	U64 pointer_type_outputs_made_bitset[4];
	U32 compound_type_base_id;
	U32 array_type_base_id;
	HccIntrinsicBasicTypeMask available_basic_types;

	U32 value_base_id;
	U32 constant_base_id;
	U32 basic_block_base_spirv_id;
	U32 local_variable_base_spirv_id;
	U32 global_variable_base_spirv_id;
	U32 function_base_spirv_id;
	U32 ext_inst_glsl_std_450_spirv_id;
	U32 next_id;
	HccSpirvOp instr_op;
	U16 instr_operands_count;
	U32 instr_operands[HCC_SPIRV_INSTR_OPERANDS_CAP];
};

typedef struct HccSpirvGenSetup HccSpirvGenSetup;
struct HccSpirvGenSetup {
	U32 out_capabilities_cap;
	U32 out_entry_points_cap;
	U32 out_debug_info_cap;
	U32 out_annotations_cap;
	U32 out_types_variables_constants_cap;
	U32 out_functions_cap;
};

extern HccSpirvOp hcc_spirv_binary_ops[HCC_BINARY_OP_COUNT][HCC_BASIC_TYPE_CLASS_COUNT];
extern HccSpirvOp hcc_spirv_unary_ops[HCC_UNARY_OP_COUNT][HCC_BASIC_TYPE_CLASS_COUNT];

U32 hcc_spirv_type_table_deduplicate_function(HccCompiler* c, HccFunction* function);
U32 hcc_spirv_type_table_deduplicate_variable(HccCompiler* c, HccDataType data_type, HccSpirvTypeKind kind);

void hcc_spirvgen_init(HccCompiler* c, HccCompilerSetup* setup);

U32 hcc_spirvgen_resolve_type_id(HccCompiler* c, HccDataType data_type);
U32 hcc_spirvgen_convert_operand(HccCompiler* c, HccIROperand ir_operand);
U32 hcc_spirvgen_pointer_type_input_id(HccCompiler* c, HccDataType data_type);
U32 hcc_spirvgen_pointer_type_output_id(HccCompiler* c, HccDataType data_type);
bool hcc_spirvgen_basic_type_is_supported(HccCompiler* c, HccIntrinsicType intrinsic_type);

void hcc_spirvgen_instr_start(HccCompiler* c, HccSpirvOp op);
void hcc_spirvgen_instr_add_operand(HccCompiler* c, U32 word);
void hcc_spirvgen_instr_add_converted_operand(HccCompiler* c, HccIROperand ir_operand);
void hcc_spirvgen_instr_add_result_operand(HccCompiler* c);
#define hcc_spirvgen_instr_add_operands_string_lit(c, string) hcc_spirvgen_instr_add_operands_string(c, string, sizeof(string) - 1)
#define hcc_spirvgen_instr_add_operands_string_c(c, string) hcc_spirvgen_instr_add_operands_string(c, string, strlen(string))
void hcc_spirvgen_instr_add_operands_string(HccCompiler* c, char* string, U32 string_size);
void hcc_spirvgen_instr_end(HccCompiler* c);

U32 hcc_spirvgen_generate_variable_type(HccCompiler* c, HccDataType data_type, HccSpirvTypeKind type_kind);
U32 hcc_spirvgen_generate_function_type(HccCompiler* c, HccFunction* function);
void hcc_spirvgen_generate_select(HccCompiler* c, U32 result_spirv_operand, HccDataType dst_type, U32 cond_value_spirv_operand, U32 a_spirv_operand, U32 b_spirv_operand);
void hcc_spirvgen_generate_convert(HccCompiler* c, HccSpirvOp spirv_convert_op, U32 result_spirv_operand, HccDataType dst_type, U32 value_spirv_operand);
void hcc_spirvgen_generate_entry_point_used_global_variable_spirv_ids(HccCompiler* c, HccFunction* function);
void hcc_spirvgen_generate_instr(HccCompiler* c, HccSpirvOp op, HccDataType return_data_type, HccIROperand* operands, U32 operands_count);
void hcc_spirvgen_generate_ext_instr(HccCompiler* c, HccSpirvGLSLSTD450Op op, HccDataType return_data_type, HccIROperand* operands, U32 operands_count);
void hcc_spirvgen_generate_intrinsic_function(HccCompiler* c, U32 function_idx, HccDataType return_data_type, HccIROperand* operands, U32 operands_count);
void hcc_spirvgen_generate_function(HccCompiler* c, U32 function_idx);
void hcc_spirvgen_generate_intrinsic_types(HccCompiler* c);
void hcc_spirvgen_generate_basic_type_constants(HccCompiler* c);
void hcc_spirvgen_generate_non_basic_type_constants(HccCompiler* c);
void hcc_spirvgen_generate_load(HccCompiler* c, U32 type_spirv_id, U32 result_id, U32 src_spirv_id);
void hcc_spirvgen_generate_store(HccCompiler* c, U32 dst_spirv_id, U32 src_spirv_id);
U32 hcc_spirvgen_generate_access_chain_single_field(HccCompiler* c, U32 base_spirv_id, HccDataType field_data_type, U32 field_idx);
void hcc_spirvgen_generate_intrinsic_input_variable(HccCompiler* c, U32 intrinsic_idx);
void hcc_spirvgen_generate(HccCompiler* c);

void hcc_spirvgen_write_word_many(FILE* f, U32* words, U32 words_count, char* path);
void hcc_spirvgen_write_word(FILE* f, U32 word, char* path);
void hcc_spirvgen_generate_binary(HccCompiler* c);

// ===========================================
//
//
// Constant Table
//
//
// ===========================================

typedef struct HccConstant HccConstant;
struct HccConstant {
	void* data;
	uint32_t size;
	HccDataType data_type;
};

typedef struct HccConstantEntry HccConstantEntry;
struct HccConstantEntry {
	uint32_t start_idx;
	uint32_t size;
	HccDataType data_type;
	HccStringId debug_string_id;
};

typedef struct HccConstantTable HccConstantTable;
struct HccConstantTable {
	void*             data;
	HccConstantEntry* entries;
	uint32_t          data_used_size;
	uint32_t          data_cap;
	uint32_t          entries_count;
	uint32_t          entries_cap;

	HccDataType       data_type;
	U32               fields_count;
	U32               fields_cap;
	HccConstantId*    data_write_ptr;
};

void hcc_constant_table_init(HccCompiler* c, uint32_t data_cap, uint32_t entries_cap);
HccConstantId hcc_constant_table_deduplicate_basic(HccCompiler* c, HccDataType data_type, HccBasic* basic);
void hcc_constant_table_deduplicate_composite_start(HccCompiler* c, HccDataType data_type);
void hcc_constant_table_deduplicate_composite_add(HccCompiler* c, HccConstantId constant_id);
HccConstantId hcc_constant_table_deduplicate_composite_end(HccCompiler* c);
HccConstantId hcc_constant_table_deduplicate_zero(HccCompiler* c, HccDataType data_type);
HccConstantId _hcc_constant_table_deduplicate_end(HccCompiler* c, HccDataType data_type, void* data, U32 data_size, U32 data_align, HccStringId debug_string_id);
HccConstant hcc_constant_table_get(HccCompiler* c, HccConstantId id);
void hcc_constant_print(HccCompiler* c, HccConstantId constant_id, FILE* f);
bool hcc_constant_as_uint(HccCompiler* c, HccConstant constant, U64* out);
bool hcc_constant_as_sint(HccCompiler* c, HccConstant constant, S64* out);
bool hcc_constant_as_sint32(HccCompiler* c, HccConstant constant, S32* out);
bool hcc_constant_as_float(HccCompiler* c, HccConstant constant, F64* out);

// ===========================================
//
//
// String Table
//
//
// ===========================================

typedef struct HccStringEntry HccStringEntry;
struct HccStringEntry {
	uint32_t start_idx;
	uint32_t size;
};

typedef struct HccStringTable HccStringTable;
struct HccStringTable {
	char*           data;
	HccStringEntry* entries;
	uint32_t        data_used_size;
	uint32_t        data_cap;
	uint32_t        entries_count;
	uint32_t        entries_cap;
};

enum {
	HCC_STRING_ID_NULL = 0,

#define HCC_STRING_ID_INTRINSIC_PARAM_NAMES_START HCC_STRING_ID_UINT8_T
	HCC_STRING_ID_UINT8_T,
	HCC_STRING_ID_UINT16_T,
	HCC_STRING_ID_UINT32_T,
	HCC_STRING_ID_UINT64_T,
	HCC_STRING_ID_UINTPTR_T,
	HCC_STRING_ID_INT8_T,
	HCC_STRING_ID_INT16_T,
	HCC_STRING_ID_INT32_T,
	HCC_STRING_ID_INT64_T,
	HCC_STRING_ID_INTPTR_T,
	HCC_STRING_ID_HCC_VERTEX_INPUT,
	HCC_STRING_ID_HCC_FRAGMENT_INPUT,
	HCC_STRING_ID_HALF,

	HCC_STRING_ID_PVEC_START,
#define HCC_STRING_ID_PVEC_END (HCC_STRING_ID_PVEC_START + HCC_VEC_COUNT)

	HCC_STRING_ID_VEC_START = HCC_STRING_ID_PVEC_END,
#define HCC_STRING_ID_VEC_END (HCC_STRING_ID_VEC_START + HCC_VEC_COUNT)

	HCC_STRING_ID_PMAT_START = HCC_STRING_ID_VEC_END,
#define HCC_STRING_ID_PMAT_END (HCC_STRING_ID_PMAT_START + HCC_MAT_COUNT)

	HCC_STRING_ID_MAT_START = HCC_STRING_ID_PMAT_END,
#define HCC_STRING_ID_MAT_END (HCC_STRING_ID_MAT_START + HCC_MAT_COUNT)

#define HCC_STRING_ID_INTRINSIC_PARAM_NAMES_END HCC_STRING_ID_KEYWORDS_START

	HCC_STRING_ID_KEYWORDS_START = HCC_STRING_ID_MAT_END,
#define HCC_STRING_ID_KEYWORDS_END (HCC_STRING_ID_KEYWORDS_START + HCC_TOKEN_KEYWORDS_COUNT)

	HCC_STRING_ID_FUNCTION_IDXS_START = HCC_STRING_ID_KEYWORDS_END,
#define HCC_STRING_ID_FUNCTION_IDXS_END (HCC_STRING_ID_FUNCTION_IDXS_START + HCC_FUNCTION_IDX_INTRINSIC_END)

	HCC_STRING_ID_PREDEFINED_MACROS_START = HCC_STRING_ID_FUNCTION_IDXS_END,
#define HCC_STRING_ID_PREDEFINED_MACROS_END (HCC_STRING_ID_PREDEFINED_MACROS_START + HCC_PP_PREDEFINED_MACRO_COUNT)

	HCC_STRING_ID_ONCE = HCC_STRING_ID_PREDEFINED_MACROS_END,
	HCC_STRING_ID_DEFINED,
	HCC_STRING_ID___VA_ARGS__,
};

extern char* hcc_string_intrinsic_param_names[HCC_STRING_ID_INTRINSIC_PARAM_NAMES_END];

void hcc_string_table_init(HccStringTable* string_table, U32 data_cap, U32 entries_cap);
#define hcc_string_table_deduplicate_lit(string_table, string_lit) hcc_string_table_deduplicate(string_table, string_lit, sizeof(string_lit) - 1)
#define hcc_string_table_deduplicate_c_string(string_table, c_string) hcc_string_table_deduplicate(string_table, c_string, strlen(c_string))
HccStringId hcc_string_table_deduplicate(HccStringTable* string_table, const char* string, U32 string_size);
HccString hcc_string_table_get(HccStringTable* string_table, HccStringId id);
HccString hcc_string_table_get_or_empty(HccStringTable* string_table, HccStringId id);

// ===========================================
//
//
// Compiler
//
//
// ===========================================

typedef U16 HccOption;
enum {
	HCC_OPTION_CONSTANT_FOLDING,
	HCC_OPTION_PP_UNDEF_EVAL,
	HCC_OPTION_PRINT_COLOR,

	HCC_OPTION_COUNT,
};

typedef struct HccOptions HccOptions;
struct HccOptions {
	U64 bitset[HCC_DIV_ROUND_UP(HCC_OPTION_COUNT, 8)];
};

typedef U8 HccCompilerStage;
enum {
	HCC_COMPILER_STAGE_TOKENGEN,
	HCC_COMPILER_STAGE_ASTGEN,
	HCC_COMPILER_STAGE_IRGEN,
	HCC_COMPILER_STAGE_SPIRVGEN,
};

typedef U32 HccCompilerFlags;
enum {
	HCC_COMPILER_FLAGS_NONE = 0x0,
	HCC_COMPILER_FLAGS_SET_LONG_JMP = 0x1,
	HCC_COMPILER_FLAGS_CHAR_IS_UNSIGNED = 0x2,
};

struct HccCompiler {
	HccCompilerFlags      flags;
	HccArch               arch;
	HccOS                 os;
	HccCompilerStage      stage;
	HccAllocTag           allocation_failure_alloc_tag;
	HccAllocTag           collection_is_full_alloc_tag;
	jmp_buf               compile_entry_jmp_loc;

	HccStringTable        string_table;
	HccConstantTable      constant_table;
	U8*                   basic_type_size_and_aligns; // [HCC_DATA_TYPE_BASIC_COUNT]
	U64*                  basic_type_int_mins; // [HCC_DATA_TYPE_BASIC_COUNT]
	U64*                  basic_type_int_maxes; // [HCC_DATA_TYPE_BASIC_COUNT]
	HccConstantId         basic_type_zero_constant_ids[HCC_DATA_TYPE_BASIC_COUNT];
	HccConstantId         basic_type_one_constant_ids[HCC_DATA_TYPE_BASIC_COUNT];
	HccOptions            options;
	HccMessageSys         message_sys;
	HccStack(char)        string_buffer;
	HccStack(HccString)   include_paths;
	HccStack(HccCodeFile) code_files;
	HccHashTable(HccStringId, HccCodeFileId) path_to_code_file_id_map;
	U32                   code_file_lines_cap;
	U32                   code_file_pp_if_spans_cap;
	HccCodeFile*          concat_buffer_code_file;

	HccPP                 pp;
	HccTokenGen           tokengen;
	HccAstGen             astgen;
	HccIRGen              irgen;
	HccSpirvGen           spirvgen;
};

typedef struct HccCompilerSetup HccCompilerSetup;
struct HccCompilerSetup {
	HccPPSetup       pp;
	HccTokenGenSetup tokengen;
	HccAstGenSetup   astgen;
	HccIRGenSetup    irgen;
	HccSpirvGenSetup spirvgen;

	U32 messages_cap;
	U32 message_strings_cap;
	U32 string_table_data_cap;
	U32 string_table_entries_cap;
	U32 string_buffer_cap;
	U32 include_paths_cap;
	U32 code_files_cap;
	U32 code_file_lines_cap;
	U32 code_file_pp_if_spans_cap;
};

extern HccCompilerSetup hcc_compiler_setup_default;

bool hcc_compiler_init(HccCompiler* c, HccCompilerSetup* setup);
bool hcc_compiler_compile(HccCompiler* c, char* file_path);
noreturn void hcc_compiler_bail(HccCompiler* c);
noreturn void hcc_compiler_bail_allocation_failure(HccCompiler* c, HccAllocTag tag);
noreturn void hcc_compiler_bail_collection_is_full(HccCompiler* c, HccAllocTag tag);

bool hcc_options_is_enabled(HccCompiler* c, HccOption opt);
void hcc_options_set_enabled(HccCompiler* c, HccOption opt);
void hcc_options_set_disabled(HccCompiler* c, HccOption opt);

