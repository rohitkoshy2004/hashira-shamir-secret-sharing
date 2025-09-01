
// Shamir's Secret Sharing Solution in JavaScript
// Run with: node ShamirSecretSharing.js

class BigNumber {
    constructor(value) {
        if (typeof value === 'string') {
            this.value = BigInt(value);
        } else if (typeof value === 'number') {
            this.value = BigInt(value);
        } else if (typeof value === 'bigint') {
            this.value = value;
        } else {
            this.value = BigInt(0);
        }
    }

    add(other) {
        return new BigNumber(this.value + other.value);
    }

    subtract(other) {
        return new BigNumber(this.value - other.value);
    }

    multiply(other) {
        return new BigNumber(this.value * other.value);
    }

    divide(other) {
        return new BigNumber(this.value / other.value);
    }

    toString() {
        return this.value.toString();
    }

    toNumber() {
        return Number(this.value);
    }
}

function convertFromBase(value, base) {
    let result = new BigNumber(0);
    let power = new BigNumber(1);
    const baseValue = new BigNumber(base);

    for (let i = value.length - 1; i >= 0; i--) {
        const digit = value[i];
        let digitValue;

        if (digit >= '0' && digit <= '9') {
            digitValue = digit.charCodeAt(0) - '0'.charCodeAt(0);
        } else if (digit >= 'a' && digit <= 'z') {
            digitValue = digit.charCodeAt(0) - 'a'.charCodeAt(0) + 10;
        } else if (digit >= 'A' && digit <= 'Z') {
            digitValue = digit.charCodeAt(0) - 'A'.charCodeAt(0) + 10;
        } else {
            throw new Error(`Invalid digit: ${digit}`);
        }

        if (digitValue >= base) {
            throw new Error(`Digit ${digit} is invalid for base ${base}`);
        }

        result = result.add(new BigNumber(digitValue).multiply(power));
        power = power.multiply(baseValue);
    }

    return result;
}

function lagrangeInterpolation(points) {
    let result = 0;
    const n = points.length;

    for (let i = 0; i < n; i++) {
        const xi = points[i].x;
        const yi = points[i].y;

        let li = 1;
        for (let j = 0; j < n; j++) {
            if (i !== j) {
                const xj = points[j].x;
                li *= (0 - xj) / (xi - xj);
            }
        }

        result += yi * li;
    }

    return Math.round(result);
}

function solveShamirSecretSharing(testCase) {
    const { keys, ...pointsData } = testCase;
    const { n, k } = keys;

    console.log(`n = ${n}, k = ${k}`);
    console.log(`Need ${k} points to reconstruct polynomial of degree ${k-1}`);

    const points = [];

    // Parse points from test case
    for (const [key, pointData] of Object.entries(pointsData)) {
        const x = parseInt(key);
        const base = parseInt(pointData.base);
        const value = pointData.value;

        const y = convertFromBase(value, base);
        points.push({
            x: x,
            y: y.toNumber(),
            originalBase: base,
            originalValue: value
        });

        console.log(`Point ${key}: x=${x}, y=${y.toString()} (from base ${base} value '${value}')`);
    }

    // Sort points by x coordinate
    points.sort((a, b) => a.x - b.x);

    // Take first k points for reconstruction
    const selectedPoints = points.slice(0, k);
    console.log(`\nSelected ${selectedPoints.length} points for reconstruction:`);
    selectedPoints.forEach(p => console.log(`(${p.x}, ${p.y})`));

    // Find the secret using Lagrange interpolation
    const secret = lagrangeInterpolation(selectedPoints);

    console.log(`\nSecret (constant term): ${secret}`);

    return secret;
}

// Test case 1
const testCase1 = {
    "keys": {
        "n": 4,
        "k": 3
    },
    "1": {
        "base": "10",
        "value": "4"
    },
    "2": {
        "base": "2", 
        "value": "111"
    },
    "3": {
        "base": "10",
        "value": "12"
    },
    "6": {
        "base": "4",
        "value": "213"
    }
};

// Test case 2
const testCase2 = {
    "keys": {
        "n": 10,
        "k": 7
    },
    "1": {
        "base": "6",
        "value": "13444211440455345511"
    },
    "2": {
        "base": "15",
        "value": "aed7015a346d635"
    },
    "3": {
        "base": "15", 
        "value": "6aeeb69631c227c"
    },
    "4": {
        "base": "16",
        "value": "e1b5e05623d881f"
    },
    "5": {
        "base": "8",
        "value": "316034514573652620673"
    },
    "6": {
        "base": "3",
        "value": "2122212201122002221120200210011020220200"
    },
    "7": {
        "base": "3",
        "value": "20120221122211000100210021102001201112121"
    },
    "8": {
        "base": "6",
        "value": "20220554335330240002224253"
    },
    "9": {
        "base": "12",
        "value": "45153788322a1255483"
    },
    "10": {
        "base": "7",
        "value": "1101613130313526312514143"
    }
};

console.log("=== Test Case 1 ===");
const secret1 = solveShamirSecretSharing(testCase1);

console.log("\n=== Test Case 2 ===");
const secret2 = solveShamirSecretSharing(testCase2);

console.log("\n=== Results ===");
console.log(`Test Case 1 Secret: ${secret1}`);
console.log(`Test Case 2 Secret: ${secret2}`);
