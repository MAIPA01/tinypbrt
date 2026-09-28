#pragma once
#ifndef _TINYPBRT_TESTS_PCH_HPP_
#define _TINYPBRT_TESTS_PCH_HPP_

#ifdef __cplusplus
extern "C" {
#endif

// API
#include <tinypbrt/tinypbrt.h>

// INTERNAL
#include <tinypbrt/detail/accelerator_internal.h>
#include <tinypbrt/detail/camera_internal.h>
#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/film_internal.h>
#include <tinypbrt/detail/filter_internal.h>
#include <tinypbrt/detail/integrator_internal.h>
#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/parser_internal.h>
#include <tinypbrt/detail/sampler_internal.h>
#include <tinypbrt/detail/shape_internal.h>
#include <tinypbrt/detail/texture_internal.h>
#include <tinypbrt/detail/token_internal.h>
#include <tinypbrt/detail/tokenizer_internal.h>
#include <tinypbrt/pch.h>

#ifdef __cplusplus
}
#endif

// third party
#include <gtest/gtest.h>

#endif // _TINYPBRT_PCH_H_
