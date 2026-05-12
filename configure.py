#!/usr/bin/env python3
import argparse
import json
import os
import shutil
import sys
from pathlib import Path

import ninja_syntax
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry

BASENAME = "paperboy"
YAML_FILE = Path(f"{BASENAME}.yaml")

LD_PATH = f"{BASENAME}.ld"

CROSS = "mips-linux-gnu-"
CROSS_AS = f"{CROSS}as"
CROSS_CPP = f"{CROSS}cpp"
CROSS_LD = f"{CROSS}ld"
CROSS_OBJCOPY = f"{CROSS}objcopy"

# SN toolchain
CC1N64 = "tools/bin/cc1n64.exe"
CC1PLN64 = "tools/bin/cc1pln64.exe"
MODERN_ASN64 = "tools/modern-asn64/modern-asn64.py"

INCLUDES = "-Iinclude -Iultralib/include -Iultralib/include/PR"
AS_FLAGS = f"-EB -march=vr4300 -mtune=vr4300 -G0 {INCLUDES}"

COMMON_DEFINES = (
    "-nostdinc -undef -D__GNUC__=2 -D__OPTIMIZE__"
    " -Dmips -D__mips__ -D__mips -Dn64 -D__n64__ -D__n64"
    " -D_PSYQ -D__EXTENSIONS__ -D_MIPSEB -D__CHAR_UNSIGNED__"
    " -DINCLUDE_ASM_USE_MACRO_INC=1"
    " -D_FINALROM -DNDEBUG -DBUILD_VERSION=VERSION_J"
)

C_CPP_FLAGS = f"{COMMON_DEFINES} -lang-c -D_LANGUAGE_C {INCLUDES}"
CXX_CPP_FLAGS = f"{COMMON_DEFINES} -lang-c++ -D_LANGUAGE_C_PLUS_PLUS {INCLUDES}"

CC_FLAGS = "-quiet -G0 -O2"

VARIANTS = {
    "ntsc": {"defines": "", "ld_flags": "", "verify": True},
    "pal": {"defines": "-DPAL=1", "ld_flags": "-T ../../undefined_syms_pal.txt", "verify": False},
}

# Post-build ROM patches per variant: list of (rom_offset, hex_bytes).
PATCHES = {
    "ntsc": [
        (0x14A8, "6573203D20363430"),
        (0x1768, "657265206F70656E"),
        (0x359C, "800400E4"),
        (0x4B88, "00000000800263A8"),
        (0x5C18, "0000000080048B74"),
        (0x7658, "4105221B3D3FB61B"),
        (0x1743C, "94820028"),
        (0x177A8, "8C82000803E00008"),
        (0x1787C, "8FB00010"),
        (0x2A924, "0222102B1040002600008021"),
        (0x2CD44, "3C018000C42627B846040003"),
        (0x3EE48, "844400088C42000C"),
        (0x3D634, "AE00000C8FBF00148FB00010"),
    ],
    "pal": [],
}


def clean():
    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("build", ignore_errors=True)
    if os.path.exists(".splache"):
        os.remove(".splache")


