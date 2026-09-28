<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v2.23.4

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v2.23.4** was hardened automatically. 4 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### unpinned-uses (severity: high)

The action uses `actions/setup-python@v5` which is pinned to a mutable tag rather than a full 40-character commit SHA. This is vulnerable to supply-chain attacks if the tag is moved to a different commit.

Locations:

- `action.yml:27`

### script-injection (severity: high)

Sub-rule (a): Multiple `${{ }}` expressions are directly interpolated inside `run:` shell command strings, allowing script injection. In the `cibw` step: `${{ steps.python.outputs.python-path }}` is used as the shell interpreter path, `${{ github.action_path }}` is embedded in a Python heredoc, and `${{ runner.temp }}` is embedded in a Python heredoc — all without going through env vars. In the bash run step: `${{ steps.cibw.outputs.cibw-path }}`, `${{ inputs.package-dir }}`, `${{ inputs.output-dir }}`, `${{ inputs.config-file }}`, and `${{ inputs.only }}` are all interpolated directly into the shell command string. In the pwsh run step: the same set of expressions (`${{ steps.cibw.outputs.cibw-path }}`, `${{ inputs.package-dir }}`, `${{ inputs.output-dir }}`, `${{ inputs.config-file }}`, `${{ inputs.only }}`) are interpolated directly into the PowerShell command string. Attacker-controlled inputs (package-dir, output-dir, config-file, only) can inject arbitrary shell commands.

Locations:

- `action.yml:35`
- `action.yml:53`
- `action.yml:57`
- `action.yml:69`
- `action.yml:70`
- `action.yml:71`
- `action.yml:72`
- `action.yml:73`
- `action.yml:79`
- `action.yml:80`
- `action.yml:81`
- `action.yml:82`
- `action.yml:83`

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
1. Pinned actions/setup-python@v5 to full SHA a26af69be951a213d495a4c3e4e4022e16d87065.
2. In the cibw step: moved ${{ steps.python.outputs.python-path }}, ${{ github.action_path }}, and ${{ runner.temp }} to env: block as PYTHON_PATH, ACTION_PATH, RUNNER_TEMP_PATH; updated the Python heredoc to use os.environ[] instead of inline expressions.
3. In the bash run step: moved all ${{ }} expressions (cibw-path, package-dir, output-dir, config-file, only) to env: block; replaced the inline conditional format() expressions with bash array construction for optional flags.
4. In the pwsh run step: moved all ${{ }} expressions to env: block; replaced inline conditional format() expressions with PowerShell array construction using $env:VAR_NAME syntax and & invocation operator.

