# Architecture

## Purpose

FORDLINK is the Ford manufacturer product face over LINK, owning Ford network taxonomy, module identity, verified enhanced-diagnostics knowledge and manufacturer-specific interpretation.

## System decomposition

- Ford product facade
- exact LINK dependency
- Ford network/module/signal/procedure model
- Linux/iPhone/Windows-facing product surfaces
- manufacturer scan and product tests

## Ownership boundaries

LINK owns generic transports, OBD, UDS, sequencing, safety and common application behaviour. FORDLINK owns Ford-specific network lanes, module identities, definitions and verified enhanced diagnostics.

Platform APIs, hosted services, research sources and first-party shared libraries provide mechanisms or evidence behind explicit boundaries. They do not silently own the product's interpretation or policy.

## Source of truth

Executable behaviour is defined by code and tests. This document defines architectural ownership and dependency direction. Specialist documents refine narrower domains and must remain consistent with it.

## Change discipline

Keep platform handles/toolkit details out of domain contracts where practical. Keep generic behaviour in its shared owner rather than copying it. Preserve explicit unavailable/unsupported/failure states across layers.

## Specialist documentation

- docs/FORD_DATA_MODEL.md
- docs/FORD_MODULES.md
- docs/FORD_NETWORKS.md
