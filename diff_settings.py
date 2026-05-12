#!/usr/bin/env python3


def add_custom_arguments(parser):
    parser.add_argument("--pal", action="store_true", help="Diff against the PAL build")


_use_make = False


def apply(config, args):
    global _use_make
    _use_make = args.make

    variant = "pal" if args.pal else "ntsc"

    config["baseimg"] = "baserom.z64"
    config["myimg"] = f"build/{variant}/paperboy.z64"
    config["mapfile"] = f"build/{variant}/paperboy.map"
    config["source_directories"] = ["src", "include"]
    config["make_command"] = ["ninja"]
    config["objdump_flags"] = ["-M", "reg-names=32"]
    config["build_dir"] = f"build/{variant}/"
    config["expected_dir"] = "expected/"

    import diff as _diff
    _orig_search = _diff.search_map_file
    _build_dir = f"build/{variant}"
    def _patched_search(fn_name, project, config, *, for_binary):
        import os
        result = _orig_search(fn_name, project, config, for_binary=for_binary)
        if result[0] and not os.path.isfile(result[0]):
            patched = os.path.join(_build_dir, result[0])
            if os.path.isfile(patched):
                return patched, result[1]
        return result
    _diff.search_map_file = _patched_search


def map_build_target(make_target):
    if _use_make:
        return make_target

    import os
    return os.path.abspath(make_target)
