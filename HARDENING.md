<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.1.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.1.0** was hardened automatically. 2 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The Python script in the 'cibw' step writes user-controlled input values to $GITHUB_OUTPUT without newline sanitization. The variables cmd-bash and cmd-pwsh are constructed from inputs.package-dir, inputs.output-dir, inputs.config-file, and inputs.only (passed as INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY env vars). These are written directly via Python's f.write() with no `tr -d '\n\r'` sanitization step. A newline character in any of these inputs could inject additional key=value pairs into GITHUB_OUTPUT, potentially overwriting subsequent step outputs.

Locations:

- `action.yml:88`

### suspicious-run-content (severity: high)

eval-dynamic: The bash step uses `eval "$CIBW_CMD_BASH"` where CIBW_CMD_BASH is set from steps.cibw.outputs.cmd-bash — a value derived from user-controlled inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only). This matches the eval-dynamic pattern (eval followed by $). An attacker who can supply shell metacharacters in these inputs could achieve arbitrary command execution via the eval call.

Locations:

- `action.yml:101`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection, suspicious-run-content

**Notes:**

Fixed both findings in action.yml:

1. github-env-injection: Added a sanitize() function in the Python script that strips \r and \n from all values before writing to $GITHUB_OUTPUT, preventing newline injection from user-controlled inputs.

2. suspicious-run-content (eval-dynamic): Eliminated eval entirely by refactoring the approach:
   - Python script now writes individual arguments (cibw-bin, cibw-package-dir, cibw-output-dir, cibw-config-file, cibw-only) as separate sanitized GITHUB_OUTPUT values instead of a shell-quoted command string
   - Bash step now constructs a bash array from individual env vars and executes directly with "${args[@]}" — no eval
   - PowerShell step now uses a PowerShell array with the call operator & and splatting (@args_list) instead of Invoke-Expression
   - Removed dead code: shlex import, cmd_args, pwsh_quote, cmd_pwsh

