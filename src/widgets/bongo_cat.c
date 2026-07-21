/*
 * Keypress-driven bongo cat widget.
 *
 * Art originally from the ZMK community bongo cat (MIT).
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <lvgl.h>

#include "bongo_cat.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Frames used for the two-state keypress animation.
LV_IMG_DECLARE(idle_img1);
LV_IMG_DECLARE(fast_img1);
LV_IMG_DECLARE(fast_img2);

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct bongo_cat_state {
    bool typing;
    bool toggle;
};

// Number of keys currently held and which slap frame to show next. Tracked in the event accessor (runs per event)
static int keys_held = 0;
static bool slap_toggle = false;

static struct bongo_cat_state bongo_cat_get_state(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);
    if (ev != NULL) {
        if (ev->state) {
            keys_held++;
            slap_toggle = !slap_toggle;
        } else if (keys_held > 0) {
            keys_held--;
        }
    }
    return (struct bongo_cat_state){.typing = keys_held > 0, .toggle = slap_toggle};
}

static void bongo_cat_set_img(lv_obj_t *img, struct bongo_cat_state state) {
    if (!state.typing) {
        lv_img_set_src(img, &idle_img1);
    } else {
        lv_img_set_src(img, state.toggle ? &fast_img1 : &fast_img2);
    }
}

static void bongo_cat_update_cb(struct bongo_cat_state state) {
    struct zmk_widget_bongo_cat *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { bongo_cat_set_img(widget->obj, state); }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_bongo_cat, struct bongo_cat_state, bongo_cat_update_cb,
                            bongo_cat_get_state)
ZMK_SUBSCRIPTION(widget_bongo_cat, zmk_position_state_changed);

int zmk_widget_bongo_cat_init(struct zmk_widget_bongo_cat *widget, lv_obj_t *parent) {
    widget->obj = lv_img_create(parent);
    lv_img_set_src(widget->obj, &idle_img1);

    sys_slist_append(&widgets, &widget->node);

    widget_bongo_cat_init();
    return 0;
}

lv_obj_t *zmk_widget_bongo_cat_obj(struct zmk_widget_bongo_cat *widget) { return widget->obj; }
