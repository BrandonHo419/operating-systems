#!/usr/bin/env bash
# Automated test suite for the course test-mergesort executable.
# Expected interface: ./test-mergesort <input size> <cutoff level> <seed>
set -u
cd "$(dirname "$(realpath "$0")")"
EXEC=${EXEC:-./test-mergesort}
RESULTS=${RESULTS:-test-results.csv}
TIME_LIMIT=${TIME_LIMIT:-30}

if ! make; then
  echo 'BUILD FAILED: fix compiler errors before testing.' >&2
  exit 1
fi
if [[ ! -x "$EXEC" ]]; then
  echo "Executable $EXEC not found. Check Makefile target or set EXEC=./your-program" >&2
  exit 1
fi
printf 'category,size,cutoff,seed,result,seconds,details\n' > "$RESULTS"
passed=0; failed=0
run_case() {
  local category=$1 n=$2 cutoff=$3 seed=$4 output status result seconds details
  output=$(timeout "${TIME_LIMIT}s" "$EXEC" "$n" "$cutoff" "$seed" 2>&1)
  status=$?
  if (( status == 0 )) && grep -Eq 'Sorting [0-9]+ elements took' <<< "$output"; then
    result=PASS; ((passed+=1))
  else
    result=FAIL; ((failed+=1))
  fi
  seconds=$(sed -nE 's/.* took ([0-9]+\.[0-9]+) seconds\..*/\1/p' <<< "$output" | head -n 1)
  details=${output//$'\n'/ }
  details=${details//\"/\"\"}
  printf '%s,%s,%s,%s,%s,%s,"%s"\n' "$category" "$n" "$cutoff" "$seed" "$result" "$seconds" "exit=$status $details" >> "$RESULTS"
  printf '%-5s %-13s size=%-9s cutoff=%-3s seed=%s\n' "$result" "$category" "$n" "$cutoff" "$seed"
}

# Test minimum valid sizes, odd/even splits, and cutoffs exceeding the recursion depth.
for n in 2 3 4 5 7 8 9 16 17 31 32 33 100; do
  for cutoff in 0 1 2 3 4 8; do
    run_case boundary "$n" "$cutoff" 42
  done
done
# Repeat different deterministic random arrays at fixed size.
for seed in 0 1 2 7 42 123 2026 99999; do
  for cutoff in 0 1 2 3 4; do
    run_case seeds 1000 "$cutoff" "$seed"
  done
done
# Larger runs to check parallel correctness; timings are only rough.
for n in 10000 100000 1000000; do
  for cutoff in 0 1 2 3 4; do
    run_case stress "$n" "$cutoff" 42
  done
done
printf '\nSummary: %d passed; %d failed. Results: %s\n' "$passed" "$failed" "$RESULTS"
if ((failed > 0)); then exit 1; fi
