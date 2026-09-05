<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.2.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.2.1** was hardened automatically. 3 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### script-injection (severity: high)

Sub-rule (a): The 'Test cibuildwheel' step directly interpolates ${{ }} expressions inside a run: shell command string. Specifically: `run: uv run --no-sync bin/run_tests.py --test-select=${{ matrix.test_select || 'native' }} ${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }}`. The `matrix.*` and `runner.*` context values are substituted by the GitHub Actions template engine before the shell ever sees the command, allowing an attacker who controls matrix values to inject arbitrary shell commands.

Locations:

- `.github/workflows/test.yml:192`

### script-injection (severity: high)

Sub-rule (a): The 'Run update: docs user projects' step directly interpolates a ${{ }} expression inside a run: shell command string: `run: uvx nox --force-color -s update_proj -- --auth=${{ secrets.GITHUB_TOKEN }}`. Any ${{ ... }} expression directly inside a run: block is a script-injection finding regardless of the context it reads from, because the value is substituted by the template engine before the shell processes the command.

Locations:

- `.github/workflows/update-dependencies.yml:56`

### github-env-injection (severity: high)

The composite action's Python script writes user-controlled input values to $GITHUB_OUTPUT without the required `tr -d '\n\r'` sanitization. The values `cmd-bash` and `cmd-pwsh` are derived from `inputs.package-dir`, `inputs.output-dir`, `inputs.config-file`, and `inputs.only` (via INPUT_PACKAGE_DIR, INPUT_OUTPUT_DIR, INPUT_CONFIG_FILE, INPUT_ONLY env vars). These are written as: `f.write(f"cmd-bash={cmd_bash}\n")` and `f.write(f"cmd-pwsh={cmd_pwsh}\n")`. If any input contains a newline character, it would inject additional key=value pairs into GITHUB_OUTPUT, allowing an attacker to set arbitrary step outputs.

Locations:

- `action.yml:91`

## Iteration Notes

### Iteration 1

**Fixes applied:** script-injection, github-env-injection

**Notes:**

Fixed 3 findings: (1) test.yml line 192: moved matrix.test_select, runner.os, and runner.arch into the step's env block (as TEST_SELECT, RUNNER_OS, RUNNER_ARCH) and rewrote the run command to use shell variables with bash conditional logic for the --run-podman flag. (2) update-dependencies.yml line 56: moved secrets.GITHUB_TOKEN into the step's env block as GITHUB_AUTH_TOKEN and referenced it as "$GITHUB_AUTH_TOKEN" in the shell command. (3) action.yml line 91: added a sanitize() Python helper function that strips \r and \n characters from values before writing cmd-bash, cmd-pwsh, and prepend-path to $GITHUB_OUTPUT, preventing newline injection of additional key=value pairs.

