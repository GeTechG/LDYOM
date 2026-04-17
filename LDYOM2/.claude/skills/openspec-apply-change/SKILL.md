---
name: openspec-apply-change
description: Implement tasks from an OpenSpec change. Use when the user wants to start implementing, continue implementation, or work through tasks.
license: MIT
compatibility: Requires openspec CLI.
metadata:
  author: openspec
  version: "1.1"
  generatedBy: "1.3.0"
---

Implement tasks from an OpenSpec change.

**Execution model** — This skill is an **orchestrator**. It never edits application code directly. Each task (or a small batch of tightly-related tasks) is delegated to a `general-purpose` subagent running on **Sonnet** via `Agent({ subagent_type: "general-purpose", model: "sonnet", ... })`. The orchestrator keeps the main context minimal (only `tasks.md`, `session.md`, and short subagent summaries) so implementation can run far longer than a single-context session.

**Input**: Optionally specify a change name. If omitted, try to infer from conversation context. If vague or ambiguous you MUST prompt for available changes.

---

## Steps

### 1. Select the change

If a name is provided, use it. Otherwise:
- Infer from conversation context if the user mentioned a change
- Auto-select if only one active change exists
- If ambiguous, run `openspec list --json` and use the **AskUserQuestion tool** to let the user choose

Announce: `Using change: <name>` and how to override (e.g., `/opsx:apply <other>`).

### 2. Check status / schema

```bash
openspec status --change "<name>" --json
```

Parse: `schemaName`, which artifact holds the tasks (typically `tasks`).

### 3. Get apply instructions

```bash
openspec instructions apply --change "<name>" --json
```

Handle states:
- `blocked` (missing artifacts) → show message, suggest re-running `/opsx:propose` for the missing artifact
- `all_done` → congratulate, suggest `/opsx:archive`
- otherwise → proceed

### 4. Resume or start a session

Check for `openspec/changes/<name>/session.md`:

