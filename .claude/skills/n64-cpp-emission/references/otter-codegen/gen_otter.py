#!/usr/bin/env python3
"""Preserve matched Otter bodies while regenerating switchable layouts.

The function bodies live beside this script in source.cpp.  Keep edits there;
this script decides only where each preserved body is emitted:

    class_inline  ->  s32 vfunc4(const char* path) { ... } inside class Otter
    header_inline ->  inline s32 Otter::vfunc4(...) { ... } after the class
    cpp_member    ->  s32 Otter::vfunc4(...) { ... } in src/otter.cpp
    cpp_inline    ->  inline s32 Otter::vfunc4(...) { ... } in src/otter.cpp

That keeps body edits orthogonal to the layout experiments.
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
from textwrap import indent


ROOT = Path(__file__).resolve().parents[5]
SOURCE_OF_TRUTH = Path(__file__).with_name("source.cpp")
HEADER_OUT = ROOT / "include" / "otter.h"
CPP_OUT = ROOT / "src" / "otter.cpp"

VALID_PLACEMENTS = {"class_inline", "header_inline", "cpp_member", "cpp_inline"}


@dataclass(frozen=True)
class FunctionSpec:
    key: str
    source_sig: str
    class_decl: str
    cpp_sig: str


FUNCTIONS = [
    FunctionSpec("ctor", "Otter::Otter()", "Otter()", "Otter::Otter()"),
    FunctionSpec("dtor", "Otter::~Otter()", "virtual ~Otter()", "Otter::~Otter()"),
    FunctionSpec("vfunc2", "s32 Otter::vfunc2()", "virtual s32 vfunc2()", "s32 Otter::vfunc2()"),
    FunctionSpec("vfunc3", "void Otter::vfunc3()", "virtual void vfunc3()", "void Otter::vfunc3()"),
    FunctionSpec(
        "vfunc4",
        "s32 Otter::vfunc4(const char* path)",
        "virtual s32 vfunc4(const char* path)",
        "s32 Otter::vfunc4(const char* path)",
    ),
    FunctionSpec(
        "vfunc5",
        "s32 Otter::vfunc5(const char* name)",
        "virtual s32 vfunc5(const char* name)",
        "s32 Otter::vfunc5(const char* name)",
    ),
    FunctionSpec(
        "vfunc6",
        "s32 Otter::vfunc6(FileInfo* arg1)",
        "virtual s32 vfunc6(FileInfo* arg1)",
        "s32 Otter::vfunc6(FileInfo* arg1)",
    ),
    FunctionSpec(
        "vfunc7",
        "s32 Otter::vfunc7(s32* out1, s32* out2)",
        "virtual s32 vfunc7(s32* out1, s32* out2)",
        "s32 Otter::vfunc7(s32* out1, s32* out2)",
    ),
    FunctionSpec(
        "vfunc8",
        "s32 Otter::vfunc8(s32* out)",
        "virtual s32 vfunc8(s32* out)",
        "s32 Otter::vfunc8(s32* out)",
    ),
    FunctionSpec(
        "vfunc9",
        "s32 Otter::vfunc9(s32 arg1, FileInfo* info)",
        "virtual s32 vfunc9(s32 arg1, FileInfo* info)",
        "s32 Otter::vfunc9(s32 arg1, FileInfo* info)",
    ),
    FunctionSpec(
        "func_80049980",
        "void Otter::func_80049980(const char* path, char* name, char* ext)",
        "static void func_80049980(const char* path, char* name, char* ext)",
        "void Otter::func_80049980(const char* path, char* name, char* ext)",
    ),
    FunctionSpec(
        "func_80049B50",
        "void Otter::func_80049B50()",
        "void func_80049B50()",
        "void Otter::func_80049B50()",
    ),
    FunctionSpec(
        "func_80049B58",
        "void Otter::func_80049B58(s32 mode)",
        "void func_80049B58(s32 mode)",
        "void Otter::func_80049B58(s32 mode)",
    ),
    FunctionSpec(
        "func_80049CC4",
        "void Otter::func_80049CC4(Stream* stream, s32 mode, s32 hasInit)",
        "void func_80049CC4(Stream* stream, s32 mode, s32 hasInit)",
        "void Otter::func_80049CC4(Stream* stream, s32 mode, s32 hasInit)",
    ),
    FunctionSpec(
        "func_80049DA0",
        "Stream* Otter::func_80049DA0()",
        "Stream* func_80049DA0()",
        "Stream* Otter::func_80049DA0()",
    ),
    FunctionSpec(
        "func_80049DAC",
        "char* Otter::func_80049DAC()",
        "char* func_80049DAC()",
        "char* Otter::func_80049DAC()",
    ),
    FunctionSpec(
        "func_80049DB4",
        "s32 Otter::func_80049DB4()",
        "s32 func_80049DB4()",
        "s32 Otter::func_80049DB4()",
    ),
]

FUNCTION_BY_KEY = {spec.key: spec for spec in FUNCTIONS}

NATURAL_CLASS_ORDER = [
    "ctor",
    "dtor",
    "vfunc2",
    "vfunc3",
    "vfunc4",
    "vfunc5",
    "vfunc6",
    "vfunc7",
    "vfunc8",
    "vfunc9",
    "func_80049980",
    "func_80049B50",
    "func_80049B58",
    "func_80049CC4",
    "func_80049DA0",
    "func_80049DAC",
    "func_80049DB4",
]

# Preserve the observed text order when functions are emitted outside the class.
EMISSION_ORDER = [
    "vfunc5",
    "vfunc6",
    "vfunc8",
    "vfunc9",
    "vfunc7",
    "func_80049980",
    "vfunc4",
    "func_80049B50",
    "func_80049B58",
    "vfunc3",
    "vfunc2",
    "func_80049CC4",
    "dtor",
    "ctor",
    "func_80049DA0",
    "func_80049DAC",
    "func_80049DB4",
]

REVERSE_EMISSION_CLASS_ORDER = [
    "func_80049DB4",
    "func_80049DAC",
    "func_80049DA0",
    "ctor",
    "dtor",
    "func_80049CC4",
    "vfunc2",
    "vfunc3",
    "func_80049B58",
    "func_80049B50",
    "vfunc4",
    "func_80049980",
    "vfunc7",
    "vfunc9",
    "vfunc8",
    "vfunc5",
    "vfunc6",
]

CLASS_ORDERS = {
    "natural": NATURAL_CLASS_ORDER,
    "reverse_emission": REVERSE_EMISSION_CLASS_ORDER,
}

CPP_INLINE_MATCH_ORDER = [
    "vfunc5",
    "vfunc6",
    "func_80049DB4",
    "func_80049DAC",
    "func_80049DA0",
    "ctor",
    "dtor",
    "func_80049CC4",
    "vfunc2",
    "vfunc3",
    "func_80049B58",
    "func_80049B50",
    "vfunc4",
    "func_80049980",
    "vfunc7",
    "vfunc9",
    "vfunc8",
]

CPP_ORDERS = {
    "emission": EMISSION_ORDER,
    "matching_cpp_inline": CPP_INLINE_MATCH_ORDER,
}


@dataclass(frozen=True)
class Preset:
    placements: dict[str, str]
    class_order: str
    cpp_order: str
    pragma_mode: str = "none"


PRESETS = {
    # The layout the user found manually: keep vfunc5/vfunc6 out-of-line and
    # move the rest into the class definition.  The derived override declarations
    # are ordered so this compiler's reverse in-class emission order lands back
    # on the original text sequence.
    "matching_header_split": Preset(
        placements={
            spec.key: ("cpp_member" if spec.key in {"vfunc5", "vfunc6"} else "class_inline")
            for spec in FUNCTIONS
        },
        class_order="reverse_emission",
        cpp_order="emission",
    ),
    # Same placement split, but with the readable declaration order.  Useful as
    # a control case: it preserves the desired .rdata shape while changing the
    # emitted .text order.
    "natural_header_split": Preset(
        placements={
            spec.key: ("cpp_member" if spec.key in {"vfunc5", "vfunc6"} else "class_inline")
            for spec in FUNCTIONS
        },
        class_order="natural",
        cpp_order="emission",
    ),
    # Declaration-only header, two ordinary out-of-line definitions, then
    # source-file inline definitions in the order needed for this compiler's
    # reverse deferred emission to recover the original object order.
    "matching_cpp_inline_split": Preset(
        placements={
            spec.key: ("cpp_member" if spec.key in {"vfunc5", "vfunc6"} else "cpp_inline")
            for spec in FUNCTIONS
        },
        class_order="natural",
        cpp_order="matching_cpp_inline",
    ),
    # Plausible mixed split: the tiny accessors plus lifetime functions are
    # class-inline in the header, while the heavier inline bodies remain in the
    # source file.  The header declaration order is chosen so the compiler's
    # reverse deferred emission still yields ctor/accessors at the original tail.
    "matching_mixed_inline_split": Preset(
        placements={
            spec.key: (
                "cpp_member"
                if spec.key in {"vfunc5", "vfunc6"}
                else "class_inline"
                if spec.key in {"func_80049DB4", "func_80049DAC", "func_80049DA0", "ctor", "dtor"}
                else "cpp_inline"
            )
            for spec in FUNCTIONS
        },
        class_order="reverse_emission",
        cpp_order="matching_cpp_inline",
    ),
    # Current-style control: every decompiled body lives in the source file.
    "all_cpp": Preset(
        placements={spec.key: "cpp_member" for spec in FUNCTIONS},
        class_order="natural",
        cpp_order="emission",
    ),
}


def extract_body(source: str, signature: str) -> str:
    sig_start = source.index(signature)
    brace_start = source.index("{", sig_start)
    depth = 0
    for i in range(brace_start, len(source)):
        char = source[i]
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return source[brace_start : i + 1]
    raise ValueError(f"unterminated body for {signature}")


def render_header(
    placements: dict[str, str],
    bodies: dict[str, str],
    class_order: list[str],
    pragma_mode: str = "none",
) -> str:
    class_lines: list[str] = []
    for key in class_order:
        spec = FUNCTION_BY_KEY[key]
        placement = placements[key]
        if placement == "class_inline":
            class_lines.append(indent(f"{spec.class_decl} {bodies[key]}", "    "))
        else:
            class_lines.append(f"    {spec.class_decl};")

    header_inline_defs = [
        f"inline {FUNCTION_BY_KEY[key].cpp_sig} {bodies[key]}"
        for key in EMISSION_ORDER
        if placements[key] == "header_inline"
    ]

    chunks = [
        "#ifndef OTTER_H",
        "#define OTTER_H",
        "",
        '#include "streams.h"',
        "",
        'extern "C" {',
        "extern s32 func_8004767C(void*, void*);",
        "extern s32 func_80047A64(void*, s32);",
        "extern s32 func_80047A50(void*, s32);",
        "extern s32 func_8004798C(void*, void*, s32);",
        "extern s32 func_800477D4(void*, char*, u16, s32, char*, char*, void*);",
        "extern s32 func_80047490(void*, char*, s32*, s32*);",
        "extern s32 func_80047428(void*, char*, s32, void*);",
        "extern s32 func_800473C4(void*, char*, s32*);",
        "extern s32 func_8004775C(void*, char*, u16, s32, char*, char*);",
        "extern void func_800073E0(const void* src, void* dst, s32 n);",
        "extern void func_8000752C(const char*, char*, s32);",
        "extern unsigned strlen(const char*);",
        "extern s32 stricmp(const char*, const char*);",
        "}",
        "",
        "struct DirEntry {",
        "    /* 0x0 */ s32 size;",
        "    /* 0x4 */ s32 unk4;",
        "    /* 0x8 */ u16 unk8;",
        "    /* 0xA */ char ext[4];",
        "    /* 0xE */ char name[0x10];",
        "};",
        "",
        "struct Otter : public Sentry {",
        "    /* 0xC  */ Stream* stream;",
        "    /* 0x10 */ s32 unk10;",
        "    /* 0x14 */ char buf[0x68];",
        "",
        "\n\n".join(class_lines),
        "};",
    ]

    if header_inline_defs:
        chunks.extend(["", "\n\n".join(header_inline_defs)])

    chunks.extend(["", "#endif", ""])
    rendered = "\n".join(chunks)
    if pragma_mode == "interface_implementation":
        rendered = rendered.replace("#define OTTER_H\n\n", "#define OTTER_H\n\n#pragma interface\n\n", 1)
    return rendered


def render_cpp(
    placements: dict[str, str],
    bodies: dict[str, str],
    cpp_order: list[str] = EMISSION_ORDER,
    pragma_mode: str = "none",
) -> str:
    defs = [
        f"{'inline ' if placements[key] == 'cpp_inline' else ''}{FUNCTION_BY_KEY[key].cpp_sig} {bodies[key]}"
        for key in cpp_order
        if placements[key] in {"cpp_member", "cpp_inline"}
    ]
    include_lines = ['#include "otter.h"']
    if pragma_mode == "interface_implementation":
        include_lines.insert(0, '#pragma implementation "otter.h"')
    return "\n".join(
        [
            "// Generated by .claude/skills/n64-cpp-emission/references/otter-codegen/gen_otter.py.",
            "// Edit the adjacent source.cpp instead.",
            *include_lines,
            "",
            "\n\n".join(defs),
            "",
        ]
    )


def build_placements(
    preset: str,
    overrides: list[str],
) -> tuple[dict[str, str], list[str], list[str], str]:
    preset_config = PRESETS[preset]
    placements = dict(preset_config.placements)
    for override in overrides:
        if "=" not in override:
            raise ValueError(f"override must look like key=placement: {override}")
        key, placement = override.split("=", 1)
        if key not in FUNCTION_BY_KEY:
            raise ValueError(f"unknown function key: {key}")
        if placement not in VALID_PLACEMENTS:
            raise ValueError(f"unknown placement: {placement}")
        placements[key] = placement
    return (
        placements,
        CLASS_ORDERS[preset_config.class_order],
        CPP_ORDERS[preset_config.cpp_order],
        preset_config.pragma_mode,
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--preset",
        choices=sorted(PRESETS),
        default="matching_mixed_inline_split",
        help="starting layout to emit",
    )
    parser.add_argument(
        "--set",
        dest="overrides",
        action="append",
        default=[],
        metavar="KEY=PLACEMENT",
        help=f"override one function placement ({', '.join(sorted(VALID_PLACEMENTS))})",
    )
    parser.add_argument(
        "--class-order",
        choices=sorted(CLASS_ORDERS),
        help="override the class declaration order used by the selected preset",
    )
    parser.add_argument(
        "--cpp-order",
        choices=sorted(CPP_ORDERS),
        help="override the source-definition order used by the selected preset",
    )
    parser.add_argument(
        "--pragma-mode",
        choices=["none", "interface_implementation"],
        help="optionally add old-G++ interface/implementation pragmas",
    )
    parser.add_argument("--list", action="store_true", help="list function keys and exit")
    args = parser.parse_args()

    if args.list:
        for key in NATURAL_CLASS_ORDER:
            print(key)
        return 0

    source = SOURCE_OF_TRUTH.read_text()
    bodies = {spec.key: extract_body(source, spec.source_sig) for spec in FUNCTIONS}
    placements, class_order, cpp_order, pragma_mode = build_placements(args.preset, args.overrides)
    if args.class_order is not None:
        class_order = CLASS_ORDERS[args.class_order]
    if args.cpp_order is not None:
        cpp_order = CPP_ORDERS[args.cpp_order]
    if args.pragma_mode is not None:
        pragma_mode = args.pragma_mode

    HEADER_OUT.write_text(render_header(placements, bodies, class_order, pragma_mode))
    CPP_OUT.write_text(render_cpp(placements, bodies, cpp_order, pragma_mode))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
