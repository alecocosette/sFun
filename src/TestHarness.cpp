//
// TestHarness.cpp - Simple CPU test harness for manual JSON tests
//

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "../include/CPU.h"
#include "../include/Memory.h"
#include "../include/Bus.h"

using json = nlohmann::json;

struct TestResult {
    bool passed;
    std::string name;
    std::string failureReason;
};

// Helper: safely get a value from JSON, return default if not present
template<typename T>
T getOrDefault(const json& obj, const std::string& key, T defaultVal) {
    if (obj.contains(key)) {
        return obj[key].get<T>();
    }
    return defaultVal;
}

void printMismatch(const std::string& regName, uint32_t expected, uint32_t actual) {
    std::cout << "    " << regName << ": expected 0x"
              << std::hex << std::setw(4) << std::setfill('0') << expected
              << ", got 0x" << std::setw(4) << std::setfill('0') << actual
              << std::dec << std::endl;
}

TestResult runTest(const json& testCase) {
    TestResult result;
    result.name = testCase["name"].get<std::string>();
    result.passed = true;
    result.failureReason = "";

    std::cout << "Testing: " << result.name << std::flush;

    // Create fresh Bus and CPU
    Bus bus;
    CPU cpu(&bus);


    // Parse initial state
    const auto& initial = testCase["initial"];

    uint16_t pc = getOrDefault(initial, "pc", 0);
    uint32_t a = getOrDefault(initial, "a", 0U);
    uint32_t x = getOrDefault(initial, "x", 0U);
    uint32_t y = getOrDefault(initial, "y", 0U);
    uint16_t sp = getOrDefault(initial, "sp", 0x01FFU);
    uint8_t p = getOrDefault(initial, "p", 0x34U);
    uint8_t db = getOrDefault(initial, "db", 0x7EU);
    uint8_t pb = getOrDefault(initial, "pb", 0x7EU);
    uint16_t dp = getOrDefault(initial, "dp", 0U);


    // Set CPU state
    cpu.PC = pc;
    cpu.A = a;
    cpu.X = x;
    cpu.Y = y;
    cpu.SP = sp;
    cpu.P = p;
    cpu.DB = db;
    cpu.PB = pb;
    cpu.DP = dp;

    std::string opcodeStr = testCase["opcode"].get<std::string>();
    uint8_t opcode = std::stoi(opcodeStr, nullptr, 16);

    uint32_t addr = (pb << 16) | pc;
    bus.write8(addr, opcode);

    // Write operands if present
    if (testCase.contains("operands")) {
        std::string operands = testCase["operands"].get<std::string>();
        if (!operands.empty()) {
            uint32_t operandAddr = addr + 1;
            for (size_t i = 0; i < operands.length(); i += 2) {
                std::string byteStr = operands.substr(i, 2);
                uint8_t operandByte = std::stoi(byteStr, nullptr, 16);
                bus.write8(operandAddr, operandByte);

                // DEBUG: verify operand was written
                uint8_t verify = bus.read8(operandAddr);
                std::cout << " [wrote operand 0x" << std::hex << (int)operandByte
                          << ", verify read 0x" << (int)verify << std::dec << "]";

                operandAddr++;
            }
        }
    }
    std::cout << std::flush;


    // Execute
    cpu.step();

    // Parse expected state
    const auto& expected = testCase["expected"];

    // Compare each field if it exists in expected
    if (expected.contains("pc")) {
        uint16_t expPC = expected["pc"].get<uint16_t>();
        if (cpu.PC != expPC) {
            result.passed = false;
            printMismatch("PC", expPC, cpu.PC);
        }
    }

    if (expected.contains("a")) {
        uint32_t expA = expected["a"].get<uint32_t>();
        if (cpu.A != expA) {
            result.passed = false;
            printMismatch("A", expA, cpu.A);
        }
    }

    if (expected.contains("x")) {
        uint32_t expX = expected["x"].get<uint32_t>();
        if (cpu.X != expX) {
            result.passed = false;
            printMismatch("X", expX, cpu.X);
        }
    }

    if (expected.contains("y")) {
        uint32_t expY = expected["y"].get<uint32_t>();
        if (cpu.Y != expY) {
            result.passed = false;
            printMismatch("Y", expY, cpu.Y);
        }
    }

    if (expected.contains("sp")) {
        uint16_t expSP = expected["sp"].get<uint16_t>();
        if (cpu.SP != expSP) {
            result.passed = false;
            printMismatch("SP", expSP, cpu.SP);
        }
    }

    if (expected.contains("p")) {
        uint8_t expP = expected["p"].get<uint8_t>();
        if (cpu.P != expP) {
            result.passed = false;
            std::cout << "    P: expected 0x"
                      << std::hex << std::setw(2) << std::setfill('0') << (int)expP
                      << ", got 0x" << std::setw(2) << std::setfill('0') << (int)cpu.P
                      << std::dec << std::endl;
        }
    }

    if (expected.contains("db")) {
        uint8_t expDB = expected["db"].get<uint8_t>();
        if (cpu.DB != expDB) {
            result.passed = false;
            printMismatch("DB", expDB, cpu.DB);
        }
    }

    if (expected.contains("pb")) {
        uint8_t expPB = expected["pb"].get<uint8_t>();
        if (cpu.PB != expPB) {
            result.passed = false;
            printMismatch("PB", expPB, cpu.PB);
        }
    }

    if (expected.contains("dp")) {
        uint16_t expDP = expected["dp"].get<uint16_t>();
        if (cpu.DP != expDP) {
            result.passed = false;
            printMismatch("DP", expDP, cpu.DP);
        }
    }

    return result;
}

int runTestSuite(const std::string& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open test file: " << jsonPath << std::endl;
        return 1;
    }

    json tests;
    try {
        file >> tests;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to parse JSON: " << e.what() << std::endl;
        return 1;
    }

    if (!tests.is_array()) {
        std::cerr << "ERROR: Test file root is not an array" << std::endl;
        return 1;
    }

    std::cout << "Running " << tests.size() << " CPU tests...\n" << std::endl;

    int passed = 0;
    int failed = 0;

    for (size_t i = 0; i < tests.size(); i++) {
        TestResult result = runTest(tests[i]);

        if (result.passed) {
            passed++;
            std::cout << "[PASS] " << result.name << std::endl;
        } else {
            failed++;
            std::cout << "[FAIL] " << result.name << std::endl;
        }
    }

    std::cout << "\n========================================" << std::endl;
    std::cout << "Results: " << passed << " passed, " << failed << " failed" << std::endl;
    if (tests.size() > 0) {
        std::cout << "Pass rate: " << (100.0 * passed / tests.size()) << "%" << std::endl;
    }
    std::cout << "========================================" << std::endl;

    return failed == 0 ? 0 : 1;
}