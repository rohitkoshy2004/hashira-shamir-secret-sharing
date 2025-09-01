package ShamirSecretSharing;

import java.math.BigInteger;
import java.util.*;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

public class ShamirSecretSharing {

    public static class Point {
        public BigInteger x;
        public BigInteger y;

        public Point(BigInteger x, BigInteger y) {
            this.x = x;
            this.y = y;
        }
    }

    // Convert a number from given base to decimal using BigInteger
    public static BigInteger convertFromBase(String value, int base) {
        BigInteger result = BigInteger.ZERO;
        BigInteger baseValue = BigInteger.valueOf(base);
        BigInteger power = BigInteger.ONE;

        // Process digits from right to left
        for (int i = value.length() - 1; i >= 0; i--) {
            char digit = value.charAt(i);
            int digitValue;

            if (digit >= '0' && digit <= '9') {
                digitValue = digit - '0';
            } else if (digit >= 'a' && digit <= 'z') {
                digitValue = digit - 'a' + 10;
            } else if (digit >= 'A' && digit <= 'Z') {
                digitValue = digit - 'A' + 10;
            } else {
                throw new IllegalArgumentException("Invalid digit: " + digit);
            }

            if (digitValue >= base) {
                throw new IllegalArgumentException("Digit " + digit + " is invalid for base " + base);
            }

            result = result.add(BigInteger.valueOf(digitValue).multiply(power));
            power = power.multiply(baseValue);
        }

        return result;
    }

    // Lagrange interpolation to find the constant term (secret)
    public static BigInteger lagrangeInterpolation(List<Point> points) {
        BigInteger result = BigInteger.ZERO;
        int n = points.size();

        for (int i = 0; i < n; i++) {
            Point point_i = points.get(i);
            BigInteger xi = point_i.x;
            BigInteger yi = point_i.y;

            // Calculate Lagrange basis polynomial Li(0)
            BigInteger numerator = BigInteger.ONE;
            BigInteger denominator = BigInteger.ONE;

            for (int j = 0; j < n; j++) {
                if (i != j) {
                    Point point_j = points.get(j);
                    BigInteger xj = point_j.x;

                    // For Li(0): numerator *= (0 - xj) = -xj
                    numerator = numerator.multiply(xj.negate());
                    // denominator *= (xi - xj)
                    denominator = denominator.multiply(xi.subtract(xj));
                }
            }

            // Calculate yi * Li(0) = yi * (numerator / denominator)
            // Since we need exact division, we'll use BigInteger division
            BigInteger term = yi.multiply(numerator).divide(denominator);
            result = result.add(term);
        }

        return result;
    }

    public static void main(String[] args) {
        try {
            // Example with test case 1
            String jsonInput1 = """
            {
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
            }""";

            ObjectMapper mapper = new ObjectMapper();
            JsonNode root = mapper.readTree(jsonInput1);

            int n = root.get("keys").get("n").asInt();
            int k = root.get("keys").get("k").asInt();

            List<Point> points = new ArrayList<>();

            // Parse points from JSON
            Iterator<String> fieldNames = root.fieldNames();
            while (fieldNames.hasNext()) {
                String fieldName = fieldNames.next();
                if (!fieldName.equals("keys")) {
                    JsonNode pointNode = root.get(fieldName);
                    int x = Integer.parseInt(fieldName);
                    int base = pointNode.get("base").asInt();
                    String value = pointNode.get("value").asText();

                    BigInteger y = convertFromBase(value, base);
                    points.add(new Point(BigInteger.valueOf(x), y));
                }
            }

            // Sort points by x coordinate
            points.sort((p1, p2) -> p1.x.compareTo(p2.x));

            // Take first k points for reconstruction
            List<Point> selectedPoints = points.subList(0, k);

            // Find the secret using Lagrange interpolation
            BigInteger secret = lagrangeInterpolation(selectedPoints);

            System.out.println("Secret: " + secret);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
