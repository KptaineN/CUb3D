#!/bin/bash

# Colors for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
total_tests=0
passed_tests=0
failed_tests=0

# Results storage
declare -a failed_valid_maps
declare -a failed_invalid_maps
declare -a passed_valid_maps
declare -a passed_invalid_maps

# Function to print header
print_header() {
    echo -e "${BLUE}===============================================${NC}"
    echo -e "${BLUE}         CUB3D MAP TESTING SCRIPT${NC}"
    echo -e "${BLUE}===============================================${NC}"
    echo ""
}

# Function to test a single map
test_map() {
    local map_file="$1"
    local expected_result="$2"  # "valid" or "invalid"
    local map_name=$(basename "$map_file")
    
    # Test the map with timeout to prevent hanging
    timeout 5s ./cub3d "$map_file" >/dev/null 2>&1
    local exit_code=$?
    
    # Handle timeout case
    if [ $exit_code -eq 124 ]; then
        echo -e "${YELLOW}TIMEOUT${NC} - $map_name (took too long)"
        return 1
    fi
    
    total_tests=$((total_tests + 1))
    
    # Determine if test passed based on exit code
    # Exit code 0 = valid, non-zero = invalid
    if [ $exit_code -eq 0 ]; then
        actual_result="valid"
    else
        actual_result="invalid"
    fi
    
    # Check if result matches expectation
    if [ "$actual_result" = "$expected_result" ]; then
        echo -e "${GREEN}PASS${NC} - $map_name (expected: $expected_result, got: $actual_result)"
        passed_tests=$((passed_tests + 1))
        
        if [ "$expected_result" = "valid" ]; then
            passed_valid_maps+=("$map_name")
        else
            passed_invalid_maps+=("$map_name")
        fi
        
        return 0
    else
        echo -e "${RED}FAIL${NC} - $map_name (expected: $expected_result, got: $actual_result, exit_code: $exit_code)"
        failed_tests=$((failed_tests + 1))
        
        if [ "$expected_result" = "valid" ]; then
            failed_valid_maps+=("$map_name")
        else
            failed_invalid_maps+=("$map_name")
        fi
        
        return 1
    fi
}

# Function to test maps in a directory
test_directory() {
    local dir="$1"
    local expected_result="$2"
    local dir_name=$(basename "$dir")
    
    echo -e "${BLUE}Testing $dir_name maps...${NC}"
    echo "----------------------------------------"
    
    if [ ! -d "$dir" ]; then
        echo -e "${RED}Directory $dir does not exist!${NC}"
        return 1
    fi
    
    local count=0
    for map_file in "$dir"/*.cub; do
        if [ -f "$map_file" ]; then
            test_map "$map_file" "$expected_result"
            count=$((count + 1))
        fi
    done
    
    if [ $count -eq 0 ]; then
        echo -e "${YELLOW}No .cub files found in $dir${NC}"
    fi
    
    echo ""
}

# Function to print summary
print_summary() {
    echo -e "${BLUE}===============================================${NC}"
    echo -e "${BLUE}                 SUMMARY${NC}"
    echo -e "${BLUE}===============================================${NC}"
    echo -e "Total tests: ${BLUE}$total_tests${NC}"
    echo -e "Passed:      ${GREEN}$passed_tests${NC}"
    echo -e "Failed:      ${RED}$failed_tests${NC}"
    
    if [ $total_tests -gt 0 ]; then
        local success_rate=$(echo "scale=2; $passed_tests * 100 / $total_tests" | bc -l)
        echo -e "Success rate: ${BLUE}$success_rate%${NC}"
    fi
    
    echo ""
    
    # Show failed tests details
    if [ ${#failed_valid_maps[@]} -gt 0 ]; then
        echo -e "${RED}Failed VALID maps (should have passed):${NC}"
        printf '%s\n' "${failed_valid_maps[@]}" | sed 's/^/  - /'
        echo ""
    fi
    
    if [ ${#failed_invalid_maps[@]} -gt 0 ]; then
        echo -e "${RED}Failed INVALID maps (should have been rejected):${NC}"
        printf '%s\n' "${failed_invalid_maps[@]}" | sed 's/^/  - /'
        echo ""
    fi
    
    # Show passed tests summary
    echo -e "${GREEN}Summary of passed tests:${NC}"
    echo -e "  Valid maps that passed: ${GREEN}${#passed_valid_maps[@]}${NC}"
    echo -e "  Invalid maps that failed correctly: ${GREEN}${#passed_invalid_maps[@]}${NC}"
}

# Function to generate detailed report
generate_report() {
    local report_file="test_results_$(date +%Y%m%d_%H%M%S).txt"
    
    {
        echo "CUB3D MAP TESTING REPORT"
        echo "Generated on: $(date)"
        echo "========================================"
        echo ""
        echo "SUMMARY:"
        echo "--------"
        echo "Total tests: $total_tests"
        echo "Passed: $passed_tests"
        echo "Failed: $failed_tests"
        
        if [ $total_tests -gt 0 ]; then
            local success_rate=$(echo "scale=2; $passed_tests * 100 / $total_tests" | bc -l)
            echo "Success rate: $success_rate%"
        fi
        
        echo ""
        echo "FAILED TESTS:"
        echo "-------------"
        
        if [ ${#failed_valid_maps[@]} -gt 0 ]; then
            echo "Valid maps that failed (false negatives):"
            printf '%s\n' "${failed_valid_maps[@]}" | sed 's/^/  - /'
            echo ""
        fi
        
        if [ ${#failed_invalid_maps[@]} -gt 0 ]; then
            echo "Invalid maps that passed (false positives):"
            printf '%s\n' "${failed_invalid_maps[@]}" | sed 's/^/  - /'
            echo ""
        fi
        
        echo "PASSED TESTS:"
        echo "-------------"
        echo "Valid maps that passed: ${#passed_valid_maps[@]}"
        echo "Invalid maps that failed correctly: ${#passed_invalid_maps[@]}"
        
    } > "$report_file"
    
    echo -e "${BLUE}Detailed report saved to: $report_file${NC}"
}

# Main execution
main() {
    print_header
    
    # Check if cub3d executable exists
    if [ ! -f "./cub3d" ]; then
        echo -e "${RED}Error: cub3d executable not found!${NC}"
        echo "Please compile the project first with 'make'"
        exit 1
    fi
    
    # Check if bc is available for calculations
    if ! command -v bc &> /dev/null; then
        echo -e "${YELLOW}Warning: bc not found. Success rate calculation will be skipped.${NC}"
    fi
    
    # Test valid maps
    test_directory "map/valid" "valid"
    
    # Test invalid maps  
    test_directory "map/invalid" "invalid"
    
    # Print summary
    print_summary
    
    # Generate detailed report
    generate_report
    
    # Exit with appropriate code
    if [ $failed_tests -eq 0 ]; then
        echo -e "${GREEN}All tests passed! 🎉${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed! 😞${NC}"
        exit 1
    fi
}

# Run main function
main "$@"