- **Exists** → read it. If `status: paused`, report the last in-flight task + any open blockers/questions and ask the user whether to resume or restart. If `status: active`, treat as a crash-recovery; resume from the in-flight task.
- **Does not exist** → create a fresh one using the [Session State Schema](#session-state-schema) below.

### 5. Read minimal context

Into the **orchestrator's** context, read only:
- `openspec/changes/<name>/tasks.md`
- `openspec/changes/<name>/session.md`

Do NOT eagerly read `proposal.md`, `design.md`, or any spec files — those are passed to the per-task subagent as paths.

### 6. Show current progress

Display:
- Schema name
- Progress: `N/M tasks complete`
- Remaining tasks (brief list)
- Session status (fresh | resuming — show last in-flight task if any)

### 7. Per-task subagent loop

For each pending task (or tightly related batch):

#### 7a. Plan the subagent call

- Identify which context files the task needs (from task text — e.g., a task about "update the X schema" needs the spec covering X).
- **Isolate large tasks**: if the task text implies cross-cutting work (`refactor across`, `rewrite module`, `touches many files`, explicit file-count hints), dispatch it alone.
- **Batch small tasks**: otherwise group up to 2-3 tightly-related simple edits (same capability, same file, same concept).
- Never batch tasks from different spec capabilities.

#### 7b. Dispatch the subagent

```
Agent({
  subagent_type: "general-purpose",
  model: "sonnet",
  description: "Apply task <id>",
  prompt: <self-contained prompt, see template below>
})
```

**Prompt template** (the subagent starts with no prior history — the prompt must be fully self-contained):

```
You are implementing a single task (or small batch) from OpenSpec change `<name>`.

## Task(s) to implement
- [ ] <verbatim task text from tasks.md, preserving IDs>

## Context files (read these first)
- openspec/changes/<name>/proposal.md
- openspec/changes/<name>/design.md            (if exists)
- openspec/changes/<name>/specs/<capability>/spec.md   (only the capabilities this task touches)
- <any codebase files the task text clearly implicates>

## Session notes from prior subagents
<paste the `## Decisions` and `## Gotchas` sections from session.md, if non-empty; otherwise "none">

## What to do
1. Read the context files listed above.
2. Make the minimal code changes required by the task — nothing else.
3. Do NOT modify tasks.md (the orchestrator owns the checkbox).
4. Report back under 250 words with exactly these fields:
   - **changed**: list of files touched (path + one-line summary each)
   - **decisions**: new decisions you made that are NOT yet in design.md (empty list if none)
   - **gotchas**: surprises the next subagent or user needs to know (empty list if none)
   - **blockers**: anything that prevented completion (empty list if none)
   - **context_pressure**: `ok` | `tight` | `exhausted` — your honest read of how much headroom you have left
```

#### 7c. Handle the subagent result

- **Success** (no blockers) → mark the task(s) as `- [x]` in `tasks.md`. Append any `decisions` and `gotchas` into the matching sections of `session.md`, tagged with the task ID.
- **Blocker reported** → do NOT mark done. Write the blocker into `session.md` `## Open Questions` and jump to the [pause protocol](#9-pause-protocol).
- **`context_pressure: tight` or `exhausted`** → finish updating `session.md`, then trigger the pause protocol.

#### 7d. Periodic checkpoint

Every 3-5 tasks (or whenever the subagent returns a large `decisions`/`gotchas` batch), flush `session.md` — ensure `## In-Flight` is empty when idle, and `## Next Steps` points to the next 1-2 pending tasks.

### 8. Continue until

- All tasks are `- [x]` → set `session.md` `status: completed`, congratulate, suggest `/opsx:archive`
- Pause triggered → see section 9

### 9. Pause protocol

When pause is triggered (context pressure, blocker, or user interrupt):

1. **Finalize `session.md`**:
   - `status: paused`
   - `## In-Flight` = the task that was started-but-not-finished (or `none`)
   - Flush all accumulated `decisions` / `gotchas` / `open questions`
   - `## Next Steps` = IDs + one-line description of the next 1-2 pending tasks
2. **Verify `tasks.md`** checkboxes match what the subagents actually finished.
3. **Emit this exact message to the user**:

   ```
   ## Implementation Paused

   **Change:** <change-name>
   **Progress:** N/M tasks complete
   **Reason:** <context tight | subagent blocker | user interrupt>

   State saved to:
   - openspec/changes/<name>/tasks.md
   - openspec/changes/<name>/session.md

   To resume with a fresh context:
   1. Run /clear
   2. Invoke /opsx:apply <change-name>

   The skill will re-read session.md and continue from the next pending task.
   ```

---

## Session State Schema

File: `openspec/changes/<name>/session.md`

This file captures **only what `tasks.md` and `design.md` cannot**. It is append-mostly during a session, compacted at each pause, and archived along with the change.

```markdown
---
change: <name>
updated: <ISO-8601 UTC timestamp>
status: active | paused | completed
---

## In-Flight
<task-id or "none"> — <what was partially done, if any>

## Decisions
<decisions made during implementation that are NOT yet in design.md; each tagged with the task that produced it>
- [task <id>] <decision>

## Gotchas
<unexpected findings the next subagent or user needs to know>
- <gotcha>

## Open Questions
<questions for the user that block progress until answered>
- <question>

## Next Steps
1. <next task id + short description>
2. <next task id + short description>
```

**Schema rules**:
- Keep each bullet terse — max 2 lines
- Do NOT duplicate `tasks.md` (no listing of done tasks)
- Do NOT duplicate `design.md` (only decisions that have not yet been migrated there)
- When a decision lands in `design.md`, remove it from `session.md`

---

## Guardrails

- **Never read `proposal.md` / `design.md` / spec files into the orchestrator's context** — always pass paths to the subagent.
- **Never make code edits from the orchestrator** — always delegate via `Agent`.
- If a task is ambiguous, pause immediately (step 9) and ask the user before dispatching any subagent.
- Mark tasks complete in `tasks.md` ONLY after the subagent returns success AND you have parsed its summary.
- If the subagent's summary shows scope creep (touched files unrelated to the task), flag in `session.md` `## Gotchas` and consider asking the user whether to revert.
- Every pause must leave `session.md` + `tasks.md` in a state where a fresh-context resume works mechanically.
- Do NOT try to precompute a "context budget" — trust the subagent's `context_pressure` flag.
