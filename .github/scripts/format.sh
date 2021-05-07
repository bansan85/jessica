#!/bin/bash
# Need bash for shopt

shopt -s globstar dotglob

for i in **/CMakeLists.txt
do
  echo "cmake-format $i"
  cmake-format -i "$i"
done

for i in {**/*.cc,**/*.h}
do
  echo "clang-format $i... "
  clang-format-11 -style=file "$i" -i
done

echo "ESLint... "
npx eslint --ext .js,.js.in,.html,.html.in . --fix

npx prettier --write .

exit 0
