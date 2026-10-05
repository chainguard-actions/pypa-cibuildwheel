<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.3.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.3.0** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The `cibw` step writes user-controlled input values to `$GITHUB_OUTPUT` without stripping newlines. The values `cmd-bash` and `cmd-pwsh` are derived from `INPUT_PACKAGE_DIR`, `INPUT_OUTPUT_DIR`, `INPUT_CONFIG_FILE`, and `INPUT_ONLY`, which are all set from `inputs.*` (attacker-controllable). Although `shlex.join()` and `pwsh_quote()` provide shell quoting, neither strips newline (`\n`) or carriage-return (`\r`) characters. A newline embedded in any input value would inject additional `key=value` lines into `GITHUB_OUTPUT`, allowing an attacker to overwrite arbitrary step outputs. The required sanitization (`printf '%s' "$VAR" | tr -d '\n\r'`) is absent before every write to the special environment file.

Locations:

- `action.yml:100`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a `sanitize()` function in the Python heredoc of the `cibw` step that strips newline (\n) and carriage-return (\r) characters using `str.translate(str.maketrans("", "", "\n\r"))`. All three values written to GITHUB_OUTPUT (`prepend-path`, `cmd-bash`, `cmd-pwsh`) are now passed through `sanitize()` before being written, preventing injection of additional key=value lines via attacker-controlled inputs (package-dir, output-dir, config-file, only).

