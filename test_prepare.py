import pytest
from prepare import is_legal_uname, prepare_const_bytes


# TODO: 单测文件放 tests 文件夹下就 import 不了 pyd 了
# TODO: 给 prepare_const_bytes 传一个需要参数的函数，直接 Windows fatal exception: access violation 了
def test_prepare_const_bytes():
    assert prepare_const_bytes(lambda: 177) == bytes([1, 1, 4, 5, 1, 4, 1, 9, 1, 9, 8, 10])


def test_is_legal_uname():
    cases = [
        'hans7', 'I\'m on my way to school', ' ', '\t', '\n'
    ]
    ans = [True, True, True, False, False]
    res = [is_legal_uname(cas) for cas in cases]
    assert res == ans
    with pytest.raises(ValueError) as e_info:
        is_legal_uname('a' * 51)
    assert 'len(uname) should be less than or equal to 50' in str(e_info.value)
