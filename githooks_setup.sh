#!/bin/bash

# Define colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Setting up pre-commit hook...${NC}"
touch $PWD/.git/hooks/pre-commit

# Script content for the pre-commit hook
cat << 'EOF' > "$PWD/.git/hooks/pre-commit"
#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Info's:
echo -e "${YELLOW}Running git hooks...${NC}"

# echo -e "${YELLOW}+ Check if the local branch is behind its upstream ${NC}"
# git fetch origin  # Fetch updates from the remote repository
# LOCAL=$(git rev-parse @)
# REMOTE=$(git rev-parse @{u})
# BASE=$(git merge-base @ @{u})
#
# if [ $LOCAL = $REMOTE ]; then
#     echo -e "${GREEN}✓ Local branch is up to date with its upstream.${NC}"
# elif [ $LOCAL = $BASE ]; then
#     echo -e "${RED}x Error: Local branch is behind its upstream. Please pull changes before committing.${NC}"
#     exit 1
# fi

echo -e "${YELLOW}+ Checking clang-format path${NC}"
CLANG_FORMAT_PATH=$(command -v clang-format 2> /dev/null)
if [ -z "$CLANG_FORMAT_PATH" ]; then
    echo -e "${RED}Error: clang-format not found in PATH${NC}"
    echo -e "${RED}Please consider installing it and add it to the PATH${NC}"
    exit 1
fi

echo -e "${YELLOW}+ Checking formatter${NC}"
is_formatted=0
# Or check only for the staged files that are about to be committed
# for file in $(git diff --name-only --cached); do
for file in $(find $PWD/srcs $PWD/include/ -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \)); do
    output=$($CLANG_FORMAT_PATH --dry-run -Werror "$file" 2>&1)
    # Check if output contains any suggestions
    if [[ -n "${output//[$'\n\r ']}" ]]; then
        echo -e "${RED}x ${NC}$(basename $file)"
        is_formatted=1
    else
        echo -e "${GREEN}✓ ${NC}$(basename $file)"
    fi
done
if [[ $is_formatted -eq 1 ]]; then
    echo "Some files aren't properly formatted"
    exit 1
fi
EOF

chmod +x "$PWD/.git/hooks/pre-commit"
echo -e "${GREEN}Pre-commit hook setup complete.${NC}"

