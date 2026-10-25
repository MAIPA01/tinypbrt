// NOLINTNEXTLINE
#define __STDC_WANT_LIB_EXT1__ 1
#include <example_utils.h>

#include <stdio.h>

const char* example_name(const char* const* names, const size_t count, const unsigned value) {
	return value < count && names[value] != NULL ? names[value] : "?";
}

const char* const example_camera_names[TPBRT_CAMERA_TYPE_MAX_NUM]	= { "orthographic", "perspective", "realistic", "spherical" };
const char* const example_film_names[TPBRT_FILM_TYPE_MAX_NUM]		= { "rgb", "gbuffer", "spectral" };
const char* const example_sampler_names[TPBRT_SAMPLER_TYPE_MAX_NUM] = { "halton", "independent", "paddedsobol", "sobol",
	"stratified", "zsobol" };
const char* const example_integrator_names[TPBRT_INTEGRATOR_TYPE_MAX_NUM] = { "ambientocclusion", "bdpt", "lightpath", "mlt",
	"path", "randomwalk", "simplepath", "simplevolpath", "sppm", "volpath" };
const char* const example_filter_names[TPBRT_FILTER_TYPE_MAX_NUM]		  = { "box", "gaussian", "mitchell", "sinc", "triangle" };
const char* const example_accelerator_names[TPBRT_ACCELERATOR_TYPE_MAX_NUM] = { "bvh", "kdtree" };
const char* const example_shape_names[TPBRT_SHAPE_TYPE_MAX_NUM]		  = { "bilinearmesh", "curve", "cylinder", "disk", "sphere",
	"trianglemesh", "plymesh", "loopsubdiv" };
const char* const example_material_names[TPBRT_MATERIAL_TYPE_MAX_NUM] = { "coateddiffuse", "coatedconductor", "conductor",
	"dielectric", "diffuse", "diffusetransmission", "hair", "interface", "measured", "mix", "subsurface", "thindielectric" };
const char* const example_texture_class_names[TPBRT_TEXTURE_CLASS_MAX_NUM] = { "bilerp", "checkerboard", "constant",
	"directionmix", "dots", "fbm", "imagemap", "marble", "mix", "ptex", "scale", "windy", "wrinkled" };
const char* const example_light_names[TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM]	   = { "distant", "goniometric", "infinite", "point",
	"projection", "spot" };
const char* const example_media_names[TPBRT_MEDIA_TYPE_MAX_NUM] = { "cloud", "homogeneous", "nanovdb", "rgbgrid", "uniformgrid" };

void example_print_error(const char* const what, const tpbrt_error_t err) {
	char message[128]; // NOLINT(readability-magic-numbers)
	tpbrt_get_error_message(err, message, sizeof(message));
	fprintf(stderr, "%s: %s (error %d)\n", what, message, (int)err);
}

void example_print_spectrum(FILE* const out, const tpbrt_spectrum_t* const s) {
		switch (s->type) {
		case TPBRT_SPECTRUM_TYPE_RGB:		fprintf(out, "rgb(%g, %g, %g)", s->as.rgb.r, s->as.rgb.g, s->as.rgb.b); break;
		case TPBRT_SPECTRUM_TYPE_BLACKBODY: fprintf(out, "blackbody(%g K)", s->as.blackbody); break;
		case TPBRT_SPECTRUM_TYPE_CONSTANT:	fprintf(out, "%g", s->as.constant); break;
		case TPBRT_SPECTRUM_TYPE_BUILTIN:	fprintf(out, "builtin #%u", (unsigned)s->as.builtin); break;
		case TPBRT_SPECTRUM_TYPE_FILE:		fprintf(out, "file \"" EXAMPLE_STR_FMT "\"", EXAMPLE_STR_ARG(s->as.file_name)); break;
		case TPBRT_SPECTRUM_TYPE_WAVELENGTHS:
			fprintf(out, "%zu (lambda, value) pairs", s->as.wavelengths.count);
				if (s->as.wavelengths.count > 0u) {
					fprintf(out, " [%g nm: %g ...]", s->as.wavelengths.data[0].wavelength, s->as.wavelengths.data[0].value);
				}
			break;
		default: fprintf(out, "<unknown spectrum>"); break;
		}
}

void example_print_texture_handle(FILE* const out, const tpbrt_textures_list_t textures,
  const tpbrt_texture_handle_t* const handle) {
		if (handle->value_type == TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM) {
			example_print_spectrum(out, &handle->as.spectrum);
			return;
		}

	const tpbrt_texture_t* texture = NULL;
		if (tpbrt_get_texture_by_handle(&textures, handle, &texture) == TPBRT_ERROR_NONE && texture != NULL) {
			fprintf(out, "texture \"" EXAMPLE_STR_FMT "\" (%s)", EXAMPLE_STR_ARG(texture->name),
			  EXAMPLE_NAME(example_texture_class_names, texture->tex_class));
		}
		else { fprintf(out, "<none>"); }
}

tpbrt_vec3_t example_transform_point(const tpbrt_mat4_t* const m, const tpbrt_vec3_t p) {
	tpbrt_vec3_t r;
	// NOLINTBEGIN(readability-magic-numbers)
	const float w = (m->m[12] * p.x) + (m->m[13] * p.y) + (m->m[14] * p.z) + m->m[15];
	r.x			  = (m->m[0] * p.x) + (m->m[1] * p.y) + (m->m[2] * p.z) + m->m[3];
	r.y			  = (m->m[4] * p.x) + (m->m[5] * p.y) + (m->m[6] * p.z) + m->m[7];
	r.z			  = (m->m[8] * p.x) + (m->m[9] * p.y) + (m->m[10] * p.z) + m->m[11];
	// NOLINTEND(readability-magic-numbers)
		if (w != 0.0f && w != 1.0f) {
			r.x /= w;
			r.y /= w;
			r.z /= w;
		}
	return r;
}

tpbrt_mat4_t example_mat4_mul(const tpbrt_mat4_t* const a, const tpbrt_mat4_t* const b) {
	tpbrt_mat4_t r;
		for (int row = 0; row < 4; ++row) {
				for (int col = 0; col < 4; ++col) {
					float sum = 0.0f;
						for (int k = 0; k < 4; ++k) { sum += a->m[(row * 4) + k] * b->m[(k * 4) + col]; }
					r.m[(row * 4) + col] = sum;
				}
		}
	return r;
}

FILE* example_fopen(const char* const path, const char* const mode) {
#if defined(_WIN32) || defined(__STDC_LIB_EXT1__)
	/* CRT Microsoftu (MSVC, clang, MinGW) oznacza fopen jako przestarzałe; fopen_s jest w Annex K C11 */
	FILE* file = NULL;
	return fopen_s(&file, path, mode) == 0 ? file : NULL;
#else
	/* glibc, musl, macOS: Annex K (fopen_s) nie jest dostępny */
	return fopen(path, mode);
#endif
}
