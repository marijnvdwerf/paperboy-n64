from splat.segtypes.common.rodata import CommonSegRodata


class N64SegDtors(CommonSegRodata):
    def get_linker_section(self) -> str:
        return ".dtors"
