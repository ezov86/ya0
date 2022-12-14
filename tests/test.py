# python -m pytest tests/test.py
import os
import re
from subprocess import Popen, PIPE

HOME_DIR = '.'
TEST_PATH = 'tests/'
BIN_PATH = 'build/ya'


""" Dirty unescape of C-like extended ASCII string. decode('unicode_escape') or eval() is not used because of unicode conversion. """
def esc(s: bytes) -> bytes:
    res = bytes()
    i = 0

    def read_next() -> bytes:
        return s[i].to_bytes(1, byteorder='big')

    while i < len(s):
        c = read_next()
        i += 1
        if c == b'\\':
            c = read_next()
            i += 1
            escapes = {
                b'0': b'\0',
                b'v': b'\v',
                b't': b'\t',
                b'a': b'\a',
                b'b': b'\b',
                b'f': b'\f',
                b'n': b'\n',
                b'r': b'\r',
                b'\'': b'\'',
                b'\\': b'\\',
                b'\"': b'\"'
            }

            if c in escapes:
                res += escapes[c]
                continue

            if c == b'x':
                num = read_next()
                i += 1
                num += read_next()
                i += 1
                res += int(num, 16).to_bytes(1, byteorder='big')
        else:
            res += c

    return res


def load_sample_data(path: str) -> list:
    with open(path, 'rb') as f:
        text = f.read()

    data = text.split(b'\n%%%%%%%%%%\n')

    # Escaping $-strings.
    expected_res = []
    for line in data[1].split(b'\n'):
        if not line.startswith(b'$'):
            expected_res.append(line)
            continue

        expected_res.append(esc(line[1:]))

    data[1] = b'\n'.join(expected_res)
    return data


def sample(path: str, stage: str):
    data = load_sample_data(os.path.join(HOME_DIR, TEST_PATH, stage, path))
    path = os.path.join(HOME_DIR, TEST_PATH, stage, path)
    process = Popen([os.path.join(HOME_DIR, BIN_PATH), '--test-' + stage], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate(input=data[0])
    exit_code = process.wait()

    assert stderr == b''
    assert exit_code == 0

    # If encoding is possible, compare as string. If assertion fails, readable diff will be provided.
    try:
        assert data[1].decode('utf-8') == stdout.decode('utf-8')
    # Else assert as bytes.
    except UnicodeDecodeError:
        assert data[1] == stdout


def test_lexer():
    sample('valid0.ya', 'lex')
    sample('valid1.ya', 'lex')


if __name__ == "__main__":
    test_lexer()
