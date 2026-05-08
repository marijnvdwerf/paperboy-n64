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
MAP_PATH = f"build/{BASENAME}.map"
ELF_PATH = f"build/{BASENAME}.elf"
Z64_PATH = f"build/{BASENAME}.z64"
OK_PATH = f"build/{BASENAME}.ok"

CROSS = "mips-linux-gnu-"
CROSS_AS = f"{CROSS}as"
CROSS_CPP = f"{CROSS}cpp"
CROSS_LD = f"{CROSS}ld"
CROSS_OBJCOPY = f"{CROSS}objcopy"

# SN toolchain
CC1N64 = "tools/bin/cc1n64.exe"
MODERN_ASN64 = "tools/modern-asn64/modern-asn64.py"

INCLUDES = "-Iinclude"
AS_FLAGS = f"-EB -march=vr4300 -mtune=vr4300 -G0 {INCLUDES}"

CPP_FLAGS = (
    f"-nostdinc -undef -D__GNUC__=2 -D__OPTIMIZE__ -lang-c"
    f" -Dmips -D__mips__ -D__mips -Dn64 -D__n64__ -D__n64"
    f" -D_PSYQ -D__EXTENSIONS__ -D_MIPSEB -D__CHAR_UNSIGNED__"
    f" -D_LANGUAGE_C -DINCLUDE_ASM_USE_MACRO_INC=1 {INCLUDES}"
)

CC_FLAGS = "-quiet -G0 -O2"


def clean():
    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("build", ignore_errors=True)
    if os.path.exists(".splache"):
        os.remove(".splache")


def create_build_script(linker_entries: list[LinkerEntry]):
    os.makedirs("build", exist_ok=True)

    ninja = ninja_syntax.Writer(open("build.ninja", "w"), width=9999)

    ninja.rule(
        "as",
        description="as $in",
        command=f"cpp {INCLUDES} $in | {CROSS_AS} {AS_FLAGS} -o $out",
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=(
            f"{CROSS_CPP} {CPP_FLAGS} -MD -MF $out.d -MT $out -o $out.i $in"
            f" && wibo {CC1N64} {CC_FLAGS} $out.i -o $out.s"
            f" && python3 {MODERN_ASN64} {CROSS_AS} $out.s {AS_FLAGS} -I. -o $out"
        ),
        depfile="$out.d",
    )

    ninja.rule(
        "bin",
        description="bin $in",
        command=f"{CROSS_LD} -r -b binary $in -o $out",
    )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{CROSS_LD} -T undefined_funcs_auto.txt -T undefined_syms_auto.txt -Map $mapfile -T $in -o $out",
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

    built_objects = []

    for entry in linker_entries:
        if entry.object_path is None:
            continue

        seg = entry.segment
        if seg.type[0] == ".":
            continue

        src = str(entry.src_paths[0])
        obj = str(entry.object_path)

        if src.endswith(".s"):
            ninja.build(obj, "as", src)
            built_objects.append(obj)
        elif src.endswith(".c"):
            ninja.build(obj, "cc", src)
            built_objects.append(obj)
        elif src.endswith(".bin"):
            ninja.build(obj, "bin", src)
            built_objects.append(obj)

    ninja.build(
        ELF_PATH,
        "ld",
        LD_PATH,
        implicit=built_objects,
        variables={"mapfile": MAP_PATH},
    )

    ninja.build(Z64_PATH, "z64", ELF_PATH)
    ninja.build(OK_PATH, "sha1sum", "checksum.sha1", implicit=[Z64_PATH])
    ninja.default(OK_PATH)


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
