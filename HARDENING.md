<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.3.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.3.1** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In the 'cibw' step of action.yml, user-controlled input values (INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY — all sourced from inputs.*) are incorporated into cmd_bash and cmd_pwsh via shlex.join/pwsh_quote and then written directly to $GITHUB_OUTPUT using Python's f.write(). Neither shlex.join nor pwsh_quote strips newline characters from the values. A newline embedded in any of these inputs (e.g. inputs.package-dir) would inject additional key=value pairs into the GITHUB_OUTPUT file, potentially overwriting subsequent step outputs. The required sanitization step (printf '%s' ... | tr -d '\n\r') is absent before every write to GITHUB_OUTPUT that involves user-supplied data.

Locations:

- `action.yml:96`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

Added a sanitize() Python function in the cibw step's inline Python script that strips \n and \r characters from all values before writing them to $GITHUB_OUTPUT. The three outputs (prepend-path, cmd-bash, cmd-pwsh) are now all sanitized. The cmd-bash and cmd-pwsh values are the primary injection vectors since they incorporate user-controlled inputs (INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY) via shlex.join() and pwsh_quote() respectively — neither of which strips newlines. The sanitize() function is applied inline at the point of writing, consistent with the guidance to sanitize where a value is WRITTEN.

