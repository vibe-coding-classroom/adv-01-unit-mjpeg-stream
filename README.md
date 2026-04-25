針對 **`adv-01-unit-mjpeg-stream` (MJPEG 編碼與 HTTP Boundary 原理)** 單元，這是一個讓學生從「調包俠 (Library User)」進化為「協議理解者 (Protocol Implementer)」的關鍵進階課程。

以下是在 **GitHub Classroom** 部署其作業 (Assignments) 的具體建議：

### 1. 範本倉庫 (Template Repo) 配置建議
進階協議單元的範本應強調「零依賴 (Dependency-free)」的實作，建議包含：
*   **📂 `src/mjpeg_server.cpp`**：故意移除高階影像串流庫，僅保留原始的 `WiFiClient` 物件。學員需在此實作手動拼接 HTTP 多部份 (Multipart) 數據包的邏輯。
*   **📂 `analysis/packet_dump.md`**：預置一個報告模板，讓學員貼入瀏覽器開發者工具 (F12) 擷取的 HTTP 標頭 (Header) 分析。
*   **📂 `web/stream_test.html`**：一個極簡的前端頁面，用於驗證學員實作的串流是否能被標準 `<img>` 標籤解析。
*   **📂 `.github/workflows/protocol_checker.yml`**：自動化腳本，初步檢查原始碼中是否包含正確的 `\r\n` (CRLF) 換行邏輯。

---

### 2. 作業任務部署細節

#### 任務 1：封包解剖師 (Packet Analysis Lab)
*   **目標**：透過觀察真實流量，理解 MJPEG 在 HTTP 層級的運作機制。
*   **Classroom 部署建議**：
    *   **要求**：學員需擷取 `Content-Type: multipart/x-mixed-replace; boundary=...` 這一行，並解釋 `boundary` 的具體作用。
    *   **驗證方式**：在 `analysis/packet_dump.md` 中檢查學員是否能正確計算出單張 JPEG 的 SOI (`0xFFD8`) 與 EOI (`0xFFD9`) 在二進位流中的位置。

#### 任務 2：手動拼接 MJPEG 數據流 (Manual Streamer Implementation)
*   **目標**：不依賴第三方庫，手動透過 TCP Socket 推送影像流。
*   **Classroom 部署建議**：
    *   **核心代碼檢核**：
        ```cpp
        // 學生需實作：手動傳送邊界與長度標頭
        client.print("--" + boundary + "\r\n");
        client.print("Content-Type: image/jpeg\r\n");
        client.print("Content-Length: " + String(fb->len) + "\r\n\r\n");
        client.write(fb->buf, fb->len);
        client.print("\r\n");
        ```
    *   **核核重點**：導師應在 PR 中檢查學員是否正確使用了 **CRLF (`\r\n`)**。在 HTTP 協議中，少一個位元組都會導致瀏覽器渲染失敗（出現破碎畫面或綠屏）。

#### 任務 3：串流穩定性與 MTU 優化 (Stream Stability Lab)
*   **目標**：解決高解析度下的影像撕裂 (Tearing) 問題。
*   **Classroom 部署建議**：
    *   **數據採集**：要求學員紀錄在 VGA 解析度下，調整 `CONFIG_TCP_MSS` (最大報文段大小) 前後，影像傳輸的流暢度差異。
    *   **成果展示**：上傳一段錄影或截圖，對比「優化前（影像腰斬/撕裂）」與「優化後（完整幀平滑切換）」的視覺效果。

---

### 3. 進階協議單元導師點評標準 (Advanced Benchmarks)
此單元的價值在於 **「對數位邊界的精確控制」**：
*   [ ] **協議嚴謹度**：手動封裝的 HTTP 流是否完全符合規範，且不依賴任何外部渲染輔助？
*   [ ] **故障排除能力**：當瀏覽器無法連線時，學員是否能透過 F12 工具診斷出是 Boundary 名稱不匹配還是換行符出錯？
*   [ ] **性能直覺**：學員是否理解為什麼在頻寬受限時，MJPEG 的表現會優於/劣於其他編碼格式？

### 📁 推薦範本結構 (GitHub Classroom Template)：
```text
.
├── src/
│   └── mjpeg_driver.cpp    # 核心：學員需在此實作位元組級別的發送邏輯
├── docs/
│   └── protocol_report.md  # 協議解剖報告 (含 Header 截圖)
├── test/
│   └── stability_test.csv  # 穩定性測試數據 (解析度 vs. 撕裂次數)
└── README.md               # 專案總結：從 Socket 層面看影像連線
```

透過這種部署方式，學生能從「會寫程式碼」提升到「**理解數據流動規律**」的高度，這是成為資深物聯網或影音工程師的必經之路。
