<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.0.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **pypa--cibuildwheel/v4.0.0** was hardened automatically. 2 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The 'cibw' step writes user-controlled input values to $GITHUB_OUTPUT without sanitizing newlines. Specifically, `f.write(f"cmd-bash={cmd_bash}\n")` and `f.write(f"cmd-pwsh={cmd_pwsh}\n")` write values derived from inputs.package-dir, inputs.output-dir, inputs.config-file, and inputs.only. While shlex.join() shell-quotes arguments, it does NOT strip newline characters. A newline in any input could inject additional key=value pairs into GITHUB_OUTPUT. The required sanitization (`printf '%s' ... | tr -d '\n\r'`) is not applied before these writes.

Locations:

- `action.yml:101`
- `action.yml:102`

### script-injection (severity: high)

Sub-rule (b): The bash step executes `eval "$CIBW_CMD_BASH"` where CIBW_CMD_BASH is set from `${{ steps.cibw.outputs.cmd-bash }}` — a step output derived from user-controlled inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only). Even though the variable is double-quoted, `eval` re-parses the string as shell code, making this a script-injection risk when the value contains workflow-controllable data. Similarly, the PowerShell step uses `Invoke-Expression $env:CIBW_CMD_PWSH` (unquoted in PowerShell syntax) where CIBW_CMD_PWSH is set from `${{ steps.cibw.outputs.cmd-pwsh }}`, also derived from user inputs.

Locations:

- `action.yml:113`
- `action.yml:123`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection, script-injection

**Notes:**

Fixed both high-severity findings in action.yml:

1. **github-env-injection**: Added a `strip_newlines()` helper in the Python script that removes \n and \r characters before writing values to $GITHUB_OUTPUT. More importantly, eliminated the problematic `cmd-bash` and `cmd-pwsh` outputs entirely — these were shell command strings derived from user inputs that could inject additional key=value pairs via embedded newlines.

2. **script-injection**: Replaced `eval "$CIBW_CMD_BASH"` (bash) and `Invoke-Expression $env:CIBW_CMD_PWSH` (PowerShell) with direct array-based execution. The bash step now builds a bash array `args=(...)` from individual env vars and executes `"${args[@]}"` directly. The PowerShell step builds a `$args_list` array and uses `& $env:CIBW_BIN @args_list` (PowerShell splatting). User-controlled inputs (package-dir, output-dir, config-file, only) are passed as environment variables and used as typed array elements, never interpolated into shell command strings that get re-parsed as code.

