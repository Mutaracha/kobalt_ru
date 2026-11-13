#!/usr/bin/env bash
# Run with the repository owner's gh connection (Actions write permission required).
# Default: preview only. --delete irreversibly removes selected runs and artifacts.
set -euo pipefail
case "${1:-}" in
  '') delete=false ;;
  --delete) delete=true ;;
  *) echo "Usage: $0 [--delete]" >&2; exit 2 ;;
esac
repo=Mutaracha/kobalt_ru
branch=arena/9e626d86-kobalt-ru
selection='.status == "completed" and
  ((.conclusion | IN("failure", "cancelled", "timed_out", "startup_failure", "action_required"))
   or (.display_title | IN("Add files via upload", "Delete doc/diagnostics directory")))'
tmp=$(mktemp)
trap 'rm -f "$tmp"' EXIT
# Materialize first so an API/auth failure cannot be mistaken for an empty inventory.
gh api --paginate "repos/$repo/actions/runs?branch=arena%2F9e626d86-kobalt-ru&per_page=100" \
  --jq ".workflow_runs[] | select($selection) | [.id, .head_sha, .conclusion] | @tsv" > "$tmp"
if ! $delete; then
  printf 'Dry run: completed failed/cancelled or exact upload-title runs on %s\n' "$branch"
  printf 'RUN_ID\tSOURCE_SHA\tCONCLUSION\n'
  cat "$tmp"
  echo 'No changes. Review run IDs; --delete also removes their logs and artifacts.'
  exit 0
fi
while IFS=$'\t' read -r id sha conclusion; do
  [[ "$id" =~ ^[0-9]+$ && "$sha" =~ ^[0-9a-f]{40}$ ]] || exit 2
  # Recheck immediately before deleting; never remove an active/restarted run.
  eligible=$(gh api "repos/$repo/actions/runs/$id" \
    --jq "(.head_branch == \"$branch\" and .head_sha == \"$sha\" and ($selection))")
  if [[ "$eligible" != true ]]; then
    printf 'Skipped changed/ineligible run %s\n' "$id"
    continue
  fi
  gh api --method DELETE "repos/$repo/actions/runs/$id"
  printf 'Deleted run %s (%s)\n' "$id" "$conclusion"
done < "$tmp"
