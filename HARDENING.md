<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.3.4

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.3.4** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

Step 'cibw' (action.yml) runs a Python script inside its run: block that writes `cmd-bash=` and `cmd-pwsh=` to $GITHUB_OUTPUT (`f.write(f"cmd-bash={cmd_bash}\n")`). Those values are built from untrusted inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only, passed in as INPUT_* env vars). Nothing strips newlines or carriage returns first. shlex.join and pwsh_quote handle shell quoting but leave embedded newlines in place, so a value containing a newline can add arbitrary lines to GITHUB_OUTPUT, including new output definitions. This matters because the next steps run `eval "$CIBW_CMD_BASH"` and `Invoke-Expression $env:CIBW_CMD_PWSH`, so a forged output becomes command execution. Fix: remove \n and \r from every input-derived value before writing it, or reject such values (the equivalent of printf '%s' | tr -d '\n\r').

Locations:

- `action.yml:47`
- `action.yml:127`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

In the 'cibw' step of action.yml, I added a `strip_nl` helper to the inline Python script. It removes \n and \r from a value, the same as `tr -d '\n\r'`. It runs on package-dir, output-dir, config-file and only before they go into cmd_args. It also runs on prepend-path, cmd-bash and cmd-pwsh right before they are written to GITHUB_OUTPUT. A value with a newline can no longer add lines or forge outputs there. That closes the route to the later `eval` and `Invoke-Expression` steps. Valid inputs never contain newlines, so normal behaviour is unchanged. Nothing was run to test this.

