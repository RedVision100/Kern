// Login Settings Page - Pre-login configuration (security, display,
// screensaver)

#include "login_settings.h"
#include "login.h"
#include "../../core/settings.h"
#include "../../ui/dropdown_page.h"
#include "../../ui/input_helpers.h"
#include "../../ui/menu.h"
#include "../../ui/theme_widgets.h"
#include "../../utils/session.h"
#include "../settings/firmware_update.h"
#include "security_settings.h"
#include <bsp/display.h>
#include <lvgl.h>

// -- Top-level settings menu --
static ui_menu_t *settings_menu = NULL;
static lv_obj_t *settings_screen = NULL;
static void (*return_callback)(void) = NULL;

// -- Brightness detail page --
static lv_obj_t *brightness_screen = NULL;
static lv_obj_t *brightness_slider = NULL;
static lv_obj_t *brightness_label = NULL;

// -- Screensaver detail page --
static lv_obj_t *screensaver_screen = NULL;

// -- Text-size detail page --
static lv_obj_t *text_size_screen = NULL;
static bool text_size_rebuild_pending = false;

// Forward declarations
static void show_brightness_page(void);
static void destroy_brightness_page(void);
static void show_screensaver_page(void);
static void destroy_screensaver_page(void);
static void show_text_size_page(void);
static void destroy_text_size_page(void);
static void rebuild_settings_menu(void);
static void rebuild_prelogin_hierarchy_async(void *user_data);

// ── Screen Brightness detail page ──

static void brightness_slider_cb(lv_event_t *e) {
  lv_obj_t *slider = lv_event_get_target(e);
  bsp_display_brightness_set(lv_slider_get_value(slider));
}

static void brightness_back_cb(lv_event_t *e) {
  (void)e;
  int32_t val = lv_slider_get_value(brightness_slider);
  settings_set_brightness((uint8_t)val);
  destroy_brightness_page();
  ui_menu_show(settings_menu);
}

static void show_brightness_page(void) {
  ui_menu_hide(settings_menu);

  brightness_screen = theme_create_page_container(lv_screen_active());

  ui_create_back_button(brightness_screen, brightness_back_cb);
  theme_create_page_title(brightness_screen, "Screen Brightness");

  uint8_t cur = settings_get_brightness();
  brightness_label = lv_label_create(brightness_screen);
  lv_label_set_text(brightness_label, "Brightness");
  lv_obj_set_style_text_font(brightness_label, theme_font_medium(), 0);
  lv_obj_set_style_text_color(brightness_label, primary_color(), 0);
  lv_obj_align(brightness_label, LV_ALIGN_CENTER, 0, -theme_min_touch_size());

  brightness_slider = lv_slider_create(brightness_screen);
  lv_slider_set_range(brightness_slider, BRIGHTNESS_MIN, 100);
  lv_slider_set_value(brightness_slider, cur, LV_ANIM_OFF);
  lv_obj_set_width(brightness_slider, LV_HOR_RES * 60 / 100);
  lv_obj_align(brightness_slider, LV_ALIGN_CENTER, 0, theme_button_spacing());
  theme_apply_slider(brightness_slider);

  lv_obj_add_event_cb(brightness_slider, brightness_slider_cb,
                      LV_EVENT_VALUE_CHANGED, NULL);
}

static void destroy_brightness_page(void) {
  if (brightness_screen) {
    lv_obj_del(brightness_screen);
    brightness_screen = NULL;
  }
  brightness_slider = NULL;
  brightness_label = NULL;
}

// ── Screensaver detail page ──

// Screensaver options: index → seconds (0=off)
static const uint16_t screensaver_values[] = {0, 60, 120, 300, 900, 1800};
static const char *screensaver_options =
    "Off\n1 min\n2 min\n5 min\n15 min\n30 min";

static void screensaver_dropdown_cb(lv_event_t *e) {
  uint16_t sel = lv_dropdown_get_selected(lv_event_get_target(e));
  if (sel < sizeof(screensaver_values) / sizeof(screensaver_values[0])) {
    settings_set_screensaver_timeout(screensaver_values[sel]);
    session_set_screensaver_timeout(screensaver_values[sel]);
  }
}

static void screensaver_back_cb(lv_event_t *e) {
  (void)e;
  destroy_screensaver_page();
  ui_menu_show(settings_menu);
}

static void show_screensaver_page(void) {
  ui_menu_hide(settings_menu);
  uint16_t sel = ui_index_of_u16(
      screensaver_values,
      sizeof(screensaver_values) / sizeof(screensaver_values[0]),
      settings_get_screensaver_timeout(), 2 /* 2 min */);
  screensaver_screen =
      ui_dropdown_page_create("Screensaver", NULL, screensaver_options, sel,
                              screensaver_dropdown_cb, screensaver_back_cb);
}

static void destroy_screensaver_page(void) {
  if (screensaver_screen) {
    lv_obj_delete(screensaver_screen);
    screensaver_screen = NULL;
  }
}

