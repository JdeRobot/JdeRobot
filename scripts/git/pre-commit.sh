#!/bin/bash
# Installation:
#   cd $JdeRobot$
#   ln -s ../../scripts/git/pre-commit.sh .git/hooks/pre-commit

# pre-commit.sh
echo "Running pre-commit lint test...(skip with --no-verify)"

SOURCE="${BASH_SOURCE[0]}"
RDIR="$( dirname "$SOURCE" )"

#git stash -q --keep-index

# Lint prospective commit
FILES=`git diff --cached --name-only --diff-filter=ACMR | grep -E "\.(c|cpp|h)$"`

for FILE in $FILES; do
    echo "===================="
    echo "Run Cpplint for $FILE..." 
    python $RDIR/../../scripts/python/cpplint.py $FILE
done

#git stash pop -q

# Allows us to read user input below, assigns stdin to keyboard, use 'exec <&-' to close it
exec < /dev/tty

echo "===================="
echo "Please make sure your coding conventions are conhenrent with JdeRobot Programming Sytle!" 
echo "Juding from the Lint messages, commit or not? (y/n):" 
read response
if [ "$response" == "y" ]; then
    exit 0
else
    exit 1
fi

