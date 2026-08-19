<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v3.4.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v3.4.1** was hardened automatically. 3 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In action.yml, the Python heredoc script writes user-controlled input values to $GITHUB_OUTPUT without the required sanitization step (printf '%s' ... | tr -d '\n\r'). Specifically, cmd_bash (built via shlex.join() from INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY — all sourced from inputs.*) and cmd_pwsh (built via pwsh_quote() from the same inputs) are written directly to GITHUB_OUTPUT. While shlex.join() and pwsh_quote() handle shell quoting, they do not strip newline characters. A newline embedded in any input value (e.g. inputs.package-dir) would inject additional key=value pairs into GITHUB_OUTPUT, potentially overwriting subsequent step outputs.

Locations:

- `action.yml:112`
- `action.yml:113`

### script-injection (severity: high)

Rule (a) violation in .github/workflows/test.yml: The 'Test cibuildwheel' step directly interpolates GitHub Actions expressions into the run: shell command string. The offending line is: `run: uv run --no-sync bin/run_tests.py --test-select=${{ matrix.test_select || 'native' }} ${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }}`. Both ${{ matrix.test_select }} and ${{ runner.os }}/${{ runner.arch }} are substituted by the Actions template engine before the shell ever sees the command, allowing an attacker who controls matrix values to inject arbitrary shell commands.

Locations:

- `.github/workflows/test.yml:195`

### script-injection (severity: high)

Rule (a) violation in .github/workflows/update-dependencies.yml: The 'Run update: docs user projects' step directly interpolates a GitHub Actions expression into the run: shell command string. The offending line is: `run: nox --force-color -s update_proj -- --auth=${{ secrets.GITHUB_TOKEN }}`. Any ${{ ... }} expression interpolated directly into a run: block is a script-injection risk because the value is substituted by the Actions template engine before the shell parses the command. The token value should instead be passed via an env: variable and referenced as $ENV_VAR in the script.

Locations:

- `.github/workflows/update-dependencies.yml:49`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection, script-injection

**Notes:**

Fixed three security findings: (1) action.yml github-env-injection: Added a sanitize() Python helper that strips \n and \r from cmd_bash, cmd_pwsh, and prepend-path values before writing to GITHUB_OUTPUT, preventing newline injection. (2) test.yml script-injection: Moved matrix.test_select and runner.os/runner.arch expressions into env: block as TEST_SELECT and RUN_PODMAN, using a bash array to safely pass the optional --run-podman flag. (3) update-dependencies.yml script-injection: Moved secrets.GITHUB_TOKEN into an env: block as GITHUB_AUTH_TOKEN and referenced it as "$GITHUB_AUTH_TOKEN" in the shell command.

