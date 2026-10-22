#include <benchmark/benchmark.h>

#include <tinypbrt/tinypbrt.h>

#include <scene_generators.hpp>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {

	void fail(benchmark::State& state, const tpbrt_error_t err) {
		static constexpr uint8_t MESSAGE_SIZE = 128u;

		char message[MESSAGE_SIZE]			  = {};
		tpbrt_get_error_message(err, message, sizeof(message));
		state.SkipWithError(message);
	}

	void run_load(benchmark::State& state, const std::string& source, const std::int64_t items) {
			for (auto _ : state) {
				tpbrt_scene_t scene;
				const tpbrt_error_t err = tpbrt_scene_load_from_memory(source.c_str(), ".", &scene);
					if (err != TPBRT_ERROR_NONE) {
						fail(state, err);
						return;
					}
				benchmark::DoNotOptimize(scene.shapes.count);
				tpbrt_scene_free(&scene);
			}

		static constexpr double ONE_KB = 1024.0;

		state.SetBytesProcessed(state.iterations() * static_cast<std::int64_t>(source.size()));
		state.SetItemsProcessed(state.iterations() * items);
		state.counters["scene_MiB"] = static_cast<double>(source.size()) / (ONE_KB * ONE_KB);
	}

	void bm_load_spheres(benchmark::State& state) {
		const std::size_t n = static_cast<std::size_t>(state.range(0));
		run_load(state, tpbrt_bench::spheres(n), state.range(0));
	}

	BENCHMARK(bm_load_spheres)->RangeMultiplier(10)->Range(100, 100'000)->Unit(benchmark::kMillisecond);

	void bm_load_triangle_mesh(benchmark::State& state) {
		const std::size_t n = static_cast<std::size_t>(state.range(0));
		run_load(state, tpbrt_bench::triangle_mesh(n), state.range(0));
	}

	BENCHMARK(bm_load_triangle_mesh)->RangeMultiplier(10)->Range(1000, 1'000'000)->Unit(benchmark::kMillisecond);

	void bm_load_named_materials(benchmark::State& state) {
		const std::size_t n = static_cast<std::size_t>(state.range(0));
		run_load(state, tpbrt_bench::named_materials(n), state.range(0));
	}

	BENCHMARK(bm_load_named_materials)->RangeMultiplier(10)->Range(10, 10000)->Unit(benchmark::kMillisecond);

	void bm_load_instances(benchmark::State& state) {
		const std::size_t n = static_cast<std::size_t>(state.range(0));
		run_load(state, tpbrt_bench::instances(n), state.range(0));
	}

	BENCHMARK(bm_load_instances)->RangeMultiplier(10)->Range(100, 100'000)->Unit(benchmark::kMillisecond);

	void bm_load_shared_transforms(benchmark::State& state) {
		const std::size_t n = static_cast<std::size_t>(state.range(0));
		run_load(state, tpbrt_bench::shared_transforms(n), state.range(0));
	}

	BENCHMARK(bm_load_shared_transforms)->RangeMultiplier(10)->Range(100, 100'000)->Unit(benchmark::kMillisecond);

	void bm_load_nested_attributes(benchmark::State& state) {
		const std::size_t n = static_cast<std::size_t>(state.range(0));
		run_load(state, tpbrt_bench::nested_attributes(n), state.range(0));
	}

	BENCHMARK(bm_load_nested_attributes)->RangeMultiplier(10)->Range(10, 10000)->Unit(benchmark::kMillisecond);

	void bm_load_mixed(benchmark::State& state) {
		const std::size_t n = static_cast<std::size_t>(state.range(0));
		run_load(state, tpbrt_bench::mixed(n), state.range(0));
	}

	BENCHMARK(bm_load_mixed)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);

	void bm_load_from_file(benchmark::State& state) {
		const std::string source = tpbrt_bench::spheres(static_cast<std::size_t>(state.range(0)));
		const std::string path	 = "tinypbrt_benchmark_scene.pbrt";
		{
			std::ofstream out(path, std::ios::binary);
			out << source;
		}
			for (auto _ : state) {
				tpbrt_scene_t scene;
				const tpbrt_error_t err = tpbrt_scene_load_from_file(path.c_str(), &scene);
					if (err != TPBRT_ERROR_NONE) {
						fail(state, err);
						break;
					}
				tpbrt_scene_free(&scene);
			}
		std::remove(path.c_str());
		state.SetBytesProcessed(state.iterations() * static_cast<std::int64_t>(source.size()));
	}

	BENCHMARK(bm_load_from_file)->Arg(10000)->Unit(benchmark::kMillisecond);

	class loaded_scene : public benchmark::Fixture {
	public:
		void SetUp(benchmark::State& state) override {
			_source = tpbrt_bench::named_materials(static_cast<std::size_t>(state.range(0)));
			_err	= tpbrt_scene_load_from_memory(_source.c_str(), ".", &_scene);
		}

		void TearDown(benchmark::State&) override {
				if (_err == TPBRT_ERROR_NONE) { tpbrt_scene_free(&_scene); }
		}

	protected:
		std::string _source;
		tpbrt_scene_t _scene {};
		tpbrt_error_t _err = TPBRT_ERROR_NONE;
	};

	BENCHMARK_DEFINE_F(loaded_scene, resolve_shape_handles)(benchmark::State& state) {
			if (_err != TPBRT_ERROR_NONE) {
				fail(state, _err);
				return;
			}
			for (auto _ : state) {
					for (std::size_t i = 0; i < _scene.shapes.count; ++i) {
						const tpbrt_shape_t& shape		 = _scene.shapes.shapes[i];
						const tpbrt_material_t* material = nullptr;
						const tpbrt_mat4_t* transform	 = nullptr;
						tpbrt_get_material_by_handle(_scene.materials, shape.material, &material);
						tpbrt_get_transform(_scene.transforms, shape.transform.start_transform, &transform);
						benchmark::DoNotOptimize(material);
						benchmark::DoNotOptimize(transform);
					}
			}
		state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(_scene.shapes.count));
	}

	BENCHMARK_REGISTER_F(loaded_scene, resolve_shape_handles)->Arg(1000)->Arg(10000);

	BENCHMARK_DEFINE_F(loaded_scene, material_by_name)(benchmark::State& state) {
			if (_err != TPBRT_ERROR_NONE) {
				fail(state, _err);
				return;
			}
		std::vector<std::string> names;
			for (std::int64_t i = 0; i < state.range(0); ++i) { names.push_back("mat" + std::to_string(i)); }
		std::size_t next = 0;
			for (auto _ : state) {
				const std::string& name = names[next];
				next					= (next + 1u) % names.size();
				tpbrt_string_t key;
				key.data						 = name.c_str();
				key.size						 = name.size();
				const tpbrt_material_t* material = nullptr;
				benchmark::DoNotOptimize(tpbrt_get_material_by_name(_scene.materials, key, &material));
				benchmark::DoNotOptimize(material);
			}
		state.SetItemsProcessed(state.iterations());
	}

	BENCHMARK_REGISTER_F(loaded_scene, material_by_name)->Arg(10)->Arg(1000)->Arg(10000);

	void register_scene_file(const std::string& path) {
		benchmark::RegisterBenchmark("BM_load_file/" + path, [path](benchmark::State& state) {
			const std::ifstream in(path, std::ios::binary);
			std::stringstream ss;
			ss << in.rdbuf();
			const std::int64_t size = static_cast<std::int64_t>(ss.str().size());
				for (auto _ : state) {
					tpbrt_scene_t scene;
					const tpbrt_error_t err = tpbrt_scene_load_from_file(path.c_str(), &scene);
						if (err != TPBRT_ERROR_NONE) {
							fail(state, err);
							break;
						}
					state.counters["shapes"] = static_cast<double>(scene.shapes.count);
					tpbrt_scene_free(&scene);
				}
			state.SetBytesProcessed(state.iterations() * size);
		})->Unit(benchmark::kMillisecond);
	}

} // namespace

int main(int argc, char** argv) {
	benchmark::Initialize(&argc, argv);
		for (int i = 1; i < argc; ++i) { register_scene_file(argv[i]); }
	benchmark::RunSpecifiedBenchmarks();
	benchmark::Shutdown();
	return 0;
}
