#pragma once
#ifndef _TINYPBRT_EXAMPLE_UTILS_H_
#define _TINYPBRT_EXAMPLE_UTILS_H_

#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

#define EXAMPLE_STR_FMT	   "%.*s"
#define EXAMPLE_STR_ARG(s) (int)(s).size, ((s).data != NULL ? (s).data : "")

static const char* example_name(const char* const* names, const size_t count, const unsigned value) {
	return value < count ? names[value] : "?";
}

#define EXAMPLE_NAME(table, value) example_name((table), sizeof(table) / sizeof((table)[0]), (unsigned)(value))

static const char* const example_camera_names[]		= { "orthographic", "perspective", "realistic", "spherical" };
static const char* const example_film_names[]		= { "rgb", "gbuffer", "spectral" };
static const char* const example_sampler_names[]	= { "halton", "independent", "paddedsobol", "sobol", "stratified", "zsobol" };
static const char* const example_integrator_names[] = { "ambientocclusion", "bdpt", "lightpath", "mlt", "path", "randomwalk",
	"simplepath", "simplevolpath", "sppm", "volpath" };
static const char* const example_filter_names[]		= { "box", "gaussian", "mitchell", "sinc", "triangle" };
static const char* const example_accelerator_names[] = { "bvh", "kdtree" };
static const char* const example_shape_names[]		 = { "bilinearmesh", "curve", "cylinder", "disk", "sphere", "trianglemesh",
	"plymesh", "loopsubdiv" };
static const char* const example_material_names[]	 = { "coateddiffuse", "coatedconductor", "conductor", "dielectric", "diffuse",
	"diffusetransmission", "hair", "interface", "measured", "mix", "subsurface", "thindielectric" };
static const char* const example_texture_class_names[] = { "bilerp", "checkerboard", "constant", "directionmix", "dots", "fbm",
	"imagemap", "marble", "mix", "ptex", "scale", "windy", "wrinkled" };
static const char* const example_light_names[]		   = { "distant", "goniometric", "infinite", "point", "projection", "spot" };
static const char* const example_media_names[]		   = { "cloud", "homogeneous", "nanovdb", "rgbgrid", "uniformgrid" };

static void example_print_error(const char* const what, const tpbrt_error_t err) {
	char message[128]; // NOLINT(readability-magic-numbers)
	tpbrt_get_error_message(err, message, sizeof(message));
	fprintf(stderr, "%s: %s (error %d)\n", what, message, (int)err);
}

static void example_print_spectrum(FILE* const out, const tpbrt_spectrum_t* const s) {
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

static void example_print_texture_handle(FILE* const out, const tpbrt_textures_list_t textures,
  const tpbrt_texture_handle_t handle) {
		if (handle.value_type == TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM) {
			example_print_spectrum(out, &handle.as.spectrum);
			return;
		}

	const tpbrt_texture_t* texture = NULL;
		if (tpbrt_get_texture_by_handle(textures, handle, &texture) == TPBRT_ERROR_NONE && texture != NULL) {
			fprintf(out, "texture \"" EXAMPLE_STR_FMT "\" (%s)", EXAMPLE_STR_ARG(texture->name),
			  EXAMPLE_NAME(example_texture_class_names, texture->tex_class));
		}
		else { fprintf(out, "<none>"); }
}

static tpbrt_vec3_t example_transform_point(const tpbrt_mat4_t* const m, const tpbrt_vec3_t p) {
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

static tpbrt_mat4_t example_mat4_mul(const tpbrt_mat4_t* const a, const tpbrt_mat4_t* const b) {
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

#endif /* _TINYPBRT_EXAMPLE_UTILS_H_ */
