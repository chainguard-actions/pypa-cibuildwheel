<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.1.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.1.1** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The `cibw` step reads user-controlled inputs (`inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, `inputs.only`, `inputs.extras`) via environment variables (`INPUT_PACKAGE_DIR`, `INPUT_OUTPUT_DIR`, `INPUT_CONFIG_FILE`, `INPUT_ONLY`, `INPUT_EXTRAS`) and writes derived values (`cmd-bash`, `cmd-pwsh`, `prepend-path`) to `$GITHUB_OUTPUT` using Python's `f.write()` without the required `printf '%s' ... | tr -d '\n\r'` sanitization. A newline character embedded in any input value would allow an attacker to inject additional key=value pairs into GITHUB_OUTPUT, potentially overwriting subsequent step outputs. The fix requires stripping newlines from each user-controlled value before writing it to the special environment file.

Locations:

- `action.yml:97`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a `sanitize()` helper function in the inline Python script that strips `\r` and `\n` characters from values before writing them to `$GITHUB_OUTPUT`. All three output values (`prepend-path`, `cmd-bash`, `cmd-pwsh`) are now sanitized through this function before being written to the GITHUB_OUTPUT file, preventing newline injection attacks from user-controlled inputs (`inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, `inputs.only`, `inputs.extras`).

