import hashlib


def get_passwd_part1(s: str):
    hsh = hashlib.sha256(s.encode('utf-8')).hexdigest()
    return hsh[:16]


def get_passwd_part2(uname: str):
    b = [1, 1, 4, 5, 1, 4, 1, 9, 1, 9, 8, 10]
    res = ''.join([chr(b[i % len(b)] ^ ord(c)) for i, c in enumerate(uname)])
    return res


def main():
    unames = [
        'hans7',
        'hans7' * 3,
        'abc321' * 3,
    ]
    for uname in unames:
        passwd = get_passwd_part1(uname) + '-' + get_passwd_part2(uname)
        print(uname, passwd)


if __name__ == '__main__':
    main()
