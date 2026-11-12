# API overview {#api_overview}

| Header                        | Contents                                                                                          |
|-------------------------------|---------------------------------------------------------------------------------------------------|
| `tinypbrt/tinypbrt.h`         | everything (include this one)                                                                     |
| `tinypbrt/detail/scene.h`     | `tpbrt_scene_t`, `tpbrt_scene_load_from_file`, `tpbrt_scene_load_from_memory`, `tpbrt_scene_free` |
| `tinypbrt/detail/error.h`     | `tpbrt_error_t`, `tpbrt_get_error_message`                                                        |
| `tinypbrt/detail/material.h`  | materials, `tpbrt_get_material_by_name`, `tpbrt_get_material_by_handle`                           |
| `tinypbrt/detail/texture.h`   | textures and texture handles                                                                      |
| `tinypbrt/detail/transform.h` | the shared matrix pool, `tpbrt_get_transform`                                                     |

## C++20 wrapper

| Header                           | Contents                                                                                        |
|----------------------------------|-------------------------------------------------------------------------------------------------|
| `tinypbrt/tinypbrt.hpp`          | everything (include this one), namespace `tpbrt`                                                |
| `tinypbrt/detail/cpp/scene.hpp`  | `tpbrt::scene` (owns the C scene), `tpbrt::scene_view`, `tpbrt::load_scene_from_file`/`_memory` |
| `tinypbrt/detail/cpp/error.hpp`  | `tpbrt::error`, `tpbrt::result<T>`, `tpbrt::to_string(error)`                                   |
| `tinypbrt/detail/cpp/common.hpp` | `tpbrt::string`, `tpbrt::array` (views over C strings and arrays)                               |
| `tinypbrt/detail/cpp/fwd.hpp`    | forward declarations, scalar aliases and constants                                              |

Every C struct has a view type of the same name without the `tpbrt_` prefix and `_t` suffix (`tpbrt_material_t` →
`tpbrt::material`). Views do not own memory, they point into the scene and are valid as long as the scene lives.
Tagged unions are exposed as `std::variant` (`visit`, `to_variant`, `as<T>()`, `holds<T>()` and `as_xxx()`/`is_xxx()`),
handles and names are resolved with `at()` of the matching list, which returns `tpbrt::result<T>`.

## Examples

- @ref examples/quickstart/main.c "quickstart"
- @ref examples/scene_info/main.c "scene_info"
- @ref examples/materials/main.c "materials"
- @ref examples/cpp/quickstart/main.cpp "C++ quickstart"
- @ref examples/cpp/scene_info/main.cpp "C++ scene_info"
- @ref examples/cpp/materials/main.cpp "C++ materials"
