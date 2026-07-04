Import("env")

from pathlib import Path
import re


MARKER = "Stabilizacja: przy braku RAM nie dopuszczaj do zapisu przez null pointer."
GLYPH_MARKER = "Stabilizacja: brak RAM na bufor glyph, pomiń znak bez restartu."

GUARD_BLOCK = (
    "\n"
    "  if (!gUnicode || !gHeight || !gWidth || !gxAdvance || !gdY || !gdX || !gBitmap)\n"
    "  {\n"
    "    // Stabilizacja: przy braku RAM nie dopuszczaj do zapisu przez null pointer.\n"
    "    unloadFont();\n"
    "    return;\n"
    "  }\n"
)

GLYPH_GUARD_BLOCK = (
    "\n"
    "      if (pbuffer == nullptr)\n"
    "      {\n"
    "        // Stabilizacja: brak RAM na bufor glyph, pomiń znak bez restartu.\n"
    "        cursor_x += gxAdvance[gNum];\n"
    "        bg_cursor_x = cursor_x;\n"
    "        last_cursor_x = cursor_x;\n"
    "        return;\n"
    "      }\n"
)


def patch_smooth_font(cpp_path: Path) -> None:
    if not cpp_path.exists():
        print(f"[smooth-font-patch] Skip, file not found: {cpp_path}")
        return

    text = cpp_path.read_text(encoding="utf-8", errors="ignore")
    metrics_patched = MARKER in text
    glyph_patched = GLYPH_MARKER in text

    metrics_pattern = re.compile(
        r"(\s*gBitmap\s*=\s*\(uint32_t\*\)malloc\(\s*gFont\.gCount\s*\*\s*4\s*\);[^\n]*\n\s*}\n)",
        re.MULTILINE,
    )

    glyph_pattern = re.compile(
        r"(\s*pbuffer\s*=\s*\(uint8_t\*\)malloc\(gWidth\[gNum\]\);\s*\n\s*}\n)",
        re.MULTILINE,
    )

    updated = text
    changed = False

    if not metrics_patched:
        updated_metrics, count_metrics = metrics_pattern.subn(r"\1" + GUARD_BLOCK, updated, count=1)
        if count_metrics == 1:
            updated = updated_metrics
            changed = True
        else:
            print(f"[smooth-font-patch] Metrics pattern not found: {cpp_path}")

    if not glyph_patched:
        updated_glyph, count_glyph = glyph_pattern.subn(r"\1" + GLYPH_GUARD_BLOCK, updated, count=1)
        if count_glyph == 1:
            updated = updated_glyph
            changed = True
        else:
            print(f"[smooth-font-patch] Glyph pattern not found: {cpp_path}")

    if not changed:
        print(f"[smooth-font-patch] Already patched: {cpp_path}")
        return

    cpp_path.write_text(updated, encoding="utf-8")
    print(f"[smooth-font-patch] Patched: {cpp_path}")


libdeps_dir = Path(env.subst("$PROJECT_LIBDEPS_DIR"))
pioenv = env.subst("$PIOENV")
target = libdeps_dir / pioenv / "TFT_eSPI" / "Extensions" / "Smooth_font.cpp"

patch_smooth_font(target)
