#!/bin/bash
# Prepare an isolated source export, never a branch or worktree.
set -euo pipefail
root=$(cd "$(dirname "$0")/../.." && pwd)
base=1796a32559dbb2912bd565990d1c7a0ca7ac4e52
out="$root/build/control-beta10"
if [[ -e "$out" ]]; then
    echo "Refusing to overwrite $out; preserve/remove it explicitly before preparing again." >&2
    exit 1
fi
git -C "$root" cat-file -e "$base^{commit}" 2>/dev/null ||
    git -C "$root" fetch --no-tags https://github.com/nicoverbruggen/kobalt.git "$base"
mkdir -p "$out"
git -C "$root" archive "$base" | tar -x -C "$out"
git -C "$root" apply --check --directory=build/control-beta10 "$root/platforms/control-beta10/ru.patch"
git -C "$root" apply --directory=build/control-beta10 "$root/platforms/control-beta10/ru.patch"
echo "Prepared $out from $base + platforms/control-beta10/ru.patch"
echo 'Not a binary. Build only this export, not the parent fork checkout.'
