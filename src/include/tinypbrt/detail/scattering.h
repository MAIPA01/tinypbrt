#pragma once
#ifndef _TINYPBRT_SCATTERING_H_
#define _TINYPBRT_SCATTERING_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup scattering-public Scattering
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Scattering builtins.
	 * @ingroup scattering-public
	 */
	typedef enum tpbrt_scattering_builtin_t {
		/// @brief Apple.
		TPBRT_SCATTERING_BUILTIN_APPLE						 = 0,
		/// @brief Chicken 1.
		TPBRT_SCATTERING_BUILTIN_CHICKEN_1					 = 1,
		/// @brief Chicken 2.
		TPBRT_SCATTERING_BUILTIN_CHICKEN_2					 = 2,
		/// @brief Cream.
		TPBRT_SCATTERING_BUILTIN_CREAM						 = 3,
		/// @brief Ketchup.
		TPBRT_SCATTERING_BUILTIN_KETCHUP					 = 4,
		/// @brief Marble.
		TPBRT_SCATTERING_BUILTIN_MARBLE						 = 5,
		/// @brief Potato.
		TPBRT_SCATTERING_BUILTIN_POTATO						 = 6,
		/// @brief Skim milk.
		TPBRT_SCATTERING_BUILTIN_SKIM_MILK					 = 7,
		/// @brief Skin 1.
		TPBRT_SCATTERING_BUILTIN_SKIN_1						 = 8,
		/// @brief Skin 2.
		TPBRT_SCATTERING_BUILTIN_SKIN_2						 = 9,
		/// @brief Spectralon.
		TPBRT_SCATTERING_BUILTIN_SPECTRALON					 = 10,
		/// @brief Whole milk.
		TPBRT_SCATTERING_BUILTIN_WHOLE_MILK					 = 11,
		/// @brief Low fat milk.
		TPBRT_SCATTERING_BUILTIN_LOW_FAT_MILK				 = 12,
		/// @brief Reduced milk.
		TPBRT_SCATTERING_BUILTIN_REDUCED_MILK				 = 13,
		/// @brief Regular milk.
		TPBRT_SCATTERING_BUILTIN_REGULAR_MILK				 = 14,
		/// @brief Espresso.
		TPBRT_SCATTERING_BUILTIN_ESPRESSO					 = 15,
		/// @brief Mint mocha coffee.
		TPBRT_SCATTERING_BUILTIN_MINT_MOCHA_COFFEE			 = 16,
		/// @brief Low fat soy milk.
		TPBRT_SCATTERING_BUILTIN_LOW_FAT_SOY_MILK			 = 17,
		/// @brief Regular soy milk.
		TPBRT_SCATTERING_BUILTIN_REGULAR_SOY_MILK			 = 18,
		/// @brief Low fat chocolate milk.
		TPBRT_SCATTERING_BUILTIN_LOW_FAT_CHOCOLATE_MILK		 = 19,
		/// @brief Regular chocolate milk.
		TPBRT_SCATTERING_BUILTIN_REGULAR_CHOCOLATE_MILK		 = 20,
		/// @brief Coke.
		TPBRT_SCATTERING_BUILTIN_COKE						 = 21,
		/// @brief Pepsi.
		TPBRT_SCATTERING_BUILTIN_PEPSI						 = 22,
		/// @brief Sprite.
		TPBRT_SCATTERING_BUILTIN_SPRITE						 = 23,
		/// @brief Gatorade.
		TPBRT_SCATTERING_BUILTIN_GATORADE					 = 24,
		/// @brief Chardonnay.
		TPBRT_SCATTERING_BUILTIN_CHARDONNAY					 = 25,
		/// @brief White Zinfandel.
		TPBRT_SCATTERING_BUILTIN_WHITE_ZINFANDEL			 = 26,
		/// @brief Merlot.
		TPBRT_SCATTERING_BUILTIN_MERLOT						 = 27,
		/// @brief Budweiser beer.
		TPBRT_SCATTERING_BUILTIN_BUDWEISER_BEER				 = 28,
		/// @brief Coors light beer.
		TPBRT_SCATTERING_BUILTIN_COORS_LIGHT_BEER			 = 29,
		/// @brief Clorox.
		TPBRT_SCATTERING_BUILTIN_CLOROX						 = 30,
		/// @brief Apple juice.
		TPBRT_SCATTERING_BUILTIN_APPLE_JUICE				 = 31,
		/// @brief Cranberry juice.
		TPBRT_SCATTERING_BUILTIN_CRANBERRY_JUICE			 = 32,
		/// @brief Grape juice.
		TPBRT_SCATTERING_BUILTIN_GRAPE_JUICE				 = 33,
		/// @brief Ruby grapefruit juice.
		TPBRT_SCATTERING_BUILTIN_RUBY_GRAPEFRUIT_JUICE		 = 34,
		/// @brief White grapefruit juice.
		TPBRT_SCATTERING_BUILTIN_WHITE_GRAPEFRUIT_JUICE		 = 35,
		/// @brief Shampoo.
		TPBRT_SCATTERING_BUILTIN_SHAMPOO					 = 36,
		/// @brief Strawberry shampoo.
		TPBRT_SCATTERING_BUILTIN_STRAWBERRY_SHAMPOO			 = 37,
		/// @brief Head & Shoulders shampoo.
		TPBRT_SCATTERING_BUILTIN_HEAD_AND_SHOULDERS_SHAMPOO	 = 38,
		/// @brief Lemon tea powder.
		TPBRT_SCATTERING_BUILTIN_LEMON_TEA_POWDER			 = 39,
		/// @brief Orange powder.
		TPBRT_SCATTERING_BUILTIN_ORANGE_POWDER				 = 40,
		/// @brief Pink lemonade powder.
		TPBRT_SCATTERING_BUILTIN_PINK_LEMONADE_POWDER		 = 41,
		/// @brief Cappuccino powder.
		TPBRT_SCATTERING_BUILTIN_CAPPUCCINO_POWDER			 = 42,
		/// @brief Salt powder.
		TPBRT_SCATTERING_BUILTIN_SALT_POWDER				 = 43,
		/// @brief Sugar powder.
		TPBRT_SCATTERING_BUILTIN_SUGAR_POWDER				 = 44,
		/// @brief Mocha powder.
		TPBRT_SCATTERING_BUILTIN_SUISSE_MOCHA_POWDER		 = 45,
		/// @brief Pacific ocean surface water.
		TPBRT_SCATTERING_BUILTIN_PACIFIC_OCEAN_SURFACE_WATER = 46,
		/// @brief Count of available scattering builtins.
		TPBRT_SCATTERING_BUILTIN_MAX_NUM					 = 47,
	} tpbrt_scattering_builtin_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCATTERING_H_
