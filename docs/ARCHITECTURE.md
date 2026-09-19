# Architecture

## Purpose

FORDLINK is the Ford manufacturer product face over LINK. It is deliberately richer than the current BMW/Audi faces because the repository already owns a Ford-specific network, module, signal and procedure model.

## Dependency hierarchy

```text
Infiltratr Common
        ↓
       LINK
        ↓
    FORDLINK
```

`fordlink-core` links publicly against `LINK::Core`. Generic transport, standards, diagnostic sequencing, safety and shared application behaviour remain in LINK.

## Ford product core

The product core is split by responsibility:

- `src/network.c` — Ford network/lane model;
- `src/module.c` — module catalogue and identity;
- `src/identity.c` — product/vehicle identity helpers;
- `src/module_scan.c` — bounded manufacturer module probing;
- `src/signal.c` — Ford-specific signal metadata/model;
- `src/procedure.c` — Ford procedure metadata/contract;
- `src/fordlink.c` — product facade/version identity.

This separation prevents a growing manufacturer layer from turning into one undifferentiated database.

## Network model

FORDLINK distinguishes HS-CAN, MS-CAN and additional profile-specific Ford network lanes. Standard OBD activity uses the normal LINK/OBD lane. Enhanced lanes require explicit adapter capability and verified physical routing; the data model does not pretend one generic CAN path reaches every Ford module.

## Module discovery

The manufacturer scanner uses LINK's shared ECU-probe/safety machinery. Ford-specific target candidates and interpretation stay here.

A module catalogue entry or known endpoint does not itself authorize every possible service. Discovery and procedure permissions remain bounded.

## Signals and procedures

Signals and procedures are separate domains. A decoded live-data item does not imply a service procedure is safe; a documented procedure does not make every parameter writable.

## Platform faces

Linux GTK4, LINK-shared Windows Discover and native iPhone presentation all consume FORDLINK::Core. Product identity remains local; shared application mechanics remain in LINK.

## Version identity

Root VERSION and the public FORDLINK version are checked for agreement. The smoke test also records the pinned LINK version, making dependency drift visible.

## Tests

Dedicated tests cover network modelling, module catalogue, module scanning, signals, procedures and product smoke/dependency integration.
