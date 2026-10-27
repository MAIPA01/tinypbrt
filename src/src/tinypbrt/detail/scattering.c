#include <tinypbrt/pch.h>

#include <tinypbrt/detail/scattering_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_scattering_builtin_from_string(const tpbrt_string_t builtin_str,
	  tpbrt_scattering_builtin_t* const builtin) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_SCATTERING_BUILTIN_MAX_NUM] = {
			TPBRT_STRING_INIT("Apple"),
			TPBRT_STRING_INIT("Chicken1"),
			TPBRT_STRING_INIT("Chicken2"),
			TPBRT_STRING_INIT("Cream"),
			TPBRT_STRING_INIT("Ketchup"),
			TPBRT_STRING_INIT("Marble"),
			TPBRT_STRING_INIT("Potato"),
			TPBRT_STRING_INIT("Skimmilk"),
			TPBRT_STRING_INIT("Skin1"),
			TPBRT_STRING_INIT("Skin2"),
			TPBRT_STRING_INIT("Spectralon"),
			TPBRT_STRING_INIT("Wholemilk"),
			TPBRT_STRING_INIT("Lowfat Milk"),
			TPBRT_STRING_INIT("Reduced Milk"),
			TPBRT_STRING_INIT("Regular Milk"),
			TPBRT_STRING_INIT("Espresso"),
			TPBRT_STRING_INIT("Mint Mocha Coffee"),
			TPBRT_STRING_INIT("Lowfat Soy Milk"),
			TPBRT_STRING_INIT("Regular Soy Milk"),
			TPBRT_STRING_INIT("Lowfat Chocolate Milk"),
			TPBRT_STRING_INIT("Regular Chocolate Milk"),
			TPBRT_STRING_INIT("Coke"),
			TPBRT_STRING_INIT("Pepsi"),
			TPBRT_STRING_INIT("Sprite"),
			TPBRT_STRING_INIT("Gatorade"),
			TPBRT_STRING_INIT("Chardonnay"),
			TPBRT_STRING_INIT("White Zinfandel"),
			TPBRT_STRING_INIT("Merlot"),
			TPBRT_STRING_INIT("Budweiser Beer"),
			TPBRT_STRING_INIT("Coors Light Beer"),
			TPBRT_STRING_INIT("Clorox"),
			TPBRT_STRING_INIT("Apple Juice"),
			TPBRT_STRING_INIT("Cranberry Juice"),
			TPBRT_STRING_INIT("Grape Juice"),
			TPBRT_STRING_INIT("Ruby Grapefruit Juice"),
			TPBRT_STRING_INIT("White Grapefruit Juice"),
			TPBRT_STRING_INIT("Shampoo"),
			TPBRT_STRING_INIT("Strawberry Shampoo"),
			TPBRT_STRING_INIT("Head & Shoulders Shampoo"),
			TPBRT_STRING_INIT("Lemon Tea Powder"),
			TPBRT_STRING_INIT("Orange Powder"),
			TPBRT_STRING_INIT("Pink Lemonade Powder"),
			TPBRT_STRING_INIT("Cappuccino Powder"),
			TPBRT_STRING_INIT("Salt Powder"),
			TPBRT_STRING_INIT("Sugar Powder"),
			TPBRT_STRING_INIT("Suisse Mocha Powder"),
			TPBRT_STRING_INIT("Pacific Ocean Surface Water"),
		};

			if (builtin == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(builtin_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_SCATTERING_BUILTIN; }

			for (tpbrt_scattering_builtin_t p = 0; p < TPBRT_SCATTERING_BUILTIN_MAX_NUM; ++p) {
					if (tpbrt_string_equals(builtin_str, TYPES_STRS[p]) == TPBRT_TRUE) {
						*builtin = p;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_SCATTERING_BUILTIN;
	}

#ifdef __cplusplus
}
#endif
