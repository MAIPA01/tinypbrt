#include <math.h>
#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

#include <example_utils.h>

typedef struct exporter_t {
	FILE* out;
	const tpbrt_scene_t* scene;
	size_t vertex_count;
	size_t triangle_count;
	size_t object_count;
} exporter_t;

static void emit_vertex(const exporter_t* const e, const tpbrt_mat4_t* const m, const tpbrt_vec3_t p) {
	const tpbrt_vec3_t w = example_transform_point(m, p);
	fprintf(e->out, "v %g %g %g\n", w.x, w.y, w.z);
}

static void emit_triangle(exporter_t* const e, const size_t base, const size_t a, const size_t b, const size_t c) {
	fprintf(e->out, "f %zu %zu %zu\n", base + a + 1u, base + b + 1u, base + c + 1u);
	++e->triangle_count;
}

static void emit_material(const exporter_t* const e, const tpbrt_shape_t* const shape) {
	const tpbrt_material_t* material = TPBRT_NULL;
		if (tpbrt_get_material_by_handle(&e->scene->materials, shape->material, &material) != TPBRT_ERROR_NONE) { return; }
		if (material->name.size > 0u) { fprintf(e->out, "usemtl " EXAMPLE_STR_FMT "\n", EXAMPLE_STR_ARG(material->name)); }
		else { fprintf(e->out, "usemtl %s_%zu\n", EXAMPLE_NAME(example_material_names, material->type), (size_t)material->idx); }
}

static void export_sphere(exporter_t* const e, const tpbrt_mat4_t* const m, const tpbrt_shape_sphere_params_t* const s) {
	enum { RINGS = 12, SEGMENTS = 24 };

	const float pi	  = 3.14159265358979f;
	const size_t base = e->vertex_count;
		for (int r = 0; r <= RINGS; ++r) {
			const float theta = pi * (float)r / (float)RINGS;
				for (int seg = 0; seg <= SEGMENTS; ++seg) {
					const float phi = 2.0f * pi * (float)seg / (float)SEGMENTS;
					tpbrt_vec3_t p;
					p.x = s->radius * sinf(theta) * cosf(phi);
					p.y = s->radius * sinf(theta) * sinf(phi);
					p.z = s->radius * cosf(theta);
					emit_vertex(e, m, p);
				}
		}
	e->vertex_count += (size_t)(RINGS + 1) * (SEGMENTS + 1);
		for (size_t r = 0; r < RINGS; ++r) {
				for (size_t seg = 0; seg < SEGMENTS; ++seg) {
					const size_t i0 = (r * (SEGMENTS + 1)) + seg;
					const size_t i1 = i0 + SEGMENTS + 1;
					emit_triangle(e, base, i0, i1, i0 + 1u);
					emit_triangle(e, base, i0 + 1u, i1, i1 + 1u);
				}
		}
}

static void export_cylinder(exporter_t* const e, const tpbrt_mat4_t* const m, const tpbrt_shape_cylinder_params_t* const c) {
	enum { SEGMENTS = 24 };

	const float phi_max = c->phi_max * 3.14159265358979f / 180.0f;
	const size_t base	= e->vertex_count;
		for (int seg = 0; seg <= SEGMENTS; ++seg) {
			const float phi = phi_max * (float)seg / (float)SEGMENTS;
			tpbrt_vec3_t p;
			p.x = c->radius * cosf(phi);
			p.y = c->radius * sinf(phi);
			p.z = c->z_min;
			emit_vertex(e, m, p);
			p.z = c->z_max;
			emit_vertex(e, m, p);
		}
	e->vertex_count += 2u * (SEGMENTS + 1);
		for (size_t seg = 0; seg < SEGMENTS; ++seg) {
			emit_triangle(e, base, 2u * seg, (2u * seg) + 2u, (2u * seg) + 1u);
			emit_triangle(e, base, (2u * seg) + 1u, (2u * seg) + 2u, (2u * seg) + 3u);
		}
}

static void export_disk(exporter_t* const e, const tpbrt_mat4_t* const m, const tpbrt_shape_disk_params_t* const d) {
	enum { SEGMENTS = 24 };

	const float phi_max = d->phi_max * 3.14159265358979f / 180.0f;
	const size_t base	= e->vertex_count;
		for (int seg = 0; seg <= SEGMENTS; ++seg) {
			const float phi = phi_max * (float)seg / (float)SEGMENTS;
			tpbrt_vec3_t p;
			p.z = d->height;
			p.x = d->inner_radius * cosf(phi);
			p.y = d->inner_radius * sinf(phi);
			emit_vertex(e, m, p);
			p.x = d->radius * cosf(phi);
			p.y = d->radius * sinf(phi);
			emit_vertex(e, m, p);
		}
	e->vertex_count += 2u * (SEGMENTS + 1);
		for (size_t seg = 0; seg < SEGMENTS; ++seg) {
			emit_triangle(e, base, 2u * seg, (2u * seg) + 1u, (2u * seg) + 3u);
			emit_triangle(e, base, 2u * seg, (2u * seg) + 3u, (2u * seg) + 2u);
		}
}

