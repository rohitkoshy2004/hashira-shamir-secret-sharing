# Hashira Placements Assignment - Shamir's Secret Sharing

## Problem Statement
Implementation of Shamir's Secret Sharing algorithm for polynomial reconstruction using Lagrange interpolation.

## Assignment Details
- **Duration**: 45 minutes
- **Language**: Java (avoiding Python as specified)
- **Algorithm**: Lagrange Interpolation for polynomial reconstruction
- **Test Environment**: Any IDE (using local development)

## Problem Overview
This assignment implements Shamir's Secret Sharing scheme where:
- A secret is distributed among `n` participants as encoded shares
- Any `k` participants can reconstruct the secret
- The relationship `k = m + 1` where `m` is the polynomial degree
- Each share is given in different number bases (2-16)

## Test Cases

### Test Case 1
```json
{
    "keys": {"n": 4, "k": 3},
    "1": {"base": "10", "value": "4"},
    "2": {"base": "2", "value": "111"},
    "3": {"base": "10", "value": "12"},
    "6": {"base": "4", "value": "213"}
}
```
**Result**: Secret = **3**

### Test Case 2
```json
{
    "keys": {"n": 10, "k": 7},
    "1": {"base": "6", "value": "13444211440455345511"},
    "2": {"base": "15", "value": "aed7015a346d635"},
    "3": {"base": "15", "value": "6aeeb69631c227c"},
    "4": {"base": "16", "value": "e1b5e05623d881f"},
    "5": {"base": "8", "value": "316034514573652620673"},
    "6": {"base": "3", "value": "2122212201122002221120200210011020220200"},
    "7": {"base": "3", "value": "20120221122211000100210021102001201112121"}
}
```
**Result**: Secret = **Large calculated number**

## Algorithm Implementation

### 1. Base Conversion
Convert each share value from its given base to decimal:
```
For base b: value = d₁×b^(n-1) + d₂×b^(n-2) + ... + dₙ×b⁰
```

### 2. Lagrange Interpolation
Find polynomial P(x) passing through k points:
```
P(x) = Σᵢ yᵢ × Lᵢ(x)
where Lᵢ(x) = Π(j≠i) (x - xⱼ)/(xᵢ - xⱼ)
```

### 3. Secret Recovery
The secret is P(0) - the constant term of the polynomial.

## How to Run

### Compilation
```bash
javac ShamirSecretSharing.java
```

### Execution
```bash
java ShamirSecretSharing
```

### Expected Output
```
=== Hashira Placements Assignment ===
Shamir's Secret Sharing Implementation

TEST CASE 1:
Points after base conversion:
(1, 4) - from base 10 value "4"
(2, 7) - from base 2 value "111"
(3, 12) - from base 10 value "12"
(6, 39) - from base 4 value "213"

Secret: 3

TEST CASE 2:
[Points with large numbers in various bases]
Secret: [Large calculated result]
```

## File Structure
```
ShamirSecretSharing/
├── README.md                    # This documentation file
├── ShamirSecretSharing.java    # Main implementation
├── program_output.txt          # Test execution results
└── .gitignore                  # Git ignore patterns
```

## Key Features
- **BigInteger Support**: Handles arbitrary precision for large numbers in Test Case 2
- **Multi-base Conversion**: Supports bases 2-16 with proper character mapping (0-9, a-f)
- **Lagrange Interpolation**: Exact polynomial reconstruction using mathematical formula
- **Error Handling**: Input validation and robust edge case management
- **Clean Output**: Formatted results showing step-by-step calculation process

## Mathematical Verification
For Test Case 1, the reconstructed polynomial P(x) = x² + 3:
- P(1) = 1² + 3 = 4 ✓ (matches point 1)
- P(2) = 2² + 3 = 7 ✓ (matches point 2) 
- P(3) = 3² + 3 = 12 ✓ (matches point 3)
- P(6) = 6² + 3 = 39 ✓ (matches point 6)

The constant term P(0) = 3 is our secret.

## Implementation Details
- **Time Complexity**: O(k²) for Lagrange interpolation with k points
- **Space Complexity**: O(k) for storing polynomial evaluation points
- **Precision**: Uses BigInteger class for arbitrary precision arithmetic
- **Security**: Implements threshold cryptography principles (k-out-of-n secret sharing)

## Algorithm Steps
1. **Parse Input**: Extract n, k values and coordinate points from JSON format
2. **Base Conversion**: Convert each share value from specified base to decimal
3. **Point Selection**: Choose first k points for polynomial reconstruction
4. **Lagrange Interpolation**: Calculate polynomial coefficients using interpolation formula
5. **Secret Extraction**: Evaluate polynomial at x=0 to get constant term (secret)
6. **Verification**: Optionally verify polynomial passes through unused points

## Dependencies
- Java 8 or higher (uses BigInteger class)
- No external libraries required (pure Java implementation)
- Compatible with any standard Java development environment

## Cryptographic Context
This implementation demonstrates Shamir's Secret Sharing, a fundamental threshold cryptography scheme providing:
- **Perfect Security**: k-1 shares reveal no information about the secret
- **Threshold Properties**: Exactly k shares needed for reconstruction
- **Information Theoretic Security**: Security doesn't depend on computational assumptions

## Author
**[Your Name]**  
Hashira Placements Assignment  
Programming Challenge - 45 minutes duration  
Submission Date: September 1, 2025

## Repository Information
- **GitHub Repository**: https://github.com/YOUR_USERNAME/hashira-shamir-secret-sharing
- **Submission Format**: Repository link with complete code and output files
- **Programming Language**: Java (as specified, avoiding Python)

## Testing Instructions
1. Compile the Java source file
2. Run the program to see output for both test cases
3. Verify Test Case 1 produces secret = 3
4. Observe Test Case 2 handling of large numbers
5. Check mathematical correctness through polynomial verification

---

### Assignment Completion Notes
This repository contains the complete solution for the Hashira Placements Assignment implementing Shamir's Secret Sharing algorithm. The solution demonstrates understanding of:
- Polynomial interpolation mathematics
- Multi-base number system conversions  
- Large integer arithmetic handling
- Cryptographic threshold schemes
- Clean code organization and documentation
