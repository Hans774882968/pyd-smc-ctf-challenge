from typing import Dict, List, Tuple
from os.path import join, splitext, basename
import pefile


class DllFn():
    def __init__(self, name: str, enc_key: List[int], st: int, sz: int) -> None:
        self.name = name
        self.enc_key = enc_key
        self.st = st
        self.sz = sz


class DllFile():
    def __init__(self, fpath: str, func_infos: Dict[str, Tuple[List[int], int]]) -> None:
        self.out_fname = splitext(basename(fpath))[0]
        self.pe_obj = pefile.PE(fpath)
        functions = []
        for symb in self.pe_obj.DIRECTORY_ENTRY_EXPORT.symbols:
            fn_name = symb.name.decode('utf-8')
            if fn_name in func_infos:
                enc_key, sz = func_infos[fn_name]
                fn = DllFn(fn_name, enc_key, symb.address, sz)
                functions.append(fn)
        self.functions: List[DllFn] = functions

    def process(self):
        for fn in self.functions:
            self.enc(fn)
        res = self.pe_obj.write()
        with open(self.out_fname, 'wb') as res_f:
            res_f.write(res)

    def enc(self, fn: DllFn):
        st = fn.st
        sz = fn.sz
        enc_key = fn.enc_key
        dat = self.pe_obj.get_data(st, sz + 1)
        res = bytes([v ^ enc_key[i % len(enc_key)] for i, v in enumerate(dat)])
        self.pe_obj.set_bytes_at_rva(st, res)


def get_dll_path(name: str):
    return join('build', name)


# 本项目所有 smc 都是左闭右闭
# 如果没有要进行加密的函数，那么此代码做的事可以理解为单纯复制文件 + 重命名
decider_enc_key = [67, 91, 48, 88, 80, 75, 41, 93, 116, 92, 33, 83, 68, 69]
decider = DllFile(get_dll_path('decider.pyd.dll'), {
    'jdg_part1': (decider_enc_key, 0x321E414B9 - 0x321E41370),
    'jdg_part2': (decider_enc_key, 0x321E4158D - 0x321E414BA),
    'jdg': (decider_enc_key, 0x321E4184B - 0x321E4158E),
})
prepare_enc_key = [55, 228, 83, 146, 49, 220, 225, 87, 135, 171, 147, 50, 87, 250]
prepare = DllFile(get_dll_path('prepare.pyd.dll'), {
    'prepare_const_bytes': (prepare_enc_key, 0x38BB11541 - 0x38BB11370),
    'is_legal_uname': (prepare_enc_key, 0x38BB116A6 - 0x38BB11542),
})


def main():
    print(decider.out_fname)
    decider.process()
    print(prepare.out_fname)
    prepare.process()


if __name__ == '__main__':
    main()
