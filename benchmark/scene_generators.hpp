#pragma once
#ifndef _TINYPBRT_BENCHMARK_SCENE_GENERATORS_HPP_
#define _TINYPBRT_BENCHMARK_SCENE_GENERATORS_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

namespace tpbrt_bench {

	class rng {
	public:
		// NOLINTBEGIN(readability-magic-numbers)

		explicit rng(const std::uint32_t seed) : _state(seed == 0u ? 0x9E37'79B9u : seed) {}

		float next01() {
			_state ^= _state << 13;
			_state ^= _state >> 17;
			_state ^= _state << 5;
			return static_cast<float>(_state & 0xFFFFFFu) / static_cast<float>(0x1000000u);
		}

		// NOLINTEND(readability-magic-numbers)

		float range(const float lo, const float hi) { return lo + ((hi - lo) * next01()); }

	private:
		std::uint32_t _state;
	};

	inline void append_float(std::string& out, const float value) {
		static constexpr uint32_t BUFFER_SIZE = 32u;

		char buffer[BUFFER_SIZE];
		const int n = std::snprintf(buffer, sizeof(buffer), "%.6g", static_cast<double>(value));
		out.append(buffer, static_cast<std::size_t>(n));
	}

	inline void append_uint(std::string& out, const std::size_t value) { out += std::to_string(value); }

	inline std::string header() {
		return "LookAt 0 5 -20  0 0 0  0 1 0\n" "Camera \"perspective\" \"float fov\" 45\n" "Film \"rgb\" \"integer " "xresolutio" "n\" 1280 " "\"integer " "yresolution\" 720 \"string " "filename\" \"out.exr\"\n" "Sampler " "\"zsobol" "\" " "\"integer" " pixelsam" "ples\" " "64\n" "In" "te" "gr" "at" "or" " " "\"" "vo" "lp" "at" "h" "\"" " " "\"" "in" "te" "ge" "r " "ma" "xd" "ep" "th" "\"" " 8" "\n" "WorldBegin\n" "LightSource \"infinite\" \"rgb L\" [ .4 .45 .5 ]\n";
	}

	inline std::string spheres(const std::size_t count) {
		rng rng(1u);
		std::string s = header();
		s.reserve(s.size() + (count * 110u)); // NOLINT(readability-magic-numbers)
		s += "Material \"diffuse\" \"rgb reflectance\" [ .5 .5 .5 ]\n";
			for (std::size_t i = 0; i < count; ++i) {
				s += "AttributeBegin\n  Translate ";
				append_float(s, rng.range(-100.0f, 100.0f));
				s += ' ';
				append_float(s, rng.range(-100.0f, 100.0f));
				s += ' ';
				append_float(s, rng.range(-100.0f, 100.0f));
				s += "\n  Shape \"sphere\" \"float radius\" ";
				append_float(s, rng.range(0.1f, 2.0f)); // NOLINT(readability-magic-numbers)
				s += "\nAttributeEnd\n";
			}
		return s;
	}

	inline std::string triangle_mesh(const std::size_t vertices) {
		rng rng(2u);
		std::size_t side = 2u;
			while (side * side < vertices) { ++side; }

		std::string s = header();
		s.reserve(s.size() + (side * side * 80u)); // NOLINT(readability-magic-numbers)
		s += "Shape \"trianglemesh\"\n  \"point3 P\" [";
			for (std::size_t y = 0; y < side; ++y) {
					for (std::size_t x = 0; x < side; ++x) {
						s += ' ';
						append_float(s, static_cast<float>(x));
						s += ' ';
						append_float(s, rng.range(-0.5f, 0.5f)); // NOLINT(readability-magic-numbers)
						s += ' ';
						append_float(s, static_cast<float>(y));
					}
			}
		s += " ]\n  \"normal N\" [";
			for (std::size_t i = 0; i < side * side; ++i) { s += " 0 1 0"; }
		s += " ]\n  \"point2 uv\" [";
			for (std::size_t y = 0; y < side; ++y) {
					for (std::size_t x = 0; x < side; ++x) {
						s += ' ';
						append_float(s, static_cast<float>(x) / static_cast<float>(side - 1u));
						s += ' ';
						append_float(s, static_cast<float>(y) / static_cast<float>(side - 1u));
					}
			}
		s += " ]\n  \"integer indices\" [";
			for (std::size_t y = 0; y + 1u < side; ++y) {
					for (std::size_t x = 0; x + 1u < side; ++x) {
						const std::size_t i0 = (y * side) + x;
						const std::size_t i1 = i0 + 1u;
						const std::size_t i2 = i0 + side;
						const std::size_t i3 = i2 + 1u;
							for (const std::size_t i : { i0, i1, i2, i1, i3, i2 }) {
								s += ' ';
								append_uint(s, i);
							}
					}
			}
		s += " ]\n";
		return s;
	}

