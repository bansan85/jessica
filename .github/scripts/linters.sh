#!/bin/bash
# Need bash for shopt

shopt -s globstar dotglob

retval=0

npx eslint --ext .js,.js.in,.html,.html.in,.ts . || retval=1

for i in **/*.yml
do
  if [[ $i == *"node_modules"* ]]; then
    continue
  fi
  echo "yamllint $i"
  yamllint "$i" || retval=1
done

echo shellcheck -- **/*.sh
shellcheck -- **/*.sh || retval=1

safety check -r requirements-linter.txt || retval=1

var=$(find . -name "*.py" ! -path "./vcpkg/*" ! -path "./src/angular/node_modules/*" ! -path "./venv/*")
echo "Python files: ${var}"
# shellcheck disable=SC2086
pylint $var || { retval=1 && echo "Failure pylint"; }
# shellcheck disable=SC2086
flake8 $var || { retval=1 && echo "Failure flake8"; }
# shellcheck disable=SC2086
prospector $var || { retval=1 && echo "Failure prospector"; }
# shellcheck disable=SC2086
mypy $var || { retval=1 && echo "Failure mypy"; }

mkdir build
cmake -S . -B build
cd build || exit 1
sed -i -e 's#-I\(\S*build\S*_deps\S*\)#-isystem \1#g' compile_commands.json
sed -i "1N;1N;$!N;N;s/^{\n[^\n]*directory[^\n]*_deps[^\n]*\n[^\n]*command[^\n]*\n[^\n]*file[^\n]*\n},//;P;D" compile_commands.json
sed -i "1N;1N;$!N;N;s/^{\n[^\n]*directory[^\n]*_deps[^\n]*\n[^\n]*command[^\n]*\n[^\n]*file[^\n]*\n},//;P;D" compile_commands.json
run-clang-tidy-13 '^((?!_deps).)*$' || { retval=1 && echo "Failure clang-tidy"; }

echo "Start Iwyu"
iwyu_tool -p . -- -Xiwyu --mapping_file="$(pwd)/../.iwyu-suppressions" -Xiwyu --no_fwd_decls > iwyu_tool.log
echo "End Iwyu"
cat iwyu_tool.log
if [ "$(grep -c -e "should add these lines" -e "should remove these lines" < iwyu_tool.log)" -ne "0" ]
then
  retval=1
  echo "Failure iwyu_tool"
fi
cppcheck --enable=all --project=compile_commands.json --error-exitcode=1 --inline-suppr --template="{file},{line},{severity},{id},{message}" --suppressions-list=../.cppcheck-suppressions --suppress=unusedFunction:*/decorator.cc --suppress=missingIncludeSystem --suppress=unmatchedSuppression || { retval=1 && echo "Failure cppcheck"; }
cd .. || exit 1

exit $retval