static void export_shape(exporter_t* const e, const tpbrt_shape_t* const shape, const tpbrt_mat4_t* const object_to_world) {
	fprintf(e->out, "o %s_%zu\n", EXAMPLE_NAME(example_shape_names, shape->type), e->object_count++);
	emit_material(e, shape);

	const size_t base = e->vertex_count;
		switch (shape->type) {
			case TPBRT_SHAPE_TYPE_TRIANGLE_MESH: {
				const tpbrt_shape_triangle_mesh_params_t* const mesh = &shape->as.triangle_mesh;
					for (size_t i = 0; i < mesh->p.count; ++i) { emit_vertex(e, object_to_world, mesh->p.data[i]); }
				e->vertex_count += mesh->p.count;
					for (size_t i = 0; i + 2u < mesh->indices.count; i += 3u) {
						emit_triangle(e, base, mesh->indices.data[i], mesh->indices.data[i + 1u], mesh->indices.data[i + 2u]);
					}
				break;
			}
			case TPBRT_SHAPE_TYPE_BILINEAR_MESH: {
				const tpbrt_shape_bilinear_mesh_params_t* const mesh = &shape->as.bilinear_mesh;
					for (size_t i = 0; i < mesh->p.count; ++i) { emit_vertex(e, object_to_world, mesh->p.data[i]); }
				e->vertex_count += mesh->p.count;
					for (size_t i = 0; i + 3u < mesh->indices.count; i += 4u) {
						const tpbrt_uint_t* const q = &mesh->indices.data[i];
						emit_triangle(e, base, q[0], q[1], q[3]);
						emit_triangle(e, base, q[0], q[3], q[2]);
					}
				break;
			}
		case TPBRT_SHAPE_TYPE_SPHERE:	export_sphere(e, object_to_world, &shape->as.sphere); break;
		case TPBRT_SHAPE_TYPE_CYLINDER: export_cylinder(e, object_to_world, &shape->as.cylinder); break;
		case TPBRT_SHAPE_TYPE_DISK:		export_disk(e, object_to_world, &shape->as.disk); break;
		case TPBRT_SHAPE_TYPE_PLY_MESH:
			fprintf(e->out, "# plymesh \"" EXAMPLE_STR_FMT "\" is not loaded by tinypbrt\n",
			  EXAMPLE_STR_ARG(shape->as.ply_mesh.file_name));
			break;
		default: fprintf(e->out, "# %s is not exported\n", EXAMPLE_NAME(example_shape_names, shape->type)); break;
		}
}

int main(const int argc, char** argv) {
		if (argc < 2) {
			fprintf(stderr, "usage: %s scene.pbrt [out.obj]\n", argv[0]);
			return 1;
		}

	tpbrt_scene_t scene;
	const tpbrt_error_t err = tpbrt_scene_load_from_file(argv[1], &scene);
		if (err != TPBRT_ERROR_NONE) {
			example_print_error(argv[1], err);
			return 1;
		}

	FILE* const out = argc > 2 ? example_fopen(argv[2], "w") : stdout;
		if (out == TPBRT_NULL) {
			fprintf(stderr, "cannot open %s\n", argv[2]);
			tpbrt_scene_free(&scene);
			return 1;
		}

	exporter_t e = { .out = out, .scene = &scene, .vertex_count = 0u, .triangle_count = 0u, .object_count = 0u };
	fprintf(out, "# exported by tinypbrt-example-export_obj from %s\n", argv[1]);

		for (size_t i = 0; i < scene.shapes.count; ++i) {
			const tpbrt_shape_t* const shape	= &scene.shapes.shapes[i];
			const tpbrt_mat4_t* object_to_world = TPBRT_NULL;
				if (tpbrt_get_transform(&scene.transforms, shape->transform.start_transform, &object_to_world) ==
					TPBRT_ERROR_NONE) {
					export_shape(&e, shape, object_to_world);
				}
		}

		for (size_t i = 0; i < scene.instances.count; ++i) {
			const tpbrt_object_instance_t* const instance = &scene.instances.instances[i];
			const tpbrt_object_t* object				  = TPBRT_NULL;
			const tpbrt_mat4_t* instance_to_world		  = TPBRT_NULL;
				if (tpbrt_get_object_by_handle(&scene.objects, instance->object, &object) != TPBRT_ERROR_NONE ||
					tpbrt_get_transform(&scene.transforms, instance->transform.start_transform, &instance_to_world) !=
					  TPBRT_ERROR_NONE) {
					continue;
				}
			fprintf(out, "g instance_%zu_" EXAMPLE_STR_FMT "\n", i, EXAMPLE_STR_ARG(object->name));
				for (size_t c = 0; c < object->children.count; ++c) {
					const tpbrt_shape_t* const shape	= &object->children.shapes[c];
					const tpbrt_mat4_t* shape_to_object = TPBRT_NULL;
						if (tpbrt_get_transform(&scene.transforms, shape->transform.start_transform, &shape_to_object) !=
							TPBRT_ERROR_NONE) {
							continue;
						}
					const tpbrt_mat4_t shape_to_world = example_mat4_mul(instance_to_world, shape_to_object);
					export_shape(&e, shape, &shape_to_world);
				}
		}

	fprintf(stderr, "exported %zu vertices, %zu triangles\n", e.vertex_count, e.triangle_count);
		if (out != stdout) { fclose(out); }
	tpbrt_scene_free(&scene);
	return 0;
}
