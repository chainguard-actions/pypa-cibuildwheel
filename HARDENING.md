<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v3.4.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **pypa--cibuildwheel/v3.4.1** was hardened automatically. 2 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

The `cibw` step writes user-controlled input values to $GITHUB_OUTPUT without stripping newline characters. The values `cmd-bash` and `cmd-pwsh` are constructed from `inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, and `inputs.only` (via `shlex.join()` and `pwsh_quote()` respectively), but neither function removes `\n` or `\r`. An attacker supplying a newline in any of these inputs can inject additional key=value lines into $GITHUB_OUTPUT, setting arbitrary step outputs. The required sanitization (`printf '%s' ... | tr -d '\n\r'`) is absent before every write.

Locations:

- `action.yml:113`
- `action.yml:114`
- `action.yml:115`
- `action.yml:116`

### script-injection (severity: high)

Two steps execute user-controlled content as shell/PowerShell code:

(b) Line 132: `eval "$CIBW_CMD_BASH"` — `CIBW_CMD_BASH` is set from `steps.cibw.outputs.cmd-bash`, which is constructed from user-controlled inputs (`inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, `inputs.only`). Even though the variable is double-quoted, `eval` re-interprets the content as shell code, making this a script injection vector for any shell metacharacters that survive `shlex.join()` quoting.

(b) Line 142: `Invoke-Expression $env:CIBW_CMD_PWSH` — `CIBW_CMD_PWSH` is set from `steps.cibw.outputs.cmd-pwsh`, derived from the same user-controlled inputs. `Invoke-Expression` executes its argument as PowerShell code and the variable is unquoted, allowing an attacker to inject arbitrary PowerShell commands via crafted input values.

Locations:

- `action.yml:132`
- `action.yml:142`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection, script-injection

**Notes:**

Fixed both findings by changing the approach: instead of writing user-controlled command strings (cmd-bash, cmd-pwsh) to $GITHUB_OUTPUT and then executing them via eval/Invoke-Expression, the Python step now writes the commands to script files (cibw_run.sh, cibw_run.ps1) in $RUNNER_TEMP. Only the script file paths (not user-controlled) are written to $GITHUB_OUTPUT with newline stripping. The execution steps now use 'sh "$CIBW_BASH_SCRIPT"' and '& "$env:CIBW_PWSH_SCRIPT"' to execute the script files directly, eliminating both the GITHUB_OUTPUT injection vector and the eval/Invoke-Expression script injection vectors.

