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

## Ford network model

FORDLINK now has an explicit Ford network taxonomy rather than treating every
module as one generic CAN responder. HS-CAN is modelled as the standard OBD
lane, MS-CAN as a separate enhanced-diagnostics lane requiring explicit adapter
support, and additional Ford CAN lanes are represented as profile-specific
networks whose physical details must come from verified vehicle evidence.

This structure was informed by observing how mature Ford diagnostic tools keep
vehicle networks, module identity, live data, tests and service procedures
separate. No proprietary FORScan code, module database, PID table or service
definition is included. See `docs/FORD_NETWORKS.md`.

## Ford manufacturer layer

FORDLINK contains a product-owned Ford module catalogue, diagnostic endpoint
candidates, a standards-based ECU identity DID catalogue, and a bounded
read-only manufacturer scanner. The scanner uses LINK's ECU-probe machinery and
safety policy: it can issue only UDS identity reads (0x22) and DTC inventory
(0x19). Reset, security access, routine control, DTC clearing, configuration
writes and programming remain blocked.

The data model also includes a generic Ford signal descriptor for byte/bit
offsets, scaling, units and Dashboard/Table/Graph presentation, plus a service
and test capability taxonomy. Those structures are data-driven so verified Ford
PIDs, DIDs, broadcast frames and procedures can be added without rewriting the
UI or transport engine.

The architecture is informed by observable behaviour in mature Ford diagnostic
software, including FORScan Lite NG, but FORDLINK does not contain FORScan's
native code, private database records, proprietary PID catalogue, security
algorithms or procedure command tables. See `docs/FORD_MODULES.md` and
`docs/FORD_DATA_MODEL.md`.
