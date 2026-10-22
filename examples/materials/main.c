#include <stdio.h>
#include <string.h>

#include <tinypbrt/tinypbrt.h>

#include <example_utils.h>

static void print_roughness(const tpbrt_textures_list_t textures, const tpbrt_material_roughness_params_t* const r) {
	printf("      uroughness: ");
	example_print_texture_handle(stdout, textures, r->u_roughness);
	printf("\n      vroughness: ");
	example_print_texture_handle(stdout, textures, r->v_roughness);
	printf("\n      remaproughness: %s\n", r->remap_roughness ? "true" : "false");
}

static void print_material(const tpbrt_scene_t* const scene, const tpbrt_material_t* const material) {
	const tpbrt_textures_list_t textures = scene->textures;

	printf("    %s", EXAMPLE_NAME(example_material_names, material->type));
		if (material->name.size > 0u) { printf(" \"" EXAMPLE_STR_FMT "\"", EXAMPLE_STR_ARG(material->name)); }
	printf("\n");

		switch (material->type) {
		case TPBRT_MATERIAL_TYPE_DIFFUSE:
			printf("      reflectance: ");
			example_print_texture_handle(stdout, textures, material->as.diffuse.reflectance);
			printf("\n");
			break;
		case TPBRT_MATERIAL_TYPE_COATED_DIFFUSE:
			printf("      reflectance: ");
			example_print_texture_handle(stdout, textures, material->as.coated_diffuse.reflectance);
			printf("\n      thickness: %g\n", material->as.coated_diffuse.thickness);
			print_roughness(textures, &material->as.coated_diffuse.roughness_params);
			break;
			case TPBRT_MATERIAL_TYPE_CONDUCTOR: {
				const tpbrt_material_conductor_params_t* const c = &material->as.conductor;
					if (c->value_type == TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K) {
						printf("      eta: ");
						example_print_texture_handle(stdout, textures, c->as.conductor.eta);
						printf("\n      k: ");
						example_print_texture_handle(stdout, textures, c->as.conductor.k);
					}
					else {
						printf("      reflectance: ");
						example_print_texture_handle(stdout, textures, c->as.reflectance);
					}
				printf("\n");
				print_roughness(textures, &c->roughness_params);
				break;
			}
		case TPBRT_MATERIAL_TYPE_DIELECTRIC:
			printf("      eta: ");
			example_print_texture_handle(stdout, textures, material->as.dielectric.eta);
			printf("\n");
			print_roughness(textures, &material->as.dielectric.roughness_params);
			break;
		case TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION:
			printf("      reflectance: ");
			example_print_texture_handle(stdout, textures, material->as.diffuse_transmission.reflectance);
			printf("\n      transmittance: ");
			example_print_texture_handle(stdout, textures, material->as.diffuse_transmission.transmittance);
			printf("\n");
			break;
		case TPBRT_MATERIAL_TYPE_MIX:
				for (int i = 0; i < 2; ++i) {
					const tpbrt_material_t* inner = NULL;
						if (tpbrt_get_material_by_handle(scene->materials, material->as.mix.materials[i], &inner) ==
							TPBRT_ERROR_NONE) {
							printf("      materials[%d]: \"" EXAMPLE_STR_FMT "\"\n", i, EXAMPLE_STR_ARG(inner->name));
						}
				}
			break;
		default: printf("      (parameters of this type are not printed by the example)\n"); break;
		}
}

int main(const int argc, char** argv) {
		if (argc < 2) {
			fprintf(stderr, "usage: %s scene.pbrt [material_name]\n", argv[0]);
			return 1;
		}

	tpbrt_scene_t scene;
	const tpbrt_error_t err = tpbrt_scene_load_from_file(argv[1], &scene);
		if (err != TPBRT_ERROR_NONE) {
			example_print_error(argv[1], err);
			return 1;
		}

		for (size_t i = 0; i < scene.shapes.count; ++i) {
			const tpbrt_shape_t* const shape = &scene.shapes.shapes[i];
			printf("shape %zu: %s\n", i, EXAMPLE_NAME(example_shape_names, shape->type));

			const tpbrt_material_t* material = NULL;
				if (tpbrt_get_material_by_handle(scene.materials, shape->material, &material) == TPBRT_ERROR_NONE) {
					print_material(&scene, material);
				}
				else { printf("    no material (pbrt-v4 uses \"diffuse\")\n"); }

			const tpbrt_area_light_t* area_light = NULL;
				if (tpbrt_get_area_light_by_handle(scene.lights.area_lights, shape->area_light, &area_light) ==
					TPBRT_ERROR_NONE) {
					printf("    emits: ");
					example_print_spectrum(stdout, &area_light->as.diffuse.l);
					printf(" scale=%g\n", area_light->scale);
				}
		}

	const char* const wanted = argc > 2 ? argv[2] : "gold";
	tpbrt_string_t name;
	name.data					  = wanted;
	name.size					  = strlen(wanted);

	const tpbrt_material_t* found = NULL;
		if (tpbrt_get_material_by_name(scene.materials, name, &found) == TPBRT_ERROR_NONE) {
			printf("\nmaterial \"%s\" found by name:\n", wanted);
			print_material(&scene, found);
		}
		else { printf("\nmaterial \"%s\" not found\n", wanted); }

	printf("\ntextures:\n");
		for (size_t i = 0; i < scene.textures.count; ++i) {
			const tpbrt_texture_t* const texture = &scene.textures.textures[i];
			printf("  \"" EXAMPLE_STR_FMT "\": %s %s\n", EXAMPLE_STR_ARG(texture->name),
			  texture->type == TPBRT_TEXTURE_TYPE_FLOAT ? "float" : "spectrum",
			  EXAMPLE_NAME(example_texture_class_names, texture->tex_class));
				if (texture->tex_class == TPBRT_TEXTURE_CLASS_CHECKER_BOARD) {
					printf("    tex1: ");
					example_print_texture_handle(stdout, scene.textures, texture->as.checkerboard.tex1);
					printf("\n    tex2: ");
					example_print_texture_handle(stdout, scene.textures, texture->as.checkerboard.tex2);
					printf("\n    uscale=%g vscale=%g\n", texture->mapping_as.uv.u_scale, texture->mapping_as.uv.v_scale);
				}
		}

	tpbrt_scene_free(&scene);
	return 0;
}
