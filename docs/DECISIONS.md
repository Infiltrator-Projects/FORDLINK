# Decisions

## ADR-001 — Ford network lanes are explicit

**Decision.** Model HS-CAN, MS-CAN and profile-specific additional networks separately.

**Rationale.** Enhanced Ford diagnostics are not reachable through one universal generic OBD path.

**Consequence.** Module discovery and procedures carry network identity and require adapter capability.

## ADR-002 — Generic diagnostic mechanics remain in LINK

**Decision.** ISO-TP, UDS, OBD, sequencing, safety and common application flow stay in LINK.

**Rationale.** Ford-specific capability should not fork standards behaviour.

**Consequence.** FORDLINK owns targets/definitions/interpretation, not the shared protocol engine.

## ADR-003 — Manufacturer scanning is bounded and read-only

**Decision.** The current Ford manufacturer scanner is limited to reviewed identity/DTC-style reads through LINK safety machinery.

**Rationale.** Broad address sweeps and service execution are not equivalent to harmless discovery.

**Consequence.** Adding a target or decoder does not grant new write/control services.

## ADR-004 — Signals and procedures are separate models

**Decision.** Live-data signals and service procedures do not share one undifferentiated manufacturer catalogue.

**Rationale.** Their safety, evidence and execution semantics differ.

**Consequence.** Procedure support can remain narrower than signal/identity support.

## ADR-005 — External Ford tools are architecture evidence only

**Decision.** Mature tools may inform information architecture but proprietary implementation/database content is not copied.

**Rationale.** The project needs independent, explainable source and provenance.

**Consequence.** Unsupported definitions stay absent instead of being imported without evidence.

## ADR-006 — Exact LINK dependency identity is tested

**Decision.** Product smoke testing records the expected LINK version from the pinned dependency.

**Rationale.** Manufacturer behaviour must be tied to the shared engine revision it was tested with.

**Consequence.** Dependency drift becomes visible during validation.
