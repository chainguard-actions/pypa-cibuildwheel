<!-- markdownlint-disable -->

# Hardening Report: pypa--cibuildwheel/v4.3.2

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **pypa--cibuildwheel/v4.3.2** was hardened automatically. 1 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### github-env-injection (severity: high)

In action.yml, the 'cibw' step writes to $GITHUB_OUTPUT from Python: f.write(f"cmd-bash={cmd_bash}\n") and f.write(f"cmd-pwsh={cmd_pwsh}\n"). Both values come from untrusted inputs (inputs.package-dir, inputs.output-dir, inputs.config-file, inputs.only) passed in through the INPUT_* env vars. Newlines and carriage returns are not removed before the write. shlex.join and pwsh_quote handle shell quoting but leave newlines in, so an input with a newline can inject extra step outputs, such as a forged cmd-bash that the next step runs with eval. Remove \n and \r from each value before writing, which is the same job as the required printf '%s' ... | tr -d '\n\r' step, or reject inputs that contain them.

Locations:

- `action.yml:46`
- `action.yml:122`

## Iteration Notes

### Iteration 1

**Fixes applied:** github-env-injection

**Notes:**

In the cibw step of action.yml I added a small Python helper, strip_newlines, that removes \n and \r from a value. The step now runs it on prepend-path, cmd-bash and cmd-pwsh before writing them to $GITHUB_OUTPUT. A newline in an input can no longer add extra outputs, such as a fake cmd-bash that the next step would run with eval. Nothing was run to test this. Input values without newlines produce exactly the same command lines as before. An input containing a newline now has those characters dropped from the command line rather than causing the step to fail.

