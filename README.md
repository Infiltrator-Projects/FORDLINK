# FORDLINK

FORDLINK is the Ford-specific member of the LINK diagnostic family.

## Architecture

FORDLINK owns only Ford-specific diagnostic behaviour: vehicle/profile
selection, ECU/module knowledge, proprietary identifiers, manufacturer DTC
knowledge, addressing, security/session policy and brand-specific workflows.

Reusable transport and protocol code belongs in
[LINK](https://github.com/Infiltrator-Projects/LINK): CAN/CAN-FD, ISO-TP,
OBD-II/J1979, UDS, KWP where shared, diagnostic flow, adapters/transports and
portable platform support.

This repository deliberately starts small. Manufacturer-specific behaviour
must be evidence-backed rather than guessed or copied from another brand.

## Dependency

The `src/link` gitlink pins a tested LINK release. Product code must consume
that pin rather than duplicate LINK sources.

## Baseline functionality

Even before Ford-specific definitions are added, FORDLINK is already a
functional **standard OBD-II core** because it consumes the pinned LINK
diagnostic engine.

That baseline includes standard supported-PID discovery, current-data PID
request construction and decoding, freeze-frame reads, readiness, VIN,
stored/pending/permanent DTC reads, generic SAE DTC decoding/knowledge, and
LINK's standard OBDonUDS mapping. See `docs/OBD2.md` and `docs/GENERIC_BASELINE.md`.

CI exercises this inherited OBD path directly so FORDLINK cannot silently
become a brand shell that no longer exposes LINK's standard diagnostics.

## Status

Generic diagnostic baseline: **available through LINK** — adapter capability modelling, ELM327 sessions/CAN, standard OBD-II, ISO-TP, UDS, DoIP framing, read-only discovery safety, parameters/scheduling, telemetry/evidence and transport-neutral diagnostic requests.

Ford-specific/proprietary vehicle coverage: not claimed yet; it will be added
here only from evidence-backed Ford data.
