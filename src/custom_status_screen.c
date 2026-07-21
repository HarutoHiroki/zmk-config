/*
 * Custom OLED status screen for the Lily58 (128x32).
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/battery_status.h>
#include <zmk/display/widgets/output_status.h>

#include "widgets/bongo_cat.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static struct zmk_widget_bongo_cat bongo_cat_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
static struct zmk_widget_layer_status layer_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_STATUS)
static struct zmk_widget_battery_status battery_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
static struct zmk_widget_output_status output_status_widget;
#endif

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
    // Endpoint (USB/BLE) + selected BLE profile number, top-left.
    zmk_widget_output_status_init(&output_status_widget, screen);
    lv_obj_set_style_text_font(zmk_widget_output_status_obj(&output_status_widget),
                               lv_theme_get_font_small(screen), LV_PART_MAIN);
    lv_obj_align(zmk_widget_output_status_obj(&output_status_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_STATUS)
    // Battery top-right; the cat/label sit in the empty space directly below it.
    zmk_widget_battery_status_init(&battery_status_widget, screen);
    lv_obj_set_style_text_font(zmk_widget_battery_status_obj(&battery_status_widget),
                               lv_theme_get_font_small(screen), LV_PART_MAIN);
    lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget), LV_ALIGN_TOP_RIGHT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
    zmk_widget_layer_status_init(&layer_status_widget, screen);
    lv_obj_set_style_text_font(zmk_widget_layer_status_obj(&layer_status_widget),
                               lv_theme_get_font_small(screen), LV_PART_MAIN);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), LV_ALIGN_BOTTOM_LEFT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    // Left half: per-half bongo cat, bottom-right (transparent bg, so it only
    // draws the cat over whatever is behind it).
    zmk_widget_bongo_cat_init(&bongo_cat_widget, screen);
    lv_obj_align(zmk_widget_bongo_cat_obj(&bongo_cat_widget), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
#else
    // Right half: static nickname/label, bottom-right.
    lv_obj_t *label = lv_label_create(screen);
    lv_obj_set_style_text_font(label, lv_theme_get_font_small(screen), LV_PART_MAIN);
    lv_label_set_text(label, CONFIG_OLED_LABEL_TEXT);
    lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
#endif

    return screen;
}
