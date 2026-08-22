# Kern Testnet Testing

> **Testing status:** The wallet workflow documented here is a verified development workflow from our Linux setup. It is not a requirement of the Kern project.

This guide describes the documented Testnet transaction and PSBT workflow using the Kern desktop simulator.

Testnet should be used for wallet and transaction development whenever possible.

> **Important:** Testnet testing does not replace security review or testing on physical Kern hardware.

## 1. What This Tests

The Testnet workflow can be used to exercise:

- PSBT parsing
- Transaction review
- Fee handling
- Input and output handling
- QR encoding and decoding
- Signing workflow
- Wallet interoperability
- Simulator UI behavior

The documented wallet used for this workflow is Coconut Wallet. Coconut Wallet is **not a Kern project requirement**; it is an example wallet used to test Bitcoin transaction and PSBT interoperability.

## 2. Before You Start

You need:

- Kern desktop simulator
- A Testnet-compatible wallet
- Testnet transaction data
- A development machine that does not contain sensitive credentials

The documented wallet setup uses Coconut Wallet on an iPhone or iPad.

Never use real Bitcoin mainnet funds for simulator testing.

## 3. Configure Kern for Testnet

The simulator starts with Mainnet selected.

Start the simulator:

```bash
./simulator/build/kern_simulator
```

Then:

1. Open **Settings**.
2. Open **Network**.
3. Change **Mainnet** to **Testnet**.
4. Verify that Testnet is selected.

No source-code change is required to switch the running simulator between the documented networks.

### Important

Do not assume the simulator is still configured for Testnet because it was configured that way during a previous session.

Verify the network before every testing session.

## 4. Configure the Wallet

The documented Testnet PSBT workflow uses Coconut Wallet on an iPhone or iPad.

Before testing, verify:

- The wallet is configured for Testnet.
- Kern is configured for Testnet.
- The transaction is a Testnet transaction.
- No mainnet funds are involved.

The wallet and signer must be operating on the same Bitcoin network.

## 5. Understand the PSBT Workflow

The basic workflow is:

```text
Coconut Wallet
      |
      | Testnet transaction / PSBT
      v
   QR Code
      |
      v
Kern Simulator
      |
      | Review and sign
      v
 Signed PSBT
      |
      | QR Code
      v
Coconut Wallet
```

This lets the wallet and signer exchange a transaction without using real Bitcoin.

The exact QR interaction may change as Kern and the wallet software evolve.

## 6. Perform a Testnet Transaction Test

Follow these steps in order.

### Step 1 — Start Kern

Launch:

```bash
./simulator/build/kern_simulator
```

### Step 2 — Verify the Network

Open:

```text
Settings → Network
```

Select:

```text
Testnet
```

Verify the setting.

### Step 3 — Open the Wallet

Open Coconut Wallet on the iPhone or iPad.

Verify that it is also configured for Testnet.

### Step 4 — Prepare a Transaction

Create or prepare a Testnet transaction in the wallet.

Do not use a mainnet transaction.

### Step 5 — Export the PSBT

Export the transaction as a PSBT using the wallet's QR workflow.

### Step 6 — Import Into Kern

Use the Kern simulator's QR workflow to scan or import the PSBT.

### Step 7 — Review the Transaction

Review the transaction information shown by Kern.

Pay attention to the transaction details relevant to the test, including inputs, outputs, and fees.

### Step 8 — Sign

Sign the Testnet transaction in the simulator.

### Step 9 — Return the Signed PSBT

Use the appropriate Kern QR workflow to return the signed PSBT to the wallet.

### Step 10 — Verify

Verify the signed transaction in the wallet.

Do not treat a successful simulator transaction as proof of equivalent behavior on physical Kern hardware.

## 7. Testnet Safety Rules

Before signing:

- Verify the network.
- Verify that the transaction is Testnet.
- Verify the expected transaction details.
- Verify that no real funds are involved.

Never paste or enter:

- Bitcoin seed phrases
- Mainnet private keys
- SSH private keys
- GPG private keys
- Passwords
- Other sensitive credentials

into the simulator or development tools.

## 8. Repeating Tests

For repeatable development, record what is being tested.

A useful test note should identify:

```text
Date:
Kern commit:
Simulator build:
Network: Testnet
Wallet:
Test:
Expected result:
Actual result:
```

When a test reveals a meaningful bug or design issue, record the result in the project's appropriate development documentation.

## 9. Testnet Is Not Hardware Validation

A successful Testnet workflow establishes that the tested software path works in the simulated environment.

It does **not** establish:

- Hardware cryptographic equivalence
- Secure key storage
- Hardware-specific behavior
- Correct eFuse/HMAC behavior
- Equivalent physical-device security
- Overall production readiness

The distinction is:

```text
Testnet + Simulator
        =
software/workflow testing

Physical Kern hardware
        =
hardware validation
```

## 10. Troubleshooting

### Kern is on Mainnet

Open:

```text
Settings → Network
```

Change:

```text
Mainnet → Testnet
```

Verify before continuing.

### Wallet and Kern disagree about the network

Stop the test.

Verify both sides independently:

```text
Kern       → Testnet
Wallet     → Testnet
Transaction → Testnet
```

Do not continue until all three agree.

### PSBT QR import fails

Verify:

- The PSBT is valid.
- The QR data is complete.
- The correct QR workflow is being used.
- The simulator is running.
- The wallet is exporting the expected transaction format.

For simulator image-based QR testing, see [Simulator Development](simulator-development.md).

### Signed PSBT cannot be returned

Verify that:

- The transaction was actually signed.
- The simulator completed the signing workflow.
- The correct QR output workflow is being used.
- The wallet is expecting the corresponding signed PSBT format.

## 11. Test Record

For meaningful tests, preserve enough information to reproduce the result without preserving sensitive secrets.

Do not record seed phrases, private keys, or credentials.

Record:

- Repository commit
- Simulator build
- Testnet network state
- Wallet software used
- Test scenario
- Expected behavior
- Actual behavior
- Relevant error messages
- Reproduction steps

This makes Testnet testing useful as a development and debugging process rather than simply a manual demonstration.
