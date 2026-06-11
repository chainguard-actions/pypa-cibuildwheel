<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v2.23.4

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **pypa--cibuildwheel/v2.23.4** was hardened automatically. 4 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### unpinned-uses (severity: high)

The action uses `actions/setup-python@v5` with a mutable tag reference instead of a pinned 40-character SHA commit hash. This is vulnerable to supply-chain attacks if the tag is moved to a different commit.

Locations:

- `action.yml:27`

### script-injection (severity: high)

Sub-rule (a): Multiple ${{ }} expressions are directly interpolated inside run: shell command strings without going through env: variables.

In the 'cibw' step (bash heredoc):
- Line 35: `"${{ steps.python.outputs.python-path }}"` is used directly as the shell command to invoke.
- Line 53: `r"${{ github.action_path }}"` is interpolated into the Python script string inside the heredoc.
- Line 56: `Path(r"${{ runner.temp }}")` is interpolated into the Python script string inside the heredoc.

In the bash run step (Linux/macOS):
- Line 68: `"${{ steps.cibw.outputs.cibw-path }}"` used directly as the command.
- Line 69: `"${{ inputs.package-dir }}"` — attacker-controlled input directly interpolated.
- Lines 70-72: `${{ inputs.output-dir ... }}`, `${{ inputs.config-file ... }}`, `${{ inputs.only ... }}` — attacker-controlled inputs directly interpolated.

In the PowerShell run step (Windows):
- Lines 77-82: Same set of expressions (`steps.cibw.outputs.cibw-path`, `inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, `inputs.only`) directly interpolated.

Any ${{ ... }} expression inside a run: block is a script-injection risk because YAML template substitution happens before the shell ever sees the string, allowing injection of shell metacharacters.

Locations:

- `action.yml:35`
- `action.yml:53`
- `action.yml:56`
- `action.yml:68`
- `action.yml:69`
- `action.yml:70`
- `action.yml:71`
- `action.yml:72`
- `action.yml:77`
- `action.yml:78`
- `action.yml:79`
- `action.yml:80`
- `action.yml:81`

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

1. Pinned actions/setup-python@v5 to SHA a26af69be951a213d495a4c3e4e4022e16d87065.
2. Fixed script-injection in the 'cibw' bash heredoc step: moved PYTHON_PATH, ACTION_PATH, RUNNER_TEMP_PATH to env: block; Python code now reads ACTION_PATH and RUNNER_TEMP_PATH via os.environ[] instead of inline ${{ }} expressions.
3. Fixed script-injection in the bash run step (Linux/macOS): moved all ${{ }} expressions (cibw-path, package-dir, output-dir, config-file, only) to env: block; rewrote script to build args array using env vars with conditional checks.
4. Fixed script-injection in the PowerShell run step (Windows): same env: approach; rewrote script to build PowerShell args array using $env: variables.
5. The static-inline-injection findings for inputs.package-dir in both bash and PowerShell steps are resolved by the same env: fixes.

