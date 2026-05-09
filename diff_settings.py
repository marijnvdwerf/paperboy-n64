#!/usr/bin/env python3


def apply(config, args):
    config["baseimg"] = "baserom.z64"
    config["myimg"] = "build/ntsc/paperboy.z64"
    config["mapfile"] = "build/ntsc/paperboy.map"
    config["source_directories"] = ["src", "include"]
    config["make_command"] = []
    config["objdump_flags"] = ["-M", "reg-names=32"]
    config["build_dir"] = "build/ntsc/"
    config["expected_dir"] = "expected/"

    # asm-differ resolves objfiles from the map relative to cwd.
    # Since our objects are in build/ntsc/, we patch search_map_file
    # to prepend the build dir.
    import diff as _diff
    _orig_search = _diff.search_map_file
    def _patched_search(fn_name, project, config, *, for_binary):
        import os
        result = _orig_search(fn_name, project, config, for_binary=for_binary)
        if result[0] and not os.path.isfile(result[0]):
            patched = os.path.join("build/ntsc", result[0])
            if os.path.isfile(patched):
                return patched, result[1]
        return result
    _diff.search_map_file = _patched_search
