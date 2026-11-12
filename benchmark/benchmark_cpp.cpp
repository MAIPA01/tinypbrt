// C++ wrapper benchmarks: every C++ benchmark has a C twin doing the same work, so the overhead of the wrapper
// (views, iterators, std::variant, std::optional, result) can be read directly from the results.
#include <benchmark/benchmark.h>

#include <tinypbrt/tinypbrt.hpp>

#include <scene_generators.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace {
	/// @brief Scene loaded once per benchmark (both APIs read the same C scene).
	struct loaded_scene {
		tpbrt::scene scene;
		std::string source;
	};

	loaded_scene load_or_skip(benchmark::State& state, std::string source) {
		auto loaded = tpbrt::load_scene_from_memory(source);
			if (!loaded) {
				state.SkipWithError(tpbrt::to_string(loaded.get_error()).c_str());
				auto empty = tpbrt::load_scene_from_memory("WorldBegin\n");
				return { std::move(*empty), std::move(source) };
			}
		return { std::move(*loaded), std::move(source) };
	}

	// region LOAD
	void bm_c_load_spheres(benchmark::State& state) {
		const std::string source = tpbrt_bench::spheres(static_cast<std::size_t>(state.range(0)));
			for (auto _ : state) {
				tpbrt_scene_t scene;
					if (tpbrt_scene_load_from_memory(source.c_str(), ".", &scene) != TPBRT_ERROR_NONE) {
						state.SkipWithError("load failed");
						return;
					}
				benchmark::DoNotOptimize(scene.shapes.count);
				tpbrt_scene_free(&scene);
			}
		state.SetItemsProcessed(state.iterations() * state.range(0));
	}

	BENCHMARK(bm_c_load_spheres)->Arg(1000)->Arg(100'000)->Unit(benchmark::kMillisecond);

	void bm_cpp_load_spheres(benchmark::State& state) {
		const std::string source = tpbrt_bench::spheres(static_cast<std::size_t>(state.range(0)));
			for (auto _ : state) {
				const auto scene = tpbrt::load_scene_from_memory(source);
					if (!scene) {
						state.SkipWithError(tpbrt::to_string(scene.get_error()).c_str());
						return;
					}
				benchmark::DoNotOptimize(scene->shapes().size());
			}
		state.SetItemsProcessed(state.iterations() * state.range(0));
	}

	BENCHMARK(bm_cpp_load_spheres)->Arg(1000)->Arg(100'000)->Unit(benchmark::kMillisecond);
	// endregion

	// region TRAVERSE_SHAPES
	void bm_c_sum_radii(benchmark::State& state) {
		const loaded_scene loaded		  = load_or_skip(state, tpbrt_bench::spheres(static_cast<std::size_t>(state.range(0))));
		const tpbrt_shapes_list_t& shapes = loaded.scene.native().shapes;
			for (auto _ : state) {
				float sum = 0.0f;
					for (tpbrt_size_t i = 0; i < shapes.count; ++i) {
							switch (shapes.shapes[i].type) {
							case TPBRT_SHAPE_TYPE_SPHERE:	sum += shapes.shapes[i].as.sphere.radius; break;
							case TPBRT_SHAPE_TYPE_DISK:		sum += shapes.shapes[i].as.disk.radius; break;
							case TPBRT_SHAPE_TYPE_CYLINDER: sum += shapes.shapes[i].as.cylinder.radius; break;
							default:						break;
							}
					}
				benchmark::DoNotOptimize(sum);
			}
		state.SetItemsProcessed(state.iterations() * state.range(0));
	}

	BENCHMARK(bm_c_sum_radii)->Arg(100'000);

	template<class... Fs>
	struct overloaded : Fs... {
		using Fs::operator()...;
	};
	template<class... Fs>
	overloaded(Fs...) -> overloaded<Fs...>;

	void bm_cpp_sum_radii_visit(benchmark::State& state) {
		const loaded_scene loaded = load_or_skip(state, tpbrt_bench::spheres(static_cast<std::size_t>(state.range(0))));
			for (auto _ : state) {
				float sum = 0.0f;
					for (const tpbrt::shape shape : loaded.scene.shapes()) {
						shape.visit(overloaded {
							[&](const tpbrt::shape_sphere_params& p) { sum += p.radius(); },
							[&](const tpbrt::shape_disk_params& p) { sum += p.radius(); },
							[&](const tpbrt::shape_cylinder_params& p) { sum += p.radius(); },
							[](const auto&) {},
						});
					}
				benchmark::DoNotOptimize(sum);
			}
		state.SetItemsProcessed(state.iterations() * state.range(0));
	}

	BENCHMARK(bm_cpp_sum_radii_visit)->Arg(100'000);

	void bm_cpp_sum_radii_as(benchmark::State& state) {
		const loaded_scene loaded = load_or_skip(state, tpbrt_bench::spheres(static_cast<std::size_t>(state.range(0))));
			for (auto _ : state) {
				float sum = 0.0f;
					for (const tpbrt::shape shape : loaded.scene.shapes()) {
							if (const auto sphere = shape.as_sphere()) { sum += sphere->radius(); }
					}
				benchmark::DoNotOptimize(sum);
			}
		state.SetItemsProcessed(state.iterations() * state.range(0));
	}

	BENCHMARK(bm_cpp_sum_radii_as)->Arg(100'000);
	// endregion

	// region MESH_VERTICES
	void bm_c_mesh_bounds(benchmark::State& state) {
		const loaded_scene loaded	  = load_or_skip(state, tpbrt_bench::triangle_mesh(static_cast<std::size_t>(state.range(0))));
		const tpbrt_point3_array_t& p = loaded.scene.native().shapes.shapes[0].as.triangle_mesh.p;
			for (auto _ : state) {
				float max_x = 0.0f;
					for (tpbrt_size_t i = 0; i < p.count; ++i) { max_x = p.data[i].x > max_x ? p.data[i].x : max_x; }
				benchmark::DoNotOptimize(max_x);
			}
		state.SetItemsProcessed(state.iterations() * state.range(0));
	}

	BENCHMARK(bm_c_mesh_bounds)->Arg(1'000'000);

	void bm_cpp_mesh_bounds(benchmark::State& state) {
		const loaded_scene loaded	= load_or_skip(state, tpbrt_bench::triangle_mesh(static_cast<std::size_t>(state.range(0))));
		const tpbrt::point3_array p = loaded.scene.shapes()[0].as_triangle_mesh()->p();
			for (auto _ : state) {
				float max_x = 0.0f;
					for (const tpbrt::point3 point : p) { max_x = point.x() > max_x ? point.x() : max_x; }
				benchmark::DoNotOptimize(max_x);
			}
		state.SetItemsProcessed(state.iterations() * state.range(0));
	}

	BENCHMARK(bm_cpp_mesh_bounds)->Arg(1'000'000);
	// endregion

	// region LOOKUP_BY_NAME
	std::vector<std::string> material_names(const std::size_t count) {
		std::vector<std::string> names;
		names.reserve(count);
			for (std::size_t i = 0; i < count; ++i) { names.push_back("mat" + std::to_string(i)); }
		return names;
	}

	void bm_c_material_by_name(benchmark::State& state) {
		const auto count							  = static_cast<std::size_t>(state.range(0));
		const loaded_scene loaded					  = load_or_skip(state, tpbrt_bench::named_materials(count));
		const auto names							  = material_names(count);
		const tpbrt_materials_list_t* const materials = &loaded.scene.native().materials;
		std::size_t i								  = 0;
			for (auto _ : state) {
				const std::string& name		= names[i++ % count];
				const tpbrt_material_t* out = nullptr;
				benchmark::DoNotOptimize(tpbrt_get_material_by_name(materials, tpbrt_string_t { name.data(), name.size() },
				  &out));
				benchmark::DoNotOptimize(out);
			}
	}

	BENCHMARK(bm_c_material_by_name)->Arg(10)->Arg(10'000);

	void bm_cpp_material_by_name(benchmark::State& state) {
		const auto count					  = static_cast<std::size_t>(state.range(0));
		const loaded_scene loaded			  = load_or_skip(state, tpbrt_bench::named_materials(count));
		const auto names					  = material_names(count);
		const tpbrt::materials_list materials = loaded.scene.materials();
		std::size_t i						  = 0;
			for (auto _ : state) {
				auto material = materials.at(names[i++ % count]);
				benchmark::DoNotOptimize(material);
			}
	}

	BENCHMARK(bm_cpp_material_by_name)->Arg(10)->Arg(10'000);
	// endregion
} // namespace

BENCHMARK_MAIN();
