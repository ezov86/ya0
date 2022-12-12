import os
import re
from subprocess import Popen, PIPE

HOME_DIR = '.'
TEST_PATH = 'tests/'
BIN_PATH = 'build/ya'


def load_sample_data(path: str) -> list:
    with open(path) as f:
        text = f.read()

    data = re.split(r'\n%%%%%%%%%%\n', text)

    # Преобразование экранированных символов в строках, которые начинаются с $.
    expected_res = []
    for line in data[1].split('\n'):
        if line.startswith('$'):
            expected_res.append(line[1:].encode('utf-8').decode('unicode_escape'))
        else:
            expected_res.append(line)

    data[1] = '\n'.join(expected_res)
    return data


def sample(path: str, stage: str):
    data = load_sample_data(os.path.join(HOME_DIR, TEST_PATH, stage, path))
    process = Popen([os.path.join(HOME_DIR, BIN_PATH), path, '--test-' + stage], stdout=PIPE)
    stdout, stderr = process.communicate()
    exit_code = process.wait()

    assert stderr == None
    assert exit_code == 0
    assert data[1] + data[2] == stdout


def test_lexer():
    sample('valid0.ya', 'lexer')


if __name__ == "__main__":
    test_lexer()
