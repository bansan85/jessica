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
  clang-format-13 -style=file "$i" -i
done

echo "prettier..."
npx prettier --write .

echo "eslint..."
npx eslint --ext .js,.js.in,.html,.html.in,.ts . --fix

exit 0
