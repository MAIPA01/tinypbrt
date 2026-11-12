// C++ materials: list every material, then look one up by name and follow its texture handles.
//   tinypbrt-cpp-example-materials <scene.pbrt> [material name]
#include <tinypbrt/tinypbrt.hpp>

#include <algorithm>
#include <cstdio>
#include <ranges>
#include <string>

namespace {
	std::string describe(const tpbrt::textures_list textures, const tpbrt::texture_handle handle) {
			// a texture handle is either a constant spectrum or an index of a texture defined with Texture
			if (const auto spectrum = handle.as_spectrum()) {
					if (const auto rgb = spectrum->as_rgb()) {
						return "rgb(" + std::to_string(rgb->r()) + ", " + std::to_string(rgb->g()) + ", " +
							   std::to_string(rgb->b()) + ")";
					}
					if (const auto builtin = spectrum->as_builtin()) {
						return "builtin \"" + std::string(tpbrt::to_string(*builtin)) + "\"";
					}
					if (const auto constant = spectrum->as_constant()) { return "constant " + std::to_string(*constant); }
				return std::string(tpbrt::to_string(spectrum->type())) + " spectrum";
			}
		const auto texture = textures.at(handle);
		return texture ? "texture \"" + std::string(texture->name().to_string_view()) + "\"" : "<missing texture>";
	}
} // namespace

int main(const int argc, const char* const* const argv) {
		if (argc < 2) {
			std::fprintf(stderr, "usage: %s <scene.pbrt> [material name]\n", argv[0]);
			return 1;
		}
	const auto scene = tpbrt::load_scene_from_file(argv[1]);
		if (!scene) {
			std::fprintf(stderr, "cannot load %s: %s\n", argv[1], tpbrt::to_string(scene.get_error()).c_str());
			return 1;
		}

	// lists are standard ranges: views and algorithms work on them directly
	auto named = scene->materials() | std::views::filter([](const tpbrt::material m) { return !m.name().empty(); });
	std::printf("%zu materials, %zu named:\n", static_cast<std::size_t>(scene->materials().size()),
	  static_cast<std::size_t>(std::ranges::distance(named)));
		for (const tpbrt::material material : named) {
			std::printf("  %-8.*s %s\n", static_cast<int>(material.name().size()), material.name().data(),
			  tpbrt::to_string(material.type()).data());
		}

	const std::string name = argc > 2 ? argv[2] : "gold";
	const auto material	   = scene->materials().at(name);
		if (!material) {
			std::printf("no material \"%s\": %s\n", name.c_str(), tpbrt::to_string(material.get_error()).c_str());
			return 0;
		}
	std::printf("\n\"%s\" is %s\n", name.c_str(), tpbrt::to_string(material->type()).data());
		if (const auto diffuse = material->as_diffuse()) {
			std::printf("  reflectance: %s\n", describe(scene->textures(), diffuse->reflectance()).c_str());
		}
		if (const auto conductor = material->as_conductor()) {
				if (const auto eta_k = conductor->as_eta_k()) {
					std::printf("  eta: %s\n  k:   %s\n", describe(scene->textures(), eta_k->eta()).c_str(),
					  describe(scene->textures(), eta_k->k()).c_str());
				}
				if (const auto reflectance = conductor->as_reflectance()) {
					std::printf("  reflectance: %s\n", describe(scene->textures(), *reflectance).c_str());
				}
			std::printf("  roughness: %s\n", describe(scene->textures(), conductor->roughness_params().roughness()).c_str());
		}

	// which shapes use it
	const auto users = std::ranges::count_if(scene->shapes(), [&](const tpbrt::shape shape) {
		const auto used = scene->materials().at(shape.material());
		return used && used->name() == name;
	});
	std::printf("  used by %zu shape(s)\n", static_cast<std::size_t>(users));
	return 0;
}
