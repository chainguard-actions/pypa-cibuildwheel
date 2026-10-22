<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v2.23.4

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v2.23.4** was hardened automatically. 6 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### unpinned-uses (severity: high)

action.yml uses an action by a version tag instead of a full commit SHA: `uses: actions/setup-python@v5`.

Locations:

- `action.yml:28`

### script-injection (severity: high)

Sub-rule (a): the 'cibw' step puts ${{ }} expressions straight into the run: script. The lines are `"${{ steps.python.outputs.python-path }}" -u << "EOF"`, `r"${{ github.action_path }}"` and `Path(r"${{ runner.temp }}")`. Because the heredoc is quoted, bash does not expand it, but the YAML template substitution still happens first. These values should be passed through env: variables instead.

Locations:

- `action.yml:35`
- `action.yml:37`
- `action.yml:57`
- `action.yml:61`

### script-injection (severity: high)

Sub-rule (a): the bash run step (non-Windows) puts expressions straight into the shell command. The lines are `"${{ steps.cibw.outputs.cibw-path }}"`, `"${{ inputs.package-dir }}"`, and `${{ inputs.output-dir != '' && format('--output-dir "{0}"', inputs.output-dir) || ''}}`, plus the same pattern for inputs.config-file and inputs.only. An attacker who controls an input can inject shell commands. These values should go through env: variables and be double-quoted when the script uses them.

Locations:

- `action.yml:74`

### script-injection (severity: high)

Sub-rule (a): the pwsh run step (Windows) puts expressions straight into the script. The lines are `& "${{ steps.cibw.outputs.cibw-path }}"`, `"${{ inputs.package-dir }}"`, and format() expressions built from inputs.output-dir, inputs.config-file and inputs.only. An attacker who controls an input can inject PowerShell commands. These values should go through env: variables, read in the script as $env:VAR.

Locations:

- `action.yml:85`

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

Pinned actions/setup-python to a26af69be951a213d495a4c3e4e4022e16d87065 (v5). Moved every ${{ }} expression into step env: blocks. The install step reads the Python path, action path and runner temp from env vars, and the Python heredoc reads them with os.environ. The bash run step builds a cibw_args array with conditional --output-dir/--config-file/--only flags, keeping 2>&1. The pwsh step builds a $cibwArgs array from $env: vars and runs & "$env:CIBW_EXE_PATH" @cibwArgs. As before, each optional flag is passed only when its input is non-empty.

