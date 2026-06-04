# Hardening Report: pypa--cibuildwheel/v3.4.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `ff50f15e4b79bfbf764dafdfd2579175a6ea9771`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **pypa--cibuildwheel/v3.4.1** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In the 'cibw' step of action.yml, attacker-controlled inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only, inputs.extras) are passed via env vars (INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY, INPUT_EXTRAS) into a Python heredoc that constructs cmd-bash and cmd-pwsh strings and writes them to $GITHUB_OUTPUT without the required sanitization step (printf '%s' ... | tr -d '\n\r'). A newline character embedded in any of these inputs could inject additional key=value pairs into GITHUB_OUTPUT, allowing an attacker to set arbitrary step outputs. Python's shlex.join() and pwsh_quote() provide shell quoting but do NOT strip newlines, so the write is not sanitized per the required pattern.

Locations:

- `action.yml:97`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a `sanitize()` helper function inside the Python heredoc in the `cibw` step of action.yml. The function strips `\n` and `\r` characters from values before they are written to `$GITHUB_OUTPUT`. All three outputs (`prepend-path`, `cmd-bash`, `cmd-pwsh`) now pass through `sanitize()` before being written, preventing newline injection attacks from attacker-controlled inputs (package-dir, output-dir, config-file, only, extras).

