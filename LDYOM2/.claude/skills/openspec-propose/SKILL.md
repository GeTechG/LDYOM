---
name: openspec-propose
description: Propose a new change with all artifacts generated in one step. Use when the user wants to quickly describe what they want to build and get a complete proposal with design, specs, and tasks ready for implementation.
license: MIT
compatibility: Requires openspec CLI.
metadata:
  author: openspec
  version: "1.1"
  generatedBy: "1.3.0"
---

Propose a new change — create the change and generate all artifacts in one step.

**Execution model** — Artifact drafting is research-heavy (proposal/design/specs require reasoning about the codebase). Each artifact is delegated to a `general-purpose` subagent running on **Opus** via `Agent({ subagent_type: "general-purpose", model: "opus", ... })`. This skill itself is the orchestrator: it manages the artifact build order, handles user interrupts (`AskUserQuestion`), and verifies outputs. It should not draft artifact content directly.

I'll create a change with artifacts:
- `proposal.md` (what & why)
- `design.md` (how)
- `tasks.md` (implementation steps)

When ready to implement, run `/opsx:apply`.

---

## Steps

### 1. Get the user's intent

If a clear description wasn't provided, use the **AskUserQuestion tool** (open-ended, no preset options) to ask:

> "What change do you want to work on? Describe what you want to build or fix."

From the description, derive a kebab-case name (e.g., "add user authentication" → `add-user-auth`).

**IMPORTANT**: Do NOT proceed without understanding what the user wants to build.

### 2. Create the change directory

```bash
openspec new change "<name>"
```

If a change with that name already exists, ask whether to continue it (switch to `/opsx:apply`) or pick a new name.

### 3. Get the artifact build order

```bash
openspec status --change "<name>" --json
```

Parse:
- `applyRequires` — artifact IDs needed before implementation
- `artifacts` — status + dependencies for each

### 4. Create each artifact via an Opus subagent

Use the **TaskCreate tool** to track progress through the artifact list.

Loop artifacts in dependency order (those whose dependencies are `done` first):

#### 4a. Fetch instructions for the artifact

```bash
openspec instructions <artifact-id> --change "<name>" --json
```

This returns `context`, `rules`, `template`, `instruction`, `outputPath`, `dependencies`.

#### 4b. Dispatch the subagent

```
Agent({
  subagent_type: "general-purpose",
  model: "opus",
  description: "Draft <artifact-id>",
  prompt: <self-contained prompt, see template below>
})
```

**Prompt template** — the subagent has no prior history, so pass everything it needs inline:

```
You are drafting the `<artifact-id>` artifact for OpenSpec change `<name>`.

## User's original request
<verbatim description the user gave>

## Instructions payload (from `openspec instructions <artifact-id> --change "<name>" --json`)
<paste the entire JSON output here>

## Dependency artifacts (read these first for context)
<list of file paths taken from the `dependencies` field>

## What to do
1. Read each dependency file.
2. Follow the `template` field as the structure for the output file.
3. Apply `context` and `rules` as constraints — do NOT copy those sections into the file.
4. Write the artifact to `outputPath`.
5. If something is critically unclear, do NOT loop — write a best-effort draft with a TODO marker and list the question in your summary.
6. Report back under 200 words with exactly these fields:
   - **wrote**: the file path you wrote
   - **assumptions**: assumptions you made to proceed (empty list if none)
   - **needs_clarification**: questions the orchestrator should ask the user (empty list if none)
```

#### 4c. Handle the subagent result

- If `needs_clarification` is non-empty → use **AskUserQuestion tool** with those questions, then dispatch the subagent again with a prompt appendix containing the user's answers. Do NOT merge user answers into the orchestrator's own reasoning — always pass them to the subagent.
- Otherwise → verify `outputPath` exists on disk before proceeding.

#### 4d. Re-check status

```bash
openspec status --change "<name>" --json
```

Continue the loop. Stop when every ID in `applyRequires` has `status: "done"`.

### 5. Show final status

```bash
openspec status --change "<name>"
```

---

## Output

After all artifacts are created, summarize:
- Change name and location
- Artifacts created (one line each)
- `All artifacts created! Ready for implementation.`
- `Run /opsx:apply or ask me to implement to start working on the tasks.`

## Guardrails

- **Never draft artifact content in the orchestrator** — always delegate.
- Always pass dependency files as paths, not inline content.
- If the subagent reports `needs_clarification`, resolve via `AskUserQuestion` before redispatching — never let artifacts drift on silent assumptions.
- Verify each artifact file exists on disk after the subagent returns.
- If a change with that name already exists, ask whether to continue it or create a new one.
- Do NOT copy `context` / `rules` / `<project_context>` blocks into artifact files — they guide drafting, they are not content.
