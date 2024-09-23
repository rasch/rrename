#!/bin/sh

errors=0

test_renamed_dir() {
  mkdir -p "$1"

  ./rrename -r -i Makefile -i README
  
  if ! test -d "$2"; then
    errors=$((errors + 1))
    printf 'FAILED RENAME: %s -> %s\n' "$1" "$2"
  fi

  rm -rf test_dir
}

test_renamed_file() {
  mkdir -p "${1%/*}"
  touch "$1"

  ./rrename -r -i Makefile -i README
  
  if ! test -f "$2"; then
    errors=$((errors + 1))
    printf 'FAILED RENAME: %s -> %s\n' "$1" "$2"
  fi

  rm -rf test_dir
}

test_dry_run() {
  mkdir -p "${1%/*}"
  touch "$1"

  ./rrename -rd
  
  if test -f "$2" || ! test -f "$1"; then
    errors=$((errors + 1))
    printf 'FAILED DRYRUN: %s -> %s\n' "$1" "$2"
  fi

  rm -rf test_dir
}

test_dry_run \
  'test_dir/FiRsT/Resumé.md' \
  'test_dir/first/resumé.md'

test_renamed_dir \
  'test_dir/FiRsT' \
  'test_dir/first'

test_renamed_dir \
  'test_dir/FiRsT/2nd - In command.' \
  'test_dir/first/2nd_in_command'

test_renamed_dir \
  'test_dir/FiRsT/TOO Deep' \
  'test_dir/first/too_deep'

test_renamed_dir \
  'test_dir/SECond' \
  'test_dir/second'

test_renamed_dir \
  'test_dir/SECond/Going Deeper' \
  'test_dir/second/going_deeper'

test_renamed_dir \
  'test_dir/SECond/Going Deeper/Even Deeper' \
  'test_dir/second/going_deeper/even_deeper'

test_renamed_dir \
  'test_dir/SECond/Going Deeper/Even Deeper/Even More Deeper' \
  'test_dir/second/going_deeper/even_deeper/even_more_deeper'

test_renamed_file \
  'test_dir/FiRsT/2nd - In command./This & That.md' \
  'test_dir/first/2nd_in_command/this_and_that.md'

test_renamed_file \
  'test_dir/FiRsT/I'\''m WAITING....txt' \
  'test_dir/first/im_waiting.txt'

test_renamed_file \
  'test_dir/FiRsT/Resumé.md' \
  'test_dir/first/resumé.md'

test_renamed_file \
  'test_dir/FiRsT/TOO Deep/59%.md' \
  'test_dir/first/too_deep/59.md'

test_renamed_file \
  'test_dir/el niño.txt' \
  'test_dir/el_niño.txt'

test_renamed_file \
  'test_dir/two__under__scores.html' \
  'test_dir/two_under_scores.html'

exit $errors
