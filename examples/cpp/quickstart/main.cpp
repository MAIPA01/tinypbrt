// C++ quickstart: load a scene, walk over its shapes and print what each one is made of.
//   tinypbrt-cpp-example-quickstart <scene.pbrt>
#include <tinypbrt/tinypbrt.hpp>

#include <cstdio>
#include <string>
#include <utility>

namespace {
	// helper for std::visit with one lambda per alternative
	template<class... Fs>
	struct overloaded : Fs... {
		using Fs::operator()...;
	};
	template<class... Fs>
	overloaded(Fs...) -> overloaded<Fs...>;
} // namespace

int main(const int argc, const char* const* const argv) {
		if (argc < 2) {
			std::fprintf(stderr, "usage: %s <scene.pbrt>\n", argv[0]);
			return 1;
		}

	// result<scene>: either the loaded scene or an error code (no exceptions)
	auto loaded = tpbrt::load_scene_from_file(argv[1]);
		if (!loaded) {
			std::fprintf(stderr, "cannot load %s: %s\n", argv[1], tpbrt::to_string(loaded.get_error()).c_str());
			return 1;
		}
	// the scene owns the C memory and frees it in its destructor
	const tpbrt::scene scene = std::move(*loaded);

	std::printf("film %ux%u, %zu shapes, %zu materials, %zu lights\n", scene.film().x_resolution(), scene.film().y_resolution(),
	  static_cast<std::size_t>(scene.shapes().size()), static_cast<std::size_t>(scene.materials().size()),
	  static_cast<std::size_t>(scene.lights().lights_sources().size() + scene.lights().area_lights().size()));

		for (const tpbrt::shape shape : scene.shapes()) {
			// every alternative is a separate type, so the visitor knows exactly which shape it got
			const std::string geometry =
			  shape
				.visit(overloaded {
					[](const tpbrt::shape_sphere_params& s) { return "sphere r=" + std::to_string(s.radius()); },
					[](const tpbrt::shape_disk_params& d) { return "disk r=" + std::to_string(d.radius()); },
					[](const tpbrt::shape_triangle_mesh_params& m) {
						return "mesh with " + std::to_string(m.indices().size() / 3) + " triangles";
					},
					[&](const auto&) { return std::string(tpbrt::to_string(shape.type())); },
				})
				.value_or("unknown shape");

			// handles are resolved by the lists, a missing element is an error code, not a crash
			const auto material				= scene.materials().at(shape.material());
			const std::string material_name = !material				   ? "<none>" :
											  material->name().empty() ? std::string(tpbrt::to_string(material->type())) :
																		 std::string(material->name().to_string_view());

			std::printf("  %-28s material: %s%s\n", geometry.c_str(), material_name.c_str(),
			  scene.lights().area_lights().at(shape.area_light()) ? " (emissive)" : "");
		}
	return 0;
}
