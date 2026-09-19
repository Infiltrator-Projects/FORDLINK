# Validation

## Automated coverage

FORDLINK has direct C regression tests for:

- product/dependency smoke integration;
- Ford network model;
- module catalogue/identity;
- manufacturer module scan;
- signal model;
- procedure model.

These tests sit above LINK's own protocol, transport and safety suite.

## Layered evidence

A passing FORDLINK unit test proves product-model behaviour. LINK tests prove shared transport/protocol contracts. Neither alone proves an MS-CAN switch or module response on a physical Ford vehicle.

## Network validation

Enhanced-network support requires three independent facts:

1. the vehicle/profile actually uses the lane;
2. the adapter can access/switch to the lane;
3. the request/target is appropriate on that lane.

The data model may represent a network before all three are physically qualified, but the product must not present unqualified access as working.

## Procedure validation

Procedures require explicit preconditions, request semantics, expected response/failure handling and safety classification. Presence in a catalogue is not execution proof.

## Physical evidence

Real Ford vehicles and adapters remain required for module/network qualification. Captured traces may become regression fixtures once provenance and sanitisation are acceptable.

## Release criterion

The exact FORDLINK source and LINK dependency must pass product and shared CI. Documentation must distinguish modelled, test-covered and physically verified capability.
