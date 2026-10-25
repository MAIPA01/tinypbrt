#include <tinypbrt/pch.h>

#include <tinypbrt/detail/name_index_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region NAME_INDEX

	static const tpbrt_string_t* tpbrt_name_index_name_at(const tpbrt_name_index_view_t* const view,
	  const tpbrt_size_t element_idx) {
		return (const tpbrt_string_t*)((const tpbrt_uchar_t*)view->elements + (element_idx * view->stride) + view->name_offset);
	}

	// FNV-1a (32 bit)
	static inline uint32_t tpbrt_name_index_hash(const tpbrt_string_t name) {
		// clang-format off
		static const uint32_t TPBRT_FNV1A_32_OFFSET_BASIS = 2166136261u;
		static const uint32_t TPBRT_FNV1A_32_PRIME		  = 16777619u;
		// clang-format on

		uint32_t hash = TPBRT_FNV1A_32_OFFSET_BASIS;
			for (tpbrt_size_t i = 0u; i < name.size; ++i) {
				hash ^= (uint32_t)(tpbrt_uchar_t)name.data[i];
				hash *= TPBRT_FNV1A_32_PRIME;
			}
		return hash;
	}

	static tpbrt_uint_t tpbrt_name_index_slot(const uint32_t hash, const tpbrt_uint_t capacity) {
		enum { HASH_BITS = 32 };

		return (tpbrt_uint_t)(((uint64_t)hash * (uint64_t)capacity) >> HASH_BITS);
	}

	static tpbrt_bool_t tpbrt_name_index_names_equal(const tpbrt_string_t* const a, const tpbrt_string_t b) {
		return a->size == b.size && (b.size == 0u || memcmp(a->data, b.data, b.size) == 0) ? TPBRT_TRUE : TPBRT_FALSE;
	}

	static void tpbrt_name_index_place(tpbrt_idx_t* const slots, const tpbrt_uint_t capacity, const uint32_t hash,
	  const tpbrt_idx_t element_idx) {
		tpbrt_uint_t slot = tpbrt_name_index_slot(hash, capacity);
			while (slots[slot] != TPBRT_IDX_INVALID) { slot = slot + 1u == capacity ? 0u : slot + 1u; }
		slots[slot] = element_idx;
	}

	static tpbrt_error_t tpbrt_name_index_rebuild(tpbrt_name_index_t* const index, const tpbrt_name_index_view_t* const view,
	  const tpbrt_uint_t capacity) {
		tpbrt_idx_t* const slots = malloc(sizeof(tpbrt_idx_t) * capacity);
			if (slots == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			for (tpbrt_uint_t i = 0u; i < capacity; ++i) { slots[i] = TPBRT_IDX_INVALID; }

			for (tpbrt_size_t i = 0u; i < view->count; ++i) {
				const tpbrt_string_t* const name = tpbrt_name_index_name_at(view, i);
					if (name->size == 0u) { continue; }
				tpbrt_name_index_place(slots, capacity, tpbrt_name_index_hash(*name), (tpbrt_idx_t)i);
			}

		free(index->slots);
		index->slots	= slots;
		index->capacity = capacity;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_uint_t tpbrt_name_index_min_capacity(const tpbrt_uint_t count) { return count + (count / 3u) + 1u; }

	tpbrt_error_t tpbrt_name_index_insert(tpbrt_name_index_t* const index, const tpbrt_name_index_view_t* const view,
	  const tpbrt_idx_t element_idx) {
			if (index == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_string_t* const name = tpbrt_name_index_name_at(view, element_idx);
			if (name->size == 0u) { return TPBRT_ERROR_NONE; }

			if (index->count == TPBRT_UINT_MAX) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		++index->count;

			if (index->count <= TPBRT_NAME_INDEX_LINEAR_MAX) { return TPBRT_ERROR_NONE; }

			if (index->capacity == 0u || (uint64_t)index->count * 4u > (uint64_t)index->capacity * 3u) {
				const tpbrt_uint_t capacity = index->capacity == 0u ? 4u * TPBRT_NAME_INDEX_LINEAR_MAX : index->capacity * 2u;
				return tpbrt_name_index_rebuild(index, view, capacity);
			}

		tpbrt_name_index_place(index->slots, index->capacity, tpbrt_name_index_hash(*name), element_idx);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_idx_t tpbrt_name_index_find(const tpbrt_name_index_t* const index, const tpbrt_name_index_view_t* const view,
	  const tpbrt_string_t name, const tpbrt_name_index_filter_t filter, const void* const context) {
			if (index == TPBRT_NULL || name.size == 0u) { return TPBRT_IDX_INVALID; }

			if (index->capacity == 0u) {
					for (tpbrt_size_t i = 0u; i < view->count; ++i) {
							if (tpbrt_name_index_names_equal(tpbrt_name_index_name_at(view, i), name) == TPBRT_FALSE) {
								continue;
							}
							if (filter != TPBRT_NULL &&
								filter((const tpbrt_uchar_t*)view->elements + (i * view->stride), context) == TPBRT_FALSE) {
								continue;
							}
						return (tpbrt_idx_t)i;
					}
				return TPBRT_IDX_INVALID;
			}

		tpbrt_uint_t slot = tpbrt_name_index_slot(tpbrt_name_index_hash(name), index->capacity);
			for (;;) {
				const tpbrt_idx_t element_idx = index->slots[slot];
					if (element_idx == TPBRT_IDX_INVALID) { return TPBRT_IDX_INVALID; }

					if (tpbrt_name_index_names_equal(tpbrt_name_index_name_at(view, element_idx), name) == TPBRT_TRUE &&
						(filter == TPBRT_NULL ||
						  filter((const tpbrt_uchar_t*)view->elements + (element_idx * view->stride), context) == TPBRT_TRUE)) {
						return element_idx;
					}

				slot = slot + 1u == index->capacity ? 0u : slot + 1u;
			}
	}

	tpbrt_error_t tpbrt_name_index_shrink(tpbrt_name_index_t* const index, const tpbrt_name_index_view_t* const view) {
			if (index == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (index->count <= TPBRT_NAME_INDEX_LINEAR_MAX) {
				free(index->slots);
				index->slots	= TPBRT_NULL;
				index->capacity = 0u;
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_uint_t capacity = tpbrt_name_index_min_capacity(index->count);
			if (capacity >= index->capacity) { return TPBRT_ERROR_NONE; }
		return tpbrt_name_index_rebuild(index, view, capacity);
	}

	void tpbrt_name_index_free(tpbrt_name_index_t* const index) {
			if (index == TPBRT_NULL) { return; }
		free(index->slots);
		index->slots	= TPBRT_NULL;
		index->capacity = 0u;
		index->count	= 0u;
	}

#pragma endregion

#pragma region ARRAY

	void* tpbrt_array_reserve_append(void* const data, const tpbrt_size_t count, const tpbrt_size_t element_size) {
			if (data != TPBRT_NULL && count != 0u && (count & (count - 1u)) != 0u) { return data; }

		const tpbrt_size_t new_capacity = count == 0u ? 1u : count * 2u;
			if (element_size == 0u || new_capacity < count || new_capacity > SIZE_MAX / element_size) { return TPBRT_NULL; }

		return realloc(data, new_capacity * element_size);
	}

	void* tpbrt_array_shrink_to_fit(void* const data, const tpbrt_size_t count, const tpbrt_size_t element_size) {
			if (data == TPBRT_NULL) { return TPBRT_NULL; }

			if (count == 0u) {
				free(data);
				return TPBRT_NULL;
			}

			if ((count & (count - 1u)) == 0u) { return data; }

		void* const new_data = realloc(data, count * element_size);
		return new_data != TPBRT_NULL ? new_data : data;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif
