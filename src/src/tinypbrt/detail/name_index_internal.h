#pragma once
#ifndef _TINYPBRT_NAME_INDEX_INTERNAL_H_
#define _TINYPBRT_NAME_INDEX_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * Indeks nazw dla list elementów, które mają pole `tpbrt_string_t name` (materiały, tekstury, media, obiekty).
	 *
	 *  - tablica haszująca z adresowaniem otwartym (sondowanie liniowe); slot przechowuje indeks elementu listy,
	 *  - nazwa nie jest kopiowana: porównujemy z polem `name` elementu (widok przez stride/offset),
	 *  - dopóki nazwanych elementów jest <= TPBRT_NAME_INDEX_LINEAR_MAX, indeksu nie ma (capacity == 0)
	 *    i wyszukiwanie jest liniowe - małe sceny nie płacą ani bajtu,
	 *  - w trakcie wczytywania tablica rośnie x2 przy zapełnieniu 75%; po wczytaniu jest przycinana do
	 *    ~1,33 slotu na nazwę (tpbrt_name_index_shrink).
	 */
	enum { TPBRT_NAME_INDEX_LINEAR_MAX = 8 };

	/* Widok na elementy listy: name elementu i to *(tpbrt_string_t*)((char*)elements + i * stride + name_offset). */
	typedef struct tpbrt_name_index_view_t {
		const void* elements;
		tpbrt_size_t count;
		tpbrt_size_t stride;
		tpbrt_size_t name_offset;
	} tpbrt_name_index_view_t;

#define TPBRT_NAME_INDEX_VIEW(list_elements, list_count, element_type)                      \
	(tpbrt_name_index_view_t) {                                                             \
		.elements = (list_elements), .count = (list_count), .stride = sizeof(element_type), \
		.name_offset = offsetof(element_type, name)                                         \
	}

	/* Dodatkowy warunek dopasowania (np. typ tekstury); NULL = każdy element o tej nazwie. */
	typedef tpbrt_bool_t (*tpbrt_name_index_filter_t)(const void* element, const void* context);

	/* Zapisuje w indeksie element `element_idx` (już dopisany do listy, view.count obejmuje go). Puste nazwy są pomijane. */
	tpbrt_error_t tpbrt_name_index_insert(tpbrt_name_index_t* index, const tpbrt_name_index_view_t* view,
	  tpbrt_idx_t element_idx);

	/* Pierwszy (najmniejszy indeks) element o nazwie `name` spełniający `filter`; TPBRT_IDX_INVALID gdy brak. */
	tpbrt_idx_t tpbrt_name_index_find(const tpbrt_name_index_t* index, const tpbrt_name_index_view_t* view, tpbrt_string_t name,
	  tpbrt_name_index_filter_t filter, const void* context);

	/* Po wczytaniu sceny: minimalny rozmiar tablicy (albo zwolnienie przy małej liczbie nazw). */
	tpbrt_error_t tpbrt_name_index_shrink(tpbrt_name_index_t* index, const tpbrt_name_index_view_t* view);

	void tpbrt_name_index_free(tpbrt_name_index_t* index);

	/*
	 * Tablice list (materiały, kształty, ...) rosną geometrycznie bez osobnego pola capacity:
	 * pojemność to najbliższa potęga dwójki >= count, więc realokacja jest potrzebna tylko gdy count
	 * jest potęgą dwójki (albo 0). Po wczytaniu sceny tpbrt_array_shrink_to_fit zwalnia nadmiar.
	 */
	// Zwraca tablicę z miejscem na element `count` (tę samą albo realokowaną); NULL = brak pamięci (stara jest ważna).
	void* tpbrt_array_reserve_append(void* data, tpbrt_size_t count, tpbrt_size_t element_size);
	// Zwraca tablicę dokładnie na `count` elementów (NULL dla count == 0); przy nieudanym realloc - starą tablicę.
	void* tpbrt_array_shrink_to_fit(void* data, tpbrt_size_t count, tpbrt_size_t element_size);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_NAME_INDEX_INTERNAL_H_