	inline std::string named_materials(const std::size_t count) {
		rng rng(3u);
		std::string s = header();
		s.reserve(s.size() + (count * 400u)); // NOLINT(readability-magic-numbers)
			for (std::size_t i = 0; i < count; ++i) {
				const std::string id = std::to_string(i);
				s += "Texture \"checks" + id +
					 "\" \"spectrum\" \"checkerboard\" \"float uscale\" 8 \"float vscale\" 8" " \"rgb tex1\" [ ";
				append_float(s, rng.next01());
				s += " .1 .1 ] \"rgb tex2\" [ .9 .9 .9 ]\n";
				s += "Texture \"rough" + id + "\" \"float\" \"fbm\" \"integer octaves\" 4\n";
				s += "MakeNamedMaterial \"mat" + id + "\" \"string type\" \"coateddiffuse\" \"texture reflectance\" \"checks" +
					 id + "\" \"texture roughness\" \"rough" + id + "\"\n";
			}
			for (std::size_t i = 0; i < count; ++i) {
				// NOLINTBEGIN(readability-magic-numbers)
				s += "AttributeBegin\n  NamedMaterial \"mat" + std::to_string((i * 7919u) % count) + "\"\n  Translate ";
				// NOLINTEND(readability-magic-numbers)
				append_uint(s, i);
				s += " 0 0\n  Shape \"disk\"\nAttributeEnd\n";
			}
		return s;
	}

	inline std::string instances(const std::size_t count) {
		rng rng(4u);
		std::string s = header();
		s.reserve(s.size() + (count * 90u)); // NOLINT(readability-magic-numbers)
		s +=
		  "ObjectBegin \"tree\"\n" "  Shape \"cylinder\" \"float radius\" .1 \"float zmin\" 0 \"float zmax\" 2\n" "  " "Transl" "ate" " " "0 0 " "2\n  " "Shape " "\"sphere\" " "\"float " "radius\" " ".8\n" "Obje" "ctEn" "d\n";
			for (std::size_t i = 0; i < count; ++i) {
				s += "AttributeBegin\n  Translate ";
				append_float(s, rng.range(-500.0f, 500.0f)); // NOLINT(readability-magic-numbers)
				s += " 0 ";
				append_float(s, rng.range(-500.0f, 500.0f)); // NOLINT(readability-magic-numbers)
				s += "\n  Rotate ";
				append_float(s, rng.range(0.0f, 360.0f)); // NOLINT(readability-magic-numbers)
				s += " 0 0 1\n  ObjectInstance \"tree\"\nAttributeEnd\n";
			}
		return s;
	}

	inline std::string shared_transforms(const std::size_t count) {
		std::string s = header();
		s.reserve(s.size() + (count * 70u)); // NOLINT(readability-magic-numbers)
			for (std::size_t i = 0; i < count; ++i) {
				s += "AttributeBegin\n  Translate 1 2 3\n  Shape \"sphere\"\nAttributeEnd\n";
			}
		return s;
	}

	inline std::string nested_attributes(const std::size_t depth) {
		std::string s = header();
		s.reserve(s.size() + (depth * 160u)); // NOLINT(readability-magic-numbers)
			for (std::size_t i = 0; i < depth; ++i) {
				s += "AttributeBegin\n  Attribute \"shape\" \"float radius\" ";
				append_float(s, 1.0f + (static_cast<float>(i % 10u) * 0.1f)); // NOLINT(readability-magic-numbers)
				s += "\n  Rotate 1 0 1 0\n  Scale 1.001 1.001 1.001\n  Shape \"sphere\"\n";
			}
			for (std::size_t i = 0; i < depth; ++i) { s += "AttributeEnd\n"; }
		return s;
	}

	inline std::string mixed(const std::size_t scale) {
		std::string s = header();
		s +=
		  "MakeNamedMedium \"fog\" \"string type\" \"homogeneous\" \"rgb sigma_a\" [ .01 .01 .01 ] \"rgb sigma_s\" [ .1 .1 .1 " "]\n" "LightSource \"distant\" \"point3 from\" [ 0 10 0 ] \"point3 to\" [ 0 0 0 ] \"blackbody L\" 5500\n" "Texture " "\"wood\" " "\"spectru" "m\" " "\"imagema" "p\" " "\"string " "filename" "\" " "\"wood." "png\"\n" "MakeNamedMaterial \"floor\" \"string type\" \"diffuse\" \"texture reflectance\" \"wood\"\n" "MakeNamedMaterial \"gold\" \"string type\" \"conductor\" \"spectrum eta\" \"metal-Au-eta\" \"spectrum k\" \"metal-Au-k\" \"float roughness\" .05\n" "MakeNamedMaterial \"glass\" \"string type\" \"dielectric\" \"spectrum eta\" \"glass-BK7\"\n";
		s +=
		  "AttributeBegin\n  NamedMaterial \"floor\"\n  Shape \"bilinearmesh\" \"point3 P\" [ -100 0 -100 100 0 -100 -100 0 " "10" "0 " "10" "0 " "0 " "10" "0 " "]" "\n" "At" "tr" "ib" "ut" "eE" "nd" "\n";
		s += instances(scale).substr(header().size());
		s += spheres(scale).substr(header().size());
		s += triangle_mesh(scale * 10u).substr(header().size()); // NOLINT(readability-magic-numbers)
		return s;
	}

} // namespace tpbrt_bench

#endif // _TINYPBRT_BENCHMARK_SCENE_GENERATORS_HPP_
