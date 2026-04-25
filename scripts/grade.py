import re
import sys
import os

def grade():
    src_file = "src/main.cpp"
    if not os.path.exists(src_file):
        print(f"Error: {src_file} not found.")
        return False

    with open(src_file, "r") as f:
        content = f.read()

    score = 0
    total = 3

    # Check 1: Use of multipart/x-mixed-replace
    if "multipart/x-mixed-replace" in content:
        print("[PASS] Found MJPEG Content-Type.")
        score += 1
    else:
        print("[FAIL] Missing MJPEG Content-Type (multipart/x-mixed-replace).")

    # Check 2: CRLF usage (\r\n)
    # We look for client.print with \r\n or client.printf with \r\n
    crlf_count = content.count("\\r\\n")
    if crlf_count >= 5: # Expecting several CRLF in headers
        print(f"[PASS] Found {crlf_count} CRLF sequences.")
        score += 1
    else:
        print(f"[FAIL] Only found {crlf_count} CRLF sequences. HTTP headers require CRLF (\\r\\n).")

    # Check 3: Manual boundary and image header construction
    # Look for the pattern of sending boundary then image header
    if "--" in content and "image/jpeg" in content and "Content-Length" in content:
        print("[PASS] Manual MJPEG header construction detected.")
        score += 1
    else:
        print("[FAIL] Could not verify manual MJPEG header construction.")

    print(f"\nFinal Score: {score}/{total}")
    return score == total

if __name__ == "__main__":
    if grade():
        sys.exit(0)
    else:
        sys.exit(1)
