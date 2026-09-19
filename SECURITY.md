# Security

## Supported source

Security fixes target current `main` and, where appropriate, the latest published release. Superseded development snapshots should not be assumed to receive backports.

## Reporting

Do not publish a vulnerability that could expose users, vehicles, credentials, private diagnostic data, adapter secrets, build infrastructure or signing material in a public issue.

Use GitHub private vulnerability reporting/security advisories when available. Include:

- exact FORDLINK revision and LINK dependency identity;
- platform and adapter;
- vehicle/model/module context when relevant;
- impact and reliable reproduction;
- sanitised logs/captures;
- whether the issue requires a physical vehicle.

## Security-sensitive areas

Particular attention belongs to:

- diagnostic request allowlists and any change that broadens transmit capability;
- malformed ELM327/J2534/Bluetooth/vehicle responses;
- ISO-TP/UDS framing and length/bounds handling inherited through the product boundary;
- manufacturer-specific routing, sessions or procedures;
- local evidence/profile/configuration handling;
- Objective-C/Swift/native bridge lifetime and buffer boundaries;
- package/release/dependency identity.

## Automotive safety boundary

A successful build, unit test, simulator session or captured replay does not establish safe behaviour on every physical Ford vehicle.

Testing must not endanger people, vehicles or third-party systems. Potentially state-changing diagnostics require a stronger evidence and review bar than passive/read-only inventory.

## Response

Treat security defects as correctness defects. Reproduce at the narrowest safe layer, add regression coverage where practical, fix the owning contract and validate physical hardware when the failure crosses that boundary.

A defect in generic LINK behaviour should be fixed in LINK and then consumed by FORDLINK; do not hide it with a product-only workaround unless necessary as an explicit temporary mitigation.

## Disclosure

Public disclosure should follow a fix or clear mitigation and identify the affected and corrected source/dependency identities.