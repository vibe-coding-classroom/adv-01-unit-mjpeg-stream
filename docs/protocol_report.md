# Protocol Analysis Lab: MJPEG over HTTP

## Task 1: HTTP Header Inspection

### Captured Response Headers
Capture the headers sent by the ESP32 server when accessing `/stream`.

```http
HTTP/1.1 200 OK
Content-Type: multipart/x-mixed-replace; boundary=...
...
```

### Analysis of the Boundary Parameter
Explain the role of the `boundary` string in the `Content-Type` header:
- [Your answer here]

## Task 2: MJPEG Packet Structure

### SOI and EOI Markers
Locate the Start of Image (SOI) and End of Image (EOI) markers in your captured data.

- **SOI (0xFFD8) position:** [Offset/Context]
- **EOI (0xFFD9) position:** [Offset/Context]

### Manual Header Verification
Paste the manual header implementation from your code and explain the importance of CRLF (`\r\n`).

```cpp
// Your code snippet here
```

## Task 3: Performance & Stability

### Observations
- **Resolution:** [VGA/QVGA]
- **Frame Rate:** [FPS]
- **Tearing/Artifacts:** [Describe any visual issues]

### Optimization Results
If you modified `CONFIG_TCP_MSS`, describe the impact:
- [Your answer here]
