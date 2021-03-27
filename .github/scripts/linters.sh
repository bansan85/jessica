#!/bin/bash
# Need bash for shopt

shopt -s globstar dotglob

retval=0

for i in **/*.yml
do
  echo "yamllint $i"
  yamllint "$i" || retval=1
done

echo shellcheck -- **/*.sh
shellcheck -- **/*.sh || retval=1

safety check -r requirements-linter.txt || retval=1

exit $retval
