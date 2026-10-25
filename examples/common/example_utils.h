#pragma once
#ifndef _TINYPBRT_EXAMPLE_UTILS_H_
#define _TINYPBRT_EXAMPLE_UTILS_H_

#include <stddef.h>
#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

#define EXAMPLE_STR_FMT	   "%.*s"
#define EXAMPLE_STR_ARG(s) (int)(s).size, ((s).data != NULL ? (s).data : "")

/** Nazwa wartości enuma z tablicy `names` (albo "?" spoza zakresu). */
const char* example_name(const char* const* names, size_t count, unsigned value);

#define EXAMPLE_NAME(table, value) example_name((table), sizeof(table) / sizeof((table)[0]), (unsigned)(value))

/* Nazwy typów z pliku sceny, indeksowane wartością enuma (rozmiar tablic = *_MAX_NUM). */
extern const char* const example_camera_names[TPBRT_CAMERA_TYPE_MAX_NUM];
extern const char* const example_film_names[TPBRT_FILM_TYPE_MAX_NUM];
extern const char* const example_sampler_names[TPBRT_SAMPLER_TYPE_MAX_NUM];
extern const char* const example_integrator_names[TPBRT_INTEGRATOR_TYPE_MAX_NUM];
extern const char* const example_filter_names[TPBRT_FILTER_TYPE_MAX_NUM];
extern const char* const example_accelerator_names[TPBRT_ACCELERATOR_TYPE_MAX_NUM];
extern const char* const example_shape_names[TPBRT_SHAPE_TYPE_MAX_NUM];
extern const char* const example_material_names[TPBRT_MATERIAL_TYPE_MAX_NUM];
extern const char* const example_texture_class_names[TPBRT_TEXTURE_CLASS_MAX_NUM];
extern const char* const example_light_names[TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM];
extern const char* const example_media_names[TPBRT_MEDIA_TYPE_MAX_NUM];

/** Wypisuje kod błędu i jego opis z tpbrt_get_error_message na stderr. */
void example_print_error(const char* what, tpbrt_error_t err);

/** Wypisuje widmo (rgb, blackbody, stała, wbudowane, plik, pary długość fali/wartość). */
void example_print_spectrum(FILE* out, const tpbrt_spectrum_t* s);

/** Wypisuje parametr teksturowalny: wartość wpisaną w scenie albo nazwę tekstury. */
void example_print_texture_handle(FILE* out, tpbrt_textures_list_t textures, const tpbrt_texture_handle_t* handle);

/** Punkt przekształcony macierzą row-major (p' = M * (p, 1), z dzieleniem przez w). */
tpbrt_vec3_t example_transform_point(const tpbrt_mat4_t* m, tpbrt_vec3_t p);

/** Iloczyn macierzy a * b (najpierw b, potem a). */
tpbrt_mat4_t example_mat4_mul(const tpbrt_mat4_t* a, const tpbrt_mat4_t* b);

/** fopen_s tam, gdzie jest dostępne (Windows, Annex K), w przeciwnym razie fopen. */
FILE* example_fopen(const char* path, const char* mode);

#endif /* _TINYPBRT_EXAMPLE_UTILS_H_ */
