<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.1.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **pypa--cibuildwheel/v4.1.0** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The Python heredoc in the 'cibw' step writes values derived from user-controlled inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only) to $GITHUB_OUTPUT without stripping newlines. The values are processed through shlex.join() and pwsh_quote() for shell-quoting purposes, but neither function removes newline characters. An attacker supplying a newline in any of these inputs could inject additional key=value pairs into GITHUB_OUTPUT (e.g., 'cmd-bash=...\nsome-key=injected-value'). The required sanitization step (printf '%s' "$VAR" | tr -d '\n\r') is not applied before writing to the special environment file.

Locations:

- `action.yml:82`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a `sanitize()` helper function inside the Python heredoc in the 'cibw' step of action.yml. The function strips `\n` and `\r` characters from values before they are written to $GITHUB_OUTPUT. All three output values (prepend-path, cmd-bash, cmd-pwsh) are now passed through `sanitize()` before being written, preventing newline injection attacks where an attacker could supply a newline in inputs like package-dir, output-dir, config-file, or only to inject additional key=value pairs into GITHUB_OUTPUT.

