#include "font_policy.h"

typedef struct {
  uint32_t max_diagonal_px;
  ui_font_policy_t fonts;
} ui_font_policy_entry_t;

static const ui_font_policy_entry_t font_policy_entries[] = {
#define UI_FONT_POLICY_ENTRY(max_diagonal_px, small_font_px, medium_font_px)   \
  {max_diagonal_px, {small_font_px, medium_font_px}},
#include "font_policy.def"
#undef UI_FONT_POLICY_ENTRY
};

#define FONT_POLICY_ENTRY_COUNT                                                \
  (sizeof(font_policy_entries) / sizeof(font_policy_entries[0]))

/* The large accessibility policy retains the current branch's font sizes. */
static const ui_font_policy_entry_t large_text_policy_entries[] = {
    {755, {16, 22}},
    {0, {34, 48}},
};

#define LARGE_TEXT_POLICY_ENTRY_COUNT                                           \
  (sizeof(large_text_policy_entries) / sizeof(large_text_policy_entries[0]))

static ui_font_policy_t
font_policy_for_entries(const ui_font_policy_entry_t *entries,
                        uint32_t entry_count, uint32_t width, uint32_t height) {
  uint64_t diagonal_sq = (uint64_t)width * width + (uint64_t)height * height;

  for (uint32_t i = 0; i < entry_count; i++) {
    uint32_t max_diagonal = entries[i].max_diagonal_px;
    if (max_diagonal == 0 ||
        diagonal_sq < (uint64_t)max_diagonal * max_diagonal) {
      return entries[i].fonts;
    }
  }

  return entries[entry_count - 1].fonts;
}

ui_font_policy_t ui_font_policy_for_display(uint32_t width, uint32_t height) {
  return font_policy_for_entries(font_policy_entries, FONT_POLICY_ENTRY_COUNT,
                                 width, height);
}

ui_font_policy_t ui_font_policy_for_large_text(uint32_t width, uint32_t height) {
  return font_policy_for_entries(large_text_policy_entries,
                                 LARGE_TEXT_POLICY_ENTRY_COUNT, width, height);
}
