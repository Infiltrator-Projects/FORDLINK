# FORDLINK

**Project copyright:** © 1993-2026 Shannon Smith

FORDLINK is the Ford-specific member of the LINK diagnostic family.

## Engineering ethos

What should a Ford diagnostic product built from first principles own when Ford network behaviour extends beyond the generic OBD lane? FORDLINK turns Ford-specific network, module and diagnostic evidence into product behaviour on top of the exact shared LINK engine.

LINK owns standards-based transport, OBD, UDS, sequencing, safety and common application behaviour. FORDLINK owns Ford network taxonomy, module identities, verified enhanced-diagnostic definitions and manufacturer-specific interpretation. Existing tools and public material are evidence to learn from, not code or proprietary databases to depend on or imitate blindly; unsupported behaviour remains explicitly unsupported.

The project prefers evidence-backed, explainable behaviour over novelty. Generic fixes are made once in LINK, and Ford-specific capability is added only when its network assumptions, safety boundary and expected responses can be tested.

## Architecture

FORDLINK owns only Ford-specific diagnostic behaviour: vehicle/profile
selection, ECU/module knowledge, proprietary identifiers, manufacturer DTC
knowledge, addressing, security/session policy, branding and brand-specific
workflows.

LINK is the shared automotive application engine for the family, not only a
protocol library. It owns reusable diagnostic behaviour and common application
infrastructure, including CAN/CAN-FD, ISO-TP, OBD-II/J1979, generic DTC
knowledge, UDS, KWP where shared, diagnostic flow, adapters/transports,
portable platform support, the operator-task information architecture and
shared presentation behaviour that should remain consistent across product
faces.

Ford network, module, signal, test and service knowledge feeds that shared task
model; FORDLINK must not fork generic navigation, diagnostic sequencing or
common application behaviour merely to present Ford-specific content.

Manufacturer-specific behaviour must be evidence-backed rather than guessed or
copied from another brand.

## Dependency

The `src/link` gitlink pins the tested LINK release consumed by FORDLINK. The
product build uses LINK's shared implementation directly rather than copying
standard OBD-II, UDS, transport, Apple platform or generic diagnostic/application
code into FORDLINK-owned compatibility layers.

The product smoke test verifies the expected LINK version at build time, while
LINK's own CI owns regression coverage for the generic standards and shared
application engine.

## Standard diagnostics

FORDLINK inherits its standards-based diagnostic foundation and common
application model from LINK: supported-PID discovery, current and freeze-frame
data, readiness, VIN, stored/pending/permanent DTC inventory, generic SAE DTC
knowledge, OBDonUDS, ISO-TP, UDS, KWP where applicable, adapter capability
modelling, telemetry, evidence, read-only safety policy and the shared
operator-task information architecture.

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
