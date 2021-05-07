#!/bin/bash
# Need bash for shopt

shopt -s globstar dotglob

retval=0

npx eslint --ext .js,.js.in,.html,.html.in . || retval=1

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

cd build || exit 1
iwyu_tool -p . -- -Xiwyu --mapping_file="$(pwd)/../.iwyu-suppressions" -Xiwyu --no_fwd_decls > iwyu_tool.log
if [ "$(grep -c -e "should add these lines" -e "should remove these lines" < iwyu_tool.log)" -ne "0" ]
then
  cat iwyu_tool.log
  retval=1
  echo "Failure iwyu_tool"
fi
cppcheck --enable=all --project=compile_commands.json --error-exitcode=1 --inline-suppr --template="{file},{line},{severity},{id},{message}" --suppressions-list=../.cppcheck-suppressions || { retval=1 && echo "Failure cppcheck"; }
cd .. || exit 1

exit $retval
