#include <stdio.h>

#include "ui/font_policy.h"

static int failures = 0;

static void expect_policy(const char *name, ui_font_policy_t actual,
                          uint16_t expected_small, uint16_t expected_medium) {
  printf("Testing: %s... ", name);
  if (actual.small_px != expected_small || actual.medium_px != expected_medium) {
    printf("FAIL: got {%u, %u}, expected {%u, %u}\n", actual.small_px,
           actual.medium_px, expected_small, expected_medium);
    failures++;
    return;
  }

  printf("PASS\n");
}

int main(void) {
  expect_policy("standard wave_4b policy",
                ui_font_policy_for_display(720, 720), 24, 32);
  expect_policy("large wave_4b policy",
                ui_font_policy_for_large_text(720, 720), 34, 48);
  expect_policy("large wave_35 policy",
                ui_font_policy_for_large_text(320, 480), 16, 22);

  if (failures) {
    printf("\n%d test(s) failed\n", failures);
    return 1;
  }

  printf("\nAll font policy tests passed\n");
  return 0;
}
