from pathlib import Path
from typing import List

from splat.segtypes.common.rodata import CommonSegRodata
from splat.segtypes.linker_entry import LinkerEntry, LinkerWriter


class LinkerEntrySymbol(LinkerEntry):
    """A zero-size linker entry that just emits a symbol at the current address."""

    def __init__(self, segment, sym_name: str):
        super().__init__(segment, [], Path("_sym"), "linker_symbol", "linker_symbol", False)
        self.object_path = None
        self.sym_name = sym_name

    def emit_entry(self, linker_writer: LinkerWriter):
        linker_writer._write_symbol(self.sym_name, ".")


class N64SegDtors(CommonSegRodata):
    def get_linker_section(self) -> str:
        return ".dtors"

    def get_linker_entries(self) -> List[LinkerEntry]:
        entries = super().get_linker_entries()

        dtors_siblings = [
            s
            for s in self.parent.subsegments
            if type(s).__name__ == "N64SegDtors"
        ]

        result = []
        if dtors_siblings and dtors_siblings[0] is self:
            result.append(LinkerEntrySymbol(self, "entry_DTORS_START"))
        result.extend(entries)
        if dtors_siblings and dtors_siblings[-1] is self:
            result.append(LinkerEntrySymbol(self, "entry_DTORS_END"))

        return result
