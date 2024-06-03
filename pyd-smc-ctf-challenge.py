import hashlib
from prepare import prepare_const_bytes
from decider import jdg
from prepare import is_legal_uname
from resources import read_lyric_txt


def get_hash(s: str):
    return hashlib.sha256(s.encode('utf-8')).hexdigest()


def input_to_quit(s: str):
    return s == '' or s.isspace() or s == 'quit' or s == 'q'


def main():
    CONST_BYTES = prepare_const_bytes(read_lyric_txt)
    print('Welcome to pyd-smc-ctf-challenge! u can enter "quit" or "q" to exit this app at any time.')
    while True:
        uname = input('Plz input user name: ')
        if input_to_quit(uname):
            return
        if not is_legal_uname(uname):
            print(f'Illegal user name "{uname}"')
            continue
        passwd = input('Plz input password: ')
        if input_to_quit(passwd):
            return
        if not passwd.isprintable():
            print(f'Illegal password "{passwd}"')
            continue
        if jdg(uname, passwd, get_hash, CONST_BYTES):
            print(f'Congratulations! Your flag is flag{{{passwd}}}')
            break
        else:
            print('Try again!')
    input('Press any key to continue...')


if __name__ == '__main__':
    main()
