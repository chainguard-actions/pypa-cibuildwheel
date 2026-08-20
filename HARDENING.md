<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v2.23.4

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v2.23.4** was hardened automatically. 6 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### unpinned-uses (severity: high)

action.yml uses `actions/setup-python@v5` which is pinned to a mutable tag rather than a full 40-character commit SHA. This is vulnerable to supply-chain attacks if the tag is moved. It should be pinned to a specific commit SHA (e.g., `actions/setup-python@<40-char-sha> # v5`).

Locations:

- `action.yml:27`

### script-injection (severity: high)

Rule (a) violation: Multiple `${{ }}` expressions are interpolated directly into `run:` shell command strings without going through env vars.

Step `cibw` (bash, starting line 32):
- Line 34: `"${{ steps.python.outputs.python-path }}" -u << "EOF"` — steps context interpolated directly as the shell command
- Line 53: `r"${{ github.action_path }}"` — github context interpolated inside a Python heredoc
- Line 57: `r"${{ runner.temp }}"` — runner context interpolated inside a Python heredoc

Any of these values flowing through YAML template substitution before the shell parses them constitutes a script-injection risk.

Locations:

- `action.yml:34`
- `action.yml:53`
- `action.yml:57`

### script-injection (severity: high)

Rule (a) violation: The bash `run:` step (line 68) directly interpolates multiple `${{ }}` expressions into the shell command string:
- `"${{ steps.cibw.outputs.cibw-path }}"` (line 69)
- `"${{ inputs.package-dir }}"` (line 70) — attacker-controlled input
- `${{ inputs.output-dir != '' && format('--output-dir "{0}"', inputs.output-dir) || ''}}` (line 71) — attacker-controlled input
- `${{ inputs.config-file != '' && format('--config-file "{0}"', inputs.config-file) || ''}}` (line 72) — attacker-controlled input
- `${{ inputs.only != '' && format('--only "{0}"', inputs.only) || ''}}` (line 73) — attacker-controlled input

All `inputs.*` values are caller-controlled and are interpolated directly into the shell command before the shell ever sees them, enabling command injection.

Locations:

- `action.yml:68`
- `action.yml:69`
- `action.yml:70`
- `action.yml:71`
- `action.yml:72`
- `action.yml:73`

### script-injection (severity: high)

Rule (a) violation: The PowerShell `run:` step (line 79) directly interpolates the same set of `${{ }}` expressions into the shell command string:
- `"${{ steps.cibw.outputs.cibw-path }}"` (line 80)
- `"${{ inputs.package-dir }}"` (line 81) — attacker-controlled input
- `${{ inputs.output-dir != '' && format('--output-dir "{0}"', inputs.output-dir) || ''}}` (line 82) — attacker-controlled input
- `${{ inputs.config-file != '' && format('--config-file "{0}"', inputs.config-file) || ''}}` (line 83) — attacker-controlled input
- `${{ inputs.only != '' && format('--only "{0}"', inputs.only) || ''}}` (line 84) — attacker-controlled input

All `inputs.*` values are caller-controlled and are interpolated directly into the PowerShell command before the shell ever sees them, enabling command injection.

Locations:

- `action.yml:79`
- `action.yml:80`
- `action.yml:81`
- `action.yml:82`
- `action.yml:83`
- `action.yml:84`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.package-dir }}" appears directly in run: block of step ""; move to env: map

Locations:

- `action.yml:75`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.package-dir }}" appears directly in run: block of step ""; move to env: map

Locations:

- `action.yml:86`

## Iteration Notes

### Iteration 1

**Fixes applied:** unpinned-uses, script-injection, static-inline-injection

**Notes:**

Fixed all findings in hardened/action/action.yml:
1. Pinned actions/setup-python@v5 to full SHA a26af69be951a213d495a4c3e4e4022e16d87065
2. In the 'cibw' step: moved steps.python.outputs.python-path → PYTHON_PATH, github.action_path → ACTION_PATH, runner.temp → RUNNER_TEMP_PATH into env: block; updated Python heredoc to use os.environ[] instead of r"${{ }}" interpolation
3. In the bash run step: moved all ${{ }} expressions (cibw-path, package-dir, output-dir, config-file, only) to env: block; used bash array to safely build argument list with proper quoting
4. In the PowerShell run step: same env vars used with a PowerShell array (@()) for safe argument construction
5. package-dir is always passed as a positional argument (not optional) so it uses "$INPUT_PACKAGE_DIR" directly; optional inputs (output-dir, config-file, only) use conditional array appending

