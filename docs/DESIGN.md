# Design

## First-principles position

FORDLINK starts from the behaviour it must own. Existing products, research, provider APIs and tools are studied as evidence, then accepted, changed or rejected according to the needs of this project.

## Goals

- model Ford network lanes explicitly rather than pretending all modules sit on one generic bus
- keep manufacturer actions evidence-backed and bounded
- reuse LINK for every genuinely generic behaviour
- separate data, tests and procedures so one kind of evidence cannot silently imply another

## Non-goals

FORScan or other mature tools may inform architecture but their proprietary code/databases are not copied. Generic OBD support does not imply enhanced Ford coverage.

## Dependency and language policy

Prefer first-party C/C++ implementation for native/core behaviour where suitable. Use platform-native services where they provide a stronger documented contract. A dependency or external source must not become an undocumented source of semantic truth.

## Failure and uncertainty

Unavailable, unsupported, uncertain and failed are distinct. Prefer visible uncertainty or refusal to guessed success. Persistent or destructive operations require explicit preconditions and post-verification appropriate to their risk.

## Decision quality

A change should improve correctness, safety, fidelity, performance, usability or maintainability and include a validation method. Newness alone is not a design argument.
