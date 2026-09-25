<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v3.4.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v3.4.1** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In the `cibw` step of action.yml, the Python heredoc builds `cmd_bash` and `cmd_pwsh` from user-controlled inputs (`inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, `inputs.only`) via env vars `INPUT_PACKAGE_DIR`, `INPUT_OUTPUT_DIR`, `INPUT_CONFIG_FILE`, and `INPUT_ONLY`, then writes them directly to `$GITHUB_OUTPUT` using `f.write(f"cmd-bash={cmd_bash}\n")` and `f.write(f"cmd-pwsh={cmd_pwsh}\n")`. Although `shlex.join` and `pwsh_quote` provide shell quoting, neither strips embedded newline characters. An attacker-controlled input containing a newline (e.g. `package-dir: ".\nsome-key=injected-value"`) would inject additional key=value pairs into GITHUB_OUTPUT, potentially overwriting other step outputs. The required sanitization step (`printf '%s' "$VAR" | tr -d '\n\r'`) is absent before every write.

Locations:

- `action.yml:100`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a `sanitize()` Python function in the `cibw` step's heredoc that strips `\n` and `\r` characters from all values before writing them to `$GITHUB_OUTPUT`. The three writes (`prepend-path`, `cmd-bash`, `cmd-pwsh`) now all call `sanitize()` on their values, preventing newline injection attacks where user-controlled inputs (package-dir, output-dir, config-file, only) could inject additional key=value pairs into GITHUB_OUTPUT.

