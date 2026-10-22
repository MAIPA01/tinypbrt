# tinypbrt

Tiny but powerful [pbrt-v4](https://pbrt.org/fileformat-v4) scene file loader written in C11.

tinypbrt parses `.pbrt` scene descriptions (cameras, film, samplers, integrators, shapes, materials,
textures, lights, participating media, object instancing, animated transforms, `Include`/`Import`)
into plain C structures that you can feed into your own renderer. It does not render anything and
it does not load external assets (PLY meshes, images, spectra files) - it gives you their file names.

```c
tpbrt_scene_t scene;
if (tpbrt_scene_load_from_file("scene.pbrt", &scene) == TPBRT_ERROR_NONE) {
    printf("%zu shapes\n", scene.shapes.count);
    tpbrt_scene_free(&scene);
}
```

## Contents

- [Features](#features)
- [Requirements](#requirements)
- [Adding tinypbrt to your project](#adding-tinypbrt-to-your-project)
  - [CPM.cmake (recommended)](#cpmcmake-recommended)
  - [FetchContent](#fetchcontent)
  - [add_subdirectory](#add_subdirectory)
  - [Installed package (find_package)](#installed-package-find_package)
  - [Static or shared library](#static-or-shared-library)
- [Quick start](#quick-start)
- [Using the scene](#using-the-scene)
- [Error handling](#error-handling)
- [CMake options](#cmake-options)
- [Building and testing tinypbrt](#building-and-testing-tinypbrt)
- [License](#license)

## Features

- **The whole pbrt-v4 scene language**
  - transforms: `Identity`, `Translate`, `Scale`, `Rotate`, `LookAt`, `Transform`, `ConcatTransform`,
    `CoordinateSystem`, `CoordSysTransform` (including the built-in `"camera"` system),
    `TransformTimes`, `ActiveTransform` (motion blur: every transform has a start and an end matrix)
  - scene options: `Camera`, `Film`, `Sampler`, `Integrator`, `Accelerator`, `PixelFilter`,
    `ColorSpace`, `Option`
  - world: `AttributeBegin`/`AttributeEnd`, `Attribute` defaults, `ReverseOrientation`, `Shape`,
    `Material`, `MakeNamedMaterial`/`NamedMaterial`, `Texture`, `LightSource`, `AreaLightSource`,
    `MakeNamedMedium`/`MediumInterface`, `ObjectBegin`/`ObjectEnd`/`ObjectInstance`
  - `Include` and `Import` (paths relative to the directory of the main file)
  - deprecated `TransformBegin`/`TransformEnd` are accepted
- **Parameters** of every type (`integer`, `float`, `point2/3`, `vector2/3`, `normal`, `bool`,
  `string`, `rgb`, `spectrum`, `blackbody`, `texture`) with pbrt-v4 defaults applied
- **Spectra** as RGB, blackbody, `(wavelength, value)` pairs, named built-ins (`"metal-Cu-eta"`,
  `"stdillum-D65"`, ...) or a file name
- **pbrt-v4 semantics** where they matter: the camera is created at `WorldBegin`, so `Film`
  may come after `Camera`; `uroughness`/`vroughness` default to `roughness`; the explicit parameter
  wins over `Attribute`; string escapes (`\"`, `\\`, `\n`, ...) are decoded
- **Validation**: unknown types, wrong parameter types, out-of-range mesh indices, mismatched
  `AttributeBegin`/`ObjectBegin` nesting, include cycles - every problem is reported as an error code
- **Compact output**: identical matrices are stored once and referenced by handles

| Category | Supported types |
|---|---|
| Cameras | `perspective`, `orthographic`, `realistic`, `spherical` |
| Films | `rgb`, `gbuffer`, `spectral` |
| Samplers | `zsobol`, `halton`, `sobol`, `paddedsobol`, `independent`, `stratified` |
| Integrators | `volpath`, `path`, `bdpt`, `mlt`, `sppm`, `lightpath`, `randomwalk`, `simplepath`, `simplevolpath`, `ambientocclusion` |
| Filters | `gaussian`, `box`, `mitchell`, `sinc`, `triangle` |
| Accelerators | `bvh`, `kdtree` |
| Shapes | `sphere`, `cylinder`, `disk`, `trianglemesh`, `bilinearmesh`, `curve`, `plymesh`, `loopsubdiv` |
| Materials | `diffuse`, `coateddiffuse`, `conductor`, `coatedconductor`, `dielectric`, `thindielectric`, `diffusetransmission`, `hair`, `measured`, `mix`, `subsurface`, `interface` |
| Textures | `constant`, `checkerboard`, `imagemap`, `bilerp`, `dots`, `fbm`, `wrinkled`, `windy`, `marble`, `mix`, `directionmix`, `scale`, `ptex` |
| Lights | `point`, `spot`, `distant`, `infinite`, `goniometric`, `projection`, area `diffuse` |
| Media | `homogeneous`, `uniformgrid`, `rgbgrid`, `cloud`, `nanovdb` |

## Requirements

- a C11 compiler (tested with GCC 13 and Clang 18; the Windows presets use Clang)
- CMake 3.30 or newer
- no runtime dependencies (only the C standard library; `libm` is linked automatically where needed)

## Adding tinypbrt to your project

The CMake target is **`tinypbrt::tinypbrt`**. Tests, benchmarks, documentation and examples are
built only when tinypbrt is the top-level project, so adding it as a dependency builds just the library.

### CPM.cmake (recommended)

[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) downloads and caches the dependency for you.

```cmake
cmake_minimum_required(VERSION 3.30)
project(my_renderer LANGUAGES C)

# https://github.com/cpm-cmake/CPM.cmake#adding-cpm
include(cmake/CPM.cmake)

CPMAddPackage("gh:MAIPA01/tinypbrt@v1.0.0")

add_executable(my_renderer main.c)
target_link_libraries(my_renderer PRIVATE tinypbrt::tinypbrt)
```

`gh:MAIPA01/tinypbrt@v1.0.0` fetches the git tag `v1.0.0`. To follow a branch or pin a commit, or to
pass options, use the long form:

```cmake
CPMAddPackage(
    NAME tinypbrt
    GITHUB_REPOSITORY MAIPA01/tinypbrt
    GIT_TAG main            # branch, tag or commit hash
    OPTIONS
        "TINYPBRT_BUILD_SHARED ON"   # optional: build tinypbrt as a .dll/.so (default: static)
)
```

If your project does not use CPM yet, the smallest setup is to download `CPM.cmake` at configure time:

```cmake
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
if (NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})
```

Set the `CPM_SOURCE_CACHE` environment variable (for example to `~/.cache/CPM`) to share downloaded
packages between projects.

### FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(tinypbrt
    GIT_REPOSITORY https://github.com/MAIPA01/tinypbrt.git
    GIT_TAG        v1.0.0
)
FetchContent_MakeAvailable(tinypbrt)

target_link_libraries(my_renderer PRIVATE tinypbrt::tinypbrt)
```

### add_subdirectory

```cmake
add_subdirectory(external/tinypbrt)
target_link_libraries(my_renderer PRIVATE tinypbrt::tinypbrt)
```

### Installed package (find_package)

```sh
cmake -S tinypbrt -B build -DCMAKE_BUILD_TYPE=Release -DTINYPBRT_BUILD_TESTS=OFF
cmake --build build
cmake --install build --prefix /opt/tinypbrt
```

```cmake
find_package(tinypbrt 1.0 REQUIRED CONFIG)   # -DCMAKE_PREFIX_PATH=/opt/tinypbrt
target_link_libraries(my_renderer PRIVATE tinypbrt::tinypbrt)
```

### Static or shared library

tinypbrt is a static library by default. Set `TINYPBRT_BUILD_SHARED=ON` (or the global
`BUILD_SHARED_LIBS=ON`) to build `tinypbrt.dll` / `libtinypbrt.so` / `libtinypbrt.dylib` instead.
Nothing changes in your code: linking `tinypbrt::tinypbrt` adds the `TINYPBRT_SHARED_LIBS` definition
for you, and the public functions are declared with `TPBRT_API`:

| | building tinypbrt | using tinypbrt |
|---|---|---|
| Windows (MSVC, clang-cl, Clang, MinGW) | `__declspec(dllexport)` | `__declspec(dllimport)` |
| Linux / macOS (GCC, Clang) | `__attribute__((visibility("default")))` | same |
| static library | *(empty)* | *(empty)* |

Only the public API (`tpbrt_scene_load_*`, `tpbrt_scene_free`, `tpbrt_get_*`) is exported; everything
else is hidden. On Windows the DLL has to be found at run time - for example copy it next to your
executable:

```cmake
if (WIN32)
    add_custom_command(TARGET my_renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:my_renderer> $<TARGET_FILE_DIR:my_renderer>
        COMMAND_EXPAND_LISTS)
endif()
```

If you build tinypbrt without CMake, define `TINYPBRT_SHARED_LIBS` everywhere and `TINYPBRT_EXPORTS`
only while compiling the library.

## Quick start

```c
#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s scene.pbrt\n", argv[0]);
		return 1;
	}

	tpbrt_scene_t scene;
	const tpbrt_error_t err = tpbrt_scene_load_from_file(argv[1], &scene);
	if (err != TPBRT_ERROR_NONE) {
		char message[128];
		tpbrt_get_error_message(err, message, sizeof(message));
		fprintf(stderr, "failed to load %s: %s (%d)\n", argv[1], message, (int)err);
		return 1; // on error the scene is already freed
	}

	printf("film:   %ux%u -> %.*s\n", scene.film.x_resolution, scene.film.y_resolution, (int)scene.film.file_name.size,
	  scene.film.file_name.data);
	printf("shapes: %zu, lights: %zu, materials: %zu, textures: %zu\n", scene.shapes.count,
	  scene.lights.lights_sources.count, scene.materials.count, scene.textures.count);

	for (size_t i = 0; i < scene.shapes.count; ++i) {
		const tpbrt_shape_t* shape = &scene.shapes.shapes[i];

		// object -> world matrix (row-major: m[row * 4 + column])
		const tpbrt_mat4_t* object_to_world = NULL;
		tpbrt_get_transform(scene.transforms, shape->transform.start_transform, &object_to_world);

		const tpbrt_material_t* material = NULL;
		tpbrt_get_material_by_handle(scene.materials, shape->material, &material);

		if (shape->type == TPBRT_SHAPE_TYPE_SPHERE) {
			printf("sphere r=%g at (%g %g %g)\n", shape->as.sphere.radius, object_to_world->m[3], object_to_world->m[7],
			  object_to_world->m[11]);
		}
		else if (shape->type == TPBRT_SHAPE_TYPE_TRIANGLE_MESH) {
			printf("triangle mesh: %zu vertices, %zu triangles\n", shape->as.triangle_mesh.P.count,
			  shape->as.triangle_mesh.indices.count / 3);
		}
		(void)material;
	}

	tpbrt_scene_free(&scene);
	return 0;
}
```

Scenes can also be parsed from memory. The second argument is the directory used to resolve
`Include`/`Import` paths:

```c
const char* source =
    "Camera \"perspective\" \"float fov\" 45\n"
    "WorldBegin\n"
    "Include \"geometry.pbrt\"\n";

tpbrt_scene_t scene;
tpbrt_error_t err = tpbrt_scene_load_from_memory(source, "assets/scenes", &scene);
```

## Using the scene

Everything you get back lives in `tpbrt_scene_t` and is owned by it: call `tpbrt_scene_free`
once you are done (and only after a successful load).

| Field | Contents |
|---|---|
| `camera`, `film`, `sampler`, `integrator`, `accelerator`, `filter`, `options` | scene-wide settings with pbrt-v4 defaults filled in |
| `start_time`, `end_time` | shutter interval from `TransformTimes` |
| `shapes` | shapes outside of `ObjectBegin` blocks |
| `objects`, `instances` | object definitions (their shapes are in `objects.objects[i].children`) and their instances |
| `materials`, `textures`, `medias` | every material, texture and medium, named or not |
| `lights.lights_sources`, `lights.area_lights` | light sources and area lights referenced by shapes |
| `transforms` | shared pool of matrices referenced by transform handles |

Things worth knowing:

- **Tagged unions.** Every object with variants has a `type` field and an `as` union, e.g.
  `shape->type == TPBRT_SHAPE_TYPE_SPHERE` → `shape->as.sphere`.
- **Strings are not NUL-terminated.** `tpbrt_string_t` is `{ data, size }`; print it with
  `printf("%.*s", (int)s.size, s.data)`.
- **Handles instead of pointers.** Shapes refer to materials, area lights, media and transforms by
  small handles. Resolve them with `tpbrt_get_material_by_handle`, `tpbrt_get_area_light_by_handle`,
  `tpbrt_get_media_by_handle`, `tpbrt_get_texture_by_handle`, `tpbrt_get_object_by_handle` and
  `tpbrt_get_transform`. Named objects can be looked up with the `*_by_name` variants.
- **Transforms.** Matrices are row-major `float m[16]` mapping object space to world space (for the
  camera: camera to world). Shapes, instances and the camera have a `start_transform` and an
  `end_transform` for motion blur; both handles are equal when the transform is not animated.
  Lights, media and textures have a single transform, as in pbrt-v4.
- **Texture parameters** (`tpbrt_texture_handle_t`) are either a reference to a texture
  (`value_type == TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX`) or an inline value
  (`..._SPECTRUM`); plain floats are stored as constant spectra (`as.spectrum.as.constant`).
- **Spectra** (`tpbrt_spectrum_t`) are RGB (in the color space active at that point), blackbody
  temperature, wavelength/value pairs, a built-in name, a file name or a constant.
- **External files** (PLY meshes, images, spectra, NanoVDB grids, lens files) are not opened; their
  paths are stored as written in the scene file.

## Error handling

All functions return `tpbrt_error_t` (`TPBRT_ERROR_NONE` on success). Loading stops at the first
error and frees everything that was allocated, so a failed `tpbrt_scene_load_*` needs no cleanup.
`tpbrt_get_error_message` turns a code into a readable message:

```c
char message[128];
tpbrt_get_error_message(err, message, sizeof(message));
```

## CMake options

| Option | Default | Description |
|---|---|---|
| `TINYPBRT_BUILD_SHARED` | `BUILD_SHARED_LIBS` | build tinypbrt as a shared library ([details](#static-or-shared-library)) |
| `BUILD_SHARED_LIBS` | `OFF` | CMake's global default for all libraries (also GoogleTest in this repository) |
| `TINYPBRT_BUILD_TESTS` | top-level only | build the GoogleTest suite and the C API test |
| `TINYPBRT_BUILD_BENCHMARK` | top-level only | build benchmarks |
| `TINYPBRT_BUILD_EXAMPLES` | top-level only | build examples |
| `TINYPBRT_BUILD_DOCUMENTATION` | top-level only | generate Doxygen documentation |
| `TINYPBRT_BUILD_COVERAGE` | top-level only | coverage instrumentation (Clang) |
| `TINYPBRT_ENABLE_CLANG_TIDY` | top-level only | run clang-tidy while building |
| `TINYPBRT_WARNINGS_AS_ERRORS` | top-level only | treat compiler warnings as errors when building tinypbrt |
| `TINYPBRT_ENABLE_SANITIZERS` | `OFF` | build the library and tests with sanitizers |
| `TINYPBRT_SANITIZERS` | `address;undefined` | sanitizers to use (`address`, `undefined`, `leak`, `memory`, `thread`) |
| `TINYPBRT_INSTALL` | top-level only | generate install rules |

Compiler warnings and optimization flags used to build tinypbrt are private to the library: they are
not added to your targets. Link-time optimization and static linking are left to your project
(e.g. `CMAKE_INTERPROCEDURAL_OPTIMIZATION`).


## Building and testing tinypbrt

```sh
git clone https://github.com/MAIPA01/tinypbrt.git
cd tinypbrt

# Linux / macOS: Clang, Debug, AddressSanitizer + UndefinedBehaviorSanitizer + LeakSanitizer
cmake --preset linux-debug-sanitizers
cmake --build build/linux-debug-sanitizers
ctest --preset linux-debug-sanitizers
```

On Windows (Clang + Ninja) use the `x64-debug-c11` preset, or `x64-debug-c11-sanitizers` for
AddressSanitizer + UndefinedBehaviorSanitizer:

```sh
cmake --preset x64-debug-c11-sanitizers
cmake --build build/x64-debug-c11-sanitizers
ctest --preset x64-debug-c11-sanitizers
```

The shared-library build has its own presets: `linux-debug-shared-sanitizers` and
`x64-debug-c11-shared` (the GoogleTest suite then links a static copy of the library, because it
also tests internal functions; `tinypbrt-c-api-tests` runs against the DLL / `.so`).

Without presets:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DTINYPBRT_ENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The test suite (`tinypbrt-tests`, GoogleTest, C++17) covers the parser, parameter handling, every
directive and object type, error paths and memory safety under the sanitizers; `tinypbrt-c-api-tests`
checks that the public headers compile and work as strict C11.

## License

tinypbrt is released under the BSD 3-Clause License with an attribution requirement - see
[LICENSE](LICENSE) for the full terms.