def create_build_script(linker_entries: list[LinkerEntry]):
    os.makedirs("build", exist_ok=True)

    ninja = ninja_syntax.Writer(open("build.ninja", "w"), width=9999)

    # Shared rules (asm/bin don't vary between variants)
    ninja.rule(
        "as",
        description="as $in",
        command=f"cpp {INCLUDES} $in | {CROSS_AS} {AS_FLAGS} -o $out",
    )

    ninja.rule(
        "bin",
        description="bin $in",
        command=f"{CROSS_LD} -r -b binary $in -o $out",
    )

    # C/C++ rules take $extra_defines for variant-specific flags
    ninja.rule(
        "cc",
        description="cc $in",
        command=(
            f"{CROSS_CPP} {C_CPP_FLAGS} $extra_defines -MD -MF $out.d -MT $out -o $out.i $in"
            f" && wibo {CC1N64} {CC_FLAGS} $out.i -o $out.s"
            f" && python3 {MODERN_ASN64} {CROSS_AS} $out.s {AS_FLAGS} -I. -o $out"
        ),
        depfile="$out.d",
    )

    ninja.rule(
        "cxx",
        description="cxx $in",
        command=(
            f"{CROSS_CPP} {CXX_CPP_FLAGS} $extra_defines -MD -MF $out.d -MT $out -o $out.i $in"
            f" && wibo {CC1PLN64} {CC_FLAGS} $out.i -o $out.s"
            f" && python3 {MODERN_ASN64} {CROSS_AS} $out.s {AS_FLAGS} -I. -o $out"
        ),
        depfile="$out.d",
    )

    # Link from inside the build dir so the linker script's relative paths resolve
    ninja.rule(
        "ld",
        description="link $out",
        command=f"cd $build_dir && {CROSS_LD} $extra_ld_flags -T ../../undefined_syms.txt -T ../../undefined_funcs_auto.txt -T ../../undefined_syms_auto.txt -Map {BASENAME}.map -T ../../{LD_PATH} -o {BASENAME}.elf",
    )

    ninja.rule(
        "z64",
        description="rom $out",
        command=f"{CROSS_OBJCOPY} $in $out -O binary && python3 tools/patch_rom.py $out $patches",
    )

    ninja.rule(
        "sha1sum",
        description="sha1sum $in",
        command="sha1sum -c $in && touch $out",
    )

    for name, variant in VARIANTS.items():
        build_dir = f"build/{name}"
        elf_path = f"{build_dir}/{BASENAME}.elf"
        z64_path = f"{build_dir}/{BASENAME}.z64"
        ok_path = f"{build_dir}/{BASENAME}.ok"

        built_objects = []

        for entry in linker_entries:
            if entry.object_path is None:
                continue
            seg = entry.segment
            if seg.type[0] == ".":
                continue

            src = str(entry.src_paths[0])
            obj_rel = str(entry.object_path)
            obj = f"{build_dir}/{obj_rel}"

            if src.endswith(".s"):
                ninja.build(obj, "as", src)
            elif src.endswith(".c"):
                ninja.build(obj, "cc", src, variables={"extra_defines": variant["defines"]})
            elif src.endswith(".cpp"):
                ninja.build(obj, "cxx", src, variables={"extra_defines": variant["defines"]})
            elif src.endswith(".bin"):
                ninja.build(obj, "bin", src)

            built_objects.append(obj)

        ninja.build(
            elf_path, "ld", implicit=built_objects + [LD_PATH, "undefined_funcs_auto.txt", "undefined_syms_auto.txt"],
            variables={"build_dir": build_dir, "extra_ld_flags": variant["ld_flags"]},
        )

        patches = PATCHES.get(name, [])
        patch_args = " ".join(f"{off:#x}:{hx}" for off, hx in patches)
        ninja.build(z64_path, "z64", elf_path, variables={"patches": patch_args})

        if variant["verify"]:
            ninja.build(ok_path, "sha1sum", "checksum.sha1", implicit=[z64_path])
            ninja.default(ok_path)
        else:
            ninja.default(z64_path)


def create_compile_commands(linker_entries: list[LinkerEntry]):
    cwd = os.getcwd()
    entries = []
    for entry in linker_entries:
        if entry.object_path is None:
            continue
        seg = entry.segment
        if seg.type[0] == ".":
            continue

        src = str(entry.src_paths[0])
        if src.endswith(".c"):
            flags = C_CPP_FLAGS
        elif src.endswith(".cpp"):
            flags = CXX_CPP_FLAGS
        else:
            continue

        entries.append({
            "directory": cwd,
            "command": f"{CROSS_CPP} {flags} {CC_FLAGS} -c {src}",
            "file": src,
        })

    with open("compile_commands.json", "w") as f:
        json.dump(entries, f, indent=2)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--clean", action="store_true")
    args = parser.parse_args()

    if args.clean:
        clean()

    if not Path("baserom.z64").exists():
        print("baserom.z64 not found!")
        sys.exit(1)

    split.main([YAML_FILE], modes=["all"], verbose=False)
    create_build_script(split.linker_writer.entries)
    create_compile_commands(split.linker_writer.entries)

    print("Build configured. Run 'ninja' to build.")
