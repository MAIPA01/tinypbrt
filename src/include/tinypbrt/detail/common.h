#pragma once
#ifndef _TINYPBRT_TYPES_H_
#define _TINYPBRT_TYPES_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_string_t {
		const tpbrt_char_t* data;
		tpbrt_size_t size;
	} tpbrt_string_t;

	typedef struct tpbrt_string_array_t {
		tpbrt_string_t* data;
		tpbrt_size_t count;
	} tpbrt_string_array_t;

	/**
	 * Indeks nazw listy (materiałów, tekstur, mediów, obiektów) - tablica haszująca z adresowaniem otwartym,
	 * przechowująca tylko indeksy elementów (4 B na slot, ~1,33 slotu na nazwany element po wczytaniu sceny).
	 * Przy niewielu nazwach (capacity == 0) wyszukiwanie jest liniowe i indeks nie zajmuje pamięci.
	 * Pole wewnętrzne: używaj funkcji tpbrt_get_*_by_name, nie czytaj go bezpośrednio.
	 */
	typedef struct tpbrt_name_index_t {
		tpbrt_idx_t* slots;
		tpbrt_uint_t capacity;
		tpbrt_uint_t count;
	} tpbrt_name_index_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TYPES_H_
