import json
import os
from pathlib import Path
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


find_translations()
