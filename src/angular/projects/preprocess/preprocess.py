import fileinput
import json
import os
from pathlib import Path
import re
from typing import List

import translation


__p: str = os.path.dirname(os.path.realpath(__file__))


def find_translations() -> None:
    Path(__p + "/../app-main/src/app/util/translate").mkdir(
        parents=True, exist_ok=True
    )
    all_translations: List[str] = sorted(
        set(
            translation.find_translation(
                __p + "/../jessica/src/assets/i18n",
                "jessica-",
                ".json",
            )
            + translation.find_translation(
                __p + "/../app-main/src/assets/i18n", "", ".json"
            )
        )
    )
    with open(
        __p + "/../app-main/src/app/util/translate/all-lang.json",
        "w",
        encoding="utf8",
    ) as file:
        json.dump(all_translations, file)
    replace_translations = "(" + "|".join(all_translations) + ")"
    regex = re.compile("(.*webpackInclude.*)(\\(.*\\))(.*)$", re.IGNORECASE)
    for line in fileinput.input(
        __p + "/../app-main/src/app/app.component.ts", inplace=True
    ):
        if regex.match(line):
            line = re.sub(regex, "\\1" + replace_translations + "\\3", line)

        print(line, end="")


find_translations()
