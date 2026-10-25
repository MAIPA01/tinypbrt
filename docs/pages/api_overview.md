# API overview {#api_overview}

<!-- Strona do uzupełnienia. Każdy plik *.md w docs/pages trafia do dokumentacji (cmake --build build --target docs). -->

| Header | Contents |
|---|---|
| `tinypbrt/tinypbrt.h` | everything (include this one) |
| `tinypbrt/detail/scene.h` | `tpbrt_scene_t`, `tpbrt_scene_load_from_file`, `tpbrt_scene_load_from_memory`, `tpbrt_scene_free` |
| `tinypbrt/detail/error.h` | `tpbrt_error_t`, `tpbrt_get_error_message` |
| `tinypbrt/detail/material.h` | materials, `tpbrt_get_material_by_name`, `tpbrt_get_material_by_handle` |
| `tinypbrt/detail/texture.h` | textures and texture handles |
| `tinypbrt/detail/transform.h` | the shared matrix pool, `tpbrt_get_transform` |

## Examples

- @ref examples/quickstart/main.c "quickstart"
- @ref examples/scene_info/main.c "scene_info"
- @ref examples/materials/main.c "materials"
