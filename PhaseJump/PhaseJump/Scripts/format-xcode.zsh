# Runs clang-format for Xcode project

cd "$PROJECT_DIR"/../../PhaseJump/Classes

if [[ "$(uname -m)" == arm64 ]]; then
    export PATH="/opt/homebrew/bin:$PATH"
fi

if which clang-format > /dev/null; then
find -E . -regex '.*\.(cpp|hpp|h|h)' -exec clang-format -style=file -i {} \;
fi
