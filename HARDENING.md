<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.2.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.2.0** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In the `cibw` step of action.yml, the Python script writes `cmd-bash` and `cmd-pwsh` to `$GITHUB_OUTPUT` using Python's `f.write()` without any newline sanitization. These values are constructed from user-controlled inputs (`INPUT_PACKAGE_DIR`, `INPUT_OUTPUT_DIR`, `INPUT_CONFIG_FILE`, `INPUT_ONLY`) which are set from `${{ inputs.* }}` expressions. An attacker-controlled input containing a newline character (`\n`) could inject additional `key=value` pairs into `$GITHUB_OUTPUT`, potentially overwriting other outputs or injecting malicious values. The required sanitization step (`printf '%s' ... | tr -d '\n\r'`) is absent before the writes. Affected writes: `f.write(f"cmd-bash={cmd_bash}\n")` and `f.write(f"cmd-pwsh={cmd_pwsh}\n")`.

The subsequent steps then consume these outputs via `CIBW_CMD_BASH: ${{ steps.cibw.outputs.cmd-bash }}` and execute them with `eval "$CIBW_CMD_BASH"` (bash) and `Invoke-Expression $env:CIBW_CMD_PWSH` (PowerShell), amplifying the impact of any injected content.

Locations:

- `action.yml:99`
- `action.yml:100`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a `sanitize()` helper function in the Python script embedded in the `cibw` step of action.yml. The function strips `\n` and `\r` characters from user-controlled input values (`INPUT_PACKAGE_DIR`, `INPUT_OUTPUT_DIR`, `INPUT_CONFIG_FILE`, `INPUT_ONLY`) before they are used to construct `cmd_bash` and `cmd_pwsh`, which are then written to `$GITHUB_OUTPUT`. This prevents an attacker from injecting additional key=value pairs into `$GITHUB_OUTPUT` via newline characters in input values. The optional inputs were updated to use `sanitize(os.environ.get(..., ""))` to ensure sanitization occurs before the walrus operator truthiness check.

