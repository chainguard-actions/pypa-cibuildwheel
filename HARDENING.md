<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.1.1

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.1.1** was hardened automatically. 4 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### script-injection (severity: high)

Sub-rule (a): Direct expression interpolation in a run: block. The 'Test cibuildwheel' step interpolates ${{ matrix.test_select || 'native' }} and ${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }} directly into the shell command string. These ${{ }} expressions are substituted before the shell sees the command, allowing injection of shell metacharacters.

Locations:

- `.github/workflows/test.yml:183`

### script-injection (severity: high)

Sub-rule (a): Direct expression interpolation in a run: block. The 'Run update: docs user projects' step interpolates ${{ secrets.GITHUB_TOKEN }} directly into the shell command string: `run: uvx nox --force-color -s update_proj -- --auth=${{ secrets.GITHUB_TOKEN }}`. Any ${{ }} expression directly inside a run: block is a script-injection finding regardless of which context it reads from.

Locations:

- `.github/workflows/update-dependencies.yml:47`

### script-injection (severity: high)

Sub-rule (b): The run: block executes `eval "$CIBW_CMD_BASH"` where CIBW_CMD_BASH is set from ${{ steps.cibw.outputs.cmd-bash }}. The cmd-bash output is constructed from user-controlled inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only). Passing this value to eval allows shell metacharacters in user inputs to be interpreted as shell commands.

Locations:

- `action.yml:101`

### github-env-injection (severity: high)

The Python script in the 'cibw' step writes cmd-bash and cmd-pwsh values to $GITHUB_OUTPUT. These values are derived from user-controlled inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only) via the INPUT_* environment variables. The values are not sanitized with `printf '%s' ... | tr -d '\n\r'` before being written to $GITHUB_OUTPUT, allowing newline injection that could add arbitrary key=value pairs to the output file.

Locations:

- `action.yml:86`

## Iteration Notes

### Iteration 1

**Fixes applied:** script-injection, github-env-injection

**Notes:**

Fixed all four findings:

1. test.yml (line 183) - script-injection: Moved `${{ matrix.test_select || 'native' }}` and `${{ (runner.os == 'Linux' && runner.arch == 'X64') && '--run-podman' || '' }}` into the step's `env:` block as TEST_SELECT and RUN_PODMAN. The run block uses a bash array to safely construct the command.

2. update-dependencies.yml (line 47) - script-injection: Moved `${{ secrets.GITHUB_TOKEN }}` into the step's `env:` block as GITHUB_AUTH_TOKEN. The run block references it as a plain env var.

3. action.yml (line 101) - script-injection: Replaced `eval "$CIBW_CMD_BASH"` with a safe array-based execution. The Python script now writes each command argument on its own line using GITHUB_OUTPUT multiline syntax. The bash step uses `mapfile -t cibw_args <<< "$CIBW_CMD_ARGS"` to read args into an array and executes `"${cibw_args[@]}"` directly without eval.

4. action.yml (line 86) - github-env-injection: Added a `sanitize()` function in the Python script that strips \n and \r from all values before writing to $GITHUB_OUTPUT, preventing newline injection attacks.

