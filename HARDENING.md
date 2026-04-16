# Hardening Report: pypa--cibuildwheel/v2.23.4

> This file was generated automatically by the hardening agent.

**Policy SHA:** `c40cfe5fa14e08549b1b988e7e5a26da4816abf0`

**Test Policy SHA:** `f2e7d85641cde4267138117189b8eba7ba2bfbde`

Action **pypa--cibuildwheel/v2.23.4** was hardened automatically. 4 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### unpinned-uses (severity: high)

action.yml uses `actions/setup-python@v5` — the ref `@v5` is a mutable tag, not a pinned 40-character commit SHA. This exposes the action to supply-chain attacks if the upstream tag is moved or compromised. It should be pinned to a full SHA, e.g. `actions/setup-python@<40-char-sha> # v5`.

Locations:

- `action.yml:28`

### script-injection (severity: high)

Two `run:` steps directly interpolate user-supplied `inputs.*` expressions into shell command strings without first assigning them to environment variables. Specifically, `${{ inputs.package-dir }}`, `${{ inputs.output-dir }}`, `${{ inputs.config-file }}`, and `${{ inputs.only }}` are embedded directly in the bash and pwsh run blocks. A caller could supply a value containing shell metacharacters (e.g. `"; malicious-command #`) to achieve arbitrary command execution. These inputs should be passed via `env:` variables and referenced as `$ENV_VAR` in the shell script instead.

Locations:

- `action.yml:68`
- `action.yml:79`

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

1. Pinned actions/setup-python@v5 to full SHA a26af69be951a213d495a4c3e4e4022e16d87065 (SHA sourced from another hardened action in the repo). 2. Fixed script injection in both bash (line 68) and pwsh (line 79) run blocks by moving all inputs.* expressions (package-dir, output-dir, config-file, only) into env: blocks as INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY. Shell scripts now use array-based argument construction referencing plain environment variables, eliminating shell metacharacter injection risk.

