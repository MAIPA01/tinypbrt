#pragma once
#ifndef _TINYPBRT_SHAPE_HPP_
#define _TINYPBRT_SHAPE_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/shape.h>

#include <tinypbrt/detail/cpp/light.hpp>
#include <tinypbrt/detail/cpp/material.hpp>
#include <tinypbrt/detail/cpp/math.hpp>
#include <tinypbrt/detail/cpp/texture.hpp>

namespace tpbrt {
	/**
	 * @defgroup shape-cpp-public Shape
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @copybrief tpbrt_shape_type_t
	 * @ingroup shape-cpp-public
	 */
	enum class shape_type : char_t {
		/// @copybrief TPBRT_SHAPE_TYPE_BILINEAR_MESH
		BILINEAR_MESH = TPBRT_SHAPE_TYPE_BILINEAR_MESH,
		/// @copybrief TPBRT_SHAPE_TYPE_CURVE
		CURVE		  = TPBRT_SHAPE_TYPE_CURVE,
		/// @copybrief TPBRT_SHAPE_TYPE_CYLINDER
		CYLINDER	  = TPBRT_SHAPE_TYPE_CYLINDER,
		/// @copybrief TPBRT_SHAPE_TYPE_DISK
		DISK		  = TPBRT_SHAPE_TYPE_DISK,
		/// @copybrief TPBRT_SHAPE_TYPE_SPHERE
		SPHERE		  = TPBRT_SHAPE_TYPE_SPHERE,
		/// @copybrief TPBRT_SHAPE_TYPE_TRIANGLE_MESH
		TRIANGLE_MESH = TPBRT_SHAPE_TYPE_TRIANGLE_MESH,
		/// @copybrief TPBRT_SHAPE_TYPE_PLY_MESH
		PLY_MESH	  = TPBRT_SHAPE_TYPE_PLY_MESH,
		/// @copybrief TPBRT_SHAPE_TYPE_LOOP_SUBDIV
		LOOP_SUBDIV	  = TPBRT_SHAPE_TYPE_LOOP_SUBDIV,
		/// @copybrief TPBRT_SHAPE_TYPE_MAX_NUM
		MAX_NUM		  = TPBRT_SHAPE_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts shape_type enum value to string.
	 * @param type Shape type.
	 * @return String value of shape type.
	 * @ingroup shape-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const shape_type type) noexcept {
		using enum shape_type;
			switch (type) {
			case BILINEAR_MESH: return "bilinearmesh";
			case CURVE:			return "curve";
			case CYLINDER:		return "cylinder";
			case DISK:			return "disk";
			case SPHERE:		return "sphere";
			case TRIANGLE_MESH: return "trianglemesh";
			case PLY_MESH:		return "plymesh";
			case LOOP_SUBDIV:	return "loopsubdiv";
			case MAX_NUM:		return "maxnum";
			default:			return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_shape_curve_basis_t
	 * @ingroup shape-cpp-public
	 */
	enum class shape_curve_basis : char_t {
		/// @copybrief TPBRT_SHAPE_CURVE_BASIS_BEZIER
		BEZIER	= TPBRT_SHAPE_CURVE_BASIS_BEZIER,
		/// @copybrief TPBRT_SHAPE_CURVE_BASIS_BSPLINE
		BSPLINE = TPBRT_SHAPE_CURVE_BASIS_BSPLINE,
		/// @copybrief TPBRT_SHAPE_CURVE_BASIS_MAX_NUM
		MAX_NUM = TPBRT_SHAPE_CURVE_BASIS_MAX_NUM,
	};

	/**
	 * @brief Converts shape_curve_basis enum value to string.
	 * @param basis Shape curve basis.
	 * @return String value of shape curve basis.
	 * @ingroup shape-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const shape_curve_basis basis) noexcept {
		using enum shape_curve_basis;
			switch (basis) {
			case BEZIER:  return "bezier";
			case BSPLINE: return "bspline";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_shape_curve_degree_t
	 * @ingroup shape-cpp-public
	 */
	enum class shape_curve_degree : char_t {
		/// @copybrief TPBRT_SHAPE_CURVE_DEGREE_2
		DEGREE_2 = TPBRT_SHAPE_CURVE_DEGREE_2,
		/// @copybrief TPBRT_SHAPE_CURVE_DEGREE_3
		DEGREE_3 = TPBRT_SHAPE_CURVE_DEGREE_3,
	};

	/**
	 * @brief Converts shape_curve_degree enum value to string.
	 * @param degree Shape curve degree.
	 * @return String value of shape curve degree.
	 * @ingroup shape-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const shape_curve_degree degree) noexcept {
		using enum shape_curve_degree;
			switch (degree) {
			case DEGREE_2: return "2";
			case DEGREE_3: return "3";
			default:	   return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_shape_curve_type_t
	 * @ingroup shape-cpp-public
	 */
	enum class shape_curve_type : char_t {
		/// @copybrief TPBRT_SHAPE_CURVE_TYPE_FLAT
		FLAT	 = TPBRT_SHAPE_CURVE_TYPE_FLAT,
		/// @copybrief TPBRT_SHAPE_CURVE_TYPE_CYLINDER
		CYLINDER = TPBRT_SHAPE_CURVE_TYPE_CYLINDER,
		/// @copybrief TPBRT_SHAPE_CURVE_TYPE_RIBBON
		RIBBON	 = TPBRT_SHAPE_CURVE_TYPE_RIBBON,
		/// @copybrief TPBRT_SHAPE_CURVE_TYPE_MAX_NUM
		MAX_NUM	 = TPBRT_SHAPE_CURVE_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts shape_curve_type enum value to string.
	 * @param type Shape curve type.
	 * @return String value of shape curve type.
	 * @ingroup shape-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const shape_curve_type type) noexcept {
		using enum shape_curve_type;
			switch (type) {
			case FLAT:	   return "flat";
			case CYLINDER: return "cylinder";
			case RIBBON:   return "ribbon";
			case MAX_NUM:  return "maxnum";
			default:	   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_shape_bilinear_mesh_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_bilinear_mesh_params : wrapper<shape_bilinear_mesh_params, tpbrt_shape_bilinear_mesh_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_array, point3_array, normal_array, point2_array>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns indices value.
		[[nodiscard]] constexpr uint_array indices() const noexcept { return _native.indices; }

		/// @brief Returns p value.
		[[nodiscard]] constexpr point3_array p() const noexcept { return _native.p; }

		/// @brief Returns n value.
		[[nodiscard]] constexpr normal_array n() const noexcept { return _native.n; }

		/// @brief Returns uv value.
		[[nodiscard]] constexpr point2_array uv() const noexcept { return _native.uv; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(indices(), p(), n(), uv()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_curve_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_curve_params : wrapper<shape_curve_params, tpbrt_shape_curve_params_t> {
		/// @brief Type used to read p value.
		using p_array_type = array<tpbrt_point3_t[4], point3>;

		/// @brief Type used to read n value.
		using n_array_type = array<tpbrt_normal_t[2], normal>;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<p_array_type, n_array_type, float_t, opt_float, opt_float, uint_t, shape_curve_basis,
		  shape_curve_degree, shape_curve_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns p value.
		[[nodiscard]] constexpr p_array_type p() const noexcept { return _native.p; }

		/// @brief Returns n value.
		[[nodiscard]] constexpr n_array_type n() const noexcept { return _native.n; }

		/// @brief Returns width value.
		[[nodiscard]] constexpr float_t width() const noexcept { return _native.width; }

		/// @brief Returns width0 value.
		[[nodiscard]] constexpr opt_float width0() const noexcept { return _native.width0; }

		/// @brief Returns width1 value.
		[[nodiscard]] constexpr opt_float width1() const noexcept { return _native.width1; }

		/// @brief Returns split_depth value.
		[[nodiscard]] constexpr uint_t split_depth() const noexcept { return _native.split_depth; }

		/// @brief Returns basis value.
		[[nodiscard]] constexpr shape_curve_basis basis() const noexcept { return static_cast<shape_curve_basis>(_native.basis); }

		/// @brief Returns degree value.
		[[nodiscard]] constexpr shape_curve_degree degree() const noexcept {
			return static_cast<shape_curve_degree>(_native.degree);
		}

		/// @brief Returns type value.
		[[nodiscard]] constexpr shape_curve_type type() const noexcept { return static_cast<shape_curve_type>(_native.type); }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(p(), n(), width(), width0(), width1(), split_depth(), basis(), degree(), type());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_cylinder_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_cylinder_params : wrapper<shape_cylinder_params, tpbrt_shape_cylinder_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, float_t, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns radius value.
		[[nodiscard]] constexpr float_t radius() const noexcept { return _native.radius; }

		/// @brief Returns z_min value.
		[[nodiscard]] constexpr float_t z_min() const noexcept { return _native.z_min; }

		/// @brief Returns z_max value.
		[[nodiscard]] constexpr float_t z_max() const noexcept { return _native.z_max; }

		/// @brief Returns phi_max value.
		[[nodiscard]] constexpr float_t phi_max() const noexcept { return _native.phi_max; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(radius(), z_min(), z_max(), phi_max());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_disk_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_disk_params : wrapper<shape_disk_params, tpbrt_shape_disk_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, float_t, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns height value.
		[[nodiscard]] constexpr float_t height() const noexcept { return _native.height; }

		/// @brief Returns radius value.
		[[nodiscard]] constexpr float_t radius() const noexcept { return _native.radius; }

		/// @brief Returns inner_radius value.
		[[nodiscard]] constexpr float_t inner_radius() const noexcept { return _native.inner_radius; }

		/// @brief Returns phi_max value.
		[[nodiscard]] constexpr float_t phi_max() const noexcept { return _native.phi_max; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(height(), radius(), inner_radius(), phi_max());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_sphere_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_sphere_params : wrapper<shape_sphere_params, tpbrt_shape_sphere_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, float_t, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns radius value.
		[[nodiscard]] constexpr float_t radius() const noexcept { return _native.radius; }

		/// @brief Returns z_min value.
		[[nodiscard]] constexpr float_t z_min() const noexcept { return _native.z_min; }

		/// @brief Returns z_max value.
		[[nodiscard]] constexpr float_t z_max() const noexcept { return _native.z_max; }

		/// @brief Returns phi_max value.
		[[nodiscard]] constexpr float_t phi_max() const noexcept { return _native.phi_max; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(radius(), z_min(), z_max(), phi_max());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_triangle_mesh_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_triangle_mesh_params : wrapper<shape_triangle_mesh_params, tpbrt_shape_triangle_mesh_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_array, point3_array, normal_array, vec3_array, point2_array>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns indices value.
		[[nodiscard]] constexpr uint_array indices() const noexcept { return _native.indices; }

		/// @brief Returns p value.
		[[nodiscard]] constexpr point3_array p() const noexcept { return _native.p; }

		/// @brief Returns n value.
		[[nodiscard]] constexpr normal_array n() const noexcept { return _native.n; }

		/// @brief Returns s value.
		[[nodiscard]] constexpr vec3_array s() const noexcept { return _native.s; }

		/// @brief Returns uv value.
		[[nodiscard]] constexpr point2_array uv() const noexcept { return _native.uv; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(indices(), p(), n(), s(), uv()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_ply_mesh_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_ply_mesh_params : wrapper<shape_ply_mesh_params, tpbrt_shape_ply_mesh_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<string, texture_handle, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native.file_name; }

		/// @brief Returns displacement value.
		[[nodiscard]] constexpr texture_handle displacement() const noexcept { return _native.displacement; }

		/// @brief Returns edge_length value.
		[[nodiscard]] constexpr float_t edge_length() const noexcept { return _native.edge_length; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(file_name(), displacement(), edge_length());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_loop_subdiv_params_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape_loop_subdiv_params : wrapper<shape_loop_subdiv_params, tpbrt_shape_loop_subdiv_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_array, point_array, uint_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns indices value.
		[[nodiscard]] constexpr uint_array indices() const noexcept { return _native.indices; }

		/// @brief Returns p value.
		[[nodiscard]] constexpr point_array p() const noexcept { return _native.p; }

		/// @brief Returns levels value.
		[[nodiscard]] constexpr uint_t levels() const noexcept { return _native.levels; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(indices(), p(), levels()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_shape_t.
	 * @ingroup shape-cpp-public
	 */
	struct shape : wrapper<shape, tpbrt_shape_t>,
				   with_variant<shape, shape_bilinear_mesh_params, shape_curve_params, shape_cylinder_params, shape_disk_params,
					 shape_sphere_params, shape_triangle_mesh_params, shape_ply_mesh_params, shape_loop_subdiv_params> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<texture_handle, transform_animated, material_handle, area_light_handle, media_handle,
		  media_handle, shape_type, bool_t, variant_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns alpha value.
		[[nodiscard]] constexpr texture_handle alpha() const noexcept { return _native.alpha; }

		/// @brief Returns transform value.
		[[nodiscard]] constexpr transform_animated transform() const noexcept { return _native.transform; }

		/// @brief Returns material value.
		[[nodiscard]] constexpr material_handle material() const noexcept { return _native.material; }

		/// @brief Returns area_light value.
		[[nodiscard]] constexpr area_light_handle area_light() const noexcept { return _native.area_light; }

		/// @brief Returns interior_media_handle value.
		[[nodiscard]] constexpr media_handle interior_media_handle() const noexcept { return _native.interior_media_handle; }

		/// @brief Returns exterior_media_handle value.
		[[nodiscard]] constexpr media_handle exterior_media_handle() const noexcept { return _native.exterior_media_handle; }

		/// @brief Returns reverse_orientation value.
		[[nodiscard]] constexpr bool_t reverse_orientation() const noexcept { return _native.reverse_orientation; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr shape_type type() const noexcept { return static_cast<shape_type>(_native.type); }

		/// @brief Returns if type is bilinear_mesh.
		[[nodiscard]] constexpr bool_t is_bilinear_mesh() const noexcept { return type() == shape_type::BILINEAR_MESH; }

		/// @brief Returns if type is curve.
		[[nodiscard]] constexpr bool_t is_curve() const noexcept { return type() == shape_type::CURVE; }

		/// @brief Returns if type is cylinder.
		[[nodiscard]] constexpr bool_t is_cylinder() const noexcept { return type() == shape_type::CYLINDER; }

		/// @brief Returns if type is disk.
		[[nodiscard]] constexpr bool_t is_disk() const noexcept { return type() == shape_type::DISK; }

		/// @brief Returns if type is sphere.
		[[nodiscard]] constexpr bool_t is_sphere() const noexcept { return type() == shape_type::SPHERE; }

		/// @brief Returns if type is triangle_mesh.
		[[nodiscard]] constexpr bool_t is_triangle_mesh() const noexcept { return type() == shape_type::TRIANGLE_MESH; }

		/// @brief Returns if type is ply_mesh.
		[[nodiscard]] constexpr bool_t is_ply_mesh() const noexcept { return type() == shape_type::PLY_MESH; }

		/// @brief Returns if type is loop_subdiv.
		[[nodiscard]] constexpr bool_t is_loop_subdiv() const noexcept { return type() == shape_type::LOOP_SUBDIV; }

		/// @brief Returns shape parameters as bilinear_mesh.
		[[nodiscard]] constexpr std::optional<shape_bilinear_mesh_params> as_bilinear_mesh() const noexcept {
				if (!is_bilinear_mesh()) { return std::nullopt; }
			return _native.as.bilinear_mesh;
		}

		/// @brief Returns shape parameters as curve.
		[[nodiscard]] constexpr std::optional<shape_curve_params> as_curve() const noexcept {
				if (!is_curve()) { return std::nullopt; }
			return _native.as.curve;
		}

		/// @brief Returns shape parameters as cylinder.
		[[nodiscard]] constexpr std::optional<shape_cylinder_params> as_cylinder() const noexcept {
				if (!is_cylinder()) { return std::nullopt; }
			return _native.as.cylinder;
		}

		/// @brief Returns shape parameters as disk.
		[[nodiscard]] constexpr std::optional<shape_disk_params> as_disk() const noexcept {
				if (!is_disk()) { return std::nullopt; }
			return _native.as.disk;
		}

		/// @brief Returns shape parameters as sphere.
		[[nodiscard]] constexpr std::optional<shape_sphere_params> as_sphere() const noexcept {
				if (!is_sphere()) { return std::nullopt; }
			return _native.as.sphere;
		}

		/// @brief Returns shape parameters as triangle_mesh.
		[[nodiscard]] constexpr std::optional<shape_triangle_mesh_params> as_triangle_mesh() const noexcept {
				if (!is_triangle_mesh()) { return std::nullopt; }
			return _native.as.triangle_mesh;
		}

		/// @brief Returns shape parameters as ply_mesh.
		[[nodiscard]] constexpr std::optional<shape_ply_mesh_params> as_ply_mesh() const noexcept {
				if (!is_ply_mesh()) { return std::nullopt; }
			return _native.as.ply_mesh;
		}

		/// @brief Returns shape parameters as loop_subdiv.
		[[nodiscard]] constexpr std::optional<shape_loop_subdiv_params> as_loop_subdiv() const noexcept {
				if (!is_loop_subdiv()) { return std::nullopt; }
			return _native.as.loop_subdiv;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum shape_type;
				switch (type()) {
				case BILINEAR_MESH: return _native.as.bilinear_mesh;
				case CURVE:			return _native.as.curve;
				case CYLINDER:		return _native.as.cylinder;
				case DISK:			return _native.as.disk;
				case SPHERE:		return _native.as.sphere;
				case TRIANGLE_MESH: return _native.as.triangle_mesh;
				case PLY_MESH:		return _native.as.ply_mesh;
				case LOOP_SUBDIV:	return _native.as.loop_subdiv;
				case MAX_NUM:		[[fallthrough]];
				default:			break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(alpha(), transform(), material(), area_light(), interior_media_handle(),
			  exterior_media_handle(), type(), reverse_orientation(), to_variant());
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_SHAPE_HPP_
