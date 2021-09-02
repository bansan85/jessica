from os import listdir
from os.path import isfile, join
from typing import List


def find_translation(path: str, prefix: str, postfix: str) -> List[str]:
    return [
        f[len(prefix) : -len(postfix)]
        for f in listdir(path)
        if f.startswith(prefix)
        and f.endswith(postfix)
        and isfile(join(path, f))
    ]
