# Run this script from the PhaseJump/Classes folder to format all files
find -E . -regex '.*\.(cpp|hpp|h|h)' -exec clang-format -style=file -i {} \;
