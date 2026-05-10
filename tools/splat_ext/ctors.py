from splat.segtypes.common.rodata import CommonSegRodata


class N64SegCtors(CommonSegRodata):
    def get_linker_section(self) -> str:
        return ".ctors"
