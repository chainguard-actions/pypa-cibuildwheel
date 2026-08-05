<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.2.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.2.0** was hardened automatically. 3 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### script-injection (severity: high)

Sub-rule (a): The 'Test cibuildwheel' step directly interpolates ${{ }} expressions into the run: shell command string. Specifically: `run: uv run --no-sync bin/run_tests.py --test-select=${{ matrix.test_select || 'native' }} ${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }}`. The `matrix.test_select`, `runner.os`, and `runner.arch` contexts are substituted by the YAML template engine before the shell ever sees the command, allowing shell metacharacters to be injected. These values should be moved to `env:` variables and referenced as `"$VAR"` in the run block.

Locations:

- `.github/workflows/test.yml:199`

### script-injection (severity: high)

Sub-rule (a): The 'Run update: docs user projects' step directly interpolates a ${{ }} expression into the run: shell command string: `run: uvx nox --force-color -s update_proj -- --auth=${{ secrets.GITHUB_TOKEN }}`. Any ${{ ... }} expression directly inside a run: block is a script-injection risk because the value is substituted by the YAML template engine before the shell processes the command. The token should be passed via an env: variable instead.

Locations:

- `.github/workflows/update-dependencies.yml:53`

### github-env-injection (severity: high)

The composite action's Python script writes values derived from user inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only) to $GITHUB_OUTPUT without newline sanitization. The env vars INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, and INPUT_ONLY are set from inputs.* and then used to build cmd_bash and cmd_pwsh, which are written directly to GITHUB_OUTPUT via `f.write(f'cmd-bash={cmd_bash}\n')` and `f.write(f'cmd-pwsh={cmd_pwsh}\n')`. While shlex.join() and pwsh_quote() provide shell quoting, they do not strip newlines. A newline in any input value could inject additional key=value pairs into $GITHUB_OUTPUT. The required sanitization (`printf '%s' "$VAR" | tr -d '\n\r'`) must be applied before writing to the special environment file.

Locations:

- `action.yml:82`

## Iteration Notes

### Iteration 1

**Fixes applied:** script-injection, github-env-injection

**Notes:**

Fixed three security findings: (1) test.yml: Moved matrix.test_select, runner.os, runner.arch expressions out of the run: shell command into env: variables (MATRIX_TEST_SELECT, RUNNER_OS_CTX, RUNNER_ARCH_CTX), using bash arrays for the optional --run-podman flag. (2) update-dependencies.yml: Moved secrets.GITHUB_TOKEN out of the run: command into an env: variable (GITHUB_AUTH_TOKEN), referenced safely as $GITHUB_AUTH_TOKEN in the shell. (3) action.yml: Added a sanitize() Python function that strips \r and \n from values before writing prepend-path, cmd-bash, and cmd-pwsh to $GITHUB_OUTPUT, preventing newline injection attacks.

