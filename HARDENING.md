<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.0.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.0.0** was hardened automatically. 2 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The Python heredoc in the `cibw` step writes `cmd-bash` and `cmd-pwsh` to `$GITHUB_OUTPUT` using values derived from user-controlled inputs (`inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, `inputs.only`) via the env vars `INPUT_PACKAGE_DIR`, `INPUT_OUTPUT_DIR`, `INPUT_CONFIG_FILE`, `INPUT_ONLY`. While `shlex.join` and `pwsh_quote` handle shell quoting, neither strips newline characters (`\n`). A newline embedded in any input value would inject an additional `key=value` line into `GITHUB_OUTPUT`, enabling output injection. The required sanitization (`printf '%s' ... | tr -d '\n\r'`) is absent before the `f.write(...)` calls.

Locations:

- `action.yml:88`

### script-injection (severity: high)

Sub-rule (b): The bash step runs `eval "$CIBW_CMD_BASH" 2>&1`, where `CIBW_CMD_BASH` is set from `steps.cibw.outputs.cmd-bash`. That output was constructed from user-controlled inputs (`inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, `inputs.only`). Although the variable is double-quoted (preventing word splitting), `eval` re-parses the expanded string as a shell command, so shell metacharacters (`;`, `|`, `&`, `$(...)`, backticks, etc.) embedded in any input value will be interpreted by the shell. This allows an attacker to inject arbitrary shell commands via the action inputs.

Locations:

- `action.yml:97`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection, script-injection

**Notes:**

Fixed two high-severity findings in hardened/action/action.yml:

1. github-env-injection: Added a `strip_newlines()` helper in the Python heredoc that strips `\n` and `\r` from all three values written to $GITHUB_OUTPUT (`prepend-path`, `cmd-bash`, `cmd-pwsh`). This prevents newline injection via user-controlled inputs (package-dir, output-dir, config-file, only).

2. script-injection: Replaced `eval "$CIBW_CMD_BASH" 2>&1` with a safe xargs-based array construction: `printf '%s' "$CIBW_CMD_BASH" | xargs printf '%s\0'` parses the shell-quoted string into NUL-delimited tokens (xargs honors shell quoting but does not interpret shell metacharacters), which are read into a bash array and executed directly as `"${args[@]}"` without any shell re-parsing.

