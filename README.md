# 物件小專題：黑白探險

## 組別：第 8 組

## 小專題介紹：
地圖探險遊戲，玩家在黑白地圖上移動、撿道具、打怪、找鑰匙，最終逃出地圖，同時也會顯示玩家的痕跡。

## 成員資訊 / 分工：

| 學號 | 姓名 | 分工 |
|---|---|---|
| B2229264 | 莊子霆 | 組長 / 想法搜集、錯誤修正 |
| B1406804 | 翁嘉佑 | 組員 / UML 圖製作、程式碼撰寫 |
| B3208724 | 周子棋 | 組員 / error |

---

## 功能介紹

- 隨機生成地圖，每次玩都不同
- 只顯示玩家探索過的區域
- 撿道具：武器、藥水、鑰匙、寶藏
- 遭遇敵人時進入戰鬥，可選擇fight或running
- 需要找到鑰匙才能解鎖出口
- 獲勝：帶著鑰匙抵達出口 `E`

### 遊戲初始介面設計

| 符號 | 代表 |
|---|---|
| `@` | 玩家位置 |
| `#` | 牆壁 |
| `K` | 鑰匙 |
| `$` | 寶藏 |
| 空白 | 未探索區域 |

### 遊戲方式

| 按鍵 | 功能 |
|---|---|
| `w` `a` `s` `d` | 上下左右移動 |
| `i` | 查看背包 |
| `p` | 使用藥水 |
| `h` | 顯示說明 |
| `q` | 離開遊戲 |

---

## 程式介紹

專案共分成 5 個 .h/.cpp 檔案 + main.cpp。

### Item.h / Item.cpp

| 符號 | 種類 | 效果 |
|---|---|---|
| `W` | WEAPON 武器 | 提升攻擊力 |
| `P` | POTION 藥水 | 恢復 HP |
| `K` | KEY 鑰匙 | 解鎖出口 |
| `$` | TREASURE 寶藏 | 增加分數 |

### Entity.h / Entity.cpp

- `Entity`：hp、atk
- `Player` 繼承 Entity：inventory、score、level、hasKey
- `Enemy` 繼承 Entity：symbol、dropItem、aggressive

### Map.h / Map.cpp

- `Room`：type、items、enemy\*、visited、locked

| 符號 | 種類 | 說明 |
|---|---|---|
| `#` | WALL | 牆壁 |
| `S` | START | 出發點 |
| `E` / `X` | EXIT | 出口 |
| `.` | EMPTY | 空地 |
| `G` / `T` | ENEMY | 敵人|

- `Map`：width、height、grid、enemies

### Game.h / Game.cpp — 遊戲主控

- 屬性：map、player、state、turnCount
- GameState 三種狀態：
  - `RUNNING`：遊戲進行中
  - `WIN`：玩家帶著鑰匙抵達出口
  - `LOSE`：玩家 HP 歸零或主動離開

### main.cpp

---

## UML 圖

---
## 執行成果截圖
---
