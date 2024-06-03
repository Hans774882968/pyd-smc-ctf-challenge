from decider import jdg


def test_jdg():
    CONST_BYTES = bytes([1, 1, 4, 5, 1, 4, 1, 9, 1, 9, 8, 10])
    params = [
        ('hans7', 'a' * 16 + '-i`jv6', lambda _: 'a' * 16, CONST_BYTES),
        ('hans7', 'a' * 16 + '-i`jv7', lambda _: 'a' * 16, CONST_BYTES),
    ]
    res = [jdg(*p) for p in params]
    ans = [True, False]
    assert res == ans
