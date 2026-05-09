# /// script
# requires-python = ">=3.11"
# dependencies = ["mapfile_parser"]
# ///
import sys
import mapfile_parser

pal = mapfile_parser.MapFile()
pal.readMapFile("build/pal/paperboy.map")

us = mapfile_parser.MapFile()
us.readMapFile("build/ntsc/paperboy.map")

for arg in sys.argv[1:]:
    addr = int(arg, 16) if arg.startswith("0x") or arg.startswith("0X") else int(arg, 16)

    result = pal.findSymbolByVrom(addr)
    if not result:
        print(f"PAL 0x{addr:X}: not found in PAL map")
        continue

    pal_info = result[0]
    sym_name = pal_info.symbol.name
    offset = pal_info.offset

    us_info = us.findSymbolByName(sym_name)
    if not us_info:
        print(f"PAL 0x{addr:X}: {sym_name}+0x{offset:X} not found in US map")
        continue

    us_vrom = us_info.symbol.vrom + offset
    print(f"PAL 0x{addr:X} -> {sym_name}+0x{offset:X} -> US 0x{us_vrom:X}")
