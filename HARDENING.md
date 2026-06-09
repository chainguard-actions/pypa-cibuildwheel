# Hardening Report: pypa--cibuildwheel/v4.0.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `ff50f15e4b79bfbf764dafdfd2579175a6ea9771`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **pypa--cibuildwheel/v4.0.0** was hardened automatically. 1 finding(s) were identified and resolved across 2 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The 'cibw' step writes attacker-controlled input values to $GITHUB_OUTPUT without sanitization. The Python heredoc constructs cmd_bash and cmd_pwsh from INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, and INPUT_ONLY (all mapped from inputs.* via env:), then writes them directly with f.write(f"cmd-bash={cmd_bash}\n") and f.write(f"cmd-pwsh={cmd_pwsh}\n"). No printf '%s' ... | tr -d '\n\r' sanitization is applied before writing. An attacker-controlled input containing newline characters could inject additional key=value pairs into $GITHUB_OUTPUT, potentially overwriting subsequent step outputs.

Locations:

- `action.yml:88`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a `sanitize()` helper function in the Python heredoc within the 'cibw' step that strips newline (\n) and carriage-return (\r) characters from values before writing them to $GITHUB_OUTPUT. All three outputs (prepend-path, cmd-bash, cmd-pwsh) are now passed through `sanitize()` before being written, preventing attacker-controlled input values from injecting additional key=value pairs into $GITHUB_OUTPUT via embedded newlines.

### Iteration 2

**Fixes applied:** suspicious-run-content

**Notes:**

Eliminated the eval-dynamic pattern by replacing `eval "$CIBW_CMD_BASH"` with a direct bash array invocation. The Python setup step now writes the cibw binary path as a `cibw-bin` output instead of a shell-joined command string. The bash run step constructs a bash array (`args=(...)`) from environment variables and executes it with `"${args[@]}"` — no eval, no dynamic command string evaluation. All user inputs are passed via the `env:` block and referenced as plain shell variables. Also removed the now-unused `import shlex` statement.

