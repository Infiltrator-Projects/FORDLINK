# Design

## First-principles position

FORDLINK models Ford diagnostics from the vehicle-network structure outward rather than treating every ECU as a generic OBD responder.

## Goals

- explicit Ford network lanes and adapter capability;
- Ford-specific module/identity knowledge separated from generic standards behaviour;
- bounded read-only manufacturer discovery;
- signals, procedures and service knowledge represented as distinct contracts;
- one shared LINK engine for transport, standards, sequencing and safety.

## Evidence policy

Mature Ford tools may reveal architectural ideas worth studying, but proprietary code, databases, PID tables, security algorithms and procedure payloads are not copied.

Manufacturer knowledge should be traceable to public documentation, verified capture or repeatable physical observation.

## Network safety

An MS-CAN or other enhanced lane must not be selected simply because a module is listed there. The adapter must prove the capability and the product must know how that vehicle/profile exposes the lane.

## Procedure safety

Procedure definitions have a higher evidence bar than read-only identity/live data. The project separates procedure metadata from signal metadata so one cannot silently grant the other.

## Shared ownership

If logic is generic across manufacturers, it belongs in LINK. Ford-specific interpretation remains here. This avoids a "Ford fork" of OBD/UDS while still allowing Ford's network model to be explicit.

## Failure policy

Unsupported lane switching, unknown module meaning, failed discovery and clean DTC state are separate conditions. The UI/data model must not collapse them.
