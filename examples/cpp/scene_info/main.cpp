// C++ scene summary: camera, film, sampler, integrator, lights and textures with their parameters.
//   tinypbrt-cpp-example-scene_info <scene.pbrt>
#include <tinypbrt/tinypbrt.hpp>

#include <cstdio>
#include <type_traits>

namespace {
	void print_spectrum(const char* const label, const tpbrt::spectrum spectrum) {
		std::printf("%s ", label);
		// a generic lambda with if constexpr also works as a visitor
		spectrum.visit([](const auto& value) {
			using T = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<T, tpbrt::rgb>) { std::printf("rgb(%g, %g, %g)", value.r(), value.g(), value.b()); }
				else if constexpr (std::is_same_v<T, tpbrt::blackbody>) { std::printf("blackbody %g K", value.temperature()); }
				else if constexpr (std::is_same_v<T, tpbrt::spectrum_builtin>) {
					std::printf("builtin %s", tpbrt::to_string(value).data());
				}
				else if constexpr (std::is_same_v<T, tpbrt::float_t>) { std::printf("constant %g", value); }
				else if constexpr (std::is_same_v<T, tpbrt::wavelength_array>) { std::printf("%zu samples", value.size()); }
				else { std::printf("file %.*s", static_cast<int>(value.size()), value.data()); }
		});
		std::printf("\n");
	}
} // namespace

int main(const int argc, const char* const* const argv) {
		if (argc < 2) {
			std::fprintf(stderr, "usage: %s <scene.pbrt>\n", argv[0]);
			return 1;
		}
	const auto scene = tpbrt::load_scene_from_file(argv[1]);
		if (!scene) {
			std::fprintf(stderr, "cannot load %s: %s\n", argv[1], tpbrt::to_string(scene.get_error()).c_str());
			return 1;
		}

	std::printf("camera:     %s", tpbrt::to_string(scene->camera().type()).data());
		if (const auto perspective = scene->camera().as_perspective()) { std::printf(" (fov %g)", perspective->fov()); }
	std::printf("\nfilm:       %s %ux%u -> %.*s\n", tpbrt::to_string(scene->film().type()).data(), scene->film().x_resolution(),
	  scene->film().y_resolution(), static_cast<int>(scene->film().file_name().size()), scene->film().file_name().data());
	std::printf("sampler:    %s\n", tpbrt::to_string(scene->sampler().type()).data());
	std::printf("integrator: %s\n", tpbrt::to_string(scene->integrator().type()).data());
	std::printf("filter:     %s (radius %g x %g)\n", tpbrt::to_string(scene->filter().type()).data(), scene->filter().x_radius(),
	  scene->filter().y_radius());

		for (const tpbrt::light_source light : scene->lights().lights_sources()) {
			std::printf("light:      %s, scale %g\n", tpbrt::to_string(light.type()).data(), light.scale());
				if (const auto infinite = light.as_infinite()) { print_spectrum("            L", infinite->l()); }
				if (const auto point = light.as_point()) { print_spectrum("            I", point->i()); }
		}
		for (const tpbrt::area_light light : scene->lights().area_lights()) {
			std::printf("area light: %s", tpbrt::to_string(light.type()).data());
				if (light.power().has_value()) { std::printf(", power %g W", light.power().value()); }
			std::printf("\n");
				if (const auto diffuse = light.as_diffuse()) { print_spectrum("            L", diffuse->l()); }
		}
		for (const tpbrt::texture texture : scene->textures()) {
			std::printf("texture:    \"%.*s\" %s %s, mapping %s\n", static_cast<int>(texture.name().size()),
			  texture.name().data(), tpbrt::to_string(texture.type()).data(), tpbrt::to_string(texture.tex_class()).data(),
			  tpbrt::to_string(texture.mapping()).data());
		}
	return 0;
}
