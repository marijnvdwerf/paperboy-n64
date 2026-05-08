#!/usr/bin/env python3
import argparse
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

INCLUDES = "-Iinclude"
AS_FLAGS = f"-EB -march=vr4300 -mtune=vr4300 -G0 {INCLUDES}"

COMMON_DEFINES = (
    "-nostdinc -undef -D__GNUC__=2 -D__OPTIMIZE__"
    " -Dmips -D__mips__ -D__mips -Dn64 -D__n64__ -D__n64"
    " -D_PSYQ -D__EXTENSIONS__ -D_MIPSEB -D__CHAR_UNSIGNED__"
    " -DINCLUDE_ASM_USE_MACRO_INC=1"
)

C_CPP_FLAGS = f"{COMMON_DEFINES} -lang-c -D_LANGUAGE_C {INCLUDES}"
CXX_CPP_FLAGS = f"{COMMON_DEFINES} -lang-c++ -D_LANGUAGE_C_PLUS_PLUS {INCLUDES}"

CC_FLAGS = "-quiet -G0 -O2"

VARIANTS = {
    "ntsc": {"defines": "", "verify": True},
    "pal": {"defines": "-DPAL=1", "verify": False},
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
        command=f"cd $build_dir && {CROSS_LD} -T ../../undefined_funcs_auto.txt -T ../../undefined_syms_auto.txt -Map {BASENAME}.map -T ../../{LD_PATH} -o {BASENAME}.elf",
    )

    ninja.rule(
        "z64",
        description="rom $out",
        command=f"{CROSS_OBJCOPY} $in $out -O binary",
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
            variables={"build_dir": build_dir},
        )

        ninja.build(z64_path, "z64", elf_path)

        if variant["verify"]:
            ninja.build(ok_path, "sha1sum", "checksum.sha1", implicit=[z64_path])
            ninja.default(ok_path)
        else:
            ninja.default(z64_path)


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

    print("Build configured. Run 'ninja' to build.")
