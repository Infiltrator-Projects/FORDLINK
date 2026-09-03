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

Manufacturer-specific behaviour must be evidence-backed rather than guessed or
copied from another brand.

## Dependency

The `src/link` gitlink pins the tested LINK release consumed by FORDLINK. The
product build uses LINK's shared implementation directly rather than copying
standard OBD-II, UDS, transport, Apple platform or generic diagnostic code into
FORDLINK-owned compatibility layers.

The product smoke test verifies the expected LINK version at build time, while
LINK's own CI owns regression coverage for the generic standards engine.

## Standard diagnostics

FORDLINK inherits its standards-based diagnostic foundation from LINK:
supported-PID discovery, current and freeze-frame data, readiness, VIN,
stored/pending/permanent DTC inventory, generic SAE DTC knowledge, OBDonUDS,
ISO-TP, UDS, KWP where applicable, adapter capability modelling, telemetry,
evidence and read-only safety policy.

Fault presentation also follows LINK's shared scan-state contract. An empty DTC
list is presented as clean only after the standard fault inventory completed
successfully; not-scanned, scanning and failed outcomes remain distinct.

## Ford network model

FORDLINK has an explicit Ford network taxonomy rather than treating every module
as one generic CAN responder. HS-CAN is modelled as the standard OBD lane,
MS-CAN as a separate enhanced-diagnostics lane requiring explicit adapter
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
