<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.3.5

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.3.5** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In action.yml, the composite step 'cibw' runs an inline Python script that writes `cmd-bash=` and `cmd-pwsh=` lines to $GITHUB_OUTPUT. Both values are built from untrusted action inputs: inputs.package-dir, inputs.output-dir, inputs.config-file and inputs.only, passed in through the INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE and INPUT_ONLY env vars. shlex.join and pwsh_quote quote the values for the shell, but they don't remove newline or carriage-return characters. If an input contains a newline, it can end the current line and add extra output entries, such as a forged prepend-path or cmd-bash, which the next steps then run through eval / Invoke-Expression. Nothing like `printf '%s' ... | tr -d '\n\r'` strips \n and \r before the write. Fix: remove \n and \r from each input-derived value before writing it, or reject inputs that contain them, e.g. `value.replace('\n','').replace('\r','')`.

Locations:

- `action.yml:122`
- `action.yml:124`
- `action.yml:125`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a strip_nl() helper to the Python script in the 'cibw' step. It removes \n and \r from INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE and INPUT_ONLY before they go into cmd_args. The prepend-path, cmd-bash and cmd-pwsh values are also passed through strip_nl() as they are written to $GITHUB_OUTPUT. The heredoc is quoted, so the shell leaves the Python escapes alone. Normal behaviour doesn't change: real paths and selectors don't contain newlines. Nothing was run or tested.

