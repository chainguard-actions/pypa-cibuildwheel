# Hardening Report: pypa--cibuildwheel/v4.0.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `ff50f15e4b79bfbf764dafdfd2579175a6ea9771`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **pypa--cibuildwheel/v4.0.0** was hardened automatically. 2 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The 'cibw' run step writes values derived from inputs.* (package-dir, output-dir, config-file, only) to $GITHUB_OUTPUT via Python without the required sanitization step (printf '%s' ... | tr -d '\n\r'). The inputs are passed as env vars (INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY), assembled into cmd_bash and cmd_pwsh via shlex.join/pwsh_quote, then written directly to GITHUB_OUTPUT with f.write(...). An attacker-controlled input containing newlines could inject arbitrary key=value pairs into GITHUB_OUTPUT.

Locations:

- `action.yml:107`

### suspicious-run-content (severity: high)

Sub-check: eval-dynamic. The bash run step executes 'eval "$CIBW_CMD_BASH"' where CIBW_CMD_BASH is set from the step output cmd-bash, which is a shell command string constructed from user-controlled inputs (package-dir, output-dir, config-file, only). This matches the eval-dynamic pattern 'eval\s+[\x60$]' and dynamically executes a command string derived from attacker-controlled values.

Locations:

- `action.yml:119`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection, suspicious-run-content

**Notes:**

Fixed both findings in action.yml:

1. **github-env-injection**: Added a `sanitize()` helper function in the Python heredoc that strips `\r` and `\n` from all values before writing to `$GITHUB_OUTPUT`. All three outputs (`prepend-path`, `cmd-pwsh`, `args-file`) are now sanitized. The old `cmd-bash` output (a shell-quoted string built from user inputs) was removed entirely.

2. **suspicious-run-content (eval-dynamic)**: Eliminated `eval "$CIBW_CMD_BASH"` by replacing the shell-string approach with a NUL-delimited args file. The Python script now writes command arguments as NUL-separated bytes to `$RUNNER_TEMP/cibw_args` and exposes the file path as the `args-file` output. The bash step reads the file with `mapfile -d '' cmd_args < "$CIBW_ARGS_FILE"` and executes `"${cmd_args[@]}"` — a direct array expansion that never passes user-controlled values through shell interpretation.

The Windows PowerShell step retains `Invoke-Expression` with `cmd-pwsh` (which is sanitized), as that is the existing pattern for Windows and was not flagged as a finding.

