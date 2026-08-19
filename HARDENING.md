<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.0.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.0.0** was hardened automatically. 2 finding(s) were identified and resolved across 2 iteration(s).

## Findings Fixed

### script-injection (severity: high)

Sub-rule (a): Direct ${{ ... }} expression interpolation inside a run: shell command. The 'Test cibuildwheel' step interpolates ${{ matrix.test_select || 'native' }} and ${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }} directly into the shell command string: `run: uv run --no-sync bin/run_tests.py --test-select=${{ matrix.test_select || 'native' }} ${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }}`. These expressions flow through YAML template substitution before the shell processes them, enabling potential command injection via matrix values.

Locations:

- `.github/workflows/test.yml:196`

### script-injection (severity: high)

Sub-rule (a): Direct ${{ ... }} expression interpolation inside a run: shell command. The 'Run update: docs user projects' step interpolates ${{ secrets.GITHUB_TOKEN }} directly into the shell command string: `run: uvx nox --force-color -s update_proj -- --auth=${{ secrets.GITHUB_TOKEN }}`. Any ${{ ... }} expression directly inside a run: block flows through YAML template substitution before the shell processes it, which is a script-injection risk.

Locations:

- `.github/workflows/update-dependencies.yml:50`

## Iteration Notes

### Iteration 1

**Fixes applied:** script-injection

**Notes:**

Fixed two script-injection findings:
1. hardened/action/.github/workflows/test.yml (line 196): Moved `${{ matrix.test_select || 'native' }}` into env var TEST_SELECT and `${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }}` into env var RUN_PODMAN. Shell script uses `"$TEST_SELECT"` and `${RUN_PODMAN:+"$RUN_PODMAN"}` (conditional expansion drops the flag when empty).
2. hardened/action/.github/workflows/update-dependencies.yml (line 50): Moved `${{ secrets.GITHUB_TOKEN }}` into env var GITHUB_AUTH_TOKEN. Shell script uses `--auth="$GITHUB_AUTH_TOKEN"`.

### Iteration 2

**Fixes applied:** github-env-injection

**Notes:**

Fixed two github-env-injection findings:
1. action.yml: Added a `sanitize()` Python helper that strips \n and \r from cmd_bash, cmd_pwsh, and prepend-path values before writing to $GITHUB_OUTPUT, preventing newline injection from user-supplied inputs.
2. .github/workflows/test.yml: Added `safe_cibw_enable=$(printf '%s' "${CIBW_ENABLE}" | tr -d '\n\r')` before writing CIBW_ENABLE to $GITHUB_ENV, preventing injection via attacker-controlled branch names or PR label values.

