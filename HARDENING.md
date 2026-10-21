<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.3.3

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.3.3** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In the composite step 'cibw' (action.yml), an embedded Python heredoc builds cmd_args from the inputs package-dir, output-dir, config-file and only, passed in through the env vars INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE and INPUT_ONLY. It writes these to $GITHUB_OUTPUT with f.write(f"cmd-bash={cmd_bash}\n") and f.write(f"cmd-pwsh={cmd_pwsh}\n"). shlex.join and pwsh_quote handle shell quoting but leave newline and carriage-return characters in place. Nothing like printf '%s' | tr -d '\n\r' strips them before the write. An input containing a newline can therefore inject extra output lines, such as a forged cmd-bash or cmd-pwsh value. The next steps then run that value with eval "$CIBW_CMD_BASH" and Invoke-Expression $env:CIBW_CMD_PWSH, which turns output injection into command execution. Remove \n and \r from every input-derived value, or reject inputs that contain them, before writing to GITHUB_OUTPUT.

Locations:

- `action.yml:122`
- `action.yml:124`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

I changed the embedded Python in the 'cibw' step of action.yml. A new strip_nl() helper removes \n and \r from every item in cmd_args and from clean_bin_path. This runs before cmd-bash and cmd-pwsh are built and before anything is written to $GITHUB_OUTPUT. An input containing a newline can no longer add extra output lines, so it can't forge a cmd-bash or cmd-pwsh value for the later eval / Invoke-Expression steps. The shell quoting (shlex.join and pwsh_quote) is unchanged. I haven't run the action to test this.