// ── Text-size detail page ──

static const uint16_t text_size_values[] = {TEXT_SIZE_STANDARD, TEXT_SIZE_LARGE};
static const char *text_size_options = "Standard\nLarge";

static void rebuild_prelogin_hierarchy_async(void *user_data) {
  (void)user_data;
  text_size_rebuild_pending = false;

  /* This runs after the dropdown's value-change event has returned, so deleting
   * the detail page and its ancestors cannot invalidate the active event path. */
  login_settings_page_destroy();
  login_page_destroy();
  login_page_create(lv_screen_active());
  login_page_show();
}

static void text_size_dropdown_cb(lv_event_t *e) {
  uint16_t sel = lv_dropdown_get_selected(lv_event_get_target(e));
  if (sel >= sizeof(text_size_values) / sizeof(text_size_values[0]))
    return;

  text_size_t size = (text_size_t)text_size_values[sel];
  settings_set_text_size(size);
  theme_set_text_size(size);

  if (!text_size_rebuild_pending) {
    text_size_rebuild_pending =
        lv_async_call(rebuild_prelogin_hierarchy_async, NULL) == LV_RESULT_OK;
  }
}

static void text_size_back_cb(lv_event_t *e) {
  (void)e;
  destroy_text_size_page();
  ui_menu_show(settings_menu);
}

static void show_text_size_page(void) {
  ui_menu_hide(settings_menu);
  uint16_t sel = ui_index_of_u16(
      text_size_values, sizeof(text_size_values) / sizeof(text_size_values[0]),
      settings_get_text_size(), TEXT_SIZE_STANDARD);
  text_size_screen =
      ui_dropdown_page_create("Text Size", NULL, text_size_options, sel,
                              text_size_dropdown_cb, text_size_back_cb);
}

static void destroy_text_size_page(void) {
  if (text_size_screen) {
    lv_obj_delete(text_size_screen);
    text_size_screen = NULL;
  }
}

// ── Security submenu ──

static void security_return_cb(void) {
  security_settings_page_destroy();
  ui_menu_show(settings_menu);
}

static void security_cb(void) {
  ui_menu_hide(settings_menu);
  security_settings_page_create(lv_screen_active(), security_return_cb);
  security_settings_page_show();
}

// ── Firmware update submenu ──

static void firmware_update_return_cb(void) {
  firmware_update_page_destroy();
  ui_menu_show(settings_menu);
}

static void firmware_update_cb(void) {
  ui_menu_hide(settings_menu);
  firmware_update_page_create(lv_screen_active(), firmware_update_return_cb);
  firmware_update_page_show();
}

// ── Category menu callbacks ──

static void brightness_cb(void) { show_brightness_page(); }

static void text_size_cb(void) { show_text_size_page(); }

static void screensaver_cb(void) { show_screensaver_page(); }

static void settings_back_cb(void) {
  if (return_callback)
    return_callback();
}

static void rebuild_settings_menu(void) {
  if (settings_menu) {
    ui_menu_destroy(settings_menu);
    settings_menu = NULL;
  }

  settings_menu = ui_menu_create(settings_screen, "Settings", settings_back_cb);
  ui_menu_add_entry(settings_menu, "Security", security_cb);
  ui_menu_add_entry(settings_menu, "Screen Brightness", brightness_cb);
  ui_menu_add_entry(settings_menu, "Text Size", text_size_cb);
  ui_menu_add_entry(settings_menu, "Screensaver", screensaver_cb);
  ui_menu_add_entry(settings_menu, "Firmware Update", firmware_update_cb);
}

// ── Public lifecycle ──

void login_settings_page_create(lv_obj_t *parent, void (*return_cb)(void)) {
  // Statics may dangle if session expiry cleaned the screen while a detail
  // page was open; drop them so destroy doesn't delete freed objects.
  settings_menu = NULL;
  brightness_screen = NULL;
  brightness_slider = NULL;
  brightness_label = NULL;
  screensaver_screen = NULL;
  text_size_screen = NULL;
  return_callback = return_cb;
  settings_screen = theme_create_page_container(parent);
  rebuild_settings_menu();
}

void login_settings_page_show(void) {
  if (settings_menu)
    ui_menu_show(settings_menu);
}

void login_settings_page_hide(void) {
  if (settings_menu)
    ui_menu_hide(settings_menu);
}

void login_settings_page_destroy(void) {
  lv_async_call_cancel(rebuild_prelogin_hierarchy_async, NULL);
  text_size_rebuild_pending = false;
  security_settings_page_destroy();
  destroy_brightness_page();
  destroy_screensaver_page();
  destroy_text_size_page();
  if (settings_menu) {
    ui_menu_destroy(settings_menu);
    settings_menu = NULL;
  }
  if (settings_screen) {
    lv_obj_del(settings_screen);
    settings_screen = NULL;
  }
  return_callback = NULL;
}
