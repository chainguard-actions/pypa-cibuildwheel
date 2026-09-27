<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.2.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.2.1** was hardened automatically. 2 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The 'cibw' step writes user-controlled values to $GITHUB_OUTPUT without sanitization. The Python script builds cmd-bash and cmd-pwsh from user inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only) via INPUT_* env vars, then writes them with `f.write(f"cmd-bash={cmd_bash}\n")` and `f.write(f"cmd-pwsh={cmd_pwsh}\n")`. A newline character embedded in any of these inputs would inject additional key=value pairs into GITHUB_OUTPUT. The required sanitization (`printf '%s' ... | tr -d '\n\r'`) is absent.

Locations:

- `action.yml:100`

### script-injection (severity: high)

Rule (b): Two run steps execute user-controlled data as shell commands. (1) The bash step runs `eval "$CIBW_CMD_BASH"` where CIBW_CMD_BASH is set from `steps.cibw.outputs.cmd-bash`, which is constructed from user-supplied inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only). eval re-parses the shell-quoted string, and a crafted input can escape the quoting. (2) The PowerShell step runs `Invoke-Expression $env:CIBW_CMD_PWSH` (unquoted expansion) where CIBW_CMD_PWSH similarly contains user-controlled input values. Both patterns allow an attacker to inject arbitrary shell/PowerShell commands via action inputs.

Locations:

- `action.yml:119`
- `action.yml:127`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection, script-injection

**Notes:**

Fixed both findings by restructuring the action to run cibuildwheel directly from within the Python heredoc using subprocess.run(cmd_args) instead of building shell command strings and evaluating them. The two separate bash/pwsh steps that used eval and Invoke-Expression have been removed entirely. The Python script now: (1) builds cmd_args as a list, (2) writes only prepend-path (not user-controlled) to GITHUB_OUTPUT, (3) prepends the clean bin path to PATH in the subprocess environment, and (4) runs cibuildwheel directly via subprocess.run(cmd_args, env=env) — a list-based exec with no shell parsing, eliminating both the GITHUB_OUTPUT injection and the eval/Invoke-Expression injection vectors.

